#ifndef _MAIN_HEADER_
#define _MAIN_HEADER_

#include "pin_config.h"
#include "messages.h"

volatile uint8_t right_direction_lights = 0;
volatile uint8_t left_direction_lights = 0;
uint8_t auto_lights = 0;
uint8_t lights = 0;
uint8_t engine_direction = FORWARD_DIRECTION;

void change_speed(int intensity, int direction);
void change_direction(int intensity, int direction);

#endif