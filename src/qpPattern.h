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
    int nextRenderTick = 1;

    // ~ Periodic activation

    bool isActive = true;
    bool patternShouldActivatePeriodically = false;

    bool patternShouldActivate();
    bool patternShouldDeactivate();
    bool removeOnDeactivation = false;

    unsigned long *periodCounterActivationsAreBoundTo = nullptr;
//    unsigned long *boundPeriodCounter = nullptr;
    unsigned long nextPeriodToActivateAt = 0;
    unsigned int minPeriodsBetweenActivations = 0;
    unsigned int maxPeriodsBetweenActivations = 0;

    unsigned int ticksUntilActive = 0;

    unsigned long *activePeriodsCounter = nullptr;
    unsigned int periodCountAtLastActivation = 0;
    unsigned int minPeriodsToStayActive = 1;
    unsigned int maxPeriodsToStayActive = 0;
    unsigned int currentPeriodsToStayActive = 0;
    byte chanceToActivatePattern = 0;

    void setActivePeriod(int minPeriods, int maxPeriods);
    void resetActivationTimer();

    qpPattern &activatePeridically(unsigned int *periodCounter, int periodsBetweenActivations);

  protected:

    // ~ LEDs -- can this be removed altogether.... !?
    CRGB *_targetLeds;
    int _numLeds = 0;

    // ~ Color
    CRGB _getColor(byte index = 0);
    CRGBPalette16 _getPalette(byte index = 0);

    inline void _countCycle() { this->cycles++; frames = 0; }

    virtual void draw() = 0; //called at each update interval, must be implemented by child classes
    virtual void onActivate() {}
    virtual void onDeactivate() {}

  public:

    qpPattern();

    // ~ Counters
    //TODO: only ticks should need to be long. learn about pointer casting.
    unsigned long ticks = 0;
    unsigned long frames = 0;
    unsigned long cycles = 0;
    unsigned long activations = 0;
    unsigned long deactivations = 0;

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

    qpPattern *drawEveryNTicks(int ticks);


    // Scheduling
    qpPattern &removeWhenDeactivated(bool value);

    qpPattern *activatePeriodicallyEveryNTicks(int minTicks, int maxTicks = 0);

    qpPattern &activateAfterPatternPHasRunForNTicks(qpPattern &P, int ticks);

    qpPattern &stayActiveForNTicks(int minTicks, int maxTicks = 0);

    qpPattern &stayActiveForNFrames(int minFrames, int maxFrames = 0);

    qpPattern *stayActiveForNCycles(int minCycles, int maxCycles = 0);

    qpPattern &withChanceOfActivation(byte percentage);


    // Color values
    qpPattern *singleColor(CRGB color);

    qpPattern &usePalette(CRGBPalette16 colorPalette);

    //TODO: can this be a pointer..... ?
    qpPattern *chooseColorFromPalette(CRGBPalette16 colorPalette, QP_COLOR_MODE mode);

    //TODO: gooooo back
    qpPattern *chooseColorFromPaletteSequentially(CRGBPalette16 colorPalette, int paletteStepSize = 3);
//    qpPattern *setPaletteStepSize(int size);

    qpPattern &useColorSet(CRGB *colorSet, byte numColorsInSet);

    qpPattern &chooseColorFromSet(CRGB *colorSet, byte numElements, QP_COLOR_MODE mode);

    // Color timing
    qpPattern &changeColorEveryNTicks(int minTicks, int maxTicks = 0);

    qpPattern &changeColorEveryNCycles(int minCycles, int maxCycles = 0);

    qpPattern &changeColorEveryNFrames(int minFrames, int maxFrames = 0);

    qpPattern &changeColorEveryNActivations(int minActivations, int maxActivations = 0);

    qpPattern &withChanceToChangeColor(byte percentage);

    // ~ State management

    qpPattern *activateWhenPatternPDeactivates(qpPattern *P);

    qpPattern *beginInActiveState();

    bool activate();
    void deactivate();
    bool shouldRemoveWhenDecativated();

};

#endif