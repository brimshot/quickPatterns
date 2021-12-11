namespace qpPatternEvents {
    
    enum EventType {ACTIVATED, DEACTIVATED, CYCLE, FINISHED};

    typedef struct {
        int layerIndex;
        int patternIndex;
        EventType event;
    } Event;

    typedef struct {
        int layerIndex;
        int patternIndex;
        EventType event;
        void (*action)();
    } EventHook;

}