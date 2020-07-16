#include <qpPattern.h>


qpPattern::qpPattern() {

  this->_currentColor = CRGB::Black;
  this->updateActiveStatus = (&qpPattern::doNothing);
  this->deactiveWhenAppropriate = (&qpPattern::doNothing);
  this->updateColorFunction = (&qpPattern::doNothing);
  this->loadNextColorFunction = (&qpPattern::doNothing);

  //initialize color 0
  this->colors.append(new qpColor());
}


void qpPattern::render() {

  if((this->ticks % this->ticksBetweenFrames) == 0) {
    this->updates++;

    /*
    while(qpColor *currentColor = this->colors.fetch())
      (currentColor->*(currentColor->updateColorFunction))();
    */

    this->draw();
  }

  this->ticks++;

  (this->*updateColorFunction)();

  (this->*deactiveWhenAppropriate)();
}


bool qpPattern::isActive() {

  return this->currentlyActive;
}


void qpPattern::activatePeriodically() {

  if(this->currentlyActive)
    return;

  if(this->ticksUntilActive > 0) {
    this->ticksUntilActive--;
    return;
  }

  this->activate();
  this->resetActivationTimer();
}


void qpPattern::deactivatePeriodically() {

  if((*this->activePeriodsCounter - this->periodCountAtLastActivation) >= this->currentPeriodsToStayActive) {
    this->deactivate();
    this->resetActivationTimer();
  }

}

bool qpPattern::activate() {

  // If we are only activating with a chance, check that here
  if(this->chanceToActivatePattern > 0) {
    if(random16(100) > this->chanceToActivatePattern) {
      return false;
    }
  }

  // If we are staying active for a random period count, do that here
  if(this->maxPeriodsToStayActive)
    this->currentPeriodsToStayActive = random16(this->minPeriodsToStayActive, this->maxPeriodsToStayActive);

  this->periodCountAtLastActivation = *this->activePeriodsCounter;

  this->currentlyActive = true;

  this->activations++;

  return true;
}


void qpPattern::resetActivationTimer() {

  // If we are activating at a random internval, calculate the next interval
  if(this->maxTicksBetweenActivations)
    this->ticksUntilActive = random16(this->minTicksBetweenActivations, this->maxTicksBetweenActivations);
  else
    this->ticksUntilActive = this->minTicksBetweenActivations;
}


void qpPattern::deactivate() {

  this->currentlyActive = false;
}


// Pattern speed

qpPattern &qpPattern::drawEveryNTicks(int ticks) {

  this->ticksBetweenFrames = ticks;

  return *this;
}


// Periodic pattern activation

qpPattern &qpPattern::activatePeriodicallyEveryNTicks(int minTicks, int maxTicks) {

  this->currentlyActive = false;

  this->minTicksBetweenActivations = minTicks;
  this->maxTicksBetweenActivations = maxTicks;
  this->resetActivationTimer();

  this->updateActiveStatus = (&qpPattern::activatePeriodically);

  return *this;
}


qpPattern &qpPattern::stayActiveForNTicks(int minTicks, int maxTicks) {

  this->activePeriodsCounter = &this->ticks;
  this->setActivePeriodRange(minTicks, maxTicks);
  this->deactiveWhenAppropriate = (&qpPattern::deactivatePeriodically);

  return *this;
}


qpPattern &qpPattern::stayActiveForNFrames(int minUpdates, int maxUpdates) {

  this->activePeriodsCounter = &this->updates;
  this->setActivePeriodRange(minUpdates, maxUpdates);
  this->deactiveWhenAppropriate = (&qpPattern::deactivatePeriodically);

  return *this;
}


qpPattern &qpPattern::stayActiveForNCycles(int minCycles, int maxCycles) {

  this->activePeriodsCounter = &this->cycles;
  this->setActivePeriodRange(minCycles, maxCycles);
  this->deactiveWhenAppropriate = (&qpPattern::deactivatePeriodically);

  return *this;
}


qpPattern &qpPattern::withChanceOfActivation(int percentage) {

  this->chanceToActivatePattern = constrain(percentage, 0, 100);

  return *this;
}

void qpPattern::setActivePeriodRange(int minPeriods, int maxPeriods) {

  this->currentPeriodsToStayActive = this->minPeriodsToStayActive = max(1, minPeriods);
  this->maxPeriodsToStayActive = max(0, maxPeriods);
}


// Color timing

qpPattern &qpPattern::changeColorEveryNTicks(int minTicks, int maxTicks) {

  /*
  this->setColorDurationRange(minTicks, maxTicks);
  this->colorPeriodsCounter = &this->ticks;
  */

  this->color(0).changeColorEveryNTicks(minTicks, maxTicks);

  return *this;
}


qpPattern &qpPattern::changeColorEveryNCycles(int minCycles, int maxCycles) {

  /*
  this->setColorDurationRange(minCycles, maxCycles);
  this->colorPeriodsCounter = &this->cycles;
  */

  this->color(0).changeColorEveryNCycles(minCycles, maxCycles);

  return *this;
}

qpPattern &qpPattern::changeColorEveryNFrames(int minFrames, int maxFrames) {

  /*
  this->setColorDurationRange(minFrames, maxFrames);
  this->colorPeriodsCounter = &this->updates;
  */

  this->color(0).changeColorEveryNFrames(minFrames, maxFrames);

  return *this;
}

