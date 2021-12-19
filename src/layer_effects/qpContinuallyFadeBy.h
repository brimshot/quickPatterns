class qpContinuallyFadeBy : public qpLayerEffect {

    private:
        int fadeAmount = 0;

    public:

        qpContinuallyFadeBy(int fadeAmount = 20) : fadeAmount(fadeAmount) {}

        void apply(CRGB *targetLeds, int numLeds) {
            fadeToBlackBy(targetLeds, numLeds, this->fadeAmount);
        }
};