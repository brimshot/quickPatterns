
class qpPaletteBreathe : public qpPattern {

  private:
    byte speed;

  public:
    qpPaletteBreathe(byte speed) : speed(speed) {}

    void draw() {

      int flowWave = beatsin8(this->speed/4);

      for(int i = 0; i < _numLeds; i++)
          _targetLeds[i] = ColorFromPalette(_getPalette(), (flowWave +i), beatsin8(this->speed, 30, 255));
          
    }

};