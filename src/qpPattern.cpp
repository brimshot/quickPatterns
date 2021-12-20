#include "qpPattern.h"

qpPattern::qpPattern() {
  this->_color = new qpColor();
}

/*--------
Setup
*/

void qpPattern::assignTargetLeds(CRGB *leds, int numLeds) {
  this->_targetLeds = leds;
  this->_numLeds = numLeds;
}


/*--------
Rendering
*/

void qpPattern::render() { 
  
  this->ticks++;

  if(this->patternShouldActivatePeriodically) {
    if(this->patternShouldActivate()) {
      if(this->chanceToActivatePattern > 0) {
        if(random16(100) < this->chanceToActivatePattern) {
          this->activate();
        } else {
          this->resetActivationTimer();
        }
      } else {
          this->activate();
      }
    }
  }

  if(this->_isActive) {
    if(this->ticks == this->nextRenderTick) {
        this->frames++;
        this->nextRenderTick += this->ticksBetweenFrames;                    

        this->draw(); 
    }

    this->_color->update();

    if(this->patternShouldDeactivatePeriodically) {
      if(this->patternShouldDeactivate()) {
        this->deactivate();
        if(this->patternShouldActivatePeriodically) {
          this->resetActivationTimer();
        }
      }
    }
  }
  
}

CRGB qpPattern::_getColor() {
  return this->_color->getColor();
}

CRGBPalette16 qpPattern::_getPalette() {
   return this->_color->getPalette();
}


/*--------
Activation and deactivation
*/

bool qpPattern::patternShouldActivate() {

    if(this->_isActive) {
        return false;
    }

    if(*this->periodCounterActivationsAreBoundTo >= this->nextPeriodToActivateAt) {
      return true;
    }

    return false;
}


bool qpPattern::activate() {

  if(this->maxPeriodsToStayActive) {
    this->currentPeriodsToStayActive = random16(this->minPeriodsToStayActive, this->maxPeriodsToStayActive);
  } else {
    this->currentPeriodsToStayActive = this->minPeriodsToStayActive;
  }

  if(this->patternShouldDeactivatePeriodically) {
    this->periodCountAtLastActivation = *this->activePeriodsCounter;
  }

  this->_isActive = true;

  this->activations++;

  this->nextRenderTick = this->ticks;

  this->onActivate();

  return true;
}

bool qpPattern::patternShouldDeactivate() {
    if((*this->activePeriodsCounter - this->periodCountAtLastActivation) >= this->currentPeriodsToStayActive) {
        return true;
    }

    return false;
}

void qpPattern::deactivate() {

  this->_isActive = false;

  this->deactivations++;

  this->onDeactivate();
}

void qpPattern::resetActivationTimer() {

  this->_isActive = false;

  int nextActivationOffset = 0;
  if(this->maxPeriodsBetweenActivations) {
    nextActivationOffset = random16(this->minPeriodsBetweenActivations, this->maxPeriodsBetweenActivations);
  } else {
    nextActivationOffset = this->minPeriodsBetweenActivations;
  }

  this->nextPeriodToActivateAt = (*this->periodCounterActivationsAreBoundTo + nextActivationOffset);
}

bool qpPattern::shouldRemoveWhenDeactivated() {
  return this->removeOnDeactivation;
}


/*--------
Activation timers
*/

void qpPattern::bindPeriodicActivationTimer(unsigned long *periodCounter, int minPeriodsBetweenActivations, int maxPeriodsBetweenActivations) {

  this->patternShouldActivatePeriodically = true;

  this->periodCounterActivationsAreBoundTo = periodCounter;
  this->minPeriodsBetweenActivations = minPeriodsBetweenActivations;
  this->maxPeriodsBetweenActivations = maxPeriodsBetweenActivations;

  this->resetActivationTimer();
}

qpPattern &qpPattern::withChanceOfActivation(uint8_t percentage) {

  this->chanceToActivatePattern = constrain(percentage, 0, 100);

  return *this;
}

// ~ Periodic activation

qpPattern &qpPattern::activatePeriodicallyEveryNTicks(int minTicks, int maxTicks) {

  this->bindPeriodicActivationTimer(&this->ticks, minTicks, maxTicks);

  return *this;
}

// ~ Linked activations

qpPattern &qpPattern::activateWhenPatternPActivates(qpPattern &P) {

  this->bindPeriodicActivationTimer(&P.activations, 1);

  return *this;
}

qpPattern &qpPattern::activateWhenPatternPDeactivates(qpPattern &P) {

  this->bindPeriodicActivationTimer(&P.deactivations, 1);

  return *this;
}

qpPattern &qpPattern::activateWhenPatternPHasCompletedNCycles(qpPattern &P, int minCycles, int maxCycles) {

  this->bindPeriodicActivationTimer(&P.cycles, minCycles, maxCycles);

  return *this;
}

qpPattern &qpPattern::activateWhenPatternPHasRenderedNFrames(qpPattern &P, int minFrames, int maxFrames) {

  this->bindPeriodicActivationTimer(&P.frames, minFrames, maxFrames);

  return *this;
}

qpPattern &qpPattern::activateWhenPatternPHasActivatedNTimes(qpPattern &P, int minTimes, int maxTimes) {

  this->bindPeriodicActivationTimer(&P.activations, minTimes, maxTimes);

  return *this;
}

