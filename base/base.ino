#include <FastLED.h>
#include <WiFi.h>
#include <esp_now.h>

#include "egg_shared.h"

#include "RotaryEncoder.h"

#define DEBUG

// Pins
#define LEFT_KNOB_CLK 25
#define LEFT_KNOB_DT 26
#define LEFT_KNOB_SW 27
#define RIGHT_KNOB_CLK 5
#define RIGHT_KNOB_DT 18
#define RIGHT_KNOB_SW 19
#define SWIVEL_CLK 21
#define SWIVEL_DT 22
#define SWIVEL_SW 23
#define LEFT_LED_PIN 13
#define RIGHT_LED_PIN 14

#define MAX_CLICKS 40
#define SATURATION 100
#define BRIGHTNESS 255
#define MSG_BUFFER 50 // milliseconds to pause between sending messages

bool cursorOn = INIT_CURSOR_ON;
CHSV color = CHSV(INIT_HUE, SATURATION, BRIGHTNESS);

RotaryEncoder leftKnob = {
    LEFT_KNOB_CLK, LEFT_KNOB_DT, LEFT_KNOB_SW, 0, 0, 0, 0};
RotaryEncoder rightKnob = {
    RIGHT_KNOB_CLK, RIGHT_KNOB_DT, RIGHT_KNOB_SW, 0, 0, 0, 0};
RotaryEncoder swivel = {SWIVEL_CLK, SWIVEL_DT, SWIVEL_SW, 0, 0, 0, 0};

CRGB leds[2];

msg changeColor = {ACTION_CHANGE_COLOR, INIT_HUE, INIT_HUE};
msg moveHorizontal = {ACTION_MOVE_HORIZONTAL, INIT_DEGREES, INIT_DEGREES};
msg moveVertical = {ACTION_MOVE_VERTICAL, INIT_HEIGHT, INIT_HEIGHT};
msg toggleCursor = {ACTION_TOGGLE_CURSOR, 1, 1};
msg clearCanvas = {ACTION_CLEAR_CANVAS};

esp_now_peer_info_t peerInfo;

void setup() {
  pinMode(LEFT_KNOB_CLK, INPUT);
  pinMode(LEFT_KNOB_DT, INPUT);
  pinMode(LEFT_KNOB_SW, INPUT);
  pinMode(RIGHT_KNOB_CLK, INPUT);
  pinMode(RIGHT_KNOB_DT, INPUT);
  pinMode(RIGHT_KNOB_SW, INPUT);
  pinMode(SWIVEL_CLK, INPUT);
  pinMode(SWIVEL_DT, INPUT);
  pinMode(SWIVEL_SW, INPUT);

  FastLED.addLeds<WS2812B, LEFT_LED_PIN, GRB>(leds, 1);
  FastLED.addLeds<WS2812B, RIGHT_LED_PIN, GRB>(leds, 1, 1);

  leftKnob.led = &leds[0];
  rightKnob.led = &leds[1];

#ifdef DEBUG
  Serial.begin(115200);
  delay(500);
#endif

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddressEgg, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Send initial state to egg
  send(changeColor);
  send(moveHorizontal);
  send(moveVertical);
  send(toggleCursor);
}

bool send(msg &m) {
  if (millis() > 1000 && millis() - m.lastSentAt > MSG_BUFFER) {
    esp_err_t result =
        esp_now_send(broadcastAddressEgg, (uint8_t *)&m, sizeof(m));
    if (result == ESP_OK) {
      Serial.print("Sent with success at ");
      m.lastSentAt = millis();
      Serial.println(m.lastSentAt);
      return true;
    } else {
      Serial.println("Error sending the data");
    }
  }
  return false;
}

void loop() {
  leftKnob.update();
  rightKnob.update();
  swivel.update();

  if (bothButtonsHeld()) {
    Serial.print("Clear canvas");
    send(clearCanvas);
    delay(MSG_BUFFER);
    cursorOn = false;
    toggleCursor.value = cursorOn;
    // send(toggleCursor); // egg has to set this itself after clear canvas
  }

  if (anyButtonClicked()) {
    Serial.print("Cursor toggled ");
    cursorOn = !cursorOn;
    if (cursorOn) {
      Serial.println("ON");
    } else {
      Serial.println("OFF");
    }
    toggleCursor.value = cursorOn;
    send(toggleCursor);
  }

  if (leftKnob.buttonHeld) {
    int hue = (changeColor.sentValue + clicksToHue(leftKnob.clicks)) % MAX_HUE;
    if (hue != changeColor.sentValue) {
      changeColor.value = hue;
      bool sent = send(changeColor);
      if (sent) {
        leftKnob.resetClicks();
        changeColor.sentValue = changeColor.value;
        color = CHSV(hue, SATURATION, BRIGHTNESS);

        // If the color was changed and the cursor was off, turn it on
        if (!cursorOn) {
          delay(MSG_BUFFER);
          cursorOn = true;
          toggleCursor.value = cursorOn;
          send(toggleCursor);
        }
      }
      Serial.println("Color changed");
    }
  }

  if (rightKnob.buttonHeld) {
    int hue = (changeColor.sentValue + clicksToHue(rightKnob.clicks)) % MAX_HUE;
    if (hue != changeColor.sentValue) {
      changeColor.value = hue;
      bool sent = send(changeColor);
      if (sent) {
        rightKnob.resetClicks();
        changeColor.sentValue = changeColor.value;
        color = CHSV(hue, SATURATION, BRIGHTNESS);

        // If the color was changed and the cursor was off, turn it on
        if (!cursorOn) {
          delay(MSG_BUFFER);
          cursorOn = true;
          toggleCursor.value = cursorOn;
          send(toggleCursor);
        }
      }
      Serial.println("Color changed");
    }
  }

  int leftKnobAngle =
      (moveHorizontal.sentValue + clicksToDegrees(leftKnob.clicks)) %
      MAX_DEGREES;
  if (leftKnobAngle != moveHorizontal.sentValue) {
    moveHorizontal.value = leftKnobAngle;
    bool sent = send(moveHorizontal);
    if (sent) {
      leftKnob.resetClicks();
      moveHorizontal.sentValue = moveHorizontal.value;
    }
    Serial.print("Cursor moved horizontally ");
    Serial.println(moveHorizontal.value);
  }

  int rightKnobAngle =
      (moveVertical.sentValue + clicksToHeight(rightKnob.clicks)) % MAX_HEIGHT;
  if (rightKnobAngle != moveVertical.sentValue) {
    moveVertical.value = rightKnobAngle;
    bool sent = send(moveVertical);
    if (sent) {
      rightKnob.resetClicks();
      moveVertical.sentValue = moveVertical.value;
    }
    Serial.print("Cursor moved vertically ");
    Serial.println(moveVertical.value);
  }

  FastLED.clear();
  if (cursorOn || anyButtonHeld()) {
    leftKnob.setLED(color);
    rightKnob.setLED(color);
  }
  FastLED.show();
}

int clicksToDegrees(int clicks) {
  return map(clicks, 0, MAX_CLICKS, 0, MAX_DEGREES);
}

int clicksToHeight(int clicks) {
  return map(clicks, 0, MAX_CLICKS, 0, MAX_HEIGHT);
}

int clicksToHue(int clicks) { return map(clicks, 0, MAX_CLICKS, 0, MAX_HUE); }

bool anyButtonClicked() {
  return leftKnob.buttonClicked || rightKnob.buttonClicked;
}

bool anyButtonHeld() { return leftKnob.buttonHeld || rightKnob.buttonHeld; }

bool bothButtonsHeld() { return leftKnob.buttonHeld && rightKnob.buttonHeld; }
