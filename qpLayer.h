#ifndef QP_LAYER_H
#define QP_LAYER_H

#include <qpLinkedList.h>
#include <qpPattern.h>

enum QP_BRUSH_TYPE {ADD, SUBTRACT, COMBINE, OVERLAY, OVERWRITE, MASK};

class qpLayer {

  private:

    CRGB *leds;
    int numLeds;

    qpLinkedList <qpPattern> patterns;
    qpPattern *lastReferencedPattern;

    int continualFadeAmount = 0;
    bool bPersistWhenPatternsInactive = true;

    // Brush functions
    void addToLeds(CRGB *toLeds, CRGB *sourceLeds, int numLeds);
    void subtractFromLeds(CRGB *toLeds, CRGB *sourceLeds, int numLeds);
    void overlayOnLeds(CRGB *toLeds, CRGB *sourceLeds, int numLeds);
    void overwriteLeds(CRGB *toLeds, CRGB *sourceLeds, int numLeds);
    void combineWithLeds(CRGB *toLeds, CRGB *sourceLeds, int numLeds);
    void maskLeds(CRGB *toLeds, CRGB *sourceLeds, int numLeds);

    void (qpLayer::*applyLeds)(CRGB *toLeds, CRGB *sourceLeds, int numLeds); //pointer to brush function

  public:

    qpLayer(CRGB *leds, int numLeds);

    // ~ Rendering

    void draw(CRGB *targetLeds, int numLeds);


    // ~ Config

    qpPattern &addPattern(qpPattern *pattern);

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
