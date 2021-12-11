class qpPaletteWave : public qpPattern {

  private:
    byte speed;
  
  public:

    qpPaletteWave(byte speed) : speed(speed) {}

    void draw() {

      for(int i = 0; i < _numLeds; i++) 
        _targetLeds[i] = ColorFromPalette(_getPalette(), (beatsin8(this->speed) + i));

    }

};