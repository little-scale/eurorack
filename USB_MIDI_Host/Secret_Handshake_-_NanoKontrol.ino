#include <USBHost_t36.h>
const int LED = 13;
const int in_1 = 15;
const int in_2 = 16;
const int gate_1 = 2;
const int gate_2 = 3;
const int gate_3 = 4;
const int gate_4 = 5;
const int cv_out_2 = A21;
const int cv_out_1 = A22;

USBHost myusb;
USBHub hub1(myusb);
USBHub hub2(myusb);
MIDIDevice midi1(myusb);

byte faders[8];
byte knobs[8];
byte solos[8];
byte mutes[8];

byte data;
byte clock_tick;
byte clock_count;
byte current_reading;
byte previous_reading;


void setup() {
  analogWriteResolution(7);
  analogReadResolution(3);
  pinMode(in_2, INPUT);
  pinMode(gate_1, OUTPUT);
  pinMode(gate_2, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWriteFast(LED, HIGH);
  Serial.begin(57600);
  delay(1500);
  myusb.begin();
  midi1.setHandleControlChange(myControlChange);


}

void loop() {
  myusb.Task();
  midi1.read();

  current_reading = analogRead(A1);

  if (current_reading != previous_reading) {
    previous_reading = current_reading;
    analogWrite(A21, faders[current_reading]);
    analogWrite(A22, knobs[current_reading]);
    digitalWriteFast(gate_1, solos[current_reading]);
    digitalWriteFast(gate_2, mutes[current_reading]);
    for (int i = 0; i < 8; i ++) {
      midi1.sendControlChange(i + 64, 0, 1);
    }
    midi1.sendControlChange(current_reading + 64, 127, 1);
    delay(3);
    digitalWriteFast(gate_1, LOW);
    digitalWriteFast(gate_2, LOW);
  }



  data = digitalReadFast(in_2);


  if (data != clock_tick) {
    clock_tick = data;
    if (clock_tick == 1) {
      digitalWriteFast(LED, HIGH);
      doClock();
    }
    else if (clock_tick == 0) {
      digitalWriteFast(LED, LOW);
      doClockOff();
    }
  }

}

void myControlChange(byte channel, byte control, byte value) {
  if (channel == 1) {
    if (control >= 0 && control <= 7) {
      faders[control] = value;
    }

    else if (control >= 16 && control <= 23) {
      knobs[control - 16] = value;
    }

    else if (control >= 32 && control <= 39) {
      midi1.sendControlChange(control, value, channel);
      solos[control - 32] = value & 1;
    }

    else if (control >= 48 && control <= 55) {
      midi1.sendControlChange(control, value, channel);
      mutes[control - 48] = value & 1;
    }
  }
}

void doClock() {
  clock_count ++;
  if (clock_count > 7) {
    clock_count = 0;
  }

  analogWrite(A21, faders[clock_count]);
  analogWrite(A22, knobs[clock_count]);
  digitalWriteFast(gate_1, solos[clock_count]);
  digitalWriteFast(gate_2, mutes[clock_count]);
  midi1.sendControlChange(clock_count + 64, 127, 1);
}

void doClockOff() {
  digitalWriteFast(gate_1, LOW);
  digitalWriteFast(gate_2, LOW);
  midi1.sendControlChange(clock_count + 64, 0, 1);
}
