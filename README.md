# quickPatterns

A FastLED based patterns manager for addressable LEDs (WS2812B, NeoPixels, etc.) that allows multiple animations to run simultaneously on the same strand of lights with configurable colors and timings.

[Example sketch](#example-sketch)

[Adding patterns](#adding-patterns)

[Using layers](#using-layers)

[Layer brushes](#layer-brushes)

[Configuring pattern colors](#configuring-pattern-colors)

[Periodic color changes](#periodic-color-changes)

[Periodic pattern activation](#periodic-pattern-activation)

[Creating scenes](#creating-scenes)

[Additional options](#additional-options)

[Writing new patterns](#writing-new-patterns)

[Sample patterns](#sample-patterns)


## Example sketch
A simple example that can be run right away

```
#include <quickPatterns.h>

#define CHIPSET     WS2812B
#define DATA_PIN    8
#define NUM_LEDS    100
#define BRIGHTNESS  64
#define COLOR_ORDER GRB         //GRB for WS2812, RGB for WS2811
#define TICKLENGTH  25

CRGB leds[NUM_LEDS];

//declare quickPatterns controller and pass in main led array
quickPatterns quickPatterns(leds, 100);

void setup() {

  delay(3000); // Recovery delay

  randomSeed(analogRead(1));

  // ~ Configure FastLED

  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
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

  //cylon of 8 pixels that cycles through the rainbow
  quickPatterns.addPattern(new qpBouncingBars(8))
    .chooseColorFromPalette(RainbowColors_p, SEQUENTIAL)
    .changeColorEveryNTicks(2);

  //Periodically toss in some lightning flashes at random places along the strand. Flash for 10x
  quickPatterns.addPattern(new qpLightning(10))
      .singleColor(CRGB::White)
      .activatePeriodicallyEveryNTicks(100, 200)
      .stayActiveForNCycles(2, 4);
}

void loop()
{
  quickPatterns.draw();
  FastLED.show();

  #ifdef ESP8266
  FastLED.delay(TICKLENGTH);
  #endif
}
```



## Adding patterns

### Basic setup

quickPatterns uses the [FastLED](link) library which must be installed on your system first, and this document assumes you are familiar with the basics of using FastLED

At the top of your file declare an array of LEDs as you would in any FastLED based sketch

```
CRGB leds[NUM_LEDS];
```

Next, declare the quickPatterns controller, passing in your LEDs and length
```
quickPatterns quickPatterns(leds, NUM_LEDS);
```

In your *setup()* function, configure FastLED as normal, then add patterns to the quickPatterns controller as will be described below.
```
void setup() {

  FastLED.addLeds<...>

  ...  // configure FastLED


  // configure quickPatterns (see below)

  ...

}
```

Finally, in your *loop()* function, call the *draw()* method of the quickPatterns controller and then FastLED.show()
```
void loop()
{
  quickPatterns.draw();
  FastLED.show();
}
```

### Adding the patterns

Once the quickPatterns controller has been instantiated you can use the addPattern() method and pass in a new instance of any class that inherits from the qpPattern class ([write your own](#writing-new-patterns) or use one of the [included patterns](#included-patterns))

For example, a simple pulse of eight pixels that moves back and forth a string of lights:
```
quickPatterns.addPattern(new qpComet(8))
  .singleColor(CRGB::Red);
```
addPattern() returns a reference to the pattern object passed as a parameter which can be used to continue to chain configuration methods.
In this case we are chaining the singleColor() method which sets our pulse pattern to a constant red.


### Setting pattern speeds

Updates and changes in quickPatterns operate on a tick system. Each tick, active patterns are drawn and any pending changes processed.

The default tick length is 25 milliseconds, but the length of a tick is configurable on the controller. To change the master tick length, use the setTickMillis() method

```
quickPatterns.setTickMillis(50); //sets tick length to 50 milliseconds
```

By default each pattern will update once per tick. This can be changed with the pattern configuration method drawEveryNTicks(), which allows for lowering a patterns render frequency.

```
quickPatterns.addPattern(new qpComet(8))
  .singleColor(CRGB::Red);
  .drawEveryNTicks(2);
```
By adding drawEveryNTicks(2) to the configuration chain, our pulse pattern will now animate at half speed.

Ticks are also used to calculate pattern activation and duration timings (see [periodic pattern activation](#periodic-pattern-activation)).


## Using layers

Patterns are pre-rendered on separate arrays of leds in memory which are then combined to make the final image displayed i.e. 'layers'.

The default behavior is that filled pixels in upper layers cover those in lower layers, for example:

Layer one - red every 4th LED:
```
----------------------
- R - - - R - - - R -
----------------------
```
Layer two - green every 3rd LED:
```
----------------------
G - - G - - G - - G -
----------------------
```
Final result that gets displayed on the LED strand:
```
----------------------
G R - G - R G - - G -
----------------------
```
Layers are applied to the final output in ascending order. Since layer two (green pixels) is on 'top' of layer one, the value at any collision will be the value in the upper layer.

To configure how layers interact with each other when combined see [layer brushes](#layer-brushes).

By default each call to addPattern() creates a new layer in memory for that pattern to run on. However, multiple patterns can be added on to a single layer (pixel data will be written in the order patterns are rendered).

To add a pattern to a specific layer, insert the layer() method into the access chain
```
quickPatterns.layer(0).addPattern(new qpComet(8)); //add a pattern directly to layer 0
quickPatterns.layer(1).addPattern(new qpConfetti(60)); //add a pattern directly to layer 1
```

If the *layer()* method is called for an index that does yet exist, a new layer will be created automatically and a reference to the new layer returned.

Once created, patterns can be re-accessed directly via *layer().pattern()* i.e.
```
quickPatterns.layer(0).pattern(0) //provides access to pattern 0 on layer 0
quickPatterns.layer(0).pattern(1) //provides access to pattern 1 on layer 0
```

## Layer brushes

Layer brushes give control over how each layer interacts with those below it and whether pixels in the same location on two separate layers overwrite, combine or blend.

Setting a layer brush is via the *setLayerBrush()* method which accepts one of the enumerated brush types
```
quickPatterns.layer(1).setLayerBrush(ADD); //layer 1 will now add it's light to below layers instead of replacing underlying pixels
```

There are 6 different brush types which affect the application of each layer to those below:

### OVERLAY
This is the default brush if none set - any pixels in this layer that are set to black (CRGB::Black) are not written, allowing whatever is below to show through, while any pixels that have a color value replace those from layers below

### ADD
The light value of pixels in this layer will be added to the light values of pixels below

### SUBTRACT
The light value of pixels in this layer are subtracted from those below. Using this brush, we can turn 'off' pixels on underlying layers and render patterns using negative space.
Set a pattern color to full white (CRGB::White) and the layer brush to SUBTRACT and any pixels the pattern fills in will be removed from below layers.

### OVERWRITE
Completely overwrite below layers with this layers pixels, including pixels that are 'off' (CRGB::Black).

### COMBINE
Combine the values of this layers pixels with those below.

### MASK
The mask brush subtracts the inverse of the light values in the current layer from those below.
When a pattern with black and white pixels used is as a mask, any white pixels in the mask will allow those from below to show through, while any black pixels will eliminate what's underneath. Use this brush to leave 'windows' of visible space from patterns that are being rendered on the below layers.


## Configuring pattern colors

Patterns are drawn using either a palette of colors or a single color per frame.

### Palettes

For patterns that use a complete palette to render frames, the *usePalette()* method sets the palette to be used.

```
quickPatterns.newScene().addPattern(new qpPaletteWave(5))
  .usePalette(RainbowColors_p);
```

In the *draw()* step of any pattern, the function *_getPalette()* returns the configured palette for use in rendering, see [writing new patterns](#writing-new-patterns) for more details.

### One color per frame

For patterns that draw one color per frame, the color drawn can be chosen one of three ways: retrieved from a color palette,  retrieved from a set of colors or set as a constant.

The function *_getColor()* returns the current color value at the time of rendering, which can be configured

#### Constant color

To show a pattern using a single color, use the *singleColor()* method in the configuration chain
```
quickPatterns.addPattern(new qpComet(8))
  .singleColor(CRGB::Yellow); // Will always be yellow
```

#### Color from a palette

Frame colors can chosen from palettes, either sequentially or randomly.

To configure a pattern to cycle through palette colors sequentially, use the *chooseColorFromPalette()* method. Pass in any CRGBPalette16 and the constnat *SEQUENTIAL* as the second argument.
```
quickPatterns.addPattern(new qpBouncingBars(10))
  .chooseColorFromPalette(RainbowColors_p, SEQUENTIAL);
```

Each time the pattern is rendered, the next color from the palette will automatically be loaded into the value returned by *_getColor()*

Use the *setPaletteStep()* method to set the number of hues to jump when moving through the palette.

```
quickPatterns.addPattern(new qpComet(8))
  .chooseColorFromPalette(RainbowColors_p, SEQUENTIAL)
  .setPaletteStep(15); //jump 15 hues each color change
```
If none provided, hue step will default to 3 which, giving a smooth gradient across the palette.

For colors to be chosen randomly from the supplied palette, pass *RANDOM* as the second parameter to *chooseColorFromPalette*
```
quickPatterns.addPattern(new qpComet(8))
  .chooseColorFromPalette(RainbowColors_p, RANDOM);
```



#### Colors using sets

Frame colors can also be configured using sets of concrete colors.

To use a color set, first create an array of CRGB objects
```
CRGB[3] myColorSet;
myColorSet[0] = CRGB::Red;
myColorSet[1] = CRGB::Yellow;
myColorSet[2] = CRGB::Pink;
```
You can now configure the pattern to use colors from the set using *chooseColorFromSet*
```
quickPatterns.addPattern(new qpComet(8))
  .chooseColorFromSet(myColorSet, 3, SEQUENTIAL);
```

The first parameter is a pointer to the array of colors, the second is the number of colors in the set, and the third is the mode constant.

To have the frame color chosen from the color set randomly, simply change the mode constant to *RANDOM*.
```
quickPatterns.addPattern(new qpComet(8))
  .chooseColorFromSet(myColorSet, 3, RANDOM);
```

## Periodic color changes

By default, frame colors do not change. A pattern will only use the first hue from the supplied palette or set unless change timing is configured.

When to change colors can be set relative to one of 4 periods - ticks, updates, activations or cycles.

To change a color based on ticks, use the *changeColorEveryNTicks()* method
```
quickPatterns.addPattern(new qpComet(8))
  .chooseColorFromPalette(RainbowColors_p, SEQUENTIAL)
  .changeColorEveryNTicks(4); //pattern color changes to the next hue in the palette every 4 ticks
```

A *frame* is every time the pattern is drawn. Timing via frame can be used to make color changes keyed to pattern updates, independent of overall pattern speed. To link color changes to frames use the *changeColorEveryNFrames()* method
```
quickPatterns.addPattern(new qpComet(8))
  .drawEveryNTicks(4) //only render this pattern every 4th tick
  .chooseColorFromPalette(RainbowColors_p, SEQUENTIAL)
  .changeColorEveryNFrames(2); //change color every second time this pattern is rendered
```

An *activation* is each time the pattern is activated (see [peridodic pattern activation](#periodic-pattern-activation)). To link color changes use the *changeColorEveryNActivations()* method
```
quickPatterns.addPattern(new qpFlashRandomSection(8))
  .activatePeriodicallyEveryNTicks(100)
  .stayActiveForNTicks(100)
  .chooseColorFromPalette(RainbowColors_p, SEQUENTIAL)
  .changeColorEveryNActivations(2); //change color every second time this pattern is activated
```

A *cycle* is an internal counter tracked by the pattern that can be used to mark a completion point in the animation.
To time color changes using cycles, use the *changeColorEveryNCycles()* method
```
quickPatterns.addPattern(new qpComet(8))
  .chooseColorFromPalette(RainbowColors_p, SEQUENTIAL)
  .changeColorEveryNCycles(3); //color changes every third time the pulse traverses the strand (every third cycle)
```

### Random length intervals

Each method to configure color timing can accept a second optional parameter which, when included, sets color changes to trigger at intervals of random lengths bounded by the first and second parameters

```
quickPatterns.addPattern(new qpComet(8))
  .chooseColorFromPalette(RainbowColors_p, SEQUENTIAL)
  .changeColorEveryNTicks(4, 16); //pattern color will change at intervals of random lengths between 4 and 16 ticks


.changeColorEveryNFrames(5, 10); //color change occurs every 5 to 8 pattern updates randomly

.changeColorEveryNActivations(3, 6); //color change occurs every 3 to 6 activations randomly

.changeColorEveryNCycles(3, 8); //color change occurs every 3 to 8 cycles randomly

```

## Periodic pattern activation

By default all patterns begin in an active state and are rendered. Patterns can also be configured to turn on and off periodically at intervals.

The active and inactive intervals are measured in ticks. Configure activation via the *activatePeriodicallyEveryNTicks()* method
```
quickPatterns.addPattern(new qpComet(8))
  .activatePeriodicallyEveryNTicks(300); //pattern will wait 300 ticks between activations
```

The above pattern will activate after 300 ticks have passed and once the active period has completed and the pattern has deactivated, it will wait another 300 ticks before activating again.

The activatePeriodicallyEveryNTicks() method can optionally take a second parameter which, when supplied, sets the ticks between each activation to a random number bounded by the two parameters

```
quickPatterns.addPattern(new qpComet(8))
  .activatePeriodicallyEveryNTicks(300, 500); //time between activations will be random, between 300 to 500 ticks
```

### Active period duration

Once active, the period for which a pattern stays active can be set relative to one of three periods: ticks, updates or cycles

To set the active state duration in ticks, use the *stayActiveForNTicks()* method
```
quickPatterns.addPattern(new qpComet(8))
  .activatePeriodicallyEveryNTicks(300)
  .stayActiveForNTicks(200); //once activated, pattern will stay active for 200 ticks before deactivating
```

A *frame* is every time the pattern is drawn. To set the activate state duration in frames, use the *stayActiveForNFrames()* method
```
quickPatterns.addPattern(new qpComet(8))
  .drawEveryNTicks(2) //run pattern at half speed
  .activatePeriodicallyEveryNTicks(300)
  .stayActiveForNFrames(50); //once activated, pattern will render 50 times before deactivating
```

A *cycle* is an internal counter tracked by the pattern that can be used to mark a completion point in the pattern animation.  To set the activate state duration in  cycles use the *stayActiveForNCycles()* method
```
quickPatterns.addPattern(new qpComet(8))
  .activatePeriodicallyEveryNTicks(300)
  .stayActiveForNCycles(4); //once activated, pattern will stay active for 4 complete pulses
```

### Random active state length

Each method to configure active state duration can accept a second number as an optional parameter. If the second parameter is included, each active period will be a random duration in the chosen units, bounded by the two parameters

```
quickPatterns.addPattern(new qpComet(8))
  .activatePeriodicallyEveryNTicks(300)
  .stayActiveForNTicks(200, 400); //once activated, pattern will stay active for a random period between 200 and 400 ticks

  .stayActiveForNFrames(25, 50); //once activated, pattern will stay active for a random number of frames, between 25 and 50

  .stayActiveForNCycles(4, 10); //once activated, pattern will stay active for random number of cycles between 4 and 10
```


## Creating scenes

Scenes are collections of layers that can be referenced as a unit. By default, calls to *addPattern()* and *layer()*, as used in our first examples,  reference scene 0 and scene 0 will be rendered when *quickPatterns.draw()* is called unless otherwise specified.

Creating multiple scenes, each with their own layers, allows us to switch between various combinations of patterns on the same strand of lights as desired.

To create a new scene simply use the *newScene()* method, which returns a reference to the scene created, like so

```
//Creates a new scene (index will be 1) and adds a pattern
quickPatterns.newScene().addPattern(new qpComet(8))
  .singleColor(CRGB::Red);
```

Since our initial scene is always scene 0, the next scene created gets assigned index 1. You can now access the new scene directly using the *scene()* method

```
//Access scene 1 directly
quickPatterns.scene(1).addPattern(new qpComet(8))
  .singleColor(CRGB::Red);
```

#### Note
If no scene yet exists at the index passed to the *scene()* method, a new scene will be created automatically and a reference to the newly created scene returned.

If you want to reference a specific layer on a specific scene, simply insert the *layer()* method into the access chain after *scene()*
```
//Add a pattern directly to scene 2, layer 1
quickPatterns.scene(2).layer(1).addPattern(new qpComet(8))
  .singleColor(CRGB::Red);
```

### Changing scenes

By default, scene 0 is the first to display. To change the scene being displayed, you can jump directly to a scene via index, cycle through all scenes sequentially or choose randomly
```
quickPatterns.playScene(1); //change to scene 1 right away
quickPatterns.playScene(2); //change to scene 2 right away
```
The *nextScene()* method automatically changes to the next scene stored (usually called periodically by your main loop). This loops back to 0 when all scenes have been played.
```
quickPatterns.nextScene(); //sets current scene to the next available, loops back to 0 once all scenes shown
```
To play a random scene
```
quickPatterns.playRandomScene();
```


## Additional options

### Layer fading

Frequently, a pattern benefits from having older pixels fade out slowly while new pixels are written, such that pixels that aren't refreshed eventually disappear.
Layers can be configured to fade a set amount each tick before pattern rendering by using the *continuallyFadeLayerBy()* method
```
quickPatterns.layer(1).continuallyFadeLayerBy(20); //layer 1 will fade to black by 20/255 once per tick
```

### Layer persistence

By default, to facilitate fading and blending effects, layers stay visible and the last written information continues to be rendered even when the layer contains no active patterns.
To stop a layer from being rendered when none of it's patterns are active, use *hideWhenNoActivePatterns()*
```
quickPatterns.layer(1).hideWhenNoActivePatterns(); //layer 1 will no longer be rendered if none of it's patterns are active
```

### sameLayer(), sameScene(), samePattern()

Use these methods to gain access to the last referenced scene, layer or pattern.

For example, use the sameScene() method when you are adding multiple patterns to a new scene remove the need to track the scenes index
```
quickPatterns.newScene().addPattern(new qpComet(8)) //creates a new scene and adds the pattern
  .singleColor(CRGB::Red);

quickPatterns.sameScene().addPattern(new qpComet(8)) //creates layer 1, pattern 0 on the same scene that was just created
  .singleColor(CRGB::Blue);

quickPatterns.sameScene().addPattern(new qpSinelon(13)) //creates layer 2, pattern 0 on the same scene
  .singleColor(CRGB::Green);

//use the sameLayer() method to continue to configure the last accessed layer
quickPatterns.sameLayer().setLayerBrush(ADD); //sets the brush for our new scene, layer 2 to ADD
quickPatterns.sameLayer().continuallyFadeBy(20); //adds fading to our new scene, layer 2
```

### Quick access operators
For quick access to the first pattern (pattern 0) on a given layer simply call
```
quickPatterns(1)
```
is the same as
```
quickPatterns.scene(0).layer(1).pattern(0)
```
i.e.
```
quickPatterns.scene(0).layer(1).addPattern(new qpComet(8));

quickPatterns(1).singleColor(CRGB::Red); //sets pattern 0 on layer 1 to red
```

#### Scene, layer

Quick access to the first pattern (pattern 0) a specific scene / layer:
```
quickPatterns(1, 2)
```
is the same as
```
quickPatterns.scene(1).layer(2).pattern(0)
```

#### Scene, layer, pattern

Quick access to a specific scene, layer and pattern:
```
quickPatterns(1, 0, 2)
```
is the same as
```
quickPattern.scene(1).layer(0).pattern(2)
```


## Writing new patterns

To write a custom pattern simply create a class that inherits from *qpPattern* and write a *draw()* method for the class

```
//inherit from base class qpPattern
class myCustomPattern : public qpPattern {

  int pos = 0;

  //draw() is called whenever the pattern is rendered, default is once per tick
  void draw() {

    //clears all pixels on the layer
    _clearLeds();

    //move a single pixel along the strand step by step
    _targetLeds[pos++] = _getColor();

    //start over at first led once we hit last
    if(pos >= _numLeds)
      pos = 0;
  }

}
```

The class member *_targetLeds* points to the array of leds for the layer the pattern is being rendered on and should be used for writing light data

Once written, include your pattern file in your sketch and you can now add your pattern to be displayed like so
```
quickPatterns.addPattern(new myCustomPattern())
  .singleColor(CRGB::Blue)
  ...
```

### qpPattern members and methods

The following class members and methods are available inside pattern classes for use when creating custom patterns:

**_targetLeds**

These are the leds for the layer the pattern is being rendered on. This is what to write light data to. Use as you would any FastLED CRGB array

**_numLeds**

Number of leds in the light strand

**_getColor()**

Returns a CRGB object with the current frame color as per this patterns [color configuration](#configuring-pattern-colors)

**_getPalette()**

Returns the palette that was configured on this pattern via the *usePalette()* method

**_clearLeds()**

Clears the leds on this pattern's layer - be aware that this will clear data written by other patterns on the same layer as well

**_countCycle()**

Count one *cycle* - used for timing activations and color changes via cycles

**_inBounds(int index)**

Returns true / false if *index* is between 0 and the number of leds in this pattern's layer


### Initialize

There is also an optional method *initialize()* that can be overloaded on custom patterns, which is called when the pattern is first linked to the layer it will be rendered on

#### Note:
The number of leds will not be known to the patterns at the time of class creation, therefore any initial calculations that require knowing the total number of leds must be placed in the *initialize()* method i.e.

```
//constructor - *wrong* way
myCustomPattern() {
  this->halfwayPoint = _numLeds / 2; //will NOT work, since the number of leds is unknown in pattern constructor
}

//overloaded initialize() method - correct way
void initialize() {
  this->halfwayPoint = _numLeds / 2; //correct way, number of leds is known when initialize() is called
}
```


## Sample patterns

The following patterns are included with the library. 

**qpComet**
```
qpComet(int size, bool bounce = true, int direction = DIR_FORWARD)
```
Section of lights of length *size* that move along the light strip. If the *bounce* parameter is set to true, the comet will reverse direction and return after hitting each end of the light strip. If the *direction* parameter is set to DIR_REVERSE, the initial starting point will be from the end of the light strip.

**qpBouncingBars**
```
qpBouncingBars(int size)
```
Two sections of length *size* that start at opposite ends of the light strip, meet in the middle, then return.

**qpConfetti**
```
qpConfetti(int spread)
```
Illuminates a random number of pixels between 0 and the total available each frame. Increase spread or decrease draw frequency to increase spacing.

**qpElectricy**
```
qpElectricity(int speed)
```
Trails of lights that move up and down the light strand. Must add a fade to pattern's layer when using to avoid filling the strand solid.

**qpFeathers**
```
qpFeathers(int size, int direction = DIR_FORWARD)
```
Turns on the lights of the strand in sections of size *size* sequentially.

**qpJuggle**
```
qpJuggle()
```
Lights that weave in and out sync, from FastLED demo reel.

**qpLightning**
```
qpLightning(int sectionSize, byte numFlashes = 8)
```
Flashes a random section of lights of length *sectionSize* on and off *numFlashes* times.

**qpPaletteBreathe.h**
```
qpPaletteBreathe(int speed)
```
Fills strand with a moving gradient using palette set via *usePalette()* that fades in and out as per *speed* parameter

**qpPaletteDissolve.h**
```
qpPaletteBreathe(int speed)
```
Waves of palette colors that move in and out as per *speed* parameter

**qpPaletteGradient**
```
qpPaletteGradient(int deltaX = 3, int deltaY = 1)
```
Fills the strand with a gradient using the palette set via *usePalette()*. deltaX is the change between hues each pixel. deltaY is the change in the initial hue each frame.

**qpPaletteTwinkle**
```
qpPaletteTwinkle(int speed)
```
Twinkling palette colors (set via *usePalette()*) that move and blink as per *speed* parameter

**qpPaletteWave**
```
qpPaletteTwinkle(int speed)
```
Waves of palette colors (set via *usePalette()*) that move as per *speed* parameter

**qpSinelon**
```
qpSinelon(int speed)
```
Pixel weaving back and forth across the strand. Must add a fade to pattern's layer.

**qpSparkles**
```
qpSparkles(int speed)
```
Randomly illuminates pixels up and down the strand

**qpTheaterChase**
```
qpTheaterChase()
```
Classic theater chase pattern that moves lights in threes.

**qpWanderingLine**
```
qpWanderingLine(int size)
```
Section of lights of length *size* that move back and forth randomly along the light strip.

---
Copyright (c) 2020 Chris Brim

Tested platforms: ESP8266, ESP32, Teensy 3.2, Arduino Mega
