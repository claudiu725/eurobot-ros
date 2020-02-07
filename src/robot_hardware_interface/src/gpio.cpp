#include "Motor.h"
#include "Encoder.h"
#include <ros/ros.h>
#include <ros/console.h>
#include <pigpio.h>
#include <boost/algorithm/string.hpp>

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
    ros::NodeHandle n;
    int rate, queueSize;
    n.param<int>("/gpio/params/queue_size", queueSize, 10) ;
    n.param<int>("/gpio/params/rate", rate, 20);
    std::vector<std::string> params, motorNames, encoderNames;
    n.getParamNames(params);
    std::vector<std::string> parts;
    std::set<std::string> devices;
    for (auto param : params)
    {
        boost::trim_if(param, boost::is_any_of("/"));
        boost::split(parts, param, boost::is_any_of("/"));
        if (parts[0] == "gpio")
        {
            devices.insert(parts[1]);
        }
    }
    for (const auto &device : devices)
    {
        std::string type;
        n.getParam("/gpio/" + device + "/type", type);
        if (type == "motor")
        {
            motorNames.push_back(device);
        }
        else if (type == "encoder")
        {
            encoderNames.push_back(device);
        }
        else
        {
            ROS_FATAL_STREAM("Invalid type for " << device);
            return 1;
        }
    }

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
    stop(encoders);
    gpioTerminate();
    return 0;
}
