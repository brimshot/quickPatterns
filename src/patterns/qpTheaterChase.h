//rename to Chase ? or make Chase
class qpTheaterChase : public qpPattern {

  private:
    byte offset = 0;

    bool _inBounds(int pos) { return ((pos >= 0) && (pos < _numLeds)); }

  public:

    void draw() {

      fill_solid(_targetLeds, _numLeds, CRGB::Black);

      for (int i=0; i < _numLeds; i=i+3) {
        if(_inBounds(i+this->offset))
          _targetLeds[(i+this->offset)] = _getColor();
      }

      if(++this->offset >= 3) {
        this->offset = 0;
        _countCycle();
      }


    }

};
