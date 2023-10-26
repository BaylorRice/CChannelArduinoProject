#ifndef CCHANNELARUDINOPROJECT_DETECT_H
#define CCHANNELARUDINOPROJECT_DETECT_H

#include "setup.h"

class detect {
private:
    bool caseReady;
    bool palletReady;
    bool buttonReady;

public:
    // gets and sets
    bool getCaseReady(void);
    bool getPalletReady(void);
    bool getButtonReady(void);
    void setCaseReady(bool);
    void setPalletReady(bool);
    void setButtonReady(bool);

    // detect functions
    bool caseDetect();
    bool palletDetect();
    NewPing detectPress();

};


#endif //CCHANNELARUDINOPROJECT_DETECT_H
