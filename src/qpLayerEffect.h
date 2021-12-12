#ifndef QP_LAYER_EFFECT_H
#define QP_LAYER_EFFECT_H

class CRGB;

class qpLayerEffect {

    public:
        virtual void apply(CRGB *targetLeds, int numLeds) = 0;
};

#endif