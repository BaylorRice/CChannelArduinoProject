#include "detect.h"

// gets and sets

bool detect::getCaseReady(void) {
    return caseReady;
}
bool detect::getPalletReady(void) {
    return palletReady;
}
bool detect::getButtonReady(void) {
    return buttonReady;
}

void detect::setCaseReady(bool sensorData) {
    caseReady = sensorData;
}
void detect::setPalletReady(bool sensorData) {
    palletReady = sensorData;
}
void detect::setButtonReady(bool data) {
    buttonReady = data;
}

// detect functions
void detect::caseDetect(NewPing selection) {
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

void detect::palletDetect() {
    int time, distance;
    time = sonarPLL.ping_median(NUM_PINGS);
    distance = sonarPLL.convert_cm(time);
    // FIXME need correct distance range
    if (distance < 10 && distance > 5) {
        setPalletReady(true);
    }
    else {
        setPalletReady(false);
    }
}

NewPing detect::detectPress(Button *buttons) {
    Button btn;
    NewPing selection;
    while (buttonReady) {
        if (buttons[0].resetClicked()) {
            selection = sonarGreen;
            setButtonReady(false);
        }
        if (buttons[1].resetClicked()) {
            selection = sonarGold;
            setButtonReady(false);
        }
    }
    return selection;
}