qpPattern &qpPattern::changeColorEveryNActivations(int minActivations, int maxActivations) {

  /*
  this->setColorDurationRange(minActivations, maxActivations);
  this->colorPeriodsCounter = &this->activations;
  */

  this->color(0).changeColorEveryNActivations(minActivations, maxActivations);

  return *this;
}


/*

void qpPattern::setColorDurationRange(unsigned int minPeriods, unsigned int maxPeriods) {

  this->currentColorDuration = this->minColorDuration = minPeriods;
  this->maxColorDuration = maxPeriods;

  this->updateColorFunction = (&qpPattern::updateColorPeriodically);
}


qpPattern &qpPattern::withChanceToChangeColor(int percentage) {

  this->chanceToChangeColor = constrain(percentage, 0, 100);

  return *this;
}

*/


// Color settings

CRGB qpPattern::_getColor(int index) {

   return this->colors.getItem(index)->getColor();
}

qpColor &qpPattern::color(int index) {

  if(index > (this->colors.numElements - 1))
    this->lastReferencedColor = this->colors.append(new qpColor());
  else
    this->lastReferencedColor = this->colors.getItem(index);

  return *this->lastReferencedColor;
}

qpPattern &qpPattern::singleColor(CRGB color) {

  this->colors.getItem(0)->singleColor(color);

//  this->_currentColor = color;
//  this->updateColorFunction = (&qpPattern::doNothing);

  return *this;
}


qpPattern &qpPattern::chooseColorSequentiallyFromPalette(CRGBPalette16 colorPalette, byte stepSize) {

  /*
  this->setPalette(colorPalette, stepSize);
  this->loadNextColorFunction = (&qpPattern::loadNextPaletteColorSequentially);
  */

  this->color(0).chooseColorSequentiallyFromPalette(colorPalette, stepSize);

  return *this;
}


qpPattern &qpPattern::chooseColorRandomlyFromPalette(CRGBPalette16 colorPalette) {

  /*
  this->setPalette(colorPalette, 0);
  this->loadNextColorFunction = (&qpPattern::loadNextPaletteColorRandomly);

  // Initialize to a random color
  this->loadNextPaletteColorRandomly();
  */

  this->color(0).chooseColorRandomlyFromPalette(colorPalette);

  return *this;
}

qpPattern &qpPattern::chooseColorSequentiallyFromSet(CRGB *colorSet, byte numColorsInSet) {

  this->setColorSet(colorSet, numColorsInSet);
  this->loadNextColorFunction = (&qpPattern::loadNextColorFromSetSequentially);
  this->loadNextColorFromSetSequentially();

  return *this;
}

qpPattern &qpPattern::chooseColorRandomlyFromSet(CRGB *colorSet, byte numColorsInSet) {

  /*
  this->setColorSet(colorSet, numColorsInSet);
  this->loadNextColorFunction = (&qpPattern::loadNextColorFromSetRandomly);

  // Initialize to a random color
  this->loadNextColorFromSetRandomly();
  */

  this->color(0).chooseColorRandomlyFromSet(colorSet, numColorsInSet);

  return *this;
}


void qpPattern::updateColorPeriodically() {

  if((*this->colorPeriodsCounter - this->periodCountAtLastColorChange) >= this->currentColorDuration) {

    this->periodCountAtLastColorChange = *this->colorPeriodsCounter;

    this->_loadNextColor();
  }

}

CRGB qpPattern::_loadNextColor() {

  if(this->maxColorDuration)
    this->currentColorDuration = random16(this->minColorDuration, this->maxColorDuration);

  if(this->chanceToChangeColor > 0) {
    if(random16(100) > this->chanceToChangeColor)
      return this->_currentColor; //failed our chance check, just return the same color we're on
  }

  (this->*loadNextColorFunction)();

  return this->_currentColor;
}


// Load color functions

void qpPattern::loadNextPaletteColorSequentially() {

  this->_currentColor =  ColorFromPalette(this->_colorPalette, this->_paletteIndex);
  this->_paletteIndex += this->_paletteStep;
}


void qpPattern::loadNextPaletteColorRandomly() {

  this->_currentColor =  ColorFromPalette(this->_colorPalette, random16(0, 255));
}

void qpPattern::loadNextColorFromSetSequentially() {

  this->_currentColor = this->_colorSet[this->colorSetIndex];
  this->colorSetIndex = (++this->colorSetIndex % this->numColorsInSet);
}

void qpPattern::loadNextColorFromSetRandomly() {

  this->_currentColor = this->_colorSet[random16(0, this->numColorsInSet)];
}



// Basic config - called by Layer

void qpPattern::assignTargetLeds(CRGB *leds, int numLeds) {
  this->_targetLeds = leds;
  this->_numLeds = numLeds;
}


// Direct color configuration

qpPattern &qpPattern::setPalette(CRGBPalette16 colorPalette, byte stepSize) {

  this->_colorPalette = colorPalette;
  this->_paletteStep = stepSize;
  this->_currentColor = ColorFromPalette(colorPalette, 0);

  return *this;
}

qpPattern &qpPattern::setColorSet(CRGB *colorSet, int numElements) {

  this->_colorSet = colorSet;
  this->numColorsInSet = numElements;
  this->colorSetIndex = 0;

  return *this;
}
