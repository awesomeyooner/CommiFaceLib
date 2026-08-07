#include "CommiFaceLib/protocols/i2c.hpp"


using namespace std;
using namespace status_utils;

namespace fs = filesystem;

using ByteData = vector<uint8_t>;


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
    for(auto entry : fs::directory_iterator(SYS_DEVICE_PATH))
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
        if(!fs::is_regular_file(name_file))
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

} // end of "get_m_bus"


StatusCode I2C::transmit_bytes(i2c_device* device, const vector<uint8_t>& bytes)
{
    StatusCode status = i2c_write(device, 0, bytes.data(), bytes.size()) == bytes.size() ? 
        StatusCode::OK : StatusCode::FAILED;

    return status;

} // end of "transmit_bytes(i2c_device*, const vector<uint8_t>&)"


StatusedValue<vector<uint8_t>> I2C::receive_bytes(i2c_device* device, int num_bytes)
{
    // Store the read bytes
    vector<uint8_t> bytes;

    // Perform the i2c read
    StatusCode status = i2c_read(device, 0, bytes.data(), num_bytes) == num_bytes ?
        StatusCode::OK : StatusCode::FAILED;

    return StatusedValue<vector<uint8_t>>(bytes, status);

} // end of "receive_bytes(i2c_device, int)"


StatusedValue<vector<uint8_t>> I2C::read_bus(i2c_device* device, size_t num_bytes)
{
    // Create an empty array to store the bytes
    uint8_t buffer[num_bytes] = {};

    vector<uint8_t> buf;

    // Read the m_bus and store it in `buffer` 
    // If it returns the number of requested bytes, it worked (OK), if not, it failed (FAILED)
    StatusCode status = i2c_read(device, 0, buf.data(), num_bytes) == num_bytes ? StatusCode::OK : StatusCode::FAILED;

    // Create the vector equivalent of `buffer`
    vector<uint8_t> vec(buffer, buffer + num_bytes);

    // Return the vector and status
    return StatusedValue<vector<uint8_t>>(vec, status);

} // end of "read_m_bus"


StatusedValue<float> I2C::read_bus(i2c_device* device)
{
    // The number of bytes in a `float`
    size_t float_size = sizeof(float);

    // Read 4 bytes from the m_bus
    StatusedValue<vector<uint8_t>> read = read_bus(device, float_size);

    // Convert Byte Vector to Float
    float value = ByteConverter::bytes_to_float(read.value);

    // Return the float and status
    return StatusedValue<float>(value, read.status);
    
} // end of "read_m_bus"


StatusCode I2C::write_bus(i2c_device* device, vector<uint8_t>& write)
{
    // Create an array with the same size of `write` since libi2c uses arrays but I like vectors
    uint8_t buffer[write.size()];

    // Copy the contents from the vector to the array
    copy(write.begin(), write.end(), buffer);

    // Write to the device. If it returns the number of bytes in `write, it worked (OK), if not, it failed (FAILED)
    return i2c_write(device, 0, buffer, write.size()) == write.size() ? 
        StatusCode::OK : StatusCode::FAILED;

} // end of "write_m_bus"


StatusCode I2C::write_bus(i2c_device* device, uint8_t write)
{
    // Store the byte into a vector so we can use the original `write_m_bus` method
    vector data = {write};

    // Call the `write_m_bus` method
    return write_bus(device, data);

} // end of "write_m_bus"


StatusCode I2C::write_bus(i2c_device* device, float data)
{
    // Convert Float to Byte Array
    vector<uint8_t> bytes = ByteConverter::float_to_bytes(data);

    // Write the array to the device
    return write_bus(device, bytes);
    
} // end of "write_m_bus"