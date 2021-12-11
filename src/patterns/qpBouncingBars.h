//rename Cylon
class qpBouncingBars : public qpPattern {

  private:
    int size;
    int offset = 0;

  public:
    qpBouncingBars(int size) : size(size) {}

    void draw() {

      // Black out previous pixels before drawing the next frame
      _clearLeds();

      // Draw two bars of width size equally offset from either end of our strand
      for(int i = 0; i < this->size; i++) {
          _targetLeds[this->offset+i] = _getColor();
          _targetLeds[(_numLeds - 1) - (this->offset + i)] = _getColor();
      }

      // Increase offset by 1 or reset and cycle
      if(this->offset++ >= ((_numLeds - 1) - this->size)) {
        this->offset = 0;

        _countCycle();
      }

    }

};
