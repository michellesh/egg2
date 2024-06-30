#define NUM_BLOBS 15

struct Blob {
  float size;
  float angle;
  float height;
  float speed;

  void reset() {
    size = random(25, 40);
    angle = random(0, 360);
    speed = _getRandomSpeed();
    height = speed > 0 ? -size : MAX_RING_HEIGHT + size;
  }

  float _getRandomSpeed() { return float(random(5, 10)) / 10; }

  void update() {
    height += speed;
    if (height > size + MAX_RING_HEIGHT || height < -size) {
      reset();
    }
  }
};

Blob blobs[NUM_BLOBS];

void setupBlobs() {
  for (int k = 0; k < NUM_BLOBS; k++) {
    Blob b;
    b.reset();
    b.height = random(0, MAX_RING_HEIGHT);
    blobs[k] = b;
  }
}

void lavalamp() {
  for (int r = 0; r < NUM_RINGS; r++) {
    for (int i = 0; i < rings[r].numLEDs; i++) {

      int sum = 0;
      float dist;
      for (int k = 0; k < NUM_BLOBS; k++) {
        dist = distance(rings[r].angle[i], rings[r].height, blobs[k].angle,
                        blobs[k].height);
        if (dist < blobs[k].size) {
          sum += map(dist, 0, blobs[k].size, 255, 0);
        }
      }

      sum = constrain(sum, 0, 255);
      CRGB color = palette.mapToColor(1, 0, 1);
      rings[r].leds[i] = color.nscale8(sum);
    }
  }

  for (int k = 0; k < NUM_BLOBS; k++) {
    blobs[k].update();
  }

  EVERY_N_MILLISECONDS(500) {
    Serial.print(blobs[0].angle);
    Serial.print(" ");
    Serial.print(blobs[0].height);
    Serial.println();
  }
}
