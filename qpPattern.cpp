#include <qpPattern.h>


qpPattern::qpPattern() {

  this->updateActiveStatus = (&qpPattern::doNothing);
  this->deactiveWhenAppropriate = (&qpPattern::doNothing);

  //initialize color 0
  this->newColor();
//  this->colors.append(new qpColor(this));
}


bool qpPattern::render() {

  (this->*updateActiveStatus)();

  if(this->currentlyActive) {

    if((this->ticks % this->ticksBetweenFrames) == 0) {
      this->updates++;

      while(qpColor *currentColor = this->colors.fetch())
        (currentColor->*(currentColor->updateColorFunction))();

      this->draw();
    }

    this->ticks++;

    (this->*deactiveWhenAppropriate)();

    return true;

  }

    return false;
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



// Color settings

CRGB qpPattern::_getColor(int index) {

   return this->colors.getItem(index)->getColor();
}


qpColor &qpPattern::color(int index) {

  if(index > (this->colors.numElements - 1))
    return this->newColor();

  this->lastReferencedColor = this->colors.getItem(index);

  return *this->lastReferencedColor;
}

qpColor &qpPattern::newColor() {

  this->lastReferencedColor = this->colors.append(new qpColor(this));

  return *this->lastReferencedColor;
}

qpPattern &qpPattern::singleColor(CRGB color) {

  this->sameColor().singleColor(color);

  return *this;
}


qpPattern &qpPattern::chooseColorSequentiallyFromPalette(CRGBPalette16 colorPalette, byte stepSize) {

  this->sameColor().chooseColorSequentiallyFromPalette(colorPalette, stepSize);

  return *this;
}


qpPattern &qpPattern::chooseColorRandomlyFromPalette(CRGBPalette16 colorPalette) {

  this->sameColor().chooseColorRandomlyFromPalette(colorPalette);

  return *this;
}

qpPattern &qpPattern::chooseColorSequentiallyFromSet(CRGB *colorSet, byte numColorsInSet) {

  this->sameColor().chooseColorSequentiallyFromSet(colorSet, numColorsInSet);

  return *this;
}

qpPattern &qpPattern::chooseColorRandomlyFromSet(CRGB *colorSet, byte numColorsInSet) {

  this->sameColor().chooseColorRandomlyFromSet(colorSet, numColorsInSet);

  return *this;
}

// Color timing

qpPattern &qpPattern::changeColorEveryNTicks(int minTicks, int maxTicks) {

  this->sameColor().changeColorEveryNTicks(minTicks, maxTicks);

  return *this;
}


qpPattern &qpPattern::changeColorEveryNCycles(int minCycles, int maxCycles) {

  this->sameColor().changeColorEveryNCycles(minCycles, maxCycles);

  return *this;
}

qpPattern &qpPattern::changeColorEveryNFrames(int minFrames, int maxFrames) {

  this->sameColor().changeColorEveryNFrames(minFrames, maxFrames);

  return *this;
}

qpPattern &qpPattern::changeColorEveryNActivations(int minActivations, int maxActivations) {

  this->sameColor().changeColorEveryNActivations(minActivations, maxActivations);

  return *this;
}

qpPattern &qpPattern::withChanceToChangeColor(int percentage) {

  this->sameColor().withChanceToChangeColor(percentage);

  return *this;
}



// Basic config - called by Layer

void qpPattern::assignTargetLeds(CRGB *leds, int numLeds) {
  this->_targetLeds = leds;
  this->_numLeds = numLeds;
}
