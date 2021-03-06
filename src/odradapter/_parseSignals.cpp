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
// Created by Jens Klimke on 2019-05-12.
//

#include <memory>
#include <odr/odr1_5_structure.h>
#include <graph/Edge.h>
#include "ODRObject.h"
#include "ODRRoad.h"
#include "ODREdge.h"

std::vector<std::shared_ptr<ODRObject>> ODRRoad::_objVector{};

void sv(ODREdge* edge, double sRoad, const ODRObject *sig) {

    // calculate distance from edge start
    auto ds = sRoad - edge->_s0;
    if(edge->isBackwards())
        ds = edge->length() - ds;

    // add object to list
    edge->_objs.emplace_back(std::pair<double, const graph::Object *>{ds, sig});

}


void parseSignals(ODRRoad *road, const odr1_5::t_road &rd) {

    if (rd.sub_signals) {

        // create index
        std::map<std::string, const ODRObject*> index{};


        // iterate over signals
        for (const auto &sig : rd.sub_signals->sub_signal) {

            // get position
            double s = *sig._s;

            // get signal data
            auto obj = std::make_shared<ODROrigObject>();
            obj->_id    = *sig._id;
            obj->_type  = "signal";
            obj->_label = *sig._type;
            obj->_value = INFINITY;

            // get value (if set)
            if (sig._value)
                obj->_value = *sig._value;

            // get orientation
            if(sig._orientation) {

                // get cross section
                auto cs = road->lanes.crossSection(s);

                if (*sig._orientation != "-")
                    for (auto &e : *cs.right())
                        sv(e, s, obj.get());

                if (*sig._orientation != "+")
                    for (auto &e : *cs.left())
                        sv(e, s, obj.get());

            } else {

                // save validity
                for (const auto &v : sig.sub_validity) {

                    // get cross section
                    auto cs = road->lanes.crossSection(s);

                    // save lanes with given id
                    for (int i = *v._fromLane; i < *v._toLane; ++i) {

                        if (cs.lane(i) != nullptr)
                            sv(cs.lane(i), s, obj.get());

                    }

                }

            }

            // add to road list
            road->_objects.emplace_back(std::pair<double, const ODRObject*>{s, obj.get()});

            // add to general list and index
            ODRRoad::_objVector.emplace_back(obj);
            index[obj->getID()] = obj.get();

        }

        // iterate over signal references TODO: move some code to function to avoid redundancy
        for (const auto &sig : rd.sub_signals->sub_signalReference) {

            // get index
            if(index.find(*sig._id) == index.end())
                continue;

            // get position
            double s = *sig._s;

            // get signal data
            auto obj = std::make_shared<ODRRefObject>();
            obj->_ref = index.at(*sig._id);


            // get orientation
            if(sig._orientation) {

                // get cross section
                auto cs = road->lanes.crossSection(s);

                if (*sig._orientation != "-")
                    for (auto &e : *cs.right())
                        sv(e, s, obj.get());

                if (*sig._orientation != "+")
                    for (auto &e : *cs.left())
                        sv(e, s, obj.get());

            } else {

                // save validity
                for (const auto &v : sig.sub_validity) {

                    // get cross section
                    auto cs = road->lanes.crossSection(s);

                    // save lanes with given id
                    for (int i = *v._fromLane; i < *v._toLane; ++i) {

                        if (cs.lane(i) != nullptr)
                            sv(cs.lane(i), s, obj.get());

                    }

                }

            }

            // add signal to vector
            road->_objects.emplace_back(std::pair<double, const ODRObject*>{s, obj.get()});

            // add object to object list
            ODRRoad::_objVector.emplace_back(obj);


        }


        // sort objects in edge
        for(auto & ln : road->lanes.lanes()) {

            // get edge
            auto edge = ln.lane;

            // sort by distance
            std::sort(edge->_objs.begin(), edge->_objs.end(),
                      [](const std::pair<double, const graph::Object*> &a, const std::pair<double, const graph::Object*> &b) -> bool
                      {
                          return a.first < b.first;
                      });

        }

    }

}