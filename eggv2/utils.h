float mapf(float value, float inMin, float inMax, float outMin, float outMax) {
  float percentage = (value - inMin) / (inMax - inMin);
  return outMin + (outMax - outMin) * percentage;
}

float roundPrecision(float value, int precision) {
  int multiplier = pow(10, precision); // 10, 100, 1000, etc
  return round(value * multiplier) / multiplier;
}

float distance(float x1, float y1, float x2, float y2) {
  return (float)sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

bool isBetween(int16_t value, int16_t min, int16_t max,
               bool inclusive = false) {
  return inclusive ? value >= min && value <= max : value > min && value < max;
}

float sinwave(float minValue, float maxValue, unsigned long waveLength = 50,
              unsigned long waveLengthOffset = 0) {
  return mapf(sin((float)(ticks + waveLengthOffset) * PI / waveLength), -1, 1,
              minValue, maxValue);
}

float cosSawtooth(float minValue, float maxValue,
                  unsigned long waveLength = 50) {
  int8_t s = ticks % (waveLength * 2) < waveLength ? -1 : 1;
  return mapf(s * (cos((float)ticks * PI / waveLength)), -1, 1, minValue,
              maxValue);
}

float sawtooth(float minValue, float maxValue, unsigned long waveLength = 50,
               unsigned long waveLengthOffset = 0) {
  float t = (float)(ticks + waveLengthOffset) / waveLength;
  return mapf(t - floor(t), 0, 1, minValue, maxValue);
}

float square(float minValue, float maxValue, unsigned long waveLength = 50) {
  return ticks % (waveLength * 2) < waveLength ? minValue : maxValue;
}

uint8_t attackDecayWave8(uint8_t i) {
  if (i < 86) {
    return i * 3;
  } else {
    i -= 86;
    return 255 - (i + (i / 2));
  }
}

#define FADE_LINEAR 0
#define FADE_LINEAR_SYM 1
#define FADE_LINEAR_REVERSE 2
#define FADE_SAWTOOTH 3  // with 1/3 up 2/3 down
#define FADE_EXP2_SYM 4
#define FADE_EXP3_SYM 5
#define FADE_EXP4_SYM 6
#define FADE_CUSTOM 7

#define MIN_FADE 10

struct XY {
  uint8_t x;
  uint8_t y;
};

// creates a "fade shape" based on 2 or 3 pivot points.
// assumes p1.x < p2.x < p3.x
uint8_t fadeShape(uint8_t x, XY p1, XY p2, XY p3) {
  // clang-format off
  return x < p1.x   ? map(x, 0, p1.x, 0, p1.y)
         : x < p2.x ? map(x, p1.x, p2.x, p1.y, p2.y)
         : x < p3.x ? map(x, p2.x, p3.x, p2.y, p3.y)
                    : map(x, p3.x, 255, p3.y, 0);
  // clang-format on
}

uint8_t fadeShape(uint8_t x, XY p1, XY p2 = {127, 255}) {
  return fadeShape(x, p1, p2, {255 - p1.x, p1.y});
}

uint8_t exp(uint8_t x, uint8_t p, int div) {
  return x < 127 ? pow(x, p) / div : pow(255 - x, p) / div;
}

uint8_t max(uint8_t val1, uint8_t val2) { return val1 < val2 ? val2 : val1; }

bool isBetweenI(int16_t value, int16_t min, int16_t max) {
  return isBetween(value, min, max, true);
}

uint8_t addFadeShape(uint8_t x, uint8_t mode = FADE_LINEAR_SYM) {
  switch (mode) {
    case FADE_CUSTOM:
      return fadeShape(x, {60, 20});
    case FADE_EXP4_SYM:
      return max(MIN_FADE, exp(x, 4, 1050000));
    case FADE_EXP3_SYM:
      return max(MIN_FADE, exp(x, 3, 8200));
    case FADE_EXP2_SYM:
      return max(MIN_FADE, exp(x, 2, 65));
    case FADE_SAWTOOTH:
      return attackDecayWave8(x);
    case FADE_LINEAR_SYM:
      return x < 127 ? x * 2 : 255 - (2 * (x - 128));
    case FADE_LINEAR_REVERSE:
      return 255 - x;
    case FADE_LINEAR:
      return x;
    default:  // no fade, square shape
      return 255;
  }
}
