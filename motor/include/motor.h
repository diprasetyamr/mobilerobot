#ifndef MOTORS_MOTORS_H
#define MOTORS_MOTORS_H

#include <fcntl.h>
#include <string>
#include <cstdio>
#include <termios.h>
#include <cstring>
#include <cerrno>
#include <zconf.h>
#include <cmath>

#define PI 3.14

class motors {

public:
    motors();
    void enable();
    void disable();
    void move(float spe1, float spe2, float spe3, float spe4);
    void forward(float sped);
    void backward(float sped);
    void left(float sped);
    void right(float sped);
    void rotate(float sped);
    void diagonal(float ang, float sped);
    int readENC(int id);
    int readMtemp(int id);
    int readTvel(int id);
    int readAvel(int id);
    int readHtemp(int id);
private:
    void open_port();
    void init();
    int readCurr(int id);
    int device;
    int spe1p, spe2p, spe3p, spe4p;
    float rpm1,rpm2,rpm3,rpm4;
    std::string buff;
};


#endif //MOTORS_MOTORS_H
