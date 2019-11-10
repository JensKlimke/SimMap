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
// Created by Jens Klimke on 22.05.2019.
//

#include <gtest/gtest.h>
#include <odradapter/ODRAdapter.h>
#include <graph/Edge.h>
#include <server/MapCoordinate.h>
#include <odradapter/ODRRoad.h>


TEST(LaneSeparationTest, LaneSeparation) {

    using namespace simmap::odra;
    using namespace simmap::server;

    /*// create map and segment
    ODRAdapter map{};
    map.loadFile(base::string_format("%s/example_simple.xodr", TRACKS_DIR));

    // get edges
    auto e_r2 = dynamic_cast<const ODREdge *>(map.getEdge("R1-LS2-R3"));
    auto e_r1 = dynamic_cast<const ODREdge *>(map.getEdge("R1-LS2-R2"));
    auto e_re = dynamic_cast<const ODREdge *>(map.getEdge("R1-LS2-R1"));
    auto e_l1 = dynamic_cast<const ODREdge *>(map.getEdge("R1-LS2-L1"));
    auto e_l2 = dynamic_cast<const ODREdge *>(map.getEdge("R1-LS2-L2"));

    // check neighbors
    EXPECT_TRUE(e_r2->right().empty());
    EXPECT_EQ(e_r1, e_r2->left().front().element);
    EXPECT_EQ(e_l1, e_r1->left().front().element);
    //EXPECT_EQ(e_l2, e_l1->right().front().element);*/



}