#include <Arduino.h>
#include "./GameHost.h"
#include "./Master.h"
#include "./EspNowController.h"
#include <vector>
#include <numeric>

using namespace std;

#define START_GAME 14 //D5

Gamehost::Gamehost(){//uint8_t* (&_slaveMACS)[6][10]){
    Serial.println("Gamehost created");
    pinMode(START_GAME, INPUT_PULLUP); //D5

}

void Gamehost::Loop(){
    if(!gameRunning && digitalRead(START_GAME) == LOW){
        startGame();
    }
    else if(gameRunning && delayForNextButton != 0){
        if(timeTakenForPress.size() >= 10){ //enough samples to start automating button presses
            if(millis() > delayForNextButton){
                enableNewButton();
            }
        }
    }
}

void Gamehost::startGame(){
    if(!attemptGameStart()){
        Serial.println("Not enough connected clients");
        return;
    }
    gameRunning = true;
    Serial.println("Enough players. Starting game.");
    //initializing the buttons
    random_shuffle ( connectedSlaveIDs.begin(), connectedSlaveIDs.end());
    //random shuffle list of connected clients, first half starts at fire
    for(int i = 0; i < connectedSlaveIDs.size(); i++){
        if(i < connectedSlaveIDs.size()/2){//first half
            sendToClient(i, 0, "Fire at startup");
        } else {
            sendToClient(i, -1, "Water at startup");
            freeButtons.push_back(i);
        }
    }
}

void Gamehost::buttonPressed(int client_id, EspNowController::message_structure _received_data){
    Serial.print(client_id);
    Serial.println(" Pressed a button!");
    timeTakenForPress.push_back(_received_data.timeTurnedOn);
    freeButtons.push_back(client_id);
    if(timeTakenForPress.size() < 10){ // not enough samples, intantly enable a new button
        enableNewButton();
    }
    if(freeButtons.size() == connectedSlaveIDs.size()){ //all buttons water
        gameWon();
    }
}

void Gamehost::enableNewButton(){
    int nextDelay = 0;
    if(timeTakenForPress.size() >= 10){
        int totalTime = accumulate(timeTakenForPress.end() - 9,timeTakenForPress.end(), 0);
        int averageTime = totalTime/timeTakenForPress.size();
        nextDelay = int(averageTime * 0.9);
    }
    random_shuffle(freeButtons.begin(), freeButtons.end());
    sendToClient(freeButtons[0], nextDelay, "Firing up new button..");
    freeButtons.erase(freeButtons.begin());

    if(freeButtons.size() <= 0){ //all buttons fire
        gameOver();
    } else{
        delayForNextButton = millis() + nextDelay;
    }

}

void Gamehost::gameOver(){
    gameRunning = false;
    Serial.println("You Lost the game :( Next time better luck");
    printStatistics();
}

void Gamehost::printStatistics(){
    int totalTime = accumulate(timeTakenForPress.begin(),timeTakenForPress.end(), 0);
    int averageTime = totalTime/timeTakenForPress.size();

    Serial.println("Game statistics:"); 
    Serial.print("Buttons pressed: ");
    Serial.println(timeTakenForPress.size());

    Serial.print("Total game time: ");
    Serial.println(totalTime);

    Serial.print("Average time between presses:");
    Serial.println(averageTime);
}

void Gamehost::gameWon(){
    gameRunning = false;
    Serial.println("You won the game!");
    printStatistics();
}

bool Gamehost::gameIsRunning(){
    return gameRunning;
}