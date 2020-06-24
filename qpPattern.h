#ifndef QP_PATTERN_H
#define QP_PATTERN_H

#include <FastLED.h>

class qpPattern {

  private:

    bool currentlyActive = true;

    // Period counters

    int ticks = 0;
    int updates = 0;
    int cycles = 0;
    int activations = 0;

    // Color

    CRGB _currentColor;

    // Periodic color changes

    int *colorPeriodsCounter = NULL;
    unsigned int periodCountAtLastColorChange = 0;
    unsigned int minColorDuration = 1;
    unsigned int maxColorDuration = 0;
    unsigned int currentColorDuration = 0;
    byte chanceToChangeColor = 0;

    void setColorDurationRange(unsigned int minPeriods, unsigned int maxPeriods);

    void (qpPattern::*updateColorFunction)(); // periodic or constant
    void (qpPattern::*loadNextColorFunction)(); // random or sequential

    void updateColorPeriodically();

    void loadNextPaletteColorRandomly();
    void loadNextPaletteColorSequentially();
    void loadNextColorFromSetRandomly();
    void loadNextColorFromSetSequentially();


    // Animation

    int ticksBetweenFrames = 1;

    // Periodic activation

    unsigned int minTicksBetweenActivations = 0;
    unsigned int maxTicksBetweenActivations = 0;
    unsigned int ticksUntilActive = 0;

    int *activePeriodsCounter = NULL;
    unsigned int periodCountAtLastActivation = 0;
    unsigned int minPeriodsToStayActive = 1;
    unsigned int maxPeriodsToStayActive = 0;
    unsigned int currentPeriodsToStayActive = 0;
    byte chanceToActivatePattern = 0;

    void setActivePeriodRange(int minPeriods, int maxPeriods);
    void activatePeriodically();
    void resetActivationTimer();
    void (qpPattern::*deactiveWhenAppropriate)();

    void deactivatePeriodically();

    // Interface for layer

    void assignTargetLeds(CRGB *leds, int numLeds); // Called when pattern is added to layer
    void (qpPattern::*updateActiveStatus)();
    bool isActive();
    void render();


    void doNothing() { /* empty function for pointers to pattern update steps that do nothing as per pattern config */ }

    friend class qpLayer;

  protected:

    // Members and methods to be used in pattern animation code

    /*-->!! Note:
    * _numLeds and _targetLeds are undefined (empty pointers) when the pattern constructors are called.
    * Any pre-rendering calculations that require the number of LEDs to be known should be put in the initialize() function
    */

    CRGB *_targetLeds;
    int _numLeds = 0;

    // Color values

    CRGBPalette16 _colorPalette;
    byte _paletteIndex = 0;
    byte _paletteStep = 0;

    CRGB *_colorSet;
    byte numColorsInSet = 0;
    byte colorSetIndex = 0;

    inline CRGB _getColor(int index = 0) {
      return this->_currentColor;
    }

    // These are the core animation functions to be implemented by the sub-classes

    virtual void initialize() { /* called once when pattern is created, after LEDs are assigned */ }
    virtual void draw() { /* called at each update interval */ }


    inline bool _inBounds(int pos) {
      return ((pos >= 0) && (pos < _numLeds));
    }

    inline void _countCycle() {
      this->cycles++;
    }

    inline void _clearLeds() {
      fill_solid(_targetLeds, _numLeds, CRGB::Black);
    }

    CRGB _loadNextColor();

  public:

    qpPattern();

    // Pattern speed

    qpPattern &drawEveryNTicks(int ticks);

    // Periodic activation

    qpPattern &activatePeriodicallyEveryNTicks(int minTicks, int maxTicks = 0);

    qpPattern &stayActiveForNTicks(int minTicks, int maxTicks = 0);
    qpPattern &stayActiveForNFrames(int minUpdates, int maxUpdates = 0);
    qpPattern &stayActiveForNCycles(int minCycles, int maxCycles = 0);

    qpPattern &withChanceOfActivation(int percentage);

    // Colors

    qpPattern &singleColor(CRGB color);
    qpPattern &chooseColorSequentiallyFromPalette(CRGBPalette16 colorPalette, byte stepSize = 3);
    qpPattern &chooseColorRandomlyFromPalette(CRGBPalette16 colorPalette);

    qpPattern &chooseColorSequentiallyFromSet(CRGB *colorSet, byte numColorsInSet);
    qpPattern &chooseColorRandomlyFromSet(CRGB *colorSet, byte numColorsInSet);

    qpPattern &changeColorEveryNTicks(int minTicks, int maxTicks = 0);
    qpPattern &changeColorEveryNCycles(int minCycles, int maxCycles = 0);
    qpPattern &changeColorEveryNFrames(int minFrames, int maxFrames = 0);
    qpPattern &changeColorEveryNActivations(int minActivations, int maxActivations = 0);

    qpPattern &withChanceToChangeColor(byte percentage);

    // Real time changes

    bool activate();
    void deactivate();

    qpPattern &setPalette(CRGBPalette16 colorPalette, byte stepSize = 3);
    qpPattern &setColorSet(CRGB *colorSet, int numElements);

};

#endif
