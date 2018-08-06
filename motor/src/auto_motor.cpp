#include <wiringPi.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <math.h>
#include "ros/ros.h"
#include "motor/coordinate.h"

#define MODE1 0x00                      //Mode  register  1
#define MODE2 0x01                      //Mode  register  2
#define SUBADR1 0x02            //I2C-bus subaddress 1
#define SUBADR2 0x03            //I2C-bus subaddress 2
#define SUBADR3 0x04            //I2C-bus subaddress 3
#define ALLCALLADR 0x05     //LED All Call I2C-bus address
#define LED0 0x6                        //LED0 start register
#define LED0_ON_L 0x6           //LED0 output and brightness control byte 0
#define LED0_ON_H 0x7           //LED0 output and brightness control byte 1
#define LED0_OFF_L 0x8          //LED0 output and brightness control byte 2
#define LED0_OFF_H 0x9          //LED0 output and brightness control byte 3
#define LED_MULTIPLYER 4        // For the other 15 channels
#define ALLLED_ON_L 0xFA    //load all the LEDn_ON registers, byte 0 (turn 0-7 $
#define ALLLED_ON_H 0xFB        //load all the LEDn_ON registers, byte 1 (turn $
#define ALLLED_OFF_L 0xFC       //load all the LEDn_OFF registers, byte 0 (turn$
#define ALLLED_OFF_H 0xFD       //load all the LEDn_OFF registers, byte 1 (turn$
#define PRE_SCALE 0xFE          //prescaler for output frequency
#define CLOCK_FREQ 25000000.0 //25MHz default osc clock

#define DIR4    25
#define DIR3    0
#define DIR2    15
#define DIR1    16

#define PI 3.14

float vx, vy, vz;
int dir1, dir2, dir3, dir4;
int pwm1, pwm2, pwm3, pwm4;
float rpm1,rpm2,rpm3,rpm4;

int device;
char buf[10] = {0};

void callback(const motor::coordinate::ConstPtr& msg)
{
  vx = 0.4*(msg->x);
  vy = 0.4*(msg->y);
  vz = 0.5*(msg->z);

	rpm1 = ((60.0*43.0)/(0.05*2*PI))*((vx - vy + ((0.23 + 0.17)*vz))/2);
        rpm2 = ((60.0*43.0)/(0.05*2*PI))*((vx + vy - ((0.23 + 0.17)*vz))/2);
        rpm3 = ((60.0*43.0)/(0.05*2*PI))*((vx + vy + ((0.23 + 0.17)*vz))/2);
        rpm4 = ((60.0*43.0)/(0.05*2*PI))*((vx - vy - ((0.23 + 0.17)*vz))/2);

	if(rpm1 < 0)
        {
                dir1 = 0;
        }
        else
        {
                dir1 = 1;
        }
        if(rpm2 < 0)
        {
                dir2 = 1;
        }
        else
        {
                dir2 = 0;
        }
        if(rpm3 < 0)
        {
                dir3 = 0;
        }
        else
        {
                dir3 = 1;
        }
        if(rpm4 < 0)
        {
                dir4 = 1;
        }
	else
        {
                dir4 = 0;
        }

	pwm1 = round((2047.0*abs(rpm1))/5800.0);
        pwm2 = round((2047.0*abs(rpm2))/5800.0);
        pwm3 = round((2047.0*abs(rpm3))/5800.0);
        pwm4 = round((2047.0*abs(rpm4))/5800.0);

	digitalWrite(DIR1,dir1);
        digitalWrite(DIR2,dir2);
        digitalWrite(DIR3,dir3);
        digitalWrite(DIR4,dir4);


	//Setting PWM  Motor 1.................................................$

        buf[0] = LED0_ON_L + (0*4);
        buf[1] = 2047 - pwm1 & 0xFF;
        if (write(device,buf,2) != 2)
        {
                printf("Failed to write to the i2c bus.\n");
        }

        buf[0] = LED0_ON_H + (0*4);
        buf[1] = 2047 - pwm1 >> 8;
        if (write(device,buf,2) != 2)
        {
                printf("Failed to write to the i2c bus.\n");
        }

        buf[0] = LED0_OFF_L + (0*4);
        buf[1] = 2048 + pwm1 & 0xFF;
        if (write(device,buf,2) != 2)
        {
                printf("Failed to write to the i2c bus.\n");
        }

        buf[0] = LED0_OFF_H + (0*4);
        buf[1] = 2048 + pwm1 >> 8;
        if (write(device,buf,2) != 2)
        {
                printf("Failed to write to the i2c bus.\n");
        }

        //.................................................................

	//Setting PWM Motor 2..................................................$

        buf[0] = LED0_ON_L + (4*4);
        buf[1] = 2047 - pwm2 & 0xFF;
        if (write(device,buf,2) != 2)
        {
                printf("Failed to write to the i2c bus.\n");
        }

        buf[0] = LED0_ON_H + (4*4);
        buf[1] = 2047 - pwm2 >> 8;
        if (write(device,buf,2) != 2)
        {
                printf("Failed to write to the i2c bus.\n");
        }

        buf[0] = LED0_OFF_L + (4*4);
        buf[1] = 2048 + pwm2 & 0xFF;
        if (write(device,buf,2) != 2)
        {
                printf("Failed to write to the i2c bus.\n");
        }

        buf[0] = LED0_OFF_H + (4*4);
        buf[1] = 2048 + pwm2 >> 8;
        if (write(device,buf,2) != 2)
        {
                printf("Failed to write to the i2c bus.\n");
        }

        //.................................................................

	//Setting PWM Motor 3..................................................$

        buf[0] = LED0_ON_L + (8*4);
        buf[1] = 2047 - pwm3 & 0xFF;
        if (write(device,buf,2) != 2)
        {
                printf("Failed to write to the i2c bus.\n");
        }

        buf[0] = LED0_ON_H + (8*4);
        buf[1] = 2047 - pwm3 >> 8;
        if (write(device,buf,2) != 2)
        {
                printf("Failed to write to the i2c bus.\n");
        }

        buf[0] = LED0_OFF_L + (8*4);
        buf[1] = 2048 + pwm3 & 0xFF;
        if (write(device,buf,2) != 2)
        {
                printf("Failed to write to the i2c bus.\n");
        }

        buf[0] = LED0_OFF_H + (8*4);
        buf[1] = 2048 + pwm3 >> 8;
        if (write(device,buf,2) != 2)
        {
                printf("Failed to write to the i2c bus.\n");
        }

        //.................................................................

	//Setting PWM Motor 4..................................................$

        buf[0] = LED0_ON_L + (12*4);
        buf[1] = 2047 - pwm4 & 0xFF;
        if (write(device,buf,2) != 2)
        {
                printf("Failed to write to the i2c bus.\n");
        }

        buf[0] = LED0_ON_H + (12*4);
        buf[1] = 2047 - pwm4 >> 8;
        if (write(device,buf,2) != 2)
        {
                printf("Failed to write to the i2c bus.\n");
        }

        buf[0] = LED0_OFF_L + (12*4);
        buf[1] = 2048 + pwm4 & 0xFF;
        if (write(device,buf,2) != 2)
        {
                printf("Failed to write to the i2c bus.\n");
        }

        buf[0] = LED0_OFF_H + (12*4);
        buf[1] = 2048 + pwm4 >> 8;
        if (write(device,buf,2) != 2)
        {
                printf("Failed to write to the i2c bus.\n");
        }

        //.................................................................
}

