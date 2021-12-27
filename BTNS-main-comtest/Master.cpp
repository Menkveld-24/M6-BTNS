// BTNS project esp8266 espnow communication sketch - 26-dec-2021
// Group 30 Module 6 Y2 Creative Technology Enschede
// Issa Margherita, David Huybens, Sietze Huisman, Sade Yokubova
// Author Menke Veerman (m.l.veerman@student.utwente.nl)
// 
// Master class, this is a subclass of the base class espnowcontroller
// 

#include <Arduino.h>
#include "./Master.h"
#include <ESP8266WiFi.h>
#include <espnow.h>

#define STATUS_LED 2


//main loop function
void Master::Loop(){
  return;
}

//broadcast the same data to all macs
void Master::broadCastToAllSlaves(message_structure message){
  Serial.println("Broadcasting data to all connected slaves!");
  for(int i = 0; i < sizeof(slaveMACS); i++){
    if( memcmp(placeholderMAC, slaveMACS[i], sizeof(placeholderMAC)) != 0 ){
      SendData(slaveMACS[i], message);
    }
  }
  return;
}

//function called after data is received, Received_data is the dat that is received
void Master::handleReceivedData(uint8_t * mac){
  // followup action on data received
  if(Received_data.lookingForMaster) this->registerSlave(mac);
  return;
}

//register slaves
void Master::registerSlave(uint8_t *mac){
  for(int i = 0; i < sizeof(slaveMACS); i++){
    if( memcmp(mac, slaveMACS[i], sizeof(mac)) == 0 ){ //reconnect a slave
      message_structure _message = {i, false, true, "Reconnect to a master"};
      this->SendData(slaveMACS[i], _message);
      Serial.print("Reconnected slave mac at: ");
      Serial.println(this->convertMACtoStr(slaveMACS[i]));
      Serial.print("With id: ");
      Serial.println(i);
      return;

    } else if(memcmp(placeholderMAC, slaveMACS[i], sizeof(placeholderMAC)) == 0){ //new connection
      digitalWrite(STATUS_LED, HIGH); //onboard led showing a client was registered
      memmove(slaveMACS[i], mac, 6);
      esp_now_add_peer(slaveMACS[i], ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
      message_structure _message = {i, false, true, "Found a master"};
      this->SendData(slaveMACS[i], _message);
      Serial.print("Registered slave mac at: ");
      Serial.println(this->convertMACtoStr(slaveMACS[i]));
      Serial.println("Set slave id: " + String(i));
      digitalWrite(STATUS_LED, LOW); //onboard led showing a client was registered
      return;
    }
  }
  Serial.println("No empty mac places...");
  return;
}
