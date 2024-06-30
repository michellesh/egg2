#define ACTION_CHANGE_COLOR 0
#define ACTION_MOVE_HORIZONTAL 1
#define ACTION_MOVE_VERTICAL 2
#define ACTION_TOGGLE_CURSOR 3
#define ACTION_CLEAR_CANVAS 4

#define MAX_DEGREES 360
#define MAX_HEIGHT 177
#define MAX_HUE 255

#define INIT_DEGREES 279
#define INIT_HEIGHT 100
#define INIT_HUE HUE_BLUE
#define INIT_CURSOR_ON 0

uint8_t broadcastAddressEgg[] = {0xCC, 0xDB, 0xA7, 0x02, 0x08, 0x5C};
uint8_t broadcastAddressBase[] = {0xC8, 0xF0, 0x9E, 0x9F, 0xAC, 0x18};

char macAddressEgg[] = "CC:DB:A7:02:08:5C";
char macAddressBase[] = "C8:F0:9E:9F:AC:18";

typedef struct msg {
  int action;
  int value;
  int sentValue;
  unsigned long lastSentAt;
} msg;
