#ifndef QP_LAYER_H
#define QP_LAYER_H

#include "qpEnums.h"
#include "qpLinkedList.h"
#include "qpPattern.h"
#include "qpLayerEffect.h"

/*
 * Layer events (messaging queue)
 *
 * Layer effects:
 *
 * - Breathe
 * - Reverse (timing)
 * 
 */


class qpLayer {

  private:

//    (void *) cycleEvents[];
//    qpPattern &addLayerEffect(qpLayerEffect *effect);

    int layerId;

    CRGB *leds;
    int numLeds;

//    qpLinkedList <qpPatternController> patternControllers;
//    qpPatternController *lastReferencedPatternColor;

    qpLinkedList <qpPattern> patterns;
    qpPattern *lastReferencedPattern;

    qpLinkedList <qpLayerEffect> effects;

    int continualFadeAmount = 0;
    bool bPersistWhenPatternsInactive = true;

    // Brushes
    void addToLeds(CRGB *toLeds, CRGB *sourceLeds, int numLeds);
    void subtractFromLeds(CRGB *toLeds, CRGB *sourceLeds, int numLeds);
    void overlayOnLeds(CRGB *toLeds, CRGB *sourceLeds, int numLeds);
    void overwriteLeds(CRGB *toLeds, CRGB *sourceLeds, int numLeds);
    void combineWithLeds(CRGB *toLeds, CRGB *sourceLeds, int numLeds);
    void maskLeds(CRGB *toLeds, CRGB *sourceLeds, int numLeds);

    void (qpLayer::*applyLeds)(CRGB *toLeds, CRGB *sourceLeds, int numLeds); //pointer to brush function

  public:

    qpLayer(CRGB *leds, int numLeds, int layerId);

    // ~ Rendering

    void draw(CRGB *targetLeds, int numLeds);


    // ~ Config

    qpPattern &addPattern(qpPattern *pattern);

    // qpLayer &removePattern(qpPattern *pattern);
    // qpLayer &removePatternAtIndex(int patternIndex);

    qpLayer &continuallyFadeLayerBy(int fadeAmount);
    qpLayer &hideWhenNoActivePatterns(bool trueOrFalse = true);

    qpLayer &setLayerBrush(QP_BRUSH_TYPE brush);
//    qpLayer &setLayerBrush(void (*brushFunc)(CRGB *toLeds, CRGB *sourceLeds, int numLeds));

    // ~ Access

    // Patterns
    qpPattern &pattern(byte patternIndex);
    qpPattern &samePattern() { return *this->lastReferencedPattern; }

    // Quick access operators
    qpPattern &operator()(byte patternIndex);

};

#endif
