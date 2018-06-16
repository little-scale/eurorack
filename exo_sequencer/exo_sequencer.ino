// exo sequencer module by Sebastian Tomczak
// v 1.00 16 June 2018

const int step_pin[16] = { // Step Pins
  11, 12, 26, 13,
  9, 10, 14, 15,
  7, 8, 16, 22,
  6, 25, 18, 19
}; 

const int address[4] = { // Address Pins
  5, 4, 2, 3 
};

const int inc = 0; // Increment Pin 
const int dec = 1; // Decrement Pin 
const int pattern_length_pin = A6;
const int pattern_offset_pin = A7;
const int pattern_step_pin = A9;
const int trig = 17; // Trigger Pin

int trig_delay = 5;

int pattern_length = 15;
int pattern_offset = 0;
int pattern_step;

int current_pattern_step;

byte current_address;
byte previous_address;
int pattern_step_previous;
int inc_value;
int inc_value_previous;
int dec_value;
int dec_value_previous;

void setup() {
  analogReadAveraging(8);

  for (int i = 0; i < 16; i ++) {
    pinMode(step_pin[i], OUTPUT);
    digitalWrite(step_pin[i], HIGH);
    delay(80); 
    digitalWrite(step_pin[i], LOW);
  }

  for (int i = 0; i < 4; i ++) {
    pinMode(address[i], INPUT_PULLUP);
  }

  pinMode(trig, OUTPUT);

  pinMode(dec, INPUT_PULLUP);
  pinMode(inc, INPUT_PULLUP);

  pattern_length = analogRead(pattern_length_pin) >> 6;
  pattern_offset = analogRead(pattern_offset_pin) >> 6;
  pattern_step = analogRead(pattern_step_pin);
  pattern_step_previous = analogRead(pattern_step_pin);

  previous_address = digitalReadFast(address[3]) << 3 | digitalReadFast(address[2]) << 2 | digitalReadFast(address[1]) << 1 | digitalReadFast(address[0]) << 0;
  current_pattern_step = map(pattern_step, 0, 15, 0, pattern_length);
    setStep(current_address + current_pattern_step);
    doTrig();
}

void loop() {
  pattern_length = analogRead(pattern_length_pin) >> 6;
  pattern_offset = analogRead(pattern_offset_pin) >> 6;
  pattern_step = analogRead(pattern_step_pin) >> 6;

  if (pattern_step != pattern_step_previous) {
    pattern_step_previous = pattern_step;
    current_pattern_step = map(pattern_step, 0, 15, 0, pattern_length);
    setStep(current_address + current_pattern_step);
    doTrig();
  }

  current_address = digitalReadFast(address[3]) << 3 | digitalReadFast(address[2]) << 2 | digitalReadFast(address[1]) << 1 | digitalReadFast(address[0]) << 0;

  if (current_address ^ previous_address) {
    previous_address = current_address;
    setStep(current_address + current_pattern_step);
    doTrig();
  }

  inc_value = digitalReadFast(inc);

  if (inc_value != inc_value_previous) {
    inc_value_previous = inc_value;
    if (inc_value == 1) {
      current_pattern_step = (current_pattern_step + 1) % (pattern_length + 1);
      setStep(current_address + current_pattern_step);
      doTrig();
    }
  }

  dec_value = digitalReadFast(dec);

  if (dec_value != dec_value_previous) {
    dec_value_previous = dec_value;
    if (dec_value == 1) {
      current_pattern_step = (current_pattern_step + pattern_length) % (pattern_length + 1);
      setStep(current_address + current_pattern_step);
      doTrig();
    }
  }

}

void setStep(byte step_out) {

  step_out = (pattern_offset + step_out) & 0x0f;

  for (int i = 0; i < 16; i ++) {
    digitalWriteFast(step_pin[i], LOW);
  }
  digitalWriteFast(step_pin[step_out], HIGH);
}

void doTrig() {
  digitalWriteFast(trig, HIGH);
  delay(trig_delay);
  digitalWriteFast(trig, LOW);
  delay(trig_delay);
}

