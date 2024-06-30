class TwinkleSubPattern : public SubPattern {
 private:
  Twinkle _twinkle;
  uint8_t _percentBrightness = 0;  // percent brightness of the whole pattern

 public:
  virtual uint8_t getPercentBrightness() { return _percentBrightness; }

  virtual void setPercentBrightness(uint8_t percentBrightness) {
    _percentBrightness = percentBrightness;
    _twinkle.setPercentBrightness(percentBrightness);
  }

  virtual void show() { _twinkle.show(); }
};
