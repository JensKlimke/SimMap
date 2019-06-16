//
// Created by Jens Klimke on 2019-06-06.
//

#include <gtest/gtest.h>
#include <string>
#include <SimMap/lib.h>

class SpeedTest : public testing::Test {


    void SetUp() override {}

    void TearDown() override { simmap::clear(); }

};



TEST_F(SpeedTest, Targets) {

    // define number of targets and number of steps
    unsigned int n = 10;
    unsigned int m = 1000;

    // load map
    unsigned int id;
    simmap::loadMap("tests/tracks/CircleR100.xodr", &id);

    // define track
    std::vector<const char*> track = {"1", "-2"};

    // loop to create agents
    for(auto i = 0; i < n; ++i) {

        // register agent
        unsigned int aid = i + 1;
        simmap::registerAgent(aid, id);

        // set track
        simmap::setTrack(aid, track.data(), track.size());

        // define position
        simmap::MapPosition pos;
        pos.edgeID  = "R1-LS1-R1";
        pos.latPos  = 0.0;
        pos.longPos = 0.0; // i * 20.0;

        // define path lengths
        double frLen = 200.0;
        double bkLen =  50.0;

        // set map position
        simmap::setMapPosition(aid, pos, &frLen, &bkLen);

    }


    // loop to move targets
    for(auto i = 0; i < m; ++i) {

        // iterate over agents
        for(auto j = 0; j < n; ++j) {

            // get id
            unsigned int aid = j + 1;

            // set number of targets
            unsigned long nt = 32;

            // get target information
            std::vector<simmap::TargetInformation> ti(nt);
            simmap::targets(aid, ti.data(), &nt);

            // define path lengths
            double frLen = 200.0;
            double bkLen =  50.0;

            // move agent
            simmap::move(aid, 5.0, 0.0, &frLen, &bkLen);

        }

    }


}