
class qpSinelon : public qpPattern {

  private:
    byte speed;

  public:
    qpSinelon(byte speed) {
      this->speed = speed;
    }

    void draw() {
      int pos = beatsin16(this->speed, 0, _numLeds-1 );
      _targetLeds[pos] = _getColor();
    }

};
