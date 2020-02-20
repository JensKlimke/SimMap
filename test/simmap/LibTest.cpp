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
// Created by Jens Klimke on 25.11.18.
//

#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif

#include <gtest/gtest.h>

#include <string>
#include <iostream>
#include <cmath>
#include <vector>

#include <simmap/lib.h>
#include <base/functions.h>
#include <base/definitions.h>

using namespace simmap;


class LibraryTest : public testing::Test {


    void SetUp() override {}

    void TearDown() override { clear(); }


protected:

    static const double R;

    err_type_t _loadMap(const std::string& filename, id_type_t *id) {

        return loadMap(filename.c_str(), id);

    }


public:

    void init() {

        // load maps

        id_type_t id;

        EXPECT_EQ(0, _loadMap(base::string_format("%s/CircleR100.xodr", TRACKS_DIR), &id));
        EXPECT_EQ(1, id);

        EXPECT_EQ(26, _loadMap(base::string_format("%s/not_existing_map.xodr", TRACKS_DIR), &id));
        EXPECT_EQ(0, id);

        EXPECT_EQ(0, _loadMap(base::string_format("%s/map.xodr", TRACKS_DIR), &id));
        EXPECT_EQ(2, id);

        EXPECT_EQ(0, _loadMap(base::string_format("%s/example_simple.xodr", TRACKS_DIR), &id));
        EXPECT_EQ(3, id);

        EXPECT_EQ(0, _loadMap(base::string_format("%s/Straight10000.xodr", TRACKS_DIR), &id));
        EXPECT_EQ(4, id);


        // register agents

        EXPECT_EQ(0,  registerAgent(1, 1));
        EXPECT_EQ(45, registerAgent(1, 1)); // try to re-register agent 1 again
        EXPECT_EQ(0,  registerAgent(2, 1));
        EXPECT_EQ(0,  registerAgent(3, 2)); // register agent 3 on different map
        EXPECT_EQ(46, registerAgent(4, 9)); // try to register agent on non-existing map
        EXPECT_EQ(0,  registerAgent(5, 1));
        EXPECT_EQ(0,  registerAgent(6, 1));
        EXPECT_EQ(0,  registerAgent(7, 1));
        EXPECT_EQ(0,  registerAgent(8, 3)); // register agent 8 to simple map
        EXPECT_EQ(0,  registerAgent(9, 4));


        // set tracks

        std::vector<std::vector<const char*>> ra = {{"1", "-2"}, {"2", "-1"}, {"10", "5"}, {"1"}, {"-1"}};

        EXPECT_EQ(0,  setTrack(1, ra[0].data(), 2));
        EXPECT_EQ(0,  setTrack(2, ra[1].data(), 2));
        EXPECT_EQ(65, setTrack(3, ra[2].data(), 2));
        EXPECT_EQ(0,  setTrack(5, ra[0].data(), 2));
        EXPECT_EQ(0,  setTrack(6, ra[0].data(), 2));
        EXPECT_EQ(0,  setTrack(7, ra[0].data(), 2));
        EXPECT_EQ(0,  setTrack(8, ra[3].data(), 1));
        EXPECT_EQ(0,  setTrack(9, ra[4].data(), 1));


    }


