#include "Motor.h"
#include "Encoder.h"
#include <ros/ros.h>
#include <pigpio.h>

void initParams(ros::NodeHandle &n, const std::vector<std::string>& names, std::vector<Motor> &motors, const int queueSize)
{
    motors.resize(names.size());
    for (size_t i = 0; i < names.size(); ++i)
    {
        motors[i].initParams(n, names[i], queueSize);
    }
}

void initParams(ros::NodeHandle &n, const std::vector<std::string>& names, std::vector<Encoder> &encoders, const int rate, const int queueSize)
{
    encoders.resize(names.size());
    for (size_t i = 0; i < names.size(); ++i)
    {
        encoders[i].initParams(n, names[i], rate, queueSize);
    }
}

template <typename T>
void initHardware(T &collection)
{
    for (auto &item : collection)
    {
        item.initHardware();
    }
}

template <typename T>
void stop(T &collection)
{
    for (auto &item : collection)
    {
        item.stop();
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "gpio");
    ros::NodeHandle n("~");
    int rate, queueSize;
    n.param<int>("queue_size", queueSize, 10) ;
    n.param<int>("rate", rate, 20);
    std::vector<std::string> motorNames, encoderNames;
    n.getParam("motors", motorNames);
    n.getParam("encoders", encoderNames);
    std::vector<Motor> motors;
    std::vector<Encoder> encoders;
    initParams(n, motorNames, motors, queueSize);
    initParams(n, encoderNames, encoders, rate, queueSize);

    if (gpioInitialise() < 0)
    {
        return 1;
    }
    initHardware(motors);
    initHardware(encoders);
    ros::spin();
    stop(motors);
    stop(encoders);
    gpioTerminate();
    return 0;
}
