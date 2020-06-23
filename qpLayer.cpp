#include <qpLayer.h>

qpLayer::qpLayer() {
  this->setLayerBrush(OVERLAY);
}

qpPattern &qpLayer::addPattern(qpPattern *pattern) {

  this->patterns.append(pattern);
  if(this->leds) { //TODO: do we ever NOT have leds.. ?
    pattern->assignTargetLeds(this->leds, this->numLeds);
    pattern->initialize();
  }

  this->lastReferencedPattern = pattern;

  return *pattern;
}


void qpLayer::draw(CRGB *targetLeds, int numLeds) {

  bool patternsRendered = false;

  if(this->continualFadeAmount)
    fadeToBlackBy(this->leds, this->numLeds, this->continualFadeAmount);

  while(qpPattern *currentPattern = this->patterns.fetch()) {
      (currentPattern->*(currentPattern->updateActiveStatus))();
      if(currentPattern->isActive()) {
        currentPattern->render();
        patternsRendered = true;
      }
  }

  if(patternsRendered || this->bPersistWhenPatternsInactive)
    (this->*applyToLedsFunction)(targetLeds, numLeds);

}

// Set reference to LEDs this layer's patterns will write to which are then written onto main
void qpLayer::assignTargetLeds(CRGB *leds, int numLeds) {

  this->leds = leds;
  this->numLeds = numLeds;

  fill_solid(this->leds, this->numLeds, CRGB::Black);

  //TODO: initialize here... initialize inside the pattern... ? TODO: YES, seems to make sense right
  while(qpPattern *currentPattern = this->patterns.fetch())
    currentPattern->assignTargetLeds(this->leds, this->numLeds);

}



/*------------
Brushes
*/

qpLayer &qpLayer::setLayerBrush(QP_BRUSH_TYPE brushType) {

  switch(brushType) {
    case ADD:
      this->applyToLedsFunction = &qpLayer::addToLeds;
      break;
    case SUBTRACT:
      this->applyToLedsFunction = &qpLayer::subtractFromLeds;
      break;
    case OVERWRITE:
      this->applyToLedsFunction = &qpLayer::overwriteLeds;
      break;
    case OVERLAY:
      this->applyToLedsFunction = &qpLayer::overlayOnLeds;
      break;
    case COMBINE:
      this->applyToLedsFunction = &qpLayer::combineWithLeds;
      break;
    case MASK:
      this->applyToLedsFunction = &qpLayer::maskLeds;
      break;
  }

  return *this;
}


void qpLayer::addToLeds(CRGB *targetLeds, int numLeds) {
  for(int i = 0; i < numLeds; i++)
    targetLeds[i] += this->leds[i];
}


void qpLayer::subtractFromLeds(CRGB *targetLeds, int numLeds) {
  for(int i = 0; i < numLeds; i++)
    targetLeds[i] -= this->leds[i];
}

void qpLayer::overlayOnLeds(CRGB *targetLeds, int numLeds) {
  for(int i = 0; i < numLeds; i++) {
    if(this->leds[i] != CRGB(0, 0, 0))
      targetLeds[i] = this->leds[i];
  }
}

void qpLayer::overwriteLeds(CRGB *targetLeds, int numLeds) {
  memcpy(targetLeds, this->leds, (sizeof(CRGB)*numLeds));
}

void qpLayer::combineWithLeds(CRGB *targetLeds, int numLeds) {
  for(int i = 0; i < numLeds; i++)
    targetLeds[i] = blend(targetLeds[i], this->leds[i], 128);
}

void qpLayer::maskLeds(CRGB *targetLeds, int numLeds) {
  for(int i = 0; i < numLeds; i++) {
    targetLeds[i] -= -this->leds[i];
  }
}


/*-------
Access
*/

qpPattern &qpLayer::pattern(int patternIndex) {

  this->lastReferencedPattern = this->patterns.getItem(patternIndex);

  return *this->lastReferencedPattern;
}
