#include "ros/ros.h"
#include "mobile_robot/path.h"
#include "mobile_robot/coordinate.h"
#include "mobile_robot/ultrasonic.h"
#include "geometry_msgs/PoseStamped.h"
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>

ros::Publisher pub;
mobile_robot::coordinate velo;

void callback(const geometry_msgs::PoseStamped::ConstPtr& odoms, const mobile_robot::path::ConstPtr& path)
{
        double t1,t2,yaw1,yaw2,targetang,errorang,error1,error2,dists;
        double targetpos[2];
        double errorpos[2];
        double curpos[2];
        int angreached, posreached, errorsel;
        error1 = 0;
        error2 = 0;
        t1 = +2.0 * (((odoms->pose.orientation.w) * (odoms->pose.orientation.z)) + ((odoms->pose.orientation.x) * (odoms->pose.orientation.y)));
        t2 = +1.0 - 2.0 * (((odoms->pose.orientation.y) * (odoms->pose.orientation.y)) + ((odoms->pose.orientation.z) * (odoms->pose.orientation.z)));
        yaw1 = atan2(t1, t2);

        //Correction Angle
        targetang = atan2((path->paths.y - odoms->pose.position.y),(path->paths.x - odoms->pose.position.x));
        error1 = targetang - yaw1;

        //printf("ANGERROR: %f\n",error1);

        if(error1 < -3.1416)
        {
		         error2 = 6.2832 - abs(error1);
		         errorang = error2;
        }
        else if(error1 > 3.1416)
        {
		        error2 = -6.2832 + error1;
		          errorang = error2;
        }
	       else
	       {
		          errorang = error1;
	       }

	      velo.x = 0;
        velo.z = 0;
        velo.y = 0;

        targetpos[0] = path->paths.x;
        targetpos[1] = path->paths.y;
        curpos[0] = odoms->pose.position.x;
        curpos[1] = odoms->pose.position.y;

        dists = sqrt(pow((targetpos[0] - curpos[0]),2) + pow((targetpos[1] - curpos[1]),2));

        if(dists>0.3)
        {
          targetpos[0] = curpos[0] + ((targetpos[0]-curpos[0])*0.3)/dists;
          targetpos[1] = curpos[1] + ((targetpos[1]-curpos[1])*0.3)/dists;
        }

          errorpos[0] = targetpos[0] - curpos[0];
          errorpos[1] = targetpos[1] - curpos[1];

          printf("X error: %f, Y error: %f, ANG error: %f\n",errorpos[0], errorpos[1], errorang);
        if(errorang < 0.15 && errorang > -0.15)
        {
		            //velo.x = 0;
                velo.z = 0;
                angreached = 1;
                printf("ZEROING1\n");
        }
        else if(errorpos[0] < 0.05 && errorpos[1] < 0.05)
        {
          if(errorpos[0] > -0.05 && errorpos[1] > -0.05)
          {
            printf("ZEROING2\n");
            velo.x = 0;
            velo.z = 0;
            angreached = 1;
          }
        }
        else
        {
                printf("ZEROING3\n");
                velo.x = 0;
                angreached = 0;
        }

        if(angreached != 1)
        {
                if(errorang > 0.15)
                {
                        printf("ROTATE1\n");
                        velo.z = 40;
                }
                else if(errorang < -0.15)
                {
                        printf("ROTATE2\n");
                        velo.z = -40;
                }
        }

        //Correction Position

        if(angreached == 1)
        {
                if(errorpos[0] < 0.05 && errorpos[1] < 0.05)
                {
                        if(errorpos[0] > -0.05 && errorpos[1] > -0.05)
                        {
                                printf("ZEROING4\n");
                                velo.x = 0;
                                posreached = 1;
                        }
                }
                else
                {
                        posreached = 0;
                }


                if(posreached != 1)
                {
                  if(errorpos[0] < 0)
                  {
                    errorpos[0] = -1*errorpos[0];
                  }
                  if(errorpos[1] < 0)
                  {
                    errorpos[1] = -1*errorpos[1];
                  }
                  if(errorpos[0] > 0.05 || errorpos[1] > 0.05)
                  {
                          printf("FORWARD\n");
                          velo.x = 25;
                  }
                }
        }

        pub.publish(velo);
}



int main(int argc, char **argv)
{
        ros::init(argc, argv, "poscontrol");

        //counter = 0;
        ros::NodeHandle n;
        pub = n.advertise<mobile_robot::coordinate>("coor",1000);
        message_filters::Subscriber<geometry_msgs::PoseStamped> odom(n, "slam_out_pose", 1000);
        message_filters::Subscriber<mobile_robot::path> path(n, "path", 1000);
        typedef message_filters::sync_policies::ApproximateTime<geometry_msgs::PoseStamped, mobile_robot::path> MySyncPolicy;
        message_filters::Synchronizer<MySyncPolicy> sync(MySyncPolicy(1000), odom, path);

        sync.registerCallback(boost::bind(&callback, _1, _2));

        while (ros::ok())
        {
                ros::spinOnce();
        }

        return 0;
}
