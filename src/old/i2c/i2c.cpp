#include "plib/i2c/i2c.hpp"

namespace fs = std::filesystem;

// Define f_to_b
FloatsBytesConverter f_to_b;

// Default the m_bus to negative, indicating it has not been initialized
int I2C::m_bus = -1;


status_utils::StatusCode I2C::init(const char* name)
{
    // Open the i2c m_bus and store the m_bus number in the variable
    m_bus = i2c_open(name);

    // if the m_bus is -1 then return as FAILED, if not, return OK
    if(m_bus == -1)
        return status_utils::StatusCode::FAILED;
    else
        return status_utils::StatusCode::OK;

} // end of "init"


status_utils::StatusCode I2C::init(int adapter_number)
{
    std::string name = "/dev/i2c-" + std::to_string(adapter_number);

    return init(name.c_str());
}

status_utils::StatusCode I2C::init_name(std::string name, bool verbose)
{
    // The path where all the i2c folders are in
    const fs::path i2c_path = "/sys/bus/i2c/devices";

    // For every folder within the main path, check the names match
    // the given name. If it does, use that adapter and return OK
    for(auto entry : fs::directory_iterator(i2c_path))
    {   
        // The the i2c device folder name "i2c-<num>"
        std::string directory_name = entry.path().filename().string();

        // If it's not an i2c device then skip
        if(directory_name.compare(0, 4, "i2c-") != 0)
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
            adapter_number = std::stoi(directory_name.substr(4));
        }
        // If there's an error, tell the user that it's not a proper adapter
        catch(std::exception& e)
        {

            if(verbose)
                Logger::info("Found Folder: " + directory_name + "... " + "Not a proper adapter! Skipping...");

            continue;
        }

        // The file containing the name is `name`
        auto name_file = entry.path() / "name";

        // If it's not a regular file then skip
        if(!fs::is_regular_file(name_file))
            continue;

        // Open the file
        std::ifstream file(name_file);

        // If it didn't properly open then skip
        if(!file.is_open())
            continue;

        // Store the adapter name in this variable
        std::string adapter_name;

        // Grab the first line of the file and store it in `adapter_name`
        std::getline(file, adapter_name);

        // If the file is empty / if the adapter name is nothing, skip
        if(adapter_name.empty())
            continue;

        // If the name matches the one specified, initialize with that adapter number
        if(adapter_name.find(name) != std::string::npos)
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
    return status_utils::StatusCode::FAILED;

} //  end of "init_name"


int I2C::get_bus()
{
    return m_bus;
} // end of "get_m_bus"


float I2C::bytes_to_float(std::vector<uint8_t> v_bytes)
{
    // Fill the union with the bytes
    f_to_b.bytes[0] = v_bytes.at(0);
    f_to_b.bytes[1] = v_bytes.at(1);
    f_to_b.bytes[2] = v_bytes.at(2);
    f_to_b.bytes[3] = v_bytes.at(3);

    // return the float union value
    return f_to_b.f_value;

} // end of "bytes_to_float"


std::vector<uint8_t> I2C::float_to_bytes(float value)
{
    // Set the union's value equal to the input
    f_to_b.f_value = value;

    // Extract the bytes from the union and store it in the vector
    std::vector<uint8_t> bytes(
        {
            f_to_b.bytes[0],
            f_to_b.bytes[1],
            f_to_b.bytes[2],
            f_to_b.bytes[3]
        }
    );

    // Return the vector
    return bytes;

} //  end of "float_to_bytes"


status_utils::StatusedValue<std::vector<uint8_t>> I2C::read_bus(i2c_device* device, size_t num_bytes)
{
    // Create an empty array to store the bytes
    uint8_t buffer[num_bytes] = {};

    // Read the m_bus and store it in `buffer` 
    // If it returns the number of requested bytes, it worked (OK), if not, it failed (FAILED)
    status_utils::StatusCode status = i2c_read(device, 0, buffer, num_bytes) == num_bytes ? status_utils::StatusCode::OK : status_utils::StatusCode::FAILED;

    // Create the vector equivalent of `buffer`
    std::vector<uint8_t> vec(buffer, buffer + num_bytes);

    // Return the vector and status
    return status_utils::StatusedValue<std::vector<uint8_t>>(vec, status);

} // end of "read_m_bus"


status_utils::StatusedValue<float> I2C::read_bus(i2c_device* device)
{
    // The number of bytes in a `float`
    size_t float_size = sizeof(float);

    // Read 4 bytes from the m_bus
    status_utils::StatusedValue<std::vector<uint8_t>> read = read_bus(device, float_size);

    // Convert Byte Vector to Float
    float value = bytes_to_float(read.value);

    // Return the float and status
    return status_utils::StatusedValue<float>(value, read.status);
    
} // end of "read_m_bus"


status_utils::StatusCode I2C::write_bus(i2c_device* device, std::vector<uint8_t>& write)
{
    // Create an array with the same size of `write` since libi2c uses arrays but I like vectors
    uint8_t buffer[write.size()];

    // Copy the contents from the vector to the array
    std::copy(write.begin(), write.end(), buffer);

    // Write to the device. If it returns the number of bytes in `write, it worked (OK), if not, it failed (FAILED)
    return i2c_write(device, 0, buffer, write.size()) == write.size() ? 
        status_utils::StatusCode::OK : status_utils::StatusCode::FAILED;

} // end of "write_m_bus"


status_utils::StatusCode I2C::write_bus(i2c_device* device, uint8_t write)
{
    // Store the byte into a vector so we can use the original `write_m_bus` method
    std::vector data = {write};

    // Call the `write_m_bus` method
    return write_bus(device, data);

} // end of "write_m_bus"


status_utils::StatusCode I2C::write_bus(i2c_device* device, float data)
{
    // Convert Float to Byte Array
    std::vector<uint8_t> bytes = float_to_bytes(data);

    // Write the array to the device
    return write_bus(device, bytes);
    
} // end of "write_m_bus"
