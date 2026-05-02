#include "plib/i2c/drivers/psd17.hpp"

PSD17::PSD17(int address) : WireDevice(address){}

status_utils::StatusCode PSD17::disable()
{
    // Send the disable command
    return send_command(CommandType::DISABLE_MOTOR);

} // end of "disable"


status_utils::StatusCode PSD17::enable()
{
    // Send the enable command
    return send_command(CommandType::ENABLE_MOTOR);

} // end of "enable"


status_utils::StatusedValue<bool> PSD17::is_enabled()
{
    // Get if the motor is enabled or not
    status_utils::StatusedValue<float> is_enabled = read_from_register((uint8_t)RequestType::IS_ENABLED);

    // If the read wasn't success, return FAILED
    if(is_enabled.status != status_utils::StatusCode::OK)
        return status_utils::StatusedValue<bool>(false, status_utils::StatusCode::FAILED);

    // Return the enable status
    return status_utils::StatusedValue<bool>((int)is_enabled.value == 0 ? false : true, status_utils::StatusCode::OK);
    
} // end of "is_enabled"

status_utils::StatusCode PSD17::send_command(CommandType type)
{
    // Write to the register
    return write((uint8_t)type);

} // end of "send_command"


status_utils::StatusCode PSD17::send_command(float cmd, CommandType type)
{
    // Write to the register
    return write_to_register((uint8_t)type, cmd);

} // end of "send_command"


status_utils::StatusedValue<float> PSD17::request(RequestType type)\
{
    // Read the register
    return read_from_register((uint8_t)type);

} // end of "request"