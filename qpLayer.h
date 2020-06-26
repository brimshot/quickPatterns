#ifndef QP_LAYER_H
#define QP_LAYER_H

enum QP_BRUSH_TYPE {ADD, SUBTRACT, COMBINE, OVERLAY, OVERWRITE, MASK};

#include <qpLinkedList.h>
#include <qpPattern.h>

class qpLayer {

  private:

    qpLinkedList <qpPattern> patterns;

    CRGB *leds;
    int numLeds;

    int continualFadeAmount = 0;
    bool bPersistWhenPatternsInactive = true;

    qpPattern *lastReferencedPattern;

    void addToLeds(CRGB *targetLeds, int numLeds);
    void subtractFromLeds(CRGB *targetLeds, int numLeds);
    void overlayOnLeds(CRGB *targetLeds, int numLeds);
    void overwriteLeds(CRGB *targetLeds, int numLeds);
    void combineWithLeds(CRGB *targetLeds, int numLeds);
    void maskLeds(CRGB *targetLeds, int numLeds);

    void (qpLayer::*applyToLedsFunction)(CRGB *leds, int numLeds);

  public:

    qpLayer(CRGB *leds, int numLeds) : leds(leds), numLeds(numLeds) {
      this->setLayerBrush(OVERLAY);
      fill_solid(this->leds, this->numLeds, CRGB::Black);
    }

    void assignTargetLeds(CRGB *leds, int numLeds);

    // Config

    qpLayer &setLayerBrush(QP_BRUSH_TYPE brush);

    qpLayer &continuallyFadeLayerBy(int fadeAmount) {
      this->continualFadeAmount = constrain(fadeAmount, 0, 255);

      return *this;
    }

    qpLayer &hideWhenNoActivePatterns(bool trueOrFalse = true) {
      this->bPersistWhenPatternsInactive = (! trueOrFalse);

      return *this;
    }

    // Patterns

    qpPattern &addPattern(qpPattern *pattern);

    qpPattern &pattern(int patternIndex);

    qpPattern &operator()(int patternIndex) {

      return this->pattern(patternIndex);
    }

    qpPattern &samePattern() { return *this->lastReferencedPattern; }

    // Render

    void draw(CRGB *targetLeds, int numLeds);

};

#endif
