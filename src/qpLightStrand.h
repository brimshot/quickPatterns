#ifndef QP_LIGHT_STRAND_H
#define QP_LIGHT_STRAND_H

#include "qpLinkedList.h"
#include <FastLED.h>

/**
 * @brief This class manages all of the in memory LED arrays, allowing them to be reused across different scenes once declared
 */
class qpLightStrand {

  private:

    CRGB *displayLeds;
    int numLeds;

    qpLinkedList <CRGB> virtualLeds;

  public:

    qpLightStrand(CRGB *displayLeds, int numLeds) : displayLeds(displayLeds), numLeds(numLeds) {}

    CRGB *getLeds() { return this->displayLeds ;}

    int getNumLeds() { return this->numLeds; }

    CRGB *getVirtualLeds(byte index) {

      if(index > (this->virtualLeds.numElements-1))
        return this->virtualLeds.append(new CRGB[this->numLeds]);

      return this->virtualLeds.getItem(index);
    }

    void clearMain() { fill_solid(this->displayLeds, this->numLeds, CRGB::Black); }

    void clearAll() {

      this->clearMain();

      while(CRGB *currentLeds = this->virtualLeds.fetch())
        fill_solid(currentLeds, this->numLeds, CRGB::Black);
    }

};

#endif