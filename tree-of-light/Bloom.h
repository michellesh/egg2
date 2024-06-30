class Ripple {
 private:
  uint8_t _colorIndex;
  uint8_t _discIndex;
  int16_t _radius;
  bool _continuous;

  int16_t _maxRadius() {
    return MAX_RADIUS_DISC[_continuous ? _discIndex : 0] + 5;
  }

  void _updateDiscIndex(int8_t amount) {
    if (_continuous) {
      _discIndex = (_discIndex + amount + NUM_DISCS) % NUM_DISCS;
    }
  }

  void _updateColorIndex(uint8_t colorInc) {
    _colorIndex = (_colorIndex + NUM_DISCS + (colorInc * 2)) % NUM_DISCS;
  }

  uint8_t _getBrightness(uint8_t p, int16_t width) {
    int16_t dist = _radius - discs[_discIndex].radius(p);
    return isBetween(dist, 0, width) ? addFadeShape(map(dist, 0, width, 0, 255))
                                     : 0;
  }

 public:
  Ripple(uint8_t colorIndex = 0, uint8_t discIndex = 0, int16_t radius = 0,
         bool continuous = true) {
    _colorIndex = colorIndex;
    _discIndex = discIndex;
    _radius = radius;
    _continuous = continuous;
  }

  void updateRadius(int16_t speed, int16_t width, uint8_t colorInc) {
    int16_t newRadius = _radius + speed;
    _radius = (newRadius + _maxRadius() + width) % (_maxRadius() + width);
    if (speed < 0 && newRadius < 0) {
      _updateDiscIndex(-1);
      _updateColorIndex(colorInc);
    } else if (speed > 0 && newRadius >= _maxRadius() + width) {
      _updateDiscIndex(1);
      _updateColorIndex(colorInc);
    }
  }

  void show(int16_t width, uint8_t percentBrightness) {
    for (uint8_t p = 0; p < discs[_discIndex].numLEDs; p++) {
      uint8_t brightness = _getBrightness(p, width);
      if (brightness > 0) {
        CRGB color = palette.getColor(_colorIndex)
                         .nscale8(brightness * percentBrightness / 100);
        discs[_discIndex].setBrighter(p, color, brightness);
      }
    }
  }
};

class Bloom : public Pattern {
 private:
  Ripple _ripples[NUM_DISCS];
  uint8_t _group = 0;
  int16_t _width = WIDTH.DFLT;
  int16_t _speed = SPEED.DFLT;
  int16_t _offset = OFFSET.DFLT;
  uint8_t _colorInc = 2;  // How many colors to increment each new ripple by

 public:
  static constexpr Range WIDTH = {20, 255, 100};
  static constexpr Range SPEED = {1, 8, 2};
  static constexpr Range OFFSET = {0, 255, 0};

  Bloom(uint8_t group = 0, int16_t offset = OFFSET.DFLT) {
    _group = group;
    _offset = offset;
  }

  void setRipples(int16_t discOffset = 0, bool continuous = true) {
    for (uint8_t d = 0; d < NUM_DISCS; d++) {
      uint8_t colorIndex = ((d * _colorInc) + (_group * _colorInc)) % NUM_DISCS;
      int16_t radius = d * discOffset + _offset;
      _ripples[d] = Ripple(colorIndex, d, radius, continuous);
    }
  }

  void setGroup(uint8_t group) { _group = group; }

  void setOffset(int16_t offset) { _offset = offset; }

  void setWidth(int16_t width) { _width = width; }

  void setSpeed(int16_t speed) { _speed = speed; }

  void reverse() { _speed = _speed * -1; }

  void show() {
    for (uint8_t d = 0; d < NUM_DISCS; d++) {
      _ripples[d].show(_width, getPercentBrightness());
      _ripples[d].updateRadius(_speed, _width, _colorInc);
    }
  }
};
