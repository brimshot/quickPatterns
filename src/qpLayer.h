#ifndef QP_LAYER_H
#define QP_LAYER_H

#include "qpEnums.h"
#include "qpLinkedList.h"
#include "qpPattern.h"
#include "qpLayerEffect.h"

class qpLayer {

  private:

    CRGB *leds;
    int numLeds;

    qpLinkedList <qpPattern> patterns;
    qpPattern *lastReferencedPattern;

    qpLinkedList <qpLayerEffect> preRenderEffects;
    qpLinkedList <qpLayerEffect> postRenderEffects;

    bool bPersistWhenPatternsInactive = true;

    // Brushes
    void addToLeds(CRGB *targetLeds, CRGB *sourceLeds, int numLeds);
    void subtractFromLeds(CRGB *targetLeds, CRGB *sourceLeds, int numLeds);
    void overlayOnLeds(CRGB *targetLeds, CRGB *sourceLeds, int numLeds);
    void overwriteWithLeds(CRGB *targetLeds, CRGB *sourceLeds, int numLeds);
    void combineWithLeds(CRGB *targetLeds, CRGB *sourceLeds, int numLeds);
    void maskLeds(CRGB *targetLeds, CRGB *sourceLeds, int numLeds);

    void (qpLayer::*applyLeds)(CRGB *targetLeds, CRGB *sourceLeds, int numLeds); //pointer to brush function

  public:

    qpLayer(CRGB *leds, int numLeds);

    // ~ Rendering

    void draw(CRGB *targetLeds, int numLeds);

    // ~ Patterns

    qpPattern &addPattern(qpPattern *pattern);

    // ~ Config

    // qpLayer &removePattern(qpPattern *pattern);
    // qpLayer &removePatternAtIndex(int patternIndex);


    // Brush
    qpLayer &setLayerBrush(QP_BRUSH_TYPE brush);
//    qpLayer &setLayerBrush(void (*brushFunc)(CRGB *targetLeds, CRGB *sourceLeds, int numLeds));

    // Effects
    qpLayer &addPreRenderEffect(qpLayerEffect *effect);
    qpLayer &addAfterRenderEffect(qpLayerEffect *effect);

    qpLayer &continuallyFadeLayerBy(int fadeAmount);
    qpLayer &hideWhenNoActivePatterns(bool trueOrFalse = true);

    // ~ Access

    // Patterns
    qpPattern &pattern(byte patternIndex);
    qpPattern &samePattern() { return *this->lastReferencedPattern; }

    // Quick access operators
    qpPattern &operator()(byte patternIndex);

};

#endif
