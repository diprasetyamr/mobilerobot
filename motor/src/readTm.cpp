#include "ros/ros.h"
#include "motor/tempM.h"
#include "motor.h"


int main(int argc, char **argv)
{
        ros::init(argc,argv,"tempM");
        ros::NodeHandle n;
        ros::Publisher temppub;
        temppub = n.advertise<motor::temp>("tempMot",10);
        ros::Rate loop_rate(100);

        motor::tempM tempM;
        motors motorC;

        while(ros::ok())
        {
                tempM.tempM.clear();
                for(int i = 1; i<5; i++)
                {
                        tempM.tempM.push_back(motorC.readMtemp(i));
                }

                temppub.publish(tempM);
                ros::spinOnce();
                loop_rate.sleep();
        }
        return 0;
}
