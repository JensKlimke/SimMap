#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-use-equals-delete"
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "modernize-use-equals-delete"
#pragma ide diagnostic ignored "cert-err58-cpp"
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

#include <memory>
#include <vector>
#include <gtest/gtest.h>
#include <graph/Path.h>
#include <graph/Object.h>

class BasicPathTest : public ::graph::Path, public testing::Test {

    void SetUp() override {}

    void TearDown() override {}

};


struct BasicPathObject : graph::Object {

    std::string type = "type";
    std::string label = "label";
    std::string id;

    explicit BasicPathObject(std::string _id) : id(std::move(_id)) {}

    BasicPathObject(std::string _id, std::string _type, std::string _label)
            : type(std::move(_type)), label(std::move(_label)), id(std::move(_id)) {}

    const std::string &getType() const override { return type; }

    const std::string &getLabel() const override { return label; }

    const std::string &getID() const override { return id; }

    double getValue() const override { return 0.0; }

};


struct BasicPath : ::graph::Path {

    explicit BasicPath(const ::graph::Path &p) : Path(p) {}

    const ::graph::Path::iterator_t *iterator() const { return &_it; }

    const ::graph::Path::edge_vector_t *segments() const { return &_segments; }

};


struct BasicPathEdge : public ::graph::Edge {

    double _len;
    def::Orientation _ori;

    std::vector<std::shared_ptr<BasicPathObject>> _objVector{};
    ObjectsList _objs{};

    BasicPathEdge(double len, def::Orientation ori) : _len(len), _ori(ori) {}

    double length() const override { return _len; }

    def::Orientation orientation() const override { return _ori; }

    void addObject(double s, BasicPathObject &&obj) {

        // create shared pointer
        auto ptr = std::make_shared<BasicPathObject>(obj);

        // add to lists
        _objVector.emplace_back(ptr);
        _objs.emplace_back(s, ptr.get());

    };

    ObjectsList objects() const override { return _objs; }

};


TEST_F(BasicPathTest, Copy) {

    const def::Orientation fw = def::Orientation::FORWARDS;
    const def::Orientation bw = def::Orientation::BACKWARDS;

    auto e1 = new BasicPathEdge(2.0, fw);
    auto e2 = new BasicPathEdge(1.0, bw);
    auto e3 = new BasicPathEdge(3.0, fw);

    // update segments and reset position
    _segments = {e1, e2, e3};
    _set(0, 0.5, 0.0, 0.0);

    // check
    EXPECT_EQ(_segments.begin(), _it);

    // copy path
    BasicPath p(*dynamic_cast<Path *>(this));
    EXPECT_EQ(p.segments()->begin(), *p.iterator());

}


TEST_F(BasicPathTest, OperationsHeadBack) {

    const def::Orientation fw = def::Orientation::FORWARDS;
    const def::Orientation bw = def::Orientation::BACKWARDS;

    auto e1 = new BasicPathEdge(2.0, fw);
    auto e2 = new BasicPathEdge(1.0, bw);
    auto e3 = new BasicPathEdge(3.0, fw);

    // create segments
    _segments = {e1};

    // set position, head and back to middle of edge
    _set(0, 0.5, 0.5, 0.5);

    // test distances
    EXPECT_DOUBLE_EQ(0.0, distanceToHead());
    EXPECT_DOUBLE_EQ(0.0, distanceToBack());


    // set back to start of edge and head to end of edge
    _backPos = 0.0;
    _headPos = 2.0;

    // test distances
    EXPECT_DOUBLE_EQ(1.5, distanceToHead());
    EXPECT_DOUBLE_EQ(0.5, distanceToBack());

    // update segments and reset position
    _segments = {e1, e2, e3};
    _set(1, 0.5, 0.0, 3.0);

    // test distances
    EXPECT_DOUBLE_EQ(3.5, distanceToHead());
    EXPECT_DOUBLE_EQ(2.5, distanceToBack());

    // test head and back function
    EXPECT_EQ(_segments.end(), std::next(_head().it, 1));
    EXPECT_EQ(_segments.begin(), _back().it);

    // clean up
    _segments.clear();

    // delete edges
    delete e1;
    delete e2;
    delete e3;

}


