#ifndef WIRE_DEVICE_HPP
#define WIRE_DEVICE_HPP

#include "i2c/i2c.h"

#include <string>
#include <iostream>
#include <functional>
#include <vector>

#include "i2c.hpp"
#include "plib/util/status.hpp"

class WireDevice{

    public:

        /**
         * @brief Initialize the device with the given settings
         * 
         * @param address `int` The address this device corresponds to
         * @param bus `int` Default `I2C::get_bus()` - The bus this device is on
         * @param page_bytes `int` Default `8` - The number of bytes per page
         */
        WireDevice(int address, int bus = I2C::get_bus(), int page_bytes = 8);


        /**
         * @brief Get the device address
         * 
         * @return `int` The address 
         */
        int get_address();

        
        /**
         * @brief Get the i2c device pointer
         * 
         * @return `i2c_device*` The device pointer 
         */
        i2c_device* get();


        /**
         * @brief Read a float from the device at a specific register
         * 
         * @param reg `uint8_t` The register to read from
         * @return `status_utils::StatusedValue<float>` 
         */
        status_utils::StatusedValue<float> read_from_register(uint8_t reg);


        /**
         * @brief Write a float to the specified register
         * 
         * @param reg `uint8_t` The register to write to 
         * @param val `float` The float to write
         * @return `status_utils::StatusCode` OK if successful, FAILED otherwise 
         */
        status_utils::StatusCode write_to_register(uint8_t reg, float val);


        /**
         * @brief Read the specified number of bytes from the device
         * 
         * @param num_bytes `size_t` The number of bytes
         * @return `status_utils::StatusedValue<std::vector<uint8_t>>` The bytes read 
         */
        status_utils::StatusedValue<std::vector<uint8_t>> read(size_t num_bytes);

        
        /**
         * @brief Read a float from the device
         * 
         * @return `status_utils::StatusedValue<float>` The float read 
         */
        status_utils::StatusedValue<float> read();

        
        /**
         * @brief Write a byte to the device
         * 
         * @param write `uint8_t` The byte to write 
         * @return `status_utils::StatusCode` OK if successful, FAILED otherwise 
         */
        status_utils::StatusCode write(uint8_t write);


        /**
         * @brief Write a vector of bytes to the bus
         * 
         * @param write `std::vector<uint8_t>&` The vector of bytes to write
         * @return `status_utils::StatusCode` OK if successful, FAILED otherwise 
         */
        status_utils::StatusCode write(std::vector<uint8_t>& write);


        /**
         * @brief Write a float to the device
         * 
         * @param data `float` The float to write
         * @return `status_utils::StatusCode` OK if successful, FAILED otherwise 
         */
        status_utils::StatusCode write(float data);

    private:

        i2c_device device;

}; // class WireDevice

#endif // WIRE_DEVICE_HPP