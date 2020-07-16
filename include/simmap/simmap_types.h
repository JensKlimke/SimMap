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

/**
 * @file types.h
 *
 * The structs and types used to store the requested information.
 *
 * This header file defines the data types to store and receive the information from the SimMap framework.
 */

#ifndef SIMMAP_SIMMAP_TYPES_H
#define SIMMAP_SIMMAP_TYPES_H

#include <iostream>

namespace simmap {

    /** Enum to define the road object type. */
    enum ObjectType {
        STOP_SIGN, SPEED_LIMIT, UNKNOWN
    };

    /** Enum to define the driving direction. */
    enum Direction {
        FORWARDS, BACKWARDS
    };

    /** Enum to define the access state of the lane from another lane. */
    enum Access {
        ALLOWED, NOT_ALLOWED, NOT_POSSIBLE
    };


    /** Struct to define a point including angle and curvature in the regarding coordinate system. */
    struct Position {
        double x;     /**< the x coordinate */
        double y;     /**< the y coordinate */
        double z;     /**< the z coordinate */
        double phi;   /**< the angle at the coordinate */
        double kappa; /**< the curvature at the coordinate */
    };


    /** Struct to define a position in a map */
    struct MapPosition {
        const char *edgeID; /**< the edge ID */
        double longPos;     /**< the longitudinal position on the edge */
        double latPos;      /**< the lateral position on the edge */
    };


    /** Struct to define a point in a virtual horizon */
    struct HorizonInformation {
        double s;           /**< the longitudinal distance to the point */
        double x;           /**< the x coordinate */
        double y;           /**< the y coordinate */
        double psi;         /**< the angle of the edge at the point */
        double kappa;       /**< the curvature of the edge at the point */
        double laneWidth;   /**< the width of the ego lane at the point */
        double rightWidth;  /**< the width of the lane to the right at the point */
        double leftWidth;   /**< the width of the lane to the left at the point */
    };


    /** Struct to define an object */
    struct ObjectInformation {
        const char *id;         /**< the ID of the object */
        double distance;        /**< the longitudinal distance to the object */
        enum ObjectType type;   /**< the type of the object */
        int value;              /**< the value stored in the object, if any */
    };


    /** Struct to define an lane information */
    struct LaneInformation {
        int index;                  /**< the index of the lane (0=ego, -1=one to the right, 1 =one to the left, ... */
        double width;               /**< the width of the lane */
        enum Direction direction;   /**< the driving direction of the lane */
        double lengthOnTrack;       /**< the length of the lane along the track (route set for the agent) */
        double lengthToClosed;      /**< the length until the lane is closed */
        enum Access access;         /**< the access type to the lane from the ego lane */
        double s;                   /**< the position in the edge */
        const char *id;             /**< the id of the edge */
    };


    /** Struct to define target traffic participant information */
    struct TargetInformation {
        unsigned long id;   /**< the ID of the target */
        double x;           /**< Relative position in x-direction */
        double y;           /**< Relative position in y-direction */
        double distance;    /**< the distance to the target */
        double latOffset;   /**< the lateral offset of the target in the lane */
        int lane;           /**< the lane, the target is corresponding to */
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

#endif //SIMMAP_SIMMAP_TYPES_H

