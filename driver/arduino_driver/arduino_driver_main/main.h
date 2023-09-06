#ifndef _MAIN_HEADER_
#define _MAIN_HEADER_

#include "pin_config.h"
#include "messages.h"

volatile uint8_t right_direction_lights = 0;
volatile uint8_t left_direction_lights = 0;
uint8_t auto_lights = 0;
uint8_t lights = 0;
uint8_t engine_direction = FORWARD_DIRECTION;

int scalePercentToDutyCycle(int intensity);
void change_speed(int intensity, int direction);
void change_direction(int intensity, int direction);
uint8_t light_sensor_check();
float front_distance_check();

#endif