/*****************************************************

quickPatterns demo reel - patterns to demonstrate some of the options available in the quickPatterns library
https://github.com/brimshot/quickPatterns

MIT License

Copyright (c) 2021 Chris Brim

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*****************************************************/

#ifdef CORE_TEENSY
#define FASTLED_ALLOW_INTERRUPTS 0
#endif

#include <quickPatterns.h>

//////////////////////////////////
// HARDWARE CONSTANTS
//////////////////////////////////

#define CHIPSET     WS2812B
#define DATA_PIN    2
#define NUM_LEDS    150
#define BRIGHTNESS  64
#define COLOR_ORDER GRB         //GRB for WS2812, RGB for WS2811


//////////////////////////////////
// CUSTOM PATTERNS FOR THIS SKETCH
//
// For information on writing custom patterns for use with quickPatterns see: https://github.com/brimshot/quickPatterns
//
//////////////////////////////////

// A juggle effect that slowly increases speed over time
class increasingJuggle : public qpPattern {

  private:
    const int MAX_SPEED = 2;
    const int FRAMES_TO_SHOW_AT_MAX_SPEED = 200;

    int _startingSpeed;
    int speed;
    int velocity = 0;
    int framesRenderedAtMaxSpeed = 0;

    void reset()
    {
      speed = _startingSpeed;
      velocity = framesRenderedAtMaxSpeed = 0;
    }

  public:

    increasingJuggle(int startingSpeed)
    {
      _startingSpeed = speed = startingSpeed;
    }

    void draw() {

      // Speed limiter
      if(this->frames % speed)
        return;

      // Do juggle
      for( int i = 0; i < 8; i++)
        _targetLeds[beatsin16(i+7, 0, _numLeds - 1)] = _getColor();

      // Increase velocity on each render
      velocity += 1;
      if(velocity%10 == 0)
        speed = max(--speed, MAX_SPEED);

      // Stay at max speed for 200 frames then start over
      if(speed == MAX_SPEED) {
        if(++framesRenderedAtMaxSpeed == 200) {
          reset();
        }
      }

    }

};


// Sends segments of lights down the strip one by one and stacks them at the end
class segmentStacker : public qpPattern {

  private:
    const int FRAMES_TO_SHOW_WITH_FULL_STACK = 200;

    int stackSize = 0;
    int segmentSize = 0;
    int segmentPos = 0;
    int framesWithFullStack = 0;

    void reset() { segmentPos = stackSize = framesWithFullStack = 0; }

  public:

    segmentStacker(int segmentSize) : segmentSize(segmentSize) {}

    void draw() {

      // -. Quick note: we do NOT clear the leds here at the beginning of each loop since we are expecting other patterns on the same layer to do it for us

      // Draw stack
      for(int i = (_numLeds-1); i > ((_numLeds-1) - stackSize); i--) {
        _targetLeds[i] = _getColor();
      }

      // Pause or reset at end
      if(stackSize >= (_numLeds-1)) {
        if(framesWithFullStack < FRAMES_TO_SHOW_WITH_FULL_STACK) {
          framesWithFullStack++;
          return;
        }

        reset();
        _countCycle();
        return;
      }

      // Draw segment
      for(int i = segmentPos; i < (segmentPos + segmentSize); i++) {
        _targetLeds[i] = _getColor();
      }

      // Advance segment pos for next frame
      segmentPos++;

      // If we bumped into our stack, add it
      if((segmentPos + segmentSize) >= ((_numLeds-1) - stackSize)) {
        stackSize += segmentSize;
        segmentPos = 0;
      }

    }

};


//////////////////////////////////
// MAIN SKETCH BEGINS HERE
//////////////////////////////////

// Declare array of CRGB objects for use with FastLED
CRGB leds[NUM_LEDS];

// declare quickPatterns controller and pass in main led array
quickPatterns quickPatterns(leds, NUM_LEDS);

void setup() {

  delay(3000); // Recovery delay

  random16_add_entropy(analogRead(0));

  // ~ Configure FastLED

  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 450);

  FastLED.clear();


  /////////// quickPatterns setup

  // ----- Scene 1: Rainbow base layers with sparkling highlights that increase in speed as they shine

  quickPatterns.newScene().addPattern(new qpPaletteGradient(2))
    .usePalette(RainbowColors_p)
    .drawEveryNTicks(2);

  // Decoration layer: juggles that increase in speed then reset after a short time
  quickPatterns.sameScene().addPattern(new increasingJuggle(12))
    .singleColor(CRGB(128, 128, 128));
  quickPatterns.sameLayer().setLayerBrush(ADD).continuallyFadeLayerBy(20);

  // ----- End Scene 1

  // ----- Scene 2: Twinkling rainbow that is slowly revealed by stacking a visible window segment by segment
  
  quickPatterns.newScene().addPattern(new qpPaletteTwinkle(12))
    .usePalette(RainbowColors_p);

  quickPatterns.sameScene().addPattern(new qpFill())
    .singleColor(CRGB::Black);

  quickPatterns.sameLayer().addPattern(new segmentStacker(10))
    .singleColor(CRGB::White);
  quickPatterns.sameLayer().setLayerBrush(MASK);

  // ----- End Scene 2


  // ----- Scene 3: three overlapping layers at different speeds
  
  quickPatterns.newScene().addPattern(new qpBouncingBars(9)) // blue bars, 9 pixels width
    .singleColor(CRGB::Blue);

  quickPatterns.sameScene().addPattern(new qpBouncingBars(5)) // short red bars, 5 pixels
    .singleColor(CRGB::Red);
  quickPatterns.sameLayer().setLayerBrush(ADD); //when passing over underlying pixels, add their light together

  quickPatterns.sameScene().addPattern(new qpBouncingBars(5)) // slow yellow bars, 5 pixels
    .singleColor(CRGB::Yellow)
    .drawEveryNTicks(3); //move at a slower speed
  quickPatterns.sameLayer().setLayerBrush(COMBINE);

  // ----- End Scene 3


  // ----- Scene 4: Linked activations and deactivations

  qpPattern &Sinelon = quickPatterns.newScene().addPattern(new qpSinelon(16))
    .chooseColorFromPalette(RainbowColors_p, SEQUENTIAL)
    .changeColorEveryNTicks(2);
  quickPatterns.sameLayer().continuallyFadeLayerBy(20);

  qpPattern &WhiteLightning = quickPatterns.sameScene().addPattern(new qpLightning(12))
    .singleColor(CRGB::White)
    .activateWhenPatternPHasCompletedNCycles(Sinelon, 2)
    .stayActiveForNCycles(2);

  quickPatterns.sameScene().addPattern(new qpLightning(20))
    .singleColor(CRGB(200, 0, 180))
    .activateWhenPatternPHasDeactivatedNTimes(WhiteLightning, 3) //After pattern p has activated 3 times
    .stayActiveForNCycles(4);

  // ----- End Scene 4


}

void loop()
{
  quickPatterns.show();

  EVERY_N_MINUTES(1) {
    quickPatterns.nextScene();
  }
}
