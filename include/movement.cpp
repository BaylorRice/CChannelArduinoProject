#include "movement.h"
#include <Arduino.h>

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

void Location::moveX(double xIn) {
    double currentX = getXPos();
    double newX = currentX + xIn;
}

void Location::moveYfor(double time, int speed, int dir) {
    // Speed Check
    if (speed < 0) {
        speed = -speed;
    }
    if (speed > 255) {
        speed = 255;
    }

    // Start Motor with speed and direction
    if (dir > 0) {
        analogWrite(Y_DC_EN, speed);
        digitalWrite(Y_DC_IN1, HIGH);
        digitalWrite(Y_DC_IN2, LOW);
        delay(time);
        digitalWrite(Y_DC_IN1, LOW);
        digitalWrite(Y_DC_IN2, LOW);
    } 
    else {

    }
    
    // Wait <time> milliseconds
    delay(time);

    // Stop Motor

}

void Location::moveZ(double zIn) {
    double currentZ = getZPos();
    double newZ = currentZ + zIn;
}

void Location::rotateZ(double zRotIn) {
    double currentZrot = getZRot();
    double newZrot = currentZrot + zRotIn;
    if (newZrot > 360) {
        newZrot = 360 - newZrot;
    }
    if (newZrot < 0) {
        newZrot = 360 + newZrot;
    }

}
