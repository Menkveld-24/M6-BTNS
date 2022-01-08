#include <Arduino.h>
#include "./GameHost.h"
#include "./Master.h"

Gamehost::Gamehost(){
    Serial.println("Gamehost created?");
}

void Gamehost::test(){
    test1();
}

void Gamehost::startGame(){
    if(!attemtGameStart()){
        Serial.println("Not enough connected clients");
        return;
    }
    Serial.println("Enough players. Starting game.");
    for(int i = 0; i < connectedSlaveIDs.size(); i++){
        
    }
}