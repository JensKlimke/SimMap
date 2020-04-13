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
// Created by Jens Klimke on 2019-05-11.
//


#include <server/Track.h>
#include <server/Path.h>
#include <gtest/gtest.h>
#include <odradapter/ODRAdapter.h>
#include <server/MapCoordinate.h>
#include <server/LaneEdge.h>
#include <odradapter/ODRRoad.h>
#include <base/functions.h>


class SimplePathTest : public testing::Test, public simmap::server::Path {

public:

    simmap::odra::ODRAdapter *map = nullptr;
    simmap::server::Track track{};

    ODREdge *edge = nullptr;


    void SetUp() override {

        using namespace simmap::odra;
        using namespace simmap::server;

        // create map and segment
        map = new ODRAdapter;
        map->loadFile(base::string_format("%s/example_simple.xodr", TRACKS_DIR));

        // get roads
        ODRRoad *rd1 = dynamic_cast<ODRRoad *>(map->_roadNetwork.at("1").get());
        edge = rd1->lanes.lane(base::ContactPoint::START, -1);

        // create track
        track = Track({{base::Orientation::FORWARDS, rd1}});

        MapCoordinate mc(edge, 0.0, 0.0);
        Path::create(*this, track, 200.0, 0.0, mc);

    }

    void TearDown() override {

        delete map;

    }

};


TEST_F(SimplePathTest, Linkage) {

    using namespace simmap::server;

    EXPECT_EQ(1, edge->nexts().size());

    // get edges
    auto edge2 = dynamic_cast<LaneEdge *>(edge->nexts().front().second);
    EXPECT_EQ(1, edge2->nexts().size());

    auto edge3 = dynamic_cast<LaneEdge *>(edge->nexts().front().second);
    EXPECT_EQ(1, edge3->nexts().size());

    // check roads
    auto rd = edge->trackElement();
    EXPECT_NE(nullptr, rd.second);
    EXPECT_EQ(rd, edge2->trackElement());
    EXPECT_EQ(rd, edge3->trackElement());

}


TEST_F(SimplePathTest, Positions) {

    EXPECT_DOUBLE_EQ(200.0, distanceToHead());

    auto pos = positionAt(0.0).absolutePosition().position;
    EXPECT_NEAR(1.875, pos.x, 1e-12);
    EXPECT_NEAR(0.0, pos.y, 1e-12);

    pos = positionAt(75.0).absolutePosition().position;
    EXPECT_NEAR(3.75, pos.x, 1e-12);
    EXPECT_NEAR(75.0, pos.y, 1e-12);

    pos = positionAt(120.0).absolutePosition().position;
    EXPECT_NEAR(2.8125, pos.x, 1e-12);
    EXPECT_NEAR(120.0, pos.y, 1e-12);

    pos = positionAt(200.0).absolutePosition().position;
    EXPECT_NEAR(1.875, pos.x, 1e-12);
    EXPECT_NEAR(200.0, pos.y, 1e-12);

}