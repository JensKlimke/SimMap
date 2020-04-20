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
// Created by Jens Klimke on 2018/10/17
//

#include "Path.h"
#include "MapCoordinate.h"
#include "Track.h"

#include <algorithm>
#include <list>
#include <vector>

#include <base/functions.h>


namespace simmap {
namespace server {


base::CurvePoint Path::Matcher::operator()(double s, double &d) const
{
    // calculate position
    auto pos = path->positionAt(s).absolutePosition();

    // calculate result
    base::Vector3 diff = pos.position - *xyz;
    d = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

    return pos;

}

base::Vector3 Path::Matcher::bounds() const {

    return {-path->distanceToBack(), path->distanceToHead(), 0.0};

}



void Path::create(Path &path, Track &track, double lenHead, double lenBack, const MapCoordinate &position) {

    if (lenBack < 0.0 || lenHead < 0.0)
        throw std::invalid_argument("Lengths must be larger than zero");

    // empty segments and add position edge
    path._segments.clear();
    path._segments.push_back(position.edge());

    // create first segment and set positions
    path._it = path._segments.begin();
    path._s        = position.s();
    path._d        = position.d();
    path._headPos  = position.s();
    path._backPos  = position.s();

    // update path
    path.updatePath(lenHead, lenBack, track);

}



void Path::updatePath(double lenHead, double lenBack, Track &track) {

    if (lenHead < 0.0 && lenBack < 0.0)
        throw std::invalid_argument("length must not be negative");

    // update track
    updateTrack(position(), track);

    // reset segments list
    _segments = {*(_it)};
    _it = _segments.begin();

    // extend path in forward direction
    size_t tin = 0;
    extendPath(lenHead, track, tin);

    // extend path in backward direction
    tin = 0;
    extendPath(-lenBack, track, tin);

}



MapCoordinate Path::position() const {

    return MapCoordinate{dynamic_cast<const LaneEdge*>(*_it), _s, _d};

}


void Path::position(double s, double d) {

    ::graph::Path::position(s);
    _d = d;

}



MapCoordinate Path::head() const {

    return MapCoordinate(dynamic_cast<const LaneEdge*>(*_head().it), _head().s, 0.0);

}


MapCoordinate Path::back() const {

    return MapCoordinate(dynamic_cast<const LaneEdge*>(*_back().it), _back().s, 0.0);

}


MapCoordinate Path::positionAt(double s, double d) const {

    auto r = index(s);
    return MapCoordinate(dynamic_cast<const LaneEdge*>(*r.it), r.s, d);

}



Track Path::track() const {

    // create track
    Track tr{};
    Track::TrackElement ro = {base::Orientation::FORWARDS, nullptr};

    // iterate over edges
    for(const auto &edge : _segments) {

        // get road and direction of lane
        const auto trElem = dynamic_cast<const LaneEdge*>(edge)->trackElement();

        // add track element
        if(ro != trElem)
            tr.push_back(trElem);

        // store road and contact point
        ro = trElem;

    }

    return tr;

}


bool Path::hasRoad(const Track::TrackElement &road) const {

    // iterate over edges
    for(const auto &edge : _segments) {

        // check if road and orientation fit
        if(dynamic_cast<const LaneEdge*>(edge)->trackElement() == road)
            return true;

    }

    return false;

}



void Path::updateTrack(const MapCoordinate &coordinate, Track &track) {

    // get index
    auto i = findTrackElement(coordinate, track);

    // return if no rotation necessary
    if(i == track.size() || i == 0)
        return;

    // get end iterator
    auto endIt = track.begin();
    std::advance(endIt, i);

    // rotate
    std::rotate(track.begin(), endIt, track.end());

}



size_t Path::findTrackElement(const MapCoordinate &coordinate, const Track &track) {

    // get road and orientation of road
    auto rd = coordinate.edge()->trackElement();

    size_t i = 0;
    for(const auto t : track) {

        // segment is equal to i-th element of track ...
        if (t == rd)
            break;

        // increment
        i++;

    }

    return i;

}


void Path::extendPath(double ds, const Track &track, size_t &trackIndex) {

    if(ds == 0.0)
        return;

    // get flag
    auto forward = ds > 0.0;

    // initialize variables
    const ::graph::Oriented::Connections* con;
    size_t tin;

    // remove current edge length
    if(forward)
        ds -= (*_it)->length() - _s;
    else
        ds += _s;


    // do until ds length is reached
    while((forward && ds > 0.0) || (!forward && ds < 0.0)) {

        // get successors or predecessors
        if (forward) {

            con = &_segments.back()->nexts();
            tin = (trackIndex + 1) % track.size();

        } else {

            con = &_segments.front()->prevs();
            tin = (trackIndex - 1 + track.size()) % track.size();

        }

        // get track iterators
        auto itc = std::next(track.begin(), trackIndex);
        auto itn = std::next(track.begin(), tin);

        // check which connection fits
        auto add = false;
        for (const auto &c : *con) {

            auto edge = dynamic_cast<LaneEdge *>(c.second);

            // check if same road
            // TODO: better way with operator==?
            if (edge->trackElement().second == itc->second && (edge->orientation() == base::Orientation::NONE
                || (edge->isForward() == (itc->first == base::Orientation::FORWARDS)))) {

                add = true;

            } else if (edge->trackElement().second == itn->second && (edge->orientation() == base::Orientation::NONE
                || (edge->isForward() == (itn->first == base::Orientation::FORWARDS)))) {

                add = true;
                trackIndex = tin;

            }

            // add edge to path
            if (add && forward) {

                _segments.push_back(edge);
                ds -= edge->length();

                // stop loop
                break;

            } else if (add) {

                _segments.push_front(edge);
                ds += edge->length();

                // stop loop
                break;

            }

        }

        // abort if no connection could be found
        if(!add) {
            ds = 0.0;
            break;
        }


    }

    // set head and back
    if(forward)
        _headPos = _segments.back()->length() + ds;
    else
        _backPos = ds;

}


void Path::createRecursively(std::list<Path*> &paths, const LaneEdge *edge, double len) {

    // path and tracks
    auto p = new Path;
    auto tr = Track{};

    // create path
    create(*p, tr, 0.0, 0.0, {edge, 0.0, 0.0});

    // add path to vector
    paths.push_back(p);

    // recursion
    extendRecursively(paths, len);

}


double Path::match(const base::Vector3 &xyz, double &s, double radius) const {

    Matcher matcher{};
    matcher.path = this;
    matcher.xyz = &xyz;

    // get bounds
    auto b = matcher.bounds();

    // get bounds
    double b0 = b.x + 1e-12;
    double b1 = b.y - 1e-12;

    // set radius at least to a minimum
    radius = fmax(0.1, std::abs(radius));

    // calculate minimum on discrete steps
    auto ss = base::maxspace(fmax(s - radius, b0), fmin(s + radius, b1), 10.0);
    double erro = INFINITY;

    for(size_t i = 0; i < ss.size(); ++i) {

        double err = INFINITY;
        matcher.operator()(ss[i], err);

        if(err < erro) {
            s = ss[i];
            erro = err;
        }

    }

    return matchExec(&matcher, &s, base::EPS_DISTANCE, base::EPS_DISTANCE, 1000);

}


std::list<Path::Neighbor> Path::neighboredPaths(Track &track) const {

    // create container
    std::list<Neighbor> ret{};

    // get current position, set d to zero
    auto pos = position();
    pos.d(0.0);

    // copy track
    auto revTrack = track;
    revTrack.reverseTrack();

    // pre-calculate stuff
    double dh = distanceToHead();
    double db = distanceToBack();

    // get left lane and direction
    auto dir = pos.edge()->isForward();
    auto mc = pos.left();

    // get left paths
    size_t i = 1;
    while(!mc.outOfRoad()) {

        // create path
        int j = (int)i; i++;

        // create path and info
        Path p{};
        NeighborInformation info{};

        auto diff = mc.absolutePosition().position - pos.absolutePosition().position;

        // save information
        info.index      = j;
        info.sameDir    = dir == mc.edge()->isForward();
        info.accessible = true; // TODO
        info.allowed    = true; // TODO
        info.offset     = sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);

        // add to list
        ret.emplace_back(info, p);

        // when direction changes
        if(info.sameDir) {

            Path::create(ret.back().second, track, dh, db, mc);
            mc = mc.left();

        } else {

            Path::create(ret.back().second, revTrack, db, dh, mc);
            mc = mc.right();

        }

    }

