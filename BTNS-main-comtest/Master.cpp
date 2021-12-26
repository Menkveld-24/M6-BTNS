// BTNS project esp8266 espnow communication sketch - 26-dec-2021
// Group 30 Module 6 Y2 Creative Technology Enschede
// Issa Margherita, David Huybens, Sietze Huisman, Sade Yokubova
// Author Menke Veerman (m.l.veerman@student.utwente.nl)
// 
// Master class, created at startup when selected at boot
// 

#include <Arduino.h>
#include "Master.h"
#include <ESP8266WiFi.h>
#include <espnow.h>

#define STATUS_LED 2

//master setup
Master::Master(){
  Serial.print("ESP8266 Board Master MAC Address:  ");
  Serial.println(WiFi.macAddress());
  WiFi.mode(WIFI_STA);
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, HIGH); //low = on, high = off
  return;
}

//main loop function
void Master::Loop(){
  return;
}

//transmit data to all connected slaves
void Master::SendData(uint8_t *targetMac ,message_structure message){
  digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
  esp_now_send(targetMac, (uint8_t *) &message, sizeof(message));
  digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
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

//data sent callback function
void Master::OnDataSent(uint8_t *mac_addr, uint8_t sendStatus){
  Serial.println("Last Packet Send Status: ");
  Serial.println("Target: " + this->convertMACtoStr(mac_addr));
  Serial.println(sendStatus == 0 ? "Delivery success" : "Delivery fail");
  Serial.println();
  return;
}

//data received callback function
void Master::OnDataReceive(uint8_t *mac, uint8_t *incomingData, uint8_t len){
  
  Serial.print("Master received mail from: ");
  Serial.println(this->convertMACtoStr(mac));
  
  
  Serial.println("Received_data");
  memcpy(&Received_data, incomingData, sizeof(Received_data));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Id: ");
  Serial.println(Received_data.peerId);
  Serial.print("looking for master: ");
  Serial.println(Received_data.lookingForMaster);
  Serial.print("Message: ");
  Serial.println(String(Received_data.message));
  Serial.println();
  
  if(Received_data.lookingForMaster) this->registerSlave(mac);
  return;
}

//convert a mac address to a string
String Master::convertMACtoStr(uint8_t *mac){
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(macStr);
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
