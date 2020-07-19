#include <qpColor.h>

qpColor::qpColor(qpPattern *parentPattern) {

  this->parentPattern = parentPattern;

  this->currentColor = CRGB::Black;

  this->updateColorFunction = (&qpColor::doNothing);
  this->loadNextColorFunction = (&qpColor::doNothing);
}


void qpColor::updateColorPeriodically() {

  if((*this->colorPeriodsCounter - this->periodCountAtLastColorChange) >= this->currentColorDuration) {

    this->_loadNextColor();

    this->periodCountAtLastColorChange = *this->colorPeriodsCounter;
  }

}


void qpColor::_loadNextColor() {

  if(this->chanceToChangeColor > 0) {
    if(random8(100) > this->chanceToChangeColor)
      return;
  }

  if(this->maxColorDuration)
    this->currentColorDuration = random16(this->minColorDuration, this->maxColorDuration);

  (this->*loadNextColorFunction)();

}

// Color config

qpColor &qpColor::singleColor(CRGB color) {

  this->currentColor = color;
  this->updateColorFunction = (&qpColor::doNothing);
  this->loadNextColorFunction = (&qpColor::doNothing);

  return *this;
}


qpColor &qpColor::chooseColorSequentiallyFromPalette(CRGBPalette16 colorPalette, byte stepSize) {

  this->setPalette(colorPalette, stepSize);
  this->loadNextColorFunction = (&qpColor::loadNextPaletteColorSequentially);

  return *this;
}


qpColor &qpColor::chooseColorRandomlyFromPalette(CRGBPalette16 colorPalette) {

  this->setPalette(colorPalette, 0);
  this->loadNextColorFunction = (&qpColor::loadNextPaletteColorRandomly);

  // Initialize to a random color
  this->loadNextPaletteColorRandomly();

  return *this;
}

qpColor &qpColor::chooseColorSequentiallyFromSet(CRGB *colorSet, byte numColorsInSet) {

  this->setColorSet(colorSet, numColorsInSet);
  this->loadNextColorFunction = (&qpColor::loadNextColorFromSetSequentially);
  this->loadNextColorFromSetSequentially();

  return *this;
}

qpColor &qpColor::chooseColorRandomlyFromSet(CRGB *colorSet, byte numColorsInSet) {

  this->setColorSet(colorSet, numColorsInSet);
  this->loadNextColorFunction = (&qpColor::loadNextColorFromSetRandomly);

  // Initialize to a random color
  this->loadNextColorFromSetRandomly();

  return *this;
}



// Timing config

void qpColor::setColorDuration(int minPeriods, int maxPeriods) {

  this->currentColorDuration = this->minColorDuration = minPeriods;
  this->maxColorDuration = maxPeriods;

  this->updateColorFunction = (&qpColor::updateColorPeriodically);
}


qpColor &qpColor::changeColorEveryNTicks(int minTicks, int maxTicks) {

  this->colorPeriodsCounter = &this->parentPattern->ticks;
  this->setColorDuration(minTicks, maxTicks);

  return *this;
}

qpColor &qpColor::changeColorEveryNCycles(int minCycles, int maxCycles) {

  this->colorPeriodsCounter = &this->parentPattern->cycles;
  this->setColorDuration(minCycles, maxCycles);

  return *this;
}

qpColor &qpColor::changeColorEveryNFrames(int minFrames, int maxFrames) {

  this->colorPeriodsCounter = &this->parentPattern->updates;
  this->setColorDuration(minFrames, maxFrames);

  return *this;
}

qpColor &qpColor::changeColorEveryNActivations(int minActivations, int maxActivations) {

  this->colorPeriodsCounter = &this->parentPattern->activations;
  this->setColorDuration(minActivations, maxActivations);

  return *this;
}


qpColor &qpColor::withChanceToChangeColor(byte percentage) {

  this->chanceToChangeColor = constrain(percentage, 0, 100);

  return *this;
}


// Direct config

qpColor &qpColor::setPalette(CRGBPalette16 colorPalette, byte stepSize) {

  this->colorPalette = colorPalette;
  this->paletteStep = stepSize;
  this->currentColor = ColorFromPalette(colorPalette, 0);

  return *this;
}

qpColor &qpColor::setColorSet(CRGB *colorSet, byte numElements) {

  this->colorSet = colorSet;
  this->numColorsInSet = numElements;
  this->colorSetIndex = 0;

  return *this;
}



// Load color functions

void qpColor::loadNextPaletteColorSequentially() {

  this->currentColor =  ColorFromPalette(this->colorPalette, this->paletteIndex);
  this->paletteIndex += this->paletteStep;
}


void qpColor::loadNextPaletteColorRandomly() {

  this->currentColor =  ColorFromPalette(this->colorPalette, random8(0, 255));
}

void qpColor::loadNextColorFromSetSequentially() {

  this->currentColor = this->colorSet[this->colorSetIndex];
  this->colorSetIndex = (++this->colorSetIndex % this->numColorsInSet);
}

void qpColor::loadNextColorFromSetRandomly() {

  this->currentColor = this->colorSet[random8(0, this->numColorsInSet)];
}