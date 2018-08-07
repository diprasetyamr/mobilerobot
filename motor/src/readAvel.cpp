#include "ros/ros.h"
#include "motor/speedM.h"
#include "motor.h"


int main(int argc, char **argv)
{
	ros::init(argc,argv,"avel");
	ros::NodeHandle n;
  ros::Publisher speedpub;
  speedpub = n.advertise<motor::speedM>("avelMot",10);
  ros::Rate loop_rate(100);

  motor::speedM speedM;
  motors motorC;

	while(ros::ok())
	{

		speedM.speedM.clear();
    for(int i = 1; i<5; i++)
    {
      speedM.speedM.push_back(motorC.readAvel(i));
    }

    speedpub.publish(speedM);
		ros::spinOnce();
    loop_rate.sleep();
	}
	return 0;
}
