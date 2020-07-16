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
// Created by Jens Klimke on 2019-02-03.
//


#include <gtest/gtest.h>
#include "prep_special.h"

class LaneSectionTest : public testing::Test {

public:

    LaneSectionSequence *laneSections = nullptr;

    void SetUp() override {

        laneSections = new LaneSectionSequence;
        generateComplexLaneSections(laneSections);

    }

    void TearDown() override {

        delete laneSections;

    }

};



TEST_F(LaneSectionTest, BasicEntries) {

    // get road and first lane section
    auto entries = laneSections->laneSections();

    // check no of entries
    ASSERT_EQ(9, entries.size());

    // check overall length
    EXPECT_DOUBLE_EQ(100.0, laneSections->length());


    // check entries

    EXPECT_DOUBLE_EQ(0.0, entries.at(0).position);
    EXPECT_DOUBLE_EQ(10.0, entries.at(0).length);
    EXPECT_EQ(base::Side::BOTH, entries.at(0).laneSection->side());

    EXPECT_DOUBLE_EQ(10.0, entries.at(1).position);
    EXPECT_DOUBLE_EQ(15.0, entries.at(1).length);
    EXPECT_EQ(base::Side::BOTH, entries.at(1).laneSection->side());

    EXPECT_DOUBLE_EQ(25.0, entries.at(2).position);
    EXPECT_DOUBLE_EQ(50.0, entries.at(2).length);
    EXPECT_EQ(base::Side::RIGHT, entries.at(2).laneSection->side());

    EXPECT_DOUBLE_EQ(25.0, entries.at(3).position);
    EXPECT_DOUBLE_EQ(5.0, entries.at(3).length);
    EXPECT_EQ(base::Side::LEFT, entries.at(3).laneSection->side());

    EXPECT_DOUBLE_EQ(30.0, entries.at(4).position);
    EXPECT_DOUBLE_EQ(30.0, entries.at(4).length);
    EXPECT_EQ(base::Side::LEFT, entries.at(4).laneSection->side());

    EXPECT_DOUBLE_EQ(60.0, entries.at(5).position);
    EXPECT_DOUBLE_EQ(15.0, entries.at(5).length);
    EXPECT_EQ(base::Side::LEFT, entries.at(5).laneSection->side());

    EXPECT_DOUBLE_EQ(75.0, entries.at(6).position);
    EXPECT_DOUBLE_EQ(15.0, entries.at(6).length);
    EXPECT_EQ(base::Side::RIGHT, entries.at(6).laneSection->side());

    EXPECT_DOUBLE_EQ(75.0, entries.at(7).position);
    EXPECT_DOUBLE_EQ(15.0, entries.at(7).length);
    EXPECT_EQ(base::Side::LEFT, entries.at(7).laneSection->side());

    EXPECT_DOUBLE_EQ(90.0, entries.at(8).position);
    EXPECT_DOUBLE_EQ(10.0, entries.at(8).length);
    EXPECT_EQ(base::Side::BOTH, entries.at(8).laneSection->side());

}


TEST_F(LaneSectionTest, Linkage) {

    // get road and first lane section
    auto entries = laneSections->laneSections();

    // check successors (center)
    EXPECT_EQ(entries.at(1).laneSection, entries.at(0).laneSection->links().successorCenter);
    EXPECT_EQ(entries.at(2).laneSection, entries.at(1).laneSection->links().successorCenter);
    EXPECT_EQ(entries.at(7).laneSection, entries.at(2).laneSection->links().successorCenter);
    EXPECT_EQ(entries.at(8).laneSection, entries.at(7).laneSection->links().successorCenter);

    // check predecessors (center)
    EXPECT_EQ(entries.at(0).laneSection, entries.at(1).laneSection->links().predecessorCenter);
    EXPECT_EQ(entries.at(1).laneSection, entries.at(2).laneSection->links().predecessorCenter);
    EXPECT_EQ(entries.at(2).laneSection, entries.at(7).laneSection->links().predecessorCenter);
    EXPECT_EQ(entries.at(7).laneSection, entries.at(8).laneSection->links().predecessorCenter);


    // check successors (right hand side)
    EXPECT_EQ(entries.at(1).laneSection, entries.at(0).laneSection->links().successorRight);
    EXPECT_EQ(entries.at(2).laneSection, entries.at(1).laneSection->links().successorRight);
    EXPECT_EQ(entries.at(6).laneSection, entries.at(2).laneSection->links().successorRight);
    EXPECT_EQ(entries.at(8).laneSection, entries.at(6).laneSection->links().successorRight);

    // check predecessors (right hand side)
    EXPECT_EQ(entries.at(0).laneSection, entries.at(1).laneSection->links().predecessorRight);
    EXPECT_EQ(entries.at(1).laneSection, entries.at(2).laneSection->links().predecessorRight);
    EXPECT_EQ(entries.at(2).laneSection, entries.at(6).laneSection->links().predecessorRight);
    EXPECT_EQ(entries.at(6).laneSection, entries.at(8).laneSection->links().predecessorRight);


    // check successors (left hand side)
    EXPECT_EQ(entries.at(1).laneSection, entries.at(0).laneSection->links().successorLeft);
    EXPECT_EQ(entries.at(3).laneSection, entries.at(1).laneSection->links().successorLeft);
    EXPECT_EQ(entries.at(4).laneSection, entries.at(3).laneSection->links().successorLeft);
    EXPECT_EQ(entries.at(5).laneSection, entries.at(4).laneSection->links().successorLeft);
    EXPECT_EQ(entries.at(7).laneSection, entries.at(5).laneSection->links().successorLeft);
    EXPECT_EQ(entries.at(8).laneSection, entries.at(7).laneSection->links().successorLeft);

    // check predecessors (left hand side)
    EXPECT_EQ(entries.at(0).laneSection, entries.at(1).laneSection->links().predecessorLeft);
    EXPECT_EQ(entries.at(1).laneSection, entries.at(3).laneSection->links().predecessorLeft);
    EXPECT_EQ(entries.at(3).laneSection, entries.at(4).laneSection->links().predecessorLeft);
    EXPECT_EQ(entries.at(4).laneSection, entries.at(5).laneSection->links().predecessorLeft);
    EXPECT_EQ(entries.at(5).laneSection, entries.at(7).laneSection->links().predecessorLeft);
    EXPECT_EQ(entries.at(7).laneSection, entries.at(8).laneSection->links().predecessorLeft);

}


