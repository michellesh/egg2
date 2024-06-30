CRGB campfire[] = {
    CRGB::ForestGreen, CRGB::Lime,      CRGB::Gold,
    CRGB::DarkOrange,  CRGB::OrangeRed, CRGB::Maroon,
    CRGB::Chocolate,   CRGB::Coral,     CRGB::Lavender,
};

DEFINE_GRADIENT_PALETTE(_firePalette){0,   0,   0,   0,   // black
                                      128, 255, 0,   0,   // red
                                      200, 255, 140, 0,   // CRGB::Orange
                                      255, 255, 215, 0};  // CRGB::Gold
CRGBPalette16 firePalette = _firePalette;

DEFINE_GRADIENT_PALETTE(_tealGreenGold){0,   34,  139, 34,  // CRGB::ForestGreen
                                        85,  0,   255, 0,   // CRGB::Lime
                                        170, 255, 215, 0,   // CRGB::Gold
                                        255, 255, 140, 0};  // CRGB::DarkOrange
CRGBPalette16 tealGreenGold = _tealGreenGold;

DEFINE_GRADIENT_PALETTE(_redRoseLavendar){
    0,   128, 0,   0,     // CRGB::Maroon
    85,  210, 105, 30,    // CRGB::Chocolate
    170, 255, 127, 80,    // CRGB::Coral
    255, 230, 230, 250};  // CRGB::Lavender
CRGBPalette16 redRoseLavendar = _redRoseLavendar;

DEFINE_GRADIENT_PALETTE(_icePalette){0,   224, 240, 255,  // light blue
                                     127, 31,  147, 255,  // medium blue
                                     255, 48,  64,  72};  // dark blue
CRGBPalette16 icePalette = _icePalette;

DEFINE_GRADIENT_PALETTE(_fairyPalette){0,   63,  57,  11,    // "QuarterFairy"
                                       127, 127, 114, 22,    // "HalfFairy"
                                       224, 255, 227, 45,    // CRGB::FairyLight
                                       255, 255, 255, 255};  // full white
CRGBPalette16 fairyPalette = _fairyPalette;

// clang-format off
CRGBPalette16* activePalettes[] = {
  &redRoseLavendar,
  &tealGreenGold,
  //&firePalette,
  &icePalette,
  &fairyPalette
};
// clang-format on
