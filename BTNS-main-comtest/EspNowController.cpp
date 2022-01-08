// BTNS project esp8266 espnow communication sketch - 26-dec-2021
// Group 30 Module 6 Y2 Creative Technology Enschede
// Issa Margherita, David Huybens, Sietze Huisman, Sade Yokubova
// Author Menke Veerman (m.l.veerman@student.utwente.nl)
// 
// This is the base class of Master and Slave, all the functions defined in public (in header file)
// are accessible from a master or a slave. To prevent having lots of duped functions in a class
// 

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "EspNowController.h"
#include <espnow.h>

#define STATUS_LED 2

// initialisation of both the master and slave
EspNowController::EspNowController(){
    WiFi.mode(WIFI_STA);
    digitalWrite(STATUS_LED, HIGH); //low = on, high = off

    Serial.print("ESP8266 Board MAC Address:  ");
    Serial.println(WiFi.macAddress());

    pinMode(STATUS_LED, OUTPUT);
}

//send data to mac
void EspNowController::SendData(uint8_t *targetMac ,message_structure message){
  digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
  esp_now_send(targetMac, (uint8_t *) &message, sizeof(message));
  digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
  return;
}

void EspNowController::test2(){
  Serial.println("test");
}

//data received callback
void EspNowController::OnDataReceive(uint8_t * mac, uint8_t *incomingData, uint8_t len){
    memcpy(&Received_data, incomingData, sizeof(Received_data));

    Serial.print(isSlave ? "Slave " : "Master ");
    Serial.print("received mail from: ");
    Serial.println(this->convertMACtoStr(mac));
  
    Serial.println("Received_data");
    Serial.print("Bytes received: ");
    Serial.println(len);

    Serial.print("Id: ");
    Serial.println(Received_data.peerId);

    Serial.print("looking for master: ");
    Serial.println(Received_data.lookingForMaster);

    Serial.print("is master: ");
    Serial.println(Received_data.isMaster);

    Serial.print("Message: ");
    Serial.write(Received_data.message, sizeof(Received_data.message));
    Serial.println();
}

//data sent callback
void EspNowController::OnDataSent(uint8_t *mac_addr, uint8_t sendStatus){
    Serial.println("Last Packet Send Status: ");
    Serial.println("Target: " + this->convertMACtoStr(mac_addr));
    Serial.println(sendStatus == 0 ? "Delivery success" : "Delivery fail");
    Serial.println();
    return;
}

//convert a mac address to a string
String EspNowController::convertMACtoStr(uint8_t *mac){
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(macStr);
}