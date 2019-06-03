//
// Created by klimke on 13.03.2019.
//

#ifndef SIMMAP_ODRADAPTER_ROADMARKING
#define SIMMAP_ODRADAPTER_ROADMARKING


enum class LineCrossingRule { ALLOWED, NOT_ALLOWED, CAUTION };

struct ExplicitRoadMarking {
    double width = 0.0;
    double latOffset = 0.0;
    double lineLength = 0.0;
    LineCrossingRule rule{LineCrossingRule::ALLOWED};
};

typedef base::sequence<ExplicitRoadMarking> ODRRoadMarking;


#endif // SIMMAP_ODRADAPTER_ROADMARKING
