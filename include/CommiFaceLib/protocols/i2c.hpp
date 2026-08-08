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

#include "CommiFaceLib/interfaces/communication_interface.hpp"
#include "CommiFaceLib/util/byte_converter.hpp"

#include "plib/util/logger.hpp"
#include "plib/util/status.hpp"

/**
 * @brief Represents an I2C device. Call the static `init` or init functions to initialize which bus to use for I2C transactions.
 * 
 * Please use `sudo apt install -y i2c-tools`
 * 
 * and
 * 
 * `sudo i2cdetect -l`
 * 
 * To scan all i2c devices
 * 
 */
class I2C : public CommunicationInterface 
{

    public:

        // The base path where the i2c device folders are
        inline static const std::string SYS_DEVICE_PATH = "/sys/bus/i2c/devices";

        // What all i2c devices start with
        inline static const std::string I2C_DEVICE_PREFIX = "i2c-"; 

        // The file inside /syus/bus/i2c/devices/<device> that contains the name
        // of the adapter
        inline static const std::string DEVICE_NAME_FILE = "name";

        /**
         * @brief Initialize the internal I2C device with the given settings
         * 
         * @param address `int` The address
         * @param bytes_per_page `int = 8` The number of bytes per page. Defaults to 8
         * @param timeout_ms `int = -1` The timeout for read transactions in milliseconds. Defaults to internal
         * default
         */
        I2C(int address, int bytes_per_page = 8, int timeout_ms = -1);

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
         * @brief Get the underlying `i2c_device` struct as a reference
         * 
         * @return `i2c_device&` 
         */
        i2c_device& get_device();

        /**
         * @brief Transmit the given bytes via i2c
         * 
         * @param bytes `const std::vector<uint8_t>&`
         * @return `status_utils::StatusCode` OK if successful, FAILED otherwise 
         */
        status_utils::StatusCode transmit_bytes(const std::vector<uint8_t>& bytes) override;

        /**
         * @brief Read a specific number of bytes
         * 
         * @param num_bytes `int`
         * @param timeout_ms `int = -1`
         * @return `status_utils::StatusedValue<std::vector<uint8_t>>` 
         */
        status_utils::StatusedValue<std::vector<uint8_t>> receive_bytes(int num_bytes, int timeout_ms = -1) override;
        
    private:

        // The internal i2c bus number
        static inline int m_bus = -1;

        // The internal i2c device
        i2c_device m_device;

}; // class I2C


#endif // I2C_HPP