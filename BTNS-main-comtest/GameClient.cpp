#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include "./GameClient.h"
#include "./Master.h"
#include "./EspNowController.h"

#define BUTTON_PIN 5 //D1
#define ledRed 14 
#define ledGreen 12
#define ledBlue 13

#define ledPin 12
#define amount 1

#define Neo false
Adafruit_NeoPixel led(1, 12, NEO_GRB + NEO_KHZ400);


Gameclient::Gameclient(){
    Serial.println("Created a game client!");
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    if(Neo){
      led.begin();
    }
    else{
      pinMode(ledRed, OUTPUT);
      pinMode(ledGreen, OUTPUT);
      pinMode(ledBlue, OUTPUT);
    }   
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
    if(Neo){
      led.setPixelColor(0, 0, 255, 0);
      led.show();
    }
    else{
      analogWrite(ledRed, 255);
      analogWrite(ledBlue, 0);
      analogWrite(ledGreen, 0);
    }
    // buttonIsOn = true;
}

void Gameclient::turnButtonOff(){
    Serial.println("Turning the button off!");
    if(Neo){
        led.setPixelColor(0, 0, 0, 255);
        led.show();
    }
    else{
    analogWrite(ledBlue, 255);
    analogWrite(ledRed, 0);
    analogWrite(ledGreen, 0);
    } 
    // buttonIsOn = false;
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

// bool Gameclient::buttonIsTurnedOn(){
//     return buttonIsOn;
// }
