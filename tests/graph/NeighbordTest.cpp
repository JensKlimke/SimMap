//
// Created by Jens Klimke on 2019-01-19.
//

#include <gtest/gtest.h>
#include <graph/Graph.h>
#include <graph/Edge.h>

class Neighbor : public ::graph::Edge {

protected:

    double _length = 0.0;
    bool _forward = true;


public:

    Neighbor() = default;
    explicit Neighbor(double len) : _length(fabs(len)), _forward(len >= 0.0) {}
    double length() const override { return _length; }

    ObjectsList objects() const override {
        return graph::Edge::ObjectsList();
    }

    def::Orientation orientation() const override {
        return _forward ? def::Orientation::FORWARDS : def::Orientation::BACKWARDS;
    }


};


class NeighboredTest : public Neighbor, public testing::Test {

public:

    NeighboredTest() = default;

    void SetUp() override {}

    void TearDown() override {}

};


TEST_F(NeighboredTest, Neighbors1) {

    /*
     * l3 +>-----1------------+
     * l2 +<-----1------------+
     * l1 +<1--+-2-+--3---+4--+
     * m       +>0-------+
     * r1 +>1-+--2-+--3--+-4--+
     * r2 +>-----1------------+
     * r3 +<-----1------------+
     *
     * <--|----|----|----|----|-->
     *         0        10
     */

    // create neighbors
    Neighbor n0(10.0);

    Neighbor nl11(-5.0);
    Neighbor nl12(-4.0);
    Neighbor nl13(-7.0);
    Neighbor nl14(-4.0);

    Neighbor nr11(4.0);
    Neighbor nr12(5.0);
    Neighbor nr13(6.0);
    Neighbor nr14(5.0);

    Neighbor nl21(-20.0);
    Neighbor nl31(20.0);
    Neighbor nr21(20.0);
    Neighbor nr31(-20.0);

    // create container and fill
    std::vector<std::pair<double, std::vector<Edge *>>> container;
    container.push_back({-5.0, {&nr31}});
    container.push_back({-5.0, {&nr21}});
    container.push_back({-5.0, {&nr11, &nr12, &nr13, &nr14}});
    container.push_back({0.0, {&n0}});
    container.push_back({-5.0, {&nl11, &nl12, &nl13, &nl14}});
    container.push_back({-5.0, {&nl21}});
    container.push_back({-5.0, {&nl31}});

    // create container
    ::graph::Graph::autoConnect(container);

    /* TODO:
    // check very right edge
    auto &n1 = nr21.left();
    auto e = n1.entries();
    ASSERT_EQ(4, n1.size());
    EXPECT_DOUBLE_EQ(0.0, e[0].position);
    EXPECT_DOUBLE_EQ(4.0, e[1].position);
    EXPECT_DOUBLE_EQ(9.0, e[2].position);
    EXPECT_DOUBLE_EQ(15.0, e[3].position);

    // check right edges
    auto &n2 = nr11.left();
    ASSERT_EQ(1, n2.size());

    auto &n3 = nr11.right();
    e = n3.entries();
    ASSERT_EQ(1, n3.size());
    EXPECT_EQ(&nr21, *e[0].element);
    EXPECT_DOUBLE_EQ(0.0, e[0].position);

    auto &n4 = nr12.left();
    e = n4.entries();
    ASSERT_EQ(1, n4.size());
    EXPECT_EQ(&n0, *e[0].element);
    EXPECT_DOUBLE_EQ(1.0, e[0].position);

    auto &n5 = nr12.right();
    e = n5.entries();
    ASSERT_EQ(1, n5.size());
    EXPECT_EQ(&nr21, *e[0].element);
    EXPECT_DOUBLE_EQ(-4.0, e[0].position);

    auto &n6 = nr14.left();
    // e = n6.entries();
    ASSERT_EQ(1, n6.size());

    auto &n7 = n0.right();
    e = n7.entries();
    ASSERT_EQ(2, n7.size());
    EXPECT_EQ(&nr12, *e[0].element);
    EXPECT_EQ(&nr13, *e[1].element);
    EXPECT_DOUBLE_EQ(-1.0, e[0].position);
    EXPECT_DOUBLE_EQ(4.0, e[1].position);

    auto &n8 = n0.left();
    e = n8.entries();
    ASSERT_EQ(2, n8.size());
    EXPECT_EQ(&nl12, *e[0].element);
    EXPECT_EQ(&nl13, *e[1].element);
    EXPECT_DOUBLE_EQ(0.0, e[0].position);
    EXPECT_DOUBLE_EQ(4.0, e[1].position);

    auto &n9 = nl13.left();
    e = n9.entries();
    ASSERT_EQ(1, n9.size());
    EXPECT_EQ(&n0, *e[0].element);
    EXPECT_DOUBLE_EQ(1.0, e[0].position);

    auto &n10 = nl13.right();
    e = n10.entries();
    ASSERT_EQ(1, n10.size());
    EXPECT_EQ(&nl21, *e[0].element);
    EXPECT_DOUBLE_EQ(-4.0, e[0].position);

    auto &n11 = nl14.left();
    // e = n11.entries();
    ASSERT_EQ(0, n11.size());

    auto &n12 = nl21.left();
    e = n12.entries();
    ASSERT_EQ(4, n12.size());
    EXPECT_EQ(&nl14, *e[0].element);
    EXPECT_EQ(&nl13, *e[1].element);
    EXPECT_EQ(&nl12, *e[2].element);
    EXPECT_EQ(&nl11, *e[3].element);
    EXPECT_DOUBLE_EQ(0.0, e[0].position);
    EXPECT_DOUBLE_EQ(4.0, e[1].position);
    EXPECT_DOUBLE_EQ(11.0, e[2].position);
    EXPECT_DOUBLE_EQ(15.0, e[3].position);

    // special cases
    auto &n13 = nr21.right();
    e = n13.entries();
    ASSERT_EQ(1, n13.size());
    EXPECT_EQ(&nr31, *e[0].element);
    EXPECT_DOUBLE_EQ(0.0, e[0].position);

    auto &n14 = nr31.right();
    e = n14.entries();
    ASSERT_EQ(1, n14.size());
    EXPECT_EQ(&nr21, *e[0].element);
    EXPECT_DOUBLE_EQ(0.0, e[0].position);

    auto &n15 = nr31.left();
    // e = n15.entries();
    ASSERT_EQ(0, n15.size());

    auto &n16 = nl21.right();
    e = n16.entries();
    ASSERT_EQ(1, n16.size());
    EXPECT_EQ(&nl31, *e[0].element);
    EXPECT_DOUBLE_EQ(0.0, e[0].position);

    auto &n17 = nl31.right();
    e = n17.entries();
    ASSERT_EQ(1, n17.size());
    EXPECT_EQ(&nl21, *e[0].element);
    EXPECT_DOUBLE_EQ(0.0, e[0].position);

    auto &n18 = nl31.left();
    // e = n18.entries();
    ASSERT_EQ(0, n18.size());*/


}
