#include <quickPatterns.h>

qpLayer *lastAccessedLayer;

/*-----------
Setup
*/

//int EASYLIGHT_MEM_USE = 0;

quickPatterns::quickPatterns(CRGB *leds, int numLeds) {

  this->lightStrand = new qpLightStrand(leds, numLeds);

  this->currentScene = &this->scene(0); //creates our first scene and sets pointers to this strands LEDs, do we even need this??

  this->addEntropy();
  this->addEntropy();
  this->addEntropy();
//  EASYLIGHT_MEM_USE = ((numLeds * sizeof(CRGB)) + sizeof(quickPatterns));
}



/*---------
Rendering
*/

void quickPatterns::draw() {

  uint32_t currentMillis = millis();

  if(currentMillis >= this->nextTickMillis) {

    this->lightStrand->clearMain();

    this->currentScene->draw(this->lightStrand->leds, this->lightStrand->numLeds);

    this->nextTickMillis = (currentMillis + this->tickLengthInMillis);

    this->currentTick++;
  }

}


/*------------
Quick access add patterns
*/

qpPattern &quickPatterns::addPattern(qpPattern *pattern) {

  return this->scene(0).addLayer().addPattern(pattern);
}


/*------------
Scene and layer access
*/


// Returns requested scene or automatically adds and returns a new scene if index does not yet exist
qpScene &quickPatterns::scene(int index) {

  if(index > (this->scenes.numElements - 1)) {
    this->scenes.append(new qpScene(this->lightStrand));
    return *this->scenes.getLast();
  }

  return *this->scenes.getItem(index);
}

// Returns requested layer or automatically adds and returns a new layer if index does not yet exist
qpLayer &quickPatterns::layer(int layerIndex) {

  lastAccessedLayer = &this->scene(0).layer(layerIndex);

  return *lastAccessedLayer;
}

// Returns pattern 0 on layer indicated by index
qpPattern&quickPatterns::operator()(int layerIndex) {

  //this->lastReferencedLayer
  lastAccessedLayer = &this->scene(0).layer(layerIndex); //shouldn't be necessary?

  return this->scene(0).layer(layerIndex).pattern(0);
}

qpPattern&quickPatterns::operator()(int sceneIndex, int layerIndex) {

//  this->lastReferencedLayer
  lastAccessedLayer = &this->scene(sceneIndex).layer(layerIndex); //shouldn't be necessary?

  return this->scene(sceneIndex).layer(layerIndex).pattern(0);
}

qpPattern&quickPatterns::operator()(int sceneIndex, int layerIndex, int patternIndex) {

  //this->lastReferencedLayer
  lastAccessedLayer = &this->scene(sceneIndex).layer(layerIndex); //shouldnt be necessary

  return this->scene(sceneIndex).layer(layerIndex).pattern(patternIndex);
}


/*-------------
Scene selection
*/

void quickPatterns::nextScene() {
  this->sceneIndex++;
  if(this->sceneIndex >= this->scenes.numElements)
    this->sceneIndex = 0;

  this->playScene(this->sceneIndex);
}


void quickPatterns::playScene(int index) {

  if(index >= this->scenes.numElements)
    return;

  this->lightStrand->clearAll();

  this->sceneIndex = index;

  this->currentScene = this->scenes.getItem(index);
}


void quickPatterns::playRandomScene() {

  int index;
  do {
    index = random8(0, (this->scenes.numElements));
  } while(index == this->sceneIndex);

  this->playScene(index);
}


/*-----------
Util
*/

qpLayer &quickPatterns::sameLayer() {
  return *lastAccessedLayer;
}


inline void quickPatterns::addEntropy() {
  random16_add_entropy(analogRead(0));
}


/**
Debug
*/

/*
int quickPatterns::getMemUse() {
  return EASYLIGHT_MEM_USE;
}
*/
