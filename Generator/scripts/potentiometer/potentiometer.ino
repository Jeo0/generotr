
// potentiometer:
// https://esp32io.com/tutorials/esp32-potentiometer
// https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/

/* lcd:
  * 
*/

#include <LiquidCrystal.h>
 
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);

float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);


  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("circuitschools.");
}




void loop() {


  // read the input on analog pin GPIO32:
  // for 
  int analogValue = analogRead(32);
  // Rescale to potentiometer's voltage (from 0V to 3.3V):
  //float voltage = floatMap(analogValue, 0, 4095, 0, 3.3);
  float voltage = floatMap(analogValue, 0, 4095, 0, 5);
  float percent = floatMap(analogValue, 0, 4095, 0, 100);
  float v5 = floatMap(voltage, 0, 3.3, 0, 5);
  float p5 = floatMap(percent, 0, 100, 0, 100);

  // print out the value you read:
  Serial.print("Analog: ");
  Serial.print(analogValue);
  Serial.print(", Voltage: ");
  Serial.println(voltage);

  Serial.print(": ");
  Serial.println(percent);
  Serial.println("%");
  delay(250);
  

  ///////////////////////////////////////////////
  ///////////////////////////////////////////////
  ///////////////////////////////////////////////
  // for lcd
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(voltage);
  lcd.setCursor(0, 0);
  lcd.print(percent);


}




	
// include the library code:
// https://www.circuitschools.com/interfacing-16x2-lcd-module-with-esp32-with-and-without-i2c/

 