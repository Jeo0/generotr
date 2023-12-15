//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> One-way communication ESP32 Receiver
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#define Poten_pin 36
#define ESC_pin 25
#define MIN_SPEED 0 // speed just slow enough to turn motor off
#define MAX_SPEED 100 // speed where my motor drew 3.6 amps at 12v.
#include <LiquidCrystal.h>
#include <ESP32Servo.h>     // ESP32Servo library installed by Library Manager
//#include "ESC.h"            // RC_ESP library installed by Library Manager
//#include <Servo.h>
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);
float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
Servo ESC;

//----------------------------------------Load libraries
#include <esp_now.h>
#include <WiFi.h>
//----------------------------------------

//----------------------------------------Define variables to store incoming readings
float receive_rpmvalue;
//----------------------------------------

//----------------------------------------Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    float rpmvalue;
} struct_message;

struct_message receive_Data; //--> Create a struct_message to receive data.
//----------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&receive_Data, incomingData, sizeof(receive_Data));
  Serial.println();
  Serial.println("<<<<< Receive Data:");
  Serial.print("Bytes received: ");
  Serial.println(len);
  receive_rpmvalue = receive_Data.rpmvalue;

  Serial.println("Receive Data: ");
  Serial.println(receive_rpmvalue);

  lcd.setCursor(0,1);
  lcd.print(receive_rpmvalue);

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



void setup() {
  Serial.begin(9600);
  
  ///////////////////////////////////////////////////////////////////////////////// receiver
  WiFi.mode(WIFI_STA); //--> Set device as a Wi-Fi Station

  //----------------------------------------Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  //----------------------------------------
  
  esp_now_register_recv_cb(OnDataRecv); //--> Register for a callback function that will be called when data is received



  //////////////////////////////////////////////////////////////////////////////////// controller
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("circuitschools.");

  pinMode(Poten_pin, INPUT);

    //Brushlessmotor
  pinMode(ESC_pin, OUTPUT);//ESC_pin as OUTPUT
  //ledcSetup(0, 1000, 8);//PWM channel is 0, Frequency 1000Hz, 8 bit(Duty with 256 levels of resolution, the value is 0-255)
  //ledcAttachPin(ESC_pin,0);//Assign channel 0 to ESC_pin



  ESC.attach(ESC_pin, 1000, 2000);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ VOID LOOP
void loop() {
   // read the input on analog pin GPIX36 (A0; VP):
  // for potentiometer
  int analogValue = analogRead(Poten_pin);
  // Rescale to potentiometer's voltage (from 0V to 3.3V):
  //float voltage = floatMap(analogValue, 0, 4095, 0, 3.3);
  //float voltage = floatMap(analogValue, 0, 4095, 0, 3.3);
  float percent = floatMap(analogValue, 0, 4095, MIN_SPEED, MAX_SPEED);
  

  //////////////////////////////////////////////////////////////////
  // for lcd
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  //lcd.print(voltage);
  lcd.setCursor(0,0);
  lcd.print(percent);



  float esc_out = map(percent, MIN_SPEED, MAX_SPEED, 0, 180);
  ESC.write(esc_out);
}
