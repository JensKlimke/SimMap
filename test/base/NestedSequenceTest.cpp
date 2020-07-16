/*
 * NestedSequenceTest.h
 *
 * MIT License
 *
 * Copyright (c) 2019 Jens Klimke <jens.klimke@rwth-aachen.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 *         of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 *         to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *         copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 *         copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *         AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <gtest/gtest.h>
#include <base/nestedseq.h>

class NestedSequenceTest : public base::nestedseq<int>, public testing::Test {

    void SetUp() override {


        /*
         *
         * I  |-+-+-|-----+-|---|
         * II |-----|-+-+---|-+-|
         *    1 2 3 4 5 6 7 8 9 10
         */

        // create sub sequences
        auto a = this->create(1.0);
        auto b = this->create(4.0);
        auto c = this->create(8.0);

        // set sub sequences
        a->first.emplace(1.0, 10);
        a->first.emplace(2.0, 11);
        a->first.emplace(3.0, 12);
        a->second.emplace(1.0, 20);

        b->first.emplace(4.0, 13);
        b->first.emplace(7.0, 16);
        b->second.emplace(4.0, 23);
        b->second.emplace(5.0, 24);
        b->second.emplace(6.0, 25);

        c->first.emplace(8.0, 17);
        c->second.emplace(8.0, 27);
        c->second.emplace(9.0, 28);

        // set length
        this->length(9.0);

    }

    void TearDown() override {}

};


TEST(NestedSequenceNaiveTest, Creation) {

    base::nestedseq<int> n{};

    // create element without content
    n.create(0.0);
    n.length(10.0);

    // get element
    auto tmp = n.at(5.0);

    // check dummy content
    EXPECT_EQ(nullptr, tmp.first.element);
    EXPECT_EQ(INFINITY, tmp.first.length);
    EXPECT_EQ(INFINITY, tmp.first.position);
    EXPECT_EQ(base::nestedseq<int>::Side::FIRST, tmp.first.side);
    EXPECT_EQ(nullptr, tmp.second.element);
    EXPECT_EQ(INFINITY, tmp.second.length);
    EXPECT_EQ(INFINITY, tmp.second.position);
    EXPECT_EQ(base::nestedseq<int>::Side::SECOND, tmp.second.side);

}


TEST_F(NestedSequenceTest, CreateElements) {

    // check lengths
    EXPECT_DOUBLE_EQ(9.0, this->length());

    auto secs = this->sections().entries();

    EXPECT_DOUBLE_EQ(3.0, secs.at(0).element->first.length());
    EXPECT_DOUBLE_EQ(4.0, secs.at(1).element->first.length());
    EXPECT_DOUBLE_EQ(2.0, secs.at(2).element->first.length());

    EXPECT_DOUBLE_EQ(3.0, secs.at(0).element->second.length());
    EXPECT_DOUBLE_EQ(4.0, secs.at(1).element->second.length());
    EXPECT_DOUBLE_EQ(2.0, secs.at(2).element->second.length());

}


TEST_F(NestedSequenceTest, FrontAndBack) {

    auto front = this->front();

    EXPECT_EQ(10, *front.first.element);
    EXPECT_EQ(20, *front.second.element);
    EXPECT_EQ(Side::FIRST, front.first.side);
    EXPECT_EQ(Side::SECOND, front.second.side);
    EXPECT_DOUBLE_EQ(0.0, front.first.position);
    EXPECT_DOUBLE_EQ(0.0, front.second.position);
    EXPECT_DOUBLE_EQ(1.0, front.first.length);
    EXPECT_DOUBLE_EQ(3.0, front.second.length);


    auto back  = this->back();

    EXPECT_EQ(17, *back.first.element);
    EXPECT_EQ(28, *back.second.element);
    EXPECT_EQ(Side::FIRST, back.first.side);
    EXPECT_EQ(Side::SECOND, back.second.side);
    EXPECT_DOUBLE_EQ(2.0, back.first.position);
    EXPECT_DOUBLE_EQ(1.0, back.second.position);
    EXPECT_DOUBLE_EQ(2.0, back.first.length);
    EXPECT_DOUBLE_EQ(1.0, back.second.length);

}


