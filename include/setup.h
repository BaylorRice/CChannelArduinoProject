#define SETUP_H
#ifndef SETUP_H

#include <AbleButtons.h>
#include <NewPing.h>
#include <Stepper.h>
#include <Servo.h>

// Define component pins
// TO DO: Update Button pins
// Buttons
#define GREEN_START_BTN_PIN 9999
#define GOLD_START_BTN_PIN 9998
// Ultrasonic Sensors
#define TRIG_PIN_GREEN 52
#define ECHO_PIN_GREEN 53
#define TRIG_PIN_GOLD 50
#define ECHO_PIN_GOLD 51
#define TRIG_PIN_PLL 48
#define ECHO_PIN_PLL 49
#define MAX_DISTANCE 400
#define NUM_PINGS 15
// DC Motor
#define Y_DC_IN1 22
#define Y_DC_IN2 23
#define Y_DC_EN 2
// Stepper motors
#define X_STEP_IN1 33
#define X_STEP_IN2 35
#define X_STEP_IN3 37
#define X_STEP_IN4 39
#define Z_STEP_IN1 38
#define Z_STEP_IN2 36
#define Z_STEP_IN3 34
#define Z_STEP_IN4 32
#define STEPS_PER_REVOLUTION 200
#define SPEED 200
// Servo Motors
// TO DO: Update servo pins
#define SERVO_GRAB 9997
#define SERVO_LIFT 9996
int servo_g_pos = 0;
int servo_l_pos = 0;

// Button setup
using Button = AblePulldownCallbackClickerButton;
using ButtonList = AblePulldownCallbackClickerButtonList;
Button greenStart(GREEN_START_BTN_PIN);
Button goldStart(GOLD_START_BTN_PIN);
Button *btns[] = {&greenStart, &goldStart};
ButtonList btnList(btns);

// Initialize sensors
NewPing sonarGreen(TRIG_PIN_GREEN, ECHO_PIN_GREEN, MAX_DISTANCE);
NewPing sonarGold(TRIG_PIN_GOLD, ECHO_PIN_GOLD, MAX_DISTANCE);
NewPing sonarPLL(TRIG_PIN_PLL, ECHO_PIN_PLL, MAX_DISTANCE);

// Initialize Steppers
Stepper xStep(STEPS_PER_REVOLUTION, X_STEP_IN1, X_STEP_IN2, X_STEP_IN3, X_STEP_IN4);
Stepper zStep(STEPS_PER_REVOLUTION, Z_STEP_IN1, Z_STEP_IN2, Z_STEP_IN3, Z_STEP_IN4);



void setup() {
    Serial.begin(9600);
    // Buttons
    btnList.begin();
    pinMode(Y_DC_IN1)
}

#endif //SETUP_H