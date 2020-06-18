#include <qpPattern.h>


qpPattern::qpPattern() {

  this->_currentColor = CRGB::Black;
  this->renderFunction = (&qpPattern::renderPattern);
  this->updateColorFunction = (&qpPattern::doNothing);
  this->loadNextColorFunction = (&qpPattern::doNothing);
}

/*---------
Rendering
*/

void qpPattern::update() {

  (this->*renderFunction)();
}


void qpPattern::renderPattern() {

  if((this->ticks % this->ticksBetweenFrames) == 0) {
    this->updates++;
    this->draw(); // This is the virtual function overridden by pattern subclasses
  }

  this->ticks++;

  (this->*updateColorFunction)();
}


void qpPattern::renderPatternPeriodically() {

    this->updateActiveStatus(); //TODO: check for activation

    if(this->isActive)
      this->renderPattern();
}


void qpPattern::updateActiveStatus() {

  if(this->isActive) {

    if((*this->activePeriodsCounter - this->periodCountAtLastActivation) >= this->currentPeriodsToStayActive)
      this->deactivate();

    return;
  }

  if(this->ticksUntilActive > 0) {
    this->ticksUntilActive--;
    return;
  }

  this->activate();

  this->resetActivationTimer();
}


void qpPattern::activate() {

  if(this->chanceToActivatePattern > 0) {
    if(random16(100) > this->chanceToActivatePattern) {
      return;
    }
  }

  if(this->maxPeriodsToStayActive)
    this->currentPeriodsToStayActive = random16(this->minPeriodsToStayActive, this->maxPeriodsToStayActive);

  this->periodCountAtLastActivation = *this->activePeriodsCounter;

  this->isActive = true;

  this->activations++;
}



void qpPattern::resetActivationTimer() {

  if(this->maxTicksBetweenActivations)
    this->ticksUntilActive = random16(this->minTicksBetweenActivations, this->maxTicksBetweenActivations);
  else
    this->ticksUntilActive = this->minTicksBetweenActivations;

  this->ticks = 0; // Ticks while active goes back to 0 - doesn't this go in deactivate - does colors count on this... ?
}


void qpPattern::deactivate() {

  this->isActive = false;
  this->resetActivationTimer();
}

/*---------
These functions load a color into _currentColor TODO: move
*/

void qpPattern::loadNextPaletteColorSequentially() {

  this->_paletteIndex += this->_paletteStep;
  this->_currentColor =  ColorFromPalette(this->_colorPalette, this->_paletteIndex);
}


void qpPattern::loadNextPaletteColorRandomly() {

  // Since the last 15 hues of a palette are circular (fade back towards 0), only go to 240
  this->_currentColor =  ColorFromPalette(this->_colorPalette, random16(0, 240));
}

void qpPattern::loadNextColorFromSetSequentially() {

  this->_currentColor = this->_colorSet[++this->colorSetIndex];

  if(this->colorSetIndex >= this->numColorsInSet)
    this->colorSetIndex = 0;
}

void qpPattern::loadNextColorFromSetRandomly() {

  this->_currentColor = this->_colorSet[random16(0, this->numColorsInSet)];
}


/*---------
Pattern speed
*/

qpPattern &qpPattern::drawEveryNTicks(int ticks) {

  this->ticksBetweenFrames = ticks;

  return *this;
}


/*--------
Periodic activation
*/

qpPattern &qpPattern::activateEveryNTicksForNUpdates(int ticks, int minUpdates, int maxUpdates) {

  this->activatePeriodicallyEveryNTicks(ticks);
  this->stayActiveForNUpdates(minUpdates, maxUpdates);

  return *this;
}


qpPattern &qpPattern::activateEveryNTicksForNCycles(int ticks, int minCycles, int maxCycles) {

  this->activatePeriodicallyEveryNTicks(ticks);
  this->stayActiveForNCycles(minCycles, maxCycles);

  return *this;
}


qpPattern &qpPattern::activatePeriodicallyEveryNTicks(int minTicks, int maxTicks) {

  this->isActive = false;

  this->minTicksBetweenActivations = minTicks;
  this->maxTicksBetweenActivations = maxTicks;
  this->resetActivationTimer();

  this->renderFunction = (&qpPattern::renderPatternPeriodically);

  return *this;
}


qpPattern &qpPattern::stayActiveForNTicks(int minTicks, int maxTicks) {

  this->activePeriodsCounter = &this->ticks;
  this->setActivePeriodRange(minTicks, maxTicks);

  return *this;
}


qpPattern &qpPattern::stayActiveForNUpdates(int minUpdates, int maxUpdates) {

  this->activePeriodsCounter = &this->updates;
  this->setActivePeriodRange(minUpdates, maxUpdates);

  return *this;
}


