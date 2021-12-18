#include "qpColor.h"

// TODO: none of these methods need to be chainable.....

qpColor::qpColor() {
  this->currentColor = CRGB::Black;
  this->colorPalette = CRGBPalette16(CRGB::Black);
}

void qpColor::update() {

  if(this->colorShouldChangePeriodically) {
    if(this->nextColorShouldLoad()) {
      this->loadNextColor();

      this->periodCountAtLastColorChange = *this->colorPeriodsCounter;
    }
  }

}

bool qpColor::nextColorShouldLoad() {
    return ((*this->colorPeriodsCounter - this->periodCountAtLastColorChange) >= this->currentColorDuration);
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

void qpColor::bindColorDurationTimer(unsigned long *periodCounter, int minPeriods, int maxPeriods) {
  this->colorShouldChangePeriodically = true;

  this->colorPeriodsCounter = periodCounter;

  this->currentColorDuration = this->minColorDuration = minPeriods;

  this->maxColorDuration = maxPeriods;
}

qpColor *qpColor::withChanceToChangeColor(uint8_t percentage) {

  this->chanceToChangeColor = constrain(percentage, 0, 100);

  return this;
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

qpColor *qpColor::useColorSet(CRGB *colorSet, uint8_t numElements) {

  this->colorSet = colorSet;
  this->numColorsInSet = numElements;
  this->colorSetIndex = 0;

  return this;
}

qpColor *qpColor::chooseColorFromSet(CRGB *colorSet, uint8_t numElements, QP_COLOR_MODE mode) {

  this->useColorSet(colorSet, numElements);

  if(mode == RANDOM)
    this->loadNextColorFunction = (&qpColor::loadNextColorFromSetRandomly);
  else
    this->loadNextColorFunction = (&qpColor::loadNextColorFromSetSequentially);

  (this->*loadNextColorFunction)(); // move to first color of set

  return this;
}


/*-------- 
Palette config
*/

qpColor *qpColor::usePalette(CRGBPalette16 colorPalette) {

  this->colorPalette = colorPalette;
  this->currentColor = ColorFromPalette(colorPalette, 0);

  return this;
}


qpColor *qpColor::chooseColorFromPalette(CRGBPalette16 colorPalette, QP_COLOR_MODE mode, uint8_t stepSize) {

  this->usePalette(colorPalette);
  this->setPaletteStep(stepSize);

  if(mode == RANDOM)
    this->loadNextColorFunction = (&qpColor::loadNextPaletteColorRandomly);
  else
    this->loadNextColorFunction = (&qpColor::loadNextPaletteColorSequentially);

  return this;
}

qpColor *qpColor::setPaletteStep(uint8_t stepSize) {
      this->paletteStep = stepSize;

      return this;
}     


/*-------- 
Palette config
*/

qpColor *qpColor::singleColor(CRGB color) {

  this->currentColor = color;
  this->colorShouldChangePeriodically = false;

  return this;
}
