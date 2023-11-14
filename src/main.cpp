/// Constants
#include <AbleButtons.h>
#include <Arduino.h>
#include <NewPing.h>
#include <Servo.h>
#include <Stepper.h>

/// Define component pins
// Buttons
// TO DO: Update Button pins
const int GREEN_START_BTN_PIN = 6;
const int GOLD_START_BTN_PIN = 5;

// Ultrasonic Sensors
const int TRIG_PIN_GREEN = 3;
const int ECHO_PIN_GREEN = 4;
const int TRIG_PIN_GOLD = 8;
const int ECHO_PIN_GOLD = 9;
const int TRIG_PIN_PLL = 48;
const int ECHO_PIN_PLL = 49;
// SENSOR CONFIG
const int MAX_DISTANCE = 400;
const int NUM_PINGS = 15;

// DC Motor (Negative side of motor into Out1, Postitive into Out2)
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
const double MIL_PER_STEP = 0.19;
const double DEG_PER_STEP = 1.8;
const int STEPS_PER_REVOLUTION = 200;
const int SPEED = 200;

// Realspace Locations
const double GREEN_CASE_XPOS = 9999;  // CHANGE
const double GOLD_CASE_XPOS = -9999;  // CHANGE
const double CASE_YPOS = 9999;        // CHANGE
const double MIDDLE_XPOS = 0;         // CHANGE

// Servo Motors
// TO DO: Update servo pins
const int SERVO_GRAB_PIN = 9;
const int SERVO_LIFT_PIN = 9996;
// SERVO CONFIG
int SERVO_GRAB_CLOSED_DEG = 180;
int SERVO_GRAB_OPEN_DEG = 50;  // Open deg must be smaller than closed deg
int SERVO_LIFT_MIN = 0;
int SERVO_LEFT_MAX = 180;

// Limit Switchs
// TO DO: Update limit switch pins
const int LIMIT_SWITCH_PLL_PIN = 28;
const int LIMIT_SWITCH_CASE_PIN = 29;

// Button setup
using Button = AblePulldownCallbackClickerButton;
using ButtonList = AblePulldownCallbackClickerButtonList;
Button greenStart(GREEN_START_BTN_PIN);
Button goldStart(GOLD_START_BTN_PIN);
Button *btns[] = {&greenStart, &goldStart};
ButtonList btnList(btns);
enum possibleColors { EMPTY_COL, GREEN_COL, GOLD_COL };

// Sensor Setup
NewPing sonarGreen(TRIG_PIN_GREEN, ECHO_PIN_GREEN, MAX_DISTANCE);
NewPing sonarGold(TRIG_PIN_GOLD, ECHO_PIN_GOLD, MAX_DISTANCE);
NewPing sonarPLL(TRIG_PIN_PLL, ECHO_PIN_PLL, MAX_DISTANCE);

// Stepper setup
Stepper xStep(STEPS_PER_REVOLUTION, X_STEP_IN1, X_STEP_IN2, X_STEP_IN3,
              X_STEP_IN4);
Stepper zStep(STEPS_PER_REVOLUTION, Z_STEP_IN1, Z_STEP_IN2, Z_STEP_IN3,
              Z_STEP_IN4);

// Servo setup
Servo gServo;
Servo zServo;

// Movement Classes
class Location {
 private:
  double xPos;
  double yPos;
  bool zUp;
  double zRot;

  void setXPos(double xIn) { xPos = xIn; }
  void setYPos(double yIn) { yPos = yIn; }
  void setZUp(bool zIn) { zUp = zIn; }
  void setZRot(double zRotIn) { zRot = zRotIn; }

 public:
  Location(double xIn = 0, double yIn = 0, bool zIn = false,
           double zRotIn = 0) {
    xPos = xIn;
    yPos = yIn;
    zUp = zIn;
    zRot = zRotIn;
    Serial.print("Location Object Initialized!\n");
  }

  double getXPos() {
    Serial.print("Got XPos = ");
    Serial.print(xPos);
    Serial.print("\n");
    return xPos;
  }
  double getYPos() {
    Serial.print("Got YPos = ");
    Serial.print(yPos);
    Serial.print("\n");
    return yPos;
  }
  bool getZUp() {
    Serial.print("Got ZPos = ");
    Serial.print(zUp);
    Serial.print("\n");
    return zUp;
  }
  double getZRot() {
    Serial.print("Got ZRot = ");
    Serial.print(zRot);
    Serial.print("\n");
    return zRot;
  }

  void moveXto(double xIn) {
    Serial.print("Moving X to -> ");
    Serial.print(xIn);
    Serial.print("\n");

    double distanceMoved = xIn - getXPos();
    int stepsToMove = distanceMoved / MIL_PER_STEP;
    xStep.step(stepsToMove);
    setXPos(xIn);

    Serial.print("X Moved\n");
  }