    void initPaths() {

        double lenFront = 700.0; double lenBack = 350.0;
        EXPECT_EQ(0, setMapPosition(1, {"R1-LS1-R1", 10.0, 0.0}, &lenFront, &lenBack));
        EXPECT_NEAR(700.0,lenFront, 1e-6);
        EXPECT_NEAR(350.0,lenBack,  1e-6);

        lenFront = 200.0; lenBack = 100.0;
        EXPECT_EQ(0, setMapPosition(2, {"R2-LS1-R1", 10.0, 0.0}, &lenFront, &lenBack));
        EXPECT_NEAR(200.0,lenFront, 1e-6);
        EXPECT_NEAR(100.0,lenBack,  1e-6);

        lenFront = 200.0; lenBack = 100.0;
        EXPECT_EQ(0, setMapPosition(5, {"R1-LS2-R2", 10.0, 0.0}, &lenFront, &lenBack));
        EXPECT_NEAR(200.0,lenFront, 1e-6);
        EXPECT_NEAR(100.0,lenBack,  1e-6);

        lenFront = 200.0; lenBack = 100.0;
        EXPECT_EQ(0, setMapPosition(6, {"R1-LS1-R1", 30.0, 0.0}, &lenFront, &lenBack));
        EXPECT_NEAR(200.0,lenFront, 1e-6);
        EXPECT_NEAR(100.0,lenBack,  1e-6);

        lenFront = 200.0; lenBack = 100.0;
        EXPECT_EQ(0, setMapPosition(7, {"R1-LS1-R1", 5.0, 0.0}, &lenFront, &lenBack));
        EXPECT_NEAR(200.0,lenFront, 1e-6);
        EXPECT_NEAR(100.0,lenBack,  1e-6);

        lenFront = 165.0; lenBack = 10.0;
        EXPECT_EQ(0, setMapPosition(8, {"R1-LS1-R1", 0.0, 0.0}, &lenFront, &lenBack));
        EXPECT_NEAR(165.0,lenFront, 1e-6);
        EXPECT_NEAR(  0.0,lenBack,  1e-6);

        lenFront = 1000.0; lenBack = 50.0;
        EXPECT_EQ(0, setMapPosition(9, {"R1-LS1-R1", 0.0, 0.0}, &lenFront, &lenBack));
        EXPECT_NEAR(1000.0,lenFront, 1e-6);
        EXPECT_NEAR(   0.0,lenBack,  1e-6);



    }


    void terminate() {

        // unload agents

        EXPECT_EQ(0,  unregisterAgent(3));
        EXPECT_EQ(52, unregisterAgent(4));


        // unload maps

        EXPECT_EQ(0,  unloadMap(1));
        EXPECT_EQ(0,  unloadMap(2));
        EXPECT_EQ(0,  unloadMap(3));
        EXPECT_EQ(0,  unloadMap(4));
        EXPECT_EQ(35, unloadMap(9));

    }



};

const double LibraryTest::R = 100.0;


TEST_F(LibraryTest, Initialize) {

    init();
    initPaths();

}


TEST_F(LibraryTest, Terminate) {

    init();
    initPaths();
    terminate();

}


TEST_F(LibraryTest, ClearLib) {

    init();
    initPaths();
    EXPECT_EQ(0, clear());

}


TEST_F(LibraryTest, GetPosition) {

    init();
    initPaths();

    MapPosition mapPos{};
    Position pos{};

    // first agent
    auto RE = 101.875;
    auto phi = 10.0 / R;
    auto y = sin(phi) * RE;
    auto x = cos(phi) * RE;

    EXPECT_EQ(0, getPosition(1, &pos));
    EXPECT_EQ(0, getMapPosition(1, &mapPos));

    EXPECT_DOUBLE_EQ(10.0, mapPos.longPos);
    EXPECT_DOUBLE_EQ(0.0,  mapPos.latPos);
    EXPECT_EQ(0, strcmp("R1-LS1-R1", mapPos.edgeID));

    EXPECT_NEAR(x,            pos.x,     def::EPS_DISTANCE);
    EXPECT_NEAR(y,            pos.y,     def::EPS_DISTANCE);
    EXPECT_NEAR(0.0,          pos.z,     def::EPS_DISTANCE);
    EXPECT_NEAR(phi + M_PI_2, pos.phi,   def::EPS_DISTANCE);
    EXPECT_NEAR(RE,     1.0 / pos.kappa, def::EPS_DISTANCE);


    // second agent
    RE  = LibraryTest::R - 1.875;
    phi = -10.0 / LibraryTest::R;
    y   = sin(phi) * RE;
    x   = cos(phi) * RE;

    EXPECT_EQ(0, getPosition(2, &pos));
    EXPECT_EQ(0, getMapPosition(2, &mapPos));

    EXPECT_DOUBLE_EQ(10.0, mapPos.longPos);
    EXPECT_EQ(0, strcmp("R2-LS1-R1", mapPos.edgeID));

    EXPECT_NEAR(x,             pos.x,     def::EPS_DISTANCE);
    EXPECT_NEAR(y,             pos.y,     def::EPS_DISTANCE);
    EXPECT_NEAR(0.0,           pos.z,     def::EPS_DISTANCE);
    EXPECT_NEAR(-M_PI_2 + phi, pos.phi,   def::EPS_DISTANCE);
    EXPECT_NEAR(-1.0 / RE,     pos.kappa, def::EPS_DISTANCE);

}


