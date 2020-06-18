#ifndef QUICK_PATTERNS_H
#define QUICK_PATTERNS_H

#include <FastLED.h>

//TODO: name QP_BRUSH_TYPE
enum BRUSH_TYPE {ADD, SUBTRACT, COMBINE, OVERLAY, OVERWRITE, MASK};

#define DIR_FORWARD 1
#define DIR_REVERSE -1

#include <qpLinkedList.h>
#include <qpPattern.h>
#include <qpLayer.h>
#include <qpScene.h>

//TODO: move pattern update to draw

class qpScene;

class quickPatterns {

  private:

    short tickLengthInMillis = 25;
    uint32_t nextTickMillis = 0;
    uint32_t currentTick = 0; //todo: why do we need this?

    qpLinkedList <CRGB> ledArrays; //our main store of led 'layers' - once declared, these get re-used across scenes
    qpLinkedList <qpScene> scenes;

    int sceneIndex = 0;
    qpScene *currentScene = NULL;

    qpScene *lastReferencedScene;
    qpLayer *lastReferencedLayer;

    void clearMain();
    void clearAll();

    inline void addEntropy();

  public:

    quickPatterns(CRGB *leds, int numLeds);

    CRGB *leds;
    int numLeds;

    CRGB *getLedArray(int index);

    // ~ Setup

    void setTickMillis(int tickLengthMillis) {
      this->tickLengthInMillis = tickLengthMillis;
    }

    // ~ Rendering

    void draw();

    // ~ Patterns
    qpPattern &addPattern(qpPattern *pattern); //creates a new layer and adds passed pattern as pattern 0

    qpPattern &operator()(int layerIndex); //returns pattern 0 from the specified layer in scene 0
    qpPattern &operator()(int layerIndex, int patternIndex);
    qpPattern &operator()(int sceneIndex, int layerIndex, int patternIndex);

    // ~ Component access
    qpScene &scene(int sceneIndex);
    qpLayer &layer(int layerIndex);

    qpScene &sameScene() { return *this->lastReferencedScene; }
    qpLayer &sameLayer()  { return *this->lastReferencedLayer; }

    void setLastUsedLayer(qpLayer *layerPtr) {
      this->lastReferencedLayer = layerPtr;
    }

    // ~ Scenes
    void playScene(int index);
    void nextScene();
    void playRandomScene();

    void loadScene(qpScene &scene);

    // ~ Debug

    int getMemUse();

};

#endif
