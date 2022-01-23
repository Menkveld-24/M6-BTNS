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
#include <string>

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

//data received callback
void EspNowController::OnDataReceive(uint8_t * mac, message_structure received_data, uint8_t len){

    Serial.print(isSlave ? "Slave " : "Master ");
    Serial.print("received mail from: ");
    Serial.println(this->convertMACtoStr(mac));
  
    Serial.println("received_data");
    Serial.print("Bytes received: ");
    Serial.println(len);

    Serial.print("Id: ");
    Serial.println(received_data.peerId);

    Serial.print("looking for master: ");
    Serial.println(received_data.lookingForMaster);

    Serial.print("is master: ");
    Serial.println(received_data.isMaster);

    Serial.print("button pressed: ");
    Serial.println(received_data.isPressed);

    Serial.print("Turn on in millis: ");
    Serial.println(received_data.turnOnInMillis);

    Serial.print("Time turned on: ");
    Serial.println(received_data.timeTurnedOn);

    Serial.print("Button is blue: ");
    Serial.println(received_data.isBlue);

    Serial.print("Is end: ");
    Serial.println(received_data.isEndOfGame);

    Serial.print("Message: ");
    Serial.write(received_data.message, sizeof(received_data.message));
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

EspNowController::message_structure EspNowController::formatMessage(int peerId, bool lookingForMaster, bool isMaster, bool isPressed, int turnOnInMillis, int timeTurnedOn, bool isBlue, bool isEndOfGame, std::string message){
    message_structure _message = {
        peerId, 
        lookingForMaster, 
        isMaster,
        isPressed,
        turnOnInMillis,
        timeTurnedOn,
        isBlue, 
        "dummy"
      };
      // strncpy(_message.message, message.c_str(), sizeof(_message.message));
      return _message;
}

//convert a mac address to a string
String EspNowController::convertMACtoStr(uint8_t *mac){
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(macStr);
}