
class qpMovingGradient : public qpPattern {

  private:
    byte startHue = 0;
    byte deltaY = 0;

  public:

    /*------
    deltaX is our change in hue as we move along the strand horizontally
    deltaY is our jump in start hue as we redraw the gradient each update
    To fade the entire strand one color at a time across the gradient, set deltaX to 0
    To stop flow of colors moving down the strand, set deltaY to 0
    */
    qpMovingGradient(CRGBPalette16 palette, const byte deltaX = 3, const byte deltaY = 1) {
      // We are using parent class built in _colorPalette, and _paletteStep for deltaX just for consistency here
      this->setPalette(palette, deltaX); //TODO: why use this?
      this->deltaY = deltaY;
    }

    void draw() {

      uint8_t hue = this->startHue;

      for( uint16_t i = 0; i < _numLeds; i++) {
        _targetLeds[i] = ColorFromPalette(_colorPalette, hue, 255, LINEARBLEND);
        hue += _paletteStep;
      }

      this->startHue += this->deltaY;

    }

};
