#ifdef ESP32
  #include <esp_now.h>
  #include <WiFi.h>
#else
  #include <esp_now.h>
  #include <ESP8266WiFi.h>
#endif


#define Hall_pin 26

// variables for data holder RPM_VAL
float rpm_val;
// variables for incoming readings
float incomingRpm_val;

esp_now_peer_info_t peerInfo;

typedef struct struct_message {
    float rpm_val;
} struct_message;

// hold sensor readings
struct_message RPM_VAL;
// hold incoming sensor readings
struct_message incomingReadings;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  incomingRpm_val = incomingReadings.rpm_val;
}


void setup() {
  
  /* initialization process
  Serial.begin(9600);
  WiFi.mode(WIFI_MODE_STA);
  Serial.println(WiFi.macAddress());
  */

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
  
  // hall effect
  pinMode(Hall_pin, INPUT);

}

void loop() {
  static int count = 0;


  // receiver mac address
  uint8_t broadcastAddress[] = {0xC8, 0xF0, 0x9E, 0xA1, 0xD0, 0xF4};



  // put your main code here, to run repeatedly:
  
  float hallThresh = 100.0;
  int hallValue = digitalRead(Hall_pin);


  ///////////////////////////////
  // hall effect
  // preallocate values for tach
  float hall_count = 1.0;
  float start = micros();
  bool on_state = false;
  // counting number of times the hall sensor is tripped
  // but without double counting during the same trip
  while(true){
    if (hallValue==0){
      if (on_state==false){
        on_state = true;
        hall_count+=1.0;
      }
    } else
      on_state = false;
      
    if (hall_count>=hallThresh)
      break;
  }

  
  

  ///////////////////////////////////
  // hall effect
  float end_time = micros();
  float time_passed = ((end_time-start)/1000000.0);
  //Serial.print("Time Passed: ");
  //Serial.print(time_passed);
  //Serial.println("s");
  float rpm_val = (hall_count/time_passed)*60.0;

  delay(1);
  count++;


  

  if(count == 500){
    // pass
    count = 0;

    RPM_VAL.rpm_val = rpm_val;
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &RPM_VAL, sizeof(RPM_VAL));



    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }

//    updateDisplay();
  }

  // send data every 0.5 seconds
}
