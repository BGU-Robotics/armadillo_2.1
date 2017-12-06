//
// Created by sub on 31/10/17.
//

#include "battery_pub.h"

namespace armadillo2_hw
{
    BatteryPub::BatteryPub(ros::NodeHandle nh)
    {
        /* get batt params */
        nh.getParam("load_battery_hw", load_battery_hw_);

        if (!load_battery_hw_)
            return;

        if (!nh.hasParam(BATT_PORT_PARAM))
        {
            ROS_ERROR("[armadillo2_hw/battery_pub]: %s param is missing on param server. make sure that this param exist in battery_config.yaml "
                              "and that your launch includes this param file. shutting down...", BATT_PORT_PARAM);
            ros::shutdown();
            exit (EXIT_FAILURE);
        }
        nh.getParam(BATT_PORT_PARAM, batt_port_);
        nh.getParam("low_batt_val", low_batt_val_);
        nh.getParam("show_warnings", show_warnings_);

        /* connect to batt FTDI */
        try
        {
            bms_.connect(batt_port_);
        }
        catch (bms::BMSException exp)
        {
            ROS_ERROR("[armadillo2_hw/battery_pub]: %s", exp.what());
            ros::shutdown();
            exit(EXIT_FAILURE);
        }
        ROS_INFO("[armadillo2_hw/battery_pub]: battery port opened successfully \nport name: %s \nbaudrate: 9600", batt_port_.c_str());

        /* batt publisher */
        bat_pub_ = nh.advertise<sensor_msgs::BatteryState>("battery", 10);
        bat_pub_timer_ = nh.createTimer(ros::Duration(BATT_PUB_INTERVAL), &BatteryPub::pubTimerCB, this);
        ROS_INFO("[armadillo2_hw/battery_pub]: battery publisher is up");
    }

    void BatteryPub::pubTimerCB(const ros::TimerEvent &event)
    {
        try
        {
            bms::data bms_data =  bms_.read();

            sensor_msgs::BatteryState msg;
            msg.present = true;
            msg.voltage = bms_data.vbat;
            msg.percentage = bms_data.soc;
            msg.current = bms_data.chrg_current - bms_data.dchrg_current;
            msg.charge = bms_data.chrg_current;
            msg.capacity = bms_data.cap_full; //Ah
            msg.power_supply_status = bms_data.is_chrg;
            msg.cell_voltage = bms_data.vcells;
            msg.location = "base_link";

            /* if battery low and not in charging print warning */
            if ((low_batt_val_ >=0 && msg.percentage <= low_batt_val_) && !bms_data.is_chrg)
                ROS_WARN("[armadillo2_hw/battery_pub]: LOW BATTERY, please connect Armadillo2 to charger");

            bat_pub_.publish(msg);
        }
        catch(bms::BMSErrorException exp)
        {
            ROS_ERROR("[armadillo2_hw/battery_pub]: %s", exp.what());
            ros::shutdown();
            exit(EXIT_FAILURE);
        }
        catch(bms::BMSWarnException exp)
        {
            if (show_warnings_)
                ROS_WARN("[armadillo2_hw/battery_pub]: %s", exp.what());
        }
    }
}
