struct TransitionValue {
  float current;
  float target;
  float min;
  float max;
  float increment;
  Timer timer = {10000};

  void init(float _current, float _target, float _min, float _max,
            float _increment) {
    current = _current;
    target = _target;
    min = _min;
    max = _max;
    increment = _increment;
  }
  void update() {
    if (roundPrecision(current, 2) == roundPrecision(target, 2)) {
      reset();
    } else if (current < target) {
      current += increment;
    } else {
      current -= increment;
    }
  }
  void timedUpdate() {
    if (timer.complete()) {
      reset();
      timer.reset();
    } else if (current < target) {
      current += increment;
    } else if (current > target) {
      current -= increment;
    }
  }
  void reset() { target = float(random(min, max)); }
};

class SpiralSubPattern : public SubPattern {
private:
  Spiral _spirals[MAX_SPIRALS];
  uint8_t _numSpirals = MAX_SPIRALS;
  uint8_t _activeSubPattern = 0;
  TransitionValue _width;
  TransitionValue _widthOffset;
  TransitionValue _heightOffsetIncrement;
  float _heightOffset = 0;

  void _showSeaweed() {
    EVERY_N_MILLISECONDS(50) {
      _width.update();
      _widthOffset.update();
      _heightOffsetIncrement.timedUpdate();
      _heightOffset -= (_heightOffsetIncrement.current / 10);
      if (_heightOffset < 0) {
        _heightOffset = NUM_RINGS - 0.5;
      }
      //Serial.println(_heightOffset);
    }
    for (uint8_t i = 0; i < _numSpirals; i++) {
      for (uint8_t j = 0; j < NUM_RINGS; j++) {
        _spirals[i].setRingOffset(j, mapf(sin(j + _heightOffset), -1, 1,
                                          -_widthOffset.current,
                                          _widthOffset.current));
      }
      _spirals[i].setWidth(_width.current);
      _spirals[i].show();
    }
  }

  void _showRandomOrganic() {
    unsigned long w = 10000; // waveLength
    int offset = sawtooth(0, 360, w);
    EVERY_N_MILLISECONDS(50) { _width.update(); }
    _spirals[0].setWidth(_width.current);
    for (uint8_t i = 0; i < NUM_RINGS; i++) {
      _spirals[0].setRingOffset(i, i * offset);
    }
    _spirals[0].show();
  }

  void _showRubberBandWorm() {
    int16_t offset = sinwave(-90, 90, 500);
    int16_t width = sinwave(-180, 180, 500);
    int16_t speed = sinwave(1, 4, 400);

    for (uint8_t i = 0; i < NUM_RINGS; i++) {
      _spirals[0].setSpeed(speed);
      _spirals[0].setRingOffset(i, i * offset);
      _spirals[0].setWidth(abs(width));
    }
    _spirals[0].show();
  }

  void _showRubberBandNoAnchor() {
    int16_t angle = sinwave(0, 200, 400);
    int16_t offset = sinwave(-30, 30, 400);
    int16_t width = sinwave(-180, 180, 400);

    for (uint8_t i = 0; i < NUM_RINGS; i++) {
      _spirals[0].setAngle(angle);
      _spirals[0].setRingOffset(i, i * offset);
      _spirals[0].setWidth(abs(width));
    }
    _spirals[0].show();
  }

  void _showRubberBandAnchored() {
    int toMin = square(8, 0, 300);
    int toMax = square(0, 8, 300);

    // int16_t offset = sinwave(-360, 360, 100);
    // int16_t offset = sawtooth(-360, 360, 100); // swapping effect
    int16_t offset = cosSawtooth(-360, 360, 300);

    for (uint8_t i = 0; i < NUM_RINGS; i++) {
      int16_t ringOffset = mapf(i, toMin, toMax, 0, offset);
      _spirals[0].setRingOffset(i, ringOffset);
    }
    _spirals[0].show();
  }

  void _showGrowingSpirals() {
    unsigned long w = 300;  // waveLength
    unsigned long wo = 100; // waveLengthOffset
    _spirals[0].setHeightRangePercent(sawtooth(-50, 150, w, wo),
                                      sawtooth(-50, 150, w));
    _spirals[0].show();
    _spirals[1].setHeightRangePercent(sawtooth(-50, 150, w, wo * 2),
                                      sawtooth(-50, 150, w, wo));
    _spirals[1].show();
    _spirals[2].setHeightRangePercent(sawtooth(-50, 150, w, w),
                                      sawtooth(-50, 150, w, wo * 2));
    _spirals[2].show();
  }

