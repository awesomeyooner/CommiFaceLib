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


StatusCode SerialInterface::write_to_register(uint8_t reg, const vector<uint8_t>& data)
{
    vector<uint8_t> write_data = create_packet(reg, data);    

    return transmit_bytes(write_data);

} // end of "write_to_register(uint8_t, const std::vector<uint8_t>&)"


StatusCode SerialInterface::write_int(uint8_t reg, int data)
{
    vector<uint8_t> bytes = ByteConverter::int_to_bytes(data);

    return write_to_register(reg, bytes);

} // end of "write_int(uint8_t, int)"


StatusCode SerialInterface::write_float(uint8_t reg, float data)
{
    vector<uint8_t> bytes = ByteConverter::float_to_bytes(data);

    return write_to_register(reg, bytes);

} // end of "write_float(uint8_t, float)"


StatusCode SerialInterface::write_double(uint8_t reg, double data)
{
    vector<uint8_t> bytes = ByteConverter::double_to_bytes(data);

    return write_to_register(reg, bytes);

} // end of "write_double(uint8_t, double)"


StatusedValue<vector<uint8_t>> SerialInterface::receive_bytes(int num_bytes, int timeout_ms)
{
    // If the user did not specify a timeout then just use the member
    if(timeout_ms == -1)
        timeout_ms = m_timeout_ms;

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
        timeout_ms = m_timeout_ms;

    double delay_time = block_until((double)timeout_ms / 1000);

    string read_data;

    m_serial_port.ReadLine(read_data, '\n', timeout_ms);
    m_serial_port.FlushIOBuffers();

    vector<uint8_t> bytes = ByteConverter::string_to_bytes(read_data);
    StatusCode status = bytes.size() != 0 ? StatusCode::OK : StatusCode::FAILED;

    return StatusedValue<vector<uint8_t>>(bytes, status);

} // end of "receive_bytes(char, int)"


StatusedValue<vector<uint8_t>> SerialInterface::request(uint8_t reg, int num_bytes, int timeout_ms)
{
    // Write to the device asking to send data
    StatusCode write_status = write_to_register(reg);

    // If the write itself failed then the receive will fail, so end early
    if(write_status != StatusCode::OK)
        return StatusedValue<vector<uint8_t>>({}, StatusCode::FAILED);
    
    return receive_bytes(num_bytes, timeout_ms);

} // end of "request(uint8_t, int, int)"


StatusedValue<int> SerialInterface::request_int(uint8_t reg, int timeout_ms)
{
    auto request_status = request(reg, sizeof(int), timeout_ms);

    int data = ByteConverter::bytes_to_int(request_status.value);
    StatusCode status = request_status.status;

    return StatusedValue<int>(data, status);

} // end of "request_int(uint8_t, int)"


StatusedValue<float> SerialInterface::request_float(uint8_t reg, int timeout_ms)
{
    auto request_status = request(reg, sizeof(float), timeout_ms);

    float data = ByteConverter::bytes_to_float(request_status.value);
    StatusCode status = request_status.status;

    return StatusedValue<float>(data, status);

} // end of "request_int(uint8_t, float)"


StatusedValue<double> SerialInterface::request_double(uint8_t reg, int timeout_ms)
{
    auto request_status = request(reg, sizeof(double), timeout_ms);

    double data = ByteConverter::bytes_to_double(request_status.value);
    StatusCode status = request_status.status;

    return StatusedValue<double>(data, status);

} // end of "request_int(uint8_t, int)"


vector<uint8_t> SerialInterface::create_packet(uint8_t reg, const vector<uint8_t>& data)
{
    // 0: register
    // 1: length
    // 2... Data

    uint8_t length = data.size() + 2; // 1 for register, 1 for length

    std::vector<uint8_t> write_data;
    write_data.resize(length);

    // Put `data` in the end of the vector
    std::copy(data.begin(), data.end(), write_data.begin() + 2);

    write_data.at(0) = reg;
    write_data.at(1) = length;

    return write_data;

} // end of "create_packet(uint8_t, const vector<uint8_t>&)"


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