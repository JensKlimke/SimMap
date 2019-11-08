//
// Created by Jens Klimke on 20.12.18.
//

#include <gtest/gtest.h>
#include <odradapter/ODRAdapter.h>
#include <graph/Edge.h>
#include <server/MapCoordinate.h>
#include <odradapter/ODRRoad.h>
#include <base/functions.h>

static const double R = 100.0;



TEST(EdgeNetworkTest, LinksAndObjects) {

    using namespace simmap::odra;

    // create map and segment
    ODRAdapter map{};
    map.loadFile(base::string_format("%s/CircleR100.xodr", TRACKS_DIR));

    // check network
    ASSERT_EQ(15, map._laneNetwork.size());

    auto R1_LS1_L2 = dynamic_cast<ODREdge *>(map._laneNetwork.at("R1-LS1-L2").get());
    auto R1_LS1_L1 = dynamic_cast<ODREdge *>(map._laneNetwork.at("R1-LS1-L1").get());
    auto R1_LS1_R1 = dynamic_cast<ODREdge *>(map._laneNetwork.at("R1-LS1-R1").get());
    auto R1_LS1_R2 = dynamic_cast<ODREdge *>(map._laneNetwork.at("R1-LS1-R2").get());
    auto R1_LS2_L2 = dynamic_cast<ODREdge *>(map._laneNetwork.at("R1-LS2-L2").get());
    auto R1_LS2_L1 = dynamic_cast<ODREdge *>(map._laneNetwork.at("R1-LS2-L1").get());
    auto R1_LS2_R1 = dynamic_cast<ODREdge *>(map._laneNetwork.at("R1-LS2-R1").get());
    auto R1_LS2_R2 = dynamic_cast<ODREdge *>(map._laneNetwork.at("R1-LS2-R2").get());
    auto R2_LS1_L2 = dynamic_cast<ODREdge *>(map._laneNetwork.at("R2-LS1-L2").get());
    auto R2_LS1_L1 = dynamic_cast<ODREdge *>(map._laneNetwork.at("R2-LS1-L1").get());
    auto R2_LS1_R1 = dynamic_cast<ODREdge *>(map._laneNetwork.at("R2-LS1-R1").get());
    auto R2_LS1_R2 = dynamic_cast<ODREdge *>(map._laneNetwork.at("R2-LS1-R2").get());
    auto R1_LS1_C  = dynamic_cast<ODREdge *>(map._laneNetwork.at("R1-LS1-C").get());
    auto R1_LS2_C  = dynamic_cast<ODREdge *>(map._laneNetwork.at("R1-LS2-C").get());
    auto R2_LS1_C  = dynamic_cast<ODREdge *>(map._laneNetwork.at("R2-LS1-C").get());


    EXPECT_EQ(def::Orientation::BACKWARDS, R1_LS1_L2->orientation());
    EXPECT_EQ(def::Orientation::BACKWARDS, R1_LS1_L1->orientation());
    EXPECT_EQ(def::Orientation::FORWARDS, R1_LS1_R1->orientation());
    EXPECT_EQ(def::Orientation::FORWARDS, R1_LS1_R2->orientation());
    EXPECT_EQ(def::Orientation::BACKWARDS, R1_LS2_L2->orientation());
    EXPECT_EQ(def::Orientation::BACKWARDS, R1_LS2_L1->orientation());
    EXPECT_EQ(def::Orientation::FORWARDS, R1_LS2_R1->orientation());
    EXPECT_EQ(def::Orientation::FORWARDS, R1_LS2_R2->orientation());
    EXPECT_EQ(def::Orientation::BACKWARDS, R2_LS1_L2->orientation());
    EXPECT_EQ(def::Orientation::BACKWARDS, R2_LS1_L1->orientation());
    EXPECT_EQ(def::Orientation::FORWARDS, R2_LS1_R1->orientation());
    EXPECT_EQ(def::Orientation::FORWARDS, R2_LS1_R2->orientation());
    EXPECT_EQ(def::Orientation::NONE, R1_LS1_C->orientation());
    EXPECT_EQ(def::Orientation::NONE, R1_LS2_C->orientation());
    EXPECT_EQ(def::Orientation::NONE, R2_LS1_C->orientation());


    // check successors and predecessors

    EXPECT_EQ(1, R1_LS1_L2->nexts().size());
    EXPECT_EQ(1, R1_LS1_L1->nexts().size());
    EXPECT_EQ(1, R1_LS1_R1->nexts().size());
    EXPECT_EQ(1, R1_LS1_R2->nexts().size());
    EXPECT_EQ(1, R1_LS2_L2->nexts().size());
    EXPECT_EQ(1, R1_LS2_L1->nexts().size());
    EXPECT_EQ(1, R1_LS2_R1->nexts().size());
    EXPECT_EQ(1, R1_LS2_R2->nexts().size());
    EXPECT_EQ(1, R2_LS1_L2->nexts().size());
    EXPECT_EQ(1, R2_LS1_L1->nexts().size());
    EXPECT_EQ(1, R2_LS1_R1->nexts().size());
    EXPECT_EQ(1, R2_LS1_R2->nexts().size());
    EXPECT_EQ(1, R1_LS1_C->nexts().size());
    EXPECT_EQ(1, R1_LS2_C->nexts().size());
    EXPECT_EQ(1, R2_LS1_C->nexts().size());

    EXPECT_EQ(1, R1_LS1_L2->prevs().size());
    EXPECT_EQ(1, R1_LS1_L1->prevs().size());
    EXPECT_EQ(1, R1_LS1_R1->prevs().size());
    EXPECT_EQ(1, R1_LS1_R2->prevs().size());
    EXPECT_EQ(1, R1_LS2_L2->prevs().size());
    EXPECT_EQ(1, R1_LS2_L1->prevs().size());
    EXPECT_EQ(1, R1_LS2_R1->prevs().size());
    EXPECT_EQ(1, R1_LS2_R2->prevs().size());
    EXPECT_EQ(1, R2_LS1_L2->prevs().size());
    EXPECT_EQ(1, R2_LS1_L1->prevs().size());
    EXPECT_EQ(1, R2_LS1_R1->prevs().size());
    EXPECT_EQ(1, R2_LS1_R2->prevs().size());
    EXPECT_EQ(1, R1_LS1_C->prevs().size());
    EXPECT_EQ(1, R1_LS2_C->prevs().size());
    EXPECT_EQ(1, R2_LS1_C->prevs().size());


    // check inner and outer

    EXPECT_EQ(nullptr, R1_LS1_R1->_inner);
    EXPECT_EQ(nullptr, R1_LS2_R1->_inner);
    EXPECT_EQ(nullptr, R2_LS1_R1->_inner);
    EXPECT_EQ(nullptr, R1_LS1_L1->_inner);
    EXPECT_EQ(nullptr, R1_LS2_L1->_inner);
    EXPECT_EQ(nullptr, R2_LS1_L1->_inner);

    EXPECT_EQ(R1_LS1_R1, R1_LS1_R2->_inner);
    EXPECT_EQ(R1_LS2_R1, R1_LS2_R2->_inner);
    EXPECT_EQ(R2_LS1_R1, R2_LS1_R2->_inner);
    EXPECT_EQ(R1_LS1_L1, R1_LS1_L2->_inner);
    EXPECT_EQ(R1_LS2_L1, R1_LS2_L2->_inner);
    EXPECT_EQ(R2_LS1_L1, R2_LS1_L2->_inner);

    EXPECT_EQ(R1_LS1_R2, R1_LS1_R1->_outer);
    EXPECT_EQ(R1_LS2_R2, R1_LS2_R1->_outer);
    EXPECT_EQ(R2_LS1_R2, R2_LS1_R1->_outer);
    EXPECT_EQ(R1_LS1_L2, R1_LS1_L1->_outer);
    EXPECT_EQ(R1_LS2_L2, R1_LS2_L1->_outer);
    EXPECT_EQ(R2_LS1_L2, R2_LS1_L1->_outer);


    // neighbors

    EXPECT_EQ(R1_LS1_R1, R1_LS1_R2->left().front().element);
    EXPECT_EQ(R1_LS2_R1, R1_LS2_R2->left().front().element);
    EXPECT_EQ(R2_LS1_R1, R2_LS1_R2->left().front().element);
    EXPECT_EQ(R1_LS1_L1, R1_LS1_L2->left().front().element);
    EXPECT_EQ(R1_LS2_L1, R1_LS2_L2->left().front().element);
    EXPECT_EQ(R2_LS1_L1, R2_LS1_L2->left().front().element);

    EXPECT_EQ(0.0, R1_LS1_R2->left().front().position);
    EXPECT_EQ(0.0, R1_LS2_R2->left().front().position);
    EXPECT_EQ(0.0, R2_LS1_R2->left().front().position);
    EXPECT_EQ(0.0, R1_LS1_L2->left().front().position);
    EXPECT_EQ(0.0, R1_LS2_L2->left().front().position);
    EXPECT_EQ(0.0, R2_LS1_L2->left().front().position);

    EXPECT_EQ(R1_LS1_R2, R1_LS1_R1->right().front().element);
    EXPECT_EQ(R1_LS2_R2, R1_LS2_R1->right().front().element);
    EXPECT_EQ(R2_LS1_R2, R2_LS1_R1->right().front().element);
    EXPECT_EQ(R1_LS1_L2, R1_LS1_L1->right().front().element);
    EXPECT_EQ(R1_LS2_L2, R1_LS2_L1->right().front().element);
    EXPECT_EQ(R2_LS1_L2, R2_LS1_L1->right().front().element);

    EXPECT_EQ(0.0, R1_LS1_R1->right().front().position);
    EXPECT_EQ(0.0, R1_LS2_R1->right().front().position);
    EXPECT_EQ(0.0, R2_LS1_R1->right().front().position);
    EXPECT_EQ(0.0, R1_LS1_L1->right().front().position);
    EXPECT_EQ(0.0, R1_LS2_L1->right().front().position);
    EXPECT_EQ(0.0, R2_LS1_L1->right().front().position);

    EXPECT_EQ(R1_LS1_L1, R1_LS1_R1->left().front().element);
    EXPECT_EQ(R1_LS2_L1, R1_LS2_R1->left().front().element);
    EXPECT_EQ(R2_LS1_L1, R2_LS1_R1->left().front().element);
    EXPECT_EQ(R1_LS1_R1, R1_LS1_L1->left().front().element);
    EXPECT_EQ(R1_LS2_R1, R1_LS2_L1->left().front().element);
    EXPECT_EQ(R2_LS1_R1, R2_LS1_L1->left().front().element);

    EXPECT_EQ(0.0, R1_LS1_R1->left().front().position);
    EXPECT_EQ(0.0, R1_LS2_R1->left().front().position);
    EXPECT_EQ(0.0, R2_LS1_R1->left().front().position);
    EXPECT_EQ(0.0, R1_LS1_L1->left().front().position);
    EXPECT_EQ(0.0, R1_LS2_L1->left().front().position);
    EXPECT_EQ(0.0, R2_LS1_L1->left().front().position);


    // predecessors

    EXPECT_EQ(R1_LS2_L2, R1_LS1_L2->prevs().front().second);
    EXPECT_EQ(R1_LS2_L1, R1_LS1_L1->prevs().front().second);
    EXPECT_EQ(R2_LS1_L1, R1_LS1_R1->prevs().front().second);
    EXPECT_EQ(R2_LS1_L2, R1_LS1_R2->prevs().front().second);
    EXPECT_EQ(R2_LS1_R2, R1_LS2_L2->prevs().front().second);
    EXPECT_EQ(R2_LS1_R1, R1_LS2_L1->prevs().front().second);
    EXPECT_EQ(R1_LS1_R1, R1_LS2_R1->prevs().front().second);
    EXPECT_EQ(R1_LS1_R2, R1_LS2_R2->prevs().front().second);
    EXPECT_EQ(R1_LS2_R2, R2_LS1_L2->prevs().front().second);
    EXPECT_EQ(R1_LS2_R1, R2_LS1_L1->prevs().front().second);
    EXPECT_EQ(R1_LS1_L1, R2_LS1_R1->prevs().front().second);
    EXPECT_EQ(R1_LS1_L2, R2_LS1_R2->prevs().front().second);


    // successors

    EXPECT_EQ(R1_LS1_L2, R1_LS2_L2->nexts().front().second);
    EXPECT_EQ(R1_LS1_L1, R1_LS2_L1->nexts().front().second);
    EXPECT_EQ(R1_LS1_R1, R2_LS1_L1->nexts().front().second);
    EXPECT_EQ(R1_LS1_R2, R2_LS1_L2->nexts().front().second);
    EXPECT_EQ(R1_LS2_L2, R2_LS1_R2->nexts().front().second);
    EXPECT_EQ(R1_LS2_L1, R2_LS1_R1->nexts().front().second);
    EXPECT_EQ(R1_LS2_R1, R1_LS1_R1->nexts().front().second);
    EXPECT_EQ(R1_LS2_R2, R1_LS1_R2->nexts().front().second);
    EXPECT_EQ(R2_LS1_L2, R1_LS2_R2->nexts().front().second);
    EXPECT_EQ(R2_LS1_L1, R1_LS2_R1->nexts().front().second);
    EXPECT_EQ(R2_LS1_R1, R1_LS1_L1->nexts().front().second);
    EXPECT_EQ(R2_LS1_R2, R1_LS1_L2->nexts().front().second);


    // check track elements

    auto rd1 = dynamic_cast<ODRRoad *>(map._roadNetwork.at("1").get());
    auto rd2 = dynamic_cast<ODRRoad *>(map._roadNetwork.at("2").get());

    EXPECT_EQ(rd1, R1_LS1_L2->trackElement().second);
    EXPECT_EQ(rd1, R1_LS1_L1->trackElement().second);
    EXPECT_EQ(rd1, R1_LS1_R1->trackElement().second);
    EXPECT_EQ(rd1, R1_LS1_R2->trackElement().second);
    EXPECT_EQ(rd1, R1_LS2_L2->trackElement().second);
    EXPECT_EQ(rd1, R1_LS2_L1->trackElement().second);
    EXPECT_EQ(rd1, R1_LS2_R1->trackElement().second);
    EXPECT_EQ(rd1, R1_LS2_R2->trackElement().second);
    EXPECT_EQ(rd2, R2_LS1_L2->trackElement().second);
    EXPECT_EQ(rd2, R2_LS1_L1->trackElement().second);
    EXPECT_EQ(rd2, R2_LS1_R1->trackElement().second);
    EXPECT_EQ(rd2, R2_LS1_R2->trackElement().second);


    // check lengths

    EXPECT_DOUBLE_EQ(R * 0.5 * M_PI, R1_LS1_L2->length());
    EXPECT_DOUBLE_EQ(R * 0.5 * M_PI, R1_LS1_L1->length());
    EXPECT_DOUBLE_EQ(R * 0.5 * M_PI, R1_LS1_R1->length());
    EXPECT_DOUBLE_EQ(R * 0.5 * M_PI, R1_LS1_R2->length());
    EXPECT_DOUBLE_EQ(R * 0.5 * M_PI, R1_LS2_L2->length());
    EXPECT_DOUBLE_EQ(R * 0.5 * M_PI, R1_LS2_L1->length());
    EXPECT_DOUBLE_EQ(R * 0.5 * M_PI, R1_LS2_R1->length());
    EXPECT_DOUBLE_EQ(R * 0.5 * M_PI, R1_LS2_R2->length());
    EXPECT_DOUBLE_EQ(R * M_PI, R2_LS1_L2->length());
    EXPECT_DOUBLE_EQ(R * M_PI, R2_LS1_L1->length());
    EXPECT_DOUBLE_EQ(R * M_PI, R2_LS1_R1->length());
    EXPECT_DOUBLE_EQ(R * M_PI, R2_LS1_R2->length());


    // objects

    auto o = R1_LS1_R1->objects();
    ASSERT_EQ(2, o.size());

    EXPECT_DOUBLE_EQ(100.0, o[0].first);
    EXPECT_EQ("3", o[0].second->getID());
    EXPECT_EQ("signal", o[0].second->getType());
    EXPECT_EQ("274-50", o[0].second->getLabel());

    EXPECT_DOUBLE_EQ(110.0, o[1].first);
    EXPECT_EQ("1", o[1].second->getID());
    EXPECT_EQ("signal", o[1].second->getType());
    EXPECT_EQ("276", o[1].second->getLabel());


    o = R1_LS1_R2->objects();
    ASSERT_EQ(2, o.size());

    EXPECT_DOUBLE_EQ(100.0, o[0].first);
    EXPECT_EQ("3", o[0].second->getID());
    EXPECT_EQ("signal", o[0].second->getType());
    EXPECT_EQ("274-50", o[0].second->getLabel());

    EXPECT_DOUBLE_EQ(110.0, o[1].first);
    EXPECT_EQ("1", o[1].second->getID());
    EXPECT_EQ("signal", o[1].second->getType());
    EXPECT_EQ("276", o[1].second->getLabel());


    o = R1_LS2_R1->objects();
    ASSERT_EQ(1, o.size());

    EXPECT_DOUBLE_EQ(200.0 - M_PI_2 * 100.0, o[0].first);
    EXPECT_EQ("2", o[0].second->getID());
    EXPECT_EQ("signal", o[0].second->getType());
    EXPECT_EQ("274-70", o[0].second->getLabel());


    o = R1_LS2_R2->objects();
    ASSERT_EQ(1, o.size());

    EXPECT_DOUBLE_EQ(200.0 - M_PI_2 * 100.0, o[0].first);
    EXPECT_EQ("2", o[0].second->getID());
    EXPECT_EQ("signal", o[0].second->getType());
    EXPECT_EQ("274-70", o[0].second->getLabel());


    o = R2_LS1_R1->objects();
    ASSERT_EQ(2, o.size());

    EXPECT_DOUBLE_EQ(5.0, o[0].first);
    EXPECT_EQ("4", o[0].second->getID());
    EXPECT_EQ("signal", o[0].second->getType());
    EXPECT_EQ("206", o[0].second->getLabel());

    EXPECT_DOUBLE_EQ(100.0, o[1].first);
    EXPECT_EQ("5", o[1].second->getID());
    EXPECT_EQ("signal", o[1].second->getType());
    EXPECT_EQ("274-50", o[1].second->getLabel());


    o = R2_LS1_R2->objects();
    ASSERT_EQ(2, o.size());

    EXPECT_DOUBLE_EQ(5.0, o[0].first);
    EXPECT_EQ("4", o[0].second->getID());
    EXPECT_EQ("signal", o[0].second->getType());
    EXPECT_EQ("206", o[0].second->getLabel());

    EXPECT_DOUBLE_EQ(100.0, o[1].first);
    EXPECT_EQ("5", o[1].second->getID());
    EXPECT_EQ("signal", o[1].second->getType());
    EXPECT_EQ("274-50", o[1].second->getLabel());


    o = R2_LS1_L1->objects();
    ASSERT_EQ(1, o.size());

    EXPECT_DOUBLE_EQ(M_PI * 100.0 - 10.0, o[0].first);
    EXPECT_EQ("4", o[0].second->getID());
    EXPECT_EQ("signal", o[0].second->getType());
    EXPECT_EQ("206", o[0].second->getLabel());

}


