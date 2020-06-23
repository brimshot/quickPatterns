/*****************************************************

MIT License

Copyright (c) 2020 Chris Brim

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


/*************
Examples of using the qp library

These are examples that tend to look better on bulb style lights,

These are examples that tend to look better on flat strips of LEDs, making heavy use of negative space, state machine based patterns and

These are examples intended for a matrix with
**************/

#include <FastLED.h>
#include <quickPatterns.h>
#include <qpAllPatterns.h>

#define CHIPSET     WS2812B
#define DATA_PIN    8          // pin 11 is hardware SPI on Teensy 3.x and ATMega328P based Arduino
#define NUM_LEDS_PER_STRIP 50
#define NUM_STRIPS 2
#define NUM_LEDS    NUM_LEDS_PER_STRIP*NUM_STRIPS
#define BRIGHTNESS  32
#define COLOR_ORDER RGB         //GRB for WS2812, RGB for WS2811

/*
#define LIGHTSHOW(NAME) void NAME(quickPatterns &qp) { \
//    static quickPatterns *qp = qp;

*/

/*
#define AT(TICKS) if(doTicks(controller, TICKS)) //if((qp->getTicks() == TICKS) && (previousTick != TICKS))

#define LIGHTSHOW(NAME) void NAME(quickPatterns &controller)
*/

//#define AT(TICKS) qpShow.addEvent()->conditionalFunction = []() { return true; };

//Declare master set of leds for FastLED to write to
CRGB leds[NUM_LEDS];

//Declare EasyLightStrand and pass in CRGB array
quickPatterns quickPatterns(leds, NUM_LEDS); //NUM_STRIPS*NUM_LEDS_PER_STRIP);

//qpShow *qps = new qpShow();

/*
bool funcoTime() {
  return true;
}
*/

void setup() {

//  quickPatterns controller(leds, NUM_LEDS);
  /*
  qps->addEvent()->conditionalFunction = [](quickPatterns *controller) { return controller->isAtTick(300); };
  qps->currentEvent()->eventCode = [](quickPatterns *controller)
  */

  delay(3000); // Recovery delay

//  Serial.begin(9600);

  randomSeed(analogRead(1));

  // ~ Configure FastLED

  FastLED.addLeds<CHIPSET, 8, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);


//  FastLED.addLeds<CHIPSET, 8, COLOR_ORDER>(leds, 0, 50);
  /*
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);
  */

//  FastLED.addLeds<CHIPSET, 9, COLOR_ORDER>(leds, 50, 100);
  /*
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);
  */

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 450);

  FastLED.clear();

  // ~ Setup

  #ifdef ESP8266
  quickPatterns.setTickMillis(0);
  #endif

  #ifndef ESP8266
  quickPatterns.setTickMillis(25);
  #endif

  // ~ Scene 0 - scene 0 demonstrates simultaneous patterns, timed activation and two options for dynamically changing colors

  quickPatterns.addPattern(new qpTheaterChase())
    .singleColor(CRGB::Yellow)
    .drawEveryNTicks(3);

  return;

  /*
  quickPatterns.addPattern(new qpJuggle())
    .chooseColorSequentiallyFromPalette(CRGBPalette16(CRGB::Green, CRGB::Blue))
    .changeColorEveryNTicks(6)
    .drawEveryNTicks(4);
  quickPatterns.sameLayer().continuallyFadeLayerBy(20);

  quickPatterns.addPattern(new qpRandomBar(10))
    .singleColor(CRGB::Yellow)
    .activatePeriodicallyEveryNTicks(10)
    .stayActiveForNFrames(1);
  quickPatterns.sameLayer().continuallyFadeLayerBy(20).setLayerBrush(COMBINE);
  */

  /*
  quickPatterns.addPattern(new qpFeathers(5))
    .drawEveryNTicks(3)
    .activatePeriodicallyEveryNTicks(100)
    .stayActiveForNCycles(1)
    .chooseColorRandomlyFromPalette(RainbowColors_p)
    .changeColorEveryNCycles(1);
  quickPatterns.sameLayer().continuallyFadeLayerBy(20).setLayerBrush(ADD);
  */

  /*
  quickPatterns.addPattern(new qpJuggle())
    .chooseColorSequentiallyFromPalette(CRGBPalette16(CRGB::Yellow, CRGB::Pink))
    .changeColorEveryNTicks(10)
    .drawEveryNTicks(4); //slowing down our updates gives a wider spread to the juggle effect
  quickPatterns.sameLayer().continuallyFadeLayerBy(20);


  quickPatterns.addPattern(new qpBouncingPulse(8))
    .deactivate();
  */

