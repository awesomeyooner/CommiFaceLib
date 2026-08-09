#ifndef COMMUNICATION_INTERFACE_HPP
#define COMMUNICATION_INTERFACE_HPP


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

#include "plib/util/status.hpp"


/**
 * @brief Abstraction for communication interface like Serial, I2C, CAN, SPI, etc.
 * 
 * To use this, mainly just implement 
 * 
 * ```c++
 * status_utils::StatusCode transmit_bytes(const std::vector<uint8_t>& bytes) override;
 * 
 * status_utils::StatusedValue<std::vector<uint8_t>> receive_bytes(int num_bytes, int timeout_ms = -1) override;
 * ```
 * 
 */
class CommunicationInterface
{

    public:

        // The default timeout for read transactions in milliseconds
        static constexpr int DEFAULT_TIMEOUT_MS = 100;

        /**
         * @brief Transmits bytes through the specific communication interface. All
         * child classes must implement this method
         * 
         * @param bytes `const std::vector<uint8_t>&`
         * @return `status_utils::StatusCode` OK if successful, FAILED otherwise 
         */
        virtual status_utils::StatusCode transmit_bytes(const std::vector<uint8_t>& bytes) = 0;

        /**
         * @brief Receive a given number of bytes
         * 
         * @param num_bytes `int` The number of bytes to receive
         * @param timeout_ms `int = -1` The timeout in milliseconds. If this is not set, the internal timeout will be used
         * @return `status_utils::StatusedValue<std::vector<uint8_t>>` OK if successful, FAILED otherwise.
         * The vector is populated with the bytes read 
         */
        virtual status_utils::StatusedValue<std::vector<uint8_t>> receive_bytes(int num_bytes, int timeout_ms = -1) = 0;

        /**
         * @brief Write `data` to the given register. 
         * 
         * @param reg `uint8_t` The register byte to writ eto
         * @param data `const std::vector<uint8_t>&` The data to send
         * @param acknowledge `bool = false` Set to true to read the same bytes as `data` and check if it is the same
         * @param timeout_ms `int = -1` The timeout in millisecnds. If this is not set, it will use the internal timeout.
         * @return `status_utils::StatusCode` OK if successful, FAILED if failed to write, and ERROR if acknowledge failed
         */
        virtual status_utils::StatusCode write_packet(uint8_t reg, const std::vector<uint8_t>& data = {}, bool acknowledge = false, int timeout_ms = -1);

        /**
         * @brief Request a number of bytes from a specific register
         * 
         * @param reg `uint8_t` The register to read from
         * @param num_bytes `int` The number of bytes to read
         * @param timeout_ms `int = -1` The timeout in millisecnds. This will fallback to the default if not set
         * @return `status_utils::StatusCode` OK if successful. FAILED otherwise 
         */
        virtual status_utils::StatusedValue<std::vector<uint8_t>> request(uint8_t reg, int num_bytes, int timeout_ms = -1);

        /**
         * @brief Write a specific data type (only double, float, int) to the given register. This is a shorthand for `write_packet` that 
         * converts the data type automatically
         * 
         * @tparam `T` The type of data to send. Only `double`, `float`, and `int` are allowed. 
         * @param reg `uint8_t` The register to write to
         * @param value `T` The data to send
         * @param acknowledge `bool = false` Set to true to request data back and check if data is the same
         * @param timeout_ms `int = -1` The timeout for the read transaction (if acknowledge is true). Defaults to the internally set timeout
         * @return `status_utils::StatusCode` OK if successful, FAILED if read or write transactions failed, and ERROR if acknowlegement was incorrect 
         */
        template <typename T>
        status_utils::StatusCode write_data(uint8_t reg, T value, bool acknowledge = false, int timeout_ms = -1);

        /**
         * @brief Request a specific data type (only double, float, int) from the given register. This is shorthand for `request` that
         * converts the data type automatically
         * 
         * @tparam `T` The type of data to send. Only `double`, `float`, and `int` are allowed. 
         * @param timeout_ms `int = -1` The timeout for the read transaction (if acknowledge is true). Defaults to the internally set timeout
         * @return `status_utils::StatusedValue<std::vector<uint8_t>>` OK if successfully read, and FAILED otherwise.
         */
        template<typename T>
        status_utils::StatusedValue<T> request_data(uint8_t, int timeout = -1);


        /**
         * @brief Get the timeout in milliseconds
         * 
         * @return `int` 
         */
        virtual int get_timeout_ms();

        /**
         * @brief Set the transaction timeout in milliseconds
         * 
         */
        virtual void set_timeout_ms(int timeout);

    protected:

        /**
         * @brief Creates a vector of bytes with the given packet standard
         * 
         * Byte Order:
         * 
         * 1. REGISTER (represents the register to write to)
         * 
         * 2. LENGTH (represents the length of the packet in bytes)
         * 
         * 3. The rest is data
         * 
         * @param reg `uint8_t` The register to write to
         * @param data `const std::vector<uint8_t>` The data payload
         * @return `std::vector<uint8_t>` 
         */
        virtual std::vector<uint8_t> create_packet(uint8_t reg, const std::vector<uint8_t>& data);

    private:

        // The timeout transaction for read/writes
        int m_timeout_ms = DEFAULT_TIMEOUT_MS; // ms
    

}; // class CommunicationInterface


// Include implementation file for template methods
#include "CommiFaceLib/interfaces/communication_interface.tpp"


#endif // COMMUNICATION_INTERFACE_HPP