  void moveYfor(int time, int speed, int dir) {
    Serial.print("Moving Y for -> ");
    Serial.print(time);
    Serial.print(" milliseconds\n");

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
    } else {
      // Move away from PLL
      digitalWrite(Y_DC_IN1, LOW);
      digitalWrite(Y_DC_IN2, HIGH);
    }

    // Wait <time> milliseconds (with limit switch bump stopping)
    for (int i = 0; i < time; i++) {
      delay(1);
      if (digitalRead(LIMIT_SWITCH_PLL_PIN) == HIGH ||
          digitalRead(LIMIT_SWITCH_CASE_PIN) == HIGH) {
        break;
      }
    }

    // Stop Motor
    digitalWrite(Y_DC_IN1, LOW);
    digitalWrite(Y_DC_IN2, LOW);

    // TODO: Update yPos to reflect movement (may not be possible)
    // setYPos(getYPos() + calculated delta)

    Serial.print("Moved Y\n");
  }
  void moveYto(bool PLL) {
    Serial.print("Moving Y to -> ");
    Serial.print(PLL);
    Serial.print(": 1 for PLL\n");

    if (PLL) {
      // Move Towards PLL
      digitalWrite(Y_DC_IN1, HIGH);
      digitalWrite(Y_DC_IN2, LOW);

      while (digitalRead(LIMIT_SWITCH_PLL_PIN) != HIGH) {
        delay(1);
      }

      // Stop Motor
      digitalWrite(Y_DC_IN1, LOW);
      digitalWrite(Y_DC_IN2, LOW);
    } else {
      // Move Towards Case
      digitalWrite(Y_DC_IN1, LOW);
      digitalWrite(Y_DC_IN2, HIGH);

      while (digitalRead(LIMIT_SWITCH_CASE_PIN) != HIGH) {
        delay(1);
      }

      // Stop Motor
      digitalWrite(Y_DC_IN1, LOW);
      digitalWrite(Y_DC_IN2, LOW);
    }

    Serial.print("Moved Y\n");
  }

  void moveZup(bool up) {
    Serial.print("Moving Z up to ->");
    Serial.print(up);
    Serial.print("\n");

    // TODO Update degree values and delay
    zServo.write(180 * !up);
    delay(1000);
    setZUp(up);

    Serial.print("Z moved\n");
  }
  void rotateZto(double zRotIn) {
    Serial.print("Rotating Z to ->");
    Serial.print(zRotIn);
    Serial.print("\n");

    double currentZrot = getZRot();
    double moveAngle = currentZrot - zRotIn;
    int moveSteps = moveAngle / DEG_PER_STEP;
    zStep.step(moveSteps);
    setZRot(zRotIn);

    Serial.print("Z Rotated\n");
  }
};

class Claw {
 private:
  bool grabbed;
  int angle;

  void setGrab(bool grabIn) { grabbed = grabIn; }
  void setAngle(int angleIn) { angle = angleIn; }

 public:
  Claw(bool grabIn = true, int angleIn = 0) {
    grabbed = grabIn;
    angle = angleIn;
  }

  bool getGrab() {
    Serial.print("Got Claw Grab = ");
    Serial.print(grabbed);
    Serial.print("\n");
    return grabbed;
  }
  int getAngle() {
    Serial.print("Got Claw Angle = ");
    Serial.print(angle);
    Serial.print("\n");
    return angle;
  }

  void open() {
    if (SERVO_GRAB_CLOSED_DEG < SERVO_GRAB_OPEN_DEG) {
      Serial.print(
          "ERROR: Grab Servo open()- Closed Deg must be larger than Open "
          "Deg\n");
    }
    if (getGrab() == true) {
      Serial.print("GrabServo - Opening\n");
      for (int pos = SERVO_GRAB_CLOSED_DEG; pos >= SERVO_GRAB_OPEN_DEG; pos--) {
        gServo.write(pos);
        delay(15);
      }
      setGrab(false);
      Serial.print("GrabServo -> OPEN\n");
    }
  }
  void close() {
    if (SERVO_GRAB_CLOSED_DEG < SERVO_GRAB_OPEN_DEG) {
      Serial.print(
          "ERROR: Grab Servo closed()- Closed Deg must be larger than Open "
          "Deg\n");
    }
    if (getGrab() == false) {
      Serial.print("GrabServo - Closing\n");
      for (int pos = SERVO_GRAB_OPEN_DEG; pos <= SERVO_GRAB_CLOSED_DEG; pos++) {
        gServo.write(pos);
        delay(15);
      }
      setGrab(true);
      Serial.print("GrabServo -> CLOSED\n");
    }
  }
};

// Detection Classes
class Detect {
 private:
  bool caseReady = false;
  bool palletReady = false;
  bool buttonReady = false;

