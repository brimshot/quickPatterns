#include <qpPattern.h>
#include <qpColor.h>


qpPattern::qpPattern() {

  this->activateIfConditionsMet = (&qpPattern::doNothing);
  this->deactivateIfConditionsMet = (&qpPattern::doNothing);

  this->lastReferencedColor = this->colors.append(new qpColor(this));
}

// ~ Render

bool qpPattern::render() {
//bool qpPattern::render(CRGB *targetLeds, int numLeds) {

  (this->*activateIfConditionsMet)();

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
    
    (this->*deactivateIfConditionsMet)();

    return true; //something was rendered
  }

  return false; //nothing rendered
}


void qpPattern::activatePeriodically() {

  if(this->isActive)
    return;

  if(this->ticksUntilActive > 0) {
    this->ticksUntilActive--;
    return;
  }

  this->activate();
  this->resetActivationTimer(); 
}


bool qpPattern::activate() {

  // If we are only activating with a chance, check that here
  if(this->chanceToActivatePattern > 0) {
    if(random16(100) > this->chanceToActivatePattern) {
      return false;
    }
  }

  // If we are staying active for a random period count, set it here
  if(this->maxPeriodsToStayActive)
    this->currentPeriodsToStayActive = random16(this->minPeriodsToStayActive, this->maxPeriodsToStayActive);

  this->periodCountAtLastActivation = *this->activePeriodsCounter;

  this->activations++;

  this->isActive = true;

  return true;
}


void qpPattern::resetActivationTimer() {

  // If we are activating at a random interval, calculate the next interval
  if(this->maxTicksBetweenActivations)
    this->ticksUntilActive = random16(this->minTicksBetweenActivations, this->maxTicksBetweenActivations);
  else
    this->ticksUntilActive = this->minTicksBetweenActivations;
}

void qpPattern::deactivateIfActivePeriodComplete() {

  if((*this->activePeriodsCounter - this->periodCountAtLastActivation) >= this->currentPeriodsToStayActive)
      this->deactivate();

}

bool qpPattern::shouldRemoveWhenDecativated() {
  return this->removeOnDeactivation;
}

//direct external control
void qpPattern::deactivate() {

  this->isActive = false;
}


// ~ Animation

// Pattern speed
qpPattern &qpPattern::drawEveryNTicks(int ticks) {

  this->ticksBetweenFrames = ticks;

  return *this;
}

// Color to draw
CRGB qpPattern::_getColor(byte index) {

   return this->colors.getItem(index)->getColor();
}

CRGBPalette16 qpPattern::_getPalette(byte index) {

   return this->colors.getItem(index)->getPalette();
}

// ~ Periodic activation config

qpPattern &qpPattern::removeWhenDeactivated(bool value) {
  this->removeOnDeactivation = value;
  
  return *this;
}

qpPattern &qpPattern::activatePeriodicallyEveryNTicks(int minTicks, int maxTicks) {

  this->isActive = false;

  this->minTicksBetweenActivations = minTicks;
  this->maxTicksBetweenActivations = maxTicks;
  this->activateIfConditionsMet = (&qpPattern::activatePeriodically);

  this->resetActivationTimer();

  return *this;
}

// Active period duration

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

  this->deactivateIfConditionsMet = (&qpPattern::deactivateIfActivePeriodComplete);
}



// ~ Color config

//sets lastreferenced ptr so fluent chain can continue
qpPattern &qpPattern::color(byte index) {

  if(index > (this->colors.numElements - 1))
    this->lastReferencedColor = this->colors.append(new qpColor(this));
  else  
    this->lastReferencedColor = this->colors.getItem(index);

  return *this; 
}


qpPattern &qpPattern::singleColor(CRGB color) {

  this->sameColor().singleColor(color);

  return *this;
}


qpPattern &qpPattern::usePalette(CRGBPalette16 colorPalette) {

  this->sameColor().usePalette(colorPalette);

  return *this;
}


qpPattern &qpPattern::useColorSet(CRGB *colorSet, byte numColorsInSet){

  this->sameColor().useColorSet(colorSet, numColorsInSet);

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



// Setup - called by Layer

void qpPattern::assignTargetLeds(CRGB *leds, int numLeds) {
  this->_targetLeds = leds;
  this->_numLeds = numLeds;
}




// ~ Palette config

qpPattern &qpPattern::chooseColorFromPalette(CRGBPalette16 colorPalette, QP_COLOR_MODE mode) {

  this->sameColor().chooseColorFromPalette(colorPalette, mode);

  return *this;
}

qpPattern &qpPattern::chooseColorFromSet(CRGB *colorSet, byte numElements, QP_COLOR_MODE mode) {

  this->sameColor().chooseColorFromSet(colorSet, numElements, mode);

  return *this;
}
