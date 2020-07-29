class qpJuggle : public qpPattern {

  public:

    void draw() {

      for( int i = 0; i < 8; i++)
        _targetLeds[beatsin16(i+7, 0, _numLeds - 1)] = _getColor();
    }

};
