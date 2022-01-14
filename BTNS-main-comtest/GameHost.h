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


        virtual void sendToClient(int id, bool isBlue, std::string message) = 0;
        virtual bool attemptGameStart() = 0;
        virtual void stopGameAtAllClients(bool blueWon) = 0;

    private:
        void enableNewButton(int client_id, bool isBlue);
        // void printStatistics();
        void redWon();
        void blueWon();
        bool gameRunning = false; 
        vector<int> blueButtons; //buttons that can be set
        // uint8_t* (&slaveMACS)[6][10]);
};

#endif