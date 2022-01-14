#include <Arduino.h>
#include "./GameClient.h"
#include "./Master.h"
#include "./EspNowController.h"

#define BUTTON_PIN 12 //D6

Gameclient::Gameclient(){
    Serial.println("Created a game client!");
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void Gameclient::Loop(){

    //detect whenever the button is pressed and run:
    if(!buttonIsPressed && digitalRead(BUTTON_PIN) == LOW){
        delay(500);
        buttonPressed();
        buttonIsPressed = true;
    } else if(buttonIsPressed && digitalRead(BUTTON_PIN) == HIGH){
        buttonIsPressed = false;
    }
}

void Gameclient::receiveGameData(EspNowController::message_structure received_data){
    if(!gameRunning){
        Serial.println("Game data received!");
        Serial.print("Next color blue: ");
        Serial.println(received_data.isBlue);
        isBlue = received_data.isBlue;
        turnedOnAtMillis = millis();

        if(isBlue){
            turnButtonBlue();
        } else {
            turnButtonRed();
        }
    } else if(gameRunning && received_data.isEndOfGame){
        gameRunning = false;
        Serial.println("Game is over!");
        Serial.print("Blue won: ");
        Serial.println(received_data.isBlue);
    }

}


void Gameclient::turnButtonBlue(){
    Serial.println("Turning the button blue!");
    //do sth fancy here
}

void Gameclient::turnButtonRed(){
    Serial.println("Turning the button red!");
    //do sth fancy here
}

void Gameclient::buttonPressed(){
    if(gameRunning){
        Serial.println("Button was pressed!");
        sendButtonPressed(millis() - turnedOnAtMillis, isBlue);
        turnedOnAtMillis = millis();

        isBlue = !isBlue;
        if(isBlue){
            turnButtonBlue();
        } else {
            turnButtonRed();
        }
    }
}

bool Gameclient::gameIsRunning(){
    return gameRunning;
}