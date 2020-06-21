
//TODO: rename to Glitter
class qpConfetti : public qpPattern {

  private:
    int spread;

  public:
    qpConfetti(int spread = 30) {
      this->spread = spread;
    }

    void draw() {

      for( uint16_t i = random8(this->spread); i < _numLeds; i += (random8(this->spread) + 1))
        _targetLeds[i] = _getColor();
    }

};
