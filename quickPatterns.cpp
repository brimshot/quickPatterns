#include <quickPatterns.h>

quickPatterns::quickPatterns(CRGB *leds, int numLeds) {

  this->lightStrand = new qpLightStrand(leds, numLeds);

  //draw can't draw without this!
  this->currentScene = &this->scene(0);

  random16_add_entropy(random());
  random16_add_entropy(random());
  random16_add_entropy(analogRead(0));
}


/*------------
Render
*/

void quickPatterns::draw() {

  uint32_t currentMillis = millis();

  if(currentMillis >= this->nextTickMillis) {

//    Serial.println("Next tick millis (" + String(this->nextTickMillis) + ") and currentMillis (" + String(currentMillis) + ")");

    this->nextTickMillis = (currentMillis + this->tickLengthInMillis);

    this->currentTick++;

    this->lightStrand->clearMain();

    this->currentScene->draw(this->lightStrand->leds, this->lightStrand->numLeds);
  }

}


/*------------
Pattern quick add
*/

qpPattern &quickPatterns::addPattern(qpPattern *pattern) {

  return this->scene(0).addLayer().addPattern(pattern);
}


/*------------
Scene and layer access
*/

qpScene &quickPatterns::newScene() {

  this->scenes.append(new qpScene(this->lightStrand));  //TODO: why doesn't append return a reference to added scene?
  this->lastReferencedScene = this->scenes.getLast();

  return *this->lastReferencedScene;
}

// Returns requested scene or automatically adds and returns a new scene if index does not yet exist
qpScene &quickPatterns::scene(int index) {

  if(index > (this->scenes.numElements - 1))
    return this->newScene();

  this->lastReferencedScene = this->scenes.getItem(index);

  return *this->lastReferencedScene;
}

qpLayer &quickPatterns::layer(int layerIndex) {

  return this->scene(0).layer(layerIndex);
}


/*------------
Scene navigation
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


/*------------
Access
*/

qpPattern&quickPatterns::operator()(int layerIndex) {

  return this->scene(0).layer(layerIndex).pattern(0);
}

qpPattern&quickPatterns::operator()(int sceneIndex, int layerIndex) {

  return this->scene(sceneIndex).layer(layerIndex).pattern(0);
}

qpPattern&quickPatterns::operator()(int sceneIndex, int layerIndex, int patternIndex) {

  return this->scene(sceneIndex).layer(layerIndex).pattern(patternIndex);
}



/*------------
Debug
*/

/*
int quickPatterns::getMemUse() {
  return EASYLIGHT_MEM_USE;
}
*/
