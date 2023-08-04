#ifndef _JSON_MESSAGES_H_
#define _JSON_MESSAGES_H_

#include "messages.h"

extern uint8_t type;
extern voltage_engine_message voltage_message;
extern direction_change_message direction_message;
extern led_change_message led_message;

uint8_t new_message(char* payload);

void set_voltage();
void set_direction();
void set_lights();



#endif