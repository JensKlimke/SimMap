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
// Created by Jens Klimke on 2019-08-10.
//

#ifndef SIMMAP_TYPES_H
#define SIMMAP_TYPES_H

#include <iostream>

namespace simmap {

    enum ObjectType {
        STOP_SIGN, SPEED_LIMIT, UNKNOWN
    };

    enum Direction {
        FORWARDS, BACKWARDS
    };

    enum Access {
        ALLOWED, NOT_ALLOWED, NOT_POSSIBLE
    };


    struct Position {
        double x;
        double y;
        double z;
        double phi;
        double kappa;
    };


    struct MapPosition {
        const char *edgeID;
        double longPos;
        double latPos;
    };


    struct HorizonInformation {
        double s;
        double x;
        double y;
        double psi;
        double kappa;
        double laneWidth;
        double rightWidth;
        double leftWidth;
    };


    struct ObjectInformation {
        const char *id;
        double distance;
        enum ObjectType type;
        int value;
    };


    struct LaneInformation {
        int index;
        double width;
        enum Direction direction;
        double lengthOnTrack;
        double lengthToClosed;
        enum Access access;
        double s;
        const char *id;
    };


    struct TargetInformation {
        unsigned int id;
        double distance;
        double latOffset;
        int lane;
    };

}

inline std::ostream &operator<< (std::ostream &os, const simmap::Position& pos) {
    os << "xyz=(" << pos.x << "," << pos.y << "," << pos.z << "),psi=" << pos.phi << ",kappa=" << pos.kappa;
    return os;
}

inline std::ostream &operator<< (std::ostream &os, const simmap::MapPosition& pos) {
    os << "edge:" << pos.edgeID << ",s=" << pos.longPos << ",d=" << pos.latPos;
    return os;
}

#endif //SIMMAP_TYPES_H

