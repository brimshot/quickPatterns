#include <qpScene.h>


//TODO: just go on the light strand?! why pass the leds? is there a reason?
void qpScene::draw(CRGB *targetLeds, int numLeds) {

  while(qpLayer *currentLayer = this->layers.fetch())
    currentLayer->draw(targetLeds, numLeds);
}



qpPattern &qpScene::addPattern(qpPattern *pattern) {

  return this->addLayer().addPattern(pattern);
}


qpLayer &qpScene::addLayer() {

  this->layers.append(new qpLayer);
  this->lastReferencedLayer = this->layers.getLast();

  if(this->lightStrand)
    this->lastReferencedLayer->assignTargetLeds(this->lightStrand->getLedArray(this->layers.numElements - 1), this->lightStrand->getNumLeds());

  return *this->lastReferencedLayer;
}


qpLayer &qpScene::layer(int index) {

  if(index > (this->layers.numElements - 1))
    return this->addLayer();

  this->lastReferencedLayer = this->layers.getItem(index);

  return *this->lastReferencedLayer;
}
