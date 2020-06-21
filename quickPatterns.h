#ifndef QUICK_PATTERNS_H
#define QUICK_PATTERNS_H

#define DIR_FORWARD 1
#define DIR_REVERSE -1

//TODO: randomness not random!
//TODO: check if include defs work
//TODO: find a way to remove FastLED from this header

//TODO: why so many
#include <qpLinkedList.h>
#include <qpPattern.h>
#include <qpLayer.h>
#include <qpScene.h>
#include <qpLightStrand.h>


class quickPatterns {

  private:

    short tickLengthInMillis = 25;
    uint32_t nextTickMillis = 0;
    uint32_t currentTick = 0;

    qpLightStrand *lightStrand;

    qpLinkedList <qpScene> scenes;

    int sceneIndex = 0;
    qpScene *currentScene = NULL;

    inline void addEntropy();

  public:

    quickPatterns(CRGB *leds, int numLeds);

    // ~ Rendering

    void draw();

    // ~ Timing

    void setTickMillis(int tickLengthMillis) {
      this->tickLengthInMillis = tickLengthMillis;
    }

    int getTicks() {
      return this->currentTick;
    }


    // ~ Pattern access

    qpPattern &addPattern(qpPattern *pattern); //creates a new layer and adds passed pattern as pattern 0

    qpPattern &operator()(int layerIndex); //returns pattern 0 from the specified layer in scene 0
    qpPattern &operator()(int sceneIndex, int layerIndex); //returns pattern 0 from the specified layer in the specified scene
    qpPattern &operator()(int sceneIndex, int layerIndex, int patternIndex);

    // ~ Component access

    qpScene &scene(int sceneIndex);
    qpLayer &layer(int layerIndex);
    qpPattern &pattern(int patternIndex); //returns specified patter on layer 0

//    qpScene &sameScene() { return *this->lastReferencedScene; } //TODO: remove
    qpLayer &sameLayer();
    //going to need samePattern for color access

    // ~ Scene selection
    void playScene(int index);
    void nextScene();
    void playRandomScene();

    // ~ Debug

//    int getMemUse();

};

#endif
