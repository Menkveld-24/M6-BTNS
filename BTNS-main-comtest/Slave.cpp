// BTNS project esp8266 espnow communication sketch - 26-dec-2021
// Group 30 Module 6 Y2 Creative Technology Enschede
// Issa Margherita, David Huybens, Sietze Huisman, Sade Yokubova
// Author Menke Veerman (m.l.veerman@student.utwente.nl)
// 
// Slave class, created at startup when selected at boot
// 

#include <Arduino.h>
#include "Slave.h"
#include <ESP8266WiFi.h>
#include <espnow.h>

#define STATUS_LED 2

//slave setup
Slave::Slave(){
  Serial.print("ESP8266 Board Slave MAC Address:  ");
  Serial.println(WiFi.macAddress());
  WiFi.mode(WIFI_STA);
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, HIGH); //low = on, high = off
  return;
}

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

//transmit data to master
void Slave::SendData(uint8_t *targetMac ,message_structure message){
  digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
  esp_now_send(targetMac, (uint8_t *) &message, sizeof(message));
  digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
  return;
}

//data sent callback function
void Slave::OnDataSent(uint8_t *mac_addr, uint8_t sendStatus){
  Serial.println("Last Packet Send Status: ");
  Serial.println("Target: " + this->convertMACtoStr(mac_addr));
  Serial.println(sendStatus == 0 ? "Delivery success" : "Delivery fail");
  Serial.println();
  return;
}

//data received callback function
void Slave::OnDataReceive(uint8_t *mac, uint8_t *incomingData, uint8_t len){

  Serial.print("Slave received mail from: ");
  Serial.println(this->convertMACtoStr(mac));
  
  Serial.println("Received_data");
  memcpy(&Received_data, incomingData, sizeof(Received_data));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Id: ");
  Serial.println(Received_data.peerId);
  Serial.print("is Master: ");
  Serial.println(Received_data.isMaster);
  Serial.print("Message: ");
  Serial.println(Received_data.message);
  Serial.println();
  
  if(Received_data.isMaster) this->registerMaster(mac);
  return;
}

String Slave::convertMACtoStr(uint8_t *mac){
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(macStr);
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
