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

}