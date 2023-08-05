#include "json_messages.h"
#include <ArduinoJson.h>

DynamicJsonDocument doc(1024);
uint8_t type = 0;
voltage_engine_message voltage_message{ 1, 0 };
direction_change_message direction_message{ 2, 0 };
led_change_message led_message{3, 0, 0, 0, 0};

String password_bt = "";
String ssid_bt = "";

uint8_t new_message(char* payload) {
  deserializeJson(doc, payload);

  type = doc["type"];

  switch (type) {
    case 1: set_voltage(); break;
    case 2: set_direction(); break;
    case 3: set_lights(); break;
  }

  return type;
}

void set_voltage() {
  voltage_message.voltage = doc["voltage"];
}

void set_direction() {
  direction_message.direction = doc["direction"];
}

void set_lights() {
  led_message.left_dir_light = doc["left_dir_light"] ? 1 : 0;
  led_message.right_dir_light = doc["right_dir_light"] ? 1 : 0;
  led_message.front_light = doc["front_light"] ? 1 : 0;
  led_message.auto_lights = doc["auto_lights"] ? 1 : 0;
}

uint8_t bt_message(char* payload) {
  deserializeJson(doc, payload);

  password_bt = (const char*)doc["password"];
  ssid_bt = (const char*)doc["ssid"];

  return 1;
}