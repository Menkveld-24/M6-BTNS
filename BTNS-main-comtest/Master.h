#ifndef _MASTER_H_
#define _MASTER_H_

//childclass of espnowcontroller
#include "EspNowController.h"
#include "GameHost.h"
#include <vector>
#include <string>
using namespace std;

class Master : public EspNowController, public Gamehost{
  public:
    void Loop();
    void broadCastToAllSlaves(EspNowController::message_structure message);
    void handleReceivedData(uint8_t * mac, EspNowController::message_structure message);
    void registerSlave(uint8_t *mac);
  
    void sendToClient(int id, bool isBlue, std::string message);
    bool attemptGameStart();
    void stopGameAtAllClients(bool blueWon);

  private:
    unsigned long previousMillis = 0;
    // EspNowController::message_structure _received_data;

    //used for memcmp
    uint8_t placeholderMAC[6] = {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 
    uint8_t slaveMACS[10][6] = { //index of slavemac is the id of the slace 
            {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, //placeholder to reserve memory space for memcpy
            {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
            {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
            {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
            {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
            {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
            {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
            {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
            {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
            {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
        };
  

};

#endif