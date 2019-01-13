// Simple test of USB Host Joystick
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
byte data;

USBHost myusb;
USBHub hub1(myusb);
USBHIDParser hid1(myusb);
USBHIDParser hid2(myusb);
USBHIDParser hid3(myusb);
USBHIDParser hid4(myusb);
#define COUNT_JOYSTICKS 4
JoystickController joysticks[COUNT_JOYSTICKS](myusb);
int user_axis[64];
uint32_t buttons_prev = 0;

USBDriver *drivers[] = {&hub1, &joysticks[0], &joysticks[1], &joysticks[2], &joysticks[3], &hid1, &hid2, &hid3, &hid4};
#define CNT_DEVICES (sizeof(drivers)/sizeof(drivers[0]))
const char * driver_names[CNT_DEVICES] = {"Hub1", "joystick[0D]", "joystick[1D]", "joystick[2D]", "joystick[3D]",  "HID1", "HID2", "HID3", "HID4"};
bool driver_active[CNT_DEVICES] = {false, false, false, false};

// Lets also look at HID Input devices
USBHIDInput *hiddrivers[] = {&joysticks[0], &joysticks[1], &joysticks[2], &joysticks[3]};
#define CNT_HIDDEVICES (sizeof(hiddrivers)/sizeof(hiddrivers[0]))
const char * hid_driver_names[CNT_DEVICES] = {"joystick[0H]", "joystick[1H]", "joystick[2H]", "joystick[3H]"};
bool hid_driver_active[CNT_DEVICES] = {false};
bool show_changed_only = true;

uint8_t joystick_left_trigger_value[COUNT_JOYSTICKS] = {0};
uint8_t joystick_right_trigger_value[COUNT_JOYSTICKS] = {0};
uint64_t joystick_full_notify_mask = (uint64_t) - 1;

//=============================================================================
// Setup
//=============================================================================
void setup()
{
  // while (!Serial) ; // wait for Arduino Serial Monitor
  // Serial.println("\n\nUSB Host Joystick Testing");
  myusb.begin();

  analogWriteResolution(8);
  pinMode(gate_1, OUTPUT);
  pinMode(gate_2, OUTPUT);
  pinMode(gate_3, OUTPUT);
  pinMode(gate_4, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
}


//=============================================================================
// loop
//=============================================================================
void loop()
{
  myusb.Task();

  int joystick_index = 0;
  if (joysticks[joystick_index].available()) {
    uint64_t axis_mask = joysticks[joystick_index].axisMask();
    uint64_t axis_changed_mask = joysticks[joystick_index].axisChangedMask();
    uint32_t buttons = joysticks[joystick_index].getButtons();

    if (show_changed_only) {
      for (uint8_t i = 0; axis_changed_mask != 0; i++, axis_changed_mask >>= 1) {
        if (axis_changed_mask & 1 && i == 0) {
          data = joysticks[joystick_index].getAxis(0);
          Serial.println(data);
          analogWrite(A21, data);
        }
        else if (axis_changed_mask & 1 && i == 1) {
          data = joysticks[joystick_index].getAxis(1);
          Serial.println(data);
          analogWrite(A22, data);
        }
      }

    }


    if (buttons != buttons_prev) {
      digitalWriteFast(gate_1, bitRead(buttons, 0));
      digitalWriteFast(gate_2, bitRead(buttons, 1));
      digitalWriteFast(gate_3, bitRead(buttons, 2));
      digitalWriteFast(gate_4, bitRead(buttons, 3));

      uint8_t lr = (buttons & 1) ? 0xff : 0;
      uint8_t lg = (buttons & 2) ? 0xff : 0;
      uint8_t lb = (buttons & 4) ? 0xff : 0;
      if (buttons & 8) {
        lr = 255;
        lg = 255;
      }

      joysticks[joystick_index].setLEDs(lr, lg, lb);

      buttons_prev = buttons;
    }

    joysticks[joystick_index].joystickDataClear();
  }


}

//=============================================================================
// Show when devices are added or removed
//=============================================================================
void PrintDeviceListChanges() {
  for (uint8_t i = 0; i < CNT_DEVICES; i++) {
    if (*drivers[i] != driver_active[i]) {
      if (driver_active[i]) {
        Serial.printf("*** Device %s - disconnected ***\n", driver_names[i]);
        driver_active[i] = false;
      } else {
        Serial.printf("*** Device %s %x:%x - connected ***\n", driver_names[i], drivers[i]->idVendor(), drivers[i]->idProduct());
        driver_active[i] = true;

        const uint8_t *psz = drivers[i]->manufacturer();
        if (psz && *psz) Serial.printf("  manufacturer: %s\n", psz);
        psz = drivers[i]->product();
        if (psz && *psz) Serial.printf("  product: %s\n", psz);
        psz = drivers[i]->serialNumber();
        if (psz && *psz) Serial.printf("  Serial: %s\n", psz);
      }
    }
  }

  for (uint8_t i = 0; i < CNT_HIDDEVICES; i++) {
    if (*hiddrivers[i] != hid_driver_active[i]) {
      if (hid_driver_active[i]) {
        Serial.printf("*** HID Device %s - disconnected ***\n", hid_driver_names[i]);
        hid_driver_active[i] = false;
      } else {
        Serial.printf("*** HID Device %s %x:%x - connected ***\n", hid_driver_names[i], hiddrivers[i]->idVendor(), hiddrivers[i]->idProduct());
        hid_driver_active[i] = true;

        const uint8_t *psz = hiddrivers[i]->manufacturer();
        if (psz && *psz) Serial.printf("  manufacturer: %s\n", psz);
        psz = hiddrivers[i]->product();
        if (psz && *psz) Serial.printf("  product: %s\n", psz);
        psz = hiddrivers[i]->serialNumber();
        if (psz && *psz) Serial.printf("  Serial: %s\n", psz);
      }
    }
  }
}
