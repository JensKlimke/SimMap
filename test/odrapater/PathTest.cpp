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

#include <base/definitions.h>
#include <base/functions.h>
#include <server/Track.h>
#include <server/Path.h>
#include <gtest/gtest.h>
#include <odradapter/ODRAdapter.h>
#include <server/MapCoordinate.h>
#include <server/LaneEdge.h>
#include <odradapter/ODRRoad.h>
#include <odradapter/ODRObject.h>


class PathTest : public testing::Test, public simmap::server::Path {


public:

    const double R = 100.0;
    const double W = 3.75;

    simmap::server::Map *map = nullptr;
    simmap::server::Track track{};

    // get edges
    ODREdge *e_R1_LS1_R1 = nullptr;
    ODREdge *e_R2_LS1_L1 = nullptr;

    ODRRoad *rd1 = nullptr;
    ODRRoad *rd2 = nullptr;


    void SetUp() override {

        using namespace simmap::odra;
        using namespace simmap::server;

        // create map and segment
        auto mp = new ODRAdapter;
        mp->loadFile(base::string_format("%s/CircleR100.xodr", TRACKS_DIR));

        // get roads
        rd1 = dynamic_cast<ODRRoad *>(mp->_roadNetwork.at("1").get());
        rd2 = dynamic_cast<ODRRoad *>(mp->_roadNetwork.at("2").get());

        // create track
        track = Track({{base::Orientation::FORWARDS,  rd1},
                       {base::Orientation::BACKWARDS, rd2}});

        // get edges
        e_R1_LS1_R1 = dynamic_cast<ODREdge *>(rd1->lanes.lane(base::ContactPoint::START, -1));
        e_R2_LS1_L1 = dynamic_cast<ODREdge *>(rd2->lanes.lane(base::ContactPoint::START, 1));

        // save map
        this->map = mp;

    }

    void TearDown() override {

        delete map;

    }


    void createPath(double len, double s0) {

        if (fabs(s0) > len * 0.5 || len < 0.0)
            throw std::invalid_argument("abs(s0) must be smaller than len * 0.5 and len must be larger than zero");

        // map coordinate for path start
        simmap::server::MapCoordinate mc(e_R1_LS1_R1, 0.0, 0.0);

        // create and set position
        Path::create(*this, track, len * 0.5, len * 0.5, mc);
        position(s0, 0.0);

    }

};


TEST_F(PathTest, createPath) {

    // first test
    createPath(20.0, 0.0);

    // check lengths
    EXPECT_DOUBLE_EQ(10.0, this->distanceToHead());
    EXPECT_DOUBLE_EQ(10.0, this->distanceToBack());
    EXPECT_EQ(2, this->_segments.size());
    EXPECT_EQ(e_R2_LS1_L1, this->_segments.front());
    EXPECT_EQ(e_R1_LS1_R1, this->_segments.back());


    // second test
    createPath(20.0, 10.0);

    // check lengths
    EXPECT_DOUBLE_EQ(0.0, this->distanceToHead());
    EXPECT_DOUBLE_EQ(20.0, this->distanceToBack());
    EXPECT_EQ(2, this->_segments.size());
    EXPECT_EQ(e_R2_LS1_L1, this->_segments.front());
    EXPECT_EQ(e_R1_LS1_R1, this->_segments.back());


    // third test
    createPath(20.0, -10.0);

    // check lengths
    EXPECT_DOUBLE_EQ(20.0, this->distanceToHead());
    EXPECT_DOUBLE_EQ(0.0, this->distanceToBack());
    EXPECT_EQ(2, this->_segments.size());
    EXPECT_EQ(e_R2_LS1_L1, this->_segments.front());
    EXPECT_EQ(e_R1_LS1_R1, this->_segments.back());


    // fourth test
    createPath(2000.0, 10.0);

    // check lengths
    EXPECT_DOUBLE_EQ(990.0, this->distanceToHead());
    EXPECT_DOUBLE_EQ(1010.0, this->distanceToBack());


}

