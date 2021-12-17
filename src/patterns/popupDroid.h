
class popupDroid : public qpPattern {

  private:
    enum {POPUP, GO} state = POPUP;

    bool clearEachCycle = true;
    byte size = 0;
    uint16_t startPos = 0;
    int dir = 1;

    //popup state
    byte numFlashes = 8;
    byte flashSteps = 0;

    //go state
    byte goMoves = 0;
    byte goDist = 0;

  private:

    void reset() {
      this->flashSteps = numFlashes*2;
      this->startPos = random16(0, (_numLeds - this->size));
      this->state = POPUP;
    }

    void changeDirection() {
      this->dir *= -1;
    }

    bool _inBounds(int pos) { return ((pos >= 0) && (pos < _numLeds)); }

  public:

    popupDroid(int size = 6, bool clearEachCycle = true) {
      this->size = size;
      this->clearEachCycle = clearEachCycle;

    }

    void initialize() {
      this->reset();
    }


    void draw() {

      fill_solid(_targetLeds, _numLeds, CRGB::Black);

      switch(this->state) {
        case POPUP:
          this->doPOPUP();
          break;
        case GO:
          this->doGO();
          break;
      }

    }


    void doPOPUP() {
      for(int i = 0; i < this->size; i++) {
        if(this->flashSteps%2)
          _targetLeds[(this->startPos+i)] = _getColor();
        else
          _targetLeds[(this->startPos+i)] = CRGB::Black;
      }

      //init GO here
      if(--this->flashSteps <= 0) {
        this->state = GO;
        this->goMoves = random16(4, 8);
        this->goDist = random(this->size, (_numLeds / 2));
      }
    }

    void doGO() {

      for(int i = 0; i < this->size; i++) {
        if(_inBounds(this->startPos+i))
          _targetLeds[(this->startPos+i)] =  _getColor();
      }

      this->startPos += this->dir;

      if(!_inBounds(this->startPos+this->size) || (--this->goDist <= 0)) {
        if(--this->goMoves <= 0) {
          this->reset();
          if(this->clearEachCycle) {
            fill_solid(_targetLeds, _numLeds, CRGB::Black);
          }
          
          _countCycle();
        }
        this->goDist = random(this->size, (_numLeds / 2));
        this->changeDirection();
      }

    }

};
