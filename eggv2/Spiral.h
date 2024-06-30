#define MAX_SPIRALS 3

class Spiral : public Pattern {
 private:
  uint8_t _id = 0;
  int16_t _angle = 0;  // the current angle
  int16_t _width = WIDTH.DFLT;
  int16_t _speed = SPEED.DFLT;
  int16_t _ringOffset[NUM_RINGS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t _heightBrightness[NUM_RINGS] = {255, 255, 255, 255, 255,
                                          255, 255, 255, 255, 255, 255, 255, 255};

  // Gets the brightness of the LED if the LED's angle is close to the current
  // angle
  uint8_t _getBrightness(uint8_t i, uint8_t j) {
    //if (!isBetween(rings[i].radius(j), _minRadius[i], _maxRadius[i])) {
    //  return 0;
    //}

    int16_t targetAngle = (_angle + _ringOffset[i] + 360) % 360;
    int16_t pixelAngle = rings[i].angle[j];

    // Calculate distance from the current angle. If angle is near end
    // (360-width < a < 360), also check LEDs near beginning. If angle is near
    // beginning (0 < a < width), also check LEDs near end
    int16_t dist = isBetween(targetAngle - pixelAngle, 0, _width)
                       ? targetAngle - pixelAngle
                   : targetAngle > 360 - _width &&
                           isBetween(targetAngle - 360 - pixelAngle, 0, _width)
                       ? targetAngle - 360 - pixelAngle
                   : targetAngle < _width &&
                           isBetween(targetAngle + 360 - pixelAngle, 0, _width)
                       ? targetAngle + 360 - pixelAngle
                       : 0;

    uint8_t brightness = addFadeShape(map(dist, 0, _width, 0, 255));
    return min(brightness, _heightBrightness[i]);
    //int b = map(dist, 0, _width, 0, 255);
    //return b < 127 ? b * 2 : 255 - (2 * (b - 128));
  };

 public:
  Spiral(uint8_t id = 0) { _id = id; }

  static constexpr Range WIDTH = {
      10, 90, 90};  // How many degrees along the circumference at
                    // the current angle to light up
  static constexpr Range SPEED = {1, 10, 1};  // How many degrees to add to the
                                              // current angle each time
  static constexpr Range RING_OFFSET = {
      20, 90, 30};  // How many degrees to increase angle per
                    // ring higher = tighter spiral

  void setWidth(int16_t width) { _width = abs(width); }

  void setSpeed(int16_t speed) { _speed = speed; }

  void setAngle(int16_t angle) { _angle = angle; }

  void setRingOffset(uint8_t i, int16_t angle) { _ringOffset[i] = angle; }

  int16_t getRingOffset(uint8_t i) { return _ringOffset[i]; }

  void reverse() { _speed = _speed * -1; }

  void setHeightRangePercent(uint8_t min, uint8_t max) {
    uint8_t fade = 50;  // how much percent of full height to add fade
    for (uint8_t i = 0; i < NUM_RINGS; i++) {
      if (min == 0 && max == 100) {
        _heightBrightness[i] = 255;
      }

      float height = PERCENT_HEIGHT_RING[i];
      _heightBrightness[i] =
          isBetweenI(height, min, min + fade)
              ? map(height, min, min + fade, 0, 255)
          : isBetweenI(height, max - fade, max)
              ? map(height, max - fade, max, 255, 0)
          : min < max ? (isBetweenI(height, min + fade, max - fade) ? 255 : 0)
                      : (isBetweenI(height, max, min) ? 0 : 255);
    }
  }


  void show() {
    for (uint8_t i = 0; i < NUM_RINGS; i++) {
      for (uint8_t j = 0; j < rings[i].numLEDs; j++) {
        uint8_t brightness = _getBrightness(i, j);
        if (brightness > 0) {
          //CRGB color = palette.getColor(_id * 2).nscale8(
          //    brightness * getPercentBrightness() / 100);
          CRGB color = palette.mapToColor(_id, 0, MAX_SPIRALS);
          rings[i].leds[j] = color.nscale8(brightness);
          //color = brightness > 0 ? spiralColor : color;
          //rings[i].setBlend(j, color, brightness);
        }
      }
    }

    // Increment the angle. After 360 degrees, start over at 0 degrees
    _angle = (_angle + _speed + 360) % 360;
  }
};
