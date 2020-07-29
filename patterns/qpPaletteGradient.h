
class qpPaletteGradient : public qpPattern {

  private:
    byte startHue = 0;
    byte deltaY = 0;
    byte deltaX = 0;

  public:

    /*------
    deltaX is our change in hue as we move along the strand horizontally
    deltaY is our jump in start hue as we redraw the gradient each update
    To fade the entire strand one color at a time across the gradient, set deltaX to 0
    To stop flow of colors moving down the strand, set deltaY to 0
    */
    qpPaletteGradient(byte deltaX = 3, byte deltaY = 1) {
        this->deltaX = deltaX;
        this->deltaY = deltaY;
    }

    void draw() {

      uint8_t hue = this->startHue;

      for( uint16_t i = 0; i < _numLeds; i++) {
        _targetLeds[i] = ColorFromPalette(_getPalette(), hue, 255, LINEARBLEND);
        hue += this->deltaX;
      }

      this->startHue += this->deltaY;

      if(this->startHue > 255) {
        this->startHue = 0;
        _countCycle();
      }

    }

};
