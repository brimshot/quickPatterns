#ifndef QUICK_PATTERNS_SHOW_H
#define QUICK_PATTERNS_SHOW_H

class quickPatterns;

struct qpShowEvent {
    int eventType;
    bool (*conditionalFunction)();
    void (*eventCode)(quickPatterns &controller);
    qpShowEvent *next = NULL;
};

class qpShow {
  private:
//    qpShowEvent *nowEvent;
    qpShowEvent *lastEvent;

  public:

    qpShowEvent *currentEvent;

    qpShow() {
      this->currentEvent = new qpShowEvent;
    }

    qpShowEvent *getLastEvent() {
      return this->lastEvent;
    }

    qpShowEvent *addEvent() {

      if(! this->currentEvent)
        this->currentEvent = this->lastEvent = new qpShowEvent;
      else {
        this->lastEvent->next = new qpShowEvent;
        this->lastEvent = this->lastEvent->next;
      }

      return this->lastEvent;
    }

    void playShow() {

      if(this->currentEvent) {
        if((*(this->currentEvent->conditionalFunction))()) {
            Serial.println("It's true");
//            (*(this->currentEvent->eventCode))(new quickPatterns());
            this->currentEvent = this->currentEvent->next;
        }
      }
    }
};

#endif
