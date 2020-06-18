#ifndef QP_LAYER_H
#define QP_LAYER_H

#include <quickPatterns.h>

class qpPattern;

class qpLayer {

  private:

    int continualFadeAmount = 0;

    qpLinkedList <qpPattern> patterns;

    void addToLeds(CRGB *targetLeds, int numLeds);
    void subtractFromLeds(CRGB *targetLeds, int numLeds);
    void overlayOnLeds(CRGB *targetLeds, int numLeds);
    void overwriteLeds(CRGB *targetLeds, int numLeds);
    void combineWithLeds(CRGB *targetLeds, int numLeds);
    void maskLeds(CRGB *targetLeds, int numLeds);

    void (qpLayer::*applyToLedsFunction)(CRGB *leds, int numLeds);

  public:

    CRGB *leds;
    int numLeds;

//    qpLayer(EasyLightStrand *easyLightStrand);

    void assignTargetLeds(CRGB *leds, int numLeds);

    qpLayer &setLayerBrush(BRUSH_TYPE brush);

    // ~ Config interface

    qpPattern &addPattern(qpPattern *pattern);

    qpPattern &pattern(int patternIndex) {

      return *this->patterns.getItem(patternIndex);
    }

    qpPattern &operator()(int patternIndex) {

      return this->pattern(patternIndex);
    }

    qpLayer &continuallyFadeLayerBy(int fadeAmount) {
      this->continualFadeAmount = constrain(fadeAmount, 0, 255);

      return *this;
    }

    // ~ Drawing

    //how the hell does passing things in make sense?
    void draw(CRGB *targetLeds, int numLeds);

};

#endif
