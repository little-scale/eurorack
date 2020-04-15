/*
 * This is a USB-powered, Teensy-based interface that has 16 CV outputs and 12 gate outputs. 
 * USB MIDI channels 1 - 8 are mapped to CV outputs 1 - 16 for pitch and velocity, and gates 1 - 8 for note on and off events. 
 * Gates 9 - 12 are mapped to note on and off events only on channel 9, and also will send a sync and transport signal based on MIDI clock messages if received. 
 * CV is 12 bit from 0V to 4V, gates 1 - 8 are 0 and 5V, and gates 9 - 12 are 0 and 3.3v.
 * 
 * v2 - 15 April 2020
 * 
 * Added Modes. 
 * Mode 0 = Voices and Percussion. Note On / Off pitch and velocity channels 1 - 8 controls gate, pitch CV and velocity CV. 
 * Mode 1 = Independent Single Channel. Note On / Off channel 1 notes 0 - 19 controls gates, channel 1 CC 0 - 15 controls CV 
 * Mode 2 = Independent High Resolution. Note On / Off channel 1 notes 0 - 19 controls gates, channel 1 - 16 pitchbend controls CV
 * 
 * Update pitch change functions for currrent Teensyduino. 
 */

/*
 * This is a USB-powered, Teensy-based interface that has 16 CV outputs and 12 gate outputs. 
 * USB MIDI channels 1 - 8 are mapped to CV outputs 1 - 16 for pitch and velocity, and gates 1 - 8 for note on and off events. 
 * Gates 9 - 12 are mapped to note on and off events only on channel 9, and also will send a sync and transport signal based on MIDI clock messages if received. 
 * CV is 12 bit from 0V to 4V, gates 1 - 8 are 0 and 5V, and gates 9 - 12 are 0 and 3.3v.
 * 
 * v2 - 15 April 2020
 * 
 * Added Modes. 
 * Mode 0 = Voices and Percussion. Note On / Off pitch and velocity channels 1 - 8 controls gate, pitch CV and velocity CV. 
 * Mode 1 = Independent Single Channel. Note On / Off channel 1 notes 0 - 19 controls gates, channel 1 CC 0 - 15 controls CV 
 * Mode 2 = Independent High Resolution. Note On / Off channel 1 notes 0 - 19 controls gates, channel 1 - 16 pitchbend controls CV
 * 
 * Update pitch change functions for currrent Teensyduino. 
 */

#include <SPI.h>
const int pitchbend_value_positive = 1200;
const int pitchbend_value_negative = -1200;
const float offset_pitch = 60;


const int LED = 13;

int cs_pin =  21;
int gate_pin = 7;
int pitchbend_value[8];
int pitch_values[8];
int cs9 = 2;
byte shadow_gate;

byte clock_tick;
byte clock_value;
byte play_flag;
byte play_tick;

float voltage_range = 4.024 * 1200;

int mode = 0;
// mode 0 = Note On / Off pitch and velocity channels 1 - 8 controls gate, pitch CV and velocity CV
// mode 1 = Note On / Off channel 1 notes 0 - 19 controls gates, channel 1 CC 0 - 15 controls CV
// mode 2 = Note On / Off channel 1 notes 0 - 19 controls gates, channel 1 - 16 pitchbend controls CV

void setup() {
  SPI.begin();
  pinMode(cs9, OUTPUT);
  for (int i = 0; i < 8; i ++) {
    pinMode(cs_pin - i, OUTPUT);
    digitalWriteFast(cs_pin - i, HIGH);
    writeGate(i, HIGH);
    delay(50);
  }

  for (int i = 0; i < 4; i ++) {
    pinMode(gate_pin + i, OUTPUT);
    digitalWrite(gate_pin + i, HIGH);
    delay(50);
  }
  delay(100);

  for (int i = 0; i < 8; i ++) {
    writeGate(i, LOW);
  }

  for (int i = 0; i < 4; i ++) {
    digitalWrite(gate_pin + i, LOW);
  }
// voice and percussion mode
  if (mode == 0) { // Note On / Off pitch and velocity channels 1 - 8 controls gate, pitch CV and velocity CV
    usbMIDI.setHandleNoteOn(OnNoteOn);
    usbMIDI.setHandleNoteOff(OnNoteOff);
    usbMIDI.setHandlePitchChange(OnPitchChange);
    usbMIDI.setHandleRealTimeSystem(OnClock);
  }

// independent output, single channel mode
  else if (mode == 1) { // Note On / Off channel 1 notes 0 - 19 controls gates, channel 1 CC 0 - 15 controls CV
    usbMIDI.setHandleNoteOn(OnNoteOn_mode_1);
    usbMIDI.setHandleNoteOff(OnNoteOff_mode_1);
    usbMIDI.setHandleControlChange(OnCC_mode_1);
  }

// independent output, high res mode
  else if (mode == 2) { // Note On / Off channel 1 notes 0 - 19 controls gates, channel 1 - 16 pitchbend controls CV
    usbMIDI.setHandleNoteOn(OnNoteOn_mode_1);
    usbMIDI.setHandleNoteOff(OnNoteOff_mode_1);
    usbMIDI.setHandlePitchChange(OnPitchChange_mode_2);
  }
  
  Serial.begin(57600);
}

