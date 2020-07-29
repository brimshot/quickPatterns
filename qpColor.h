#ifndef QP_COLOR_H
#define QP_COLOR_H

enum QP_COLOR_MODE {SEQUENTIAL, RANDOM};

#include <qpPattern.h>

class qpPattern;

class qpColor {

  private:

    qpPattern *parentPattern;

    CRGB currentColor;

    // Color values

    CRGBPalette16 colorPalette;
    byte paletteStep = 3; // amount to jump when using palette sequentially
    byte paletteIndex = 0;

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
    CRGBPalette16 getPalette() { return this->colorPalette; }


    // ~ Config

    // Color sequence

    qpColor &singleColor(CRGB color);

    qpColor &usePalette(CRGBPalette16 palette);
    qpColor &chooseColorFromPalette(CRGBPalette16 palette, QP_COLOR_MODE mode);
    qpColor &setPaletteStep(byte stepSize);
    
    qpColor &useColorSet(CRGB *colorSet, byte numElements);
    qpColor &chooseColorFromSet(CRGB *colorSet, byte numElements, QP_COLOR_MODE mode);

    // Timing

    qpColor &changeColorEveryNTicks(int minTicks, int maxTicks = 0);
    qpColor &changeColorEveryNCycles(int minCycles, int maxCycles = 0);
    qpColor &changeColorEveryNFrames(int minFrames, int maxFrames = 0);
    qpColor &changeColorEveryNActivations(int minActivations, int maxActivations = 0);
    qpColor &withChanceToChangeColor(byte percentage);

};

#endif
