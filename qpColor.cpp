#include <qpColor.h>

qpColor::qpColor(qpPattern *parentPattern) {

  this->parent = parentPattern;

  this->_currentColor = CRGB::Black;

  this->updateColorFunction = (&qpColor::doNothing);
  this->loadNextColorFunction = (&qpColor::doNothing);
}

void qpColor::_loadNextColor() {

  if(this->maxColorDuration)
    this->currentColorDuration = random16(this->minColorDuration, this->maxColorDuration);

  if(this->chanceToChangeColor > 0) {
    if(random16(100) > this->chanceToChangeColor)
      return;
  }

  (this->*loadNextColorFunction)();

}

// Choosing

qpColor &qpColor::singleColor(CRGB color) {

  this->_currentColor = color;
  this->updateColorFunction = (&qpColor::doNothing);

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



// Direct color configuration

qpColor &qpColor::setPalette(CRGBPalette16 colorPalette, byte stepSize) {

  this->_colorPalette = colorPalette;
  this->_paletteStep = stepSize;
  this->_currentColor = ColorFromPalette(colorPalette, 0);

  return *this;
}

qpColor &qpColor::setColorSet(CRGB *colorSet, int numElements) {

  this->_colorSet = colorSet;
  this->numColorsInSet = numElements;
  this->colorSetIndex = 0;

  return *this;
}




// Periodic change

void qpColor::updateColorPeriodically() {

  if((*this->colorPeriodsCounter - this->periodCountAtLastColorChange) >= this->currentColorDuration) {

    this->periodCountAtLastColorChange = *this->colorPeriodsCounter;

    this->_loadNextColor();
  }

}

void qpColor::setColorDuration(int minPeriods, int maxPeriods) {

  this->currentColorDuration = this->minColorDuration = minPeriods;
  this->maxColorDuration = maxPeriods;

  this->updateColorFunction = (&qpColor::updateColorPeriodically);
}


qpColor &qpColor::changeColorEveryNTicks(int minActivations, int maxActivations) {

  this->setColorDuration(minActivations, maxActivations);
  this->colorPeriodsCounter = &this->parent->ticks;

  return *this;
}

qpColor &qpColor::changeColorEveryNCycles(int minCycles, int maxCycles) {

  this->setColorDuration(minCycles, maxCycles);
  this->colorPeriodsCounter = &this->parent->cycles;

  return *this;
}

qpColor &qpColor::changeColorEveryNFrames(int minFrames, int maxFrames) {

  this->setColorDuration(minFrames, maxFrames);
  this->colorPeriodsCounter = &this->parent->updates;

  return *this;
}

qpColor &qpColor::changeColorEveryNActivations(int minActivations, int maxActivations) {

  this->setColorDuration(minActivations, maxActivations);
  this->colorPeriodsCounter = &this->parent->activations;

  return *this;
}


qpColor &qpColor::withChanceToChangeColor(int percentage) {

  this->chanceToChangeColor = constrain(percentage, 0, 100);

  return *this;
}



// Load color functions

void qpColor::loadNextPaletteColorSequentially() {

  this->_currentColor =  ColorFromPalette(this->_colorPalette, this->_paletteIndex);
  this->_paletteIndex += this->_paletteStep;
}


void qpColor::loadNextPaletteColorRandomly() {

  this->_currentColor =  ColorFromPalette(this->_colorPalette, random16(0, 255));
}

void qpColor::loadNextColorFromSetSequentially() {

  this->_currentColor = this->_colorSet[this->colorSetIndex];
  this->colorSetIndex = (++this->colorSetIndex % this->numColorsInSet);
}

void qpColor::loadNextColorFromSetRandomly() {

  this->_currentColor = this->_colorSet[random16(0, this->numColorsInSet)];
}
