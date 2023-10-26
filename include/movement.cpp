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

double Location::getXPos() {
    return xPos;
}

double Location::getYPos() {
    return yPos;
}

double Location::getZPos() {
    return zPos;
}

double Location::getZRot() {
    return zRot;
}

void Location::moveX(double xIn) {
    double currentX = getXPos();
    double newX = currentX + xIn;
    // TO DO: Adjust value for more precise x movement
    xStep.step(/*NUMBER OF STEPS*/);
}

void Location::moveYfor(double time, int speed, int dir) {
    // Speed Check
    if (speed < 0) {
        speed = -speed;
    }
    if (speed > 255) {
        speed = 255;
    }

    analogWrite(Y_DC_EN, speed);

    // Start Motor with speed and direction
    if (dir > 0) {
        // Move Towards PLL
        digitalWrite(Y_DC_IN1, HIGH);
        digitalWrite(Y_DC_IN2, LOW);
    } 
    else {
        // Move away from PLL
        digitalWrite(Y_DC_IN1, LOW);
        digitalWrite(Y_DC_IN2, HIGH);
        

    }
    
    // Wait <time> milliseconds
    delay(time);

    // Stop Motor
    digitalWrite(Y_DC_IN1, LOW);
    digitalWrite(Y_DC_IN2, LOW);

}

void Location::moveZ(double zIn) {
    double currentZ = getZPos();
    double newZ = currentZ + zIn;
    // TO DO: Adjust value for more precise z-lift movement
    zServo.write(newZ /*Maybe change to be an encoded number*/);
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
    // TO DO: Adjust value for more precise z-rotation movement
    zStep.step(/*NUMBER OF STEPS*/);

}

void Claw::setGrab(bool grabIn) {
    grabbed = grabIn;
}

bool Claw::getGrab() {
    return grabbed;
}

void Claw::open() {
    if (getGrab() == true) {
        // SERVO -> OPEN
    }
}

void Claw::close() {
    if (getGrab() == false) {
        // SERVO -> CLOSE
    }
}
