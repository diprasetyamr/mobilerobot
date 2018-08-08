#include "motor.h"

motors::motors()
{
								motors::init();
}

void motors::init()
{
								motors::open_port();

								struct termios options;
								tcgetattr(motors::device, &options);

								cfsetispeed( &options, B9600 );
								cfsetospeed( &options, B9600 );

								options.c_cflag |= ( CLOCAL | CREAD );

								options.c_cflag &= ~CSIZE;
								options.c_cflag |= CS8;

								options.c_cflag &= ~PARENB;
								options.c_cflag &= ~CSTOPB;
								options.c_cflag &= ~CSIZE;
								options.c_cflag |= CS8;

								options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

								options.c_iflag &= ~(IXON | IXOFF | IXANY);

								options.c_oflag &= ~OPOST;

								if ( tcsetattr( motors::device, TCSANOW, &options ) == -1 )
																printf ("Error with tcsetattr = %s\n", strerror ( errno ) );

								fcntl(motors::device, F_SETFL, FNDELAY);
}

void motors::open_port()
{
								motors::device = open("/dev/ttyUSB2", O_RDWR | O_NOCTTY | O_NDELAY);
								if(motors::device == -1)
								{
																perror("open_port: Unable to open /dev/ttyUSB0 - ");
								}
								else
								{
																fcntl(motors::device, F_SETFL, FNDELAY);
																printf("PORT OPENED\n");
								}
}

void motors::enable()
{
								printf("ENABLING\n");

								if (write(motors::device, "1EN\n", 4) < 0)
																fputs("write() of 4 bytes failed!\n", stderr);
								else
																printf ("Write succeed 1\n");

								if (write(motors::device, "2EN\n", 4) < 0)
																fputs("write() of 4 bytes failed!\n", stderr);
								else
																printf ("Write succeed 2\n");

								if (write(motors::device, "3EN\n", 4) < 0)
																fputs("write() of 4 bytes failed!\n", stderr);
								else
																printf ("Write succeed 3\n");

								if (write(motors::device, "4EN\n", 4) < 0)
																fputs("write() of 4 bytes failed!\n", stderr);
								else
																printf ("Write succeed 4\n");
}

void motors::disable()
{
								printf("DISABLING\n");

								if (write(motors::device, "1DI\n", 4) < 0)
																fputs("write() of 4 bytes failed!\n", stderr);
								else
																printf ("Write succeed 1\n");

								if (write(motors::device, "2DI\n", 4) < 0)
																fputs("write() of 4 bytes failed!\n", stderr);
								else
																printf ("Write succeed 2\n");

								if (write(motors::device, "3DI\n", 4) < 0)
																fputs("write() of 4 bytes failed!\n", stderr);
								else
																printf ("Write succeed 3\n");

								if (write(motors::device, "4DI\n", 4) < 0)
																fputs("write() of 4 bytes failed!\n", stderr);
								else
																printf ("Write succeed 4\n");
}

void motors::forward(float sped)
{
								if(sped > 1)
								{
																sped = 1;
								}
								if(sped < 0)
								{
																sped = 0;
								}

								rpm3 = ((60.0*43.0)/(0.05*2*PI))*((sped)/2);
								rpm4 = ((60.0*43.0)/(0.05*2*PI))*((-sped)/2);
								rpm1 = ((60.0*43.0)/(0.05*2*PI))*((-sped)/2);
								rpm2 = ((60.0*43.0)/(0.05*2*PI))*((sped)/2);

								motors::move(rpm1, rpm2, rpm3, rpm4);
}

void motors::backward(float sped)
{
								if(sped > 1)
								{
																sped = 1;
								}
								if(sped < 0)
								{
																sped = 0;
								}

								rpm3 = ((60.0*43.0)/(0.05*2*PI))*((-sped)/2);
								rpm4 = ((60.0*43.0)/(0.05*2*PI))*((sped)/2);
								rpm1 = ((60.0*43.0)/(0.05*2*PI))*((sped)/2);
								rpm2 = ((60.0*43.0)/(0.05*2*PI))*((-sped)/2);

								motors::move(rpm1, rpm2, rpm3, rpm4);
}

void motors::left(float sped)
{
								if(sped > 1)
								{
																sped = 1;
								}
								if(sped < 0)
								{
																sped = 0;
								}

								rpm3 = ((60.0*43.0)/(0.05*2*PI))*((-sped)/2);
								rpm4 = ((60.0*43.0)/(0.05*2*PI))*((-sped)/2);
								rpm1 = ((60.0*43.0)/(0.05*2*PI))*((-sped)/2);
								rpm2 = ((60.0*43.0)/(0.05*2*PI))*((-sped)/2);

								motors::move(rpm1, rpm2, rpm3, rpm4);
}

void motors::right(float sped)
{
								if(sped > 1)
								{
																sped = 1;
								}
								if(sped < 0)
								{
																sped = 0;
								}

								rpm3 = ((60.0*43.0)/(0.05*2*PI))*((sped)/2);
								rpm4 = ((60.0*43.0)/(0.05*2*PI))*((sped)/2);
								rpm1 = ((60.0*43.0)/(0.05*2*PI))*((sped)/2);
								rpm2 = ((60.0*43.0)/(0.05*2*PI))*((sped)/2);

								motors::move(rpm1, rpm2, rpm3, rpm4);
}

void motors::diagonal(float ang, float sped)
{
}

