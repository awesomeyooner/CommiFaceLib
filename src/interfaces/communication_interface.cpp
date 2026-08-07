#include "CommiFaceLib/interfaces/communication_interface.hpp"


using namespace status_utils;
using namespace std;


vector<uint8_t> CommunicationInterface::create_packet(uint8_t reg, const vector<uint8_t>& data)
{
    // 0: register
    // 1: length
    // 2... Data

    uint8_t length = data.size() + 2; // 1 for register, 1 for length

    std::vector<uint8_t> packet;
    packet.resize(length);

    // Put `data` in the end of the vector
    std::copy(data.begin(), data.end(), packet.begin() + 2);

    packet.at(0) = reg;
    packet.at(1) = length;

    return packet;

} // end of "create_packet(uint8_t, const vector<uint8_t>&)"