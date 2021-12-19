#ifndef QP_COLOR_H
#define QP_COLOR_H

#include <FastLED.h>
#include "qpEnums.h"

class qpColor {

  private:

    // Color values

    CRGB currentColor;

    CRGBPalette16 colorPalette;
    uint8_t paletteStep = 3; // amount to jump when using palette sequentially
    uint8_t paletteIndex = 0;

    CRGB *colorSet;
    uint8_t numColorsInSet = 0;
    uint8_t colorSetIndex = 0;

    // Change timing

    bool colorShouldChangePeriodically = false;
    bool nextColorShouldLoad();

    unsigned long *colorPeriodsCounter = nullptr;
    unsigned int periodCountAtLastColorChange = 0;
    unsigned int currentColorDuration = 0;
    unsigned int minColorDuration = 1;
    unsigned int maxColorDuration = 0;
    unsigned int chanceToChangeColor = 0;

    // Load routines
    void loadNextPaletteColorRandomly();
    void loadNextPaletteColorSequentially();
    void loadNextColorFromSetRandomly();
    void loadNextColorFromSetSequentially();

    void (qpColor::*loadNextColorFunction)() = nullptr; // random or sequential

  public:

    qpColor();

    // ~ Rendering

    void update();
    void loadNextColor(); //calls appropriate load routine

    CRGB getColor() { return this->currentColor; }
    CRGBPalette16 getPalette() { return this->colorPalette; }

    // ~ Config

    // Color sequence

    void singleColor(CRGB color);

    void usePalette(CRGBPalette16 palette);
    void chooseColorFromPalette(CRGBPalette16 palette, QP_COLOR_MODE mode, uint8_t stepSize = 3);
    void setPaletteStep(uint8_t stepSize);
    
    void useColorSet(CRGB *colorSet, uint8_t numElements);
    void chooseColorFromSet(CRGB *colorSet, uint8_t numElements, QP_COLOR_MODE mode);

    void withChanceToChangeColor(uint8_t percentage);

    // Timing

    void bindColorDurationTimer(unsigned long *periodCounter, int minPeriods, int maxPeriods);
};

#endif