TEST_F(LaneSectionTest, CrossSections) {

    // get road and first lane section
    auto entries = laneSections->laneSections();

    // get cross sections
    auto cs0 = laneSections->crossSection(0.0);
    auto cs1 = laneSections->crossSection(15.0);
    auto cs2 = laneSections->crossSection(26.0);
    auto cs3 = laneSections->crossSection(31.0);
    auto cs4 = laneSections->crossSection(61.0);
    auto cs5 = laneSections->crossSection(76.0);
    auto cs6 = laneSections->crossSection(91.0);

    auto css = laneSections->crossSection(base::ContactPoint::START);
    auto cse = laneSections->crossSection(base::ContactPoint::END);


    // check cross sections against lane sections

    EXPECT_EQ(entries.at(0).laneSection, cs0.laneSectionRight());
    EXPECT_EQ(entries.at(0).laneSection, cs0.laneSectionLeft());
    EXPECT_EQ(entries.at(0).laneSection, cs0.laneSectionCenter());

    EXPECT_EQ(entries.at(1).laneSection, cs1.laneSectionRight());
    EXPECT_EQ(entries.at(1).laneSection, cs1.laneSectionLeft());
    EXPECT_EQ(entries.at(1).laneSection, cs1.laneSectionCenter());

    EXPECT_EQ(entries.at(2).laneSection, cs2.laneSectionRight());
    EXPECT_EQ(entries.at(3).laneSection, cs2.laneSectionLeft());
    EXPECT_EQ(entries.at(2).laneSection, cs2.laneSectionCenter());

    EXPECT_EQ(entries.at(2).laneSection, cs3.laneSectionRight());
    EXPECT_EQ(entries.at(4).laneSection, cs3.laneSectionLeft());
    EXPECT_EQ(entries.at(2).laneSection, cs3.laneSectionCenter());

    EXPECT_EQ(entries.at(2).laneSection, cs4.laneSectionRight());
    EXPECT_EQ(entries.at(5).laneSection, cs4.laneSectionLeft());
    EXPECT_EQ(entries.at(2).laneSection, cs4.laneSectionCenter());

    EXPECT_EQ(entries.at(6).laneSection, cs5.laneSectionRight());
    EXPECT_EQ(entries.at(7).laneSection, cs5.laneSectionLeft());
    EXPECT_EQ(entries.at(7).laneSection, cs5.laneSectionCenter());

    EXPECT_EQ(entries.at(8).laneSection, cs6.laneSectionRight());
    EXPECT_EQ(entries.at(8).laneSection, cs6.laneSectionLeft());
    EXPECT_EQ(entries.at(8).laneSection, cs6.laneSectionCenter());


    // check contact point access

    EXPECT_EQ(entries.at(0).laneSection, css.laneSectionRight());
    EXPECT_EQ(entries.at(0).laneSection, css.laneSectionLeft());
    EXPECT_EQ(entries.at(0).laneSection, css.laneSectionCenter());

    EXPECT_EQ(entries.at(8).laneSection, cse.laneSectionRight());
    EXPECT_EQ(entries.at(8).laneSection, cse.laneSectionLeft());
    EXPECT_EQ(entries.at(8).laneSection, cse.laneSectionCenter());

}


TEST_F(LaneSectionTest, RoadConnection) {

    auto ls1 = &(*laneSections->sections().atPos(0).element.first.begin()).element;

    // check different accesses
    EXPECT_EQ(ls1->left().front(),  laneSections->lane(base::ContactPoint::START,  1));
    EXPECT_EQ(ls1->right().front(), laneSections->lane(base::ContactPoint::START, -1));


    // get lane information
    auto lil = laneSections->lane(ls1->left().front());
    auto lir = laneSections->lane(ls1->right().front());


    // check information

    EXPECT_EQ(1, lil.lane->_laneID);
    EXPECT_EQ(ls1->left().front(), lil.lane);
    EXPECT_EQ(0.0, lil.lane->_s0);
    EXPECT_EQ(ls1, lil.laneSection);

    EXPECT_EQ(-1, lir.lane->_laneID);
    EXPECT_EQ(ls1->right().front(), lir.lane);
    EXPECT_EQ(0.0, lir.lane->_s0);
    EXPECT_EQ(ls1, lir.laneSection);

}

