#include <qpLayer.h>

qpLayer::qpLayer(CRGB *leds, int numLeds) {
  
  this->leds = leds;
  this->numLeds = numLeds;

  this->setLayerBrush(OVERLAY);
}


void qpLayer::draw(CRGB *targetLeds, int numLeds) {

  bool patternsRendered = false;

  if(this->continualFadeAmount) //conceivably we prevent a loop that applies 0 to each led with this check
    fadeToBlackBy(this->leds, this->numLeds, this->continualFadeAmount);

  while(qpPattern *currentPattern = this->patterns.fetch())
        patternsRendered |= currentPattern->render();

  if(patternsRendered || this->bPersistWhenPatternsInactive)
    (this->*applyLeds)(targetLeds, this->leds, numLeds);

}

// Config

qpPattern &qpLayer::addPattern(qpPattern *pattern) {

  pattern->assignTargetLeds(this->leds, this->numLeds); //hmmm
  pattern->initialize();

  this->lastReferencedPattern = this->patterns.append(pattern);

  return *pattern;
}

qpLayer &qpLayer::continuallyFadeLayerBy(int fadeAmount) {

  this->continualFadeAmount = constrain(fadeAmount, 0, 255);

  return *this;
}

qpLayer &qpLayer::hideWhenNoActivePatterns(bool trueOrFalse) {

  this->bPersistWhenPatternsInactive = (! trueOrFalse);

  return *this;
}

// Brushes

//direct function assignment - needs all brushes moved to external functions to implement
/*
qpLayer &qpLayer::setLayerBrush(void (*brushFunc)(CRGB *toLeds, CRGB *sourceLeds, int numLeds)) {

  this->applyLeds = brushFunc;

  return *this;
}
*/

//preset
qpLayer &qpLayer::setLayerBrush(QP_BRUSH_TYPE brushType) {

  switch(brushType) {
    case ADD:
      this->applyLeds = &qpLayer::addToLeds;
      break;
    case SUBTRACT:
      this->applyLeds = &qpLayer::subtractFromLeds;
      break;
    case OVERWRITE:
      this->applyLeds = &qpLayer::overwriteLeds;
      break;
    case OVERLAY:
      this->applyLeds = &qpLayer::overlayOnLeds;
      break;
    case COMBINE:
      this->applyLeds = &qpLayer::combineWithLeds;
      break;
    case MASK:
      this->applyLeds = &qpLayer::maskLeds;
      break;
  }

  return *this;
}


void qpLayer::addToLeds(CRGB *targetLeds, CRGB *sourceLeds, int numLeds) {
  for(int i = 0; i < numLeds; i++)
    targetLeds[i] += sourceLeds[i];
}


void qpLayer::subtractFromLeds(CRGB *targetLeds, CRGB *sourceLeds, int numLeds) {
  for(int i = 0; i < numLeds; i++)
    targetLeds[i] -= sourceLeds[i];
}

void qpLayer::overlayOnLeds(CRGB *targetLeds, CRGB *sourceLeds, int numLeds) {
  for(int i = 0; i < numLeds; i++) {
    if(sourceLeds[i] != CRGB(0, 0, 0)) //how costly is this.. ?
      targetLeds[i] = sourceLeds[i];
  }
}

void qpLayer::overwriteLeds(CRGB *targetLeds, CRGB *sourceLeds, int numLeds) {
  memcpy(targetLeds, sourceLeds, (sizeof(CRGB)*numLeds));
}

void qpLayer::combineWithLeds(CRGB *targetLeds, CRGB *sourceLeds, int numLeds) {
  for(int i = 0; i < numLeds; i++)
    targetLeds[i] = blend(targetLeds[i], sourceLeds[i], 128); //TODO: make this better
}

void qpLayer::maskLeds(CRGB *targetLeds, CRGB *sourceLeds, int numLeds) {
  for(int i = 0; i < numLeds; i++) {
    targetLeds[i] -= -sourceLeds[i];
  }
}


// Access

qpPattern &qpLayer::pattern(byte patternIndex) {

  this->lastReferencedPattern = this->patterns.getItem(patternIndex);

  return *this->lastReferencedPattern;
}

qpPattern &qpLayer::operator()(byte patternIndex) {

  return this->pattern(patternIndex);
}
