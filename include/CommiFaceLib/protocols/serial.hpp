#ifndef SERIAL_HPP
#define SERIAL_HPP


#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include <cstring>
#include <cstdint>
#include <fstream>
#include <filesystem>
#include <exception>
#include <cmath>

#include "CommiFaceLib/interfaces/communication_interface.hpp"

#include "CommiFaceLib/util/byte_converter.hpp"

#include "plib/util/logger.hpp"
#include "plib/util/status.hpp"

#include <libserial/SerialPort.h>


/**
 * @brief Represents a Serial device. Call the static `init` or init functions to initialize which port to use for Serial transactions.
 * 
 * Please use `sudo apt install libserial-dev`
 * 
 */
class SerialInterface : public CommunicationInterface
{

    public: 

        // The base path where the sysfs devices are
        const std::string SYS_DEVICES_PATH = "/sys/class/tty";

        // The base path where the actual ports themselves are
        const std::string DEV_DEVICES_PATH = "/dev";

        // Where the fields are located inside a /tty entry
        const std::string FIELD_PATH = "device/..";

        /**
         * @brief Get the underlying LibSerial::SerialPort object
         * 
         * @return `LibSerial::SerialPort&` 
         */
        LibSerial::SerialPort& get_port();

        /**
         * @brief Opens the serial port at the given path. Note that theses paths are expected
         * to be of format `/dev/<name>`, such as `/dev/ACM0`.
         * 
         * @param path `std::string` The device path. Example: `/dev/ACM0`
         * @param verbose `bool` Set to true to output messages that describe what files are seen
         * @return `status_utils::StatusCode`  
         */
        status_utils::StatusCode init(std::string path, bool verbose = true);

        /**
         * @brief Opens the serial port whose specified field is equal to the desired field.
         * For example, STM32 devices have a `product` field, a typical device will have `STM32 Virtual ComPort` as
         * the value in this field, in which you would call 
         * 
         * `init_field("product", "STM32 Virtual ComPort");`
         * 
         * to open the proper port
         * 
         * @param field_name `std::string` The name of the field to look at. Don't include /'s
         * @param desired `std::string` The desired value this field should have
         * @param verbose `bool` True to output debug messages
         * @return `status_utils::StatusCode` OK if found the right device, FAILED otherwise 
         */
        status_utils::StatusCode init_field(std::string field_name, std::string desired, bool verbose = true);

        /**
         * @brief Opens the serial port with the given name. For instance, instead of using `/dev/ttyUSB0`
         * you can just use this function with `ttyUSB0`
         * 
         * @param name `std::string` The device name. Example: `ttyUSB0`
         * @param verbose `bool` Set to true to output messages that describe what files are seen
         * @return `status_utils::StatusCode` OK if found, FAILED otherwise
         */
        status_utils::StatusCode init_name(std::string name, bool verbose = true);

        /**
         * @brief Close the underlying serial port
         * 
         * @return `status_utils::StatusCode` OK if successful, FAILED otherwise 
         */
        status_utils::StatusCode close();

        /**
         * @brief Transmit the given bytes over serial
         * 
         * @param bytes `const std::vector<uint8_t>&`
         * @return `status_utils::StatusCode` OK if successful, FAILED otherwise 
         */
        status_utils::StatusCode transmit_bytes(const std::vector<uint8_t>& bytes) override;

        /**
         * @brief Receive the given number of bytes. 
         * 
         * @param num_bytes `int`
         * @param timeout_ms `int`
         * @return `status_utils::StatusedValue<std::vector<uint8_t>>` OK if successful, FAILED otherwise 
         */
        status_utils::StatusedValue<std::vector<uint8_t>> receive_bytes(int num_bytes, int timeout_ms = -1) override;

        /**
         * @brief Receive bytes until the delimiter is reached. The returned vector will not include the delimiter.
         * 
         * @param delimiter `char = '\n'` The delimiter character to use.
         * @param timeout_ms `int`
         * @return `status_utils::StatusedValue<std::vector<uint8_t>>` 
         */
        status_utils::StatusedValue<std::vector<uint8_t>> receive_bytes(char delimiter = '\n', int timeout_ms = -1);

        /**
         * @brief Read bytes from a specific register until the delimiter is reached. The returned vector will not include
         * the delimiter
         * 
         * @param reg `uint8_t`
         * @param delimiter `char = '\n'` The delimiter character to use.
         * @param timeout `int = -1`
         * @return `status_utils::StatusedValue<std::vector<uint8_t>>`
         */
        status_utils::StatusedValue<std::vector<uint8_t>> request(uint8_t reg, char delimiter = '\n', int timeout = -1);

    private:

        // The underlying LibSerial port
        LibSerial::SerialPort m_serial_port;

        double block_until(double timeout_seconds);

}; // class SerialInterface


#endif // SERIAL_HPP