struct Ring {
  uint8_t ringIndex;
  uint8_t numLEDs; // number of LEDs in this ring
  float radius; // the radius of this ring
  float height; // the height (z index) of this ring

  CRGB *leds;          // the LEDs for this ring
  uint8_t *brightness; // the brightness of the LEDs for this ring

  float *angle;        // the angle of each LED in this ring

  void setBrighter(uint8_t pixelIndex, CRGB color, uint8_t colorBrightness) {
    if (colorBrightness > brightness[pixelIndex]) {
      leds[pixelIndex] = color;
      brightness[pixelIndex] = colorBrightness;
    }
  }

  void setBlend(uint8_t pixelIndex, CRGB color, uint8_t colorBrightness) {
    if (brightness[pixelIndex] == 0) {
      leds[pixelIndex] = color;
      brightness[pixelIndex] = colorBrightness;
    } else {
      leds[pixelIndex] = getColorBetween(color, leds[pixelIndex]);
      brightness[pixelIndex] = (brightness[pixelIndex] + colorBrightness) / 2;
    }
  }
};
