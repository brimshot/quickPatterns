
class qpSparkles : public qpPattern {

  private:
    byte speed;

  public:
    qpSparkles(byte speed) : speed(speed) {}
    
    void draw() {

      int pos = beatsin8(beatsin8(this->speed), 0, _numLeds);
      _targetLeds[pos] = _getColor();
    }

};
