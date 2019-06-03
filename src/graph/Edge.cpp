//
// Created by Jens Klimke on 2019-01-03.
//

#include "Edge.h"

namespace graph {


    const std::string& Edge::id() const {

        return _id;

    }


    bool Edge::isForward() const {

        return !(orientation() == def::Orientation::BACKWARDS);

    }


    bool Edge::isDirectionalCompatible(def::Orientation ori) const {

        return (orientation() == def::Orientation::NONE) || (isForward() == (ori == def::Orientation::FORWARDS));

    }

}