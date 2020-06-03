#ifndef ENCODER_H_
#define ENCODER_H_

#include "rotary_encoder.hpp"
#include <ros/ros.h>
#include <memory>

class Encoder
{
public:
    Encoder();
    void initParams(ros::NodeHandle& n, const std::string &encoderName, int rate, int queueSize);
    void initHardware();
    void stop();
private:
    void publish(const ros::TimerEvent&);
private:
    std::shared_ptr<re_decoder> encoder;
    ros::Publisher publisher;
    ros::Timer timer;
    int16_t encoderValue = 0;
    int pinA = 0, pinB = 0, rate = 0;
    bool reverse = false;
    std::string name;
    std::string topic;

    friend void encoderCallback(void *encoder, int diff);
};

#endif // ENCODER_H_
