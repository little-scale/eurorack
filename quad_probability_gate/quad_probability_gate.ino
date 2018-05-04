// Quad Probability Gate
// by Sebastian Tomczak 4 May 2018

#include <SoftPWM.h>
#include <SoftPWM_timer.h>

int gate_in = 1;
int gate_out1 = 4;
int gate_out2 = 6;
int gate_out3 = 8;
int gate_out4 = 10;
int ledclock = 2;
int ledcv = 0;
int led1 = 3;
int led2 = 5;
int led3 = 7;
int led4 = 9;
int probability_in = A0;
int seed_in = A5;
int gate_flag = 0;
int data;
int random_value;

void setup() {
  pinMode(gate_in, INPUT);
  pinMode(gate_out1, OUTPUT);
  pinMode(gate_out2, OUTPUT);
  pinMode(gate_out3, OUTPUT);
  pinMode(gate_out4, OUTPUT);

  pinMode(ledclock, OUTPUT);
  pinMode(ledcv, OUTPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  digitalWrite(led1, HIGH); 
  delay(100); 
  digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH); 
  delay(100); 
  digitalWrite(led2, LOW);
  digitalWrite(led3, HIGH); 
  delay(100); 
  digitalWrite(led3, LOW);
  digitalWrite(led4, HIGH); 
  delay(100); 
  digitalWrite(led4, LOW);
  digitalWrite(ledclock, HIGH); 
  delay(100); 
  digitalWrite(ledclock, LOW);
  digitalWrite(ledcv, HIGH); 
  delay(100); 
  digitalWrite(ledcv, LOW);
  SoftPWMBegin();
  SoftPWMSet(ledcv,0);

  randomSeed(analogRead(seed_in));

  
}

void loop() {
  data = analogRead(probability_in);
  SoftPWMSet(ledcv, data >> 2);

  if (digitalRead(gate_in) == 1 && gate_flag == 0) {
    gate_flag = 1;
    digitalWrite(ledclock, HIGH);

    random_value = random(1023);
    if (data > random_value) {
      digitalWrite(gate_out1, HIGH);
      digitalWrite(led1, HIGH);
    }

    random_value = random(1023);
    if (data > random_value) {
      digitalWrite(gate_out2, HIGH);
      digitalWrite(led2, HIGH);
    }

    random_value = random(1023);
    if (data > random_value) {
      digitalWrite(gate_out3, HIGH);
      digitalWrite(led3, HIGH);
    }

    random_value = random(1023);
    if (data > random_value) {
      digitalWrite(gate_out4, HIGH);
      digitalWrite(led4, HIGH);
    }
  }

  if (digitalRead(gate_in) == 0 && gate_flag == 1) {
    gate_flag = 0;
    digitalWrite(ledclock, LOW);
    digitalWrite(gate_out1, LOW);
    digitalWrite(led1, LOW);
    digitalWrite(gate_out2, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(gate_out3, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(gate_out4, LOW);
    digitalWrite(led4, LOW);
  }

}