void motors::rotate(float sped)
{
								if(sped > 1)
								{
																sped = 1;
								}
								if(sped < 0)
								{
																sped = 0;
								}

								rpm3 = ((60.0*43.0)/(0.05*2*PI))*((((0.23 + 0.17)*sped))/2);
								rpm4 = ((60.0*43.0)/(0.05*2*PI))*((((0.23 + 0.17)*sped))/2);
								rpm1 = ((60.0*43.0)/(0.05*2*PI))*((((0.23 + 0.17)*sped))/2);
								rpm2 = ((60.0*43.0)/(0.05*2*PI))*((((0.23 + 0.17)*sped))/2);

								motors::move(rpm1, rpm2, rpm3, rpm4);
}

void motors::move(float sped1, float sped2, float sped3, float sped4)
{
								char c1[] = "1V";
								char c2[] = "2V";
								char c3[] = "3V";
								char c4[] = "4V";

								int spe1 = sped1;
								int spe2 = sped2;
								int spe3 = sped3;
								int spe4 = sped4;

								if(spe1 != spe1p)
								{
																buff = std::to_string(spe1);
																strcat(c1,buff.c_str());
																strcat(c1,"\n");

																if (write(motors::device, c1, strlen(c1)) < 0)
																								fputs("write() of 4 bytes failed!\n", stderr);
								}

								if(spe2 != spe2p)
								{
																buff = std::to_string(spe2);
																strcat(c2,buff.c_str());
																strcat(c2,"\n");

																if (write(motors::device, c2, strlen(c2)) < 0)
																								fputs("write() of 4 bytes failed!\n", stderr);
								}

								if(spe3 != spe3p)
								{
																buff = std::to_string(spe3);
																strcat(c3,buff.c_str());
																strcat(c3,"\n");

																if (write(motors::device, c3, strlen(c3)) < 0)
																								fputs("write() of 4 bytes failed!\n", stderr);
								}

								if(spe4 != spe4p)
								{
																buff = std::to_string(spe4);
																strcat(c4,buff.c_str());
																strcat(c4,"\n");

																if (write(motors::device, c4, strlen(c4)) < 0)
																								fputs("write() of 4 bytes failed!\n", stderr);
								}

								spe1p = spe1;
								spe2p = spe2;
								spe3p = spe3;
								spe4p = spe4;
}

int motors::readENC(int id)
{
								char cc[] = "POS";
								char cc2[10];
								char cc3[100];
								motors::buff = std::to_string(id);
								strcpy(cc2,motors::buff.c_str());
								strcat(cc2,cc);
								strcat(cc2,"\n");

								usleep(25000);

								if (write(motors::device, cc2, strlen(cc2)) < 0)
																fputs("write() of 4 bytes failed!\n", stderr);

								usleep(25000);

								if (read(motors::device, cc3, sizeof cc3) != -1)
																return atoi(cc3);
								else
																printf("Error\n");
}

int motors::readCurr(int id)
{
								char cc[] = "GRC";
								char cc2[10];
								char cc3[100];
								motors::buff = std::to_string(id);
								strcpy(cc2,motors::buff.c_str());
								strcat(cc2,cc);
								strcat(cc2,"\n");

								usleep(25000);

								if (write(motors::device, cc2, strlen(cc2)) < 0)
																fputs("write() of 4 bytes failed!\n", stderr);

								usleep(25000);

								if (read(motors::device, cc3, sizeof cc3) != -1)
																return atoi(cc3);
								else
																printf("Error\n");
}

int motors::readAvel(int id)
{
								char cc[] = "GN";
								char cc2[10];
								char cc3[100];
								motors::buff = std::to_string(id);
								strcpy(cc2,motors::buff.c_str());
								strcat(cc2,cc);
								strcat(cc2,"\n");

								usleep(25000);

								if (write(motors::device, cc2, strlen(cc2)) < 0)
																fputs("write() of 4 bytes failed!\n", stderr);

								usleep(25000);

								if (read(motors::device, cc3, sizeof cc3) != -1)
																return atoi(cc3);
								else
																printf("Error\n");
}

int motors::readTvel(int id)
{
								char cc[] = "GV";
								char cc2[10];
								char cc3[100];
								motors::buff = std::to_string(id);
								strcpy(cc2,motors::buff.c_str());
								strcat(cc2,cc);
								strcat(cc2,"\n");

								usleep(25000);

								if (write(motors::device, cc2, strlen(cc2)) < 0)
																fputs("write() of 4 bytes failed!\n", stderr);

								usleep(25000);

								if (read(motors::device, cc3, sizeof cc3) != -1)
																return atoi(cc3);
								else
																printf("Error\n");
}

int motors::readHtemp(int id)
{
								char cc[] = "TEM";
								char cc2[10];
								char cc3[100];
								motors::buff = std::to_string(id);
								strcpy(cc2,motors::buff.c_str());
								strcat(cc2,cc);
								strcat(cc2,"\n");

								usleep(25000);

								if (write(motors::device, cc2, strlen(cc2)) < 0)
																fputs("write() of 4 bytes failed!\n", stderr);

								usleep(25000);

								if (read(motors::device, cc3, sizeof cc3) != -1)
																return atoi(cc3);
								else
																printf("Error\n");
}

int motors::readMtemp(int id)
{
								int current = motors::readCurr(id)/1000.0;
								float R = 1.46;
								float Rth1 = 4.7;
								float Rth2 = 15.2;
								float power = pow(current,2) * R;

								return (22 + int(power*(Rth1+Rth2)));
}
