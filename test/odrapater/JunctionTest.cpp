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

#include <gtest/gtest.h>
#include <odradapter/ODRAdapter.h>
#include <graph/Edge.h>
#include <odradapter/ODRRoad.h>
#include <base/functions.h>

class JunctionTest : public ::testing::Test {

public:

    // create objects
    simmap::odra::ODRAdapter map{};
    ODRRoad *r1 = nullptr;
    ODRRoad *r2 = nullptr;
    ODRRoad *r3 = nullptr;
    ODRRoad *r112 = nullptr;
    ODRRoad *r113 = nullptr;
    ODRRoad *r121 = nullptr;
    ODRRoad *r123 = nullptr;
    ODRRoad *r131 = nullptr;
    ODRRoad *r132 = nullptr;


    void SetUp() override {

        // create map
        map.loadFile(base::string_format("%s/junction1.xodr", TRACKS_DIR));

        // get objects
        ASSERT_NO_THROW(r1 = dynamic_cast<ODRRoad *>(map._roadNetwork.at("1").get()));
        ASSERT_NO_THROW(r2 = dynamic_cast<ODRRoad *>(map._roadNetwork.at("2").get()));
        ASSERT_NO_THROW(r3 = dynamic_cast<ODRRoad *>(map._roadNetwork.at("3").get()));
        ASSERT_NO_THROW(r112 = dynamic_cast<ODRRoad *>(map._roadNetwork.at("112").get()));
        ASSERT_NO_THROW(r113 = dynamic_cast<ODRRoad *>(map._roadNetwork.at("113").get()));
        ASSERT_NO_THROW(r121 = dynamic_cast<ODRRoad *>(map._roadNetwork.at("121").get()));
        ASSERT_NO_THROW(r123 = dynamic_cast<ODRRoad *>(map._roadNetwork.at("123").get()));
        ASSERT_NO_THROW(r131 = dynamic_cast<ODRRoad *>(map._roadNetwork.at("131").get()));
        ASSERT_NO_THROW(r132 = dynamic_cast<ODRRoad *>(map._roadNetwork.at("132").get()));

    }

    void TearDown() override {


    }


};


TEST_F(JunctionTest, LaneConnection) {

    auto ls1 = r1->lanes.crossSection(base::ContactPoint::START); // lane section of road 1
    auto ls2 = r2->lanes.crossSection(base::ContactPoint::END);   // lane section of road 2
    auto ls3 = r3->lanes.crossSection(base::ContactPoint::START); // lane section of road 3

    // check different accesses
    EXPECT_EQ(ls1.left()->front(), r1->lanes.lane(base::ContactPoint::START, 1));
    EXPECT_EQ(ls1.right()->front(), r1->lanes.lane(base::ContactPoint::START, -1));
    EXPECT_EQ(ls2.left()->front(), r2->lanes.lane(base::ContactPoint::END, 1));
    EXPECT_EQ(ls2.right()->front(), r2->lanes.lane(base::ContactPoint::END, -1));
    EXPECT_EQ(ls3.left()->front(), r3->lanes.lane(base::ContactPoint::START, 1));
    EXPECT_EQ(ls3.right()->front(), r3->lanes.lane(base::ContactPoint::START, -1));


    // check
    EXPECT_EQ(ls1.left()->front(), r1->lanes.lane(base::ContactPoint::START, 1));
    EXPECT_EQ(ls1.right()->front(), r1->lanes.lane(base::ContactPoint::START, -1));
    EXPECT_EQ(ls2.left()->front(), r2->lanes.lane(base::ContactPoint::END, 1));
    EXPECT_EQ(ls2.right()->front(), r2->lanes.lane(base::ContactPoint::END, -1));
    EXPECT_EQ(ls3.left()->front(), r3->lanes.lane(base::ContactPoint::START, 1));
    EXPECT_EQ(ls3.right()->front(), r3->lanes.lane(base::ContactPoint::START, -1));


    // get lane sections: This test is equal to LaneSectionTest::RoadConnection
    auto laneSections = &r1->lanes;

    // check different accesses
    EXPECT_EQ(ls1.left()->front(), laneSections->lane(base::ContactPoint::START, 1));
    EXPECT_EQ(ls1.right()->front(), laneSections->lane(base::ContactPoint::START, -1));


    // get lane information
    auto lil = laneSections->lane(ls1.left()->front());
    auto lir = laneSections->lane(ls1.right()->front());


    // check information

    EXPECT_EQ(1, lil.lane->_laneID);
    EXPECT_EQ(ls1.left()->front(), lil.lane);
    EXPECT_EQ(0.0, lil.lane->_s0);

    EXPECT_EQ(-1, lir.lane->_laneID);
    EXPECT_EQ(ls1.right()->front(), lir.lane);
    EXPECT_EQ(0.0, lir.lane->_s0);


}