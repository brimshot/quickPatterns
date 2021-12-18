#include "qpScene.h"

void qpScene::draw(CRGB *targetLeds, int numLeds) {

  while(qpLayer *currentLayer = this->layers.fetch())
    currentLayer->draw(targetLeds, numLeds);
}


qpPattern &qpScene::addPattern(qpPattern *pattern) {

  return this->newLayer().addPattern(pattern);
}


qpLayer &qpScene::newLayer() {

  this->lastReferencedLayer = this->layers.append(new qpLayer(this->lightStrand->getVirtualLeds(this->layers.numElements), this->lightStrand->getNumLeds()));

  return *this->lastReferencedLayer;
}


qpLayer &qpScene::layer(byte index) {

  if(index > (this->layers.numElements - 1))
    return this->newLayer();

  this->lastReferencedLayer = this->layers.getItem(index);

  return *this->lastReferencedLayer;
}
