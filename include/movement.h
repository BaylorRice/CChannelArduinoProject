#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "setup.h"

class Location {
    private:
    double xPos;
    double yPos;
    double zPos;
    double zRot;

    public:
    Location(double xIn = 0, double yIn = 0, double zIn = 0, double zRotIn = 0);

    void setXPos(double xIn);
    void setYPos(double yIn);
    void setZPos(double zIn);
    void setZRot(double zRotIn);

    double getXPos();
    double getYPos();
    double getZPos();
    double getZRot();

    void moveX(double xIn);
    void moveYfor(int time, int speed, int dir);
    void moveZ(double zIn);
    void rotateZ(double zRotIn);
};

class Claw {
    private:
    bool grabbed = false;

    public:
    Claw(bool grabIn = false);

    void setGrab(bool grabIn);
    bool getGrab();

    void open();
    void close();
};

#endif //MOVEMENT_H