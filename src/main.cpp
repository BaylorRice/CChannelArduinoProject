/// Constants
#include <AbleButtons.h>
#include <Arduino.h>
#include <NewPing.h>
#include <Servo.h>
#include <Stepper.h>

// Logic Flow Definitions
#define SPIN_TOWARDS_GOLD
//#define SPIN_TOWARDS_GREEN

/// Define component pins
// Buttons
const int GREEN_START_BTN_PIN = 11;
const int GOLD_START_BTN_PIN = 12;

// Ultrasonic Sensors
const int TRIG_PIN_GREEN = 7;
const int ECHO_PIN_GREEN = 8;
const int TRIG_PIN_GOLD = 3;
const int ECHO_PIN_GOLD = 4;
const int TRIG_PIN_PLL = 5;
const int ECHO_PIN_PLL = 6;
// SENSOR CONFIG
const int MAX_DISTANCE = 400;
const int NUM_PINGS = 15;

// DC Motor (Negative side of motor into Out1, Postitive into Out2)
const int Y_DC_IN1 = 22;
const int Y_DC_IN2 = 23;
const int Y_DC_EN = 2;

const int DC_SPEED = 100;

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
const double STEP_PER_MIL = 2.975;
const double DEG_PER_STEP = 1.8;
const int STEPS_PER_REVOLUTION = 200;
const int SPEED = 100;
const int ROT_SPEED = 15;

#ifdef SPIN_TOWARDS_GREEN
const int SPIN_DIR = -1;
#endif  // SPIN_TOWARDS_GREEN

#ifdef SPIN_TOWARDS_GOLD
const int SPIN_DIR = 1;
#endif  // SPIN_TOWARDS_GOLD

// Realspace Locations
const double GREEN_CASE_XPOS = 0;
const double GOLD_CASE_XPOS = 106;
// const double CASE_YPOS = 9999;
const double MIDDLE_XPOS = 77;

#ifdef SPIN_TOWARDS_GREEN
const double SPIN_XPOS = 110;
#endif  // SPIN_TOWARDS_GREEN

#ifdef SPIN_TOWARDS_GOLD
const double SPIN_XPOS = 10;
#endif  // SPIN_TOWARDS_GOLD

