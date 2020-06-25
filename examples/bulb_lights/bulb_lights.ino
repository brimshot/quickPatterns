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

These are examples that tend to look better on bulb style lights, making use of wave animations
**************/

#include <FastLED.h>
#include <quickPatterns.h>
#include <qpSamplePatterns.h>

#define CHIPSET     WS2811
#define DATA_PIN    8
#define NUM_LEDS    100
#define BRIGHTNESS  32
#define COLOR_ORDER RGB         //GRB for WS2812, RGB for WS2811

#define TICKLENGTH  25

//Declare master set of leds for FastLED to write to
CRGB leds[NUM_LEDS];

quickPatterns quickPatterns(leds, NUM_LEDS); //NUM_STRIPS*NUM_LEDS_PER_STRIP);
void setup() {

  delay(3000); // Recovery delay

  // ~ Configure FastLED

  FastLED.addLeds<CHIPSET, 8, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);


  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 450);

  FastLED.clear();


  // ~ Configure quickPatterns

  // Due to a potential latching issue when writing data, we use a hard delay for timing with the ESP8266
  #ifdef ESP8266
  quickPatterns.setTickMillis(0);
  #endif

  #ifndef ESP8266
  quickPatterns.setTickMillis(TICKLENGTH);
  #endif

  // ~ Scene 0 - demonstrates simultaneous patterns, timed activation and two options for dynamically changing colors

  // Each call to addPattern() automatically creates a new 'layer' on which that pattern is rendered which we can use to combine, blend and time independently

  // sine wave that moves back and forth along the strip rainbow palette sequentially
  quickPatterns.addPattern(new qpSinelon(16))
    .chooseColorSequentiallyFromPalette(RainbowColors_p)
    .changeColorEveryNTicks(2);
  // The sameLayer() function returns a reference to the last layer accessed. We are going to continually fade this layer's light such that old pixels fade away once rendered. Fade values are from 0-255
  quickPatterns.sameLayer().continuallyFadeLayerBy(20);

  // create a color set
  CRGB *pulseColors = new CRGB[3];
  pulseColors[0] = CRGB::Blue;
  pulseColors[1] = CRGB::Yellow;
  pulseColors[2] = CRGB::Pink;

  // small line of pixels that bounces back and forth
  quickPatterns.addPattern(new qpBouncingPulse(8))
    .chooseColorSequentiallyFromSet(pulseColors, 3)
    .changeColorEveryNCycles(4);

  // randomly activated lightning flashes
  quickPatterns.addPattern(new qpFlashRandomSection(10))
      .singleColor(CRGB::White)
      .activatePeriodicallyEveryNTicks(100, 200)
      .stayActiveForNCycles(2, 4);


  // ~ Scene 1 - demonstrates timed activation and duration

  // juggle effect base layer
  quickPatterns.newScene().addPattern(new qpJuggle())
    .chooseColorSequentiallyFromPalette(ForestColors_p)
    .changeColorEveryNTicks(10)
    .drawEveryNTicks(4); //slowing down our updates gives a wider spread to the juggle effect
  quickPatterns.sameLayer().continuallyFadeLayerBy(20);

  // sine wave activated randomly at intervals between 80 and 150 ticks. once active, stays active for 50 ticks. changes from blue to purple
  quickPatterns.sameScene().addPattern(new qpBouncingPulse(20))
    .chooseColorSequentiallyFromPalette(CRGBPalette16(CRGB::Blue, CRGB::Purple))
    .changeColorEveryNTicks(1)
    .activatePeriodicallyEveryNTicks(80, 150)
    .stayActiveForNCycles(4);
  quickPatterns.sameLayer().continuallyFadeLayerBy(20).setLayerBrush(ADD);

  // periodic yellow feathers
  quickPatterns.sameScene().addPattern(new qpRandomBar(10))
    .singleColor(CRGB::Yellow)
    .activatePeriodicallyEveryNTicks(160)
    .drawEveryNTicks(10)
    .stayActiveForNFrames(5);
  quickPatterns.sameLayer().continuallyFadeLayerBy(20).setLayerBrush(COMBINE);


  // ~ Scene 2 - demonstrates the MASK brush

  // rainbow gradient base layer
  quickPatterns.newScene().addPattern(new qpMovingGradient(RainbowColors_p));

  // soft white confetti. Using the ADD brush to add this layers light to what's below which gives a popping sparkle effect and will leave sparkles as the mask fades
  quickPatterns.sameScene().addPattern(new qpConfetti())
    .singleColor(CRGB::White)
    .drawEveryNTicks(4);
  quickPatterns.sameLayer().continuallyFadeLayerBy(30).setLayerBrush(ADD);

  // feather mask periodically illuminates the strand in sections
  quickPatterns.sameScene().addPattern(new qpFeathers(10))
    .singleColor(CRGB::White)
    .drawEveryNTicks(3)
    .activatePeriodicallyEveryNTicks(50, 200)
    .stayActiveForNCycles(1);
  quickPatterns.sameLayer().setLayerBrush(MASK).continuallyFadeLayerBy(10);

  // small pink runner on top
  quickPatterns.sameScene().addPattern(new qpBouncingPulse(5))
    .singleColor(CRGB::HotPink);


  // ~ Scene 3 - demonstrates periodic pattern activation

  //stop and go rainbow theater chase
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

  #ifdef ESP8266
  FastLED.delay(TICKLENGTH);
  #endif

  EVERY_N_SECONDS(30) {
    quickPatterns.nextScene();
  }

}
