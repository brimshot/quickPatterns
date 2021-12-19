class qpComet : public qpPattern {

    private:
        byte length;
        bool bounce;
        int pos = 0;
        int dir = 1;

        bool _inBounds(int pos) { return ((pos >= 0) && (pos < _numLeds)); }

    public:

        qpComet(byte length, bool bounce = false, byte dir = 1) : length(length), bounce(bounce), dir(dir) {}

        void draw() {
            
            if(_inBounds(this->pos))
                _targetLeds[this->pos] = _getColor();

            if(_inBounds(this->pos - (this->length*this->dir)))
                _targetLeds[(this->pos - (this->length*this->dir))] = CRGB::Black;
            
            if((this->pos >= (_numLeds + this->length)) || (this->pos <= (0 - this->length))) {
                if(this->bounce)
                    this->dir *= -1;
                else
                    this->pos = 0;    
                    
                _countCycle();
            }

            this->pos += this->dir;            
      }

};