#include <quickPatterns.h>

quickPatterns::quickPatterns(CRGB *leds, int numLeds) {

  this->lightStrand = new qpLightStrand(leds, numLeds);

  random16_add_entropy(random(0, 1000));
  random16_add_entropy(random(0, 1000));
  random16_add_entropy(analogRead(0));
}


// Render

void quickPatterns::draw() {

  uint32_t currentMillis = millis();

  if(currentMillis >= this->nextTickMillis) {

    this->nextTickMillis = (currentMillis + this->tickLengthInMillis);

    this->currentTick++;

    this->lightStrand->clearMain();

    this->currentScene->draw(this->lightStrand->leds, this->lightStrand->numLeds);
  }

}


// Quick add pattern

qpPattern &quickPatterns::addPattern(qpPattern *pattern) {

  return this->scene(0).newLayer().addPattern(pattern);
}


// Returns requested scene or automatically adds and returns a new scene if index does not yet exist
qpScene &quickPatterns::scene(byte index) {

  if(index > (this->scenes.numElements - 1))
    return this->newScene();

  this->lastReferencedScene = this->scenes.getItem(index);

  return *this->lastReferencedScene;
}

qpScene &quickPatterns::newScene() {

  this->lastReferencedScene = this->scenes.append(new qpScene(this->lightStrand));

  // if this is the first scene added, make it our current scene
  if(this->scenes.numElements == 1)
    this->currentScene = this->lastReferencedScene;

  return *this->lastReferencedScene;
}


// ~ Access

qpLayer &quickPatterns::layer(byte layerIndex) {

  return this->scene(0).layer(layerIndex);
}


// ~ Scene navigation

void quickPatterns::nextScene() {
  this->sceneIndex++;
  if(this->sceneIndex >= this->scenes.numElements)
    this->sceneIndex = 0;

  this->playScene(this->sceneIndex);
}


void quickPatterns::playScene(byte index) {

  if(index >= this->scenes.numElements)
    return;

  this->lightStrand->clearAll();

  this->sceneIndex = index;

  this->currentScene = this->scenes.getItem(index);
}


void quickPatterns::playRandomScene() {

  byte index;
  do {
    index = random8(0, (this->scenes.numElements));
  } while(index == this->sceneIndex);

  this->playScene(index);
}


// Quick access operators

qpPattern&quickPatterns::operator()(byte layerIndex) {

  return this->scene(0).layer(layerIndex).pattern(0);
}

qpPattern&quickPatterns::operator()(byte sceneIndex, byte layerIndex) {

  return this->scene(sceneIndex).layer(layerIndex).pattern(0);
}

qpPattern&quickPatterns::operator()(byte sceneIndex, byte layerIndex, byte patternIndex) {

  return this->scene(sceneIndex).layer(layerIndex).pattern(patternIndex);
}
