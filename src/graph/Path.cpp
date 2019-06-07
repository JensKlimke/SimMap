//
// Created by Jens Klimke on 2019-01-05.
//


#include "Path.h"
#include "Edge.h"
#include "Object.h"
#include <base/m_assert.h>
#include <tuple>

namespace graph {

    Path::Path(const Path &p) {

        // copy attributes
        this->_s = p._s;
        this->_headPos = p._headPos;
        this->_backPos = p._backPos;

        // copy segments and set iterator to first element
        this->_segments = p._segments;
        this->_it = this->_segments.begin();

        // abort if path has no elements
        if (_segments.empty())
            return;

        // set iterator
        for (auto it = p._segments.begin(); it != p._it; ++it, ++_it);

    }


    double Path::distanceToHead() const {

        return _body().back().s;

    }


    double Path::distanceToBack() const {

        return _tail().back().s;

    }


    void Path::position(double s) {

        if (s <= distanceToHead() && s >= -distanceToBack(), "wrong s");

        // get iterator
        auto r = index(s);

        // set data
        _it = r.it;
        _s = r.s;

    }


    Path::ObjectsList Path::objects() const {

        Path::ObjectsList list{};

        // create iterator and distance counter
        double ss = 0.0;
        double sPos = 0.0;

        for (auto it = _segments.begin(); it != _segments.end(); ++it) {

            // get edge
            auto edge = *it;

            // get interval in edge
            double s0 = it == _segments.begin() ? _backPos : 0.0;
            double s1 = it == _head().it ? _headPos : edge->length();

            // if edge is the position edge, save the distance from the start to the position
            if (it == _it)
                sPos = ss + _s - s0;

            // get objects
            auto objs = edge->objects();
            for (auto obj : objs) {

                // get s relative to edge and check if s is in interval
                auto s = obj.first;
                if (s < s0 || s > s1)
                    continue;

                // get distance from position and add to list
                list.push_back(std::pair<double, const Object *>{ss + s - s0, obj.second});

            }

            // update
            ss += s1 - s0;

        }

        // update position
        for (auto &el : list)
            el.first -= sPos;

        return list;

    }


    void Path::_set(size_t i, double s, double back, double head) {

        _it = std::next(_segments.begin(), i);
        _s = s;
        _backPos = back;
        _headPos = head;

    }


    Path::position_t Path::index(double s) const {

        if (s == 0.0)
            return _position();
        else if (s < 0.0)
            return {index_bw(s), s};
        else
            return {index_fw(s), s};

    }


    Path::iterator_t Path::index_fw(double &s) const {

        // check if s is positive
        m_assert(s >= 0.0, "s must be positive");

        // get body part until s is reached
        auto r = _body(s);

        // check result (resultant position must be equal to search position)
        auto sm = r.back().s;
        m_assert(fabs(sm - s) < def::EPS_DOUBLE_CMP, "Wrong s");

        // update s
        if (r.size() > 1)
            s = sm - std::next(r.begin(), r.size() - 2)->s;
        else
            s = sm + _s;

        // return iterator
        return r.back().it;

    }


    Path::iterator_t Path::index_bw(double &s) const {

        // check if s is positive
        m_assert(s <= 0.0, "s must be negative");

        // get tail part until s is reached
        auto r = _tail(-s);

        // check result (resultant position must be equal to search position)
        auto sm = r.back().s;
        m_assert(fabs(sm + s) < def::EPS_DOUBLE_CMP, "Wrong s");

        // update s
        if (r.size() > 1) {
            auto it = std::next(r.begin(), r.size() - 2);
            s = (*std::next(it, 1)->it)->length() + it->s - sm;
        } else {
            s = _s - sm;
        }

        // return iterator
        return r.back().it;

    }


    Path::position_t Path::_position() const {

        return {_it, _s};

    }


    Path::position_t Path::_head() const {

        return {std::next(_segments.begin(), _segments.size() - 1), _headPos};

    }


    Path::position_t Path::_back() const {

        return {_segments.begin(), _backPos};

    }


    Path::position_vector_t Path::_body(double stopAt) const {

        // return container
        position_vector_t ret{};
        ret.reserve(_segments.size());

        // throw error
        if (_segments.empty())
            throw std::runtime_error("No segments");

        // initialize length
        double len = -_s;

        // sum up all segments
        for (auto it = _it; it != _segments.end(); ++it) {

            // update length and add iterator and length to vector
            len += (*it)->length();
            ret.push_back({it, len});

            // stop if stopAt is reached
            if (len >= stopAt) {

                // remove distance from end and return
                ret.back().s -= len - stopAt;
                return ret;

            }

        }

        // remove distance from end and return
        ret.back().s -= (*ret.back().it)->length() - _headPos;
        return ret;

    }


    Path::position_vector_t Path::_tail(double stopAt) const {

        // return container
        position_vector_t ret{};
        ret.reserve(_segments.size());

        // throw error
        if (_segments.empty())
            throw std::runtime_error("No segments");

        // initialize length
        double len = -((*_it)->length() - _s);

        // get reverse iterator (but take the next one, because of the
        // iterator shift (see https://en.cppreference.com/w/cpp/iterator/reverse_iterator)
        auto it = std::make_reverse_iterator(std::next(_it, 1));

        // sum up all segments
        for (; it != _segments.rend(); it++) {

            // update length and add iterator and length to vector
            // save forward iterator (shift because see above)
            len += (*it)->length();
            ret.push_back({std::next(it, 1).base(), len});

            // stop if stopAt is reached
            if (len >= stopAt) {

                // remove distance from end and return
                ret.back().s -= len - stopAt;
                return ret;

            }

        }

        // remove length of last element
        ret.back().s -= _backPos;
        return ret;

    }


    std::ostream &Path::streamTo(std::ostream &os) const {

        size_t n = 0;

        // iterate over edges
        for (auto r : _segments)
            os << (n++ == 0 ? "" : " > ") << r->id();

        return os;


    }

}


std::ostream & operator<<(std::ostream &os, const graph::Path& p) {

    return p.streamTo(os);

}
