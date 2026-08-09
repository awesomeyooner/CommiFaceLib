#include "CommiFaceLib/interfaces/communication_interface.hpp"


using namespace status_utils;
using namespace std;


template <typename T>
StatusCode CommunicationInterface::write_data(uint8_t reg, T value, bool acknowledge, int timeout_ms)
{
    // The raw data to send
    vector<uint8_t> data;

    // Explicitly check the type to convert
    if constexpr (std::is_same_v<T, double>)
        data = ByteConverter::double_to_bytes(value);
    else if constexpr (std::is_same_v<T, float>)
        data = ByteConverter::float_to_bytes(value);
    else if constexpr (std::is_same_v<T, int>)
        data = ByteConverter::int_to_bytes(value);
    else
        return StatusCode::ERROR;

    return write_packet(reg, data, acknowledge, timeout_ms);

} // end of "write_data(uint8_t, T)"


template<typename T>
StatusedValue<T> CommunicationInterface::request_data(uint8_t reg, int timeout_ms)
{
    int num_bytes;

    // Explicitly check the type to convert
    // safer than num_bytes = sizeof(T) imo
    if constexpr (std::is_same_v<T, double>)
        num_bytes = sizeof(double);
    else if constexpr (std::is_same_v<T, float>)
        num_bytes = sizeof(float);
    else if constexpr (std::is_same_v<T, int>)
        num_bytes = sizeof(int);
    else
        return StatusedValue<vector<uint8_t>>({}, StatusCode::ERROR);

    auto request_status = request(reg, num_bytes, timeout_ms);

    T requested_data;

    // If the request isn't OK then return the status code
    if(!request_status.is_OK())
        return StatusedValue<T>(requested_data, request_status.status);

    // Explicitly check again...
    // TODO: find a way to optimize this repeated code
    if constexpr (std::is_same_v<T, double>)
        requested_data = ByteConverter::bytes_to_double(request_status.value);
    else if constexpr (std::is_same_v<T, float>)
        requested_data = ByteConverter::bytes_to_float(request_status.value);
    else if constexpr (std::is_same_v<T, int>)
        requested_data = ByteConverter::bytes_to_int(request_status.value);

    return StatusedValue<T>(requested_data, request_status.status);

} // end of "request_data(uint8_t, int)"