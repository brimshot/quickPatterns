#include <qpScene.h>


qpScene::qpScene(quickPatterns *lightStrand) {

  this->attachToStrand(lightStrand);
}


void qpScene::attachToStrand(quickPatterns *lightStrand) {

  this->lightStrand = lightStrand;

  qpLayer *currentLayer = this->layers.fetch();
  int layerIndex = 0;
  while(currentLayer) {
    currentLayer->assignTargetLeds(lightStrand->getLedArray(layerIndex), lightStrand->numLeds);
    currentLayer = this->layers.fetch();
    layerIndex++;
  }

}


void qpScene::draw(CRGB *targetLeds, int numLeds) {

  while(qpLayer *currentLayer = this->layers.fetch())
    currentLayer->draw(targetLeds, numLeds);
}



qpPattern &qpScene::addPattern(qpPattern *pattern) {

  return this->addLayer().addPattern(pattern);
}


qpLayer &qpScene::addLayer() {

  this->layers.append(new qpLayer);

  if(this->lightStrand) {
    this->layers.getLast()->assignTargetLeds(this->lightStrand->getLedArray(this->layers.numElements), this->lightStrand->numLeds);
    this->lightStrand->setLastUsedLayer(this->layers.getLast());
  }

  return *this->layers.getLast();
}


qpLayer &qpScene::layer(int index) {

  if(index > (this->layers.numElements - 1))
    return this->addLayer();

  this->lightStrand->setLastUsedLayer(this->layers.getItem(index));

  return *this->layers.getItem(index);
}


qpPattern &qpScene::operator()(int layerIndex) {

  return this->layer(layerIndex).pattern(0);
}
