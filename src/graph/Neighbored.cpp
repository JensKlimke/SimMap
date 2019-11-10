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
// Created by Jens Klimke on 20.12.18.
//

#include "Neighbored.h"

namespace graph {

    const Neighbored::NeighborSeq & Neighbored::left() const {

        return _left;

    }


    const Neighbored::NeighborSeq & Neighbored::right() const {

        return _right;

    }


    void Neighbored::addNeighbor(double s, graph::Neighbored *obj) {

        // add left or right depending on orientation
        if(isForward())
            addLeft(s, obj, true);
        else
            addRight(length() - s - obj->length(), obj, true);

    }


    void Neighbored::addRight(double s, Neighbored *right, bool bi) {

        // set length to zero
        if(_right.empty())
            _right.length(0.0);

        // update length
        double nLen = s + right->length() - _right.startPosition();
        if (_right.empty() || nLen > _right.length())
            _right.length(nLen);

        // check if s is already set with the same neighbor (avoid double adds). If not add.
        if(!_right.exists(s, right))
            _right.push(s, right);

        // abort here if not bi-directional connected
        if(!bi)
            return;

        // if direction is backwards, switch side and recalculate position
        if (right->isForward() == isForward())
            right->addLeft(-s, this, false);
        else
            right->addRight(right->length() + s - length(), this, false);

    }


    void Neighbored::addLeft(double s, Neighbored *left, bool bi) {

        // set length to zero
        if(_left.empty())
            _left.length(0.0);

        // update length
        double nLen = s + left->length() - _left.startPosition();
        if (nLen > _left.length())
            _left.length(nLen);

        // check if s is already set with the same neighbor (avoid double adds). If not add.
        if(!_left.exists(s, left))
            _left.push(s, left);

        // abort here if not bi-directional connected
        if(!bi)
            return;

        // if direction is backwards, switch side and recalculate position
        if (left->isForward() == isForward())
            left->addRight(-s, this, false);
        else
            left->addLeft(left->length() + s - length(), this, false);

    }


    std::pair<double, const Neighbored *> Neighbored::neighbor(double s, Side side, size_t number) const {

        if (number == 0)
            return {s, this};

        const auto ns = side == Side::LEFT ? &_left : &_right;
        number--;

        // check sequence
        if (!ns->exists(s))
            return {0.0, nullptr};

        // get element
        auto tmp = ns->atPos(s);

        // get point in neighbored element
        auto sn = tmp.position;
        if (tmp.element->isForward() != isForward()) {
            sn = tmp.element->length() - sn;
            side = Neighbored::switchSide(side);
        }

        // return or recursion
        if (number == 0)
            return {sn, tmp.element};
        else
            return tmp.element->neighbor(sn, side, number);

    }


    Neighbored::Side Neighbored::switchSide(const Neighbored::Side &sd) {

        return sd == Side::LEFT ? Side::RIGHT : Side::LEFT;

    }


}