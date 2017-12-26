#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

namespace ric_interface
{
    namespace protocol
    {
        enum class Type
        {
            KEEP_ALIVE = 100,
            LOGGER = 101,
            ULTRASONIC = 102,
            LASER = 103,
            IMU = 104,
            GPS = 105,
            SERVO = 106,
            EMERGENCY_ALARM = 107
        };

        const uint16_t MAX_PKG_SIZE = 512;
        const uint8_t HEADER_CODE = 200;
        const uint8_t HEADER_INDX = 0;
        const uint8_t PKG_SIZE_INDX = 1;

        struct package
        {
            uint8_t type = 0;
        };

        struct keepalive : package
        {
            keepalive() { type = (uint8_t)Type::KEEP_ALIVE; }
        };

        struct logger : package
        {
            logger() { type = (uint8_t)Type::LOGGER; }
            char msg[30];
            int32_t value = 0;
        };

        struct sensor : package
        {

        };

        struct actuator : package
        {
        };

        struct emergency_alarm : sensor
        {
            emergency_alarm() { type = (uint8_t)Type::EMERGENCY_ALARM; }
            bool is_on = false;
        };

        struct ultrasonic : sensor
        {
            ultrasonic() { type = (uint8_t)Type::ULTRASONIC; }
            uint16_t distance_mm = 0;
        };

        struct laser : sensor
        {
            laser() { type = (uint8_t)Type::LASER; }
            uint16_t distance_mm = 0;
        };

        struct imu : sensor
        {
            imu() { type = (uint8_t)Type::IMU; }
            float roll_rad = 0,
                    pitch_rad = 0,
                    yaw_rad = 0,
                    accl_x_rad = 0,
                    accl_y_rad = 0,
                    accl_z_rad = 0,
                    gyro_x_rad = 0,
                    gyro_y_rad = 0,
                    gyro_z_rad = 0,
                    mag_x_rad = 0,
                    mag_y_rad = 0,
                    mag_z_rad = 0;
        };

        struct gps : sensor
        {
            gps() { type = (uint8_t)Type::GPS; }
            float lat = 0,
                    lon = 0;

            float speed = 0,
                    angle = 0;

            float elevation = 0;

            /* E, N, S, W */
            char lat_mark = 0,
                    lon_mark = 0;

            uint8_t hour = 0,
                    minute = 0,
                    seconds = 0,
                    year = 0,
                    month = 0,
                    day = 0;

            uint8_t fix_quality = 0,
                    satellites = 0;

            bool fix = false;
        };


        struct servo : actuator
        {
            servo() { type = (uint8_t)Type::SERVO; }
            uint16_t cmd = 0; //servo command 1000-2000
        };
    }
}



#endif //PROTOCOL_H