TEST_F(BasicPathTest, FindPosition) {

    const def::Orientation fw = def::Orientation::FORWARDS;
    const def::Orientation bw = def::Orientation::BACKWARDS;

    auto e1 = new BasicPathEdge(2.0, fw);
    auto e2 = new BasicPathEdge(1.0, bw);
    auto e3 = new BasicPathEdge(3.0, fw);


    // update segments
    _segments = {e1};

    // update back and head
    _set(0, 0.5, 0.5, 0.5);

    auto r = index(0.0);
    EXPECT_EQ(_it, r.it);
    EXPECT_DOUBLE_EQ(0.5, r.s);



    // update segments and reset position
    _segments = {e1, e2, e3};
    _set(1, 0.5, 0.0, 3.0);

    // check current position
    r = index(0.0);
    EXPECT_EQ(_it, r.it);
    EXPECT_DOUBLE_EQ(0.5, r.s);

    // check current position
    r = index(0.25);
    EXPECT_EQ(_it, r.it);
    EXPECT_DOUBLE_EQ(0.75, r.s);

    // check current position
    r = index(-0.25);
    EXPECT_EQ(_it, r.it);
    EXPECT_DOUBLE_EQ(0.25, r.s);

//    // check for errors
//    EXPECT_EXIT(index( 10.0), ::testing::KilledBySignal(SIGABRT), ".*");
//    EXPECT_EXIT(index(-10.0), ::testing::KilledBySignal(SIGABRT), ".*");



    // create constant
    const double eps = 1e-9;
    const double err = eps * 10.0;

    // check end of current edge
    r = index(0.5 - eps);
    EXPECT_EQ(_it, r.it);
    EXPECT_NEAR(1.0, r.s, err);

    // check end of current edge
    r = index(0.5 + eps);
//    EXPECT_EQ(std::next(_segments.end(), -1), r.it);
    EXPECT_NEAR(0.0, r.s, err);

    // check start of first edge
    r = index(3.0);
//    EXPECT_EQ(std::next(_segments.end(), -1), r.it);
    EXPECT_DOUBLE_EQ(2.5, r.s);

    // check start of first edge
    r = index(3.5);
//    EXPECT_EQ(std::next(_segments.end(), -1), r.it);
    EXPECT_DOUBLE_EQ(3.0, r.s);



    // check start of current edge
    r = index(-0.5 + eps);
    EXPECT_EQ(_it, r.it);
    EXPECT_NEAR(0.0, r.s, err);

    // check start of current edge
    r = index(-0.5 - eps);
    EXPECT_EQ(_segments.begin(), r.it);
    EXPECT_NEAR(2.0, r.s, err);

    // check start of first edge
    r = index(-2.0);
    EXPECT_EQ(_segments.begin(), r.it);
    EXPECT_DOUBLE_EQ(0.5, r.s);

    // check start of first edge
    r = index(-2.5);
    EXPECT_EQ(_segments.begin(), r.it);
    EXPECT_DOUBLE_EQ(0.0, r.s);


    _set(1, 0.0, 0.0, 3.0);

    // check current position
    r = index(0.0);
    EXPECT_EQ(_it, r.it);
    EXPECT_DOUBLE_EQ(0.0, r.s);

    // check current position
    r = index(0.5);
    EXPECT_EQ(_it, r.it);
    EXPECT_DOUBLE_EQ(0.5, r.s);

    // check start of first edge
    r = index(-0.5);
    EXPECT_EQ(_segments.begin(), r.it);
    EXPECT_DOUBLE_EQ(1.5, r.s);


    _set(0, 2.0, 0.0, 3.0);

    // check current position
    r = index(0.0);
    EXPECT_EQ(_segments.begin(), r.it);
    EXPECT_DOUBLE_EQ(2.0, r.s);

    // check current position
    r = index(0.5);
    EXPECT_EQ(std::next(_segments.begin(), 1), r.it);
    EXPECT_DOUBLE_EQ(0.5, r.s);

    // check start of first edge
    r = index(-0.5);
    EXPECT_EQ(_segments.begin(), r.it);
    EXPECT_DOUBLE_EQ(1.5, r.s);


    _set(2, 3.0, 0.0, 3.0);

    // check current position
    r = index(0.0);
    EXPECT_EQ(std::next(_segments.end(), -1), r.it);
    EXPECT_DOUBLE_EQ(3.0, r.s);

    // check start of first edge
    r = index(-6.0);
    EXPECT_EQ(_segments.begin(), r.it);
    EXPECT_DOUBLE_EQ(0.0, r.s);

    // check for errors
//    EXPECT_EXIT(index( 0.1), ::testing::KilledBySignal(SIGABRT), ".*");
//    EXPECT_EXIT(index(-6.1), ::testing::KilledBySignal(SIGABRT), ".*");


    // clean up
    _segments.clear();

    // delete edges
    delete e1;
    delete e2;
    delete e3;
}


