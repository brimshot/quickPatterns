/*
#ifndef QP_SHOW_SCENE_H
#define QP_SHOW_SCENE_H

#include <quickPatterns.h>

class qpShowScene : public qpScene {

  public:

    qpPattern &playPatternFromNTicks(qpPattern *pattern, int fromTicks, int untilTicks = 0) {

        this->layer(0).addPattern(pattern);

        //  return this->layer(0).pattern(patternIndex);
    }

    void draw();

};

#endif
*/
