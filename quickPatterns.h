#ifndef QUICK_PATTERNS_H
#define QUICK_PATTERNS_H

#define DIR_FORWARD 1
#define DIR_REVERSE -1

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
    uint32_t previousTick = 0;

    qpLightStrand *lightStrand;

    qpLinkedList <qpScene> scenes;

    int sceneIndex = 0;
    qpScene *currentScene = NULL;

    qpScene *lastReferencedScene;

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

    bool isAtTick(int tick) {
      if((this->currentTick == tick) && (this->currentTick != this->previousTick)) { //this is so true is only returned one time
        this->previousTick = this->currentTick;
        return true;
      }

      return false;
    }

    // ~ Pattern access

    qpPattern &addPattern(qpPattern *pattern); //creates a new layer and adds passed pattern as pattern 0

    qpPattern &operator()(int layerIndex); //returns pattern 0 from the specified layer in scene 0
    qpPattern &operator()(int sceneIndex, int layerIndex); //returns pattern 0 from the specified layer in the specified scene
    qpPattern &operator()(int sceneIndex, int layerIndex, int patternIndex);

    // ~ Component access

    qpScene &newScene();
    qpScene &scene(int sceneIndex);
    qpLayer &layer(int layerIndex);
    qpPattern &pattern(int patternIndex); //returns specified patter on layer 0

    qpScene &sameScene() { return *this->lastReferencedScene; }
    qpLayer &sameLayer() { return this->sameScene().sameLayer(); }
    qpPattern &samePattern() { return this->sameScene().sameLayer().samePattern(); }

    // ~ Scene selection

    void playScene(int index);
    void nextScene();
    void playRandomScene();

    // ~ Debug

//    int getMemUse();

};

#endif
