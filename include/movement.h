#define MOVEMENT_H
#ifndef MOVEMENT_H

class location {
    private:
    double xPos;
    double yPos;
    double zPos;
    double zRot;

    public:
    location(double xIn, double yIn, double zIn, double zRotIn);
    
};

#endif //MOVEMENT_H