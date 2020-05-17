#include <FastLED.h>

#define LED_PIN     32
#define NUM_LEDS    150
#define BRIGHTNESS  30
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

int hue;
int hue_previous;

int bright; 
int bright_previous; 

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


void setup() {
  delay(3000); // power-up safety delay
  analogReadResolution(12);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(100);

  Serial.begin(57600);
}


void loop()
{
  hue = analogRead(A14);
  hue = map(hue, 0, 4095, 0, NUM_LEDS); 

  bright = analogRead(A15);
  bright = map(bright, 0, 4095, 0, 255); 

  if (hue != hue_previous) {
    for ( int i = 0; i < NUM_LEDS; i++) {
      hue_previous = hue; 
      leds[i] = CHSV((i / 2) + hue + 120, 255, 255);
    }

    FastLED.show();
  }

  if (bright != bright_previous) {
    bright_previous = bright; 
    FastLED.setBrightness(bright);
    FastLED.show();
  }
}
