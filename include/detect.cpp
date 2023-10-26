#include "detect.h"

// gets and sets

detect::bool getCaseReady(void) {
    return caseReady;
}
detect::bool getPalletReady(void) {
    return palletReady;
}
detect::bool getButtonReady(void) {
    return buttonReady;
}

detect::void setCaseReady(bool sensorData) {
    caseReady = sensorData;
}
detect::void setPalletReady(bool sensorData) {
    palletReady = sensorData;
}
detect::void setButtonReady(bool data) {
    buttonReady = data;
}

// detect functions
detect::void caseDetect(NewPing selection) {
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

detect::void palletDetect() {
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

detect::NewPing detectPress(Button *buttons) {
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