TEST_F(PathTest, Function) {

    // first test
    createPath(20.0, 0.0);

    // get head and back
    auto hd = head();
    auto bk = back();

    // find head
    auto ds = 10.0;
    auto pos = positionAt(ds);
    EXPECT_DOUBLE_EQ(hd.s(), pos.s());
    EXPECT_EQ(hd.edge(), pos.edge());

    // find back
    ds = -10.0;
    pos = positionAt(ds);
    EXPECT_DOUBLE_EQ(bk.s(), pos.s());
    EXPECT_EQ(bk.edge(), pos.edge());

//    // reset position
//    EXPECT_EXIT(position( 20.0), ::testing::KilledBySignal(SIGABRT), ".*"); // error
//    EXPECT_EXIT(position(-20.0), ::testing::KilledBySignal(SIGABRT), ".*"); // error

    // check path
    position(5.0);
    EXPECT_EQ(5.0, position().s());
    EXPECT_EQ(e_R1_LS1_R1, position().edge());

    // check distances
    EXPECT_DOUBLE_EQ(5.0, distanceToHead());
    EXPECT_DOUBLE_EQ(15.0, distanceToBack());

    // check path
    position(-10.0);
    EXPECT_EQ(M_PI * R - 5.0, position().s());
    EXPECT_EQ(e_R2_LS1_L1, position().edge());

    // check distances
    EXPECT_DOUBLE_EQ(15.0, distanceToHead());
    EXPECT_DOUBLE_EQ(5.0, distanceToBack());

    // check path
    position(5.001);
    EXPECT_NEAR(0.001, position().s(), 1e-12);
    EXPECT_EQ(e_R1_LS1_R1, position().edge());

    // check distances
    EXPECT_NEAR(9.999, distanceToHead(), 1e-12);
    EXPECT_NEAR(10.001, distanceToBack(), 1e-12);

}


TEST_F(PathTest, Positions) {

    // create path
    createPath(100.0, 0.0);
    EXPECT_DOUBLE_EQ(50.0, distanceToHead());
    EXPECT_DOUBLE_EQ(50.0, distanceToBack());

    // move position
    position(25.0);
    EXPECT_DOUBLE_EQ(25.0, distanceToHead());
    EXPECT_DOUBLE_EQ(75.0, distanceToBack());

    // check positionAt with 0.0
    auto pos1 = this->position().absolutePosition();
    auto pos2 = this->positionAt(0.0).absolutePosition();
    EXPECT_DOUBLE_EQ(pos1.position.x, pos2.position.x);
    EXPECT_DOUBLE_EQ(pos1.position.y, pos2.position.y);
    EXPECT_DOUBLE_EQ(pos1.angle, pos2.angle);

    // check positionAt with head position and back position
    auto pos = this->positionAt(25.0).absolutePosition();
    EXPECT_DOUBLE_EQ(89.403723492581704, pos.position.x);
    EXPECT_DOUBLE_EQ(48.841476745303154, pos.position.y);

    pos = this->positionAt(-75.0).absolutePosition();
    EXPECT_DOUBLE_EQ(89.403723492581704, pos.position.x);
    EXPECT_DOUBLE_EQ(-48.841476745303154, pos.position.y);

    // check positionAt with head position and back position
    pos = this->positionAt(25.0, 1.0).absolutePosition();
    EXPECT_DOUBLE_EQ(88.526140930691327, pos.position.x);
    EXPECT_DOUBLE_EQ(48.362051206698951, pos.position.y);

    pos = this->positionAt(-75.0, 1.0).absolutePosition();
    EXPECT_DOUBLE_EQ(88.526140930691327, pos.position.x);
    EXPECT_DOUBLE_EQ(-48.362051206698951, pos.position.y);

    // check positionAt with head position and back position
    pos = this->positionAt(25.0, -1.0).absolutePosition();
    EXPECT_DOUBLE_EQ(90.281306054472083, pos.position.x);
    EXPECT_DOUBLE_EQ(49.320902283907358, pos.position.y);

    pos = this->positionAt(-75.0, -1.0).absolutePosition();
    EXPECT_DOUBLE_EQ(90.281306054472083, pos.position.x);
    EXPECT_DOUBLE_EQ(-49.320902283907358, pos.position.y);

}

TEST_F(PathTest, ObjectsInRoadAndEdge) {

    EXPECT_EQ(3, rd1->objects().size());

    EXPECT_DOUBLE_EQ(100, rd1->objects().at(0).first);
    EXPECT_DOUBLE_EQ(110, rd1->objects().at(1).first);
    EXPECT_DOUBLE_EQ(200, rd1->objects().at(2).first);

    EXPECT_EQ(2, e_R1_LS1_R1->objects().size());

}


