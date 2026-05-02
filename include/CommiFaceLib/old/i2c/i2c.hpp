#ifndef I2C_HPP
#define I2C_HPP

#include "libi2c/include/i2c/i2c.h"

#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include <cstring>
#include <cstdint>
#include <fstream>
#include <filesystem>
#include <system_error>
#include "plib/util/logger.hpp"
#include "plib/util/status.hpp"

namespace fs = std::filesystem;

union FloatsBytesConverter{
    float f_value;
    uint8_t bytes[4];
};

// You must declare unions like this and define them in .cpp
extern FloatsBytesConverter f_to_b;

class I2C{

    public:

        static int m_bus;

        /**
         * @brief Initialize the i2c bus with the device path
         * 
         * @param name `char*` Default `"/dev/i2c-9"` - The i2c device path
         * @return `status_utils::StatusCode` The Status, OK if properly initialized, FAILED otherwise 
         */
        static status_utils::StatusCode init(const char* name = "/dev/i2c-9");


        /**
         * @brief Initialize the i2c bus with the device number
         * 
         * @param adapter_number `int` The device number, found in `"/dev/i2c-<num>"`
         * @return `status_utils::StatusCode` OK if found, FAILED otherwise
         */
        static status_utils::StatusCode init(int adapter_number);


        /**
         * @brief Initialize the i2c bus with the device name
         * 
         * @param name `std::string` The device name
         * @param verbose `bool` Default `false` - Displays the finding process if true
         * @return `status_utils::StatusCode` OK if it found the adapter, FAILED otherwise 
         */
        static status_utils::StatusCode init_name(std::string name, bool verbose = false);


        /**
         * @brief Get the bus number
         * 
         * @return `int` the bus number 
         */
        static int get_bus();


        /**
         * @brief Convert a vector of 4 bytes into a float
         * 
         * @param v_bytes `std::vector<uint8_t>` Vector full of bytes
         * @return `float` The float equivalent 
         */
        static float bytes_to_float(std::vector<uint8_t> v_bytes);


        /**
         * @brief Convert a float into a vector of 4 bytes
         * 
         * @param value `float` The float to convert
         * @return `std::vector<uint8_t>` The vector full of bytes 
         */
        static std::vector<uint8_t> float_to_bytes(float value);

        
        /**
         * @brief Read a specific number of bytes from the bus
         * 
         * @param device `i2c_device*` The i2c device to read from
         * @param num_bytes `size_t` The number of bytes to read
         * @return `status_utils::StatusedValue<std::vector<uint8_t>>` Vector full of bytes that it read with status 
         */
        static status_utils::StatusedValue<std::vector<uint8_t>> read_bus(i2c_device* device, size_t num_bytes);
        

        /**
         * @brief Read a float from the bus. Simply just reads 4 bytes and converts it to a float
         * 
         * @param device `i2c_device*` The i2c device to read from
         * @return `status_utils::StatusedValue<float>` The float and status 
         */
        static status_utils::StatusedValue<float> read_bus(i2c_device* device);


        /**
         * @brief Write a vector of bytes to a device
         * 
         * @param device `i2c_device*` The i2c device to write to
         * @param write `std::vector<uint8_t>&` The vector of bytes
         * @return `status_utils::StatusCode` OK if it was successful, FAILED otherwise 
         */
        static status_utils::StatusCode write_bus(i2c_device* device, std::vector<uint8_t>& write);


        /**
         * @brief Write a single byte to a device
         * 
         * @param device `i2c_device*` The i2c device to write to
         * @param write `uint8_t` The byte to write
         * @return `status_utils::StatusCode` OK if it was successful, FAILED otherwise 
         */
        static status_utils::StatusCode write_bus(i2c_device* device, uint8_t write);

        
        /**
         * @brief Write a float (4 bytes) to a device
         * 
         * @param device `i2c_device` The i2c device to write to
         * @param data `float` The float to write
         * @return `status_utils::StatusCode` OK if it was successful, FAILED otherwise  
         */
        static status_utils::StatusCode write_bus(i2c_device* device, float data);


    private:

}; // class I2C


#endif // I2C_HPP