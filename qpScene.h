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

    // ~ Rendering

    void draw(CRGB *targetLeds, int numLeds);


    // ~ Config

    qpPattern &addPattern(qpPattern *pattern);

    qpLayer &newLayer();


    // ~ Access

    // Layers
    qpLayer &layer(byte index);
    qpLayer &sameLayer() { return *this->lastReferencedLayer; }

    // Quick access operators
    qpPattern &operator()(byte layerIndex) { return this->layer(layerIndex).pattern(0); }

};

#endif
