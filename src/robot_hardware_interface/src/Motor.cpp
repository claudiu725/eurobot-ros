#include "Motor.h"
#include <pigpio.h>

void Motor::initParams(ros::NodeHandle& n, const std::string &motorName, int queueSize)
{
    name = motorName;
    n.param<int>("/gpio/" + name + "/pins/direction", pinDirection, 0);
    n.param<int>("/gpio/" + name + "/pins/direction_not", pinDirection2, 0);
    n.param<int>("/gpio/" + name + "/pins/pwm", pinPWM, 0);
    n.param<int>("/gpio/" + name + "/pins/power", pinPower, 0);
    n.param<std::string>("/gpio/" + name + "/topic", topic, name);
    subscriber = n.subscribe(
        topic
        , queueSize
        , &Motor::command
        , this
    );
}

void initOutputPin(const int pin)
{
    if (pin)
    {
        gpioSetMode(pin, PI_OUTPUT);
        gpioWrite(pin, 0);
    }
}

void Motor::initHardware()
{
    initOutputPin(pinDirection);
    initOutputPin(pinDirection2);
    initOutputPin(pinPWM);
    initOutputPin(pinPower);
}

void writeIf(const int pin, const uint8_t value)
{
    if (pin)
    {
        gpioWrite(pin, value);
    }
}

void pwmIf(const int pin, const uint8_t pwm)
{
    if (pin)
    {
        gpioPWM(pin, pwm);
    }
}

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

void Motor::command(const std_msgs::Float32::ConstPtr& msg)
{
    const float velocity = msg->data;
    writeIf(pinDirection, getDirection(velocity));
    writeIf(pinDirection2, !getDirection(velocity));
    pwmIf(pinPWM, getPWM(velocity));
    writeIf(pinPower, getPower(velocity));
}
