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
// Created by Jens Klimke on 2019-06-06.
//

#include <gtest/gtest.h>
#include <string>
#include <simmap/simmap.h>
#include <base/functions.h>

class SpeedTest : public testing::Test {


    void SetUp() override {}

    void TearDown() override { simmap::clear(); }

};



TEST_F(SpeedTest, Targets) {

    // define number of targets and number of steps
    unsigned int n = 100;
    unsigned int m = 100;

    // load map
    unsigned long id;
    simmap::loadMap(base::string_format("%s/CircleR100.xodr", TRACKS_DIR).c_str(), id);

    // define track
    std::vector<const char *> track = {"1", "-2"};

    // loop to create agents
    for (auto i = 0; i < n; ++i) {

        // register agent
        unsigned int aid = i + 1;
        simmap::registerAgent(aid, id);

        // set track
        simmap::setTrack(aid, track.data(), track.size());

        // define position
        simmap::MapPosition pos;
        pos.edgeID  = "R1-LS1-R1";
        pos.latPos  = 0.0;
        pos.longPos = 0.0; // i * 20.0;

        // define path lengths
        double frLen = 200.0;
        double bkLen =  50.0;

        // set map position
        simmap::setMapPosition(aid, pos, frLen, bkLen);

    }


    // loop to move targets
    for(auto i = 0; i < m; ++i) {

        // iterate over agents
        for(auto j = 0; j < n; ++j) {

            // get id
            unsigned int aid = j + 1;

            // set number of targets
            unsigned long nt = 32;

            // get target information
            std::vector<simmap::TargetInformation> ti(nt);
            simmap::targets(aid, ti.data(), nt);

            // define path lengths
            double frLen = 200.0;
            double bkLen =  50.0;

            // move agent
            simmap::move(aid, 5.0, 0.0, frLen, bkLen);

        }

    }


}