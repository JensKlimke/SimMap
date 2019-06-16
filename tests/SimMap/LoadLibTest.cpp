//
// Created by klimke on 06.06.2019.
//


#include <gtest/gtest.h>
#include <SimMap/lib.h>


class LoadLibTest : public testing::Test {


    void SetUp() override {}

    void TearDown() override { simmap::clear(); }

};


TEST_F(LoadLibTest, LoadMultipleMaps) {

    using namespace simmap;

    unsigned int id;

    EXPECT_EQ(0, loadMap("tests/tracks/CircleR100.xodr", &id));
    EXPECT_EQ(1, id);

    EXPECT_EQ(0, registerAgent(1, id));
    EXPECT_EQ(0, registerAgent(2, id));

    EXPECT_EQ(0, loadMap("tests/tracks/map.xodr", &id));
    EXPECT_EQ(2, id);

    EXPECT_EQ(0, registerAgent(5, id));
    EXPECT_EQ(0, registerAgent(6, id));

    EXPECT_EQ(0, clear());


    EXPECT_EQ(0, loadMap("tests/tracks/map.xodr", &id));
    EXPECT_EQ(1, id);

    EXPECT_EQ(0, registerAgent(5, id));
    EXPECT_EQ(0, registerAgent(6, id));

    EXPECT_EQ(0, loadMap("tests/tracks/CircleR100.xodr", &id));
    EXPECT_EQ(2, id);

    EXPECT_EQ(0, registerAgent(1, id));
    EXPECT_EQ(0, registerAgent(2, id));

    EXPECT_EQ(0, clear());

}