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
