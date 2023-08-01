#ifndef MESSAGES_HEADER
#define MESSAGES_HEADER

#include <stdio.h>
#include <stdint.h>

/*
    0 - state update
    1 - engine_voltage
    2 - direction_change
    3 - direction_engine
    4 - leds
*/

/*
 * 
 * Message interface used to fetch id of the message
 * 
*/
typedef struct message_interface_s {
    uint8_t message_type;
} message_interface;

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
 * 
*/
typedef struct velocity_engine_message_s {
    uint8_t message_type;
    uint8_t engine_voltage;
} velocity_engine_message;

/*
 * 
 * Message type 2
 * 
 * Structure representing new direction of front wheels
 * direction can be either 0 for left or 1 for right
 * intensity is angle of the front wheels, if 0, the car will go forward
 * 
*/
typedef struct direction_change_message_s {
    uint8_t message_type;
    uint8_t direction;
    uint8_t intensity;
} direction_change_message;

/*
 *
 * Message type 3
 * 
 * Structure representing new direction of rear wheels
 * direction 0 for forward and 1 for backwards
 * 
*/
typedef struct direction_rear_engine_message_s {
    uint8_t message_type;
    uint8_t direction;
} direction_rear_engine_message;

/*
 *
 * Message type 4
 * 
 * Structure representing new led lights states
 * This structure containins boolean values where 1 is turned on
 * while 0 is turned off
 * 
*/
typedef struct led_change_message_s {
    uint8_t message_type;
    uint8_t stop_lights;
    uint8_t left_dir_light;
    uint8_t right_dir_light;
    uint8_t front_light;
    uint8_t auto_lights;
} led_change_message;

#endif