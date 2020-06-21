
//TODO: add Direction
class qpFeathers : public qpPattern {

  private:
    int size;
    int direction;
    int startPos = 0;

  public:
    qpFeathers(int size, int direction = DIR_FORWARD) : size(size), direction(direction) {}

    void initialize() {
      this->startPos = (this->direction == DIR_FORWARD? 0:_numLeds);
    }

    void draw() {

      for(int i = 0; i < this->size; i++) {
        if(_inBounds(this->startPos + (this->direction*i)))
          _targetLeds[this->startPos + (this->direction*i)] = _getColor();
      }

      this->startPos += (this->size*this->direction);
      if(! _inBounds(this->startPos)) {
        _countCycle();
        this->startPos = (this->direction == DIR_FORWARD? 0:_numLeds);
      }
    }

};
