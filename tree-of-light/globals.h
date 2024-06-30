#define NUM_RINGS 13

#define LED_PIN_1 13
#define LED_PIN_2 14
#define NUM_LEDS_PIN_1 124
#define NUM_LEDS_PIN_2 128
#define NUM_LEDS 252

// Incremented once per iteration of the loop
unsigned long ticks = 0;

// number of LEDs in each ring
// int NUM_LEDS_RING[] = {15, 19, 23, 25, 25, 25, 25, 23, 21, 19, 15, 11, 7};
int NUM_LEDS_RING[] = {7, 11, 15, 19, 21, 23, 25, 25, 25, 25, 23, 19, 15};

// height (z index) of each ring
// float RING_HEIGHT[] = {0,     11.8,  26.1,  42.1,  58.7,  75.7, 92.7,
//                       109.3, 125.3, 140.6, 154.8, 167.3, 176.8};
float RING_HEIGHT[] = {100.0, 94.63, 87.56, 79.52, 70.87, 61.82, 52.43,
                       42.82, 33.2,  23.81, 14.76, 6.67,  0.0};
// float RING_HEIGHT[] = {0.0,   6.67,  14.76, 23.81, 33.2,  42.82, 52.43,
//                        61.82, 70.87, 79.52, 87.56, 94.63, 100.0};

// how many LEDs off is the beginning of the ring from where the LED strip
// starts on that ring
// float RING_OFFSET[] = {-0.5, -1.0, -1.5, -2.0, -2.5, -3.0, -3.5, -3.0, -2.5,
// -2.0, -1.5, -1.0, -0.5};
float RING_OFFSET[] = {-0.5, -1.0, -1.5, -2.0, -2.5, -3.0, -3.5,
                       -3.0, -2.5, -2.0, -1.5, -1.0, -0.5};

// radius of each ring
// float RING_RADIUS[] =
// {38.89,  51.15, 60.32,  66.295, 69.335, 69.815, 68.105, 64.455, 58.89, 51.495,
// 42.18,  30.645, 16.52};
float RING_RADIUS[] = {16.52,  30.645, 42.18,  51.495, 58.89, 64.455, 68.105,
                       69.815, 69.335, 66.295, 60.32,  51.15, 38.89};