TEST_F(NestedSequenceTest, GetElements) {

    // check elements
    auto e = this->entries();
    ASSERT_EQ(12, e.size());

    // check values
    EXPECT_DOUBLE_EQ(1.0, e[0].position);
    EXPECT_DOUBLE_EQ(1.0, e[1].position);
    EXPECT_DOUBLE_EQ(2.0, e[2].position);
    EXPECT_DOUBLE_EQ(3.0, e[3].position);
    EXPECT_DOUBLE_EQ(4.0, e[4].position);
    EXPECT_DOUBLE_EQ(4.0, e[5].position);
    EXPECT_DOUBLE_EQ(5.0, e[6].position);
    EXPECT_DOUBLE_EQ(6.0, e[7].position);
    EXPECT_DOUBLE_EQ(7.0, e[8].position);
    EXPECT_DOUBLE_EQ(8.0, e[9].position);
    EXPECT_DOUBLE_EQ(8.0, e[10].position);
    EXPECT_DOUBLE_EQ(9.0, e[11].position);

    EXPECT_EQ(Side::FIRST,  e[0].side);
    EXPECT_EQ(Side::SECOND, e[1].side);
    EXPECT_EQ(Side::FIRST,  e[2].side);
    EXPECT_EQ(Side::FIRST,  e[3].side);
    EXPECT_EQ(Side::FIRST,  e[4].side);
    EXPECT_EQ(Side::SECOND, e[5].side);
    EXPECT_EQ(Side::SECOND, e[6].side);
    EXPECT_EQ(Side::SECOND, e[7].side);
    EXPECT_EQ(Side::FIRST,  e[8].side);
    EXPECT_EQ(Side::FIRST,  e[9].side);
    EXPECT_EQ(Side::SECOND, e[10].side);
    EXPECT_EQ(Side::SECOND, e[11].side);

}



TEST_F(NestedSequenceTest, CrossSection) {

    // cross section
    auto cs = this->at(1.0);
    EXPECT_DOUBLE_EQ(0.0, cs.first.position);
    EXPECT_DOUBLE_EQ(0.0, cs.second.position);
    EXPECT_DOUBLE_EQ(1.0, cs.first.length);
    EXPECT_DOUBLE_EQ(3.0, cs.second.length);
    EXPECT_EQ(10, *cs.first.element);
    EXPECT_EQ(20, *cs.second.element);
    EXPECT_EQ(Side::FIRST, cs.first.side);
    EXPECT_EQ(Side::SECOND, cs.second.side);

    cs = this->at(2.5);
    EXPECT_DOUBLE_EQ(0.5, cs.first.position);
    EXPECT_DOUBLE_EQ(1.5, cs.second.position);
    EXPECT_DOUBLE_EQ(1.0, cs.first.length);
    EXPECT_DOUBLE_EQ(3.0, cs.second.length);
    EXPECT_EQ(11, *cs.first.element);
    EXPECT_EQ(20, *cs.second.element);
    EXPECT_EQ(Side::FIRST, cs.first.side);
    EXPECT_EQ(Side::SECOND, cs.second.side);

    cs = this->at(3.5);
    EXPECT_DOUBLE_EQ(0.5, cs.first.position);
    EXPECT_DOUBLE_EQ(2.5, cs.second.position);
    EXPECT_DOUBLE_EQ(1.0, cs.first.length);
    EXPECT_DOUBLE_EQ(3.0, cs.second.length);
    EXPECT_EQ(12, *cs.first.element);
    EXPECT_EQ(20, *cs.second.element);
    EXPECT_EQ(Side::FIRST, cs.first.side);
    EXPECT_EQ(Side::SECOND, cs.second.side);

    cs = this->at(4.5);
    EXPECT_DOUBLE_EQ(0.5, cs.first.position);
    EXPECT_DOUBLE_EQ(0.5, cs.second.position);
    EXPECT_DOUBLE_EQ(3.0, cs.first.length);
    EXPECT_DOUBLE_EQ(1.0, cs.second.length);
    EXPECT_EQ(13, *cs.first.element);
    EXPECT_EQ(23, *cs.second.element);
    EXPECT_EQ(Side::FIRST, cs.first.side);
    EXPECT_EQ(Side::SECOND, cs.second.side);

    cs = this->at(5.5);
    EXPECT_DOUBLE_EQ(1.5, cs.first.position);
    EXPECT_DOUBLE_EQ(0.5, cs.second.position);
    EXPECT_DOUBLE_EQ(3.0, cs.first.length);
    EXPECT_DOUBLE_EQ(1.0, cs.second.length);
    EXPECT_EQ(13, *cs.first.element);
    EXPECT_EQ(24, *cs.second.element);
    EXPECT_EQ(Side::FIRST, cs.first.side);
    EXPECT_EQ(Side::SECOND, cs.second.side);

    cs = this->at(9.5);
    EXPECT_DOUBLE_EQ(1.5, cs.first.position);
    EXPECT_DOUBLE_EQ(0.5, cs.second.position);
    EXPECT_DOUBLE_EQ(2.0, cs.first.length);
    EXPECT_DOUBLE_EQ(1.0, cs.second.length);
    EXPECT_EQ(17, *cs.first.element);
    EXPECT_EQ(28, *cs.second.element);
    EXPECT_EQ(Side::FIRST, cs.first.side);
    EXPECT_EQ(Side::SECOND, cs.second.side);

}