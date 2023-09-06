#ifndef _MAIN_HEADER_
#define _MAIN_HEADER_

#include "messages.h"


#define FORWARD_DIRECTION 0
#define BACKWARDS_DIRECTION 1
#define LEFT_DIRECTION 0
#define RIGHT_DIRECTION 1

#define SPEED_OF_SOUND 0.04802

#define COUNTER_PERIOD 255

#define LIGHT_SENSOR_TRESHOLD 400
#define LIGHT_SENSOR A0

#define input1_GPIO 13
#define input2_GPIO 12
#define engineIntensity 11

#define input3_GPIO 8
#define input4_GPIO 9
#define directionIntensity 10

#define STOP_LIGHTS_GPIO 2
#define LIGHTS_GPIO 3
#define LEFT_LIGHT_GPIO 4
#define RIGHT_LIGHT_GPIO 5

#define ECHO_PIN 7
#define TRIGGER_PIN 6

#define TX_GPIO 1
#define RX_GPIO 0

void change_speed(int intensity, int direction);
void change_direction(int intensity, int direction);

#endif