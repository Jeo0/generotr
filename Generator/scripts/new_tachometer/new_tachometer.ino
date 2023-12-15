//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> One-way communication ESP32 Sender
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#define Hall_pin 26
//----------------------------------------Load libraries
#include <esp_now.h>
#include <WiFi.h>
//----------------------------------------

// receiver mac address
//uint8_t broadcastAddress[] = {0xC8, 0xF0, 0x9E, 0xA1, 0xD0, 0xF4};  // original receive
uint8_t broadcastAddress[] = {0xEC, 0x94, 0xCB, 0x6D,0xCD,0xE8};      // original sender

//----------------------------------------Variables to accommodate the data to be sent.
float send_rpmvalue;
//----------------------------------------

String success; //--> Variable to store if sending data was successful.

//----------------------------------------Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
float rpm_Value;
} struct_message;

struct_message send_Data; //--> Create a struct_message to send data.
//----------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
  Serial.println(">>>>>");
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ VOID SETUP
void setup() {
  Serial.begin(115200);
 
  WiFi.mode(WIFI_STA); //--> Set device as a Wi-Fi Station.

  //----------------------------------------Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  //----------------------------------------
  
  //----------------------------------------Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  //----------------------------------------
  
  //----------------------------------------Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  //----------------------------------------
  
  //----------------------------------------Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  //----------------------------------------


    attachInterrupt(digitalPinToInterrupt(26), magnet_detect, RISING);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



volatile unsigned int count = 0;
unsigned long next = 5000;
unsigned int temp = 0;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ VOID LOOP
void loop() {
  //----------------------------------------Send message via ESP-NOW
  if (millis()> next){

    
    Serial.println();
    Serial.print(">>>>> ");
    Serial.println("Send data");

    temp = count;
    Serial.println(temp*12);
    count -= temp;
    next += 5000;

    //----------------------------------------Set values to send
    send_rpmvalue = temp;
    send_Data.rpm_Value = send_rpmvalue;
    //----------------------------------------

    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &send_Data, sizeof(send_Data));
    
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    } 
  }


}

void magnet_detect()
{
   count++;
}