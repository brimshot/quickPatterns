#include <qpPattern.h>


qpPattern::qpPattern() {

  this->updateActiveStatus = (&qpPattern::doNothing);
  this->deactivateCheck = (&qpPattern::doNothing);

  this->newColor();  //initialize color 0
}


bool qpPattern::render() {

  (this->*updateActiveStatus)();

  if(this->isActive) {

    if(this->ticks == this->nextRenderTick) {
      this->updates++;
      this->nextRenderTick += this->ticksBetweenFrames;
      
      this->draw();
    }

    //update colors after rendering frame so not to skip initial color
    while(qpColor *currentColor = this->colors.fetch())
      (currentColor->*(currentColor->updateColorFunction))();

    this->ticks++;
    
    (this->*deactivateCheck)();

    return true; //something was written
  }

    return false;
}


void qpPattern::activatePeriodically() {

  if(this->isActive)
    return;

  if(this->ticksUntilActive > 0) {
    this->ticksUntilActive--;
    return;
  }

  this->activate();
  this->resetActivationTimer(); //TODO: this is called in both activate and deactivate
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

  this->isActive = true;

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

void qpPattern::deactivateWhenActivePeriodOver() {

  if((*this->activePeriodsCounter - this->periodCountAtLastActivation) >= this->currentPeriodsToStayActive) // {
    this->deactivate();
//    this->resetActivationTimer();
//  }

}

void qpPattern::deactivate() {

  this->isActive = false;
}


// Pattern speed

qpPattern &qpPattern::drawEveryNTicks(int ticks) {

  this->ticksBetweenFrames = ticks;

  return *this;
}


// Periodic pattern activation

qpPattern &qpPattern::activatePeriodicallyEveryNTicks(int minTicks, int maxTicks) {

  this->isActive = false;

  this->minTicksBetweenActivations = minTicks;
  this->maxTicksBetweenActivations = maxTicks;
  this->resetActivationTimer();

  this->updateActiveStatus = (&qpPattern::activatePeriodically);

  return *this;
}


qpPattern &qpPattern::stayActiveForNTicks(int minTicks, int maxTicks) {

  this->activePeriodsCounter = &this->ticks;
  this->setActivePeriod(minTicks, maxTicks);

  return *this;
}


qpPattern &qpPattern::stayActiveForNFrames(int minUpdates, int maxUpdates) {

  this->activePeriodsCounter = &this->updates;
  this->setActivePeriod(minUpdates, maxUpdates);

  return *this;
}


qpPattern &qpPattern::stayActiveForNCycles(int minCycles, int maxCycles) {

  this->activePeriodsCounter = &this->cycles;
  this->setActivePeriod(minCycles, maxCycles);

  return *this;
}


qpPattern &qpPattern::withChanceOfActivation(byte percentage) {

  this->chanceToActivatePattern = constrain(percentage, 0, 100);

  return *this;
}


void qpPattern::setActivePeriod(int minPeriods, int maxPeriods) {

  this->currentPeriodsToStayActive = this->minPeriodsToStayActive = max(1, minPeriods);
  this->maxPeriodsToStayActive = max(0, maxPeriods);

  this->deactivateCheck = (&qpPattern::deactivateWhenActivePeriodOver);
}



// Color settings

CRGB qpPattern::_getColor(byte index) {

   return this->colors.getItem(index)->getColor();
}


qpColor &qpPattern::color(byte index) {

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

qpPattern &qpPattern::withChanceToChangeColor(byte percentage) {

  this->sameColor().withChanceToChangeColor(percentage);

  return *this;
}



// Basic config - called by Layer

void qpPattern::assignTargetLeds(CRGB *leds, int numLeds) {
  this->_targetLeds = leds;
  this->_numLeds = numLeds;
}
