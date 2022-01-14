#ifndef _SLAVE_H_
#define _SLAVE_H_

//childclass of espnowcontroller
#include "EspNowController.h"
#include "Gameclient.h"

class Slave : public EspNowController, public Gameclient{
  public:
    void Loop();
    void registerMaster(uint8_t *mac, EspNowController::message_structure received_data);
    void handleReceivedData(uint8_t * mac, EspNowController::message_structure received_data);
    void sendButtonPressed(int timeTurnedOn, bool isBlue);
  
  private:
    unsigned long previousMillis = 0;
    int slaveId = -1;
    // EspNowController::message_structure received_data;
    
    uint8_t masterMAC[6]; //{0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};

#endif