TEST(EdgeNetworkTest, EdgeNeighbors) {

    using namespace simmap::odra;

    // create map and segment
    ODRAdapter map{};
    map.loadFile(base::string_format("%s/LaneSections.xodr", TRACKS_DIR));

    // road and lane section
    auto *road = dynamic_cast<ODRRoad *>(map._roadNetwork.at("1").get());

    // check no of edges
    EXPECT_EQ(20, map._laneNetwork.size());

    // get edges
    auto R1_LS1_R1 = map._laneNetwork.at("R1-LS1-R1");
    auto R1_LS1_L1 = map._laneNetwork.at("R1-LS1-L1");
    auto R1_LS2_R1 = map._laneNetwork.at("R1-LS2-R1");
    auto R1_LS2_L1 = map._laneNetwork.at("R1-LS2-L1");
    auto R1_LS3_R1 = map._laneNetwork.at("R1-LS3-R1");
    auto R1_LS4_L1 = map._laneNetwork.at("R1-LS4-L1");
    auto R1_LS5_L1 = map._laneNetwork.at("R1-LS5-L1");
    auto R1_LS5_L2 = map._laneNetwork.at("R1-LS5-L2");
    auto R1_LS6_L1 = map._laneNetwork.at("R1-LS6-L1");
    auto R1_LS6_L2 = map._laneNetwork.at("R1-LS6-L2");
    auto R1_LS7_R1 = map._laneNetwork.at("R1-LS7-R1");
    auto R1_LS8_L1 = map._laneNetwork.at("R1-LS8-L1");
    auto R1_LS8_L2 = map._laneNetwork.at("R1-LS8-L2");
    auto R1_LS9_L1 = map._laneNetwork.at("R1-LS9-L1");
    auto R1_LS9_R1 = map._laneNetwork.at("R1-LS9-R1");


    // check correctness of connection
    auto e = R1_LS1_R1.get();
    auto l = e->left().points();
    auto r = e->right().points();

    EXPECT_EQ(2, l.size());
    EXPECT_DOUBLE_EQ(0.0, l[0]);
    EXPECT_DOUBLE_EQ(10.0, l[1]);
    EXPECT_EQ(R1_LS1_L1.get(), e->left().front().element);
    EXPECT_EQ(1, r.size());
    EXPECT_DOUBLE_EQ(INFINITY, r[0]);


    e = R1_LS1_L1.get();
    l = e->left().points();
    r = e->right().points();

    EXPECT_EQ(2, l.size());
    EXPECT_DOUBLE_EQ(0.0, l[0]);
    EXPECT_DOUBLE_EQ(10.0, l[1]);
    EXPECT_EQ(R1_LS1_R1.get(), e->left().front().element);
    EXPECT_EQ(1, r.size());
    EXPECT_DOUBLE_EQ(INFINITY, r[0]);


    e = R1_LS2_R1.get();
    l = e->left().points();
    r = e->right().points();

    EXPECT_EQ(2, l.size());
    EXPECT_DOUBLE_EQ(0.0, l[0]);
    EXPECT_DOUBLE_EQ(15.0, l[1]);
    EXPECT_EQ(R1_LS2_L1.get(), e->left().front().element);
    EXPECT_EQ(1, r.size());
    EXPECT_DOUBLE_EQ(INFINITY, r[0]);


    e = R1_LS2_L1.get();
    l = e->left().points();
    r = e->right().points();

    EXPECT_EQ(2, l.size());
    EXPECT_DOUBLE_EQ(0.0, l[0]);
    EXPECT_DOUBLE_EQ(15.0, l[1]);
    EXPECT_EQ(R1_LS2_R1.get(), e->left().front().element);
    EXPECT_EQ(1, r.size());
    EXPECT_DOUBLE_EQ(INFINITY, r[0]);


    e = R1_LS3_R1.get();
    l = e->left().points();
    r = e->right().points();

    EXPECT_EQ(4, l.size());
    EXPECT_DOUBLE_EQ(0.0, l[0]);
    EXPECT_DOUBLE_EQ(5.0, l[1]);
    EXPECT_DOUBLE_EQ(35.0, l[2]);
    EXPECT_DOUBLE_EQ(50.0, l[3]);
    EXPECT_EQ(R1_LS4_L1.get(), e->left().atPos(l[0] + 1e-3).element);
    EXPECT_EQ(R1_LS5_L1.get(), e->left().atPos(l[1] + 1e-3).element);
    EXPECT_EQ(R1_LS6_L2.get(), e->left().atPos(l[2] + 1e-3).element);
    EXPECT_EQ(1, r.size());
    EXPECT_DOUBLE_EQ(INFINITY, r[0]);


    e = R1_LS4_L1.get();
    l = e->left().points();
    r = e->right().points();

    EXPECT_EQ(2, l.size());
    EXPECT_DOUBLE_EQ(-45.0, l[0]);
    EXPECT_DOUBLE_EQ(5.0, l[1]);
    EXPECT_EQ(R1_LS3_R1.get(), e->left().front().element);
    EXPECT_EQ(1, r.size());
    EXPECT_DOUBLE_EQ(INFINITY, r[0]);


    e = R1_LS5_L1.get();
    l = e->left().points();
    r = e->right().points();

    EXPECT_EQ(2, l.size());
    EXPECT_DOUBLE_EQ(-15.0, l[0]);
    EXPECT_DOUBLE_EQ(35.0, l[1]);
    EXPECT_EQ(R1_LS3_R1.get(), e->left().front().element);
    EXPECT_EQ(2, r.size());
    EXPECT_DOUBLE_EQ(0.0, r[0]);
    EXPECT_DOUBLE_EQ(30.0, r[1]);
    EXPECT_EQ(R1_LS5_L2.get(), e->right().front().element);


    e = R1_LS5_L2.get();
    l = e->left().points();
    r = e->right().points();

    EXPECT_EQ(2, l.size());
    EXPECT_DOUBLE_EQ(0.0, l[0]);
    EXPECT_DOUBLE_EQ(30.0, l[1]);
    EXPECT_EQ(R1_LS5_L1.get(), e->left().front().element);
    EXPECT_EQ(1, r.size());
    EXPECT_DOUBLE_EQ(INFINITY, r[0]);


    e = R1_LS6_L1.get();
    l = e->left().points();
    r = e->right().points();

    EXPECT_EQ(1, l.size());
    EXPECT_DOUBLE_EQ(INFINITY, l[0]);
    EXPECT_EQ(1, r.size());
    EXPECT_DOUBLE_EQ(INFINITY, r[0]);


    e = R1_LS6_L2.get();
    l = e->left().points();
    r = e->right().points();

    EXPECT_EQ(2, l.size());
    EXPECT_DOUBLE_EQ(0.0, l[0]);
    EXPECT_DOUBLE_EQ(50.0, l[1]);
    EXPECT_EQ(R1_LS3_R1.get(), e->left().front().element);
    EXPECT_EQ(1, r.size());
    EXPECT_DOUBLE_EQ(INFINITY, r[0]);


    e = R1_LS7_R1.get();
    l = e->left().points();
    r = e->right().points();

    EXPECT_EQ(2, l.size());
    EXPECT_DOUBLE_EQ(0.0, l[0]);
    EXPECT_DOUBLE_EQ(15.0, l[1]);
    EXPECT_EQ(1, r.size());
    EXPECT_DOUBLE_EQ(INFINITY, r[0]);


    e = R1_LS8_L1.get();
    l = e->left().points();
    r = e->right().points();

    EXPECT_EQ(2, l.size());
    EXPECT_DOUBLE_EQ(0.0, l[0]);
    EXPECT_DOUBLE_EQ(15.0, l[1]);
    EXPECT_EQ(2, r.size());
    EXPECT_DOUBLE_EQ(0.0, r[0]);
    EXPECT_DOUBLE_EQ(15.0, r[1]);


    e = R1_LS8_L2.get();
    l = e->left().points();
    r = e->right().points();

    EXPECT_EQ(2, l.size());
    EXPECT_DOUBLE_EQ(0.0, l[0]);
    EXPECT_DOUBLE_EQ(15.0, l[1]);
    EXPECT_EQ(1, r.size());
    EXPECT_DOUBLE_EQ(INFINITY, r[0]);


    e = R1_LS9_R1.get();
    l = e->left().points();
    r = e->right().points();

    EXPECT_EQ(2, l.size());
    EXPECT_DOUBLE_EQ(0.0, l[0]);
    EXPECT_DOUBLE_EQ(10.0, l[1]);
    EXPECT_EQ(1, r.size());
    EXPECT_DOUBLE_EQ(INFINITY, r[0]);


    e = R1_LS9_L1.get();
    l = e->left().points();
    r = e->right().points();

    EXPECT_EQ(2, l.size());
    EXPECT_DOUBLE_EQ(0.0, l[0]);
    EXPECT_DOUBLE_EQ(10.0, l[1]);
    EXPECT_EQ(1, r.size());
    EXPECT_DOUBLE_EQ(INFINITY, r[0]);

}



