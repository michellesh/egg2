class SolidSubPattern : public SubPattern {
private:
  uint8_t _percentBrightness = 0; // percent brightness of the whole pattern
  uint8_t _paletteIndexRing[NUM_RINGS];
  bool _isIndexIncreasing[NUM_RINGS];
  uint8_t _maxPaletteIndex =
      240; // 0-255 doesnt form a perfect gradient for some reason so this clips
           // the end of the gradient

  void _update() {
    for (uint8_t i = 0; i < NUM_RINGS; i++) {
      if (_isIndexIncreasing[i]) {
        _paletteIndexRing[i]++;
        if (_paletteIndexRing[i] >= _maxPaletteIndex) {
          _isIndexIncreasing[i] = false;
        }
      } else {
        _paletteIndexRing[i]--;
        if (_paletteIndexRing[i] <= 0) {
          _isIndexIncreasing[i] = true;
        }
      }
    }
  }

public:
  SolidSubPattern() {
    for (uint8_t i = 0; i < NUM_RINGS; i++) {
      _paletteIndexRing[i] = map(i, 0, NUM_RINGS, 0, _maxPaletteIndex);
      _isIndexIncreasing[i] = 1;
    }
    _isIndexIncreasing[NUM_RINGS - 1] = 0;
  }

  virtual uint8_t getPercentBrightness() { return _percentBrightness; }

  virtual void setPercentBrightness(uint8_t percentBrightness) {
    _percentBrightness = percentBrightness;
  }

  virtual void show() {
    static int colorOffset;
    for (uint8_t i = 0; i < NUM_RINGS; i++) {
      for (uint8_t j = 0; j < rings[i].numLEDs; j++) {
        CRGB color = palette.getColorAtPaletteIndex(_paletteIndexRing[i]);
        rings[i].leds[j] = color;
      }
    }
    _update();
  }
};
