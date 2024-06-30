// number of LEDs in each ring
int ringNumLEDs[] = {15, 19, 23, 25, 25, 25, 25, 23, 21, 19, 15, 11, 7};

// height (z index) of each ring
float ringHeight[] = {0,     11.8,  26.1,  42.1,  58.7,  75.7, 92.7,
                      109.3, 125.3, 140.6, 154.8, 167.3, 176.8};
float maxRingHeight = 75.7; // TODO will be 176.8

// how many LEDs off is the beginning of the ring from where the LED strip
// starts on that ring
float ringOffset[] = {-0.5, -1.0, -1.5, -2.0, -2.5, -3.0, -3.5,
                      -3.0, -2.5, -2.0, -1.5, -1.0, -0.5};

// diameter of each ring
float ringDiameter[] = {77.78,  102.3,  120.64, 132.59, 138.67, 139.63, 136.21,
                        128.91, 117.78, 102.99, 84.36,  61.29,  33.04};

struct Ring {
  int ringIndex;
  int numLEDs;
  float height;
  float diameter;

  CRGB *leds; // array of LEDs in this ring
  CHSV *canvas; // array of LEDs in this ring
  float *deg; // degree position of each LED in this ring
};
