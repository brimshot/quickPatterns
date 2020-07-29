// ** Fills strand with configured color

class qpFill : public qpPattern {

  public:

    void draw() {

      for( int i = 0; i < _numLeds; i++)
        _targetLeds[i] = _getColor();
    }

};
