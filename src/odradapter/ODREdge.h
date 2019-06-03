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

class ODRRoad;

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


    double offset(double s, def::Reference ref, double d) const;

    def::CurvePoint position(double s, def::Reference ref, double d) const override;

    double width(double s) const override;

    double length() const override;

    def::Orientation orientation() const override;

    ObjectsList objects() const override;

protected:


    const ODREdge *inner() const;

    double border(double s) const;

    double sRoad(double s) const;

    double sRel(double s) const;


};


#endif // SIMMAP_ODRADAPTER_ODREDGE_H
