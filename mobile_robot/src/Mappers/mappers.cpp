#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/PoseStamped.h"
#include <math.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include "mobile_robot/coordinate.h"
#include "mobile_robot/vects.h"

#define PI 3.14159265

mobile_robot::coordinate coor;
mobile_robot::vects vect_maps;
ros::Publisher pub;

void callback(const sensor_msgs::LaserScan::ConstPtr& lasers, const geometry_msgs::PoseStamped::ConstPtr& odoms)
{
  int j = (lasers->ranges).size();
  double t1,t2,yaw;

  vect_maps.maps.clear();
  t1 = +2.0 * (((odoms->pose.orientation.w) * (odoms->pose.orientation.z)) + ((odoms->pose.orientation.x) * (odoms->pose.orientation.y)));
	t2 = +1.0 - 2.0 * (((odoms->pose.orientation.y) * (odoms->pose.orientation.y)) + ((odoms->pose.orientation.z) * (odoms->pose.orientation.z)));
	yaw = atan2(t1, t2);

  for(int i = 270; i<360; i++)
  {
	if(!isinf(lasers->ranges[i]) && lasers->ranges[i] < 2)
	{
		coor.x = odoms->pose.position.x + 0.3299*cos(yaw) + (lasers->ranges[i])*cos(yaw+((i-360)*0.01745));
      		coor.y = odoms->pose.position.y + 0.3299*sin(yaw) + (lasers->ranges[i])*sin(yaw+((i-360)*0.01745));
      		vect_maps.maps.push_back(coor);

	}
  }

  for(int i = 0; i<90; i++)
  {
        if(!isinf(lasers->ranges[i]) && lasers->ranges[i] < 2)
        {
                coor.x = odoms->pose.position.x + 0.3299*cos(yaw) + (lasers->ranges[i])*cos(yaw+((i+1)*0.01745));
                coor.y = odoms->pose.position.y + 0.3299*sin(yaw) + (lasers->ranges[i])*sin(yaw+((i+1)*0.01745));
                vect_maps.maps.push_back(coor);

        }
  }

  vect_maps.odoms = *odoms;
  pub.publish(vect_maps);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "mapper");

    ros::NodeHandle n;
    pub = n.advertise<mobile_robot::vects>("maps",1000);
    message_filters::Subscriber<sensor_msgs::LaserScan> laser(n, "scan", 1000);
    message_filters::Subscriber<geometry_msgs::PoseStamped> odom(n, "slam_out_pose", 1000);
    typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::LaserScan, geometry_msgs::PoseStamped> MySyncPolicy;
    message_filters::Synchronizer<MySyncPolicy> sync(MySyncPolicy(1000), laser, odom);

    sync.registerCallback(boost::bind(&callback, _1, _2));

    while (ros::ok())
    {
      ros::spinOnce();
    }

    return 0;
}
