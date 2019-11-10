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
// Created by Jens Klimke on 19.05.2019.
//

#include "ODREdge.h"
#include <odr/odr1_5_structure.h>
#include <vector>

void parseRoadMarking(ODREdge *edge, std::vector<odr1_5::t_road_lanes_laneSection_lcr_lane_roadMark> &def) {

    /*
    // parse road markings
    auto rm = edge->_roadMarking;
    for(const auto &rmd : def) {

        // get s in lane
        auto sl = *rmd._sOffset;
        auto wd = rmd._width ? *rmd._width : 30.0;

        // explicitly defined line
        if(rmd.sub_explicit) {

            for(const auto &ln : rmd.sub_explicit->sub_line) {

                ExplicitRoadMarking expl{};

                // check width, lateral offset
                expl.width = ln._width ? *ln._width : wd;
                expl.latOffset = ln._tOffset ? *ln._tOffset : 0.0;

                // check length
                if(ln._length)
                    expl.lineLength = *ln._length;

                // check rule
                if(ln._rule)
                    expl.rule = *ln._rule == "none" ? LineCrossingRule::ALLOWED
                                                    : (*ln._rule == "no allowed" ? LineCrossingRule::NOT_ALLOWED : LineCrossingRule::CAUTION);

                // save explicit road marking
                rm.emplace(sl + *ln._sOffset, std::move(expl));

            }

        } else if(rmd.sub_type) {

            // get width
            double width = rmd.sub_type->_width ? *rmd.sub_type->_width : wd;

            // iterate over lines
            for(const auto& ln : rmd.sub_type->sub_line) {

                ExplicitRoadMarking expl{};

                // check width, lateral offset
                expl.width = ln._width ? *ln._width : width;
                expl.latOffset = ln._tOffset ? *ln._tOffset : 0.0;

                // check length
                if(ln._length)
                    expl.lineLength = *ln._length;

                // check rule
                if(ln._rule)
                    expl.rule = *ln._rule == "none" ? LineCrossingRule::ALLOWED
                                                    : (*ln._rule == "no allowed" ? LineCrossingRule::NOT_ALLOWED : LineCrossingRule::CAUTION);

                // TODO: add each line here

                // save explicit road marking
                rm.emplace(sl + *ln._sOffset, std::move(expl));

            }

        } else {

            ExplicitRoadMarking expl{};

            // width
            expl.width = rmd._width ? *rmd._width : wd;

            // type
            std::string bs;
            if(rmd._type) {

                // set type
                bs = *rmd._type;

                // check types
                if (bs == "broken")
                    expl.rule = LineCrossingRule::ALLOWED;
                else if (bs == "broken broken")
                    expl.rule = LineCrossingRule::CAUTION;
                else if (bs == "solid broken")
                    expl.rule = LineCrossingRule::NOT_ALLOWED;
                else if (bs == "broken solid")
                    expl.rule = LineCrossingRule::ALLOWED;
                else if (bs == "botts dots")
                    expl.rule = LineCrossingRule::CAUTION;

            }

            // save explicit road marking
            rm.emplace(sl, std::move(expl));

        }

    }

     */

}