// BTNS project esp8266 espnow communication sketch - 26-dec-2021
// Group 30 Module 6 Y2 Creative Technology Enschede
// Issa Margherita, David Huybens, Sietze Huisman, Sade Yokubova
// Author Menke Veerman (m.l.veerman@student.utwente.nl)
// 
// This is a wrapper for the master and slave which is there to keep the main file clean
// 

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "Communicator.h"
#include "Master.h"
#include "Slave.h"

#define MASTERORSLAVE_PIN 4 //short to gnd for master, nc is slave (D1)

//communicator instance pointer for the callbacks
Communicator* Communicator::_communicator = nullptr;

//initialisation
Communicator::Communicator(){
    pinMode(MASTERORSLAVE_PIN, INPUT_PULLUP); //D2
    Communicator::isSlave = digitalRead(MASTERORSLAVE_PIN);

    Serial.print("I am a ");
    Serial.println(Communicator::isSlave ? "Slave " : "Master ");
    Serial.println();

    Communicator::_communicator = this;

    // Init ESP-NOW
    if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_set_self_role(digitalRead(MASTERORSLAVE_PIN) ? ESP_NOW_ROLE_SLAVE : ESP_NOW_ROLE_CONTROLLER);  

    //set callbacks
    esp_now_register_recv_cb(OnDataReceive);
    esp_now_register_send_cb(OnDataSent);

    if(Communicator::isSlave){ //if high configure as slave else master
        slave = new Slave();  
    } else{
        master = new Master();
    }

}

//pass a loop function to the master or slave
void Communicator::Loop(){
    if(Communicator::isSlave){
        slave->Loop();
    } else{
        master->Loop();
    }
}

// pass the data callback to the master or slave
void Communicator::OnDataReceive(uint8_t *mac, uint8_t *incomingData, uint8_t len){
    if(Communicator::isSlave){
        Communicator::_communicator->slave->OnDataReceive(mac, incomingData, len);
        Communicator::_communicator->slave->handleReceivedData(mac);
    } else{
        Communicator::_communicator->master->OnDataReceive(mac, incomingData, len);
        Communicator::_communicator->master->handleReceivedData(mac);
    }
}

// data sent callback to master or slave
void Communicator::OnDataSent(uint8_t *mac_addr, uint8_t sendStatus){
  if(Communicator::isSlave){
    Communicator::_communicator->slave->OnDataSent(mac_addr, sendStatus);
  } else{
    Communicator::_communicator->master->OnDataSent(mac_addr, sendStatus);
  }
}

// can be accessed to check whether the device is set as a slave or a master
bool Communicator::amISlave(){
    return Communicator::isSlave;
}