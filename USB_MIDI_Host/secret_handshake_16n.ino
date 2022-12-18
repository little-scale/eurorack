// Simple test of USB Host
//
// This example is in the public domain

#include "USBHost_t36.h"

const int LED = 13;
const int in_1 = 15;
const int in_2 = 16;
const int gate_1 = 2;
const int gate_2 = 3;
const int gate_3 = 4;
const int gate_4 = 5;
const int cv_out_2 = A21;
const int cv_out_1 = A22;

int current;
int previous;
int clk = 0;

int sliders[16];

USBHost myusb;
MIDIDevice midi1(myusb);

void setup() {
  Serial.begin(57600);
  Serial.println("Secret Handshake 16n");
  myusb.begin();

  midi1.setHandleControlChange(OnControlChange);

  pinMode(gate_1, OUTPUT);
  pinMode(gate_2, OUTPUT);
  pinMode(gate_3, OUTPUT);
  pinMode(gate_4, OUTPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(LED, HIGH);

  analogWrite(A22, 0);
  analogWrite(A21, 0);

  pinMode(in_1, INPUT);
  pinMode(in_2, INPUT);

  analogWriteResolution(7);
}

void loop() {
  myusb.Task();
  midi1.read();
  doClock();
  if(digitalRead(in_2) == HIGH) {
    clk = 0;
  }

}

void OnControlChange(byte channel, byte control, byte value) {
  if (channel == 1 && control >= 32 && control <= 47) {
    sliders[control - 32] = value;
  }
}

void doClock() {
  current = digitalRead(in_1);
  if (current != previous) {
    previous = current;
    if (current == 1) {
      clk++;
      clk &= 0x07;
      analogWrite(A22, sliders[clk]);
      analogWrite(A21, sliders[clk + 8]);

      digitalWrite(gate_1, HIGH);
      digitalWrite(gate_2, HIGH);
      digitalWrite(LED, HIGH);
    } else if (current == 0) {
      digitalWrite(gate_1, LOW);
      digitalWrite(gate_2, LOW);
      digitalWrite(LED, LOW);
    }
  }
}