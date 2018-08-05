#include "ros/ros.h"
#include "mobile_robot/path.h"

mobile_robot::path paths;

void callback1(const mobile_robot::path::ConstPtr& path)
{
  paths = *path;
}


int main(int argc, char **argv)
{
	ros::init(argc,argv,"looper");
	ros::NodeHandle n;
	ros::Subscriber pathsub;
  ros::Publisher pathpub;
  pathpub = n.advertise<mobile_robot::path>("path",10);
  pathsub = n.subscribe("paths", 10, &callback1);
  ros::Rate loop_rate(100);
	while(ros::ok())
	{
    paths.header.stamp = ros::Time::now();
		pathpub.publish(paths);
		ros::spinOnce();
    loop_rate.sleep();
	}
	return 0;
}
