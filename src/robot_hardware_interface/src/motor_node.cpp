#include "ros/ros.h"
#include "std_msgs/Float32.h"

#include <pigpio.h>

int pinDirection = 0, pinPWM = 0, pinPower = 0;

uint8_t getDirection(const float velocity)
{
    return velocity > 0;
}

uint8_t getPWM(const float velocity)
{
    return static_cast<uint8_t>(fabs(velocity) * 255);
}

uint8_t getPower(const float velocity)
{
    return velocity != 0.0;
}

void motorCmdCallback(const std_msgs::Float32::ConstPtr& msg)
{
    const float velocity = msg->data;
    gpioWrite(pinDirection, getDirection(velocity));
    gpioPWM(pinPWM, getPWM(velocity));
    gpioWrite(pinPower, getPower(velocity));
}

void resetPin(const int pin)
{
    gpioWrite(pin, 0);
}

void initOutputPin(const int pin)
{
    gpioSetMode(pin, PI_OUTPUT);
    resetPin(pin);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "motor");
    ros::NodeHandle n("~");
    n.getParam("pin_direction", pinDirection);
    n.getParam("pin_pwm", pinPWM);
    n.getParam("pin_power", pinPower);

    if (gpioInitialise() < 0)
    {
        return 1;
    }
    initOutputPin(pinDirection);
    initOutputPin(pinPWM);
    initOutputPin(pinPower);

    int queueSize;
    n.param<int>("queue_size", queueSize, 10);
    ros::Subscriber motorCmdLeft = n.subscribe("motor_cmd", queueSize, motorCmdCallback);
    ros::spin();

    resetPin(pinDirection);
    resetPin(pinPWM);
    resetPin(pinPower);

    gpioTerminate();
    return 0;
}
