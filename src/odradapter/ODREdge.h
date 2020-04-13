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
// Created by Jens Klimke on 2019-05-06.
//

#ifndef SIMMAP_ODRADAPTER_ODREDGE_H
#define SIMMAP_ODRADAPTER_ODREDGE_H

#include <server/LaneEdge.h>
#include <curve/C3Spline.h>
#include <memory>
#include <vector>
#include "ODRObject.h"

// TODO: for efficiency just calculate border by adding widths and border splines

struct ODRRoad;

struct ODREdge : public simmap::server::LaneEdge {

    enum class DrivingType {
        DRIVABLE, RESTRICTED
    };

    int _laneID = 0;
    DrivingType _drivingType = DrivingType::DRIVABLE;

    double _s0 = 0.0;
    double _s1 = 0.0;

    const ODRRoad *_road = nullptr;
    const ODREdge *_inner = nullptr;
    const ODREdge *_outer = nullptr;

    std::unique_ptr<simmap::curve::C3Spline> _width;
    std::unique_ptr<simmap::curve::C3Spline> _border;

    std::vector<int> _pred{};
    std::vector<int> _succ{};

    ObjectsList _objs{};


    double offset(double s, base::Reference ref, double d) const;

    base::CurvePoint position(double s, base::Reference ref, double d) const override;

    double width(double s) const override;

    double length() const override;

    base::Orientation orientation() const override;

    ObjectsList objects() const override;

protected:


    const ODREdge *inner() const;

    double border(double s) const;

    double sRoad(double s) const;

    double sRel(double s) const;


};


#endif // SIMMAP_ODRADAPTER_ODREDGE_H