int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "auto_motor");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("coor", 1000, callback);

  //Initialization...............................................

  printf("Opening device...");
  if ((device = open("/dev/i2c-1", O_RDWR)) < 0)
  {
        perror("open() failed");
        exit (1);
  }
  printf(" OK\n");

  if (ioctl(device, I2C_SLAVE, 0x40) < 0)
  {
        perror("Failed to acquire bus access and/or talk to slave\n");
        exit(1);
  }

  //...............................................................

  //Reset Device...................................................

  buf[0] = MODE1;
  buf[1] = 0x00;
  if (write(device,buf,2) != 2)
  {
        printf("Failed to write to the i2c bus.\n");
  }

  buf[0] = MODE2;
  buf[1] = 0x04;
  if (write(device,buf,2) != 2)
  {
        printf("Failed to write to the i2c bus.\n");
  }

  //.................................................................

  //Setting FREQ ....................................................
  uint8_t prescale_val = (CLOCK_FREQ / (4096 * 1000)) - 1;

  buf[0] = MODE1;
  buf[1] = 0x10;
  if (write(device,buf,2) != 2) {
    printf("Failed to write to the i2c bus.\n");
  }

  buf[0] = PRE_SCALE;
  buf[1] = prescale_val;
  if (write(device,buf,2) != 2) {
    printf("Failed to write to the i2c bus.\n");
  }

  buf[0] = MODE1;
  buf[1] = 0x80;
  if (write(device,buf,2) != 2) {
    printf("Failed to write to the i2c bus.\n");
  }

  buf[0] = MODE2;
  buf[1] = 0x04;
  if (write(device,buf,2) != 2) {
    printf("Failed to write to the i2c bus.\n");
  }

  //.................................................................

  wiringPiSetup();

  pinMode (DIR1, OUTPUT);
  pinMode (DIR2, OUTPUT);
  pinMode (DIR3, OUTPUT);
  pinMode (DIR4, OUTPUT);

  ros::spin();

  return 0;
}
