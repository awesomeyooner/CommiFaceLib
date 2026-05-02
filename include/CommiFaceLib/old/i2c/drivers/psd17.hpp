#ifndef PSD17_HPP
#define PSD17_HPP

#include "plib/util/status.hpp"

#include "plib/i2c/i2c.hpp"
#include "plib/i2c/wire_device.hpp"
#include "plib/i2c/wire_types.hpp"

class PSD17 : public WireDevice{

    public:

        /**
         * @brief Initialize the WireDevice with the given address
         * 
         * @param address `int` The address that this driver corresponds to 
         */
        PSD17(int address);

        /**
         * @brief Send a disable command to the motor
         * 
         * @return `status_utils::StatusCode` OK if successfully disabled, FAILED otherwise 
         */
        status_utils::StatusCode disable();

        /**
         * @brief Send an enable command to the motor
         * 
         * @return `status_utils::StatusCode` OK if successfully enabled, FAILED otherwise 
         */
        status_utils::StatusCode enable();

        /**
         * @brief Gets if the motor is enabled or not directly from the driver itself
         * 
         * @return `status_utils::StatusedValue<bool>` OK if successfully retried. FAILED otherise. 
         */
        status_utils::StatusedValue<bool> is_enabled();

        /**
         * @brief Send an empty command to the motor
         * 
         * @param type `CommandType` The type of command. Use for settings with no corresponding data
         * @return `status_utils::StatusCode` OK if successfully sent. FAILED otherwise 
         */
        status_utils::StatusCode send_command(CommandType type);

        /**
         * @brief Send an command with its corresponding data to the motor
         * 
         * @param cmd `float` The command data to send to the motor
         * @param type `CommandType` The type of command. Use for settings with no corresponding data
         * @return `status_utils::StatusCode` OK if successfully sent. FAILED otherwise 
         */
        status_utils::StatusCode send_command(float cmd, CommandType type);

        /**
         * @brief Requests data from the driver.
         * 
         * @param type `RequestType` The type to request
         * @return status_utils::StatusedValue<float> OK if successfully sent. FAILED otherwise 
         */
        status_utils::StatusedValue<float> request(RequestType type);
        

    private:

}; // class PSD17

#endif // PSD17_HPP