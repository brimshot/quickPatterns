#ifndef QP_PATTERN_CONFIG_H
#define QP_PATTERN_CONFIG_H

// TODO: rename to qpPatternController
class qpPatternConfig {

    private:
        qpPattern *pattern;
        qpPatternScheduler *scheduler;
//        qpPatternScheduler *scheduler;
        qpColor *color;

    public:
        
        qpPatternConfig(qpPattern pattern);

        qpPattern &changeColorEveryNTicks(int minTicks, int maxTicks = 0);

};

#endif