TEST_F(BasicPathTest, Objects) {

    // define constants
    const def::Orientation fw = def::Orientation::FORWARDS;
    const def::Orientation bw = def::Orientation::BACKWARDS;

    // create edges
    auto e1 = new BasicPathEdge(2.0, fw);
    auto e2 = new BasicPathEdge(1.0, bw);
    auto e3 = new BasicPathEdge(3.0, fw);

    // add objects
    e1->addObject(0.0, {"1", "objD", "objD1"});
    e1->addObject(0.5, {"2", "objA", "objA1"});
    e1->addObject(1.0, {"3", "objB", "objB1"});
    e1->addObject(2.0, {"4", "objC", "objC1"});
    e2->addObject(0.0, {"5", "objA", "objA2"});
    e2->addObject(0.5, {"6", "objB", "objB2"});
    e2->addObject(1.0, {"7", "objC", "objC2"});
    e3->addObject(0.0, {"8", "objA", "objA3"});
    e3->addObject(1.5, {"9", "objB", "objB3"});
    e3->addObject(2.5, {"10", "objC", "objC3"});
    e3->addObject(3.0, {"11", "objD", "objD3"});

    // update segments and reset position
    _segments = {e1, e2, e3};
    _set(1, 0.5, 0.5, 2.5);

    // get objects
    auto ol = objects();
    EXPECT_EQ(9, ol.size());

    EXPECT_DOUBLE_EQ(-2.0, ol[0].first);
    EXPECT_EQ("objA", ol[0].second->getType());
    EXPECT_EQ("objA1", ol[0].second->getLabel());
    EXPECT_EQ("2", ol[0].second->getID());

    EXPECT_DOUBLE_EQ(-1.5, ol[1].first);
    EXPECT_EQ("objB", ol[1].second->getType());
    EXPECT_EQ("objB1", ol[1].second->getLabel());
    EXPECT_EQ("3", ol[1].second->getID());

    EXPECT_DOUBLE_EQ(-0.5, ol[2].first);
    EXPECT_EQ("objC", ol[2].second->getType());
    EXPECT_EQ("objC1", ol[2].second->getLabel());
    EXPECT_EQ("4", ol[2].second->getID());

    EXPECT_DOUBLE_EQ(-0.5, ol[3].first);
    EXPECT_EQ("objA", ol[3].second->getType());
    EXPECT_EQ("objA2", ol[3].second->getLabel());
    EXPECT_EQ("5", ol[3].second->getID());

    EXPECT_DOUBLE_EQ(0.0, ol[4].first);
    EXPECT_EQ("objB", ol[4].second->getType());
    EXPECT_EQ("objB2", ol[4].second->getLabel());
    EXPECT_EQ("6", ol[4].second->getID());

    EXPECT_DOUBLE_EQ(0.5, ol[5].first);
    EXPECT_EQ("objC", ol[5].second->getType());
    EXPECT_EQ("objC2", ol[5].second->getLabel());
    EXPECT_EQ("7", ol[5].second->getID());

    EXPECT_DOUBLE_EQ(0.5, ol[6].first);
    EXPECT_EQ("objA", ol[6].second->getType());
    EXPECT_EQ("objA3", ol[6].second->getLabel());
    EXPECT_EQ("8", ol[6].second->getID());

    EXPECT_DOUBLE_EQ(2.0, ol[7].first);
    EXPECT_EQ("objB", ol[7].second->getType());
    EXPECT_EQ("objB3", ol[7].second->getLabel());
    EXPECT_EQ("9", ol[7].second->getID());

    EXPECT_DOUBLE_EQ(3.0, ol[8].first);
    EXPECT_EQ("objC", ol[8].second->getType());
    EXPECT_EQ("objC3", ol[8].second->getLabel());
    EXPECT_EQ("10", ol[8].second->getID());


    // clean up
    _segments.clear();

    // delete edges
    delete e1;
    delete e2;
    delete e3;

}

#pragma clang diagnostic pop