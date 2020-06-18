#ifndef QP_SCENE_H
#define QP_SCENE_H

#include <quickPatterns.h>

class quickPatterns;
class qpLayer;

class qpScene {

  private:

    quickPatterns *lightStrand;

    qpLinkedList <qpLayer> layers;

  public:

    qpScene(quickPatterns *quickPatterns);

    void attachToStrand(quickPatterns *lightStrand);

    qpLayer &layer(int index);
    qpLayer &addLayer();

    qpPattern &addPattern(qpPattern *pattern);

    qpPattern &operator()(int layerIndex); // returns pattern 0 at the specified layer

    void draw(CRGB *targetLeds, int numLeds);
};

#endif
