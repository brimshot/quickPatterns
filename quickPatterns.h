#ifndef QUICK_PATTERNS_H
#define QUICK_PATTERNS_H

#define DIR_FORWARD 1
#define DIR_REVERSE -1

#include <qpLinkedList.h>
#include <qpPattern.h>
#include <qpLayer.h>
#include <qpScene.h>
#include <qpLightStrand.h>
#include <qpColor.h>

class quickPatterns {

  private:

    short tickLengthInMillis = 25;
    uint32_t nextTickMillis = 0;
    uint32_t currentTick = 0;
    uint32_t previousTick = 0;

    qpLightStrand *lightStrand;

    qpLinkedList <qpScene> scenes;

    int sceneIndex = 0;
    qpScene *currentScene = NULL;

    qpScene *lastReferencedScene;

  public:

    quickPatterns(CRGB *leds, int numLeds);

    // Rendering

    void draw();

    void setTickMillis(int tickLengthMillis) {
      this->tickLengthInMillis = tickLengthMillis;
    }

    // Access

    qpPattern &pattern(int patternIndex); //returns specified patter on layer 0
    qpPattern &addPattern(qpPattern *pattern); //creates a new layer and adds passed pattern as pattern 0
    qpPattern &samePattern() { return this->sameScene().sameLayer().samePattern(); }

    qpLayer &layer(int layerIndex);
    qpLayer &sameLayer() { return this->sameScene().sameLayer(); }

    qpScene &scene(int sceneIndex);
    qpScene &newScene();
    qpScene &sameScene() { return *this->lastReferencedScene; }

    qpPattern &operator()(int layerIndex); //returns pattern 0 from the specified layer in scene 0
    qpPattern &operator()(int sceneIndex, int layerIndex); //returns pattern 0 from the specified layer in the specified scene
    qpPattern &operator()(int sceneIndex, int layerIndex, int patternIndex);

    // Scene navigation

    void playScene(int index);
    void nextScene();
    void playRandomScene();

};

#endif
