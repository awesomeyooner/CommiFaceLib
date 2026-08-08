#include "CommiFaceLib/interfaces/communication_interface.hpp"


using namespace status_utils;
using namespace std;


int CommunicationInterface::get_timeout_ms()
{
    return m_timeout_ms;

} // end of "get_timeout_ms()"


void CommunicationInterface::set_timeout_ms(int timeout_ms)
{
    m_timeout_ms = fabs(timeout_ms);

} // end of "set_timeout_ms(int)"


vector<uint8_t> CommunicationInterface::create_packet(uint8_t reg, const vector<uint8_t>& data)
{
    // 0: register
    // 1: length
    // 2... Data

    uint8_t length = data.size() + 2; // 1 for register, 1 for length

    std::vector<uint8_t> packet;
    packet.resize(length);

    // Put `data` in the end of the vector
    std::copy(data.begin(), data.end(), packet.begin() + 2);

    packet.at(0) = reg;
    packet.at(1) = length;

    return packet;

} // end of "create_packet(uint8_t, const vector<uint8_t>&)"


StatusCode CommunicationInterface::write_packet(uint8_t reg, const vector<uint8_t>& data, bool acknowledge, int timeout_ms)
{
    // Use default timeout if param wasn't set
    if(timeout_ms == -1)
        timeout_ms = m_timeout_ms;

    vector<uint8_t> packet = create_packet(reg, data);

    StatusCode transmit_status = transmit_bytes(packet);

    // If not using acknowledge then just end here
    if(!acknowledge)
        return transmit_status;

    // If transmit failed then don't check for acknowledgement
    if(transmit_status != StatusCode::OK)
        return StatusCode::FAILED;

    // Only read `data`, not the entire packet
    StatusedValue<vector<uint8_t>> read_status = receive_bytes(data.size(), timeout_ms);

    // If the read fails then end early
    if(!read_status.is_OK())
        return StatusCode::FAILED;

    // Check if the wanted bytes are equal to the read bytes
    bool are_bytes_same = data == read_status.value;

    // Raise ERROR if not the same
    return are_bytes_same ? StatusCode::OK : StatusCode::ERROR;
    
} // end of "write_packet(uint8_t, const vector<uint8_t>&, bool, int)"


StatusedValue<vector<uint8_t>> CommunicationInterface::request(uint8_t reg, int num_bytes, int timeout_ms)
{
    StatusCode write_status = write_packet(reg);

    if(write_status != StatusCode::OK)
        return StatusedValue<vector<uint8_t>>({}, StatusCode::FAILED);

    return receive_bytes(num_bytes, timeout_ms);

} // end of "request(uint8_t, int, int = -1)"