TEST_F(PathTest, ObjectsInPath) {

    // all signals
    double len = R * 2.0 * M_PI;

    // no signal included
    createPath(10.0, 0.0);
    auto l = this->objects();
    EXPECT_EQ(0, l.size());

    createPath(len, 0.0);
    l = this->objects();
    ASSERT_EQ(4, l.size());
    EXPECT_DOUBLE_EQ(-10.0, l[0].first);
    EXPECT_EQ("206", l[0].second->getLabel());
    EXPECT_DOUBLE_EQ(100.0, l[1].first);
    EXPECT_EQ("274-50", l[1].second->getLabel());
    EXPECT_DOUBLE_EQ(110.0, l[2].first);
    EXPECT_EQ("276", l[2].second->getLabel());
    EXPECT_DOUBLE_EQ(200.0, l[3].first);
    EXPECT_EQ("274-70", l[3].second->getLabel());

}


TEST_F(PathTest, NeighboredPaths) {

    // all signals
    double len = R * 2.0 * M_PI;

    // no signal included
    createPath(len, 0.0);
    auto np = this->neighboredPaths(track);
    ASSERT_EQ(3, np.size());

    // calculate distance
    double dh = distanceToHead();
    double db = distanceToBack();

    // check lengths
    EXPECT_DOUBLE_EQ(len * 0.5, dh);
    EXPECT_DOUBLE_EQ(len * 0.5, db);

    // check results
    auto it = np.begin();
    EXPECT_EQ(2, it->first.index);
    EXPECT_DOUBLE_EQ(dh, it->second.distanceToHead());
    EXPECT_DOUBLE_EQ(db, it->second.distanceToBack());

    it++;
    EXPECT_EQ(1, it->first.index);
    EXPECT_DOUBLE_EQ(dh, it->second.distanceToHead());
    EXPECT_DOUBLE_EQ(db, it->second.distanceToBack());

    it++;
    EXPECT_EQ(-1, it->first.index);
    EXPECT_DOUBLE_EQ(dh, it->second.distanceToHead());
    EXPECT_DOUBLE_EQ(db, it->second.distanceToBack());


}


TEST_F(PathTest, PositionInPath) {

    // number of steps
    size_t n = 200;

    // calculate angle
    auto dPhi = M_PI * 4.0 / (n - 1);
    auto phi = -2.0 * M_PI;

    // create path
    createPath(4.1 * M_PI * R, 0.0);

    // create variable to store previous position
    auto prev = positionAt((-2.0 * M_PI - dPhi) * R).absolutePosition().position;

    // calculate reference distance
    double ref = (R + 0.5 * W) * sqrt((1.0 - cos(dPhi)) * (1.0 - cos(dPhi)) + sin(dPhi) * sin(dPhi));

    // loop
    for (size_t i = 0; i < n; ++i) {

        // calculate map position
        double s = phi * R;
        auto mp = positionAt(s);
        EXPECT_LE(mp.s(), mp.edge()->length());
        EXPECT_GE(mp.s(), 0.0);

        if (sin(phi) >= 0.0)
            EXPECT_EQ("1", mp.edge()->trackElement().second->id());
        else
            EXPECT_EQ("2", mp.edge()->trackElement().second->id());

        // get absolute position
        auto pos = mp.absolutePosition();

        // calculate x and y
        double x = cos(phi) * (R + W * 0.5);
        double y = sin(phi) * (R + W * 0.5);
        double p = base::angleDiff(phi + 0.5 * M_PI, pos.angle);

        // calculate distance
        auto diff = pos.position - prev;
        auto norm = sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);

        // check x and y
        EXPECT_NEAR(x, pos.position.x, 1e-9);
        EXPECT_NEAR(y, pos.position.y, 1e-9);
        EXPECT_NEAR(0.0, p, 1e-9);

        // check step size
        EXPECT_NEAR(ref, norm, 1e-9);

        prev = pos.position;
        phi += dPhi;

    }

}


TEST_F(PathTest, UpdatePosition) {

    // number of steps
    size_t n = 100;

    // calculate angle
    auto dPhi = M_PI * 2.0 / (n - 1);
    auto phi = -M_PI;

    // create path
    createPath(4.1 * M_PI * R, 0.0);
    position(phi * R);

    // loop
    double ds = dPhi * R;
    for (size_t i = 0; i < n; ++i) {

        // calculate map position
        auto pos = position().absolutePosition();

        // calculate x and y
        double x = cos(phi) * (R + W * 0.5);
        double y = sin(phi) * (R + W * 0.5);
        double dp = base::angleDiff(phi + 0.5 * M_PI, pos.angle);

        // check x and y
        EXPECT_NEAR(x, pos.position.x, 1e-9);
        EXPECT_NEAR(y, pos.position.y, 1e-9);
        EXPECT_NEAR(0.0, dp, 1e-9);

        phi += dPhi;

        // update position
        position(ds);

    }

}


