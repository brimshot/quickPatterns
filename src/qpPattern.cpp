#include "qpPattern.h"

//TODO: this gets called as well as the child constructor?
qpPattern::qpPattern() {
  this->_color = new qpColor();
}

// ~ Render

bool qpPattern::render() {

    this->ticks++;

    if(this->patternShouldActivate()) {
        this->activate();
    }

    if(this->isActive) {
        if(this->ticks == this->nextRenderTick) {
            this->frames++;
            this->nextRenderTick += this->ticksBetweenFrames;                    
            
            //TODO: pass in leds / numLeds here and think about testing!!!!! colors!!!
            this->draw();
        }
    
        this->_color->update();

        if(this->patternShouldDeactivate()) {
            this->deactivate();
            this->resetActivationTimer();
        }

        return true;
    }

    return false;
}


void qpPattern::resetActivationTimer() {

  if(this->periodCounterActivationsAreBoundTo == nullptr) // TODO: there's a nicer way to do this
    return;

  int nextActivationOffset = 0;
  if(this->maxPeriodsBetweenActivations) {
    nextActivationOffset = random16(this->minPeriodsBetweenActivations, this->maxPeriodsBetweenActivations);
  } else {
    nextActivationOffset = this->minPeriodsBetweenActivations;
  }

  this->nextPeriodToActivateAt = (*this->periodCounterActivationsAreBoundTo + nextActivationOffset);
}

bool qpPattern::shouldRemoveWhenDecativated() {
  return this->removeOnDeactivation;
}


// ~ Animation

// Pattern speed
qpPattern *qpPattern::drawEveryNTicks(int ticks) {

  this->ticksBetweenFrames = ticks;

  if(this->nextRenderTick == 0)
      this->nextRenderTick += this->ticksBetweenFrames;

  return this;
}

// Color to draw
CRGB qpPattern::_getColor(byte index) {

  return this->_color->getColor();
//   return this->colors.getItem(index)->getColor();
}

CRGBPalette16 qpPattern::_getPalette(byte index) {

   return this->_color->getPalette();
}

// ~ Periodic activation config

qpPattern &qpPattern::removeWhenDeactivated(bool value) {
  this->removeOnDeactivation = value;
  
  return *this;
}

qpPattern *qpPattern::activatePeriodicallyEveryNTicks(int minTicks, int maxTicks) {

  // TODO: make generic

  this->isActive = false;

  this->patternShouldActivatePeriodically = true;

  // TODO: make this a method
  this->minPeriodsBetweenActivations = minTicks;
  this->maxPeriodsBetweenActivations = maxTicks;
  this->periodCounterActivationsAreBoundTo = &this->ticks;

  this->resetActivationTimer();

  return this;
}



// Active period duration

qpPattern &qpPattern::stayActiveForNTicks(int minTicks, int maxTicks) {

  this->activePeriodsCounter = &this->ticks;
  this->setActivePeriod(minTicks, maxTicks);

  return *this;
}


qpPattern &qpPattern::stayActiveForNFrames(int minUpdates, int maxUpdates) {

  this->activePeriodsCounter = &this->frames;
  this->setActivePeriod(minUpdates, maxUpdates);

  return *this;
}


qpPattern *qpPattern::stayActiveForNCycles(int minCycles, int maxCycles) {

  this->activePeriodsCounter = &this->cycles;
  this->setActivePeriod(minCycles, maxCycles);

  return this;
}


qpPattern &qpPattern::withChanceOfActivation(byte percentage) {

  this->chanceToActivatePattern = constrain(percentage, 0, 100);

  return *this;
}

// TODO: make chainable? it's private right
void qpPattern::setActivePeriod(int minPeriods, int maxPeriods) {
  // TODO: can't remove currentperiods yet
  this->minPeriodsToStayActive = this->currentPeriodsToStayActive = (1, minPeriods);
  this->maxPeriodsToStayActive = max(0, maxPeriods);
}



// ~ Color config


qpPattern *qpPattern::singleColor(CRGB color) {

  this->_color->singleColor(color);

  return this;
}


qpPattern &qpPattern::usePalette(CRGBPalette16 colorPalette) {

  this->_color->usePalette(colorPalette);

  return *this;
}


qpPattern &qpPattern::useColorSet(CRGB *colorSet, byte numColorsInSet){

  this->_color->useColorSet(colorSet, numColorsInSet);

  return *this;
}


// Color timing

