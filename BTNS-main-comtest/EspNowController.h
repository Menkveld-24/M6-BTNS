#ifndef _ESPNOWCONTROLLER_H_
#define _ESPNOWCONTROLLER_H_

//superclass
#include <stdint.h>
#include <Arduino.h>
#include <string>

class EspNowController{
  public:
    EspNowController();
    void Loop();
    void OnDataReceive(uint8_t * mac, uint8_t *incomingData, uint8_t len);
    void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);
    
    //used by children v
    //transmission data type
    typedef struct message_structure {
      int peerId; // -1 = unset for slave
      bool lookingForMaster;   // used by slave
      bool isMaster;    // used by master
      char message[80]; // char array to send a string along, limit 80 chars
    } message_structure;
    message_structure Received_data;

    String convertMACtoStr(uint8_t *mac);
    void SendData(uint8_t *targetMac, message_structure message);

    
  private:
    bool isSlave;
};

#endif