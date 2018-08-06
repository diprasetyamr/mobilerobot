#include <fcntl.h>
#include <string>
#include <cstdio>
#include <termios.h>
#include <cstring>
#include <cerrno>
#include <zconf.h>
#include <math.h>
#include "motor.h"
#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "motor/coordinate.h"

#define PI 3.14

float vx, vy, vz;
float rpm1,rpm2,rpm3,rpm4;
int spe1, spe2, spe3, spe4;
motors motorC;

std::string buff;

void speedCallback(const motor::coordinate::ConstPtr& msg)
{
        vx = msg->x;
        vy = msg->y;
        vz = msg->z;
        
	if(vx < 0)
	{
		if(vx > -0.2)
		{
			vx = 0;
		}
		if(vx < -0.6)
		{
			vx = -0.6;
		}
	}
	else
	{
		if(vx < 0.2)
		{
			vx = 0;
		}
		if(vx > 0.6)
		{
			vx = 0.6;
		}
	}

	if(vy < 0)
	{
		if(vy > -0.2)
		{
			vy = 0;
		}
		if(vy < -0.6)
		{
			vy = -0.6;
		}
	}
	else
	{
		if(vy < 0.2)
		{
			vy = 0;
		}
		if(vy > 0.6)
		{
			vy = 0.6;
		}
	}

	if(vz < 0)
	{
		if(vz > -0.3)
		{
			vz = 0;
		}
		if(vz < -1.5)
		{
			vz = -1.5;
		}
	}
	else
	{
		if(vz < 0.3)
		{
			vz = 0;
		}
		if(vz > 1.5)
		{
			vz = 1.5;
		}
	}

	rpm3 = ((60.0*43.0)/(0.05*2*PI))*((vx + vy + ((0.23 + 0.17)*vz))/2);
        rpm4 = ((60.0*43.0)/(0.05*2*PI))*((vx - vy - ((0.23 + 0.17)*vz))/2);
        rpm1 = ((60.0*43.0)/(0.05*2*PI))*((vx - vy + ((0.23 + 0.17)*vz))/2);
        rpm2 = ((60.0*43.0)/(0.05*2*PI))*((vx + vy - ((0.23 + 0.17)*vz))/2);

	spe1 = rpm1;
	spe2 = -rpm2;
	spe3 = rpm3;
	spe4 = -rpm4;

	if(spe1 < -5800)
	{
		spe1 = -5800;
	}
	if(spe2 < -5800)
        {
                spe2 = -5800;
        }
	if(spe3 < -5800)
        {
                spe3 = -5800;
        }
	if(spe4 < -5800)
        {
                spe4 = -5800;
        }

	if(spe1 > 5800)
        {
                spe1 = 5800;
        }
        if(spe2 > 5800)
        {
                spe2 = 5800;
        }
        if(spe3 > 5800)
        {
                spe3 = 5800;
        }
	if(spe4 > 5800)
	{
		spe4 = 5800;
	}

	motorC.move(spe1,spe2,spe3,spe4);
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "motorController");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("coor", 1000, speedCallback);

  motorC.enable();

  ros::spin();

  return 0;
}
