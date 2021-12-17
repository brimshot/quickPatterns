#include "qpLayer.h"
#include "layer_effects/qpContinuallyFadeBy.h"

qpLayer::qpLayer(CRGB *leds, int numLeds, int layerIndex) {
  
  this->leds = leds;
  this->numLeds = numLeds;

  this->setLayerBrush(OVERLAY);
}

void qpLayer::draw(CRGB *targetLeds, int numLeds) {

  bool patternsRendered = false;

  /*
  // Pre-render fade of any previous values
  if(this->continualFadeAmount) //conceivably we prevent a loop applying 0 to each led with this check
    fadeToBlackBy(this->leds, this->numLeds, this->continualFadeAmount);
  */

  // Apply pre-render effects
  while(qpLayerEffect *effect = this->preRenderEffects.fetch()) {
      effect->apply(this->leds, this->numLeds);
  }

  // Render patterns
  while(qpPattern *currentPattern = this->patterns.fetch()) {
    currentPattern->render(this->leds, this->numLeds); 
    patternsRendered |= currentPattern->isActive();

    // If this pattern isn't active and it's configured to auto delete when finished,
    // then destroy and remove from the patterns linked list.
    if(!currentPattern->isActive() && currentPattern->shouldRemoveWhenDeactivated()) {
      // If this is considered the lastReferencedPattern, updated it.
        if(currentPattern == this->lastReferencedPattern) {
          this->lastReferencedPattern = nullptr;
        }
        this->patterns.remove(currentPattern);
    }
  }

  // Apply post render effects
  while(qpLayerEffect *effect = this->postRenderEffects.fetch()) {
      effect->apply(this->leds, this->numLeds);
  }

  // Write values into main LED array via selected brush method
  if(patternsRendered || this->bPersistWhenPatternsInactive)
    (this->*applyLeds)(targetLeds, this->leds, numLeds);
}

// ~ Config

qpPattern *qpLayer::addPattern(qpPattern *pattern) {

  pattern->assignTargetLeds(this->leds, this->numLeds);
  pattern->initialize();

  this->lastReferencedPattern = this->patterns.append(pattern);

  return pattern;
}

// ~ Effects

qpLayer &qpLayer::addAfterRenderEffect(qpLayerEffect *effect) {
  this->postRenderEffects.append(effect);

  return *this;
}

qpLayer &qpLayer::addPreRenderEffect(qpLayerEffect *effect) {
  this->preRenderEffects.append(effect);

  return *this;
}

// qpLayer &qpLayer::removePattern(qpPattern *pattern) {
//   this->patterns.remove(pattern);

//   return *this;
// } 

// qpLayer &qpLayer::removePatternAtIndex(int index) {
//   this->patterns.removeAtIndex(index);

//   return *this;
// }

qpLayer &qpLayer::continuallyFadeLayerBy(int fadeAmount) {

  this->addPreRenderEffect(new qpContinuallyFadeBy(constrain(fadeAmount, 0, 255)));

  return *this;
}

qpLayer &qpLayer::hideWhenNoActivePatterns(bool trueOrFalse) {

  this->bPersistWhenPatternsInactive = (! trueOrFalse);

  return *this;
}

// ~ Brush config

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

//external brush assignment - TODO: would all brushes moved to external functions to implement
/*
qpLayer &qpLayer::setLayerBrush(void (*brushFunc)(CRGB *toLeds, CRGB *sourceLeds, int numLeds)) {

  this->applyLeds = brushFunc;

  return *this;
}
*/


// ~ Brushes

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
    if(sourceLeds[i] != CRGB(0, 0, 0)) 
      targetLeds[i] = sourceLeds[i];
  }
}

void qpLayer::overwriteLeds(CRGB *targetLeds, CRGB *sourceLeds, int numLeds) {
  memcpy(targetLeds, sourceLeds, (sizeof(CRGB)*numLeds));
}

void qpLayer::combineWithLeds(CRGB *targetLeds, CRGB *sourceLeds, int numLeds) {
  for(int i = 0; i < numLeds; i++)
    targetLeds[i] |= this->leds[i];
//    targetLeds[i] = blend(targetLeds[i], sourceLeds[i], 128); //TODO: implement true rgb blend
}

void qpLayer::maskLeds(CRGB *targetLeds, CRGB *sourceLeds, int numLeds) {
  for(int i = 0; i < numLeds; i++) {
    targetLeds[i] -= -sourceLeds[i];
  }
}


// ~ Access

qpPattern &qpLayer::pattern(byte patternIndex) {

  this->lastReferencedPattern = this->patterns.getItem(patternIndex);

  return *this->lastReferencedPattern;
}

qpPattern &qpLayer::operator()(byte patternIndex) {

  return this->pattern(patternIndex);
}
