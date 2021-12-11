#include "qpColor.h"

qpColor::qpColor() {

  this->currentColor = CRGB::Black;
  this->colorPalette = CRGBPalette16(CRGB::Black);
}

void qpColor::update() {

  if(this->nextColorShouldLoad()) {
    this->loadNextColor();

    this->periodCountAtLastColorChange = *this->colorPeriodsCounter;
  }
}

bool qpColor::nextColorShouldLoad() {

  if(this->colorShouldChangePeriodically) {
    return ((*this->colorPeriodsCounter - this->periodCountAtLastColorChange) >= this->currentColorDuration);
  }

  return false;
}

void qpColor::loadNextColor() {

  if(this->chanceToChangeColor > 0) {
    if(random8(100) > this->chanceToChangeColor)
      return;
  }

  if(this->maxColorDuration)
    this->currentColorDuration = random16(this->minColorDuration, this->maxColorDuration);

  (this->*loadNextColorFunction)();
}


/*-------- 
Timing config
*/

void qpColor::setPeriodCounter(unsigned int *periodCounter) {

  this->colorPeriodsCounter = periodCounter;
}

void qpColor::setColorDuration(int minPeriods, int maxPeriods) {

  this->currentColorDuration = this->minColorDuration = minPeriods;
  this->maxColorDuration = maxPeriods;
}

qpColor &qpColor::withChanceToChangeColor(byte percentage) {

  this->chanceToChangeColor = constrain(percentage, 0, 100);

  return *this;
}


/*-------- 
Load color methods
*/

void qpColor::loadNextPaletteColorSequentially() {

  this->currentColor = ColorFromPalette(this->colorPalette, this->paletteIndex);
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


/*-------- 
Color set config
*/

qpColor &qpColor::useColorSet(CRGB *colorSet, byte numElements) {

  this->colorSet = colorSet;
  this->numColorsInSet = numElements;
  this->colorSetIndex = 0;

  return *this;
}

qpColor &qpColor::chooseColorFromSet(CRGB *colorSet, byte numElements, QP_COLOR_MODE mode) {

  this->colorShouldChangePeriodically = true;

  this->useColorSet(colorSet, numElements);

  if(mode == RANDOM)
    this->loadNextColorFunction = (&qpColor::loadNextColorFromSetRandomly);
  else
    this->loadNextColorFunction = (&qpColor::loadNextColorFromSetSequentially);

  (this->*loadNextColorFunction)(); // move to first color of set

  return *this;
}


/*-------- 
Palette config
*/

qpColor &qpColor::usePalette(CRGBPalette16 colorPalette) {

  this->colorPalette = colorPalette;
  this->currentColor = ColorFromPalette(colorPalette, 0);

  return *this;
}


qpColor &qpColor::chooseColorFromPalette(CRGBPalette16 colorPalette, QP_COLOR_MODE mode, byte stepSize) {

  this->colorShouldChangePeriodically = true;

  this->usePalette(colorPalette);
  this->setPaletteStep(stepSize);

  if(mode == RANDOM)
    this->loadNextColorFunction = (&qpColor::loadNextPaletteColorRandomly);
  else
    this->loadNextColorFunction = (&qpColor::loadNextPaletteColorSequentially);

  return *this;
}

qpColor &qpColor::setPaletteStep(byte stepSize) {
      this->paletteStep = stepSize;

      return *this;
}     


/*-------- 
Palette config
*/

qpColor &qpColor::singleColor(CRGB color) {

  this->currentColor = color;
  this->colorShouldChangePeriodically = false;

  return *this;
}