// Servo Motors
const int SERVO_GRAB_PIN = 9;
const int SERVO_LIFT_PIN = 10;
// SERVO CONFIG
int SERVO_GRAB_CLOSED_DEG = 180;
int SERVO_GRAB_OPEN_DEG = 0;  // Open deg must be smaller than closed deg
int SERVO_LIFT_MIN = 50;
int SERVO_LEFT_MAX = 200;

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
    int stepsToMove = distanceMoved * STEP_PER_MIL;
    Serial.print("Steps Moved ->");
    Serial.print(stepsToMove);
    Serial.print("\n");
    xStep.step(-1 * stepsToMove);
    setXPos(xIn);

    Serial.print("X Moved\n");
  }

  void moveYfor(int time, int speed, bool dirPLL) {
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
    if (dirPLL == true) {
      // Move Towards PLL
      digitalWrite(Y_DC_IN1, HIGH);
      digitalWrite(Y_DC_IN2, LOW);

      for (int i = 0; i < time; i++) {
        delay(1);
        if (digitalRead(LIMIT_SWITCH_PLL_PIN) == LOW) {
          break;
        }
      }
    } else {
      // Move away from PLL
      digitalWrite(Y_DC_IN1, LOW);
      digitalWrite(Y_DC_IN2, HIGH);

      for (int i = 0; i < time; i++) {
        delay(1);
        if (digitalRead(LIMIT_SWITCH_CASE_PIN) == LOW) {
          break;
        }
      }
    }
    // Stop Motor
    digitalWrite(Y_DC_IN1, LOW);
    digitalWrite(Y_DC_IN2, LOW);

    // TODO: Update yPos to reflect movement (may not be possible)
    // setYPos(getYPos() + calculated delta)

    analogWrite(Y_DC_EN, DC_SPEED);
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

      while (digitalRead(LIMIT_SWITCH_PLL_PIN) != LOW) {
        delay(1);
      }

      // Stop Motor
      digitalWrite(Y_DC_IN1, LOW);
      digitalWrite(Y_DC_IN2, LOW);
    } else {
      // Move Towards Case
      digitalWrite(Y_DC_IN1, LOW);
      digitalWrite(Y_DC_IN2, HIGH);

      while (digitalRead(LIMIT_SWITCH_CASE_PIN) != LOW) {
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
    if (up) {
      zServo.write(SERVO_LEFT_MAX);
      delay(2000);
    } else {
      zServo.write(SERVO_LIFT_MIN);
      delay(2000);
    }

    Serial.print("Z moved\n");
  }
  void rotateZto(double zRotIn) {
    Serial.print("Rotating Z to ->");
    Serial.print(zRotIn);
    Serial.print("\n");

    double currentZrot = getZRot();
    double moveAngle = currentZrot - zRotIn;
    int moveSteps = moveAngle / DEG_PER_STEP;
    zStep.step(SPIN_DIR * moveSteps);
    setZRot(zRotIn);

    Serial.print("Z Rotated\n");
  }

  #ifdef SPIN_TOWARDS_GOLD
  void flip(bool PLL) {
    moveXto(SPIN_XPOS);
    if (PLL) {
      moveYfor(500,100,false);
      rotateZto(55);
      moveYto(false);
      rotateZto(171);
    } else if (!PLL) {
      rotateZto(90);
      moveYto(true);
      rotateZto(0);
    }
  }
  #endif //SPIN_TOWARDS_GOLD

  #ifdef SPIN_TOWARDS_GREEN
  void flip(bool PLL) {
    moveXto(SPIN_XPOS);
    if (PLL) {
      rotateZto(90);
      moveYto(false);
      rotateZto(171);
    } else if (!PLL) {
      rotateZto(90);
      moveYto(true);
      rotateZto(0);
    }
  }
  #endif //SPIN_TOWARDS_GREEN
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
    Serial.print("Distance = ");
    Serial.print(distance);
    Serial.print("\n");
    // FIXME need correct distance range
    if (distance < 3) {
      setPalletReady(false);
    } else {
      setPalletReady(true);
    }
  }
  possibleColors detectPress() {
    bool buttonPressed = false;
    possibleColors selection = EMPTY_COL;
    Serial.print("Waiting for Button...\n");
    while (!buttonPressed) {
      btnList.handle();
      delay(5);
      if (greenStart.resetClicked()) {
        Serial.print("Button Pressed - Green\n");
        selection = GREEN_COL;
        buttonPressed = true;
      }
      if (goldStart.resetClicked()) {
        Serial.print("Button Pressed - Gold\n");
        selection = GOLD_COL;
        buttonPressed = true;
      }
    }
    return selection;
  }
};

// Other Class Definitions
Location loc;
Claw claw;
Detect detection;

//#define TEST
#define PROD

#ifdef TEST

void setup() {
  Serial.begin(9600);
  // Buttons
  btnList.begin();
  pinMode(LIMIT_SWITCH_CASE_PIN, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH_PLL_PIN, INPUT_PULLUP);
  // DC Motor
  pinMode(Y_DC_IN1, OUTPUT);
  pinMode(Y_DC_IN2, OUTPUT);
  pinMode(Y_DC_EN, OUTPUT);
  digitalWrite(Y_DC_IN1, LOW);
  digitalWrite(Y_DC_IN2, LOW);
  digitalWrite(Y_DC_EN, LOW);
  analogWrite(Y_DC_EN, 100);
  // Steppers
  xStep.setSpeed(SPEED);
  zStep.setSpeed(ROT_SPEED);
  // Servos
  gServo.attach(SERVO_GRAB_PIN);
  zServo.attach(SERVO_LIFT_PIN);
  claw.open();
  delay(5000);
  claw.close();
}

void loop() {}

#endif  // TEST

#ifdef PROD

