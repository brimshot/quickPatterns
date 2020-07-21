#ifndef QP_COLOR_H
#define QP_COLOR_H

#include <qpPattern.h>

class qpPattern;

class qpColor {

  private:

    qpPattern *parentPattern;

    CRGB currentColor;

    // Color values

    CRGBPalette16 colorPalette;
    byte paletteIndex = 0;
    byte paletteStep = 0;

    CRGB *colorSet;
    byte numColorsInSet = 0;
    byte colorSetIndex = 0;

    // Change timing

    int *colorPeriodsCounter = nullptr;
    unsigned int periodCountAtLastColorChange = 0;
    int minColorDuration = 1;
    int maxColorDuration = 0;
    unsigned int currentColorDuration = 0;
    int chanceToChangeColor = 0;

    void setColorDuration(int minPeriods, int maxPeriods);
    void updateColorPeriodically();

    // Load routines

    void loadNextPaletteColorRandomly();
    void loadNextPaletteColorSequentially();
    void loadNextColorFromSetRandomly();
    void loadNextColorFromSetSequentially();

    void (qpColor::*loadNextColorFunction)(); // random or sequential

    void doNothing() { /* empty function for pointers to pattern update steps that do nothing as per config */ }

  public:

    qpColor(qpPattern *parentPattern);


    // ~ Rendering

    void (qpColor::*updateColorFunction)(); // periodic or nothing

    void _loadNextColor(); //calls appropriate load routine

    CRGB getColor() { return this->currentColor; }


    // ~ Config

    // Direct config
    
    qpColor &setColorSet(CRGB *colorSet, byte numElements);
    qpColor &setPalette(CRGBPalette16 colorPalette, byte stepSize);

    // Color sequence

    qpColor &singleColor(CRGB color);
    qpColor &chooseColorSequentiallyFromPalette(CRGBPalette16 colorPalette, byte stepSize);
    qpColor &chooseColorRandomlyFromPalette(CRGBPalette16 colorPalette);
    qpColor &chooseColorSequentiallyFromSet(CRGB *colorSet, byte numColorsInSet);
    qpColor &chooseColorRandomlyFromSet(CRGB *colorSet, byte numColorsInSet);

    // Timing

    qpColor &changeColorEveryNTicks(int minTicks, int maxTicks = 0);
    qpColor &changeColorEveryNCycles(int minCycles, int maxCycles = 0);
    qpColor &changeColorEveryNFrames(int minFrames, int maxFrames = 0);
    qpColor &changeColorEveryNActivations(int minActivations, int maxActivations = 0);
    qpColor &withChanceToChangeColor(byte percentage);

};

#endif