  void _showBasicSpiralRotation() {
    _spirals[0].show();
    _spirals[1].show();
  }

  void _showContinuousSpiral() {
    unsigned long w = 10000; // waveLength
    // unsigned long wo = w * 3 / 4;  // waveLengthOffset
    int offset = sawtooth(0, 360, w);
    int16_t width = sinwave(30, 180, 5000); //, w / 2, wo);
    // int16_t speed = sinwave(0, 5, w / 2, wo);
    _spirals[0].setWidth(width);
    for (uint8_t i = 0; i < NUM_RINGS; i++) {
      _spirals[0].setRingOffset(i, i * offset);
      //_spirals[0].setSpeed(speed);
    }
    _spirals[0].show();
  }

public:
  static const uint8_t RUBBER_BAND_WORM = 0;
  static const uint8_t RUBBER_BAND_NO_ANCHOR = 1;
  static const uint8_t RUBBER_BAND_ANCHORED = 2;
  static const uint8_t GROWING_SPIRALS = 3;
  static const uint8_t BASIC_SPIRAL_ROTATION = 4;
  static const uint8_t CONTINUOUS_SPIRAL = 5;
  static const uint8_t RANDOM_ORGANIC = 6;
  static const uint8_t SEAWEED = 7;

  SpiralSubPattern(uint8_t activeSubPattern = 0) {
    _activeSubPattern = activeSubPattern;
    switch (_activeSubPattern) {
    case RUBBER_BAND_WORM:
      _numSpirals = 1;
      _spirals[0] = Spiral();
      break;
    case RUBBER_BAND_NO_ANCHOR:
      _numSpirals = 1;
      _spirals[0] = Spiral();
      _spirals[0].setSpeed(0);
      break;
    case RUBBER_BAND_ANCHORED:
      _numSpirals = 1;
      _spirals[0] = Spiral();
      _spirals[0].setAngle(0);
      _spirals[0].setSpeed(0);
      break;
    case GROWING_SPIRALS:
      _numSpirals = 3;
      for (int i = 0; i < _numSpirals; i++) {
        _spirals[i] = Spiral(i + 1);
        _spirals[i].setSpeed(0);
        _spirals[i].setAngle(i * 120);
        for (uint8_t j = 0; j < NUM_RINGS; j++) {
          _spirals[i].setRingOffset(j, j * 30);
        }
      }
      break;
    case BASIC_SPIRAL_ROTATION:
      _numSpirals = 2;
      _spirals[0] = Spiral(1);
      _spirals[1] = Spiral(2);
      _spirals[0].setSpeed(2);
      _spirals[1].setSpeed(-2);
      for (uint8_t i = 0; i < NUM_RINGS; i++) {
        _spirals[0].setRingOffset(i, (NUM_RINGS - 1 - i) * 30);
        _spirals[1].setRingOffset(i, i * 30);
      }
      break;
    case CONTINUOUS_SPIRAL:
      _numSpirals = 1;
      _spirals[0] = Spiral();
      _spirals[0].setSpeed(1);
      _spirals[0].setWidth(120);
      break;
    case RANDOM_ORGANIC:
      _numSpirals = 1;
      _spirals[0] = Spiral();
      _width.init(120, 120, 30, 180, 0.5);
      break;
    case SEAWEED:
      _numSpirals = 3;
      for (uint8_t i = 0; i < _numSpirals; i++) {
        _spirals[i] = Spiral(i);
        _spirals[i].setAngle(i * 120);
        _spirals[i].setWidth(30);
        _spirals[i].setSpeed(0);
      }
      _width.init(45, 45, 30, 120, 0.5);
      _widthOffset.init(10, 10, 5, 30, 0.5);
      _heightOffsetIncrement.init(1, 1, 1, 5, 0.01);
      _heightOffset = 0;
      break;
    default:
      break;
    }
  }

  virtual void show() {
    switch (_activeSubPattern) {
    case RUBBER_BAND_WORM:
      _showRubberBandWorm();
      break;
    case RUBBER_BAND_NO_ANCHOR:
      _showRubberBandNoAnchor();
      break;
    case RUBBER_BAND_ANCHORED:
      _showRubberBandAnchored();
      break;
    case GROWING_SPIRALS:
      _showGrowingSpirals();
      break;
    case BASIC_SPIRAL_ROTATION:
      _showBasicSpiralRotation();
      break;
    case CONTINUOUS_SPIRAL:
      _showContinuousSpiral();
      break;
    case RANDOM_ORGANIC:
      _showRandomOrganic();
      break;
    case SEAWEED:
      _showSeaweed();
      break;
    default:
      break;
    }
  }
};
