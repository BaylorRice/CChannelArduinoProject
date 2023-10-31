#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "setup.h"

class Location {
    private:
    double xPos;
    double yPos;
    bool zUp;
    double zRot;

    public:
    Location(double xIn = 0, double yIn = 0, bool zIn = false, double zRotIn = 0);

    void setXPos(double xIn);
    void setYPos(double yIn);
    void setZUp(bool zIn);
    void setZRot(double zRotIn);

    double getXPos();
    double getYPos();
    bool getZUp();
    double getZRot();

    void moveX(double xIn);
    void moveYfor(int time, int speed, int dir);
    void moveYto(bool PLL);
    void moveZ(bool zIn);
    void rotateZto(double zRotIn);
};

class Claw {
    private:
    bool grabbed;
    int angle;

    public:
    Claw(bool grabIn = false, int angleIn = 0);

    void setGrab(bool grabIn);
    bool getGrab();

    void open();
    void close();
};

#endif //MOVEMENT_H