qpPattern &qpPattern::activateWhenPatternPHasDeactivatedNTimes(qpPattern &P, int minTimes, int maxTimes) {

  this->bindPeriodicActivationTimer(&P.deactivations, minTimes, maxTimes);

  return *this;
}

// ~ Linkded deactivations

qpPattern &qpPattern::deactivateWhenPatternPActivates(qpPattern &P) {
  this->bindDeactivationTimer(&P.activations, 1);
  return *this;
}

qpPattern &qpPattern::deactivateWhenPatternPDeactivates(qpPattern &P) {
  this->bindDeactivationTimer(&P.deactivations, 1);
  return *this;
}

qpPattern &qpPattern::deactivateWhenPatternPHasCompletedNCycles(qpPattern &P, int minCycles, int maxCycles) {
  this->bindDeactivationTimer(&P.cycles, minCycles, maxCycles);
  return *this;
}

qpPattern &qpPattern::deactivateWhenPatternPHasRenderedNFrames(qpPattern &P, int minFrames, int maxFrames) {
  this->bindDeactivationTimer(&P.frames, minFrames, maxFrames);
  return *this;
}

qpPattern &qpPattern::deactivateWhenPatternPHasActivatedNTimes(qpPattern &P, int minTimes, int maxTimes) {
  this->bindDeactivationTimer(&P.activations, minTimes, maxTimes);
  return *this;
}

qpPattern &qpPattern::deactivateWhenPatternPHasDeactivatedNTimes(qpPattern &P, int minTimes, int maxTimes) {
  this->bindDeactivationTimer(&P.deactivations, minTimes, maxTimes);
  return *this;
}


/*--------
Active period duration
*/

void qpPattern::bindDeactivationTimer(unsigned long *periodCounter, int minPeriodsToStayActive, int maxPeriodsToStayActive) {

  this->patternShouldDeactivatePeriodically = true;

  this->activePeriodsCounter = periodCounter;

  this->minPeriodsToStayActive = this->currentPeriodsToStayActive = max(1, minPeriodsToStayActive);

  this->maxPeriodsToStayActive = max(0, maxPeriodsToStayActive);
}

qpPattern &qpPattern::stayActiveForNTicks(int minTicks, int maxTicks) {

  this->bindDeactivationTimer(&this->ticks, minTicks, maxTicks);

  return *this;
}


qpPattern &qpPattern::stayActiveForNFrames(int minFrames, int maxFrames) {

  this->bindDeactivationTimer(&this->frames, minFrames, maxFrames);

  return *this;
}


qpPattern &qpPattern::stayActiveForNCycles(int minCycles, int maxCycles) {

  this->bindDeactivationTimer(&this->cycles, minCycles, maxCycles);

  return *this;
}



/*--------
Color config
*/

qpPattern &qpPattern::singleColor(CRGB color) {

  this->_color->singleColor(color);

  return *this;
}

qpPattern &qpPattern::usePalette(CRGBPalette16 colorPalette) {

  this->_color->usePalette(colorPalette);

  return *this;
}

qpPattern &qpPattern::useColorSet(CRGB *colorSet, byte numColorsInSet){

  this->_color->useColorSet(colorSet, numColorsInSet);

  return *this;
}

qpPattern &qpPattern::chooseColorFromPalette(CRGBPalette16 colorPalette, QP_COLOR_MODE mode) {

  this->_color->chooseColorFromPalette(colorPalette, mode);

  this->changeColorEveryNTicks(1);

  return *this;
}

qpPattern &qpPattern::setPaletteStepSize(int stepSize) {
  this->_color->setPaletteStep(stepSize);

  return *this;
}

qpPattern &qpPattern::chooseColorFromSet(CRGB *colorSet, byte numElements, QP_COLOR_MODE mode) {

  this->_color->chooseColorFromSet(colorSet, numElements, mode);

  return *this;
}


/*--------
Color duration
*/

qpPattern &qpPattern::changeColorEveryNTicks(int minTicks, int maxTicks) {

  this->_color->bindColorDurationTimer(&this->ticks, minTicks, maxTicks);

  return *this;
}


qpPattern &qpPattern::changeColorEveryNCycles(int minCycles, int maxCycles) {

  this->_color->bindColorDurationTimer(&this->cycles, minCycles, maxCycles);

  return *this;
}

qpPattern &qpPattern::changeColorEveryNFrames(int minFrames, int maxFrames) {

  this->_color->bindColorDurationTimer(&this->frames, minFrames, maxFrames);

  return *this;
}

qpPattern &qpPattern::changeColorEveryNActivations(int minActivations, int maxActivations) {

  this->_color->bindColorDurationTimer(&this->activations, minActivations, maxActivations);

  return *this;
}

qpPattern &qpPattern::withChanceToChangeColor(byte percentage) {

  this->_color->withChanceToChangeColor(percentage);

  return *this;
}


/*--------
Misc config
*/

qpPattern &qpPattern::drawEveryNTicks(int ticks) {

  this->ticksBetweenFrames = ticks;

  if(this->nextRenderTick == 0)
      this->nextRenderTick += this->ticksBetweenFrames;

  return *this;
}

qpPattern &qpPattern::removeWhenDeactivated(bool value) {
  this->removeOnDeactivation = value;
  
  return *this;
}
