#ifndef QP_SHOW_H
#define QP_SHOW_H

#include <quickPatterns.h>

struct qpShowScene {
  qpScene *scene;
  int duration;
};

class qpShow {

  private:

    quickPatterns *patternsController;

    qpLinkedList <qpShowScene> showScenes;

  public:

    qpScene &includeScene(qpScene *scene, int durationInMillis = 0);

    void draw();

};

#endif
