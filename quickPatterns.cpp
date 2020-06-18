#include <quickPatterns.h>

/*-----------
Setup
*/

//int EASYLIGHT_MEM_USE = 0;

quickPatterns::quickPatterns(CRGB *leds, int numLeds) : leds(leds), numLeds(numLeds) {

  this->currentScene = &this->scene(0); //creates our first scene and sets pointers to this strands LEDs, do we even need this?

//  EASYLIGHT_MEM_USE = ((numLeds * sizeof(CRGB)) + sizeof(quickPatterns));
}



/*---------
Rendering
*/

void quickPatterns::draw() {

  uint32_t currentMillis = millis();

  if(currentMillis >= this->nextTickMillis) {

    this->clearMain();

    this->currentScene->draw(this->leds, this->numLeds);

    this->nextTickMillis = (currentMillis + this->tickLengthInMillis);
  }

}

void quickPatterns::clearMain() {

  fill_solid(this->leds, this->numLeds, CRGB::Black);
}

void quickPatterns::clearAll() {

  this->clearMain();

  while(CRGB *currentLeds = this->ledArrays.fetch())
    fill_solid(currentLeds, this->numLeds, CRGB::Black);

}


/*-----------
Master led arrays
*/

/**
The led arrays are associated with the master strand. This allows us to re-use led arrays when switching scenes instead of each scene / layer unique pair declaring it's own set of leds in memory
*/
CRGB *quickPatterns::getLedArray(int index) {

  if(index > (this->ledArrays.numElements-1)) {
    this->ledArrays.append(new CRGB[this->numLeds]);
    return this->ledArrays.getLast();
  }

  return this->ledArrays.getItem(index);
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


qpScene &quickPatterns::scene(int index) {

  if(index > (this->scenes.numElements - 1)) {
    this->scenes.append(new qpScene(this));
    return *this->scenes.getLast();
  }

  return *this->scenes.getItem(index);
}

qpLayer &quickPatterns::layer(int layerIndex) {

  this->lastReferencedLayer = &this->scene(0).layer(layerIndex);

  return *this->lastReferencedLayer;
}

qpPattern&quickPatterns::operator()(int layerIndex) {

  this->lastReferencedLayer = &this->scene(0).layer(layerIndex); //shouldn't be necessary

  return this->scene(0).layer(layerIndex).pattern(0);
}

qpPattern&quickPatterns::operator()(int layerIndex, int patternIndex) {

  this->lastReferencedLayer = &this->scene(0).layer(layerIndex); //shouldn't be necessary

  return this->scene(0).layer(layerIndex).pattern(patternIndex);
}


qpPattern&quickPatterns::operator()(int sceneIndex, int layerIndex, int patternIndex) {

  this->lastReferencedLayer = &this->scene(sceneIndex).layer(layerIndex); //shouldnt be necessary

  return this->scene(sceneIndex).layer(layerIndex).pattern(patternIndex);
}

/*-------------
Scene selection
*/

void quickPatterns::loadScene(qpScene &scene) {

  this->clearAll();

  scene.attachToStrand(this);
  this->currentScene = &scene;
}


void quickPatterns::nextScene() {
  this->sceneIndex++;
  if(this->sceneIndex >= this->scenes.numElements)
    this->sceneIndex = 0;

  this->playScene(this->sceneIndex);
}


void quickPatterns::playScene(int index) {

  if(index >= this->scenes.numElements)
    return;

  this->clearAll();

  this->currentScene = this->scenes.getItem(index);
}


void quickPatterns::playRandomScene() {

  int index = random8(0, (this->scenes.numElements-1));
  this->playScene(index);
}


/*-----------
Util
*/

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
