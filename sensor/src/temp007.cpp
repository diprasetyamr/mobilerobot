#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "ros/ros.h"
#include "sensor/temp.h"

int main(int argc, char **argv) 
{
    ros::init(argc, argv, "temp007");
    ros::NodeHandle n;
    ros::Publisher temp007 = n.advertise<sensor::temp>("temp007",1000);
    ros::Rate loop_rate(10);
    sensor::temp abc;

	char config[3] = {0};
        config[0] = 0x02;
        config[1] = 0x15;
        config[2] = 0x40;

	// Create I2C bus
	int fh;
	char *bus = "/dev/i2c-1";
	if ((fh = open(bus, O_RDWR)) < 0)
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, TMP007 I2C address is 0x41(65)
	ioctl(fh, I2C_SLAVE, 0x40);

	write(fh, config, 3);
        sleep(1);

    	ioctl(fh, I2C_SLAVE, 0x41);

	write(fh, config, 3);
        sleep(1);

    	ioctl(fh, I2C_SLAVE, 0x44);

	write(fh, config, 3);
        sleep(1);

    	ioctl(fh, I2C_SLAVE, 0x45);

	write(fh, config, 3);
        sleep(1);

	char reg[1] = {0x03};

    while (ros::ok())
    {
	    // Read 2 bytes of data from register(0x03)
	    // temp msb, temp lsb
	    ioctl(fh, I2C_SLAVE, 0x40);
	    write(fh, reg, 1);
	    char data[2] = {0};
	    if(read(fh, data, 2) != 2)
	    {
		    printf("Erorr : Input/output Erorr \n");
	    }
	    else
	    {
		    // Convert the data to 14-bits
		    int temp1 = (data[0] * 256 + (data[1] & 0xFC)) / 4;
		    if(temp1 > 8191)
		    {
			    temp1 -= 16384;
		    }
		    float cTemp1 = temp1 * 0.03125;
		    float fTemp1 = cTemp1 * 1.8 + 32;
            abc.FL = cTemp1;
	    }
	    ioctl(fh, I2C_SLAVE, 0x41);
	    write(fh, reg, 1);
	    char data2[2] = {0};
	    if(read(fh, data2, 2) != 2)
	    {
		    printf("Erorr : Input/output Erorr \n");
	    }
	    else
	    {
		    // Convert the data to 14-bits
		    int temp2 = (data2[0] * 256 + (data2[1] & 0xFC)) / 4;
		    if(temp2 > 8191)
		    {
			    temp2 -= 16384;
		    }
		    float cTemp2 = temp2 * 0.03125;
		    float fTemp2 = cTemp2 * 1.8 + 32;
            abc.FR = cTemp2;
	    }
            ioctl(fh, I2C_SLAVE, 0x44);
	    write(fh, reg, 1);
	    char data3[2] = {0};
	    if(read(fh, data3, 2) != 2)
	    {
		    printf("Erorr : Input/output Erorr \n");
	    }
	    else
	    {
		    // Convert the data to 14-bits
		    int temp3 = (data3[0] * 256 + (data3[1] & 0xFC)) / 4;
		    if(temp3 > 8191)
		    {
			    temp3 -= 16384;
		    }
		    float cTemp3 = temp3 * 0.03125;
		    float fTemp3 = cTemp3 * 1.8 + 32;
            abc.RL = cTemp3;
	    }
            ioctl(fh, I2C_SLAVE, 0x45);
	    write(fh, reg, 1);
	    char data4[2] = {0};
	    if(read(fh, data4, 2) != 2)
	    {
		    printf("Erorr : Input/output Erorr \n");
	    }
	    else
	    {
		    // Convert the data to 14-bits
		    int temp4 = (data4[0] * 256 + (data4[1] & 0xFC)) / 4;
		    if(temp4 > 8191)
		    {
			    temp4 -= 16384;
		    }
		    float cTemp4 = temp4 * 0.03125;
		    float fTemp4 = cTemp4 * 1.8 + 32;
            abc.RR = cTemp4;
	    }
    temp007.publish(abc);
    ros::spinOnce();
    loop_rate.sleep();
    }
}
