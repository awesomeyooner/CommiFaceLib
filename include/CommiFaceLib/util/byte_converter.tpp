#include "CommiFaceLib/util/byte_converter.hpp"


using namespace std;


template <typename T>
T ByteConverter::from_bytes(const vector<uint8_t>& v_bytes)
{
    // Int
    if constexpr (std::is_same_v<T, int>)
        return bytes_to_int(v_bytes);
    // Float
    else if constexpr (std::is_same_v<T, float>)
        return bytes_to_float(v_bytes);
    // Double
    else if constexpr (std::is_same_v<T, double>)
        return bytes_to_double(v_bytes);
    // String
    else if constexpr (std::is_same_v<T, string>)
        return bytes_to_string(v_bytes);
    // Bytes (just return value since it's already bytes)
    else if constexpr (std::is_same_v<T, vector<uint8_t>>)
        return v_bytes;
    // Everything else
    else
        return T{};
        
} // end of "from_bytes(const vector<uint8_t>&)"


template <typename T>
vector<uint8_t> ByteConverter::to_bytes(T value)
{
    // Int
    if constexpr (std::is_same_v<T, int>)
        return int_to_bytes(value);
    // Float
    else if constexpr (std::is_same_v<T, float>)
        return float_to_bytes(value);
    // Double
    else if constexpr (std::is_same_v<T, double>)
        return double_to_bytes(value);
    // String
    else if constexpr (std::is_same_v<T, string>)
        return string_to_bytes(value);
    // Bytes (just return value since it's already bytes)
    else if constexpr (std::is_same_v<T, vector<uint8_t>>)
        return value;
    // Everything else
    else
        return {};

} // end of "to_bytes(T)"