//  qps->currentEvent->conditionalFunction = &funcoTime;

  /*
  quickPatterns.addPattern(new qpPusher(10))
    .singleColor(CRGB::RoyalBlue)
    .drawEveryNTicks(2);
  quickPatterns.sameLayer().continuallyFadeLayerBy(20);

  quickPatterns.addPattern(new qpPusher(6))
    .singleColor(CRGB::Red);
  quickPatterns.sameLayer().continuallyFadeLayerBy(20).setLayerBrush(ADD);

  return;
  */

  // Each call to addPattern() automatically creates a new 'layer' on which that pattern is rendered which we can use to combine, blend and time independently

  // Layer one - a sine wave that moves back and forth along the strip. The wave color moves through the rainbow palette sequentially
  quickPatterns.addPattern(new qpSinelon(16))
    .chooseColorSequentiallyFromPalette(RainbowColors_p)
    .changeColorEveryNTicks(2);
  // The sameLayer() function returns a reference to the last layer accessed. We are going to continually fade this layer's light such that old pixels fade away once rendered. Fade values are from 0-255
  quickPatterns.sameLayer().continuallyFadeLayerBy(20);

//  CRGB pulseColors[3] = {CRGB::Blue, CRGB::Yellow, CRGB::HotPink};
  // Create a color set
  CRGB *pulseColors = new CRGB[3];
  pulseColors[0] = CRGB::Blue;
  pulseColors[1] = CRGB::Yellow;
  pulseColors[2] = CRGB::Pink;

  // A small line of pixels that bounces back and forth along the strip - the color is from our set created above
  quickPatterns.addPattern(new qpBouncingPulse(8))
    .chooseColorSequentiallyFromSet(pulseColors, 3)
    .changeColorEveryNCycles(4);


  quickPatterns.addPattern(new qpFlashRandomSection(10))
      .singleColor(CRGB::White);

  quickPatterns.samePattern().activatePeriodicallyEveryNTicks(100, 200)
      .stayActiveForNCycles(2, 4);

  // ** Scene 1 - scene 1 demonstrates timed activation and duration

  // Scenes are groups of layers + patterns that can be referenced as a unit. When we call addPattern() without the scene() function preceding it (as above), we automatically access scene 0
  // To configure a set of scenes, simply prepend the scene() function before adding patterns with the id of the scene you want to manipulate

  // Lay down a base juggle effect
  quickPatterns.newScene().addPattern(new qpJuggle())
    .chooseColorSequentiallyFromPalette(ForestColors_p)
    .changeColorEveryNTicks(10)
    .drawEveryNTicks(4); //slowing down our updates gives a wider spread to the juggle effect
  quickPatterns.sameLayer().continuallyFadeLayerBy(20);

  // Yellow sine wave that will activate randomly at intervals between 80 and 150 ticks. Once active, the pattern stays active for 50 ticks
  quickPatterns.sameScene().addPattern(new qpBouncingPulse(20))
    .chooseColorSequentiallyFromPalette(CRGBPalette16(CRGB::Blue, CRGB::Purple))
    .changeColorEveryNTicks(1)
    .activatePeriodicallyEveryNTicks(80, 150)
    .stayActiveForNCycles(4);
  quickPatterns.sameLayer().continuallyFadeLayerBy(20).setLayerBrush(ADD);

  quickPatterns.sameScene().addPattern(new qpRandomBar(10))
    .singleColor(CRGB::Yellow)
    .activatePeriodicallyEveryNTicks(160)
    .drawEveryNTicks(10)
    .stayActiveForNFrames(5);
  quickPatterns.sameLayer().continuallyFadeLayerBy(20).setLayerBrush(COMBINE);


  // ** Scene 2 - scene 2 demonstrates two different layer brushes, ADD and SUBTRACT

  // Lay down a base rainbow gradient
  /*
  quickPatterns.scene(2).addPattern(new qpMovingGradient(OceanColors_p, 0))
    .drawEveryNTicks(3);

  // Soft white confetti. Using the ADD brush to add this layers light to those below, we apply a sparkle effect on the underlying gradient colors
  quickPatterns.scene(2).addPattern(new qpConfetti())
    .singleColor(CRGB(64, 64, 64))
    .drawEveryNTicks(4); //slower confetti reduces the busy-ness of the effect
  quickPatterns.sameLayer().continuallyFadeLayerBy(30).setLayerBrush(ADD);

  // Roving negative space - by setting the pattern color to white and using the SUBTRACT brush to subtract this layer's light from those below, we can draw our pattern with negative space
  quickPatterns.scene(2).addPattern(new qpWanderingLine(8))
    .singleColor(CRGB::White);
  quickPatterns.sameLayer().setLayerBrush(SUBTRACT);
  */


  // ~ Scene 3 - scene 3 demonstrates the MASK brush

  // Lay down a base layer of a moving rainbow gradient
  quickPatterns.newScene().addPattern(new qpMovingGradient(RainbowColors_p));

  // Add soft white confetti. Using the ADD brush to add this layers light to what's below which gives a popping sparkle effect and will leave sparkles as the mask fades
  quickPatterns.sameScene().addPattern(new qpConfetti())
    .singleColor(CRGB::White)
    .drawEveryNTicks(4);
  quickPatterns.sameLayer().continuallyFadeLayerBy(30).setLayerBrush(ADD);

  // Add a feather mask. Feathers illuminate the strand in sections,
  quickPatterns.sameScene().addPattern(new qpFeathers(10))
    .singleColor(CRGB::White)
    .drawEveryNTicks(3)
    .activatePeriodicallyEveryNTicks(50, 200)
    .stayActiveForNCycles(1);
  quickPatterns.sameLayer().setLayerBrush(MASK).continuallyFadeLayerBy(10);

