#include <Arduino.h>
#include <FastLED.h>

#include "globals.h"
#include "colors.h"
#include "utils.h"
#include "fade.h"
#include "Ring.h"

Ring rings[NUM_RINGS];
CRGB leds[NUM_LEDS];
uint8_t ledBrightness[NUM_LEDS];

#include "Palette.h"
Palette palette;

#include "Range.h"
#include "Pattern.h"
//#include "Bloom.h"
#include "Spiral.h"
#include "Twinkle.h"

#include "SubPattern.h"
//#include "BloomSubPattern.h"
#include "SpiralSubPattern.h"
#include "TwinkleSubPattern.h"

TwinkleSubPattern twinkle;

//BloomSubPattern bloomContinuous(BloomSubPattern::CONTINUOUS);
//BloomSubPattern bloomStartSame(BloomSubPattern::START_SAME);
//BloomSubPattern bloomEndSame(BloomSubPattern::END_SAME);
//BloomSubPattern bloomUpward(BloomSubPattern::UPWARD);
//BloomSubPattern bloomDownward(BloomSubPattern::DOWNWARD);

SpiralSubPattern rubberBandWorm(SpiralSubPattern::RUBBER_BAND_WORM);
SpiralSubPattern rubberBandNoAnchor(SpiralSubPattern::RUBBER_BAND_NO_ANCHOR);
SpiralSubPattern rubberBandAnchored(SpiralSubPattern::RUBBER_BAND_ANCHORED);
SpiralSubPattern growingSpirals(SpiralSubPattern::GROWING_SPIRALS);
SpiralSubPattern basicSpiralRotation(SpiralSubPattern::BASIC_SPIRAL_ROTATION);
//SpiralSubPattern continuousSpiral(SpiralSubPattern::CONTINUOUS_SPIRAL);

// clang-format off
SubPattern *activePatterns[] = {
  //&twinkle,
  //&bloomContinuous,
  //&bloomStartSame,
  //&bloomEndSame,
  //&bloomUpward,
  //&bloomDownward,
  //&rubberBandWorm,
  //&rubberBandNoAnchor,
  &rubberBandAnchored,
  //&growingSpirals,
  //&basicSpiralRotation,
  //&continuousSpiral
};
// clang-format on

SubPattern *sourcePattern;
SubPattern *targetPattern;
uint8_t numPatterns = sizeof(activePatterns) / sizeof(activePatterns[0]);

void setup() {
  Serial.begin(115200);
  delay(500);

  FastLED.addLeds<WS2812B, LED_PIN_1, GRB>(leds, NUM_LEDS_PIN_1);
  FastLED.addLeds<WS2812B, LED_PIN_2, GRB>(leds, NUM_LEDS_PIN_1,
                                           NUM_LEDS_PIN_2);

  // Setup ring data
  int offset = 0;
  for (int i = 0; i < NUM_RINGS; i++) {
    Ring ring = {i, NUM_LEDS_RING[i], RING_RADIUS[i], RING_HEIGHT[i],
                 &leds[offset], &ledBrightness[offset]};

    // Set the degree position 0-360 for each LED in this ring
    ring.angle = new float[NUM_LEDS_RING[i]];
    float degreeStep = 360 / (float)ring.numLEDs;
    float degreeOffset = RING_OFFSET[i] * degreeStep;
    for (int j = 0; j < ring.numLEDs; j++) {
      ring.angle[j] = 360 - (j * degreeStep + degreeOffset);
    }

    rings[i] = ring;
    offset += ring.numLEDs;
  }

  // Initialize all LEDs to black
  for (int16_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }

  FastLED.setBrightness(100);

  // Set first pattern in activePatterns to be played first
  sourcePattern = (SubPattern *)activePatterns[numPatterns - 1];
  sourcePattern->setPercentBrightness(0);
  targetPattern = (SubPattern *)activePatterns[0];
  targetPattern->setPercentBrightness(100);
}

void loop() {
  clearLEDs();
  palette.cycle();

  EVERY_N_SECONDS(15) {
    // Increment active pattern
    sourcePattern = (SubPattern *)targetPattern;
    static uint8_t nextPattern = 0;
    nextPattern = (nextPattern + 1) % numPatterns;
    targetPattern = (SubPattern *)(activePatterns[nextPattern]);
  }

  EVERY_N_MILLISECONDS(100) {
    shiftPercentBrightnessTo(targetPattern, 100);
    shiftPercentBrightnessTo(sourcePattern, 0);
  }

  for (uint8_t i = 0; i < numPatterns; i++) {
    if (activePatterns[i]->getPercentBrightness() > 0) {
      activePatterns[i]->show();
    }
  }

  FastLED.show();
  ticks++;
}

void shiftPercentBrightnessTo(SubPattern *pattern,
                              uint8_t toPercentBrightness) {
  if (pattern->getPercentBrightness() == toPercentBrightness) {
    return;
  } else if (pattern->getPercentBrightness() < toPercentBrightness) {
    pattern->setPercentBrightness(
        min(pattern->getPercentBrightness() + 3, 100));
  } else if (pattern->getPercentBrightness() > toPercentBrightness) {
    pattern->setPercentBrightness(max(pattern->getPercentBrightness() - 3, 0));
  }
}

void clearLEDs() {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    ledBrightness[i] = 0;
    leds[i].nscale8(0);
  }
}