TEST(PathMapTest, createPath) {

    using namespace simmap::server;

    // create map and segment
    auto map = new simmap::odra::ODRAdapter;
    map->loadFile(base::string_format("%s/map.xodr", TRACKS_DIR));

    // create start position
    auto *lane = map->getEdge("R4-LS1-L4");
    MapCoordinate start(lane, 0.0, 0.0);

    // create track
    Track track;
    track.push_back({base::Orientation::BACKWARDS, map->_roadNetwork.at("2").get()});
    track.push_back({base::Orientation::BACKWARDS, map->_roadNetwork.at("4").get()});
    track.push_back({base::Orientation::FORWARDS,  map->_roadNetwork.at("18").get()});

    // create path
    Path path;
    Path::create(path, track, 0.0, 0.0, start);

    // test if track was updated
    auto it = track.begin();
    EXPECT_EQ(map->_roadNetwork.at("4").get(),  it->second);
    EXPECT_EQ(map->_roadNetwork.at("18").get(), (++it)->second);
    EXPECT_EQ(map->_roadNetwork.at("2").get(),  (++it)->second);
    EXPECT_EQ(track.end(), ++it);

    // check length
    EXPECT_DOUBLE_EQ(path.distanceToHead(), 0.0);
    EXPECT_DOUBLE_EQ(path.distanceToBack(), 0.0);

    // update path to get a length of 2000
    path.updatePath(2000.0, 2000.0, track);
    EXPECT_DOUBLE_EQ(path.distanceToHead(), 2000.0);
    EXPECT_DOUBLE_EQ(path.distanceToBack(), 2000.0);

}


TEST(PathMapTest, generatePaths) {


    using namespace simmap::server;

    // create map and segment
    auto map = new simmap::odra::ODRAdapter;
    map->loadFile(base::string_format("%s/junction1.xodr", TRACKS_DIR));

    // define start points
    std::vector<const LaneEdge*> startingPoints = {
            map->getEdge("R1-LS9-L1"),
            map->getEdge("R2-LS1-R1"),
            map->getEdge("R3-LS1-L1")};

    // create strings
    std::vector<std::string> res = {
            "R1-LS9-L1 > R1-LS8-L1 > R1-LS6-L1 > R1-LS5-L1 > R1-LS4-L1 > R1-LS2-L1 > R1-LS1-L1 > R112-LS1-R1 > R2-LS1-L1",
            "R1-LS9-L1 > R1-LS8-L1 > R1-LS6-L1 > R1-LS5-L1 > R1-LS4-L1 > R1-LS2-L1 > R1-LS1-L1 > R113-LS1-R1 > R3-LS1-R1",
            "R2-LS1-R1 > R121-LS1-R1 > R1-LS1-R1 > R1-LS2-R1 > R1-LS3-R1 > R1-LS7-R1 > R1-LS9-R1",
            "R2-LS1-R1 > R123-LS1-R1 > R3-LS1-R1",
            "R3-LS1-L1 > R131-LS1-R1 > R1-LS1-R1 > R1-LS2-R1 > R1-LS3-R1 > R1-LS7-R1 > R1-LS9-R1",
            "R3-LS1-L1 > R132-LS1-R1 > R2-LS1-L1"
    };


    // iterate over starting points
    for(const auto edge : startingPoints) {

        // create path vector and calculate path
        std::list<Path*> paths;
        Path::createRecursively(paths, edge, 1000.0);

        for(const auto &p : paths) {

            // get code
            std::stringstream str;
            str << *p;

            // find element in vector
            auto it = std::find(res.begin(), res.end(), str.str());
            EXPECT_NE(res.end(), it);

            // delete element from vector
            if(it != res.end())
                res.erase(it);

            // delete element
            delete p;

        }

    }

    // check that list is empty
    EXPECT_TRUE(res.empty());


}

