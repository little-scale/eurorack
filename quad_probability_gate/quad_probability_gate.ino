 // Quad Probability Gate
// by Sebastian Tomczak 7 April 2018

int gate_in = 0;
int gate_out1 = 1;
int gate_out2 = 2;
int gate_out3 = 3;
int gate_out4 = 4;
int ledclock = 5;
int ledcv = 6;
int led1 = 9;
int led2 = 10;
int led3 = 11;
int led4 = 12;
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

  randomSeed(analogRead(seed_in));
}

void loop() {
  data = analogRead(probability_in);
  analogWrite(ledcv, data >> 2);

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
