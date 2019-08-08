//
// Created by klimke on 22.05.2019.
//

#include <gtest/gtest.h>
#include <odradapter/ODRAdapter.h>
#include <graph/Edge.h>
#include <server/MapCoordinate.h>
#include <ODRRoad.h>


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