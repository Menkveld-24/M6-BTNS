// BTNS project esp8266 espnow communication sketch - 26-dec-2021
// Group 30 Module 6 Y2 Creative Technology Enschede
// Issa Margherita, David Huybens, Sietze Huisman, Sade Yokubova
// Author Menke Veerman (m.l.veerman@student.utwente.nl)
// 
// This is a wrapper for the master and slave which is there to keep the main file clean
// 

#include <Arduino.h>
#include "ESP8266Wifi.h"
#include <espnow.h>
#include "Communicator.h"
#include "Master.h"
#include "Slave.h"

//initialisation
Communicator::Communicator(bool _isSlave){
    isSlave = _isSlave;

    Serial.print("I am a ");
    Serial.println(isSlave ? "Slave " : "Master ");
    Serial.println();

    if(isSlave){ //if high configure as slave else master
        slave = new Slave();  
    } else{
        master = new Master();
    }

}

//pass a loop function to the master or slave
void Communicator::Loop(){
    if(isSlave){
        slave->Loop();
    } else{
        master->Loop();
    }
}

// pass the data callback to the master or slave
void Communicator::OnDataReceive(uint8_t *mac, uint8_t *incomingData, uint8_t len){
    if(isSlave){
        slave->OnDataReceive(mac, incomingData, len);
        slave->handleReceivedData(mac);
    } else{
        master->OnDataReceive(mac, incomingData, len);
        master->handleReceivedData(mac);
    }
}

// data sent callback to master or slave
void Communicator::OnDataSent(uint8_t *mac_addr, uint8_t sendStatus){
  if(isSlave){
    slave->OnDataSent(mac_addr, sendStatus);
  } else{
    master->OnDataSent(mac_addr, sendStatus);
  }
}

// can be accessed to check whether the device is set as a slave or a master
bool Communicator::amISlave(){
    return isSlave;
}