//
// Created by Jens Klimke on 2019-01-03.
//


#include <gtest/gtest.h>
#include <odradapter/ODRAdapter.h>
#include <fstream>
#include <base/functions.h>


TEST(AdapterTest, LoadMap1) {

    simmap::odra::ODRAdapter odr;
    odr.loadFile(base::string_format("%s/LaneSections.xodr", TRACKS_DIR));

}



TEST(AdapterTest, LoadMap2) {

    simmap::odra::ODRAdapter odr;
    odr.loadFile(base::string_format("%s/CircleR100.xodr", TRACKS_DIR));


}


TEST(AdapterTest, LoadMap3) {

    simmap::odra::ODRAdapter odr;
    odr.loadFile(base::string_format("%s/KA-Suedtangente-atlatec-Roadshape.xodr", TRACKS_DIR));

}


TEST(AdapterTest, LoadMap5) {

    simmap::odra::ODRAdapter odr;
    odr.loadFile(base::string_format("%s/sample1.1.xodr", TRACKS_DIR));

}


TEST(AdapterTest, LoadMap6) {

    simmap::odra::ODRAdapter odr;
    odr.loadFile(base::string_format("%s/map.xodr", TRACKS_DIR));

}


TEST(AdapterTest, LoadMap7) {

    simmap::odra::ODRAdapter odr;
    odr.loadFile(base::string_format("%s/Straight10000.xodr", TRACKS_DIR));

    std::fstream file(base::string_format("%s/map.json", TRACKS_DIR), std::ios::out);
    odr.streamTo(file);

}