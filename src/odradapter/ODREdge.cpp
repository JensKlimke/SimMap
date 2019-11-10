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
// Created by Jens Klimke on 10.05.2019.
//

#include <base/functions.h>
#include "ODRRoad.h"
#include "ODREdge.h"

double ODREdge::offset(double s, def::Reference ref, double d) const {

    // get offset
    auto o = border(s) + d;
    if (ref == def::Reference::INNER)
        return o;

    // add width (half for center)
    auto w = -width(s);
    if (ref == def::Reference::CENTER)
        return o + w * 0.5;
    else if (ref == def::Reference::OUTER)
        return o + w;

    return 0.0;

}

def::CurvePoint ODREdge::position(double s, def::Reference ref, double d) const {

    // calculate ref position and offset
    auto off = offset(s, ref, d);

    // calculate position
    auto pos = _road->_curve->operator()(sRoad(s));
    pos.angle += isForward() ? 0.0 : M_PI;
    pos.curvature *= (isBackwards() ? -1.0 : 1.0) / (1.0 - pos.curvature * off * (isBackwards() ? -1.0 : 1.0));

    // transform offset to coordinate
    pos.position = base::toGlobal(pos, {0.0, off, 0.0});

    return pos;

}


const ODREdge *ODREdge::inner() const {

    return _inner;
}


double ODREdge::border(double s) const {

    auto in = inner();
    if (_border)
        return _border->operator()(sRel(s)) - width(s); // check if border is set explicitly
    else if (in != nullptr)
        return in->offset(s, def::Reference::OUTER, 0.0);

    return (isForward() ? 1.0 : -1.0) * _road->_offset->operator()(sRoad(s)); // otherwise lane offset, when set

}


double ODREdge::width(double s) const {

    if (_width)
        return _width->operator()(sRel(s)); // check if width is set explicitly
    else if (inner() != nullptr)
        return border(s) - inner()->border(s); // other wise use border and inner border, if inner is set
    else
        return border(s); // otherwise simply use border

}


double ODREdge::length() const {

return _s1 - _s0;

}


double ODREdge::sRoad(double s) const {

    return isForward() ? s + _s0 : _s1 - s;

}


double ODREdge::sRel(double s) const {

    return isForward() ? s : _s1 - _s0 - s;

}


def::Orientation ODREdge::orientation() const {

    return _laneID == 0 ? def::Orientation::NONE :
    _laneID > 0 ? def::Orientation::BACKWARDS : def::Orientation::FORWARDS;

}

graph::Edge::ObjectsList ODREdge::objects() const {

    return _objs;

}
