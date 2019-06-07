//
// Created by Jens Klimke on 2019-01-03.
//


#include <gtest/gtest.h>
#include <odradapter/ODRAdapter.h>
#include <fstream>


TEST(AdapterTest, LoadMap1) {

    simmap::odra::ODRAdapter odr;
    odr.loadFile("./tests/tracks/LaneSections.xodr");

}



TEST(AdapterTest, LoadMap2) {

    simmap::odra::ODRAdapter odr;
    odr.loadFile("./tests/tracks/CircleR100.xodr");

    std::fstream file("./tools/map.json", std::ios::out);
    odr.streamTo(file);

}


TEST(AdapterTest, LoadMap3) {

    simmap::odra::ODRAdapter odr;
    odr.loadFile("./tests/tracks/KA-Suedtangente-atlatec-Roadshape.xodr");

}


TEST(AdapterTest, LoadMap5) {

    simmap::odra::ODRAdapter odr;
    odr.loadFile("./tests/tracks/sample1.1.xodr");

}