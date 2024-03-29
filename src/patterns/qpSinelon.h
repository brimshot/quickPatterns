
class qpSinelon : public qpPattern {

  private:
    byte speed;

  public:
    qpSinelon(byte speed) : speed(speed) {}
    
    void draw() {
      
      int pos = beatsin16(this->speed, 0, _numLeds-1);
      if(pos <= 0)
        _countCycle();
        
      _targetLeds[pos] = _getColor();
    }

};
