
class qpPusher : public qpPattern {

  private:
    uint16_t length = 0;
    uint16_t offset = 0;
    uint16_t head = 0;

  public:
    qpPusher(const int length) {
      this->length = length;
    }

    void draw() {

      if(_inBounds(this->head))
        _targetLeds[this->head] = _getColor();

      this->head++;

      if(++this->offset >= this->length) {

        if(!_inBounds(this->head))
          this->head = 0;
        else
          this->head -= (this->length / 2);

        this->offset = 0;

        _countCycle();
      }

    }

};
