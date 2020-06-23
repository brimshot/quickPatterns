
class qpRandomBar : public qpPattern {
  private:
    int size;

  public:

    qpRandomBar(int size) {
      this->size = size;
    }

    void draw() {

      int pos = random16(0, (_numLeds - this->size));

      for(int i = 0; i < this->size; i++)
          _targetLeds[pos + i] = _getColor();

      _countCycle();    

    }

};
