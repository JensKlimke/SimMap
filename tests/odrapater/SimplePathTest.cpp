//
// Created by Jens Klimke on 2019-05-11.
//


#include <server/Track.h>
#include <server/Path.h>
#include <gtest/gtest.h>
#include <odradapter/ODRAdapter.h>
#include <server/MapCoordinate.h>
#include <server/LaneEdge.h>
#include <ODRRoad.h>


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
        map->loadFile("./tests/tracks/example_simple.xodr");

        // get roads
        ODRRoad *rd1 = dynamic_cast<ODRRoad *>(map->_roadNetwork.at("1").get());
        edge = rd1->lanes.lane(def::ContactPoint::START, -1);

        // create track
        track = Track({{def::Orientation::FORWARDS, rd1}});

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
    EXPECT_NEAR(1.875, pos.x(), 1e-12);
    EXPECT_NEAR(0.0, pos.y(), 1e-12);

    pos = positionAt(75.0).absolutePosition().position;
    EXPECT_NEAR(3.75, pos.x(), 1e-12);
    EXPECT_NEAR(75.0, pos.y(), 1e-12);

    pos = positionAt(120.0).absolutePosition().position;
    EXPECT_NEAR(2.8125, pos.x(), 1e-12);
    EXPECT_NEAR(120.0, pos.y(), 1e-12);

    pos = positionAt(200.0).absolutePosition().position;
    EXPECT_NEAR(1.875, pos.x(), 1e-12);
    EXPECT_NEAR(200.0, pos.y(), 1e-12);

}