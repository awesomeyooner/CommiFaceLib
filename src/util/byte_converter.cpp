#include "CommiFaceLib/util/byte_converter.hpp"

// Define the unions
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


float ByteConverter::bytes_to_float(const std::vector<uint8_t>& v_bytes)
{
    // Copy the data from the vector into the union
    f_to_b.bytes[0] = v_bytes.at(0);
    f_to_b.bytes[1] = v_bytes.at(1);
    f_to_b.bytes[2] = v_bytes.at(2);
    f_to_b.bytes[3] = v_bytes.at(3);

    // Return the converted value
    return f_to_b.f_value;

} // end of "bytes_to_float"


std::vector<uint8_t> ByteConverter::float_to_bytes(float value)
{
    // Copy the data to the union
    f_to_b.f_value = value;

    // Copy the union values into a seperate vector
    std::vector<uint8_t> bytes(
        {
            f_to_b.bytes[0],
            f_to_b.bytes[1],
            f_to_b.bytes[2],
            f_to_b.bytes[3]
        }
    );

    // Return the vector
    return bytes;

} // end of "float_to_bytes"


double ByteConverter::bytes_to_double(const std::vector<uint8_t>& v_bytes)
{
    // Copy the data from the vector into the union
    d_to_b.bytes[0] = v_bytes.at(0);
    d_to_b.bytes[1] = v_bytes.at(1);
    d_to_b.bytes[2] = v_bytes.at(2);
    d_to_b.bytes[3] = v_bytes.at(3);
    d_to_b.bytes[4] = v_bytes.at(4);
    d_to_b.bytes[5] = v_bytes.at(5);
    d_to_b.bytes[6] = v_bytes.at(6);
    d_to_b.bytes[7] = v_bytes.at(7);

    // TODO: Make this neater with a for loop
    // I'm doing this explicitly first as a sanity check

    // Return the converted value
    return d_to_b.d_value;

} // end of "bytes_to_double"


std::vector<uint8_t> ByteConverter::double_to_bytes(double value)
{
    // Copy the data to the union
    d_to_b.d_value = value;

    // Copy the union values into a seperate vector
    std::vector<uint8_t> bytes(
        {
            d_to_b.bytes[0],
            d_to_b.bytes[1],
            d_to_b.bytes[2],
            d_to_b.bytes[3],
            d_to_b.bytes[4],
            d_to_b.bytes[5],
            d_to_b.bytes[6],
            d_to_b.bytes[7]
        }
    );

    // TODO: Make this neater with a for loop
    // I'm doing this explicitly first as a sanity check

    // Return the vector
    return bytes;
} // end of "double_to_bytes"