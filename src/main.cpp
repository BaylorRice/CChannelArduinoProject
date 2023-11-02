#include <AbleButtons.h>
#include <NewPing.h>
#include <Stepper.h>
#include <Servo.h>

//Define component pins
// Buttons
// TO DO: Update Button pins
const int GREEN_START_BTN_PIN = 52;
const int GOLD_START_BTN_PIN = 53;

// Ultrasonic Sensors
const int TRIG_PIN_GREEN = 52;
const int ECHO_PIN_GREEN = 53;
const int TRIG_PIN_GOLD = 50;
const int ECHO_PIN_GOLD = 51;
const int TRIG_PIN_PLL = 48;
const int ECHO_PIN_PLL = 49;
// SENSOR CONFIG
const int MAX_DISTANCE = 400;
const int NUM_PINGS = 15;

// DC Motor
const int Y_DC_IN1 = 22;
const int Y_DC_IN2 = 23;
const int Y_DC_EN = 2;

// Stepper motors
const int X_STEP_IN1 = 33;
const int X_STEP_IN2 = 35;
const int X_STEP_IN3 = 37;
const int X_STEP_IN4 = 39;
const int Z_STEP_IN1 = 38;
const int Z_STEP_IN2 = 36;
const int Z_STEP_IN3 = 34;
const int Z_STEP_IN4 = 32;
// STEPPER CONFIG
const int STEPS_PER_REVOLUTION = 200;
const int SPEED = 200;

// Servo Motors
// TO DO: Update servo pins
const int SERVO_GRAB_PIN = 9997;
const int SERVO_LIFT_PIN = 9996;
// SERVO CONFIG
int SERVO_GRAB_CLOSED_DEG = 100;
int SERVO_LIFT_MIN = 0;

int SERVO_GRAB_OPEN_DEG = 10;
int SERVO_LEFT_MAX = 180;

// Limit Switchs
// TO DO: Update limit switch pins
const int LIMIT_SWITCH_1_PIN = 9995;
const int LIMIT_SWITCH_2_PIN = 9994;

// Button setup
using Button = AblePulldownCallbackClickerButton;
using ButtonList = AblePulldownCallbackClickerButtonList;
Button greenStart(GREEN_START_BTN_PIN);
Button goldStart(GOLD_START_BTN_PIN);
Button *btns[] = {&greenStart, &goldStart};
ButtonList btnList(btns);

// Sensor Setup
NewPing sonarGreen(TRIG_PIN_GREEN, ECHO_PIN_GREEN, MAX_DISTANCE);
NewPing sonarGold(TRIG_PIN_GOLD, ECHO_PIN_GOLD, MAX_DISTANCE);
NewPing sonarPLL(TRIG_PIN_PLL, ECHO_PIN_PLL, MAX_DISTANCE);

// Stepper setup
Stepper xStep(STEPS_PER_REVOLUTION, X_STEP_IN1, X_STEP_IN2, X_STEP_IN3, X_STEP_IN4);
Stepper zStep(STEPS_PER_REVOLUTION, Z_STEP_IN1, Z_STEP_IN2, Z_STEP_IN3, Z_STEP_IN4);

// Servo setup
Servo gServo;
Servo zServo;

void setup() {
    Serial.begin(9600);
    // Buttons
    btnList.begin();
    // DC Motor
    pinMode(Y_DC_IN1, OUTPUT);
    pinMode(Y_DC_IN2, OUTPUT);
    pinMode(Y_DC_EN, OUTPUT);
    digitalWrite(Y_DC_IN1, LOW);
    digitalWrite(Y_DC_IN2, LOW);
    digitalWrite(Y_DC_EN, LOW);
    // Steppers
    xStep.setSpeed(SPEED);
    zStep.setSpeed(SPEED);
    // Servos
    gServo.attach(SERVO_GRAB_PIN);
    gServo.write(SERVO_GRAB_CLOSED_DEG);
    zServo.attach(SERVO_LIFT_PIN);
    zServo.write(SERVO_LIFT_MIN);
}

void loop() {
  // put your main code here, to run repeatedly:
}

/* 
Loop Code from Button Testing

// put your main code here, to run repeatedly:
  btnList.handle();
  detect data;
  NewPing selection(0,0,0);
  bool i = true;

  Serial.println("Start");
  
  while (i) {
    if(greenStart.resetClicked()) {
      Serial.print("Green\n");
      i = false;
    }
    if (goldStart.resetClicked()) {
      Serial.print("Gold\n");
      i = false;
    }

  }
  delay(1000);
  
  

  Serial.println("\nStop");
  delay(1000);

  */