#ifndef QUICK_PATTERNS_H
#define QUICK_PATTERNS_H

#ifndef QP_INCLUDE_PATTERNS
#define QP_INCLUDE_PATTERNS 1
#endif

#ifndef QP_INCLUDE_EFFECTS
#define QP_INCLUDE_EFFECTS 1
#endif

#include <FastLED.h>
#include "qpEnums.h"
#include "qpLinkedList.h"
#include "qpScene.h"
#include "qpLightStrand.h"

// Patterns
#if QP_INCLUDE_PATTERNS == 1
#include "patterns/AllPatterns.h"
#endif

// Effects
#if QP_INCLUDE_EFFECTS == 1
#include "layer_effects/AllEffects.h"
#endif

/**
 *TODO:
 * - Replace byte with uint8_t
 * - Split strips
 * - nscale8 fade
 * - Reverse effect
 * - Shapes -> array of pixel positions
 * - Events
 * - Shows
 * - Transitions 
 * - More Teensy testing
 * - Sound reactive plugins + other plugins
 */

class quickPatterns {

  private:

    short tickLengthInMillis = 25;
    uint32_t nextTickMillis = 0;

    qpLightStrand *lightStrand;

    qpLinkedList <qpScene> scenes;

    int sceneIndex = 0;
    qpScene *currentScene = NULL;

    qpScene *lastReferencedScene;

  public:

    quickPatterns(CRGB *leds, int numLeds);
    ~quickPatterns();
    
    // ~ Rendering

    bool draw();

    void setTickMillis(int tickLengthMillis) { this->tickLengthInMillis = tickLengthMillis; }

    void show() {
      if(draw()) {
        FastLED.show();
      }
    }

    // ~ Config

    qpPattern *addPattern(qpPattern *pattern); //creates a new layer and adds passed pattern as pattern 0


    // ~ Access

    // Patterns
    qpPattern &pattern(byte patternIndex); //returns specified pattern on layer 0

    // Layers
    qpLayer &layer(byte layerIndex);

    // Scenes
    qpScene &newScene();
    qpScene &scene(byte sceneIndex);

    // Prev reference
    qpScene &sameScene() { return *this->lastReferencedScene; }
    qpLayer &sameLayer() { return this->sameScene().sameLayer(); }
    qpPattern &samePattern() { return this->sameScene().sameLayer().samePattern(); }

    // TODO: replace with int to make Vscode play nicer?
    // Quick access operators
    qpPattern &operator()(byte layerIndex); //returns pattern 0 from the specified layer in scene 0
    qpPattern &operator()(byte sceneIndex, byte layerIndex); //returns pattern 0 from the specified layer in the specified scene
    qpPattern &operator()(byte sceneIndex, byte layerIndex, byte patternIndex);


    // ~ Scene navigation
    void playScene(byte index);
    void nextScene();
    void playRandomScene();

};

#endif