void loop() {
  usbMIDI.read();
}

void OnNoteOn(byte channel, byte pitch, byte velocity) {
  if (channel < 9) {
    pitch_values[channel - 1] = pitch;
    writeDAC(cs_pin - channel + 1, 1, constrain(map((pitch - offset_pitch) * 100.0 + pitchbend_value[channel - 1], 0.0, voltage_range, 0.0, 4095.0), 0.0, 4095.0));
    writeDAC(cs_pin - channel + 1, 0, map(velocity, 0, 127, 0, 4095));
    if (velocity > 0) {
      writeGate(channel - 1, HIGH);
    }
    else {
      writeGate(channel - 1, LOW);
    }
  }
  else if (channel == 9) {
    if (velocity > 0) {
      digitalWriteFast(gate_pin + (3  - (pitch % 4)), HIGH);
    }

    else {
      digitalWriteFast(gate_pin + (3  - (pitch % 4)), LOW);
    }
  }
}

void OnNoteOn_mode_1(byte channel, byte pitch, byte velocity) {
  if (channel == 1) {
    pitch = pitch % 12;

    if (pitch < 8) {
      if (velocity > 0) {
        writeGate(pitch, HIGH);
      }
      else {
        writeGate(pitch, LOW);
      }
    }

    else {
      pitch = pitch - 8;
      if (velocity > 0) {
        digitalWriteFast(gate_pin + 3  - pitch, HIGH);
      }

      else {
        digitalWriteFast(gate_pin + 3  - pitch, LOW);
      }
    }
  }
}

void OnNoteOff(byte channel, byte pitch, byte velocity) {
  if (channel < 9) {
    writeGate(channel - 1, LOW);
  }

  else {
    digitalWriteFast(gate_pin + (3  - (pitch % 4)), LOW);
  }
}

void OnNoteOff_mode_1(byte channel, byte pitch, byte velocity) {
  if (channel == 1) {
    pitch = pitch % 12;

    if (pitch < 8) {
      writeGate(pitch, LOW);
    }

    else {
      pitch = pitch - 8;
      digitalWriteFast(gate_pin + 3  - pitch, LOW);
    }
  }
}

void OnPitchChange (byte channel, int pitch_change) {
  if (channel < 9) {
    pitch_change = pitch_change + 8192; 
    pitchbend_value[channel - 1] = map(pitch_change, 0, 16383, pitchbend_value_negative, pitchbend_value_positive);
    writeDAC(cs_pin - channel + 1, 1, constrain(map((pitch_values[channel - 1] - offset_pitch) * 100.0 + pitchbend_value[channel - 1], 0.0, voltage_range, 0.0, 4095.0), 0.0, 4095.0));
  }
}

void OnPitchChange_mode_2 (byte channel, int pitch_change) {
  channel = channel - 1; 
  pitch_change = pitch_change + 8192; 
  writeDAC(cs_pin - (channel >> 1), channel & 1, pitch_change >> 2);
}

void OnCC_mode_1(byte channel, byte cc_number, byte cc_value) {
  if(channel == 1) {
    if(cc_number < 16) {
      writeDAC(cs_pin - (cc_number >> 1), cc_number & 1, cc_value << 5); 
    }
  }
}

void writeDAC (int cs, int dac, int val) {
  digitalWriteFast(cs, LOW);
  dac = dac & 1;
  val = val & 4095;
  SPI.transfer(dac << 7 | 0 << 5 | 1 << 4 | val >> 8);
  SPI.transfer(val & 255);
  digitalWriteFast(cs, HIGH);
}

void writeGate(byte bit_number, byte bit_value) {
  bitWrite(shadow_gate, bit_number, bit_value);
  digitalWriteFast(cs9, LOW);
  SPI.transfer(shadow_gate);
  digitalWriteFast(cs9, HIGH);
}

void OnClock(byte clockbyte) {
  if (clockbyte == 0xf8 && play_flag == 1) {
    digitalWriteFast(9, 1 - bitRead(clock_tick, 0)); // 12 ppqn
    digitalWriteFast(8, 1 - bitRead(clock_tick / 3, 0)); // sixteenths
    digitalWriteFast(7, 1 - bitRead(clock_tick / 6, 0)); // eights
    digitalWriteFast(LED, 1 - bitRead(clock_tick / 12, 0)); // quarter note LED flash

    clock_tick ++;

    if (clock_tick == 24) {
      clock_tick = 0;
    }

    if (clock_tick == 6 && play_tick == 1) {
      play_tick = 0;
      digitalWriteFast(10, LOW);
    }
  }

  if (clockbyte == 0xfa || clockbyte == 0xfb) { // start or continue bytes
    play_flag = 1;
    play_tick = 1;
    clock_value = 0;
    clock_tick = 0;
    digitalWriteFast(10, HIGH);
  }

  if (clockbyte == 0xfc) { // stop byte
    digitalWriteFast(10, LOW);
    digitalWriteFast(9, LOW);
    digitalWriteFast(8, LOW);
    digitalWriteFast(7, LOW);
    digitalWriteFast(LED, LOW);
    play_flag = 0;
  }
}
