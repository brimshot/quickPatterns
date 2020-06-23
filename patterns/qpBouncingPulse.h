
class qpBouncingPulse : public qpPattern {

  private:
    byte length = 0;
    int pos = 0;
    int dir = 1;

  public:
    qpBouncingPulse(byte length, int startingDirection = DIR_FORWARD) {
      this->length = length;
      this->dir = (startingDirection > 0)? 1:-1;
    }

    void initialize() {
      //if our starting direction was 'reverse' then we need to start at the end of the strand
      if(this->dir == DIR_REVERSE)
        this->pos = _numLeds;
    }

    void draw() {

      _clearLeds();

      for(int i = 0; i < this->length; i++) {
        if(_inBounds(this->pos + i)) {
          _targetLeds[(this->pos + i)] = _getColor();
//          _targetLeds[(this->pos + i)].fadeToBlackBy(255 - ((255/this->length)*i)); //TODO: this isn't working....
        }
      }

      //move the pulse one position
      this->pos += this->dir;

      if(! _inBounds(this->pos)) { //we hit a boundary
        // reverse direction
        this->dir *= -1;

        _countCycle();
      }

    }

};
