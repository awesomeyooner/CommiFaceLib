#include "CommiFaceLib/protocols/can.hpp"


using namespace status_utils;
using namespace std;


StatusCode CAN::open(string interface_name, bool verbose)
{
    // Create the initial socket
    m_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    // Log errors
    if(verbose && m_socket < 0)
    {
        Logger::error("socket()");
        return StatusCode::ERROR;
    }

    // Set the Interface Request name
    strcpy(m_ifr.ifr_name, interface_name.c_str());

    // Get the interface index
    if(ioctl(m_socket, SIOCGIFINDEX, &m_ifr) < 0)
    {
        Logger::error("ioctl()");
        return StatusCode::ERROR;
    }

    // Configure CAN
    m_address.can_family = AF_CAN;
    m_address.can_ifindex = m_ifr.ifr_ifindex;

    if(bind(m_socket, (struct sockaddr*)&m_address, sizeof(m_address)) < 0)
    {
        Logger::error("bind()");
        return StatusCode::OK;
    }

    if(verbose)
        Logger::info("Successfully opened CAN at: " + interface_name);

    return StatusCode::OK;

} // end of "open(string = "can0", bool = true)"


StatusCode CAN::close_socket()
{
    if(close(m_socket) < 0)
    {
        Logger::error("close()");
        return StatusCode::FAILED;
    }

    return StatusCode::OK;

} // end of "close_socket()"


CAN::CAN(int ID)
{
    m_transmit_frame.can_id = ID;

} // end of "CAN(int)"


StatusCode CAN::transmit_bytes(const vector<uint8_t>& bytes)
{
    if(bytes.size() > MAX_DATA_SIZE)
        return StatusCode::ERROR;

    // Populate transmit frame data
    copy(bytes.begin(), bytes.end(), m_transmit_frame.data);

    // Assign transmit frame size (bytes)
    m_transmit_frame.can_dlc = bytes.size();

    // write() should return the number of bytes it writes if successful
    return write(m_socket, &m_transmit_frame, sizeof(m_transmit_frame)) == sizeof(m_transmit_frame) ? 
        StatusCode::OK : StatusCode::FAILED;

} // end of "transmit(bytes(const vector<uint8_t>& bytes))"


StatusedValue<vector<uint8_t>> CAN::receive_bytes(int num_bytes, int timeout_ms)
{
    int read_bytes = read(m_socket, &m_receive_frame, sizeof(m_receive_frame));

    if(m_receive_frame.can_dlc != num_bytes)
        return StatusedValue<vector<uint8_t>>({}, StatusCode::FAILED);

    // Array -> Vector
    std::vector<uint8_t> bytes(begin(m_receive_frame.data), end(m_receive_frame.data));

    return StatusedValue<vector<uint8_t>>(bytes, StatusCode::OK);

} // end of "receive_bytes(int, int)"