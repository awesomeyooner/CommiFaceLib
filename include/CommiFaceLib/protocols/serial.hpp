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

#include "CommiFaceLib/util/byte_converter.hpp"

#include "plib/util/logger.hpp"
#include "plib/util/status.hpp"

#include <libserial/SerialPort.h>


class SerialInterface
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
         * @brief Set the timeout duration for reads and writes in milliseconds
         * 
         * @param timeout `double` 
         */
        void set_timeout_ms(double timeout);

        /**
         * @brief Get the timeout duration for reads and writes in milliseconds
         * 
         * @return double 
         */
        double get_timeout_ms();

        /**
         * @brief Transmit a vector of bytes
         * 
         * @param bytes `const std::vector<uint8_t>& bytes
         * @return `status_utils::StatusCode` 
         */
        status_utils::StatusCode transmit_bytes(const std::vector<uint8_t>& bytes);

        status_utils::StatusCode write_to_register(uint8_t reg, const std::vector<uint8_t>& data = {}, bool acknowledge = false);

        status_utils::StatusCode write_int(uint8_t reg, int data, bool acknowledge = false);

        status_utils::StatusCode write_float(uint8_t reg, float data, bool acknowledge = false);

        status_utils::StatusCode write_double(uint8_t reg, double data, bool acknowledge = false);

        status_utils::StatusedValue<std::vector<uint8_t>> receive_bytes(int num_bytes, int timeout_ms = -1);

        status_utils::StatusedValue<std::vector<uint8_t>> receive_bytes(char delimiter, int timeout_ms = -1);

        status_utils::StatusedValue<std::vector<uint8_t>> request(uint8_t reg, int num_bytes, int timeout_ms = -1);

        status_utils::StatusedValue<int> request_int(uint8_t reg, int timeout_ms = -1);

        status_utils::StatusedValue<float> request_float(uint8_t reg, int timeout_ms = -1);

        status_utils::StatusedValue<double> request_double(uint8_t reg, int timeout_ms = -1);

        // status_utils::StatusedValue<int> request_int(uint8_t reg)

    private:

        // The underlying LibSerial port
        LibSerial::SerialPort m_serial_port;

        // The timeout for read operations in milliseconds
        int m_timeout_ms = 500;

        /**
         * @brief Creates a single vector that adds the register and length to the byte vector
         * 
         * @param reg `uint8_t` The register of this packet
         * @param data `const std::vector<uint8_t>&` The main data
         * @return `std::vector<uint8_t>` The resultant packet 
         */
        std::vector<uint8_t> create_packet(uint8_t reg, const std::vector<uint8_t>& data);

        double block_until(double timeout_seconds);

}; // class SerialInterface


#endif // SERIAL_HPP