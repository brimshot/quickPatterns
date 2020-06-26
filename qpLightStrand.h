#ifndef QP_LIGHT_STRAND_H
#define QP_LIGHT_STRAND_H

#include <qpLinkedList.h>

class qpLightStrand {

  private:

    qpLinkedList <CRGB> ledArrays;

  public:

    qpLightStrand(CRGB *leds, int numLeds) : leds(leds), numLeds(numLeds) {}

    CRGB *leds;
    int numLeds;

    int getNumLeds() { return this->numLeds; }


    CRGB *getLedArray(int index) {

      if(index > (this->ledArrays.numElements-1))
        return this->ledArrays.append(new CRGB[this->numLeds]);

      return this->ledArrays.getItem(index);
    }


    void clearMain() {

      fill_solid(this->leds, this->numLeds, CRGB::Black);
    }

    void clearAll() {

      this->clearMain();

      while(CRGB *currentLeds = this->ledArrays.fetch())
        fill_solid(currentLeds, this->numLeds, CRGB::Black);
    }

};

#endif
