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



    base::CurvePoint LaneEdge::startPoint() const {

        return position(0.0, base::Reference::CENTER, 0.0);

    }



    base::CurvePoint LaneEdge::endPoint() const {

        return position(length(), base::Reference::CENTER, 0.0);

    }



    base::CurvePoint LaneEdge::position(double s) const {

        return position(s, base::Reference::CENTER, 0.0);

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