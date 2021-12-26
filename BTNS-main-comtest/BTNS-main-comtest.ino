// BTNS project esp8266 espnow communication sketch - 26-dec-2021
// Group 30 Module 6 Y2 Creative Technology Enschede
// Issa Margherita, David Huybens, Sietze Huisman, Sade Yokubova
// Author Menke Veerman (m.l.veerman@student.utwente.nl)
// 
// This sketch determines whether the flashed esp is a master or a slave, 
// This is selected by shorting D1 to ground on startup (short = master, nc = slave)
// 

#include "./Master.h"
#include "./Slave.h"
#include <espnow.h>

#define MASTERORSLAVE_PIN 4 //short to gnd for master, nc is slave (D1)

Master *master;
Slave *slave;

bool isSlave;

void setup(){
  Serial.begin(115200);
  
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  //check if master or slave
  pinMode(MASTERORSLAVE_PIN, INPUT_PULLUP); //D2
  isSlave = digitalRead(MASTERORSLAVE_PIN);
  if(isSlave){ //if high configure as slave else master
    slave = new Slave();
    esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);    
  } else{
    master = new Master();
    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  }
   esp_now_register_recv_cb(OnDataReceive);
   esp_now_register_send_cb(OnDataSent);
}

void loop(){
  if(isSlave){
    slave->Loop();
  } else{
    master->Loop();
  }
}

//callbacks are set in main class as master or slave instances were unable to compile
void OnDataReceive(uint8_t *mac, uint8_t *incomingData, uint8_t len){
  if(isSlave){
    slave->OnDataReceive(mac, incomingData, len);
  } else{
    master->OnDataReceive(mac, incomingData, len);
  }
}

//callbacks are set in main class as master or slave instances were unable to compile
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus){
  if(isSlave){
    slave->OnDataSent(mac_addr, sendStatus);
  } else{
    master->OnDataSent(mac_addr, sendStatus);
  }
}