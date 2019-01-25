// Simple test of USB Host Mouse/Keyboard
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

byte current_velocity = 127; 
byte current_octave;
byte current_pitch;
byte play_flag = 0; 
int max_voltage = 3950; // in microvolts

byte data;
byte clock_tick;
byte clock_count;
byte current_reading;
byte previous_reading;


USBHost myusb;
USBHub hub1(myusb);
USBHub hub2(myusb);
KeyboardController keyboard1(myusb);
KeyboardController keyboard2(myusb);
USBHIDParser hid1(myusb);
USBHIDParser hid2(myusb);
USBHIDParser hid3(myusb);
USBHIDParser hid4(myusb);
USBHIDParser hid5(myusb);
MouseController mouse1(myusb);
JoystickController joystick1(myusb);
int user_axis[64];
uint32_t buttons_prev = 0;
RawHIDController rawhid1(myusb);
RawHIDController rawhid2(myusb, 0xffc90004);

USBDriver *drivers[] = {&hub1, &hub2, &keyboard1, &keyboard2, &joystick1, &hid1, &hid2, &hid3, &hid4, &hid5};
#define CNT_DEVICES (sizeof(drivers)/sizeof(drivers[0]))
const char * driver_names[CNT_DEVICES] = {"Hub1", "Hub2", "KB1", "KB2", "JOY1D", "HID1", "HID2", "HID3", "HID4", "HID5"};
bool driver_active[CNT_DEVICES] = {false, false, false, false};

// Lets also look at HID Input devices
USBHIDInput *hiddrivers[] = {&mouse1, &joystick1, &rawhid1, &rawhid2};
#define CNT_HIDDEVICES (sizeof(hiddrivers)/sizeof(hiddrivers[0]))
const char * hid_driver_names[CNT_DEVICES] = {"Mouse1", "Joystick1", "RawHid1", "RawHid2"};
bool hid_driver_active[CNT_DEVICES] = {false, false};
bool show_changed_only = false;

uint8_t joystick_left_trigger_value = 0;
uint8_t joystick_right_trigger_value = 0;
uint64_t joystick_full_notify_mask = (uint64_t) - 1;

void setup()
{
  myusb.begin();
  keyboard1.attachPress(OnPress);
  keyboard2.attachPress(OnPress);
  keyboard1.attachExtrasPress(OnHIDExtrasPress);
  keyboard1.attachExtrasRelease(OnHIDExtrasRelease);
  keyboard2.attachExtrasPress(OnHIDExtrasPress);
  keyboard2.attachExtrasRelease(OnHIDExtrasRelease);

  rawhid1.attachReceive(OnReceiveHidData);
  rawhid2.attachReceive(OnReceiveHidData);

  analogWriteResolution(12);
  pinMode(gate_1, OUTPUT);
  digitalWriteFast(gate_1, LOW); 
}


void loop()
{
  myusb.Task();
  if(play_flag == 1) {
    play_flag = 0; 
    analogWrite(A22, map(constrain((current_pitch * 100) + (current_octave * 1200), 0, max_voltage), 0, max_voltage, 0, 4095));
    analogWrite(A21, map(current_velocity, 0, 127, 0, 4095)); 
    digitalWriteFast(gate_1, HIGH); 
    delay(5); 
    digitalWriteFast(gate_1, LOW); 
  }

}



void OnPress(int key)
{
  switch (key) {
    case 97 : current_pitch = 0; play_flag = 1; break;
    case 119 : current_pitch = 1; play_flag = 1; break;
    case 115 : current_pitch = 2; play_flag = 1; break;
    case 101 : current_pitch = 3; play_flag = 1; break;
    case 100 : current_pitch = 4; play_flag = 1; break;
    case 102 : current_pitch = 5; play_flag = 1; break;
    case 116 : current_pitch = 6; play_flag = 1; break;
    case 103 : current_pitch = 7; play_flag = 1; break;
    case 121 : current_pitch = 8; play_flag = 1; break;
    case 104 : current_pitch = 9; play_flag = 1; break;
    case 117 : current_pitch = 10; play_flag = 1; break;
    case 106 : current_pitch = 11; play_flag = 1; break;
    case 107 : current_pitch = 12; play_flag = 1; break;
    case 122 : current_octave = constrain(current_octave - 1, 0, 2); break;
    case 120 : current_octave = constrain(current_octave + 1, 0, 2); break;
    case 99 : current_velocity = constrain(current_velocity - 10, 0, 127); break;
    case 118 : current_velocity = constrain(current_velocity + 10, 0, 127); break;
  }


  if (keyboard1) {

  } else {

  }

}
void OnHIDExtrasPress(uint32_t top, uint16_t key)
{

}

void OnHIDExtrasRelease(uint32_t top, uint16_t key)
{

}

bool OnReceiveHidData(uint32_t usage, const uint8_t *data, uint32_t len) {
  // Called for maybe both HIDS for rawhid basic test.  One is for the Teensy
  // to output to Serial. while still having Raw Hid...
  if (usage == 0xffc90004) {
    // Lets trim off trailing null characters.
    while ((len > 0) && (data[len - 1] == 0)) {
      len--;
    }
    if (len) {
      Serial.print("RawHid Serial: ");
      Serial.write(data, len);
    }
  } else {
    Serial.print("RawHID data: ");
    Serial.println(usage, HEX);
    while (len) {
      uint8_t cb = (len > 16) ? 16 : len;
      const uint8_t *p = data;
      uint8_t i;
      for (i = 0; i < cb; i++) {
        Serial.printf("%02x ", *p++);
      }
      Serial.print(": ");
      for (i = 0; i < cb; i++) {
        Serial.write(((*data >= ' ') && (*data <= '~')) ? *data : '.');
        data++;
      }
      len -= cb;
      Serial.println();
    }
  }

  return true;
}
