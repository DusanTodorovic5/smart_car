#ifndef _JSON_MESSAGES_H_
#define _JSON_MESSAGES_H_

#include "messages.h"
#include "Arduino.h"

extern uint8_t type;

extern voltage_engine_message voltage_message;
extern direction_change_message direction_message;
extern led_change_message led_message;

extern String password_bt;
extern String ssid_bt;

uint8_t new_message(char* payload);

void set_voltage();
void set_direction();
void set_lights();

uint8_t bt_message(char* payload);

#endif