#include "ros/ros.h"
#include "motor/temp.h"
#include "motor.h"


int main(int argc, char **argv)
{
	ros::init(argc,argv,"temp");
	ros::NodeHandle n;
  ros::Publisher temppub;
  temppub = n.advertise<motor::temp>("tempMot",10);
  ros::Rate loop_rate(100);

  motor::temp tempM;
  motors motorC;

	while(ros::ok())
	{

    for(int i = 1; i<5; i++)
    {
      tempM.tempM1 = motorC.readMtemp(i);
    }

    temppub.publish(tempM);
		ros::spinOnce();
    loop_rate.sleep();
	}
	return 0;
}
