#ifndef _GAMECLIENT_H_
#define _GAMECLIENT_H_
#include "EspNowController.h"

class Gameclient{
    public:
        Gameclient();
        void Loop();
        void receiveGameData(EspNowController::message_structure received_data);
        bool gameIsRunning();
        // bool buttonIsTurnedOn();
        virtual void sendButtonPressed(int timeTurnedOn) = 0;

    private:
        void turnButtonOn();
        void turnButtonOff();
        void buttonPressed();

        bool gameRunning = false;
        bool buttonIsOn = false;
        bool buttonIsPressed = false;
        bool canSend = false;
        unsigned long turnOnAtMillis = 0;
        unsigned long turnedOnAtMillis = 0;
};

#endif