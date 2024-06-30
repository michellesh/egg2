#define NUM_STARS_PER_RING 2
#define NUM_STARS NUM_STARS_PER_RING *NUM_RINGS
#define SPEED -0.5

struct Star {
  int ringIndex;
  float angle;
  float speed;

  void update() {
    angle += speed;
    if (angle < 0) {
      angle += 360;
    }
  }
};

Star stars[NUM_STARS];

void setupStarfield() {
  for (int i = 0; i < NUM_STARS; i++) {
    int ringIndex = i % NUM_RINGS;
    float angle = random(0, 360);
    float speed = float(random(-3, -6)) / 10;
    stars[i] = {ringIndex, angle, speed};
  }
}

void starfield() {
  for (int i = 0; i < NUM_STARS; i++) {
    int index =
        map(stars[i].angle, 0, 360, 0, rings[stars[i].ringIndex].numLEDs - 1);
    rings[stars[i].ringIndex].leds[index] = CHSV(50, 255, 255);
    stars[i].update();
  }
}
