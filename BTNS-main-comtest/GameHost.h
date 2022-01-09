#ifndef _GAMEHOST_H_
#define _GAMEHOST_H_

#include "EspNowController.h"
#include <vector>
#include <string>

using namespace std;

class Gamehost{
    public:
        Gamehost();
        void Loop();
        vector<int> connectedSlaveIDs;

        void startGame();
        void buttonPressed(int slave_id, EspNowController::message_structure received_data);
        bool gameIsRunning();


        virtual void sendToClient(int id, int turnOnInMillis, std::string message) = 0;
        virtual bool attemptGameStart() = 0;

    private:
        void enableNewButton();
        void printStatistics();
        void gameWon();
        void gameOver();
        bool gameRunning = false;
        vector<int> timeTakenForPress; 
        vector<int> freeButtons; //buttons that can be set
        int delayForNextButton = 0;
        // uint8_t* (&slaveMACS)[6][10]);
};

#endif