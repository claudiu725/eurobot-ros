#include "ros/ros.h"
#include "std_msgs/Int16.h"

#include "rotary_encoder.hpp"

int16_t encoderValue = 0;

void encoderCallback(int diff)
{
    encoderValue += diff;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "encoder");

    ros::NodeHandle n("~");
    int pinA, pinB, rate;
    n.getParam("pinA", pinA);
    n.getParam("pinB", pinB);
    re_decoder encoder(pinA, pinB, encoderCallback);

    int queueSize;
    n.param<int>("queue_size", queueSize, 10);
    ros::Publisher publisher = n.advertise<std_msgs::Int16>("encoder_value", queueSize);

    n.param<int>("rate", rate, 10);
    ros::Rate loopRate(rate);
    while (ros::ok())
    {
        std_msgs::Int16 message;
        message.data = encoderValue;
        publisher.publish(message);
        ros::spinOnce();
        loopRate.sleep();
    }
    encoder.re_cancel();
    return 0;
}
