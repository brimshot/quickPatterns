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
Demo reel using the quickPatterns library

These are examples that tend to look better on flat strips of LEDs, making use of negative space and more rigid movement
**************/


#include <quickPatterns.h>
#include <qpSamplePatterns.h>
#include "popupDroid.h"

#define CHIPSET     WS2812
#define DATA_PIN    8          // pin 11 is hardware SPI on Teensy 3.x and ATMega328P based Arduino
#define NUM_LEDS    100
#define BRIGHTNESS  32
#define COLOR_ORDER GRB         //GRB for WS2812, RGB for WS2811

//Declare master set of leds for FastLED to write to
CRGB leds[NUM_LEDS];

//Declare quickPatterns controller and pass in led data
quickPatterns quickPatterns(leds, NUM_LEDS); //NUM_STRIPS*NUM_LEDS_PER_STRIP);


void setup() {

  delay(3000); // Recovery delay

//  Serial.begin(9600);

  randomSeed(analogRead(1));

  // ~ Configure FastLED

  FastLED.addLeds<CHIPSET, 8, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 450);

  FastLED.clear();


  // ~ Configure quickPatterns

  #ifdef ESP8266
  quickPatterns.setTickMillis(0);
  #endif

  #ifndef ESP8266
  quickPatterns.setTickMillis(25);
  #endif


  // ~ Scene 0 - demonstrates running simultaneous patterns at differing speeds, the same bouncing bars pattern with 3 different configurations will weave in and out of sync

  // Each call to addPattern() automatically creates a new 'layer' on which that pattern is rendered which we can use to combine, blend and time independently

  // blue bars, 9 pixels width
  quickPatterns.addPattern(new qpBouncingBars(9))
    .singleColor(CRGB::Blue);

  // short red bars, 5 pixels
  quickPatterns.addPattern(new qpBouncingBars(5))
    .singleColor(CRGB::Red);
  quickPatterns.sameLayer().setLayerBrush(ADD); //when passing over underlying pixels, add their light together

  // slow yellow bars, 5 pixels
  quickPatterns.addPattern(new qpBouncingBars(5))
    .singleColor(CRGB::Yellow)
    .drawEveryNTicks(3); //move at a slower speed
  quickPatterns.sameLayer().setLayerBrush(COMBINE);


  // ~ Scene 1 - demonstrates a use of state machine patterns

  //popup droid is a little state machine that flashes a few times then moves randomly up and down the strand before disappearing

  //yellow version, less frequent on the lowest layer
  quickPatterns.newScene().addPattern(new popupDroid(16))
    .singleColor(CRGB::Yellow)
    .activatePeriodicallyEveryNTicks(200, 400)
    .stayActiveForNCycles(1);

  //red version, randomly every 20 to 70 ticks
  quickPatterns.sameScene().addPattern(new popupDroid(8))
    .singleColor(CRGB::Red)
    .activatePeriodicallyEveryNTicks(20, 70)
    .stayActiveForNCycles(1);

  //blue version, randomly every 20 to 70 ticks
  quickPatterns.sameScene().addPattern(new popupDroid(8))
    .singleColor(CRGB::Blue)
    .activatePeriodicallyEveryNTicks(20, 70)
    .stayActiveForNCycles(1);
  quickPatterns.sameLayer().setLayerBrush(ADD);


  // ~ Scene 2 - demonstrates the use of the SUBTRACT brush to animate with negative space

  // gradient base layer
  quickPatterns.newScene().addPattern(new qpMovingGradient(OceanColors_p));

  //randomly moving line of negative space
  quickPatterns.sameScene().addPattern(new qpWanderingLine(10))
    .singleColor(CRGB::White);
  quickPatterns.sameLayer().setLayerBrush(SUBTRACT);

  //another
  quickPatterns.sameScene().addPattern(new qpWanderingLine(10))
    .singleColor(CRGB::White);
  quickPatterns.sameLayer().setLayerBrush(SUBTRACT);


  // ~ Scene 3 - demonstrates the MASK brush

  // moving rainbow gradient base layer
  quickPatterns.newScene().addPattern(new qpMovingGradient(RainbowColors_p));

  // soft white confetti. Using the ADD brush to add this layers light to what's below which gives a sparkle effect
  quickPatterns.sameScene().addPattern(new qpConfetti())
    .singleColor(CRGB::White)
    .drawEveryNTicks(4);
  quickPatterns.sameLayer().continuallyFadeLayerBy(30).setLayerBrush(ADD);

  // create a moving visible window into below patterns using the mask brush
  quickPatterns.sameScene().addPattern(new qpWanderingLine(30))
    .singleColor(CRGB::White);
  quickPatterns.sameLayer().setLayerBrush(MASK);


  // ~ Scene 4 - demonstrates periodic pattern activation

  //stop and go rainbow theater chase that randomly starts and stops
  quickPatterns.newScene().addPattern(new qpTheaterChase())
    .drawEveryNTicks(3)
    .chooseColorSequentiallyFromPalette(RainbowColors_p)
    .changeColorEveryNTicks(6)
    .activatePeriodicallyEveryNTicks(25, 75)
    .stayActiveForNTicks(25, 75);

}


void loop()
{



  quickPatterns.draw();
  FastLED.show();

  //On ESP8266 boards due to FastLED latching / write issue, we set our 'tick' length to 0 (see above) and manage the global update speed via hard delay
  #ifdef ESP8266
  FastLED.delay(25);
  #endif

  EVERY_N_SECONDS(30) {
    quickPatterns.nextScene();
  }

}
