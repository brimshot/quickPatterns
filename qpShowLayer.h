#ifndef QP_SHOW_LAYER_H
#define QP_SHOW_LAYER_H

struct qpShowPatternTime {
  qpPattern *pattern;
  int startTick;
  int endTick;
};

class qpShowLayer : public qpLayer {

  public:

    qpPattern &playPatternFromNTick(qpPattern *pattern, int fromTick, int untilTick = 0);
    qpPattern &playPatternFromNTick(int index, int fromTick, int untilTick = 0);

    void draw();
};

#endif
