#ifndef BYTE_CONVERTER_HPP
#define BYTE_CONVERTER_HPP

#include <string>
#include <vector>
#include <cstring>
#include <cstdint>

union FloatsBytesConverter{
    float f_value;
    uint8_t bytes[4];
};

union DoublesBytesConverter{
    double d_value;
    uint8_t bytes[8];
};

// You must declare unions like this and define them in .cpp
extern FloatsBytesConverter f_to_b; // float to bytes
extern DoublesBytesConverter d_to_b; // double to bytes


class ByteConverter
{

    public:

        /**
         * @brief Converts a vector of 4 bytes into a float 
         * 
         * @param v_bytes `std::vector<uint8_t>` The vector of 4 bytes 
         * @return `float` The converted float value 
         */
        static float bytes_to_float(const std::vector<uint8_t>& v_bytes);

        /**
         * @brief Converts a float into a vector of 4 bytes
         * 
         * @param value `float` The float value to convert
         * @return `std::vector<uint8_t>` A vector of 4 bytes
         * representing the float 
         */
        static std::vector<uint8_t> float_to_bytes(float value);

        /**
         * @brief Converts a vector of 8 bytes into a double
         * 
         * @param v_bytes `std::vector<uint8_t>` The vector of 8 bytes 
         * @return `double` The converted double value 
         */
        static double bytes_to_double(const std::vector<uint8_t>& v_bytes);

        /**
         * @brief Converts a double into a vector of 8 bytes
         * 
         * @param value `double` The double value to convert
         * @return `std::vector<uint8_t>` A vector fo 8 bytes
         * representing the double 
         */
        static std::vector<uint8_t> double_to_bytes(double value);

}; // class ByteConverter


#endif // BYTE_CONVERTER_HPP