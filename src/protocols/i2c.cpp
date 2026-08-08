#include "CommiFaceLib/protocols/i2c.hpp"


using namespace std;
using namespace status_utils;


I2C::I2C(int address, int bytes_per_page, int timeout_ms)
{
    // If the timeout param was set
    // Then set the internal timeout equal to it
    if(timeout_ms != -1)
        set_timeout_ms(timeout_ms);

    // Initialize the internal device object with the following settings
    m_device.addr = address;
    m_device.bus = get_bus();
    m_device.page_bytes = bytes_per_page; // 8 bytes max for MCP2221A
    m_device.iaddr_bytes = 0;
    m_device.tenbit = 0;
    m_device.flags = 0;
    m_device.delay = get_timeout_ms(); // milliseconds

} // end of "I2C(int, int, int)"


StatusCode I2C::init(const char* name)
{
    // Open the i2c m_bus and store the m_bus number in the variable
    m_bus = i2c_open(name);

    // if the m_bus is -1 then return as FAILED, if not, return OK
    if(m_bus == -1)
        return StatusCode::FAILED;
    else
        return StatusCode::OK;

} // end of "init"


StatusCode I2C::init(int adapter_number)
{
    string name = "/dev/i2c-" + to_string(adapter_number);

    return init(name.c_str());
}


StatusCode I2C::init_name(string name, bool verbose)
{
    // For every folder within the main path, check the names match
    // the given name. If it does, use that adapter and return OK
    for(auto entry : filesystem::directory_iterator(SYS_DEVICE_PATH))
    {   
        // The the i2c device folder name "i2c-<num>"
        string directory_name = entry.path().filename().string();

        // Look for the prefix, if it exists then it's an i2c device
        // If the prefix doesn't exist
        // Then skip this entry
        if(directory_name.find(I2C_DEVICE_PREFIX) == string::npos)
        {
            // Found folder but it's not an i2c device, skip
            if(verbose)
                Logger::info("Found Folder: " + directory_name + "... " + "Not an i2c Device! Skipping...");

            continue;
        }

        // Store the adapter number in this variable
        int adapter_number;
        
        // Try to extract the number from the device name
        try
        {
            // substr(4) gets the string AFTER "i2c-"
            adapter_number = stoi(directory_name.substr(I2C_DEVICE_PREFIX.size()));
        }
        // If there's an error, tell the user that it's not a proper adapter
        catch(exception& e)
        {

            if(verbose)
                Logger::info("Found Folder: " + directory_name + "... " + "Not a proper adapter! Skipping...");

            continue;
        }

        // The file containing the name is `name`
        auto name_file = entry.path() / DEVICE_NAME_FILE;

        // If it's not a regular file then skip
        if(!filesystem::is_regular_file(name_file))
            continue;

        // Open the file
        ifstream file(name_file);

        // If it didn't properly open then skip
        if(!file.is_open())
            continue;

        // Store the adapter name in this variable
        string adapter_name;

        // Grab the first line of the file and store it in `adapter_name`
        getline(file, adapter_name);

        // If the file is empty / if the adapter name is nothing, skip
        if(adapter_name.empty())
            continue;

        // If the name matches the one specified, initialize with that adapter number
        if(adapter_name.find(name) != string::npos)
        {
            // Tell the user that the adapter has been found
            if(verbose)
                Logger::info("Found adapter " + adapter_name + " in: " + directory_name);

            return init(adapter_number);
        }
        else
        {
            // If it's not the right one then just skip
            if(verbose)
                Logger::info("Not the right adapter! Skipping...");
        }
    }

    // If it ever gets here, that means that the adapter has not been found throughout
    // the entire folder, so it FAILED
    return StatusCode::FAILED;

} //  end of "init_name"


int I2C::get_bus()
{
    return m_bus;

} // end of "get_bus()"


i2c_device& I2C::get_device()
{
    return m_device;

} // end of "get_device()"


StatusCode I2C::transmit_bytes(const vector<uint8_t>& bytes)
{
    StatusCode status = i2c_write(&m_device, 0, bytes.data(), bytes.size()) == bytes.size() ? 
        StatusCode::OK : StatusCode::FAILED;

    return status;

} // end of "transmit_bytes(const vector<uint8_t>&)"


StatusedValue<vector<uint8_t>> I2C::receive_bytes(int num_bytes, int timeout_ms)
{
    if(timeout_ms == -1)
        timeout_ms = get_timeout_ms();

    // Store the read bytes
    vector<uint8_t> bytes;

    // Perform the i2c read
    StatusCode status = i2c_read(&m_device, 0, bytes.data(), num_bytes) == num_bytes ?
        StatusCode::OK : StatusCode::FAILED;

    return StatusedValue<vector<uint8_t>>(bytes, status);

} // end of "receive_bytes(i2c_device, int)"