TEST(PathMatchTest, Algo) {

    using namespace simmap::odra;
    using namespace simmap::server;

    // create map and segment
    ODRAdapter map;
    map.loadFile(base::string_format("%s/LaneSections.xodr", TRACKS_DIR));

    // get road
    auto rd1 = dynamic_cast<ODRRoad*>(map._roadNetwork.at("1").get());

    // create path (lane 0)
    auto edge = map.getEdge("R1-LS1-R1");
    MapCoordinate mc(edge, 0.0, 0.0);
    Track track({{base::Orientation::FORWARDS, rd1}});

    // create path
    Path path;
    Path::create(path, track, 100.0, 0.0, mc);

    EXPECT_NEAR(100.0, path.distanceToHead(), 1e-15);
    EXPECT_NEAR(0.0, path.distanceToBack(), 1e-15);

    double s = 0.0;
    auto pos = MapCoordinate(edge, 0.0, 0.0).absolutePosition().position;
    double err = path.match(pos, s);
    EXPECT_NEAR(0.0, s, 1e-15);
    EXPECT_NEAR(0.0, err, 1e-15);

    s = 0.0;
    pos = MapCoordinate(edge, 0.0, 1.0).absolutePosition().position;
    err = path.match(pos, s);
    EXPECT_NEAR(0.0, s, 1e-15);
    EXPECT_NEAR(1.0, err, 1e-15);

    s = 0.0;
    pos = MapCoordinate(edge, 5.0, 1.0).absolutePosition().position;
    err = path.match(pos, s);
    EXPECT_NEAR(5.0, s, 1e-15);
    EXPECT_NEAR(1.0, err, 1e-15);

    // create path (lane -1)
    edge = map.getEdge("R1-LS1-R1");
    mc = MapCoordinate(edge, 0.0, 0.0);
    Path::create(path, track, 100.0, 0.0, mc);

    s = 0.0;
    pos = MapCoordinate(edge, 5.0, 0.0).absolutePosition().position;
    err = path.match(pos, s);
    EXPECT_NEAR(5.0, s, 1e-15);
    EXPECT_NEAR(0.0, err, 1e-15);

    // update path position
    path.position(5.0);

    s = -5.0;
    pos = MapCoordinate(edge, 5.0, 0.0).absolutePosition().position;
    err = path.match(pos, s);
    EXPECT_NEAR(0.0, s, 1e-15);
    EXPECT_NEAR(0.0, err, 1e-15);

}


TEST_F(PathTest, match) {

    // number of steps
    size_t n = 100;

    // calculate angle
    auto dPhi = M_PI / (n - 1);
    auto phi = -M_PI_2;

    // create path
    createPath(1.1 * M_PI * R, 0.0);

    // loop
    for(size_t i = 0; i < n; ++i) {

        // calculate x and y
        auto x = cos(phi) * (R + W * 0.5 + 1.0);
        auto y = sin(phi) * (R + W * 0.5 + 1.0);

        // calculate map position
        auto s = phi * R;

        // initialize values
        double sm;
        double err;

        // matching with unspecified starting point
        err = match(base::Vector3{x, y, 0.0}, sm = 0.0);

        // calculate position
        auto pos = positionAt(sm, -1.0).absolutePosition();

        // check
        EXPECT_NEAR(s, sm, 1e-5);
        EXPECT_NEAR(err, 1.0, 1e-5);
        EXPECT_NEAR(x, pos.position.x, 1e-5);
        EXPECT_NEAR(y, pos.position.y, 1e-5);

        // matching with specified starting point
        err = match(base::Vector3{x, y, 0.0}, sm = s - 10.1234, 25.0);

        // check
        EXPECT_NEAR(s, sm, 1e-5);
        EXPECT_NEAR(err, 1.0, 1e-5);

        // update phi
        phi += dPhi;

    }

}


TEST_F(PathTest, matchWithUpdate) {

    // number of steps
    size_t n = 100;
    int rounds = 2;

    // calculate angle
    auto dPhi = M_PI * 2.0 * rounds / (n - 1);
    auto phi = -M_PI * rounds;

    // create path
    double len = M_PI * 2.0 * rounds * R;
    createPath(len, phi * R);

    // set offset
    position(0.0, 0.0);

    // get position
    base::Vector3 pos;

    // loop
    for(size_t i = 0; i < n - 2; ++i) {

        // move one step
        pos = positionAt(dPhi * R).absolutePosition().position;

        // calculate reference position
        auto x = cos(phi + dPhi) * (R + W * 0.5 - 0.0);
        auto y = sin(phi + dPhi) * (R + W * 0.5 - 0.0);

        // check updated position
        EXPECT_NEAR(x, pos.x, 1e-4);
        EXPECT_NEAR(y, pos.y, 1e-4);

        // matching
        auto sm = 0.0;
        auto err = match(pos, sm, 25.0);

        // check matching result
        EXPECT_NEAR(dPhi * R, sm, 1e-5);
        EXPECT_NEAR(0.0, err, 1e-5);

        // update path position
        position(sm, 0.0);

        // check matched position
        EXPECT_NEAR(x, position().absolutePosition().position.x, 1e-4);
        EXPECT_NEAR(y, position().absolutePosition().position.y, 1e-4);

        // update phi
        phi += dPhi;

    }

}