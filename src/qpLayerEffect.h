#ifndef QP_LAYER_EFFECT_H
#define QP_LAYER_EFFECT_H


#include <FastLED.h>

class qpLayerEffect {

    private:
        int ticks;
    
    public:
        virtual void render(CRGB *targetLeds);

};

#endif