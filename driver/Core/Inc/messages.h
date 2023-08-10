#ifndef _MESSAGES_HEADER_H_
#define _MESSAGES_HEADER_h_

#include <stdio.h>
#include <stdint.h>

/*
    0 - state update
    1 - engine_voltage
    2 - direction
    3 - lights
*/

/*
 * 
 * Structure containing information about single sensor
 * 
*/
typedef struct sensor_state_s {
    uint8_t sensor_id;
    uint16_t distance;
} sensor_state;


/* 
 * 
 * Message Type 0
 * 
 * Structure representing current driver's state
 * This message is sent by the driver to comm_layer
 * 
*/
typedef struct state_update_message_s {
    uint8_t message_type;
    sensor_state sensors[2];
    uint16_t velocity;
} state_update_message;


/* 
 *
 * Message type 1
 * 
 * Structure representing new engine voltage value
 * values can be from 0 to 100 or from 155 to 255.
 * 0-100 represent positive voltage while other negative
 * 
*/
typedef struct voltage_engine_message_s {
    uint8_t message_type;
    uint8_t voltage;
} voltage_engine_message;

/*
 * 
 * Message type 2
 * 
 * Structure representing new direction of front wheels
 * direction is value from 0-100 or 155 to 255. 155 to 255 present - voltage
 * meaning, the car will turn left
 * intensity is angle of the front wheels, if 0, the car will go forward
 * 
*/
typedef struct direction_change_message_s {
    uint8_t message_type;
    uint8_t direction;
} direction_change_message;

/*
 *
 * Message type 3
 * 
 * Structure representing new led lights states
 * This structure containins boolean values where 1 is turned on
 * while 0 is turned off
 * 
*/
typedef struct led_change_message_s {
    uint8_t message_type;
    uint8_t left_dir_light;
    uint8_t right_dir_light;
    uint8_t front_light;
    uint8_t auto_lights;
} led_change_message;

#endif