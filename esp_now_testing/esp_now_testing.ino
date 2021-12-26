#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

//slave
uint8_t masterMAC[] = {0xAC, 0x0B, 0xFB, 0xD0, 0x7F, 0x16};

//status led
#define STATUS_LED 2

//data
typedef struct message_structure {
  int id;
  String message;
} message_structure;

message_structure message;

void setup() {
  Serial.begin(115200);
  pinMode(STATUS_LED, OUTPUT);
  Serial.println();
  Serial.println("Starting setup");

  //startup indication
  digitalWrite(STATUS_LED, HIGH);
  delay(400);
  digitalWrite(STATUS_LED, LOW);

  //print mac in console
  Serial.print("ESP8266 Board Slave MAC Address:  ");
  Serial.println(WiFi.macAddress());
  
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    digitalWrite(STATUS_LED, HIGH);
    return;
  }

  //register as slave and set callback functions
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataReceive);
  esp_now_register_send_cb(OnDataSent);
  
  esp_now_add_peer(masterMAC, ESP_NOW_ROLE_CONTROLLER, 1, NULL, 0);
  

}

void loop() {
  delay(1000);
  digitalWrite(STATUS_LED, HIGH);
  SendData();
  delay(1000);
  digitalWrite(STATUS_LED, LOW);
}

//sending data to master
void SendData(){
  message.id = 0;
  message.message = "Hello from the slave";
  esp_now_send(masterMAC, (uint8_t *) &message, sizeof(message));
}

//data received
void OnDataReceive(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  Serial.print("Mail from: ");
  //convert received mac to str
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  Serial.println(Str(macStr));
  Serial.println("Received_data");
  memcpy(&message, incomingData, sizeof(message));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Id: ");
  Serial.println(message.id);
  Serial.print("Message: ");
  Serial.println(message.message);
  Serial.println();
}

//data sent callback
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
  Serial.println();
}
