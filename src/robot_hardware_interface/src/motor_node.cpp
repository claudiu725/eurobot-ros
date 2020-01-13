#include "ros/ros.h"
#include "std_msgs/Int16.h"

#include <pigpio.h>

int pinForward = 0, pinBackward = 0, pinOnOff = 0;

uint8_t toByte(const float value)
{
    if (value > 0)
    {
        return value * 255;
    }
    return 0;
}

void motorCmdCallback(const std_msgs::Float32::ConstPtr& msg)
{
    const float vel = msg->data;
    gpioPWM(pinFormward, toByte(vel));
    gpioPWM(pinBackward, toByte(-vel));
    gpioWrite(pinOnOff, vel != 0.0);
}

void initOutputPin(const int pin)
{
    gpioSetMode(pin, PI_OUTPUT);
    gpioWrite(pin, 0);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "motor");
    ros::NodeHandle n("~");
    n.getParam("pin_formward", pinFormward);
    n.getParam("pin_backward", pinBackward);
    n.getParam("pin_on_off", pinOnOff);

    initOutputPin(pinFormward);
    initOutputPin(pinBackward);
    initOutputPin(pinOnOff);

    int queueSize;
    n.param<int>("queue_size", queueSize, 10);
    ros::Subscriber motorCmdLeft = n.subscribe("motor_cmd", queueSize, motorCmdCallback);
    ros::spin();
    return 0;
}

