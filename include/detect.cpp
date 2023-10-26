#include "detect.h"

// gets and sets
bool getCaseReady(void) {
    return caseReady;
}
bool getPalletReady(void) {
    return palletReady;
}
bool getButtonReady(void) {
    return buttonReady;
}

void setCaseReady(bool sensorData) {
    caseReady = sensorData;
}
void setPalletReady(bool sensorData) {
    palletReady = sensorData;
}
void setButtonReady(bool data) {
    buttonReady = data;
}

// detect functions
void caseDetect(NewPing selection) {
    int time, distance;
    time = selection.ping_median(NUM_PINGS);
    distance = selection.convert_cm(time);
    if (distance < 5) {
        setCaseReady(true);
    }
    else {
        setCaseReady(false);
    }
}

void palletDetect() {
    int time, distance;
    time = sonarPLL.ping_median(NUM_PINGS);
    distance = sonarPLL.convert_cm(time);
    if (distance < 10 && distance > 5) {
        setPalletReady(true);
    }
    else {
        setPalletReady(false);
    }
}

NewPing detectPress(Button *buttons) {
    Button btn;
    NewPing selection;
    if (buttons[0].resetClicked()) {
        selection = sonarGreen;
    }
    if (buttons[1].resetClicked()) {
        selection = sonarGold;
    }
    return selection;
}