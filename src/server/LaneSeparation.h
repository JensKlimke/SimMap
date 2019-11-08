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

        /** An enumeration for the separation type */
        enum class SeparationType { NONE, AREA, LINE, STRUCTURAL };


        /** An enumeration for crossing rule */
        enum class CrossingRule { ALLOWED, RESTRICTED, CAUTION };


        double width = 0.0; //!< The width of the separation area
        SeparationType type = SeparationType::NONE; //!< The type of the separation
        CrossingRule  rule = CrossingRule::ALLOWED; //!< The rule to cross the separation


        /**
         * Default constructor
         */
        LaneSeparation() = default;


        /**
         * Default destructor
         */
        virtual ~LaneSeparation() = default;

    };


}
}

#endif // SIMMAP_SERVER_LANESEPARATION_H
