#include <Arduino.h>
#include "./GameClient.h"
#include "./Master.h"
#include "./EspNowController.h"

#define BUTTON_PIN 5 //D1
#define ledRed 14 
#define ledGreen 12
#define ledBlue 13

Gameclient::Gameclient(){
    Serial.println("Created a game client!");
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(ledRed, OUTPUT);
    pinMode(ledGreen, OUTPUT);
    pinMode(ledBlue, OUTPUT);
}

void Gameclient::Loop(){
    unsigned long currentMillis = millis();
    if(turnOnAtMillis < currentMillis){
        turnButtonOn();
        turnOnAtMillis = 86400000; //set extremely high to one day to prevent rerunning this method
        turnedOnAtMillis = currentMillis;
    }

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
    Serial.println("Game data received!");
    if(received_data.turnOnInMillis == 0){
        //turn on now
        canSend = true;
        turnButtonOn();
    } else if (received_data.turnOnInMillis == -1){
        //start at the water color
        turnButtonOff();
    } else if (received_data.turnOnInMillis > 0){
        //turn on in x millis
        canSend = true;
        turnOnAtMillis = millis() + received_data.turnOnInMillis;
        Serial.print("Turning on in: ");
        Serial.println(received_data.turnOnInMillis);
    }
}


void Gameclient::turnButtonOn(){
    Serial.println("Turning the button on!");
    analogWrite(ledRed, 255);
    buttonIsOn = true;
}

void Gameclient::turnButtonOff(){
    Serial.println("Turning the button off!");
    analogWrite(ledBlue, 255);
    buttonIsOn = false;
}

void Gameclient::buttonPressed(){
    Serial.println("Button was pressed!");
    if(canSend){
        turnButtonOff();
        sendButtonPressed(millis() - turnedOnAtMillis);
        canSend = false;
    } else{
        Serial.println("Not allowed to send yet");
    }
}

bool Gameclient::gameIsRunning(){
    return gameRunning;
}

bool Gameclient::buttonIsTurnedOn(){
    return buttonIsOn;
}
