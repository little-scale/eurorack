#include <USBHost_t36.h>

const int interval_time = 90;
const int in1 = 16;
byte current_tick;
byte previous_tick;

const byte Gr = 0;

const byte R1 = 1;
const byte R2 = 2;
const byte R3 = 3;

const byte G1 = 16;
const byte G2 = 32;
const byte G3 = 48;

const byte Y1 = 17;
const byte Y2 = 33;
const byte Y3 = 50;

const byte O1 = 18;
const byte O2 = 19;
const byte O3 = 34;
const byte O4 = 35;
const byte O5 = 51;

const byte YG = 49;

byte pattern[128];

USBHost myusb;
USBHub hub1(myusb);
USBHub hub2(myusb);
MIDIDevice midi1(myusb);

elapsedMillis clock_count;
byte step_count;
byte round_count;

void setup() {
  for (int i = 0; i < 8; i ++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  pinMode(in1, INPUT_PULLUP);

  delay(1500);
  myusb.begin();
  midi1.setHandleNoteOn(myNoteOn);
  midi1.setHandleNoteOff(myNoteOff);
  midi1.setHandleControlChange(myControlChange);
}



void loop() {
  myusb.Task();
  midi1.read();
  current_tick = digitalReadFast(in1);
  if (current_tick != previous_tick) {
    previous_tick = current_tick;
    if (current_tick == HIGH) {
      doStep();
    }
  }
}

void myNoteOn(byte channel, byte note, byte velocity) {
  if (note % 16 < 8) {
    pattern[note] = (pattern[note] + 1) % 4;

    if (pattern[note] == 1) {
      setGrid(note % 16, note / 16, R1);
    }

    else {
      setGrid(note % 16, note / 16, Gr);
    }

  }

}

void myNoteOff(byte channel, byte note, byte velocity) {
  // midi1.sendNoteOff(note, velocity, channel);
}

void myControlChange(byte channel, byte control, byte value) {
  midi1.sendControlChange(control, value, channel);
}


void doStep() {
  for (int i = 0; i < 8; i ++) {
    if (pattern[(i * 16) + step_count] == 1) {
      setGrid(step_count, i, R1);
    }
    else if (pattern[(i * 16) + step_count] == 2 && round_count == 0) {
      setGrid(step_count, i, R1);
    }

    else if (pattern[(i * 16) + step_count] == 3 && round_count == 1) {
      setGrid(step_count, i, R1);
    }
    else {
      setGrid(step_count, i, Gr);
    }
  }

  step_count ++;
  if (step_count > 7) {
    step_count = 0;
    round_count = 1 - round_count;
  }

  for (int i = 0; i < 8; i ++) {
    if (pattern[(i * 16) + step_count] == 1) {
      setGrid(step_count, i, O4);
      midi1.sendNoteOn((i * 16) + 15, 127, 1);
      digitalWrite(7 - i , HIGH);
    }
    else if (pattern[(i * 16) + step_count] == 2 && round_count == 0) {
      setGrid(step_count, i, O4);
      midi1.sendNoteOn((i * 16) + 15, 127, 1);
      digitalWrite(7 - i , HIGH);
    }

    else if (pattern[(i * 16) + step_count] == 3 && round_count == 1) {
      setGrid(step_count, i, O4);
      midi1.sendNoteOn((i * 16) + 15, 127, 1);
      digitalWrite(7 - i , HIGH);
    }

    else {
      setGrid(step_count, i, G3);
      midi1.sendNoteOn((i * 16) + 15, 0, 1);
      digitalWrite(7 - i , LOW);
    }
  }

  delay(3);

  for (int i = 0; i < 8; i ++) {
    digitalWrite(7 - i , LOW);
  }


}



void setGrid(byte x_value, byte y_value, byte colour) {
  midi1.sendNoteOn(x_value + (y_value * 16), colour, 1);
}
