#ifndef QP_COLOR_H
#define QP_COLOR_H

#include <qpPattern.h>


class qpPattern;

class qpColor {

  private:

    qpPattern *parent;

    CRGB _currentColor;

    // Color values

    CRGBPalette16 _colorPalette;
    byte _paletteIndex = 0;
    byte _paletteStep = 0;

    CRGB *_colorSet;
    byte numColorsInSet = 0;
    byte colorSetIndex = 0;

    // Periodic change

    int *colorPeriodsCounter = nullptr;
    unsigned int periodCountAtLastColorChange = 0;
    int minColorDuration = 1;
    int maxColorDuration = 0;
    unsigned int currentColorDuration = 0;
    int chanceToChangeColor = 0;

    void setColorDuration(int minPeriods, int maxPeriods);
    void updateColorPeriodically();

    // Choosing

    void loadNextPaletteColorRandomly();
    void loadNextPaletteColorSequentially();
    void loadNextColorFromSetRandomly();
    void loadNextColorFromSetSequentially();

    void (qpColor::*loadNextColorFunction)(); // random or sequential

    void doNothing() { /* empty function for pointers to pattern update steps that do nothing as per config */ }

  public:

    /*
    qpColor &color(int index) {
      return this->parent->color(index);
    }
    */

    CRGB getColor() {

      return this->_currentColor;
    }

    void (qpColor::*updateColorFunction)(); // periodic or constant

    // Color choosing

    qpColor &singleColor(CRGB color);
    qpColor &chooseColorSequentiallyFromPalette(CRGBPalette16 colorPalette, byte stepSize);
    qpColor &chooseColorRandomlyFromPalette(CRGBPalette16 colorPalette);
    qpColor &chooseColorSequentiallyFromSet(CRGB *colorSet, byte numColorsInSet);
    qpColor &chooseColorRandomlyFromSet(CRGB *colorSet, byte numColorsInSet);
    qpColor &setColorSet(CRGB *colorSet, int numElements);
    qpColor &setPalette(CRGBPalette16 colorPalette, byte stepSize);


    // Periodic changes

    qpColor &changeColorEveryNTicks(int minActivations, int maxActivations);
    qpColor &changeColorEveryNCycles(int minCycles, int maxCycles);
    qpColor &changeColorEveryNFrames(int minFrames, int maxFrames);
    qpColor &changeColorEveryNActivations(int minActivations, int maxActivations);
    qpColor &withChanceToChangeColor(int percentage);

    void _loadNextColor();


};

#endif
