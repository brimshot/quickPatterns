#ifndef QP_LIGHT_STRAND_H
#define QP_LIGHT_STRAND_H

//#include <qpLinkedList.h>

class qpLightStrand {

  private:

    CRGB *displayLeds;
    int numLeds;

    qpLinkedList <CRGB> virtualLeds;

  public:

    qpLightStrand(CRGB *displayLeds, int numLeds) : displayLeds(displayLeds), numLeds(numLeds) {}

    int getNumLeds() { return this->numLeds; }

    CRGB *getLeds() { return this->displayLeds ;}

    CRGB *getVirtualLeds(byte index) {

      if(index > (this->virtualLeds.numElements-1))
        return this->virtualLeds.append(new CRGB[this->numLeds]);

      return this->virtualLeds.getItem(index);
    }


    void clearMain() {

      fill_solid(this->displayLeds, this->numLeds, CRGB::Black);
    }

    void clearAll() {

      this->clearMain();

      while(CRGB *currentLeds = this->virtualLeds.fetch())
        fill_solid(currentLeds, this->numLeds, CRGB::Black);
    }

};

#endif
