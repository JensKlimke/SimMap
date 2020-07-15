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
// Created by Jens Klimke on 2019/06/06
//


#include <gtest/gtest.h>
#include <simmap/simmap.h>
#include <base/functions.h>


class LoadLibTest : public testing::Test {


    void SetUp() override {}

    void TearDown() override { simmap::clear(); }

};


TEST_F(LoadLibTest, LoadMultipleMaps) {

    using namespace simmap;

    unsigned int id;

    EXPECT_EQ(0, simmap::loadMap(base::string_format("%s/CircleR100.xodr", TRACKS_DIR).c_str(), &id));
    EXPECT_EQ(1, id);

    EXPECT_EQ(0, registerAgent(1, id));
    EXPECT_EQ(0, registerAgent(2, id));

    EXPECT_EQ(0, simmap::loadMap(base::string_format("%s/map.xodr", TRACKS_DIR).c_str(), &id));
    EXPECT_EQ(2, id);

    EXPECT_EQ(0, registerAgent(5, id));
    EXPECT_EQ(0, registerAgent(6, id));

    EXPECT_EQ(0, clear());


    EXPECT_EQ(0, simmap::loadMap(base::string_format("%s/map.xodr", TRACKS_DIR).c_str(), &id));
    EXPECT_EQ(1, id);

    EXPECT_EQ(0, registerAgent(5, id));
    EXPECT_EQ(0, registerAgent(6, id));

    EXPECT_EQ(0, simmap::loadMap(base::string_format("%s/CircleR100.xodr", TRACKS_DIR).c_str(), &id));
    EXPECT_EQ(2, id);

    EXPECT_EQ(0, registerAgent(1, id));
    EXPECT_EQ(0, registerAgent(2, id));

    EXPECT_EQ(0, clear());

}