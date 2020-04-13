//
// Copyright (c) 2019 Jens Klimke <jens.klimke@rwth-aachen.de>. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
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

        if (s < 0 || s > _edge->length())
            throw std::invalid_argument("s out of bounds");

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



    base::CurvePoint MapCoordinate::absolutePosition() const {

        return _edge->position(_s, base::Reference::CENTER, _d);

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
