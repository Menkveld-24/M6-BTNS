// BTNS project esp8266 espnow communication sketch - 26-dec-2021
// Group 30 Module 6 Y2 Creative Technology Enschede
// Issa Margherita, David Huybens, Sietze Huisman, Sade Yokubova
// Author Menke Veerman (m.l.veerman@student.utwente.nl)
// 
// Slave class, this is a subclass of the base class espnowcontroller
// 

#include <Arduino.h>
#include "./Slave.h"
#include <ESP8266WiFi.h>
#include <espnow.h>

#define STATUS_LED 2

//main loop function
void Slave::Loop(){
  unsigned long currentMillis = millis();
  if(slaveId == -1 && currentMillis - previousMillis >= 2000){
    previousMillis = millis();
    message_structure _message = {slaveId, true, false, "Looking for master"};
    uint8_t broadcastMac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    this->SendData(broadcastMac, _message);
  }
  return;
}

//function called after data is received, Received_data is the dat that is received
void Slave::handleReceivedData(uint8_t * mac){
  // followup action on data received
  if(Received_data.isMaster) this->registerMaster(mac);
  return;
}

//register master
void Slave::registerMaster(uint8_t *mac){
  memmove(masterMAC, mac, 6);
  slaveId = Received_data.peerId;
  esp_now_add_peer(masterMAC, ESP_NOW_ROLE_CONTROLLER, 1, NULL, 0);
  Serial.print("Registered master mac at: ");
  Serial.println(this->convertMACtoStr(masterMAC));
  Serial.println("Set slave id: " + String(slaveId));
  Serial.println();

  digitalWrite(STATUS_LED, LOW); //onboard led showing it has found a master
  return;
}
