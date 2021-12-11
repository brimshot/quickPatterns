#ifndef QP_PATTERN_H
#define QP_PATTERN_H

#include <FastLED.h>
#include "qpLinkedList.h"
#include "qpEnums.h"
#include "qpColor.h"

class qpPattern {

  private:

    // ~ Color

    qpColor *_color;

    // ~ Animation speed

    int ticksBetweenFrames = 1;
    int nextRenderTick = 0;

    // ~ Periodic activation

    bool isActive = true;
    bool patternShouldActivatePeriodically = false;

    bool patternShouldActivate();
    bool patternShouldDeactivate();
    bool removeOnDeactivation = false;

    unsigned int minTicksBetweenActivations = 0;
    unsigned int maxTicksBetweenActivations = 0;
    unsigned int ticksUntilActive = 0;

    unsigned int *activePeriodsCounter = NULL;
    unsigned int periodCountAtLastActivation = 0;
    unsigned int minPeriodsToStayActive = 1;
    unsigned int maxPeriodsToStayActive = 0;
    unsigned int currentPeriodsToStayActive = 0;
    byte chanceToActivatePattern = 0;

    void setActivePeriod(int minPeriods, int maxPeriods);
    void resetActivationTimer();

  protected:

    // ~ LEDs
    CRGB *_targetLeds;
    int _numLeds = 0;

    // ~ Color
    CRGB _getColor(byte index = 0);
    CRGBPalette16 _getPalette(byte index = 0);

    //  ~ Animation util
    inline bool _inBounds(int pos) { return ((pos >= 0) && (pos < _numLeds)); }
    inline void _countCycle() { this->cycles++; frames = 0; }
    inline void _clearLeds() { fill_solid(_targetLeds, _numLeds, CRGB::Black); }

    virtual void draw() = 0; //called at each update interval, must be implemented by child classes

  public:

    qpPattern();

    // ~ Counters
    unsigned int ticks = 0;
    unsigned int frames = 0;
    unsigned int cycles = 0;
    unsigned int activations = 0;

    // ~ Setup
    void assignTargetLeds(CRGB *leds, int numLeds); // Called by layer when pattern is added

    /*-->!! Note:
    * _numLeds and _targetLeds are undefined (empty pointers) when the pattern constructors are called.
    * Any pre-rendering calculations that require the number of LEDs to be known should be put in the initialize() function
    */
    virtual void initialize() {} // called once when pattern is created, after LEDs are assigned

    // ~ Render

    // Public render hook
    bool render();

    // ~ Fluent config

    qpPattern &drawEveryNTicks(int ticks);


    // Scheduling
    qpPattern &removeWhenDeactivated(bool value);

    qpPattern &activatePeriodicallyEveryNTicks(int minTicks, int maxTicks = 0);

    qpPattern &stayActiveForNTicks(int minTicks, int maxTicks = 0);

    qpPattern &stayActiveForNFrames(int minUpdates, int maxUpdates = 0);

    qpPattern &stayActiveForNCycles(int minCycles, int maxCycles = 0);

    qpPattern &withChanceOfActivation(byte percentage);


    // Color values
    qpPattern &singleColor(CRGB color);

    qpPattern &usePalette(CRGBPalette16 colorPalette);

    qpPattern &chooseColorFromPalette(CRGBPalette16 colorPalette, QP_COLOR_MODE mode);

    qpPattern &useColorSet(CRGB *colorSet, byte numColorsInSet);

    qpPattern &chooseColorFromSet(CRGB *colorSet, byte numElements, QP_COLOR_MODE mode);

    // Color timing
    qpPattern &changeColorEveryNTicks(int minTicks, int maxTicks = 0);

    qpPattern &changeColorEveryNCycles(int minCycles, int maxCycles = 0);

    qpPattern &changeColorEveryNFrames(int minFrames, int maxFrames = 0);

    qpPattern &changeColorEveryNActivations(int minActivations, int maxActivations = 0);

    qpPattern &withChanceToChangeColor(byte percentage);


    // ~ Direct control

    bool activate();
    void deactivate();
    bool shouldRemoveWhenDecativated();

};

#endif