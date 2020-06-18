#include <qpLayer.h>

qpPattern &qpLayer::addPattern(qpPattern *pattern) {

  this->patterns.append(pattern);
  if(this->leds) {
    pattern->assignTargetLeds(this->leds, this->numLeds);
    pattern->initialize();
  }

  this->setLayerBrush(OVERLAY);

  return *pattern;
}


void qpLayer::draw(CRGB *targetLeds, int numLeds) {

  if(this->continualFadeAmount)
    fadeToBlackBy(this->leds, this->numLeds, this->continualFadeAmount);

  while(qpPattern *currentPattern = this->patterns.fetch())
    currentPattern->update();
    
  (this->*applyToLedsFunction)(targetLeds, numLeds);

}


void qpLayer::assignTargetLeds(CRGB *leds, int numLeds) {

  this->leds = leds;
  this->numLeds = numLeds;

  fill_solid(this->leds, this->numLeds, CRGB::Black);

  while(qpPattern *currentPattern = this->patterns.fetch())
    currentPattern->assignTargetLeds(this->leds, this->numLeds);

}

qpLayer &qpLayer::setLayerBrush(BRUSH_TYPE brushType) {

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
    default:
//          Serial.println("Missing brush");
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
