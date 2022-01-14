#ifndef _GAMECLIENT_H_
#define _GAMECLIENT_H_
#include "EspNowController.h"

class Gameclient{
    public:
        Gameclient();
        void Loop();
        void receiveGameData(EspNowController::message_structure received_data);
        bool gameIsRunning();
        virtual void sendButtonPressed(int timeTurnedOn, bool isBlue) = 0;

    private:
        void turnButtonBlue();
        void turnButtonRed();
        void buttonPressed();

        bool gameRunning = false;
        bool buttonIsPressed = false;
        bool isBlue;
        unsigned long turnedOnAtMillis = 0;
};

#endif