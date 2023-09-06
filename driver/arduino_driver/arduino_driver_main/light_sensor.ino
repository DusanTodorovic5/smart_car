uint8_t light_sensor_check()
{
    uint32_t val = analogRead(LIGHT_SENSOR);

    return val < LIGHT_SENSOR_TRESHOLD;
}