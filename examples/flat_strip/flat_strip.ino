/*****************************************************

quickPatterns demo reel - patterns that tend to look better on a flat strip, use of clearly defined shapes, state machines

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

#ifdef CORE_TEENSY
#define FASTLED_ALLOW_INTERRUPTS 0
#endif

#include <quickPatterns.h>

#define CHIPSET     WS2812
#define DATA_PIN    8          
#define NUM_LEDS    100
#define BRIGHTNESS  32
#define COLOR_ORDER GRB         //GRB for WS2812, RGB for WS2811

#define TICKLENGTH  25

//Declare master set of leds for FastLED to write to
CRGB leds[NUM_LEDS];

//Declare quickPatterns controller and pass in led data
quickPatterns quickPatterns(leds, NUM_LEDS); //NUM_STRIPS*NUM_LEDS_PER_STRIP);


void setup() {

  delay(3000); // Recovery delay

  // ~ Configure FastLED

  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 450);

  FastLED.clear();


  // ~ Configure quickPatterns

  quickPatterns.setTickMillis(TICKLENGTH);

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

  // "popupDroid" is a little state machine that flashes a few times then moves randomly up and down the strand before disappearing

  //yellow version, 16 pixels, less frequent, on the lowest layer, activates randomly at 200 - 400 tick intervals
  quickPatterns.newScene().addPattern(new popupDroid(16))
    .singleColor(CRGB::Yellow)
    .activatePeriodicallyEveryNTicks(200, 400)
    .stayActiveForNCycles(1);

  //red version, 8 pixels, randomly every 20 to 70 ticks
  quickPatterns.sameScene().addPattern(new popupDroid(8))
    .singleColor(CRGB::Red)
    .activatePeriodicallyEveryNTicks(20, 70)
    .stayActiveForNCycles(1);

  //blue version, 8 pixels, randomly every 20 to 70 ticks
  quickPatterns.sameScene().addPattern(new popupDroid(8))
    .singleColor(CRGB::Blue)
    .activatePeriodicallyEveryNTicks(20, 70)
    .stayActiveForNCycles(1);
  quickPatterns.sameLayer().setLayerBrush(ADD);


  // ~ Scene 2

  // sine wave that moves back and forth along the strip rainbow palette sequentially
  quickPatterns.newScene().addPattern(new qpSinelon(16))
    .chooseColorFromPalette(RainbowColors_p, SEQUENTIAL)
    .changeColorEveryNTicks(2);
  // The sameLayer() function returns a reference to the last layer accessed. We are going to continually fade this layer's light such that old pixels fade away once rendered. Fade values are from 0-255
  quickPatterns.sameLayer().continuallyFadeLayerBy(20);

  // create a color set
  CRGB *pulseColors = new CRGB[3];
  pulseColors[0] = CRGB::Blue;
  pulseColors[1] = CRGB::Yellow;
  pulseColors[2] = CRGB::DeepPink;

  // small line of pixels that bounces back and forth
  quickPatterns.sameScene().addPattern(new qpComet(8))
    .chooseColorFromSet(pulseColors, 3, SEQUENTIAL)
    .changeColorEveryNCycles(4);

  // randomly activated lightning flashes
  quickPatterns.sameScene().addPattern(new qpLightning(10))
      .singleColor(CRGB::White)
      .activatePeriodicallyEveryNTicks(100, 200)
      .stayActiveForNCycles(2, 4);


  // ~ Scene 3 - demonstrates the use of the SUBTRACT brush to animate with negative space

  quickPatterns.newScene().addPattern(new qpTheaterChase())
    .drawEveryNTicks(3)
    .chooseColorFromPalette(RainbowColors_p, SEQUENTIAL)
    .changeColorEveryNTicks(6);

  quickPatterns.sameScene().addPattern(new qpBouncingBars(8))
    .singleColor(CRGB::White);
  quickPatterns.sameLayer().setLayerBrush(SUBTRACT); //subtracting white pixels turns off whatever is below


  // ~ Scene 4 - demonstrates the MASK brush

  // rainbow gradient base layer
  quickPatterns.newScene().addPattern(new qpPaletteGradient())
    .usePalette(RainbowColors_p);

  // soft white confetti. Using the ADD brush to add this layers light to what's below which gives a popping sparkle effect and will leave sparkles as the mask fades
  quickPatterns.sameScene().addPattern(new qpConfetti(30))
    .singleColor(CRGB::White)
    .drawEveryNTicks(4);
  quickPatterns.sameLayer().continuallyFadeLayerBy(30).setLayerBrush(ADD);

  // feather mask periodically illuminates the strand in sections
  quickPatterns.sameScene().addPattern(new qpFeathers(10))
    .singleColor(CRGB::White)
    .drawEveryNTicks(2)
    .activatePeriodicallyEveryNTicks(50, 200)
    .stayActiveForNCycles(1);
  quickPatterns.sameLayer().setLayerBrush(MASK).continuallyFadeLayerBy(10);

  // small pink runner on top
  quickPatterns.sameScene().addPattern(new qpComet(5, true))
    .singleColor(CRGB::DeepPink);

}


void loop()
{

  // Refresh lights only when new frame data available, prevents issues with data timing on fast processors
  if(quickPatterns.draw())
      FastLED.show();

  EVERY_N_SECONDS(30) {
    quickPatterns.nextScene();
  }

}
