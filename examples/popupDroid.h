
class qpWanderingLine : public qpPattern {

  private:
    int pos = 0;
    int dir = 1;
    byte length = 0;
    int dist = 0;

    void getNewDistance() {
      this->dist = random16(_numLeds / 2);
    }

    void chooseRandomDirection() {
      this->dir = random8(100) > 50? 1:-1;
    }

    bool canMove() {
      return _inBounds(this->pos) && _inBounds(this->pos + this->length);
    }

  public:

    qpWanderingLine(byte length) {
      this->length = length;
    }

    void initialize() {
      // get an initial starting position
      this->pos = random16((_numLeds - this->length));
      this->getNewDistance();
      this->chooseRandomDirection();
    }

    void draw() {

      _clearLeds();

      for(int i = 0; i < this->length; i++) {
        if(_inBounds(this->pos + i))
          _targetLeds[this->pos + i] = _getColor();
      }

      this->pos += this->dir;

      this->dist--;

      if(! this->canMove()) {
        // We hit a boundary - reverse direction, calculate a new distance to roam and count a cycle
        this->getNewDistance();
        this->dir *= -1; // change direction

        _countCycle();

      } else if(this->dist == 0) {
        // We roamed our distance, get new values and count a cycle
        this->getNewDistance();
        this->chooseRandomDirection();

        _countCycle();

      }


    }

};