TEST(EdgeTest, CurveAndLaneOffset) {

    using namespace simmap::odra;

    // create map and segment
    ODRAdapter map{};
    map.loadFile(base::string_format("%s/CircleR100.xodr", TRACKS_DIR));

    // check network
    ASSERT_EQ(2, map._roadNetwork.size());

    // get roads
    auto road1 = reinterpret_cast<ODRRoad *>(map._roadNetwork.at("1").get());
    auto road2 = reinterpret_cast<ODRRoad *>(map._roadNetwork.at("2").get());

    // check IDs
    EXPECT_EQ("1", road1->id());
    EXPECT_EQ("2", road2->id());

    // check lengths
    EXPECT_NEAR(M_PI * R, road1->_curve->length(), def::EPS_DISTANCE);
    EXPECT_NEAR(M_PI * R, road2->_curve->length(), def::EPS_DISTANCE);

    // check positions
    EXPECT_NEAR(R, road1->_curve->operator()(0.0).position.x(), def::EPS_DISTANCE);
    EXPECT_NEAR(0.0, road1->_curve->operator()(0.0).position.y(), def::EPS_DISTANCE);
    EXPECT_NEAR(0.0, road1->_curve->operator()(0.5 * M_PI * R).position.x(), def::EPS_DISTANCE);
    EXPECT_NEAR(R, road1->_curve->operator()(0.5 * M_PI * R).position.y(), def::EPS_DISTANCE);

    // check lane offset
    EXPECT_TRUE(road1->_offset);
    EXPECT_TRUE(road2->_offset);
    EXPECT_DOUBLE_EQ(0.0, road1->_offset->operator()(0.0));
    EXPECT_DOUBLE_EQ(0.0, road1->_offset->operator()(road1->length()));

}



