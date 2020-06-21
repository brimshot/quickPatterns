#include <qpScene.h>

extern qpLayer *lastAccessedLayer;

qpScene::qpScene(qpLightStrand *lightStrand) {

  this->attachToStrand(lightStrand);
}


void qpScene::attachToStrand(qpLightStrand *lightStrand) {

  this->lightStrand = lightStrand;

  qpLayer *currentLayer = this->layers.fetch();
  int layerIndex = 0;
  while(currentLayer) {
    currentLayer->assignTargetLeds(lightStrand->getLedArray(layerIndex), lightStrand->getNumLeds());
    currentLayer = this->layers.fetch();
    layerIndex++;
  }

}

//TODO: just go on the scene man!
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
    this->layers.getLast()->assignTargetLeds(this->lightStrand->getLedArray(this->layers.numElements), this->lightStrand->getNumLeds());
    //TODO: figure out how to do this!!
//    this->lightStrand->setLastUsedLayer(this->layers.getLast());

    lastAccessedLayer = this->layers.getLast();
  }

  return *this->layers.getLast();
}


qpLayer &qpScene::layer(int index) {

  if(index > (this->layers.numElements - 1))
    return this->addLayer();

  //TODO: figure out what to do with this! a global? kind of gross right. but mayb
//  this->lightStrand->setLastUsedLayer(this->layers.getItem(index));

  //TODO: can this be quicker... ?
  lastAccessedLayer = this->layers.getItem(index);

  return *this->layers.getItem(index);
}


qpPattern &qpScene::operator()(int layerIndex) {

  return this->layer(layerIndex).pattern(0);
}
