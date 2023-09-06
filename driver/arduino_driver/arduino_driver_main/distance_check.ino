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