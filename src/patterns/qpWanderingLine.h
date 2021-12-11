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
      if(random8(100) > 50)
        this->changeDirection();
    }

    void changeDirection() {
      this->dir *= -1;
      this->pos += (this->dir * this->length);
    }

  public:

    qpWanderingLine(byte length) : length(length) {}

    void initialize() {
      // get an initial starting position
      this->pos = random16((_numLeds - this->length));
      this->getNewDistance();
      this->chooseRandomDirection();
    }

    void draw() {
  
      this->pos += this->dir;

       _targetLeds[this->pos] = _getColor();
  
      if(! _inBounds(this->pos)) {
        // We hit a boundary - reverse direction, calculate a new distance to roam and count a cycle
        this->changeDirection();
        this->getNewDistance();

        _countCycle();

      } else if(this->dist == 0) {
        // We roamed our distance, get new values and count a cycle
        this->getNewDistance();
        this->chooseRandomDirection();

        _countCycle();

      }

      if(_inBounds(this->pos - (this->length*this->dir)))
          _targetLeds[(this->pos - (this->length*this->dir))] = CRGB::Black;

      this->dist--;

    }

};
