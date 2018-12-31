const int LED = 13; 

byte clock_tick; 
byte clock_value; 
byte play_flag; 
byte play_tick; 

void setup() {
  for (int i = 0; i < 12; i ++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  pinMode(LED, OUTPUT); 

  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleRealTimeSystem(OnClock); 
}

void loop() {
  usbMIDI.read();
}

void OnNoteOn (byte channel, byte pitch, byte velocity) {
  if (channel == 1) {
    if (velocity > 0) {
      digitalWrite(pitch % 12, HIGH);
      digitalWrite(LED, HIGH);
    }
    else {
      digitalWrite(pitch % 12, LOW);
      digitalWrite(LED, LOW);
    }
  }
}

void OnNoteOff (byte channel, byte pitch, byte velocity) {
  if (channel == 1) {
    digitalWrite(pitch % 12, LOW);
    digitalWrite(LED, LOW);
  }
}

void OnClock(byte clockbyte) {
  if(clockbyte == 0xf8 && play_flag == 1) {
    digitalWriteFast(10, 1 - bitRead(clock_tick, 0)); // 12 ppqn
    digitalWriteFast(9, 1 - bitRead(clock_tick / 3, 0)); // sixteenths
    digitalWriteFast(8, 1 - bitRead(clock_tick / 6, 0)); // eights 
    digitalWriteFast(LED, 1 - bitRead(clock_tick / 12, 0)); // quarter note LED flash
    
    clock_tick ++; 

    if(clock_tick == 24) {
      clock_tick = 0; 
    }

    if(clock_tick == 3 && play_tick == 1) {
      play_tick = 0;
      digitalWriteFast(11, LOW);
    }
  }
  
  if(clockbyte == 0xfa || clockbyte == 0xfb) { // start or continue bytes
    play_flag = 1; 
    play_tick = 1;
    clock_value = 0; 
    clock_tick = 0; 
    digitalWriteFast(11, HIGH); 
  }

  if(clockbyte == 0xfc) { // stop byte
    digitalWriteFast(11, LOW);
    digitalWriteFast(10, LOW); 
    digitalWriteFast(9, LOW);
    digitalWriteFast(8, LOW);
    digitalWriteFast(LED, LOW);
    play_flag = 0; 
  }
}

