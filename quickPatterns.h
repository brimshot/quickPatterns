#ifndef QUICK_PATTERNS_H
#define QUICK_PATTERNS_H

#ifndef QP_INCLUDE_PATTERNS
#define QP_INCLUDE_PATTERNS 1
#endif

#include <Arduino.h>
#include <qpLinkedList.h>
#include <qpColor.h>
#include <qpPattern.h>
#include <qpLayer.h>
#include <qpScene.h>
#include <qpLightStrand.h>

#if QP_INCLUDE_PATTERNS == 1
#include <qpPatternFiles.h>
#endif

/**
 *TODO:
 * - Shows
 * - Scene framerates
 * - Fix blend / combine brush
 * - Transitions 
 * - Teensy 3.2 not working with single pattern?
 * - Teensy 4.0 support
 * - Linked colors
 * - Linker fixes
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

    // ~ Rendering

    bool draw();

    void setTickMillis(int tickLengthMillis) { this->tickLengthInMillis = tickLengthMillis; }


    // ~ Config

    qpPattern &addPattern(qpPattern *pattern); //creates a new layer and adds passed pattern as pattern 0


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