TEST_F(LibraryTest, UpdatePosition) {

    init();
    initPaths();

    MapPosition mapPos{};
    Position pos{};

    // get position
    getMapPosition(1, &mapPos);
    getPosition(1, &pos);

    double R = LibraryTest::R;
    double dPhi = 0.1, phi = 10.0 / R;
    double lf = 100.0, lb = 100.0;
    for(size_t i = 0; i < 100; ++i) {

        auto x = cos(phi) * (R + 1.875);
        auto y = sin(phi) * (R + 1.875);

        // get positions
        getPosition(1, &pos);

        // check positions
        EXPECT_NEAR(x, pos.x, 1e-6);
        EXPECT_NEAR(y, pos.y, 1e-6);
        EXPECT_NEAR(0.0, base::angleDiff(phi + M_PI_2, pos.phi), 1e-6);

        // move by one step
        EXPECT_EQ(0, move(1, dPhi * R, 0.0, &lf, &lb));
        EXPECT_DOUBLE_EQ(100.0, lf);
        EXPECT_DOUBLE_EQ(100.0, lb);

        phi += dPhi;

    }


}


TEST_F(LibraryTest, UpdatePositionWithLaneChange) {

    init();
    initPaths();

    MapPosition mapPos{};
    Position pos{};

    // get position
    getMapPosition(1, &mapPos);
    getPosition(1, &pos);

    double R = LibraryTest::R;
    double dPhi = 1.0, phi = 10.0 / R;
    double lf = 100.0, lb = 100.0;

    int l = -1;

    for(size_t i = 0; i < 7; ++i) {

        auto x = cos(phi) * (R + (l < 0 ? 1.875 : 5.5));
        auto y = sin(phi) * (R + (l < 0 ? 1.875 : 5.5));

        // get positions
        getPosition(1, &pos);

        // check positions
        EXPECT_NEAR(x, pos.x, 1e-6);
        EXPECT_NEAR(y, pos.y, 1e-6);
        EXPECT_NEAR(0.0, base::angleDiff(phi + M_PI_2, pos.phi), 1e-6);

        // get map position
        getMapPosition(1, &mapPos);

        if(i < 2) {

            // check map position
            EXPECT_EQ((l < 0 ? "R1-LS1-R1" : "R1-LS1-R2"), std::string(mapPos.edgeID));
            EXPECT_NEAR(mapPos.longPos, R * phi, 1e-6);
            EXPECT_NEAR(mapPos.latPos, 0.0, 1e-6);

        } else if(i < 4) {

            // check map position
            EXPECT_EQ((l < 0 ? "R1-LS2-R1" : "R1-LS2-R2"), std::string(mapPos.edgeID));
            EXPECT_NEAR(mapPos.longPos, R * (phi - M_PI_2), 1e-6);
            EXPECT_NEAR(mapPos.latPos, 0.0, 1e-6);

        } else if(i < 7) {

            // check map position
            EXPECT_EQ((l < 0 ? "R2-LS1-L1" : "R2-LS1-L2"), std::string(mapPos.edgeID));
            EXPECT_NEAR(mapPos.longPos, R * (phi - M_PI), 1e-6);
            EXPECT_NEAR(mapPos.latPos, 0.0, 1e-6);

        }

        // move by one step and switch lane
        EXPECT_EQ(0, move(1, dPhi * R, 0.0, &lf, &lb));
        EXPECT_EQ(0, switchLane(1, l));

        // this should result in a error code
        EXPECT_EQ((l > 0 ? 167 : 168), switchLane(1, l));

        l *= -1;
        phi += dPhi;

    }


}



