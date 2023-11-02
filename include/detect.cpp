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
        Serial.print("case ready");
    }
    else {
        setCaseReady(false);
        Serial.print("not case ready");
    }
    Serial.print("\nDistance: ");
    Serial.print(distance);

    
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

NewPing detect::detectPress(bool data) {
    NewPing selection(0,0,0);

    while (data) {
        Serial.print("...");
        if (greenStart.resetClicked()) {
            Serial.println("Green\n");
            selection = sonarGreen;
            setButtonReady(false);
        }
        if (goldStart.resetClicked()) {
            Serial.println("Gold\n");
            selection = sonarGold;
            setButtonReady(false);
        }
    }
    return selection;
}