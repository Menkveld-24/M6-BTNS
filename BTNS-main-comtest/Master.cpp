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
#include <vector>
#include <string>

using namespace std;

#define STATUS_LED 2
#define GAME_START_THRESHOLD 1


//main loop function
void Master::Loop(){
  Gamehost::Loop();
  return;
}

//broadcast the same data to all macs
void Master::broadCastToAllSlaves(message_structure message){
  Serial.println("Broadcasting data to all connected slaves!");
  for(int i = 0; i < sizeof(slaveMACS)/sizeof(slaveMACS[0]); i++){
    if( memcmp(placeholderMAC, slaveMACS[i], sizeof(placeholderMAC)) != 0 ){
      SendData(slaveMACS[i], message);
    }
  }
  return;
}

//function called after data is received, received_data is the dat that is received
void Master::handleReceivedData(uint8_t * mac, message_structure received_data){
  // followup action on data received
  // if(memcmp(placeholderMAC, mac, sizeof(placeholderMAC)) != 0){  //sender exists)
    if(received_data.lookingForMaster) registerSlave(mac);

    if(gameIsRunning() && received_data.isPressed){
      //found a connected client and game is running, button pressed
      // for(int i = 0; i < sizeof(slaveMACS)/sizeof(slaveMACS[0]); i++){
        // if( memcmp(placeholderMAC, slaveMACS[i], sizeof(placeholderMAC)) != 0 ){
          buttonPressed(received_data.client_id, received_data);
        // }
      // }
    // }
  }
  return;
}

//register slaves
void Master::registerSlave(uint8_t *mac){
  for(int i = 0; i < sizeof(slaveMACS)/sizeof(slaveMACS[0]); i++){
    if( memcmp(mac, slaveMACS[i], sizeof(mac)) == 0 && !gameIsRunning()){ //reconnect a slave
      message_structure _message = formatMessage(
        i, 
        false, 
        true,
        false,
        -2,
        0, 
        "Reconnect to a master"
      );
      SendData(slaveMACS[i], _message);
      Serial.print("Reconnected slave mac at: ");
      Serial.println(this->convertMACtoStr(slaveMACS[i]));
      Serial.print("With id: ");
      Serial.println(i);
      return;

    } else if(memcmp(placeholderMAC, slaveMACS[i], sizeof(placeholderMAC)) == 0){ //new connection
      digitalWrite(STATUS_LED, HIGH); //onboard led showing a client was registered
      memmove(slaveMACS[i], mac, 6);
      esp_now_add_peer(slaveMACS[i], ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
      message_structure _message = formatMessage(
        i, 
        false, 
        true,
        false,
        -2,
        0, 
        "Found a master"
      );
      SendData(slaveMACS[i], _message);
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

//send data from the gamehost to a client
void Master::sendToClient(int id, int turnOnInMillis, string message){
  message_structure _message = formatMessage(
    id, 
    false, 
    true,
    false,
    turnOnInMillis,
    0, 
    message
  );
  Serial.print("Sending to: ");
  Serial.print(id);
  Serial.print(" : ");
  Serial.println(convertMACtoStr(slaveMACS[id]));

  Serial.println(message.c_str());

  SendData(slaveMACS[id], _message);
}

// gamehost attempt to start the game
bool Master::attemptGameStart(){
  connectedSlaveIDs.clear();
  Serial.println(connectedSlaveIDs.size());
  Serial.println("Attempting to start a game, counting slaves..");
  Serial.println(sizeof(slaveMACS)/sizeof(slaveMACS[0]));
  
  for(int i = 0; i < sizeof(slaveMACS)/sizeof(slaveMACS[0]); i++){
    Serial.print("Loop: ");
    Serial.println(i);
    Serial.println(convertMACtoStr(slaveMACS[i]));
    if( memcmp(placeholderMAC, slaveMACS[i], sizeof(placeholderMAC)) != 0 ){
      //found a connected client
      Serial.println("This is a slave");
      connectedSlaveIDs.push_back(i);
    } else {
      Serial.println("Not a slave");
    }
  }
  Serial.println(connectedSlaveIDs.size());
  return connectedSlaveIDs.size() >= GAME_START_THRESHOLD;
}