#include "plib/i2c/wire_device.hpp"

WireDevice::WireDevice(int address, int bus, int page_bytes)
{
    // Initialize the internal device object with the following settings
    device.addr = address;
    device.bus = bus;
    device.page_bytes = page_bytes; // max for MCP2221A
    device.iaddr_bytes = 0;
    device.tenbit = 0;
    device.flags = 0;
    device.delay = 0; // milliseconds

} // end of "WireDevice"


int WireDevice::get_address()
{
    // Return the device address
    return device.addr;

} // end of "get_address"


i2c_device* WireDevice::get()
{
    // Return the pointer to the internal device
    return &device;
} // end of "get"


status_utils::StatusedValue<std::vector<uint8_t>> WireDevice::read(size_t num_bytes)
{
    // Read the specified number of bytes on the device
    return I2C::read_bus(&device, num_bytes);

} // end of "read"


status_utils::StatusedValue<float> WireDevice::read()
{
    // Read a float
    return I2C::read_bus(&device);

} // end of "read"


status_utils::StatusCode WireDevice::write(uint8_t write)
{
    // Write the byte to the bus
    return I2C::write_bus(&device, write);

} // end of "write"


status_utils::StatusCode WireDevice::write(std::vector<uint8_t>& write)
{
    // Write a vector of bytes to the bus
    return I2C::write_bus(&device, write);

} // end of "write"


status_utils::StatusCode WireDevice::write(float data)
{
    // Write a float to the bus
    return I2C::write_bus(&device, data);

} // end of "write"

status_utils::StatusedValue<float> WireDevice::read_from_register(uint8_t reg)
{
    // Convert the register into a vector
    std::vector<uint8_t> data;
    data.push_back(reg);

    // Write the register to the device
    // If it's not OK, then return FAILED status
    if(write(data) != status_utils::StatusCode::OK)
        return status_utils::StatusedValue<float>(0, status_utils::StatusCode::FAILED);
    
    // Read 4 bytes (float) from the device
    status_utils::StatusedValue<std::vector<uint8_t>> read_status = read(sizeof(float));

    // If the status is not OK, return FAILED
    if(read_status.status != status_utils::StatusCode::OK)
        return status_utils::StatusedValue<float>(0, status_utils::StatusCode::FAILED);

    // Convert the bytes to a float
    float val = I2C::bytes_to_float(read_status.value);

    // Return the float and OK status
    return status_utils::StatusedValue<float>(val, status_utils::StatusCode::OK);

} // end of "read_from_register"


status_utils::StatusCode WireDevice::write_to_register(uint8_t reg, float val)
{
    // Convert the float to bytes
    std::vector<uint8_t> data = I2C::float_to_bytes(val);

    // Insert the register to the start of the vector
    data.insert(data.begin(), reg);

    // Write to the device
    return write(data);
    
} // end of "write_to_register"


