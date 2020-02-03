#include "Encoder.h"
#include <std_msgs/Int16.h>

Encoder::Encoder() : encoder(), publisher()
{
}

void Encoder::initParams(ros::NodeHandle& n, const std::string &encoderName, int rate, int queueSize)
{
    name = encoderName;
    n.getParam(name + "/pinA", pinA);
    n.getParam(name + "/pinB", pinB);
    n.getParam(name + "/topic", topic);
    publisher = n.advertise<std_msgs::Int16>(topic, queueSize);
    timer = n.createTimer(ros::Duration(1.0 / rate), &Encoder::publish, this);
}

void encoderCallback(void *encoder, int diff)
{
    static_cast<Encoder*>(encoder)->encoderValue += diff;
}

void Encoder::initHardware()
{
    encoder.reset(new re_decoder(pinA, pinB, &encoderCallback, this));
}

void Encoder::stop()
{
    if (encoder)
    {
        encoder->re_cancel();
    }
}

void Encoder::publish(const ros::TimerEvent&)
{
    std_msgs::Int16 message;
    message.data = encoderValue;
    publisher.publish(message);
}
