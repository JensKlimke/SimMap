//
// Created by klimke on 07.05.2019.
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