qpPattern &qpPattern::stayActiveForNCycles(int minCycles, int maxCycles) {

  this->activePeriodsCounter = &this->cycles;
  this->setActivePeriodRange(minCycles, maxCycles);

  return *this;
}


qpPattern &qpPattern::withChanceOfActivation(int percentage) {

  this->chanceToActivatePattern = constrain(percentage, 0, 100);

  return *this;
}

void qpPattern::setActivePeriodRange(int minPeriods, int maxPeriods) {

  this->currentPeriodsToStayActive = this->minPeriodsToStayActive = max(1, minPeriods);
  this->maxPeriodsToStayActive = maxPeriods;
}


/*---------
Color timing
*/

qpPattern &qpPattern::changeColorEveryNTicks(int minTicks, int maxTicks) {

  this->setColorDurationRange(minTicks, maxTicks);
  this->colorPeriodsCounter = &this->ticks;

  return *this;
}


qpPattern &qpPattern::changeColorEveryNCycles(int minCycles, int maxCycles) {

  this->setColorDurationRange(minCycles, maxCycles);
  this->colorPeriodsCounter = &this->cycles;

  return *this;
}

qpPattern &qpPattern::changeColorEveryNFrames(int minUpdates, int maxUpdates) {

  this->setColorDurationRange(minUpdates, maxUpdates);
  this->colorPeriodsCounter = &this->updates;

  return *this;
}

qpPattern &qpPattern::changeColorEveryNActivations(int minActivations, int maxActivations) {

  this->setColorDurationRange(minActivations, maxActivations);
  this->colorPeriodsCounter = &this->activations;

  return *this;
}


void qpPattern::setColorDurationRange(unsigned int minPeriods, unsigned int maxPeriods) {

  this->currentColorDuration = this->minColorDuration = minPeriods;
  this->maxColorDuration = maxPeriods;

  this->updateColorFunction = (&qpPattern::updateColorPeriodically);
}


qpPattern &qpPattern::withChanceToChangeColor(byte percentage) {

  this->chanceToChangeColor = constrain(percentage, 0, 100);

  return *this;
}


/*--------
Color change functions
*/

qpPattern &qpPattern::singleColor(CRGB color) {

  this->_currentColor = color;
  this->updateColorFunction = (&qpPattern::doNothing);

  return *this;
}


qpPattern &qpPattern::chooseColorSequentiallyFromPalette(CRGBPalette16 colorPalette, byte stepSize) {

  this->_colorPalette = colorPalette;
  this->_paletteStep = stepSize;
  this->loadNextColorFunction = (&qpPattern::loadNextPaletteColorSequentially);

  // Initialize to the first hue in the palette
  this->_currentColor = ColorFromPalette(this->_colorPalette, 0);

  return *this;
}


qpPattern &qpPattern::chooseColorRandomlyFromPalette(CRGBPalette16 colorPalette) {

  this->_colorPalette = colorPalette;
  this->loadNextColorFunction = (&qpPattern::loadNextPaletteColorRandomly);

  // Initialize to a random color
  this->loadNextPaletteColorRandomly();

  return *this;
}

qpPattern &qpPattern::chooseColorSequentiallyFromSet(CRGB *_colorSet, byte numColorsInSet) {

  this->_colorSet = _colorSet;
  this->numColorsInSet = numColorsInSet;
  this->loadNextColorFunction = (&qpPattern::loadNextColorFromSetSequentially);

  // Initialize to first color in set
  this->_currentColor = _colorSet[0];

  return *this;
}

qpPattern &qpPattern::chooseColorRandomlyFromSet(CRGB *_colorSet, byte numColorsInSet) {

  this->_colorSet = _colorSet;
  this->numColorsInSet = numColorsInSet;
  this->loadNextColorFunction = (&qpPattern::loadNextColorFromSetRandomly);

  // Initialize to a random color
  this->loadNextColorFromSetRandomly();

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




/*---------
Basic config - called by Layer
*/

void qpPattern::assignTargetLeds(CRGB *leds, int numLeds) {
  this->_targetLeds = leds;
  this->_numLeds = numLeds;
}

/*---------
Direct color configuration
*/

qpPattern &qpPattern::setPalette(CRGBPalette16 colorPalette) {

  this->_colorPalette = colorPalette;
  this->_currentColor = ColorFromPalette(colorPalette, 0);

  return *this;
}

qpPattern &qpPattern::setPaletteStep(byte stepSize) {

  this->_paletteStep = stepSize;

  return *this;
}

qpPattern &qpPattern::setColorSet(CRGB *colorSet, int numElements) {

  this->_colorSet = colorSet;
  this->numColorsInSet = numElements;

  this->_currentColor = this->_colorSet[0];
  this->colorSetIndex = 0;

  return *this;
}
