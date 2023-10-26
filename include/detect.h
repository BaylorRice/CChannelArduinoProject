#ifndef CCHANNELARUDINOPROJECT_DETECT_H
#define CCHANNELARUDINOPROJECT_DETECT_H

#include "setup.h"

class detect {
private:
    bool caseReady = false;
    bool palletReady = false;
    bool buttonReady = false;

public:
    // default constructor
    detect() {
        caseReady = false;
        palletReady = false;
        buttonReady = false;
    }
    // gets and sets
    bool getCaseReady(void);
    bool getPalletReady(void);
    bool getButtonReady(void);
    void setCaseReady(bool);
    void setPalletReady(bool);
    void setButtonReady(bool);

    // detect functions
    void caseDetect(NewPing);
    void palletDetect();
    NewPing detectPress(ButtonList*);

};


#endif //CCHANNELARUDINOPROJECT_DETECT_H
