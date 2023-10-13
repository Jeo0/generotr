
// io pins
// https://microcontrollerslab.com/esp32-pinout-use-gpio-pins/

// potentiometer:
// https://esp32io.com/tutorials/esp32-potentiometer
// https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/
// pin VP (A0; GPIX36);

// half effect sensor
// https://www.circuitstate.com/pinouts/doit-esp32-devkit-v1-wifi-development-board-pinout-diagram-and-reference/
// https://www.electroschematics.com/linear-hall-sensor/
// pin 26

// esc
// https://www.electronicshub.org/esp32-pwm-tutorial/
// https://www.electronicshub.org/esp32-servo-control/
// https://microcontrollerslab.com/esp32-pwm-arduino-ide-led-fading-example/
// https://esp32.com/viewtopic.php?t=20450
// pin 25

/* lcd: https://www.circuitschools.com/interfacing-16x2-lcd-module-with-esp32-with-and-without-i2c/
  * 
    PIN01-VSS -> GND
    PIN02-VDD -> 5V
    PIN03 V0-> 10K Pot (Middle pin)
    PIN04 RS->  GPIO19
    PIN05 RW-> GND
    PIN06  E  ->  GPIO23
    PIN07 D0-> NOT USED
    PIN08 D1-> NOT USED
    PIN09 D2-> NOT USED
    PIN10 D3-> NOT USED
    PIN11 D4->  GPIO18
    PIN12 D5->  GPIO17
    PIN13 D6->  GPIO16
    PIN14 D7->  GPIO15
    PIN15 A-> 5V
    PIN16 K-> GND
*/
#define Poten_pin 36
#define ESC_pin 25
#define Hall_pin 26

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


  // read the input on analog pin GPIX36 (A0; VP):
  // for potentiometer
  int analogValue = analogRead(Poten_pin);
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

 
