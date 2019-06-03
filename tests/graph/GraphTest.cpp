#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
//
// Created by Jens Klimke on 2019-01-03.
//


#include <gtest/gtest.h>
#include <graph/Graph.h>
#include <graph/Vertex.h>

class GraphTest : public ::graph::Graph, public testing::Test {

    void SetUp() override {}

    void TearDown() override {}

};


struct EEdge : public ::graph::Edge {

    double length() const override { return 1.0; }

    def::Orientation orientation() const override { return def::Orientation::FORWARDS; }

    ObjectsList objects() const override { return graph::Edge::ObjectsList(); }

};


TEST_F(GraphTest, Create) {

    using namespace ::graph;

    // create vertices
    auto *v1 = new Vertex;
    auto *v2 = new Vertex;
    auto *v3 = new Vertex;
    auto *v4 = new Vertex;

    // create edges
    Edge *eA = new EEdge;
    Edge *eB = new EEdge;
    Edge *eC = new EEdge;
    Edge *eD = new EEdge;

    // link vertices
    v4->connectTo(v1, eA);
    v1->connectTo(v2, eB);
    v2->connectTo(v3, eC);
    v3->connectTo(v1, eD);

    // check the connections

    // edge nexts
    ASSERT_EQ(1, eA->nexts().size());
    ASSERT_EQ(1, eB->nexts().size());
    ASSERT_EQ(1, eC->nexts().size());
    ASSERT_EQ(1, eD->nexts().size());

    // edge prevs
    ASSERT_EQ(0, eA->prevs().size());
    ASSERT_EQ(2, eB->prevs().size());
    ASSERT_EQ(1, eC->prevs().size());
    ASSERT_EQ(1, eD->prevs().size());

    // vertex out
    ASSERT_EQ(1, v1->outLinks().size());
    ASSERT_EQ(1, v2->outLinks().size());
    ASSERT_EQ(1, v3->outLinks().size());
    ASSERT_EQ(1, v4->outLinks().size());

    // vertex in
    ASSERT_EQ(2, v1->inLinks().size());
    ASSERT_EQ(1, v2->inLinks().size());
    ASSERT_EQ(1, v3->inLinks().size());
    ASSERT_EQ(0, v4->inLinks().size());


}

#pragma clang diagnostic pop