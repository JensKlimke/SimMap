//
// Created by Jens Klimke on 12.11.18.
//

#include <iostream>
#include <algorithm>
#include "Track.h"
#include "LaneEdge.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <graph/Object.h>


namespace simmap {
namespace server {



    def::CurvePoint LaneEdge::startPoint() const {

        return position(0.0, def::Reference::CENTER, 0.0);

    }



    def::CurvePoint LaneEdge::endPoint() const {

        return position(length(), def::Reference::CENTER, 0.0);

    }



    def::CurvePoint LaneEdge::position(double s) const {

        return position(s, def::Reference::CENTER, 0.0);

    }



    Track::TrackElement LaneEdge::trackElement() const {

        return _trackElement;

    }

}
}


std::ostream &operator<<(std::ostream &os, const simmap::server::LaneEdge &edge) {

    // TODO: implement
    return os;

}