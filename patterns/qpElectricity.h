class qpElectricity : public qpPattern {

  public:

    void draw() {

      for( int i = 0; i < 8; i++)
        _targetLeds[beatsin16(i, 0, _numLeds - 1)] = ColorFromPalette(_getPalette(), beatsin8(5));
    }

};