//  quickPatterns.playScene(3);

}


void loop()
{

//  myfunc(10);
//  myFirstShow(qp);
//  qps->playShow();
  quickPatterns.draw();
  /*
  AT(100) {
    Serial.println("Hit a boundary of 100 " + String(quickPatterns.getTicks()));
    qp(0,1).stayActiveForNCycles(2)
      .activate();
    WHENDONE(qp(0,1)) {
      qp(0,2).stayActiveForNCycles(1)
        .activate();
    }
  }
  AT(900) {

  }
  */
//  memcpy(leds, myLeds, sizeof(CRGB)*100);
  FastLED.show();

  #ifdef ESP8266
  FastLED.delay(25);
  #endif

  EVERY_N_SECONDS(30) {
//    quickPatterns.playRandomScene();
    quickPatterns.nextScene();
  }

}

/*
LIGHTSHOW(myFirstShow, qp) {
  AT(0) {
    qp(2, 0).setPalette(CRGBPalette16(CRGB::Yellow, CRGB::Orange));
    quickPatterns.playScene(2);
  }
  AT(300) {
    qp(2, 0).setPalette(CRGBPalette16(CRGB::Purple, CRGB::Blue));
  }
  AT(600) {
    RESTARTLIGHTSHOW
  }
}
*/

/*
LIGHTSHOW(myFirstShow) {
  AT(300) {
    controller(0, 1).singleColor(CRGB::Blue)
        .stayActiveForNCycles(2)
        .activate();
  }
  AT(900) {
    controller(0).setPalette(CRGBPalette16(CRGB::Blue, CRGB::Purple));
  }
  AT(1000) {
    controller(0, 1).singleColor(CRGB::Red)
        .stayActiveForNCycles(4)
        .activate();
  }
}
*/

/*
LIGHTSHOW(myfunc, myparam) {
  Serial.println("my param is" + String(myparam));
}
*/
