class qpBreathe : public qpLayerEffect {

    private:
        int minBrightness = 0;
        unsigned int ticks;
        int speed = 2;
        int fadeStep = 0;
        int fadeAmount = 0;
        bool getBrighter = true;

    public:

        qpBreathe(int speed, int minBrightness = 100, int fadeStep = 10) : speed(speed), minBrightness(minBrightness), fadeStep(fadeStep) {}

        void apply(CRGB *targetLeds, int numLeds) {

            fadeToBlackBy(targetLeds, numLeds, this->fadeAmount);

            this->ticks++;

            // TODO: replace modulo with bit shift division
            if(this->ticks % this->speed)
                return;

            if(this->fadeAmount >= (255 - this->minBrightness) || this->fadeAmount == 0) {
                this->getBrighter = (! this->getBrighter);
            }

            if(getBrighter) {
                this->fadeAmount -= this->fadeStep;
            } else {
                this->fadeAmount += this->fadeStep;
            }
        }
};