#include "CommiFaceLib/util/byte_converter.hpp"


// Define the unions
IntsBytesConverter i_to_b; // int to bytes;
FloatsBytesConverter f_to_b; // float to bytes
DoublesBytesConverter d_to_b; // double to bytes


std::string ByteConverter::bytes_to_string(const std::vector<uint8_t>& v_bytes)
{
    std::string str(v_bytes.begin(), v_bytes.end());

    return str;

} // end of "bytes_to_string(const std::vector<uint8_t>&)"


std::vector<uint8_t> ByteConverter::string_to_bytes(const std::string& str)
{
    std::vector<uint8_t> bytes(str.begin(), str.end());

    return bytes;

} // end of "string_to_bytes(const std::string&)"


int ByteConverter::bytes_to_int(const std::vector<uint8_t>& v_bytes)
{
    // OOB Safety
    if(v_bytes.size() != sizeof(int))
        return 0;

    // Copy the data from the vector into the union
    for(int i = 0; i < sizeof(int); i++)
    {
        i_to_b.bytes[i] = v_bytes.at(i);
    }

    // Return the converted value
    return i_to_b.i_value;

} // end of "bytes_to_int(const std::vector<uint8_t>&)"


std::vector<uint8_t> ByteConverter::int_to_bytes(int value)
{
    // Copy the data to the union
    i_to_b.i_value = value;

    // Copy the union values into a seperate vector
    std::vector<uint8_t> bytes;

    for(const uint8_t& byte : i_to_b.bytes)
    {
        bytes.push_back(byte);
    }

    // Return the vector
    return bytes;

} // end of "int_to_bytes"


float ByteConverter::bytes_to_float(const std::vector<uint8_t>& v_bytes)
{
    // OOB Safety
    if(v_bytes.size() != sizeof(float))
        return 0;

    // Copy the data from the vector into the union
    for(int i = 0; i < sizeof(float); i++)
    {
        f_to_b.bytes[i] = v_bytes.at(i);
    }

    // Return the converted value
    return f_to_b.f_value;

} // end of "bytes_to_float"


std::vector<uint8_t> ByteConverter::float_to_bytes(float value)
{
    // Copy the data to the union
    f_to_b.f_value = value;

    // Copy the union values into a seperate vector
    std::vector<uint8_t> bytes;

    for(const uint8_t& byte : f_to_b.bytes)
    {
        bytes.push_back(byte);
    }

    // Return the vector
    return bytes;

} // end of "float_to_bytes"


double ByteConverter::bytes_to_double(const std::vector<uint8_t>& v_bytes)
{
    // OOB Safety
    if(v_bytes.size() != sizeof(double))
        return 0;

    // Copy the data from the vector into the union
    for(int i = 0; i < sizeof(double); i++)
    {
        d_to_b.bytes[i] = v_bytes.at(i);
    }

    // Return the converted value
    return d_to_b.d_value;

} // end of "bytes_to_double"


std::vector<uint8_t> ByteConverter::double_to_bytes(double value)
{
    // Copy the data to the union
    d_to_b.d_value = value;

    // Copy the union values into a seperate vector
    std::vector<uint8_t> bytes;

    for(const uint8_t& byte : d_to_b.bytes)
    {
        bytes.push_back(byte);
    }

    // Return the vector
    return bytes;
} // end of "double_to_bytes"