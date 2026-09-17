#ifndef CAN_HPP
#define CAN_HPP


#include "plib/util/logger.hpp"

#include "CommiFaceLib/interfaces/communication_interface.hpp"

#include <string>

// CANbus includes
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

// https://docs.kernel.org/networking/can.html
class CAN : public CommunicationInterface
{

    public:

        static status_utils::StatusCode open(std::string interface_name = "can0", bool verbose = true);

        static status_utils::StatusCode close_socket();

        CAN(int ID);

        status_utils::StatusCode transmit_bytes(const std::vector<uint8_t>& bytes) override;

        status_utils::StatusedValue<std::vector<uint8_t>> receive_bytes(int num_bytes, int timeout_ms = -1) override;
        
    private:

        static constexpr int MAX_DATA_SIZE = 8; // bytes

        static inline sockaddr_can m_address;

        static inline ifreq m_ifr;

        static inline int m_socket = -1;

        can_frame m_transmit_frame;

        can_frame m_receive_frame;


}; // class CAN : public CommunicationInterface


#endif // CAN_HPP