TEST_F(LibraryTest, MatchPosition) {

    init();
    initPaths();

    MapPosition mapPos{};
    Position pos{};

    // get position
    getMapPosition(1, &mapPos);
    getPosition(1, &pos);

    double R = LibraryTest::R;
    double dPhi = 0.1, phi = 0.1;
    for(size_t i = 0; i < 10; ++i) {

        auto s = phi * R;
        pos.x = cos(phi) * (R + 2.0);
        pos.y = sin(phi) * (R + 2.0);

        EXPECT_EQ(0, match(1, pos, 0.0, &mapPos));
        EXPECT_EQ(0, strcmp("R1-LS1-R1", mapPos.edgeID));
        EXPECT_NEAR(s, mapPos.longPos, 1e-5);
        EXPECT_NEAR(-0.125, mapPos.latPos, 1e-5);

        phi += dPhi;

    }

}


TEST_F(LibraryTest, MatchPosition2) {

    init();
    initPaths();

    MapPosition mapPos{};
    Position pos{};

    // get position
    getMapPosition(9, &mapPos);
    getPosition(9, &pos);

    // set position
    pos.x = 100.0;
    pos.y = -1.875;
    pos.z = 0.0;
    pos.phi = 0.0;
    pos.kappa = 0.0;

    // match
    EXPECT_EQ(0, match(9, pos, 0.0, &mapPos));

}


TEST_F(LibraryTest, MatchAndUpdatePosition) {

    init();
    initPaths();

    // create sample points
    std::vector<double> samplePoints(20);
    for(size_t i = 0; i < 20; ++i)
        samplePoints[i] = pow((double) i * 20.0 / 19.0, 2);

    // create horizon
    HorizonInformation horData[20];

    // initialize ds
    double ds = 1.0;
    double s0 = 10.0;

    // create position structs
    MapPosition mapPos{};
    Position pos{};

    // get position
    getMapPosition(1, &mapPos);
    getPosition(1, &pos);


    // iterate
    double s = s0;
    for(size_t i = 0; i < 600; ++i) {

        double lenFront = 450.0;
        double lenBack = 20.0;

        // check matching
        EXPECT_EQ(0, match(1, pos, 0.0, &mapPos));

        // check results
        if(s >= 0.0 && s <= M_PI * 0.5 * R) {

            EXPECT_EQ(0, strcmp("R1-LS1-R1", mapPos.edgeID));
            EXPECT_NEAR(s, mapPos.longPos, 1e-5);

            // update s
            s = mapPos.longPos;

        } else if(s > M_PI * 0.5 * R && s <= M_PI * R) {

            EXPECT_EQ(0, strcmp("R1-LS2-R1", mapPos.edgeID));
            EXPECT_NEAR(s - M_PI * 0.5 * R, mapPos.longPos, 1e-5);

            // update s
            s = mapPos.longPos + 0.5 * M_PI * R;

        } else {

            EXPECT_EQ(0, strcmp("R2-LS1-L1", mapPos.edgeID));
            EXPECT_NEAR(s - M_PI * R, mapPos.longPos, 1e-5);

            // update s
            s = mapPos.longPos + M_PI * R;

        }

        // set new position and create horizon
        EXPECT_EQ(0, setMapPosition(1, mapPos, &lenFront, &lenBack));
        EXPECT_NEAR(450.0, lenFront, 1e-9);
        EXPECT_NEAR(20.0, lenBack, 1e-9);

        // get horizon
        EXPECT_EQ(0, horizon(1, samplePoints.data(), horData, samplePoints.size()));

        // get horizon data
        double dsh = horData[1].s - horData[0].s;
        double x0 = horData[0].x;
        double x1 = horData[1].x;
        double y0 = horData[0].y;
        double y1 = horData[1].y;
        double p0 = horData[0].psi;
        double p1 = horData[1].psi;
        double k0 = horData[0].kappa;

        EXPECT_DOUBLE_EQ(101.875, 1.0 / k0);
        EXPECT_DOUBLE_EQ(3.75, horData[0].laneWidth);
        EXPECT_DOUBLE_EQ(3.5,  horData[0].rightWidth);
        EXPECT_DOUBLE_EQ(3.75, horData[0].leftWidth);

        EXPECT_DOUBLE_EQ(3.75, horData[19].laneWidth);
        EXPECT_DOUBLE_EQ(3.5,  horData[19].rightWidth);
        EXPECT_DOUBLE_EQ(3.75, horData[19].leftWidth);

        // update position
        pos.x = x0 + ds * (x1 - x0) / dsh;
        pos.y = y0 + ds * (y1 - y0) / dsh;
        pos.phi = p0 + ds * (p1 - p0) / dsh;

        s += ds;

    }

}


