#define MOVEMENT_H
#ifndef MOVEMENT_H

class location {
    private:
    double xPos;
    double yPos;
    double zPos;
    double zRot;

    void setXPos(double xIn);
    void setYPos(double yIn);
    void setZPos(double zIn);
    void setZRot(double zRotIn);

    
    public:
    location(double xIn = 0, double yIn = 0, double zIn = 0, double zRotIn = 0);

    const double getXPos();
    const double getYPos();
    const double getZPos();
    const double getZRot();

    void moveX(double xIn);
    void moveY(double yIn);
    void moveZ(double zIn);
    void rotateZ(double zRotIn);
};

#endif //MOVEMENT_H