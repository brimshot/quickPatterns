class qpPaletteDissolve : public qpPattern {

  private:
    byte speed;
  
  public:

    qpPaletteDissolve(byte speed) : speed(speed) {}

    void draw() {

      for(int i = 0; i < _numLeds; i++)
        _targetLeds[i] = ColorFromPalette(_getPalette(), (beatsin8(this->speed) + i), beatsin8(i/2));

    }

};