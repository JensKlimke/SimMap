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
// Created by Jens Klimke on 13.10.18.
//

#include "Oriented.h"

namespace graph {


    bool Oriented::isForward() const {

        auto o = orientation();
        return o != base::Orientation::BACKWARDS;

    }


    bool Oriented::isBackwards() const {

        auto o = orientation();
        return o == base::Orientation::BACKWARDS;

    }


    void Oriented::link(Oriented *obj, bool before) {

        if(isForward() != before)
            next(obj);
        else
            prev(obj);

    }


    void Oriented::next(Oriented *obj, base::ContactPoint contactPoint, bool bi) {

        // set successor for this road
        if(!isNext(obj, contactPoint))
            _nexts.push_back({contactPoint, obj});

        // abort here if no bi-directional connection
        if(!bi)
            return;

        // bi-directional connection
        if(contactPoint == base::ContactPoint::START)
            obj->prev(this, base::ContactPoint::END, false);
        else
            obj->next(this, base::ContactPoint::END, false);

    }


    void Oriented::prev(Oriented *obj, base::ContactPoint contactPoint, bool bi) {

        // set predecessor for this road
        if(!isPrev(obj, contactPoint))
            _prevs.push_back({contactPoint, obj});

        // abort here if no bi-directional connection
        if(!bi)
            return;

        // bi-directional connection
        if(contactPoint == base::ContactPoint::START)
            obj->prev(this, base::ContactPoint::START, false);
        else
            obj->next(this, base::ContactPoint::START, false);

    }


    const Oriented::Connections & Oriented::nexts() const {

        return _nexts;

    }

    const Oriented::Connections & Oriented::prevs() const {

        return _prevs;

    }


    bool Oriented::isNext(graph::Oriented *obj, base::ContactPoint contactPoint) const {

        // iterate over connections to find the given object
        for(const auto &e : _nexts)
            if(e.first == contactPoint && e.second == obj)
                return true;

        // not found
        return false;

    }


    bool Oriented::isPrev(graph::Oriented *obj, base::ContactPoint contactPoint) const {

        // iterate over connections to find the given object
        for(const auto &e : _prevs)
            if(e.first == contactPoint && e.second == obj)
                return true;

        // not found
        return false;

    }

}