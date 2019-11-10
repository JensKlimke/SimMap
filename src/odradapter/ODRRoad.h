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
// Created by Jens Klimke on 2019/05/07
//

#ifndef SIMMAP_ODRADAPTER_ODRROAD_H
#define SIMMAP_ODRADAPTER_ODRROAD_H

#include <graph/Edge.h>
#include <curve/Curve.h>
#include <curve/C3Spline.h>
#include <memory>
#include <list>
#include "LaneSectionSequence.h"
#include "ODRObject.h"


struct ODRRoad : public graph::Edge {

    std::shared_ptr<simmap::curve::Curve> _curve{};
    std::shared_ptr<simmap::curve::C3Spline> _offset{};

    LaneSectionSequence lanes{};
    double _length = 0;

    static std::vector<std::shared_ptr<ODRObject>> _objVector;
    ObjectsList _objects{};


    double length() const override {

        return _length;

    }


    def::Orientation orientation() const override {

        return def::Orientation::BOTH;

    }


    ObjectsList objects() const override {

        return _objects;

    }

};



#endif // SIMMAP_ODRADAPTER_ODRROAD_H