    // reverse list
    ret.reverse();

    // reset values
    mc = pos.right();
    dir = pos.edge()->isForward();

    // get right pathes
    i = 1;
    while(!mc.outOfRoad()) {

        // calculate index
        auto j = -1 * (int)i; i++;

        // create path and info
        Path p{};
        NeighborInformation info{};

        auto diff = mc.absolutePosition().position - pos.absolutePosition().position;

        // save information
        info.index      = j;
        info.sameDir    = dir == mc.edge()->isForward();
        info.accessible = true;
        info.allowed    = true;
        info.offset     = sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);

        // add to list
        ret.emplace_back(info, p);

        // when direction changes
        if(info.sameDir) {

            Path::create(ret.back().second, track, dh, db, mc);
            mc = mc.right();

        } else {

            Path::create(ret.back().second, revTrack, db, dh, mc);
            mc = mc.left();

        }

    }

    return ret;


}


std::vector<double> Path::distance(const MapCoordinate &mc) const {

    // create container
    std::vector<double> ret{};
    ret.reserve(_segments.size());

    // initialize vars and get body
    double s0 = -position().s();
    auto bd = _body();

    // iterate over body elements
    for(const auto &e : bd) {

        // check edge
        auto edge = *e.it;
        if(edge == mc.edge()) {

            // get resulting distance
            double dsMC = mc.s() + s0;

            // save position if within range
            if(dsMC >= 0.0 && dsMC <= e.s)
                ret.emplace_back(dsMC);

        }

        // update s0
        s0 = e.s;

    }



    // initialize vars and get tail
    s0 = position().s() - position().edge()->length();
    auto tl = _tail();

    // iterate over body elements
    for(const auto &e : tl) {

        // check edge
        auto edge = *e.it;

        // check for same edge
        if(edge == mc.edge()) {

            // get resulting distance
            double dsMC = mc.edge()->length() + s0 - mc.s();

            // save position if within range
            if(dsMC >= 0.0 && dsMC <= e.s)
                ret.emplace_back(-dsMC);

        }

        // update s0
        s0 = e.s;

    }


    return ret;

}


