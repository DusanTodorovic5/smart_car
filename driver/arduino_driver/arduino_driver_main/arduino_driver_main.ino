#include "main.h"

char rcvBuf[100];

int scalePercentToDutyCycle(int intensity)
{
    return (intensity / 100.0) * COUNTER_PERIOD;
}

float front_distance_check() {
  static int numTicks;
  static float distance;
  static long duration;

  numTicks = 0;
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIGGER_PIN, HIGH);

  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);

  distance = duration / 29 / 2;

  return distance;
}

ISR(TIMER1_COMPA_vect) {
    digitalWrite(LEFT_LIGHT_GPIO, left_direction_lights ?  !digitalRead(LEFT_LIGHT_GPIO) : LOW);
    digitalWrite(RIGHT_LIGHT_GPIO, right_direction_lights ? !digitalRead(RIGHT_LIGHT_GPIO) : LOW);
}

void setup()
{
    noInterrupts();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = 9375; // (16,000,000 / (1024 * 0.6)) - 1
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);
    TIMSK1 |= (1 << OCIE1A);
    interrupts();

    pinMode(engineIntensity, OUTPUT);
    pinMode(directionIntensity, OUTPUT);
    pinMode(input1_GPIO, OUTPUT);
    pinMode(input2_GPIO, OUTPUT);
    pinMode(input3_GPIO, OUTPUT);
    pinMode(input4_GPIO, OUTPUT);
    pinMode(STOP_LIGHTS_GPIO, OUTPUT);
    pinMode(LIGHTS_GPIO, OUTPUT);
    pinMode(LEFT_LIGHT_GPIO, OUTPUT);
    pinMode(RIGHT_LIGHT_GPIO, OUTPUT);
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    Serial.begin(115200);

    Serial.setTimeout(100);
    digitalWrite(STOP_LIGHTS_GPIO, HIGH);
    
}

void test_seq() {
    digitalWrite(STOP_LIGHTS_GPIO, HIGH);
    left_direction_lights = 1;
    change_direction(100, RIGHT_DIRECTION);
    change_speed(100, BACKWARDS_DIRECTION);
    // change_direction(100, LEFT_DIRECTION);
    delay(2000);
    change_speed(100, FORWARD_DIRECTION);
    // change_direction(65, LEFT_DIRECTION);
    delay(2000);

    change_speed(65, BACKWARDS_DIRECTION);
    change_direction(100, RIGHT_DIRECTION);
    delay(2000);
    digitalWrite(STOP_LIGHTS_GPIO, LOW);
    change_speed(65, FORWARD_DIRECTION);
    change_direction(100, RIGHT_DIRECTION);
    delay(2000);

    left_direction_lights = 0;

    change_speed(85, BACKWARDS_DIRECTION);
    // change_direction(100, LEFT_DIRECTION);
    delay(2000);

    right_direction_lights = 1;
    change_speed(85, FORWARD_DIRECTION);
    change_direction(100, RIGHT_DIRECTION);
    delay(2000);

    digitalWrite(LIGHTS_GPIO, HIGH);

    change_speed(55, BACKWARDS_DIRECTION);
    // change_direction(100, LEFT_DIRECTION);
    delay(2000);
    change_speed(55, FORWARD_DIRECTION);
    change_direction(100, RIGHT_DIRECTION);
    delay(2000);

    digitalWrite(LIGHTS_GPIO, LOW);
    right_direction_lights = 0;
}

void loop()
{
  memset(rcvBuf, 0, 100 * sizeof(char));

  if (Serial.readBytes(rcvBuf, sizeof(uint8_t))) {
    switch (rcvBuf[0]) {
      case 1:
      {
        Serial.readBytes(&(rcvBuf[1]), sizeof(voltage_engine_message) - 1);

        voltage_engine_message* msg = (voltage_engine_message*) rcvBuf;

        if (msg->voltage > 100) {
          if (front_distance_check() > 5) {
            change_speed(255 - msg->voltage, FORWARD_DIRECTION);
          } else {
            change_speed(0, FORWARD_DIRECTION);
          }
        } else {
          change_speed(msg->voltage, BACKWARDS_DIRECTION);
        }

        digitalWrite(STOP_LIGHTS_GPIO, msg->voltage == 0 ? HIGH : LOW);
      }
      break;
      case 2:
      {
        Serial.readBytes(&(rcvBuf[1]), sizeof(direction_change_message) - 1);

        direction_change_message* msg = (direction_change_message*) rcvBuf;

        if (msg->direction > 100) {
          change_direction(255 - msg->direction, RIGHT_DIRECTION);
        } else {
          change_direction(msg->direction, LEFT_DIRECTION);
        }
      }
      break;
      case 3:
      {
        Serial.readBytes(&(rcvBuf[1]), sizeof(led_change_message) - 1);

        led_change_message* msg = (led_change_message *) rcvBuf;

        lights = msg->front_light ? 1 : 0;
        right_direction_lights = msg->right_dir_light ? 1 : 0;
        left_direction_lights = msg->left_dir_light ? 1 : 0;
        auto_lights = msg->auto_lights ? 1 : 0;
      }
      break;
    }
  }

  digitalWrite(LIGHTS_GPIO, lights || (light_sensor_check() && auto_lights) ? HIGH : LOW);

  if (front_distance_check() < 5 && engine_direction == FORWARD_DIRECTION) {
    change_speed(0, FORWARD_DIRECTION);
  }
}