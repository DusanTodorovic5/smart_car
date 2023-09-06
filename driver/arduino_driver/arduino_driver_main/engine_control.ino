int scalePercentToDutyCycle(int intensity)
{
    return (intensity / 100.0) * COUNTER_PERIOD;
}

void change_speed(int intensity, int direction)
{
    intensity = scalePercentToDutyCycle(intensity);

    analogWrite(engineIntensity, intensity);
    switch (direction)
    {
    case FORWARD_DIRECTION:
        digitalWrite(input1_GPIO, LOW);
        digitalWrite(input2_GPIO, HIGH);
        engine_direction = FORWARD_DIRECTION;
        break;
    case BACKWARDS_DIRECTION:
        digitalWrite(input2_GPIO, LOW);
        digitalWrite(input1_GPIO, HIGH);
        engine_direction = BACKWARDS_DIRECTION;
        break;
    default:
        analogWrite(engineIntensity, 0);
        digitalWrite(input2_GPIO, LOW);
        digitalWrite(input1_GPIO, LOW);
        break;
    }
}

void change_direction(int intensity, int direction)
{
    intensity = scalePercentToDutyCycle(intensity);

    analogWrite(directionIntensity, intensity);
    switch (direction)
    {
    case LEFT_DIRECTION:
        digitalWrite(input3_GPIO, LOW);
        digitalWrite(input4_GPIO, HIGH);
        break;
    case RIGHT_DIRECTION:
        digitalWrite(input4_GPIO, LOW);
        digitalWrite(input3_GPIO, HIGH);
        break;
    default:
        analogWrite(directionIntensity, 0);
        digitalWrite(input3_GPIO, LOW);
        digitalWrite(input4_GPIO, LOW);
        break;
    }
}