TEST_F(LibraryTest, GetObjects) {

    init();
    initPaths();

    // create objects
    unsigned long n = 10;
    ObjectInformation info[10];

    objects(1, info, &n);

    ASSERT_EQ(5, n);

    EXPECT_EQ(0, strcmp("4", info[0].id));
    EXPECT_DOUBLE_EQ(-20.0, info[0].distance);
    EXPECT_EQ(ObjectType::STOP_SIGN, info[0].type);
    EXPECT_EQ(0, info[0].value);

    EXPECT_EQ(0, strcmp("3", info[1].id));
    EXPECT_DOUBLE_EQ(90.0, info[1].distance);
    EXPECT_EQ(ObjectType::SPEED_LIMIT, info[1].type);
    EXPECT_EQ(50, info[1].value);

    EXPECT_EQ(0, strcmp("1", info[2].id));
    EXPECT_DOUBLE_EQ(100.0, info[2].distance);
    EXPECT_EQ(ObjectType::UNKNOWN, info[2].type);
    EXPECT_EQ(0, info[2].value);

    EXPECT_EQ(0, strcmp("2", info[3].id));
    EXPECT_DOUBLE_EQ(190.0, info[3].distance);
    EXPECT_EQ(ObjectType::SPEED_LIMIT, info[3].type);
    EXPECT_EQ(70, info[3].value);

    EXPECT_EQ(0, strcmp("4", info[4].id));
    EXPECT_DOUBLE_EQ(2.0 * M_PI * LibraryTest::R - 20.0, info[4].distance);
    EXPECT_EQ(ObjectType::STOP_SIGN, info[4].type);
    EXPECT_EQ(0, info[4].value);

}


TEST_F(LibraryTest, GetLaneInformation) {

    // init
    init();
    initPaths();

    // create objects
    unsigned long n = 10;
    LaneInformation info[10];

    // get lane information
    lanes(1, info, &n);

    // check data
    EXPECT_EQ(3, n);

    // check left left lane
    EXPECT_EQ(2, info[0].index);
    EXPECT_EQ(Direction::BACKWARDS, info[0].direction);
    EXPECT_DOUBLE_EQ(3.5, info[0].width);
    EXPECT_DOUBLE_EQ(700.0, info[0].lengthOnTrack);
    EXPECT_DOUBLE_EQ(700.0, info[0].lengthToClosed);
    EXPECT_EQ(Access::ALLOWED, info[0].access);


    // check left lane
    EXPECT_EQ(1, info[1].index);
    EXPECT_EQ(Direction::BACKWARDS, info[1].direction);
    EXPECT_DOUBLE_EQ(3.75, info[1].width);
    EXPECT_DOUBLE_EQ(700.0, info[1].lengthOnTrack);
    EXPECT_DOUBLE_EQ(700.0, info[1].lengthToClosed);
    EXPECT_EQ(Access::ALLOWED, info[1].access);


    // check right lane
    EXPECT_EQ(-1, info[2].index);
    EXPECT_EQ(Direction::FORWARDS, info[2].direction);
    EXPECT_DOUBLE_EQ(3.5, info[2].width);
    EXPECT_DOUBLE_EQ(700.0, info[2].lengthOnTrack);
    EXPECT_DOUBLE_EQ(700.0, info[2].lengthToClosed);
    EXPECT_EQ(Access::ALLOWED, info[2].access);


}



