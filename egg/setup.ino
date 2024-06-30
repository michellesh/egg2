void setupLEDs() {
  FastLED.addLeds<WS2812B, LED_PIN_1, GRB>(leds, NUM_LEDS_PIN_1);
  FastLED.addLeds<WS2812B, LED_PIN_2, GRB>(leds, NUM_LEDS_PIN_1,
                                           NUM_LEDS_PIN_2);
  FastLED.setBrightness(MAX_BRIGHTNESS);
}

void setupRings() {
  int offset = 0;
  for (int i = 0; i < NUM_RINGS; i++) {
    Ring ring = {i, ringNumLEDs[i], ringHeight[i], ringDiameter[i],
                 &leds[offset]};

    // Set the degree position 0-360 for each LED in this ring
    ring.deg = new float[ringNumLEDs[i]];
    ring.canvas = new CHSV[ringNumLEDs[i]];
    float degreeStep = 360 / (float)ring.numLEDs;
    float degreeOffset = ringOffset[i] * degreeStep;
    for (int j = 0; j < ring.numLEDs; j++) {
      ring.deg[j] = 360 - (j * degreeStep + degreeOffset);
      ring.canvas[j] = CHSV(0, 0, 0);
    }

    rings[i] = ring;
    offset += ring.numLEDs;
  }
}