double Path::matchExec(const Path::Matcher *matcher, double *s, double s_eps, double f_eps, size_t max_f) {

    double err = INFINITY;
    double delta_s = INFINITY;

    // get boundaries
    auto bnds = matcher->bounds();
    size_t i = 0;

    while(err > f_eps && std::abs(delta_s) > s_eps) {

        // calculate position and error
        auto pos = matcher->operator()(*s, err);

        auto loc = base::toLocal(pos, *matcher->xyz);
        delta_s = loc.x;

        // calculate delta s
        *s += delta_s;

        // apply boundaries
        *s = fmax(*s, bnds.x);
        *s = fmin(*s, bnds.y);

        // check number of evaluations
        if(++i >= max_f)
            break;

    }

    return err;

}


void Path::extendRecursively(std::list<Path *> &paths, double len) {

    if(len <= 0.0)
        return;

    // get last element
    auto path = paths.back();

    // get connections
    auto conPtr = &path->_segments.back()->nexts();

    // check if empty (then abort)
    if(conPtr->empty())
        return;

    // reinterpret to LaneEdge vector
    auto next = reinterpret_cast<const std::vector<std::pair<base::ContactPoint, LaneEdge*>>*>(conPtr);

    // remove element from vector
    paths.pop_back();

    // iterate over successors
    bool any = false;
    for(const auto edge : *next) {

        // get edge
        auto e = edge.second;

        /*
         * TODO:
         * // check if edge is a start edge and the same road as first edge
         * if(e->startOfRoad() && path->hasRoad(e->trackElement()))
         *    continue;
         */

        // copy path and add edge
        auto pn = new Path(*path);
        pn->_segments.push_back(e);

        // add path to vector
        paths.push_back(pn);

        // recursion
        extendRecursively(paths, len - e->length());

        // set flag
        any = true;

    }

    // if path was continued, delete, if not safe back to vector (end of path)
    if(any)
        delete path;
    else
        paths.push_back(path);

}

}
}