qpPattern &qpPattern::changeColorEveryNTicks(int minTicks, int maxTicks) {

  this->_color->setPeriodCounter(&this->ticks);
  this->_color->setColorDuration(minTicks, maxTicks);

  return *this;
}


qpPattern &qpPattern::changeColorEveryNCycles(int minCycles, int maxCycles) {

  this->_color->setPeriodCounter(&this->cycles);
  this->_color->setColorDuration(minCycles, maxCycles);

  return *this;
}

qpPattern &qpPattern::changeColorEveryNFrames(int minFrames, int maxFrames) {

  this->_color->setPeriodCounter(&this->frames);
  this->_color->setColorDuration(minFrames, maxFrames);

  return *this;
}

qpPattern &qpPattern::changeColorEveryNActivations(int minActivations, int maxActivations) {

  this->_color->setPeriodCounter(&this->activations);
  this->_color->setColorDuration(minActivations, maxActivations);

  return *this;
}

qpPattern &qpPattern::withChanceToChangeColor(byte percentage) {

  this->_color->withChanceToChangeColor(percentage);

  return *this;
}



// Setup - called by Layer. Separate injection method allows user defined constructor

void qpPattern::assignTargetLeds(CRGB *leds, int numLeds) {
  this->_targetLeds = leds;
  this->_numLeds = numLeds;
}




// ~ Palette config

//TODO: add step size in here!
qpPattern *qpPattern::chooseColorFromPalette(CRGBPalette16 colorPalette, QP_COLOR_MODE mode) {

  this->_color->chooseColorFromPalette(colorPalette, mode);

  return this;
}

qpPattern &qpPattern::chooseColorFromSet(CRGB *colorSet, byte numElements, QP_COLOR_MODE mode) {

  this->_color->chooseColorFromSet(colorSet, numElements, mode);

  return *this;
}


/*
// ~ Events

void qpPattern::handleEvent(qpPatternEvents::Event e) {
    while(qpPatternEvents::EventHook *h = this->eventHooks.fetch()) {
      if((e.layerIndex == h->layerIndex) && (e.patternIndex == h->patternIndex) && (e.event == h->event)) {
        (*h->action)();
        break;
      }
    }

}
*/





// ~ State management

bool qpPattern::patternShouldActivate() {

    if(this->isActive) {
        return false;
    }

    if(! this->patternShouldActivatePeriodically) {
      return false;
    }

    if(*this->periodCounterActivationsAreBoundTo >= this->nextPeriodToActivateAt) {
      return true;
    }

    return false;
}

qpPattern *qpPattern::activateWhenPatternPDeactivates(qpPattern *P) {

  this->isActive = false;

  this->patternShouldActivatePeriodically = true;

  this->minPeriodsBetweenActivations = 1;
  this->periodCounterActivationsAreBoundTo = &P->deactivations;

  // TODO: it's something in here
  // Serial.println("P deactivations");
  // Serial.println(P->deactivations);
  // Serial.println("*this->periodActivations");
  // Serial.println(*this->periodCounterActivationsAreBoundTo);

  this->resetActivationTimer();

  return this;
}


bool qpPattern::activate() {

  // If we are only activating with a chance, check that here
  if(this->chanceToActivatePattern > 0) {
    if(random16(100) > this->chanceToActivatePattern) {
      return false;
    }
  }

  // If we are staying active for a random period count, set it here
  if(this->maxPeriodsToStayActive) {
    this->currentPeriodsToStayActive = random16(this->minPeriodsToStayActive, this->maxPeriodsToStayActive);
  } else {
    this->currentPeriodsToStayActive = this->minPeriodsToStayActive;
  }

  //TODO: this is the problem!!!!
  if(this->activePeriodsCounter != nullptr)
    this->periodCountAtLastActivation = *this->activePeriodsCounter; //TODO: this might not be initialized

  this->activations++;

  this->nextRenderTick = this->ticks;

  this->isActive = true;

  this->onActivate();

  return true;
}

bool qpPattern::patternShouldDeactivate() {

    if(this->activePeriodsCounter == nullptr)
      return false;

    if((*this->activePeriodsCounter - this->periodCountAtLastActivation) >= this->currentPeriodsToStayActive) {
        return true;
    }

    return false;
}

void qpPattern::deactivate() {
  this->isActive = false;
  this->deactivations++;
  this->onDeactivate();
}

qpPattern *qpPattern::beginInActiveState() {
  this->activate();

  return this;
}