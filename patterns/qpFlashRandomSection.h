
class qpFlashRandomSection : public qpPattern {

  private:
    int sectionSize;
    int startPos;
    byte numFlashes;
    byte flashSteps;

    void findNewSection() {
      this->startPos = random16(0, (_numLeds - this->sectionSize));
    }

  public:
    qpFlashRandomSection(int sectionSize, byte numFlashes = 8) {
      this->sectionSize = sectionSize;
      this->numFlashes = numFlashes;
      this->flashSteps = this->numFlashes*2;
    }

    void initialize() {
      this->findNewSection();
    }

    void draw() {

      for(int i = 0; i < this->sectionSize; i++) {
        if(this->flashSteps%2)
          _targetLeds[(this->startPos + i)] = _getColor();
        else
          _targetLeds[(this->startPos + i)] = CRGB::Black;
      }

      if(this->flashSteps-- <= 0) {
        this->findNewSection();
        this->flashSteps = this->numFlashes*2;

        _countCycle();
      }

    }

};
