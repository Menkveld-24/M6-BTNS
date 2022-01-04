// BTNS project esp8266 espnow communication sketch - 26-dec-2021
// Group 30 Module 6 Y2 Creative Technology Enschede
// Issa Margherita, David Huybens, Sietze Huisman, Sade Yokubova
// Author Menke Veerman (m.l.veerman@student.utwente.nl)
// 
// Main sketch, creates a communicator that is responsible for the communications
// short D2 to gnd to make the esp a master, otherwise it is a slave
// D2 is configurable in communicator.cpp
// 

#include <Arduino.h>
#include "Communicator.h"


Communicator *communication;

void setup(){
  Serial.begin(115200);
  delay(3000);
  Serial.println();
  Serial.println("Starting this esp...");
  communication = new Communicator();
}

void loop(){
  communication->Loop();
}
