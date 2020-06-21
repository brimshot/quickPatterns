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
    bool bPersistWhenPatternsInactive = false;

    void addToLeds(CRGB *targetLeds, int numLeds);
    void subtractFromLeds(CRGB *targetLeds, int numLeds);
    void overlayOnLeds(CRGB *targetLeds, int numLeds);
    void overwriteLeds(CRGB *targetLeds, int numLeds);
    void combineWithLeds(CRGB *targetLeds, int numLeds);
    void maskLeds(CRGB *targetLeds, int numLeds);

    void (qpLayer::*applyToLedsFunction)(CRGB *leds, int numLeds);

  public:

    void assignTargetLeds(CRGB *leds, int numLeds);


    // ~ Config interface

    qpLayer &setLayerBrush(QP_BRUSH_TYPE brush);

    qpLayer &continuallyFadeLayerBy(int fadeAmount) {
      this->continualFadeAmount = constrain(fadeAmount, 0, 255);

      return *this;
    }

    qpLayer &persistWhenPatternsInactive(bool trueOrFalse = true) {
      this->bPersistWhenPatternsInactive = trueOrFalse;

      return *this;
    }


    // ~ Patterns

    qpPattern &addPattern(qpPattern *pattern);

    qpPattern &pattern(int patternIndex) {

      return *this->patterns.getItem(patternIndex);
    }

    qpPattern &operator()(int patternIndex) {

      return this->pattern(patternIndex);
    }

    // ~ Drawing

    //how the hell does passing things in make sense?
    void draw(CRGB *targetLeds, int numLeds);

};

#endif
