// BTNS project esp8266 espnow communication sketch - 26-dec-2021
// Group 30 Module 6 Y2 Creative Technology Enschede
// Issa Margherita, David Huybens, Sietze Huisman, Sade Yokubova
// Author Menke Veerman (m.l.veerman@student.utwente.nl)
// 
// This sketch determines whether the flashed esp is a master or a slave, 
// This is selected by shorting D2 to ground on startup (short = master, nc = slave)
// This is the main file which creates a communicator. The communicator is a wrapper for the 
// Master or Slaves to keep this file somewhat clean.
// 

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "Communicator.h"


#define MASTERORSLAVE_PIN 4 //short to gnd for master, nc is slave (D1)

Communicator *communication;

void setup(){
  Serial.begin(115200);
  
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  //check if master or slave
  pinMode(MASTERORSLAVE_PIN, INPUT_PULLUP); //D2
  communication = new Communicator(digitalRead(MASTERORSLAVE_PIN));
  esp_now_set_self_role(digitalRead(MASTERORSLAVE_PIN) ? ESP_NOW_ROLE_SLAVE : ESP_NOW_ROLE_CONTROLLER);  

  //callbacks are set in main because I was unable to move them to the communicator class
  esp_now_register_recv_cb(onDataReceive);
  esp_now_register_send_cb(onDataSent);
}

void loop(){
  communication->Loop();
}

//callbacks are set in main class as master or slave instances were unable to compile
void onDataReceive(uint8_t *mac, uint8_t *incomingData, uint8_t len){
  communication->OnDataReceive(mac, incomingData, len);
}

//callbacks are set in main class as master or slave instances were unable to compile
void onDataSent(uint8_t *mac_addr, uint8_t sendStatus){
  communication->OnDataSent(mac_addr, sendStatus);
}