TEST_F(LibraryTest, GetTargetInformation) {

    // init
    init();
    initPaths();

    // create objects
    unsigned long n = 10;
    TargetInformation info[10];

    // get lane information
    targets(1, info, &n);

    // check data
    ASSERT_EQ(7, n);


    EXPECT_EQ(7,           info[0].id);
    EXPECT_EQ(0,           info[0].lane);
    EXPECT_DOUBLE_EQ(0.0,  info[0].latOffset);
    EXPECT_DOUBLE_EQ(-5.0, info[0].distance);


    EXPECT_EQ(6,           info[1].id);
    EXPECT_EQ(0,           info[1].lane);
    EXPECT_DOUBLE_EQ(0.0,  info[1].latOffset);
    EXPECT_DOUBLE_EQ(20.0, info[1].distance);


    EXPECT_EQ(2,           info[2].id);
    EXPECT_EQ(1,           info[2].lane);
    EXPECT_DOUBLE_EQ(0.0,  info[2].latOffset);
    EXPECT_DOUBLE_EQ(-20.0, info[2].distance);


    double ds = M_PI * LibraryTest::R * 0.5;
    EXPECT_EQ(5,            info[3].id);
    EXPECT_EQ(-1,           info[3].lane);
    EXPECT_DOUBLE_EQ(0.0,   info[3].latOffset);
    EXPECT_DOUBLE_EQ(ds,    info[3].distance);


    ds = M_PI * LibraryTest::R * 2.0 - 20.0;
    EXPECT_EQ(2,          info[4].id);
    EXPECT_EQ(1,          info[4].lane);
    EXPECT_DOUBLE_EQ(0.0, info[4].latOffset);
    EXPECT_DOUBLE_EQ(ds,  info[4].distance);


    ds = 2.0 * M_PI * LibraryTest::R - 5.0;
    EXPECT_EQ(7,           info[5].id);
    EXPECT_EQ(0,           info[5].lane);
    EXPECT_DOUBLE_EQ(0.0,  info[5].latOffset);
    EXPECT_DOUBLE_EQ(ds,   info[5].distance);


    ds = 2.0 * M_PI * LibraryTest::R + 20.0;
    EXPECT_EQ(6,           info[6].id);
    EXPECT_EQ(0,           info[6].lane);
    EXPECT_DOUBLE_EQ(0.0,  info[6].latOffset);
    EXPECT_DOUBLE_EQ(ds,   info[6].distance);

}


TEST_F(LibraryTest, HorizonInformation) {

    // init
    init();
    initPaths();

    unsigned long n = 20;
    HorizonInformation hor[20];

    std::vector<double> s(n);
    for(unsigned int i = 0; i < n; ++i)
        s[i] = ((double) i - 1.0) * 10.0;

    // get lane information
    horizon(8, s.data(), hor, n);

    EXPECT_DOUBLE_EQ(-INFINITY, hor[0].s);
    EXPECT_NEAR( 0.0, hor[1].s, 1e-6);
    EXPECT_NEAR(10.0, hor[2].s, 1e-6);
    EXPECT_NEAR(20.0, hor[3].s, 1e-6);
    EXPECT_DOUBLE_EQ(INFINITY, hor[18].s);
    EXPECT_DOUBLE_EQ(INFINITY, hor[19].s);

    EXPECT_NEAR(0.0,   hor[0].x, 1e-6);
    EXPECT_NEAR(1.875, hor[1].x, 1e-6);
    EXPECT_NEAR(3.45703125, hor[5].x, 1e-6);
    EXPECT_NEAR(3.75,  hor[8].x, 1e-6);
    EXPECT_NEAR(0.0,   hor[18].x, 1e-6);
    EXPECT_NEAR(0.0,   hor[19].x, 1e-6);

    EXPECT_NEAR( 0.0, hor[0].y, 1e-6);
    EXPECT_NEAR( 0.0, hor[1].y, 1e-6);
    EXPECT_NEAR(10.0, hor[2].y, 1e-6);
    EXPECT_NEAR(20.0, hor[3].y, 1e-6);
    EXPECT_NEAR( 0.0, hor[18].y, 1e-6);
    EXPECT_NEAR( 0.0, hor[19].y, 1e-6);

}