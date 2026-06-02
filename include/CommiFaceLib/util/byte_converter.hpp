#ifndef BYTE_CONVERTER_HPP
#define BYTE_CONVERTER_HPP

#include <string>
#include <vector>
#include <cstring>
#include <cstdint>


union IntsBytesConverter
{
    int i_value;
    uint8_t bytes[sizeof(int)];
};

union FloatsBytesConverter
{
    float f_value;
    uint8_t bytes[sizeof(float)];
};

union DoublesBytesConverter
{
    double d_value;
    uint8_t bytes[sizeof(double)];
};

// You must declare unions like this and define them in .cpp
extern IntsBytesConverter i_to_b; // int to bytes
extern FloatsBytesConverter f_to_b; // float to bytes
extern DoublesBytesConverter d_to_b; // double to bytes

class ByteConverter
{

    public:

        /**
         * @brief Converts a vector of bytes of arbitrary length to a string
         * 
         * @param v_bytes `const std::vector<uint8_t>&` The vector of bytes
         * @return `std::string` The converted string 
         */
        static std::string bytes_to_string(const std::vector<uint8_t>& v_bytes);

        /**
         * @brief Converts a string into a vector of bytes (`uint8_t`)
         * 
         * @param str `std::string` The string to convert
         * @return `std::vector<uint8_t>` The byte vector 
         */
        static std::vector<uint8_t> string_to_bytes(const std::string& str);

        /**
         * @brief Converts a vector of 4 bytes into an int
         * 
         * @param v_bytes `std::vector<uint8_t>` The vector of 4 bytes 
         * @return `int` The converted int value 
         */
        static int bytes_to_int(const std::vector<uint8_t>& v_bytes);

        /**
         * @brief Converts an int into a vector of 4 bytes
         * 
         * @param value `int` The int value to convert
         * @return `std::vector<uint8_t>` A vector of 4 bytes
         * representing the int 
         */
        static std::vector<uint8_t> int_to_bytes(int value);

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