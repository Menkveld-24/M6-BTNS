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
    void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);
    
    //used by children v
    //transmission data type
    typedef struct message_structure {
      int peerId; // -1 = unset for slave
      bool lookingForMaster;   // used by slave
      bool isMaster;    // used by master
      bool isPressed; // true if button is pressed
      int turnOnInMillis; //if = 0 turn on now, else wait the amount of time before turning on, -1 = start at water, -2 = ignore
      int timeTurnedOn; //time a button was turned on
      char message[80]; // char array to send a string along, limit 80 chars
    } message_structure;

    message_structure formatMessage(int peerId, bool lookingForMaster, bool isMaster, bool isPressed, int turnOnInMillis, int timeTurnedOn, std::string message);

    void OnDataReceive(uint8_t * mac, message_structure received_data, uint8_t len);

    String convertMACtoStr(uint8_t *mac);
    void SendData(uint8_t *targetMac, message_structure message);

    
  private:
    bool isSlave;
};

#endif