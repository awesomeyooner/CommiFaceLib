#include "CommiFaceLib/protocols/serial.hpp"


namespace fs = std::filesystem;
using namespace std;
using namespace status_utils;


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
            Logger::info("Failed to open device at: " + path);

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
        Logger::info("Failed to find proper device!");

    return StatusCode::FAILED;
     
} // end of "init_field(std::string, std::string, bool)"


StatusCode SerialInterface::init_name(string name, bool verbose)
{
    return init(DEV_DEVICES_PATH + "/" + name, verbose);

} // end of "init_name(std::string, bool)"


StatusCode SerialInterface::transmit_bytes(const vector<uint8_t>& bytes)
{
    m_serial_port.Write(bytes);

    return StatusCode::OK;

} // end of "transmit_bytes(const std::vector<uint8_t>&)"


StatusedValue<vector<uint8_t>> SerialInterface::receive_bytes(int num_bytes, int timeout_ms)
{
    // If there's no data to read
    // Then return FAILED
    if(!m_serial_port.IsDataAvailable())
        return StatusedValue<vector<uint8_t>>({}, StatusCode::FAILED);

    // If the user did not specify a timeout then just use the member
    if(timeout_ms == -1)
        timeout_ms = m_timeout_ms;

    string read_data;

    m_serial_port.Read(read_data, num_bytes, timeout_ms);

    
}