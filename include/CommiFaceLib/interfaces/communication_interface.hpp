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

#include "plib/util/status.hpp"


class CommunicationInterface
{

    public:

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
         * @return `status_utils::StatusedValue<std::vector<uint8_t>>` OK if successful, FAILED otherwise.
         * The vector is populated with the bytes read 
         */
        virtual status_utils::StatusedValue<std::vector<uint8_t>> receive_bytes(int num_bytes) = 0;

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
    

}; // class CommunicationInterface

#endif // COMMUNICATION_INTERFACE_HPP