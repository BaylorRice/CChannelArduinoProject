#include "movement.h"
#include <Arduino.h>

Location::Location(double xIn, double yIn, bool zIn, double zRotIn) {
    xPos = xIn;
    yPos = yIn;
    zUp = zIn;
    zRot = zRotIn;
}

void Location::setXPos(double xIn) {
    xPos = xIn;
}

void Location::setYPos(double yIn) {
    yPos = yIn;
}

void Location::setZUp(bool zIn) {
    zUp = zIn;
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

bool Location::getZUp() {
    return zUp;
}

double Location::getZRot() {
    return zRot;
}

void Location::moveX(double xIn) {
    double currentX = getXPos();
    double newX = currentX + xIn;
    double distanceMoved = newX - currentX;
    int stepsToMove = distanceMoved / MIL_PER_STEP;
    xStep.step(stepsToMove);
}

void Location::moveYfor(int time, int speed, int dir) {
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
    
    // Wait <time> milliseconds (with limit switch bump stopping)
    for (int i = 0; i < time; i++) {
        delay(1);
        if (digitalRead(LIMIT_SWITCH_PLL_PIN) == HIGH|| digitalRead(LIMIT_SWITCH_CASE_PIN) == HIGH) {
            break;
        }
    }
    

    // Stop Motor
    digitalWrite(Y_DC_IN1, LOW);
    digitalWrite(Y_DC_IN2, LOW);

}

void Location::moveYto(bool PLL) {
    if (PLL) {
        // TODO: Refine this

        // Move Towards PLL
        digitalWrite(Y_DC_IN1, HIGH);
        digitalWrite(Y_DC_IN2, LOW);

        while (digitalRead(LIMIT_SWITCH_PLL_PIN) == LOW) {
            delay(1);
        }

        // Stop Motor
        digitalWrite(Y_DC_IN1, LOW);
        digitalWrite(Y_DC_IN2, LOW);
    }
}

void Location::moveZ(bool zIn) {
    // TODO Update degree values
    if (!getZUp() && (zIn == true)) {
        // Servo to UP / TRUE
        zServo.write(0);
        setZUp(true);
    } else if (getZUp() && (zIn == false)){
        // Servo to DOWN / FALSE
        zServo.write(180);
        setZUp(false);
    }
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

Claw::Claw(bool grabIn) {
    grabbed = grabIn;
}

void Claw::setGrab(bool grabIn)
{
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
