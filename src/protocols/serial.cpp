#include "CommiFaceLib/protocols/serial.hpp"


namespace fs = std::filesystem;
using namespace std;
using namespace status_utils;


LibSerial::SerialPort& SerialInterface::get_port()
{
    return m_serial_port;

} // end of "get_port()"


StatusCode SerialInterface::init(string path, bool verbose)
{
    if(verbose)
        Logger::info("Opening device at: " + path);

    try
    {
        m_serial_port.Open(path);
    }
    catch(const exception& e)
    {
        if(verbose)
        {   
            Logger::error("Failed to open device at: " + path);
            Logger::error("Error Caught: " + string(e.what()));
        }

        return StatusCode::FAILED;
    }
    
    return StatusCode::OK;

} // end of "init(string)"


StatusCode SerialInterface::init_field(string field_name, string desired, bool verbose)
{
    fs::path devices_path = SYS_DEVICES_PATH;

    // For every device
    // Check the given field if it's equal to the desired value
    for(auto entry : fs::directory_iterator(devices_path))
    {
        string device_name = entry.path().filename().string();
        fs::path field_path = entry.path() / FIELD_PATH / field_name;
    
        // If it's not a normal file then skip
        if(!fs::is_regular_file(field_path))
            continue;

        ifstream file(field_path);

        if(!file.is_open())
            continue;

        if(verbose)
            Logger::info("Found potential device: " + device_name);

        string field;

        // Extract the contents of the file
        getline(file, field);

        if(field.empty())
            continue;

        if(field == desired)
        {
            if(verbose)
                Logger::info("Correct device at: " + device_name);

            return init_name(device_name, verbose);
        }
        else if(verbose)
            Logger::info("Not the right device. Looking for another...");
    }

    if(verbose)
        Logger::error("Failed to find proper device!");

    return StatusCode::FAILED;
     
} // end of "init_field(std::string, std::string, bool)"


StatusCode SerialInterface::init_name(string name, bool verbose)
{
    return init(DEV_DEVICES_PATH + "/" + name, verbose);

} // end of "init_name(std::string, bool)"


StatusCode SerialInterface::close()
{
    m_serial_port.Close();

    return StatusCode::OK;

} // end of "close()"


StatusCode SerialInterface::transmit_bytes(const vector<uint8_t>& bytes)
{
    m_serial_port.Write(bytes);

    return StatusCode::OK;

} // end of "transmit_bytes(const std::vector<uint8_t>&)"


StatusedValue<vector<uint8_t>> SerialInterface::receive_bytes(int num_bytes, int timeout_ms)
{
    // If the user did not specify a timeout then just use the member
    if(timeout_ms == -1)
        timeout_ms = get_timeout_ms();

    double delay_time = block_until((double)timeout_ms / 1000.0);

    string read_data;

    m_serial_port.Read(read_data, num_bytes, timeout_ms);
    m_serial_port.FlushIOBuffers();

    vector<uint8_t> bytes = ByteConverter::string_to_bytes(read_data);
    StatusCode status = bytes.size() == num_bytes ? StatusCode::OK : StatusCode::FAILED;

    return StatusedValue<vector<uint8_t>>(bytes, status);

} // end of "receive_bytes(int, int)"


StatusedValue<vector<uint8_t>> SerialInterface::receive_bytes(char delimiter, int timeout_ms)
{
    // If the user did not specify a timeout then just use the member
    if(timeout_ms == -1)
        timeout_ms = get_timeout_ms();

    double delay_time = block_until((double)timeout_ms / 1000);

    string read_data;

    m_serial_port.ReadLine(read_data, delimiter, timeout_ms);
    m_serial_port.FlushIOBuffers();

    vector<uint8_t> bytes = ByteConverter::string_to_bytes(read_data);
    StatusCode status = bytes.size() != 0 ? StatusCode::OK : StatusCode::FAILED;

    return StatusedValue<vector<uint8_t>>(bytes, status);

} // end of "receive_bytes(char, int)"


double SerialInterface::block_until(double timeout_seconds)
{
    double start = System::get_epoch();

    // Keep blocking until this is true
    while(!m_serial_port.IsDataAvailable())
    {
        double time = System::get_epoch();

        // If the time difference exceeds the timeout
        // Then break the loop
        if(time - start > timeout_seconds)
            break;
    }

    double end = System::get_epoch();

    return end - start;

} // end of "block_until(function<bool()>, double)"