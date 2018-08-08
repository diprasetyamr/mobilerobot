#include "ros/ros.h"
#include "motor/motorData.h"
#include "sensor/temp.h"
#include "motor.h"

sensor::temp temps;

void callback1(const sensor::temp::ConstPtr& temp)
{
        temps = *temp;
}

int main(int argc, char **argv)
{
        ros::init(argc,argv,"motData");
        ros::NodeHandle n;
        ros::Publisher motDatpub;
        ros::Subscriber tempSub;
        motDatpub = n.advertise<motor::motorData>("motData",10);
        tempSub = n.subscribe("/temp007", 10, &callback1);
        ros::Rate loop_rate(100);

        motor::motorData motData;
        motors motorC;

        while(ros::ok())
        {
                motData.tempM.clear();
                motData.aspeedM.clear();
                motData.tspeedM.clear();

                motData.tempM.push_back(temps.FL);
                motData.tempM.push_back(temps.FR);
                motData.tempM.push_back(temps.RL);
                motData.tempM.push_back(temps.FR);

                for(int i = 1; i<5; i++)
                {
                        motData.aspeedM.push_back(motorC.readAvel(i));
                }
                for(int i = 1; i<5; i++)
                {
                        motData.tspeedM.push_back(motorC.readTvel(i));
                }

                motDatpub.publish(motData);
                ros::spinOnce();
                loop_rate.sleep();
        }
        return 0;
}
