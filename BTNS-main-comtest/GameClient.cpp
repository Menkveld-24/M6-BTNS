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

#define NeoLed false
Adafruit_NeoPixel led(1, ledPin, NEO_GRB + NEO_KHZ400);

Gameclient::Gameclient(){
    Serial.println("Created a game client!");
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    if(NeoLed){
        led.begin();
    }
    else{
        pinMode(ledRed, OUTPUT);
        pinMode(ledGreen, OUTPUT);
        pinMode(ledBlue, OUTPUT);
      }
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
    if(NeoLed){
      led.setPixelColor(0, 0, 0, 255);
      led.show();
      }
    else{
      analogWrite(ledRed, 0);
      analogWrite(ledBlue, 255);
      analogWrite(ledGreen, 0);
      }
}

void Gameclient::turnButtonRed(){
    Serial.println("Turning the button red!");
    if(NeoLed){
      led.setPixelColor(0, 0, 255, 0);
      led.show();
      }
    else{
      analogWrite(ledRed, 255);
      analogWrite(ledBlue, 0);
      analogWrite(ledGreen, 0);
      }
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
