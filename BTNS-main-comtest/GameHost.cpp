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
            enableNewButton(i, false);
        } else {
            enableNewButton(i, true);
        }
    }
}

void Gamehost::buttonPressed(int client_id, EspNowController::message_structure _received_data){
    Serial.print(client_id);
    Serial.println(" Pressed a button!");
    // timeTakenForPress.push_back(_received_data.timeTurnedOn);
    Serial.print("Color is blue: ");
    Serial.println(_received_data.isBlue);
    if(_received_data.isBlue){
        //red pressed the blue button so blue owns one button less
        blueButtons.erase(remove(blueButtons.begin(), blueButtons.end(), client_id), blueButtons.end());
    } else {
        //button pressed was red, add to bluebuttons list
        blueButtons.push_back(client_id);
    }
    //button toggles color itself
    // enableNewButton(client_id, !_received_data.isBlue);

    if(blueButtons.size() == connectedSlaveIDs.size()){ //all buttons water
        blueWon();
    } else if (blueButtons.size() == 0){
        redWon();
    }
}

void Gamehost::enableNewButton(int client_id, bool isBlue){
    if(isBlue){
        sendToClient(client_id, isBlue, "Firing up new blue button..");
        blueButtons.push_back(client_id);
    } else {
        sendToClient(client_id, isBlue, "Firing up new red button..");
    }
}

// void Gamehost::printStatistics(){
//     // int totalTime = accumulate(timeTakenForPress.begin(),timeTakenForPress.end(), 0);
//     // int averageTime = totalTime/timeTakenForPress.size();

//     // Serial.println("Game statistics:"); 
//     // Serial.print("Buttons pressed: ");
//     // Serial.println(timeTakenForPress.size());

//     // Serial.print("Total game time: ");
//     // Serial.println(totalTime);

//     // Serial.print("Average time between presses:");
//     // Serial.println(averageTime);
// }

void Gamehost::redWon(){
    gameRunning = false;
    Serial.println("Red won the game!");
    
    // printStatistics();
}

void Gamehost::blueWon(){
    gameRunning = false;
    Serial.println("Blue won the game!");
    // printStatistics();
}

bool Gamehost::gameIsRunning(){
    return gameRunning;
}