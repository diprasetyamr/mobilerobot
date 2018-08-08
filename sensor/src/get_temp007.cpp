#include "ros/ros.h"
#include "sensor/temp.h"

void chatterCallback(const sensor::temp::ConstPtr& msg)
{
  ROS_INFO("FLcTemp: [%f]", msg->FL);
  ROS_INFO("FRcTemp: [%f]", msg->FR);
  ROS_INFO("RLcTemp: [%f]", msg->RL);
  ROS_INFO("RRcTemp: [%f]", msg->RR);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "get_temp");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("temp007", 1000, chatterCallback);
  ros::spin();

  return 0;
}
