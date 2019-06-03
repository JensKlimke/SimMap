//
// Created by Jens Klimke on 2019-01-19.
//

#ifndef SIMMAP_SERVER_LANESEPARATION_H
#define SIMMAP_SERVER_LANESEPARATION_H

#include <memory>
#include <graph/Neighbored.h>

namespace simmap {
namespace server {

    /**
     * class Separation
     * @author Jens Klimke jens.klimke@rwth-aachen.de
     */
    struct LaneSeparation {

        enum class SeparationType { NONE, AREA, LINE, STRUCTURAL };
        enum class CrossingRule { ALLOWED, RESTRICTED, CAUTION };

        double width = 0.0;
        SeparationType type = SeparationType::NONE;

        LaneSeparation() = default;
        virtual ~LaneSeparation() = default;

    };


}
}

#endif // SIMMAP_SERVER_LANESEPARATION_H
