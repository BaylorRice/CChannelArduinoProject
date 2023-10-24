#include "movement.h"

Location::Location(double xIn, double yIn, double zIn, double zRotIn) {
    xPos = xIn;
    yPos = yIn;
    zPos = zIn;
    zRot = zRotIn;
}

void Location::setXPos(double xIn) {
    xPos = xIn;
}

void Location::setYPos(double yIn) {
    yPos = yIn;
}

void Location::setZPos(double zIn) {
    zPos = zIn;
}

void Location::setZRot(double zRotIn) {
    zRot = zRotIn;
}

const double Location::getXPos() {
    return xPos;
}

const double Location::getYPos() {
    return yPos;
}

const double Location::getZPos() {
    return zPos;
}

const double Location::getZRot() {
    return zRot;
}
