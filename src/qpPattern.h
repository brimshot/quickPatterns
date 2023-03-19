#ifndef QP_PATTERN_H
#define QP_PATTERN_H

#include <FastLED.h>
#include "qpEnums.h"
#include "qpLinkedList.h"
#include "qpColor.h"

class qpPattern {

  private:

    // ~ Color

    qpColor *_color;

    // ~ Animation speed

    int ticksBetweenFrames = 1;
    int nextRenderTick = 1;

    // ~ Periodic activation

    bool _isActive = true;
    bool removeOnDeactivation = false;

    bool patternShouldActivatePeriodically = false;
    unsigned long *periodCounterActivationsAreBoundTo = nullptr;
    int minPeriodsBetweenActivations = 0;
    int maxPeriodsBetweenActivations = 0;
    unsigned long nextPeriodToActivateAt = 0;
    int chanceToActivatePattern = 0;

    bool patternShouldDeactivatePeriodically = false;
    unsigned long *activePeriodsCounter = nullptr;
    unsigned int periodCountAtLastActivation = 0;
    int minPeriodsToStayActive = 1;
    int maxPeriodsToStayActive = 0;
    int currentPeriodsToStayActive = 0;

    void bindPeriodicActivationTimer(unsigned long *periodCounter, int minPeriodsBetweenActivations, int maxPeriodsBetweenActivations = 0);
    void bindDeactivationTimer(unsigned long *periodCounter, int minPeriodsToStayActive, int maxPeriodsToStayActive = 0);

    bool patternShouldActivate();
    bool patternShouldDeactivate();
    void resetActivationTimer();

  protected:

    // ~ LEDs
    CRGB *_targetLeds;
    int _numLeds = 0;

    // ~ Color
    CRGB _getColor();
    CRGBPalette16 _getPalette();

    /** @deprecated*/
    inline void _clearLeds() { fill_solid(_targetLeds, _numLeds, CRGB::Black); }

    /** @deprecated*/
    bool _inBounds(int pos) { return ((pos >= 0) && (pos < _numLeds)); }

    inline void _countCycle() { this->cycles++; frames = 0; }
    
    /**
     * Called at each update interval, must be implemented by child classes
     */
    virtual void draw() = 0;
    virtual void onActivate() {}
    virtual void onDeactivate() {}

  public:

    qpPattern();

    // ~ Counters
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
    void render();

    // ~ Status
    bool isActive() { return _isActive; }


    /*--------
    Fluent interface
    */

    qpPattern &drawEveryNTicks(int ticks);

    // Scheduling
    qpPattern &removeWhenDeactivated(bool value);

    qpPattern &activatePeriodicallyEveryNTicks(int minTicks, int maxTicks = 0);

    qpPattern &stayActiveForNTicks(int minTicks, int maxTicks = 0);
    qpPattern &stayActiveForNFrames(int minFrames, int maxFrames = 0);
    qpPattern &stayActiveForNCycles(int minCycles, int maxCycles = 0);
    qpPattern &withChanceOfActivation(uint8_t percentage);

    // Color values
    qpPattern &singleColor(CRGB color);

    qpPattern &usePalette(CRGBPalette16 colorPalette);
    qpPattern &chooseColorFromPalette(CRGBPalette16 colorPalette, QP_COLOR_MODE mode);
    qpPattern &setPaletteStepSize(int size);

    qpPattern &useColorSet(CRGB *colorSet, uint8_t numColorsInSet);
    qpPattern &chooseColorFromSet(CRGB *colorSet, uint8_t numElements, QP_COLOR_MODE mode);

    // Color timing
    qpPattern &changeColorEveryNTicks(int minTicks, int maxTicks = 0);
    qpPattern &changeColorEveryNCycles(int minCycles, int maxCycles = 0);
    qpPattern &changeColorEveryNFrames(int minFrames, int maxFrames = 0);
    qpPattern &changeColorEveryNActivations(int minActivations, int maxActivations = 0);
    qpPattern &withChanceToChangeColor(byte percentage);

    // ~ Linked pattern activations

    qpPattern &activateWhenPatternPActivates(qpPattern &P);
    qpPattern &activateWhenPatternPDeactivates(qpPattern &P);
    qpPattern &activateWhenPatternPHasCompletedNCycles(qpPattern &P, int minCycles, int maxCycles = 0);
    qpPattern &activateWhenPatternPHasRenderedNFrames(qpPattern &P, int minFrames, int maxFrames = 0);
    qpPattern &activateWhenPatternPHasActivatedNTimes(qpPattern &P, int minActivations, int maxActivations = 0);  
    qpPattern &activateWhenPatternPHasDeactivatedNTimes(qpPattern &P, int minActivations, int maxActivations = 0);  

    qpPattern &deactivateWhenPatternPActivates(qpPattern &P);
    qpPattern &deactivateWhenPatternPDeactivates(qpPattern &P);
    qpPattern &deactivateWhenPatternPHasCompletedNCycles(qpPattern &P, int minCycles, int maxCycles = 0);
    qpPattern &deactivateWhenPatternPHasRenderedNFrames(qpPattern &P, int minFrames, int maxFrames = 0);
    qpPattern &deactivateWhenPatternPHasActivatedNTimes(qpPattern &P, int minActivations, int maxActivations = 0);  
    qpPattern &deactivateWhenPatternPHasDeactivatedNTimes(qpPattern &P, int minActivations, int maxActivations = 0);  

    bool activate();
    void deactivate();
    bool shouldRemoveWhenDeactivated();

};

#endif