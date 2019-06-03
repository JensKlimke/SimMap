//
// Created by klimke on 27.09.2018.
//

#include <gtest/gtest.h>
#include <graph/Oriented.h>


class OrientedTest : public testing::Test {

public:

    struct OrObj : ::graph::Oriented {
        def::Orientation orientation() const override { return def::Orientation::BACKWARDS; }
    };


protected:

    OrObj *A = nullptr, *B = nullptr, *C = nullptr, *D = nullptr;


public:

    void SetUp() override {

        // create local variables
        A = new OrObj;
        B = new OrObj;
        C = new OrObj;
        D = new OrObj;

    }

    void TearDown() override {

        delete A;
        delete B;
        delete C;
        delete D;

    }

    void connectManually() {

        /*
         *            +--->2-----+
         *            |          |
         *          A |          | B
         *            |          |
         *     ------>1------>3<-+
         *       D        C
         *
         */


        A->link(B, true); // linking backwards, because edges have backward orientation
        A->next(B, def::ContactPoint::START); // double connection to check if this is avoided
        B->next(C, def::ContactPoint::END);
        C->prev(A, def::ContactPoint::START);
        A->link(D);
        C->link(D);

    }


};


TEST_F(OrientedTest, Connection) {

    using namespace ::graph;

    connectManually();

    EXPECT_EQ(A->prevs().size(), 2);
    EXPECT_EQ(B->prevs().size(), 1);
    EXPECT_EQ(C->prevs().size(), 2);
    EXPECT_EQ(D->prevs().size(), 0);

    EXPECT_EQ(A->nexts().size(), 1);
    EXPECT_EQ(B->nexts().size(), 1);
    EXPECT_EQ(C->nexts().size(), 1);
    EXPECT_EQ(D->nexts().size(), 2);

    if (A->prevs().at(0).second == C) {
        EXPECT_EQ(A->prevs().at(0), Oriented::OPair({def::ContactPoint::START, C}));
        EXPECT_EQ(A->prevs().at(1), Oriented::OPair({def::ContactPoint::END, D}));
    } else {
        EXPECT_EQ(A->prevs().at(1), Oriented::OPair({def::ContactPoint::START, C}));
        EXPECT_EQ(A->prevs().at(0), Oriented::OPair({def::ContactPoint::END, D}));
    }

    EXPECT_EQ(B->prevs().at(0), Oriented::OPair({def::ContactPoint::END, A}));

    if (C->prevs().at(0).second == A) {
        EXPECT_EQ(C->prevs().at(0), Oriented::OPair({def::ContactPoint::START, A}));
        EXPECT_EQ(C->prevs().at(1), Oriented::OPair({def::ContactPoint::END, D}));
    } else {
        EXPECT_EQ(C->prevs().at(1), Oriented::OPair({def::ContactPoint::START, A}));
        EXPECT_EQ(C->prevs().at(0), Oriented::OPair({def::ContactPoint::END, D}));
    }

    EXPECT_EQ(A->nexts().at(0), Oriented::OPair({def::ContactPoint::START, B}));
    EXPECT_EQ(B->nexts().at(0), Oriented::OPair({def::ContactPoint::END, C}));
    EXPECT_EQ(C->nexts().at(0), Oriented::OPair({def::ContactPoint::END, B}));

    if (D->nexts().at(0).second == A) {
        EXPECT_EQ(D->nexts().at(0), Oriented::OPair({def::ContactPoint::START, A}));
        EXPECT_EQ(D->nexts().at(1), Oriented::OPair({def::ContactPoint::START, C}));
    } else {
        EXPECT_EQ(D->nexts().at(1), Oriented::OPair({def::ContactPoint::START, A}));
        EXPECT_EQ(D->nexts().at(0), Oriented::OPair({def::ContactPoint::START, C}));
    }

}