TEST(EdgeTest, LanePositions) {

    using namespace simmap::server;
    using namespace simmap::odra;

    // create map and segment
    ODRAdapter map{};
    map.loadFile(base::string_format("%s/example_simple.xodr", TRACKS_DIR));

    // check coordinates
    MapCoordinate mc(map.getEdge("R1-LS1-R2"), 0.0, 0.0);
    auto pos = mc.absolutePosition();
    EXPECT_NEAR(5.625, pos.position.x(), def::EPS_DISTANCE);
    EXPECT_NEAR(0.0, pos.position.y(), def::EPS_DISTANCE);
    EXPECT_NEAR(0.5 * M_PI, pos.angle, def::EPS_DISTANCE);

    mc = MapCoordinate(map.getEdge("R1-LS1-R1"), 0.0, 0.0);
    pos = mc.absolutePosition();
    EXPECT_NEAR(1.875, pos.position.x(), def::EPS_DISTANCE);
    EXPECT_NEAR(0.0, pos.position.y(), def::EPS_DISTANCE);
    EXPECT_NEAR(0.5 * M_PI, pos.angle, def::EPS_DISTANCE);

    mc = MapCoordinate(map.getEdge("R1-LS1-L1"), 10.0, 0.0);
    pos = mc.absolutePosition();
    EXPECT_NEAR(-1.75, pos.position.x(), def::EPS_DISTANCE);
    EXPECT_NEAR(0.0, pos.position.y(), def::EPS_DISTANCE);
    EXPECT_NEAR(1.5 * M_PI, pos.angle, def::EPS_DISTANCE);

    mc = MapCoordinate(map.getEdge("R1-LS1-L2"), 10.0, 0.0);
    pos = mc.absolutePosition();
    EXPECT_NEAR(-5.375, pos.position.x(), def::EPS_DISTANCE);
    EXPECT_NEAR(0.0, pos.position.y(), def::EPS_DISTANCE);
    EXPECT_NEAR(1.5 * M_PI, pos.angle, def::EPS_DISTANCE);



    // check coordinates
    mc = MapCoordinate(map.getEdge("R1-LS6-R2"), 40.0, 0.0);
    pos = mc.absolutePosition();
    EXPECT_NEAR(5.625, pos.position.x(), def::EPS_DISTANCE);
    EXPECT_NEAR(200.0, pos.position.y(), def::EPS_DISTANCE);
    EXPECT_NEAR(0.5 * M_PI, pos.angle, def::EPS_DISTANCE);

    mc = MapCoordinate(map.getEdge("R1-LS6-R1"), 40.0, 0.0);
    pos = mc.absolutePosition();
    EXPECT_NEAR(1.875, pos.position.x(), def::EPS_DISTANCE);
    EXPECT_NEAR(200.0, pos.position.y(), def::EPS_DISTANCE);
    EXPECT_NEAR(0.5 * M_PI, pos.angle, def::EPS_DISTANCE);

    mc = MapCoordinate(map.getEdge("R1-LS6-L1"), 0.0, 0.0);
    pos = mc.absolutePosition();
    EXPECT_NEAR(-1.75, pos.position.x(), def::EPS_DISTANCE);
    EXPECT_NEAR(200.0, pos.position.y(), def::EPS_DISTANCE);
    EXPECT_NEAR(1.5 * M_PI, pos.angle, def::EPS_DISTANCE);

    mc = MapCoordinate(map.getEdge("R1-LS6-L2"), 0.0, 0.0);
    pos = mc.absolutePosition();
    EXPECT_NEAR(-5.375, pos.position.x(), def::EPS_DISTANCE);
    EXPECT_NEAR(200.0, pos.position.y(), def::EPS_DISTANCE);
    EXPECT_NEAR(1.5 * M_PI, pos.angle, def::EPS_DISTANCE);

    mc = MapCoordinate(map.getEdge("R1-LS6-L3"), 0.0, 0.0);
    pos = mc.absolutePosition();
    EXPECT_NEAR(-9.125, pos.position.x(), def::EPS_DISTANCE);
    EXPECT_NEAR(200.0, pos.position.y(), def::EPS_DISTANCE);
    EXPECT_NEAR(1.5 * M_PI, pos.angle, def::EPS_DISTANCE);



    // check coordinate
    mc = MapCoordinate(map.getEdge("R1-LS2-L2"), 2.0, 0.0);
    pos = mc.absolutePosition();
    EXPECT_NEAR(-7.055, pos.position.x(), 0.01);
    EXPECT_NEAR(108.0, pos.position.y(), def::EPS_DISTANCE);
    EXPECT_NEAR(1.5 * M_PI, pos.angle, def::EPS_DISTANCE);

    mc = MapCoordinate(map.getEdge("R1-LS3-R2"), 5.0, 0.0);
    pos = mc.absolutePosition();
    EXPECT_NEAR(3.157, pos.position.x(), 0.01);
    EXPECT_NEAR(115.0, pos.position.y(), def::EPS_DISTANCE);
    EXPECT_NEAR(0.5 * M_PI, pos.angle, def::EPS_DISTANCE);

    mc = MapCoordinate(map.getEdge("R1-LS5-L4"), 8.0, 0.0);
    pos = mc.absolutePosition();
    EXPECT_NEAR(-8.611, pos.position.x(), 0.01);
    EXPECT_NEAR(152.0, pos.position.y(), def::EPS_DISTANCE);
    EXPECT_NEAR(1.5 * M_PI, pos.angle, def::EPS_DISTANCE);

}