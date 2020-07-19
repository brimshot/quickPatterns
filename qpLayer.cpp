#include <qpLayer.h>

qpPattern &qpLayer::addPattern(qpPattern *pattern) {

  this->lastReferencedPattern = this->patterns.append(pattern);

  if(this->leds) { //TODO: do we ever NOT have leds.. ?
    pattern->assignTargetLeds(this->leds, this->numLeds);
    pattern->initialize();
  }

  return *pattern;
}


void qpLayer::draw(CRGB *targetLeds, int numLeds) {

  bool patternsRendered = false;

  if(this->continualFadeAmount) //conceivably we prevent a loop applying 0 to each led with this check
    fadeToBlackBy(this->leds, this->numLeds, this->continualFadeAmount);

  while(qpPattern *currentPattern = this->patterns.fetch())
        patternsRendered &= currentPattern->render();

  if(patternsRendered || this->bPersistWhenPatternsInactive)
    (this->*applyToLeds)(targetLeds, numLeds);

}


// Brushes

qpLayer &qpLayer::setLayerBrush(QP_BRUSH_TYPE brushType) {

  switch(brushType) {
    case ADD:
      this->applyToLeds = &qpLayer::addToLeds;
      break;
    case SUBTRACT:
      this->applyToLeds = &qpLayer::subtractFromLeds;
      break;
    case OVERWRITE:
      this->applyToLeds = &qpLayer::overwriteLeds;
      break;
    case OVERLAY:
      this->applyToLeds = &qpLayer::overlayOnLeds;
      break;
    case COMBINE:
      this->applyToLeds = &qpLayer::combineWithLeds;
      break;
    case MASK:
      this->applyToLeds = &qpLayer::maskLeds;
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
    if(this->leds[i] != CRGB(0, 0, 0)) //how costly is this.. ?
      targetLeds[i] = this->leds[i];
  }
}

void qpLayer::overwriteLeds(CRGB *targetLeds, int numLeds) {
  memcpy(targetLeds, this->leds, (sizeof(CRGB)*numLeds));
}

void qpLayer::combineWithLeds(CRGB *targetLeds, int numLeds) {
  for(int i = 0; i < numLeds; i++)
    targetLeds[i] = blend(targetLeds[i], this->leds[i], 128); //TODO: make this better
}

void qpLayer::maskLeds(CRGB *targetLeds, int numLeds) {
  for(int i = 0; i < numLeds; i++) {
    targetLeds[i] -= -this->leds[i];
  }
}


// Access

qpPattern &qpLayer::pattern(byte patternIndex) {

  this->lastReferencedPattern = this->patterns.getItem(patternIndex);

  return *this->lastReferencedPattern;
}
