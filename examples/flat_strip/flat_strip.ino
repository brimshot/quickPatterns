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
Examples of using the quickPatterns library

These are examples that tend to look better on flat strips of LEDs, making use of negative space and state machine based patterns
**************/


#include <quickPatterns.h>
#include <qpAllPatterns.h>

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

  //TODO: esp8266
  quickPatterns.setTickMillis(25);


  // ~ Scene 0 - scene 0 demonstrates simultaneous patterns at differing speeds
  /*
  // Each call to addPattern() automatically creates a new 'layer' on which that pattern is rendered which we can use to combine, blend and time independently

  // Layer 0 - blue bars, 9 pixels width
  quickPatterns.addPattern(new qpBouncingBars(9))
    .singleColor(CRGB::Blue);

  // Layer 1 - short red bars, 5 pixels
  quickPatterns.addPattern(new qpBouncingBars(5))
    .singleColor(CRGB::Red);
  quickPatterns.sameLayer().setLayerBrush(ADD); //when passing over underlying pixels, add their light together

  // Layer 2 - slow yellow bars, 5 pixels
  quickPatterns.addPattern(new qpBouncingBars(5))
    .singleColor(CRGB::Yellow)
    .drawEveryNTicks(3); //move at a slower speed
  quickPatterns.sameLayer().setLayerBrush(COMBINE);

  */

  quickPatterns.addPattern(new qpMovingGradient(CRGBPalette16(CRGB::Blue, CRGB::Green)))
    .drawEveryNTicks(20);

  quickPatterns.addPattern(new qpBouncingPulse(10))
      .singleColor(CRGB::White);
  quickPatterns.sameLayer().setLayerBrush(SUBTRACT);

}


void loop()
{



  quickPatterns.draw();
  FastLED.show();

  EVERY_N_SECONDS(30) {
    quickPatterns.nextScene();
  }

}
