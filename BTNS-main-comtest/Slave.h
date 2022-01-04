#ifndef _SLAVE_H_
#define _SLAVE_H_

//childclass of espnowcontroller
#include "EspNowController.h"

class Slave : public EspNowController{
  public:
    void Loop();
    void registerMaster(uint8_t *mac);
    void handleReceivedData(uint8_t * mac);
  
  private:
    unsigned long previousMillis = 0;
    int slaveId = -1;
    
    uint8_t masterMAC[6]; //{0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};

#endif