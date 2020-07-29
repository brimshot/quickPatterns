/*****************************************************

quickPatterns demo reel - patterns that tend to look better on bulb style lights, patterns that use waves with random highlights and sparkles

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

#define CHIPSET     WS2811
#define PIN_A       8
#define NUM_LEDS    100
#define BRIGHTNESS  32
#define COLOR_ORDER RGB         //GRB for WS2812, RGB for WS2811
#define TICKLENGTH  20

CRGB leds[NUM_LEDS];

quickPatterns quickPatterns(leds, NUM_LEDS);

void setup() {

  delay(3000); // Recovery delay

  randomSeed(analogRead(1));

  // ~ Configure FastLED

  FastLED.addLeds<CHIPSET, PIN_A, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 450);

  FastLED.clear();

  // ~ Configure quickPatterns

  quickPatterns.setTickMillis(TICKLENGTH);

  // Due to a potential latching issue when writing data, we use a hard delay for timing with the ESP8266
  #ifdef ESP8266
  quickPatterns.setTickMillis(0);
  #endif

  // ~

  quickPatterns.addPattern(new qpPaletteDissolve(5))
    .usePalette(ForestColors_p);

  quickPatterns.addPattern(new qpElectricity())
    .usePalette(OceanColors_p)
    .activatePeriodicallyEveryNTicks(200, 300)
    .stayActiveForNTicks(80, 140);
  quickPatterns.sameLayer().setLayerBrush(COMBINE).continuallyFadeLayerBy(30);  


 // ~ 

 quickPatterns.newScene().addPattern(new qpPaletteWave(5))
  .usePalette(CRGBPalette16(CRGB::Yellow, CRGB::Orange, CRGB::Goldenrod, CRGB::Red));

  quickPatterns.sameScene().addPattern(new qpSparkles(6))
    .chooseColorFromPalette(CRGBPalette16(CRGB::Red, CRGB(120, 0, 255)), SEQUENTIAL);
  quickPatterns.sameLayer().continuallyFadeLayerBy(20);  


 // ~

  quickPatterns.newScene().addPattern(new qpPaletteWave(5))
    .usePalette(RainbowColors_p);

  quickPatterns.sameScene().addPattern(new qpJuggle())
    .drawEveryNTicks(2)
    .singleColor(CRGB(64, 64, 64))
    .activatePeriodicallyEveryNTicks(30)
    .stayActiveForNTicks(30);
  quickPatterns.sameLayer().setLayerBrush(ADD).continuallyFadeLayerBy(20);  

  quickPatterns.sameScene().addPattern(new qpLightning(8))
    .singleColor(CRGB::White)
    .activatePeriodicallyEveryNTicks(300, 400)
    .stayActiveForNCycles(3, 6);
  quickPatterns.sameLayer().setLayerBrush(SUBTRACT);  


  // ~

  quickPatterns.newScene().addPattern(new qpPaletteTwinkle(6))
    .usePalette(RainbowColors_p);

  quickPatterns.sameScene().addPattern(new qpWanderingLine(30))
    .singleColor(CRGB::White);
  quickPatterns.sameLayer().setLayerBrush(MASK);    
  



}

void loop()
{

  quickPatterns.draw();
  FastLED.show();

  #ifdef ESP8266
  FastLED.delay(25);
  #endif

  EVERY_N_SECONDS(30) {
    quickPatterns.nextScene();
  }

}
