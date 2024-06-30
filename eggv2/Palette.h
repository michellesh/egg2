#define MAX_PALETTE_INDEX 245

class Palette {
private:
  CRGBPalette16 _currentPalette = *(activePalettes[0]);
  CRGBPalette16 _targetPalette = *(activePalettes[0]);
  uint8_t _activeColorMode = VERTICAL_GRADIENT;

  void _setNextColorPalette() {
    const uint8_t numberOfPalettes =
        sizeof(activePalettes) / sizeof(activePalettes[0]);
    static uint8_t whichPalette = -1;
    whichPalette = addmod8(whichPalette, 1, numberOfPalettes);

    _targetPalette = *(activePalettes[whichPalette]);
  }

public:
  static const uint8_t SOLID = 0;
  static const uint8_t VERTICAL_GRADIENT = 1;
  static const uint8_t ANGLE_GRADIENT = 3;

  uint8_t getActiveColorMode() { return _activeColorMode; }

  void setColorMode(uint8_t colorMode) { _activeColorMode = colorMode; }

  void cycle() {
    EVERY_N_SECONDS(SECONDS_PER_PALETTE) { _setNextColorPalette(); }

    EVERY_N_MILLISECONDS(10) {
      nblendPaletteTowardPalette(_currentPalette, _targetPalette, 12);
    }
  }

  CRGB getColorAtPaletteIndex(uint8_t paletteIndex) {
    return ColorFromPalette(_currentPalette, paletteIndex);
  }

  CRGB mapToColor(int value, int fromLow, int fromHigh) {
    uint8_t paletteIndex = map(value, fromLow, fromHigh, 0, MAX_PALETTE_INDEX);
    return ColorFromPalette(_currentPalette, paletteIndex);
  }

  CRGB getColor(uint8_t i) {
    uint8_t paletteIndex = map(i, 0, NUM_RINGS - 1, 0, MAX_PALETTE_INDEX);
    return ColorFromPalette(_currentPalette, paletteIndex);
  }

  CRGB getColor(uint8_t i, uint8_t j) {
    uint8_t paletteIndex = 0;
    switch (_activeColorMode) {
    case VERTICAL_GRADIENT: {
      paletteIndex = map(i, 0, NUM_RINGS - 1, 0, MAX_PALETTE_INDEX);
      break;
    }
    case ANGLE_GRADIENT: {
      paletteIndex = map(rings[i].angle[j], 0, 360, 0, MAX_PALETTE_INDEX);
      break;
    }
    default:
      break;
    }
    return ColorFromPalette(_currentPalette, paletteIndex);
  }
};
