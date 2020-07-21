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

    // Brushes
    void addToLeds(CRGB *targetLeds, int numLeds);
    void subtractFromLeds(CRGB *targetLeds, int numLeds);
    void overlayOnLeds(CRGB *targetLeds, int numLeds);
    void overwriteLeds(CRGB *targetLeds, int numLeds);
    void combineWithLeds(CRGB *targetLeds, int numLeds);
    void maskLeds(CRGB *targetLeds, int numLeds);

    void (qpLayer::*applyToLeds)(CRGB *leds, int numLeds); //pointer to selected brush function

  public:

    qpLayer(CRGB *leds, int numLeds);

    // ~ Setup

    void assignTargetLeds(CRGB *leds, int numLeds);


    // ~ Rendering

    void draw(CRGB *targetLeds, int numLeds);


    // ~ Config

    qpPattern &addPattern(qpPattern *pattern);

    qpLayer &setLayerBrush(QP_BRUSH_TYPE brush);
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