/// Setup
void setup() {
  Serial.begin(9600);
  // Buttons
  btnList.begin();
  pinMode(LIMIT_SWITCH_CASE_PIN, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH_PLL_PIN, INPUT_PULLUP);
  // DC Motor
  pinMode(Y_DC_IN1, OUTPUT);
  pinMode(Y_DC_IN2, OUTPUT);
  pinMode(Y_DC_EN, OUTPUT);
  digitalWrite(Y_DC_IN1, LOW);
  digitalWrite(Y_DC_IN2, LOW);
  digitalWrite(Y_DC_EN, LOW);
  analogWrite(Y_DC_EN, DC_SPEED);
  // Steppers
  xStep.setSpeed(SPEED);
  zStep.setSpeed(ROT_SPEED);
  // Servos
  gServo.attach(SERVO_GRAB_PIN);
  claw.open();
  zServo.attach(SERVO_LIFT_PIN);
  loc.moveYto(true);
  delay(1000);
  loc.moveZup(false);
  zStep.step(-10);
  delay(10);
  zStep.step(10);
  delay(10);
}

/// Main Driver
void loop() {
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
      break;
    }

    // Zero X Movement
    loc.moveXto(0);

    for (colorCount = 0; colorCount < 4; colorCount++) {
      // 1) Move to Case XPOS
      Serial.print("1) -MOVING TO CASE XPOS-\n");
      loc.moveXto(caseXPos);

      // 2) Wait for Case
      Serial.print("2) -WAITING FOR CASE-\n");
      detection.setCaseReady(false);
      while (!detection.getCaseReady()) {
        detection.caseDetect(caseSonarPtr);
        delay(15);
      }

      // 3) Move Forward to Case
      Serial.print("3) -MOVING FORWARD TO CASE-\n");
      loc.moveYto(false);

      // 4) Grab Case
      Serial.print("4) -GRABBING CASE-\n");
      claw.close();

      // 5) Lift Case
      Serial.print("5) -LIFTING CASE-\n");
      loc.moveZup(true);

      // 6) Move Back
      Serial.print("6) -MOVING BACK FOR FLIP-\n");
      loc.moveYto(true);

      // 8) Wait for PLL
      Serial.print("8) -WAITING FOR PLL-\n");
      detection.setPalletReady(false);
      while (!detection.getPalletReady()) {
        detection.palletDetect();
      }

      // 9) Move Y to Case & Rotate to 171deg at the same time
      Serial.print("9) -FLIPPING-\n");
      loc.flip(true);

      // 10) Move to Middle
      Serial.print("10) -MOVING TO MIDDLE X-\n");
      loc.moveXto(MIDDLE_XPOS);

      // 11) Finish Spin
      Serial.print("11) -ALIGNING TO PLL");
      loc.rotateZto(180);

      // 12) Move to PLL
      Serial.print("12) -MOVING TO PLL Y-\n");
      loc.moveYfor(400, 120, true);

      // 13) Lower Case
      Serial.print("13) -LOWERING CASE-\n");
      loc.moveZup(false);

      // 14) Drop Case
      Serial.print("14) -DROPPING CASE-\n");
      claw.open();

      // 15) Raise Claw
      Serial.print("15) -RAISING CLAW-\n");
      loc.moveZup(true);

      // 16) Move to Spin POS
      Serial.print("16) -MOVING TO SPIN X-\n");
      loc.moveXto(SPIN_XPOS);
      loc.moveYto(false);

      // 17) Flip to face case
      Serial.print("17) -FLIPPING-\n");
      loc.flip(false);

      // 18) Move to 0 POS
      Serial.print("18) -MOVING TO X CasePos-\n");
      loc.moveXto(caseXPos);

      // 19) Move Down
      Serial.print("19) -MOVE CLAW DOWN-\n");
      loc.moveZup(false);
    }
    startingColor = nextColor;
  }
}

#endif  // PROD

/// Old Logic Flow
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
      break;
    }

    for (colorCount = 0; colorCount < 4; colorCount++) {

    }
    startingColor = nextColor;
*/
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
    */