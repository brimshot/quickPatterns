#ifndef QP_SCENE_H
#define QP_SCENE_H

#include <FastLED.h>
#include <qpLinkedList.h>
#include <qpLightStrand.h>
#include <qpLayer.h>

class qpScene {

  private:

    qpLightStrand *lightStrand;

    qpLinkedList <qpLayer> layers;

    qpLayer *lastReferencedLayer;

  public:

    qpScene(qpLightStrand *lightStrand) : lightStrand(lightStrand) {}

    qpLayer &layer(byte index);
    qpLayer &newLayer();
    qpLayer &sameLayer() { return *this->lastReferencedLayer; }

    qpPattern &addPattern(qpPattern *pattern);

    qpPattern &operator()(byte layerIndex) { return this->layer(layerIndex).pattern(0); }

    void draw(CRGB *targetLeds, int numLeds);
};

#endif
