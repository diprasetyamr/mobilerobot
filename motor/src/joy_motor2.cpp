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

#define PI 3.14

float vx, vy, vz;
float rpm1,rpm2,rpm3,rpm4;
int spe1, spe2, spe3, spe4;
motor motorC;

std::string buff;

void joyCallback(const sensor_msgs::Joy::ConstPtr& msg)
{
  vx = -1*0.6*(msg->axes[4]);
  vy = 0.6*(msg->axes[3]);
  vz = -1*(1.5*(msg->axes[2] - msg->axes[5])/2);

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
		if(vz > -0.5)
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
		if(vz < 0.5)
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

	if(spe1 > 0)
	{
		if(spe1 > 0 && spe1 <= 800)
        	{
                	spe1 = 800;
        	}
        	if(spe1 > 800 && spe1 <= 1600)
        	{
                	spe1 = 1600;
        	}
        	if(spe1 > 1600 && spe1 <= 2400)
        	{
                	spe1 = 2400;
        	}
        	if(spe1 > 2400)
        	{
                	spe1 = 3000;
        	}
	}
	else if(spe1 < 0)
	{
		if(spe1 < 0 && spe1 >= -800)
		{
			spe1 = -800;
		}
		if(spe1 < -800 && spe1 >= -1600)
                {
                        spe1 = -1600;
                }
		if(spe1 < -1600 && spe1 >= -2400)
                {
                        spe1 = -2400;
                }
		if(spe1 < -2400)
                {
                        spe1 = -3000;
                }
	}
	else
	{
		spe1 = 0;
	}

	if(spe2 > 0)
        {
                if(spe2 > 0 && spe2 <= 800)
                {
                        spe2 = 800;
                }
                if(spe2 > 800 && spe2 <= 1600)
                {
                        spe2 = 1600;
                }
                if(spe2 > 1600 && spe2 <= 2400)
                {
                        spe2 = 2400;
                }
                if(spe2 > 2400)
                {
                        spe2 = 3000;
                }
        }
        else if(spe2 < 0)
        {
                if(spe2 < 0 && spe2 >= -800)
                {
                        spe2 = -800;
                }
                if(spe2 < -800 && spe2 >= -1600)
                {
                        spe2 = -1600;
                }
                if(spe2 < -1600 && spe2 >= -2400)
                {
                        spe2 = -2400;
                }
                if(spe2 < -2400)
                {
                        spe2 = -3000;
                }
        }
        else
        {
                spe2 = 0;
        }

	if(spe3 > 0)
        {
                if(spe3 > 0 && spe3 <= 800)
                {
                        spe3 = 800;
                }
                if(spe3 > 800 && spe3 <= 1600)
                {
                        spe3 = 1600;
                }
                if(spe3 > 1600 && spe3 <= 2400)
                {
                        spe3 = 2400;
                }
                if(spe3 > 2400)
                {
                        spe3 = 3000;
                }
        }
        else if(spe3 < 0)
        {
                if(spe3 < 0 && spe3 >= -800)
                {
                        spe3 = -800;
                }
                if(spe3 < -800 && spe3 >= -1600)
                {
                        spe3 = -1600;
                }
                if(spe3 < -1600 && spe3 >= -2400)
                {
                        spe3 = -2400;
                }
                if(spe3 < -2400)
                {
                        spe3 = -3000;
                }
        }
        else
        {
                spe3 = 0;
        }

	if(spe4 > 0)
        {
                if(spe4 > 0 && spe4 <= 800)
                {
                        spe4 = 800;
                }
                if(spe4 > 800 && spe4 <= 1600)
                {
                        spe4 = 1600;
                }
                if(spe4 > 1600 && spe4 <= 2400)
                {
                        spe4 = 2400;
                }
                if(spe4 > 2400)
                {
                        spe4 = 3000;
                }
        }
        else if(spe4 < 0)
        {
                if(spe4 < 0 && spe4 >= -800)
                {
                        spe4 = -800;
                }
                if(spe4 < -800 && spe4 >= -1600)
                {
                        spe4 = -1600;
                }
                if(spe4 < -1600 && spe4 >= -2400)
                {
                        spe4 = -2400;
                }
                if(spe4 < -2400)
                {
                        spe4 = -3000;
                }
        }
        else
        {
                spe4 = 0;
        }

	motorC.move(spe1,spe2,spe3,spe4);
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "joy_motor");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("joy", 1000, joyCallback);

  motorC.enable();

  ros::spin();

  return 0;
}
