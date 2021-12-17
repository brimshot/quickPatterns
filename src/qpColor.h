#ifndef QP_COLOR_H
#define QP_COLOR_H

#include <FastLED.h>
#include "qpEnums.h"

class qpColor {

  private:

    // Color values

    CRGB currentColor;

    CRGBPalette16 colorPalette;
    byte paletteStep = 3; // amount to jump when using palette sequentially
    byte paletteIndex = 0;

    CRGB *colorSet;
    byte numColorsInSet = 0;
    byte colorSetIndex = 0;

    // Change timing

    //TODO: convert all counters to unsigned
    
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

    qpColor *singleColor(CRGB color);

    qpColor *usePalette(CRGBPalette16 palette);
    qpColor *chooseColorFromPalette(CRGBPalette16 palette, QP_COLOR_MODE mode, byte stepSize = 3);
    qpColor *setPaletteStep(byte stepSize);
    
    qpColor *useColorSet(CRGB *colorSet, byte numElements);
    qpColor *chooseColorFromSet(CRGB *colorSet, byte numElements, QP_COLOR_MODE mode);

    qpColor *withChanceToChangeColor(byte percentage);

    // Timing

    // TODO: do it this way instead, for consistency
    //void qpColor::bindColorDurationTimer(unsigned long *periodCounter, int minPeriods, int maxPeriods);

    void setPeriodCounter(unsigned long *periodCounter);
    void setColorDuration(int minPeriods, int maxPeriods);


};

#endif
