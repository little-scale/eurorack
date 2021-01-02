#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

const int LED = 13; 
const int GATE_SYNC = 10; 

Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

int clk = 0; 

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  pinMode(LED, OUTPUT); 
  pinMode(GATE_SYNC, OUTPUT); 
  digitalWrite(LED, HIGH); 

  MIDI.setHandleClock(doClock); 
  MIDI.setHandleStart(doStart); 
  MIDI.setHandleStop(doStop); 
  MIDI.setHandleContinue(doContinue); 

}

void loop() {
  MIDI.read(); 

}

void doClock () {
  digitalWrite(LED, (clk / 3) & 1);
  digitalWrite(GATE_SYNC, (1 - (clk / 3)) & 1); 
  
  clk++;
  if(clk >= 24) {
    clk = 0; 
  }
}

void doStart () {
  clk = 0; 
}

void doStop() {
  digitalWrite(LED, HIGH);
  digitalWrite(GATE_SYNC, LOW);
}

void doContinue () {
  clk = 0; 
}
