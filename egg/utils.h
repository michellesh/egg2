float mapf(float value, float inMin, float inMax, float outMin, float outMax) {
  float percentage = (value - inMin) / (inMax - inMin);
  return outMin + (outMax - outMin) * percentage;
}

float distance(float x1, float y1, float x2, float y2) {
  return (float)sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}
