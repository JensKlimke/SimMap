//
// Created by Jens Klimke on 18.11.18.
//

#include "MapCoordinate.h"
#include "LaneEdge.h"


namespace simmap {
namespace server {


    MapCoordinate::MapCoordinate(const LaneEdge *edge, double s, double d) : _edge(edge), _s(0.0), _d(d) {

        // abort here if edge is null
        if(edge == nullptr)
            return;

        this->s(s);

    }



    const LaneEdge* MapCoordinate::edge() const {

        return _edge;

    }



    double MapCoordinate::s() const {

        return _s;

    }



    void MapCoordinate::s(double s) {

        m_assert(s >= 0 && s <= _edge->length(), "s out of bounds");
        _s = s;

    }



    double MapCoordinate::d() const {

        return _d;

    }



    void MapCoordinate::d(double d) {

        _d = d;

    }



    void MapCoordinate::shift(double ds) {

        s(_s + ds);

    }



    def::CurvePoint MapCoordinate::absolutePosition() const {

        return _edge->position(_s, def::Reference::CENTER, _d);

    }



    double MapCoordinate::width() const {

        return _edge->width(_s);

    }


    MapCoordinate MapCoordinate::neighbor(size_t n, bool toRight) const {

        // get neighbored edge
        auto e = edge()->neighbor(s(), toRight ? ::graph::Neighbored::Side::RIGHT : ::graph::Neighbored::Side::LEFT, n);
        auto *edge = dynamic_cast<const LaneEdge*>(e.second);

        // return out of road map coordinate
        if(e.second == nullptr)
            return MapCoordinate(nullptr, 0.0, 0.0);

        // create map coordinate
        return MapCoordinate(edge, e.first, 0.0);

    }


    MapCoordinate MapCoordinate::right(size_t n) const {

        return neighbor(n, true);

    }



    MapCoordinate MapCoordinate::left(size_t n) const {

        return neighbor(n, false);

    }



    bool MapCoordinate::outOfRoad() const {

        return _edge == nullptr;

    }

}
}

std::ostream &operator<<(std::ostream &o, const simmap::server::MapCoordinate &c) {

    return o << *c.edge() << " (s=" << c.s() << ", d=" << c.d() << ")";

}