 public:
  // default constructor
  Detect(bool caseIn = false, bool palletIn = false, bool buttonIn = false) {
    caseReady = caseIn;
    palletReady = palletIn;
    buttonReady = buttonIn;
  }

  // gets and sets
  bool getCaseReady(void) {
    Serial.print("Get Case Ready = ");
    Serial.print(caseReady);
    Serial.print("\n");
    return caseReady;
  }
  bool getPalletReady(void) {
    Serial.print("Get Pallet Ready = ");
    Serial.print(palletReady);
    Serial.print("\n");
    return palletReady;
  }
  bool getButtonReady(void) {
    Serial.print("Get Button Ready = ");
    Serial.print(buttonReady);
    Serial.print("\n");
    return buttonReady;
  }
  void setCaseReady(bool sensorData) { caseReady = sensorData; }
  void setPalletReady(bool sensorData) { palletReady = sensorData; }
  void setButtonReady(bool data) { buttonReady = data; }

  // detect functions
  void caseDetect(NewPing *selection) {
    Serial.print("Detecting Case...\n");
    int time, distance;

    time = selection->ping_median(NUM_PINGS);
    distance = selection->convert_cm(time);

    if (distance < 5) {
      setCaseReady(true);
      Serial.print("Result -> Case ready\n");
    } else {
      setCaseReady(false);
      Serial.print("Result -> Not Case ready\n");
    }
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print("\n");
  }
  void palletDetect() {
    int time, distance;
    time = sonarPLL.ping_median(NUM_PINGS);
    distance = sonarPLL.convert_cm(time);
    // FIXME need correct distance range
    if (distance < 10 && distance > 5) {
      setPalletReady(true);
    } else {
      setPalletReady(false);
    }
  }
  possibleColors detectPress(bool data = true) {
    possibleColors selection = EMPTY_COL;
    Serial.print("Waiting for Button...\n");
    while (data) {
      btnList.handle();
      delay(5);
      if (greenStart.resetClicked()) {
        Serial.print("Button Pressed - Green\n");
        selection = GREEN_COL;
        setButtonReady(false);
      }
      if (goldStart.resetClicked()) {
        Serial.print("Button Pressed - Gold\n");
        selection = GOLD_COL;
        setButtonReady(false);
      }
    }
    return selection;
  }
};

/// Setup
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

// Other Class Definitions
Location loc;
Claw claw;
Detect detection;

/// Main Driver
void loop() {
  /// Testing DC, Limit Switches, and Buttons

  possibleColors runColor = EMPTY_COL;

  // Detect Button Press
  runColor = detection.detectPress();

  if (runColor == GREEN_COL) {
    loc.moveYto(true);
  } else if (runColor == GOLD_COL) {
    loc.moveYto(false);
  }
}
/*
possibleColors startingColor = EMPTY_COL;
  possibleColors nextColor = EMPTY_COL;
  NewPing *caseSonarPtr = NULL;
  double caseXPos = -1;
  int colorCount = 0;
  int runCount = 0;

  // Button Press -> Constants
  startingColor = detection.detectPress();

  for (runCount = 0; runCount < 2; runCount++) {
    if (startingColor == GREEN_COL) {
      caseSonarPtr = &sonarGreen;
      caseXPos = GREEN_CASE_XPOS;
      nextColor = GOLD_COL;
    } else if (startingColor == GOLD_COL) {
      caseSonarPtr = &sonarGold;
      caseXPos = GOLD_CASE_XPOS;
      nextColor = GREEN_COL;
    } else {
      Serial.print("ERROR: Constant Setting -> No constants set");
      // TODO Loop Stop
    }

    for (colorCount = 0; colorCount < 4; colorCount++) {
      // Move to Case x
      loc.moveXto(caseXPos);

      // Wait for Case
      detection.setCaseReady(false);
      while (!detection.getCaseReady()) {
        detection.caseDetect(caseSonarPtr);
        delay(15);
      }

      // Move to Case in y dir
      loc.moveYto(false);

      // Lower to Case
      loc.moveZup(false);

      // Grab with Claw
      claw.close();

      // Upsies
      loc.moveZup(true);

      // Move Back
      loc.moveYfor(250, 255, 1);
      // Move for 2.5 seconds at full speed towards the PLL

      // Move to Middle
      loc.moveXto(GREEN_CASE_XPOS);

      // Rotate to PLL
      loc.rotateZto(180);

      // Detect PLL-readiness
      detection.setPalletReady(false);
      while (!detection.getPalletReady()) {
        detection.palletDetect();
      }

      // Move to PLL
      loc.moveYto(true);

      // Open Claw
      claw.open();

      // Move y to middle
      loc.moveYfor(250, 255, -1);

      // Rotate Z
      loc.rotateZto(0);
    }
    startingColor = nextColor;
  }
  */