#ifndef QP_COLOR_H
#define QP_COLOR_H

class qpColor {

  private:
    _currentColor;

    int *colorPeriodsCounter = NULL;
    unsigned int periodCountAtLastColorChange = 0;
    unsigned int minColorDuration = 1;
    unsigned int maxColorDuration = 0;
    unsigned int currentColorDuration = 0;
    byte chanceToChangeColor = 0;

    void setColorDurationRange(unsigned int minPeriods, unsigned int maxPeriods);

    void loadNextPaletteColorRandomly();
    void loadNextPaletteColorSequentially();
    void loadNextColorFromSetRandomly();
    void loadNextColorFromSetSequentially();
    void updateColorPeriodically();

    void (qpPattern::*updateColorFunction)(); // periodic or constant
    void (qpPattern::*loadNextColorFunction)(); // random or sequential

  public:

    CRGB getColor();
}

#endif
