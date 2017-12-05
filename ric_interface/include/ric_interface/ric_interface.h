//
// Created by Eli Eli on 18/11/2017.
//

#ifndef RIC_INTERFACE_BOARD_MANAGER_H
#define RIC_INTERFACE_BOARD_MANAGER_H

#include "communicator.h"
#include "timer.h"
#include "protocol.h"
#include "sensors_state.h"
#include <string.h>

namespace ric_interface
{
    class RicInterface
    {
    private:
        const int SEND_KA_TIMEOUT = 300; //ms
        const int GET_KA_TIMEOUT = 2000; //ms

        /* is board sent keep alive on time */
        bool is_board_alive_, got_keepalive_;
        Timer send_keepalive_timer_, get_keepalive_timer_;
        Communicator comm_;
        sensors_state sensors_state_;

        bool readHeader(protocol::header &h);
        void sendKeepAlive();
        void handleHeader(const protocol::header &h);
        void keepAliveAndRead();
        bool readLoggerPkg(protocol::logger &logger_pkg);
        bool readUltrasonicPkg(protocol::ultrasonic &ultrasonic_pkg);
        bool readImuPkg(protocol::imu &imu_pkg);
        bool readLaserPkg(protocol::laser &laser_pkg);

    public:
        RicInterface();
        void connect(std::string port);
        void loop();
        bool isBoardAlive() { return is_board_alive_; }
        sensors_state getSensorsState() { return sensors_state_; }

    };
}



#endif //RIC_INTERFACE_BOARD_MANAGER_H
