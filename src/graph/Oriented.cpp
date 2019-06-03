//
// Created by Jens Klimke on 13.10.18.
//

#include "Oriented.h"

namespace graph {


    bool Oriented::isForward() const {

        auto o = orientation();
        return o != def::Orientation::BACKWARDS;

    }


    bool Oriented::isBackwards() const {

        auto o = orientation();
        return o == def::Orientation::BACKWARDS;

    }


    void Oriented::link(Oriented *obj, bool before) {

        if(isForward() != before)
            next(obj);
        else
            prev(obj);

    }


    void Oriented::next(Oriented *obj, def::ContactPoint contactPoint, bool bi) {

        // set successor for this road
        if(!isNext(obj, contactPoint))
            _nexts.push_back({contactPoint, obj});

        // abort here if no bi-directional connection
        if(!bi)
            return;

        // bi-directional connection
        if(contactPoint == def::ContactPoint::START)
            obj->prev(this, def::ContactPoint::END, false);
        else
            obj->next(this, def::ContactPoint::END, false);

    }


    void Oriented::prev(Oriented *obj, def::ContactPoint contactPoint, bool bi) {

        // set predecessor for this road
        if(!isPrev(obj, contactPoint))
            _prevs.push_back({contactPoint, obj});

        // abort here if no bi-directional connection
        if(!bi)
            return;

        // bi-directional connection
        if(contactPoint == def::ContactPoint::START)
            obj->prev(this, def::ContactPoint::START, false);
        else
            obj->next(this, def::ContactPoint::START, false);

    }


    const Oriented::Connections & Oriented::nexts() const {

        return _nexts;

    }

    const Oriented::Connections & Oriented::prevs() const {

        return _prevs;

    }


    bool Oriented::isNext(graph::Oriented *obj, def::ContactPoint contactPoint) const {

        // iterate over connections to find the given object
        for(const auto &e : _nexts)
            if(e.first == contactPoint && e.second == obj)
                return true;

        // not found
        return false;

    }


    bool Oriented::isPrev(graph::Oriented *obj, def::ContactPoint contactPoint) const {

        // iterate over connections to find the given object
        for(const auto &e : _prevs)
            if(e.first == contactPoint && e.second == obj)
                return true;

        // not found
        return false;

    }

}