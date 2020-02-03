#include "Motor.h"
#include <pigpio.h>

void Motor::initParams(ros::NodeHandle& n, const std::string &motorName, int queueSize)
{
    name = motorName;
    n.getParam(name + "/pin_direction", pinDirection);
    n.getParam(name + "/pin_pwm", pinPWM);
    n.getParam(name + "/pin_power", pinPower);
    n.getParam(name + "/topic", topic);
    motorCmdSubscriber = n.subscribe(
        topic
        , queueSize
        , &Motor::motorCmdCallback
        , this
    );
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

void resetPin(const int pin)
{
    gpioWrite(pin, 0);
}

void initOutputPin(const int pin)
{
    gpioSetMode(pin, PI_OUTPUT);
    resetPin(pin);
}

void Motor::initHardware()
{
    initOutputPin(pinDirection);
    initOutputPin(pinPWM);
    initOutputPin(pinPower);
}

void Motor::motorCmdCallback(const std_msgs::Float32::ConstPtr& msg)
{
    const float velocity = msg->data;
    gpioWrite(pinDirection, getDirection(velocity));
    gpioPWM(pinPWM, getPWM(velocity));
    gpioWrite(pinPower, getPower(velocity));
}

void Motor::stop()
{
    resetPin(pinDirection);
    resetPin(pinPWM);
    resetPin(pinPower);
}
