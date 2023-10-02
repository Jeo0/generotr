#include <FastLED.h>
#define DISPLAY_PIN 4
#define TOTAL_LEDS 100
#define TRAFFIC_LEDS 5
#define HEADLIGHT 1



CRGB leds[TOTAL_LEDS];

void setup() {
	FastLED.addLeds<NEOPIXEL, DISPLAY_PIN> (leds, TOTAL_LEDS);
	//FastLED.setBrightness(10);
}

void loop() {
    

	for(int f = 0; f < TOTAL_LEDS; f++){
		leds[f].setRGB(0,0,10);
		FastLED.show();
    delay(10);
	}
  for (int i = (TOTAL_LEDS-1); i > -1; i--){
    leds[i].setRGB(0,0,0);
		FastLED.show();
    delay(10);
  }
}
