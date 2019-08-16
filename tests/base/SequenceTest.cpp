/*
 * SequenceTest.h
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
#include <base/sequence.h>

class SequenceTest : public base::sequence<int>, public testing::Test {

    void SetUp() override {}

    void TearDown() override {}

};


TEST_F(SequenceTest, OrganizeElements1) {

    this->clear();

    // check length etc. of empty sequence
    EXPECT_DOUBLE_EQ(0.0, this->length());
    ASSERT_TRUE(this->empty());
    ASSERT_EQ(0, this->size());
    ASSERT_EQ(1, this->points().size());
    EXPECT_EQ(INFINITY, this->points().at(0));

    // update length (move s(0))
    this->length(10.0);
    EXPECT_EQ(this->length(), 0.0);
    ASSERT_TRUE(this->empty());
    ASSERT_EQ(0, this->size());
    ASSERT_EQ(1, this->points().size());
    EXPECT_DOUBLE_EQ(10.0, this->points()[0]);

    // check clearance
    this->clear();
    EXPECT_EQ(0, this->size());
    EXPECT_EQ(INFINITY, this->points()[0]);

    // set sequence manually
    this->length(1.0);
    this->emplace_back(1.0, 7);
    this->emplace_back(1.0, 1);
    this->emplace_back(1.0, 2);
    this->emplace_back(1.0, 5);
    this->emplace_back(1.0, 3);
    this->emplace_back(1.0, 6);
    this->length(7.0);

    auto e = this->entries();

    ASSERT_EQ(6, this->size());
    EXPECT_DOUBLE_EQ(7.0, this->length());

    EXPECT_DOUBLE_EQ(1.0, e[0].position);
    EXPECT_DOUBLE_EQ(2.0, e[1].position);
    EXPECT_DOUBLE_EQ(3.0, e[2].position);
    EXPECT_DOUBLE_EQ(4.0, e[3].position);
    EXPECT_DOUBLE_EQ(5.0, e[4].position);
    EXPECT_DOUBLE_EQ(6.0, e[5].position);

    EXPECT_DOUBLE_EQ(1.0, e[0].length);
    EXPECT_DOUBLE_EQ(1.0, e[1].length);
    EXPECT_DOUBLE_EQ(1.0, e[2].length);
    EXPECT_DOUBLE_EQ(1.0, e[3].length);
    EXPECT_DOUBLE_EQ(1.0, e[4].length);
    EXPECT_DOUBLE_EQ(2.0, e[5].length);

    EXPECT_EQ(7, *e[0].element);
    EXPECT_EQ(1, *e[1].element);
    EXPECT_EQ(2, *e[2].element);
    EXPECT_EQ(5, *e[3].element);
    EXPECT_EQ(3, *e[4].element);
    EXPECT_EQ(6, *e[5].element);

}


TEST_F(SequenceTest, OrganizeElements2) {

    this->clear();

    // set sequence manually
    this->length(1.0);
    this->emplace_back(1.0, 7); // del
    this->emplace_back(1.0, 1); // 0
    this->emplace_back(1.0, 2); // 1
    this->emplace_back(1.0, 5); // del
    this->emplace_back(1.0, 3); // 2
    this->emplace_back(1.0, 6); // del

    // check deletion
    this->erase(std::next(this->begin(), 0));
    this->erase(std::next(this->begin(), 2));
    this->erase(std::next(this->begin(), 3));

    // check length
    EXPECT_EQ(3, this->size());
    EXPECT_DOUBLE_EQ(5.0, this->length());

    auto e = this->entries();

    ASSERT_EQ(3, this->size());
    EXPECT_DOUBLE_EQ(5.0, this->length());

    EXPECT_DOUBLE_EQ(2.0, e[0].position);
    EXPECT_DOUBLE_EQ(3.0, e[1].position);
    EXPECT_DOUBLE_EQ(5.0, e[2].position);

    EXPECT_DOUBLE_EQ(1.0, e[0].length);
    EXPECT_DOUBLE_EQ(2.0, e[1].length);
    EXPECT_DOUBLE_EQ(2.0, e[2].length);

    EXPECT_EQ(1, *e[0].element);
    EXPECT_EQ(2, *e[1].element);
    EXPECT_EQ(3, *e[2].element);

}


TEST_F(SequenceTest, OrganizeElements3) {

    this->clear();

    // set sequence manually
    this->length(1.0);
    this->append(1.0, 7); // del
    this->append(1.0, 1); // 0
    this->append(1.0, 2); // 1
    this->append(1.0, 5); // del
    this->append(1.0, 3); // 2
    this->append(1.0, 6); // del

    // check deletion
    this->erase(std::next(this->begin(), 0));
    this->erase(std::next(this->begin(), 2));
    this->erase(std::next(this->begin(), 3));

    // check length
    EXPECT_EQ(3, this->size());
    EXPECT_DOUBLE_EQ(5.0, this->length());

    auto e = this->entries();

    ASSERT_EQ(3, this->size());
    EXPECT_DOUBLE_EQ(5.0, this->length());

    EXPECT_DOUBLE_EQ(2.0, e[0].position);
    EXPECT_DOUBLE_EQ(3.0, e[1].position);
    EXPECT_DOUBLE_EQ(5.0, e[2].position);

    EXPECT_DOUBLE_EQ(1.0, e[0].length);
    EXPECT_DOUBLE_EQ(2.0, e[1].length);
    EXPECT_DOUBLE_EQ(2.0, e[2].length);

    EXPECT_EQ(1, *e[0].element);
    EXPECT_EQ(2, *e[1].element);
    EXPECT_EQ(3, *e[2].element);

}


TEST_F(SequenceTest, OrganizeElements4) {

    // check deletion
    this->clear();
    EXPECT_EQ(this->length(), 0.0);
    EXPECT_TRUE(this->empty());

    // set initial point
    this->length(0.0);

    // append
    this->emplace_back(1.0, 1);
    this->emplace_back(1.0, 2);
    this->emplace_back(1.0, 3);

    // set length
    this->length(5.0);

    // insert
    this->emplace(-1.0, 0);
    this->emplace(1.5, 15);
    this->emplace(2.5, 25);
    this->emplace(4.0, 40);

    // check number and length
    ASSERT_EQ(7, this->size());
    EXPECT_DOUBLE_EQ(6.0, this->length());

    std::vector<double> s = {-1.0, 0.0, 1.0, 1.5, 2.0, 2.5, 4.0, 5.0};
    std::vector<int> v = {0, 1, 2, 15, 3, 25, 40};

    size_t i = 0;
    for(auto const &e : *this) {

        EXPECT_NEAR(s[i], e.position, 1e-12);
        EXPECT_EQ(v[i], e.element);
        EXPECT_DOUBLE_EQ(s[i + 1] - s[i], e.length);

        i++;

    }

}


TEST_F(SequenceTest, CheckExistance) {

    // set sequence manually
    this->length(1.0);
    this->emplace_back(1.0, 7);
    this->emplace_back(1.0, 1);
    this->emplace_back(1.0, 2);
    this->emplace_back(1.0, 5);
    this->emplace_back(1.0, 3);
    this->emplace_back(1.0, 6);

    EXPECT_TRUE(this->exists(1.0, 7));
    EXPECT_TRUE(this->exists(4.0, 5));
    EXPECT_TRUE(this->exists(6.0, 6));
    EXPECT_TRUE(this->exists(1.1, 7));
    EXPECT_TRUE(this->exists(4.1, 5));
    EXPECT_TRUE(this->exists(6.2, 6));

    EXPECT_FALSE(this->exists(3.0, 3));


}


TEST_F(SequenceTest, AccessElements) {

    // set sequence manually
    this->length(1.0);
    this->emplace_back(1.0, 7);
    this->emplace_back(1.0, 1);
    this->emplace_back(1.0, 2);
    this->emplace_back(1.0, 5);
    this->emplace_back(1.0, 3);
    this->emplace_back(1.0, 6);

    // positions of first and last element
    EXPECT_EQ(1.0, this->startPosition());
    EXPECT_EQ(7.0, this->endPosition());

    // check element and position with at spacial access
    EXPECT_EQ(7, this->atPos(1.0).element);
    EXPECT_NEAR(0.0, this->atPos(1.0).position, def::EPS_DISTANCE);
    EXPECT_EQ(2, this->atPos(3.999).element);
    EXPECT_NEAR(0.999, this->atPos(3.999).position, def::EPS_DISTANCE);
    EXPECT_EQ(5, this->atPos(4.001).element);
    EXPECT_NEAR(0.001, this->atPos(4.001).position, def::EPS_DISTANCE);
    EXPECT_EQ(6, this->atPos(7.0).element);
    EXPECT_NEAR(1.0, this->atPos(7.0).position, def::EPS_DISTANCE);

    // check wrong access
    EXPECT_THROW(this->atPos(0.999999), std::invalid_argument);
    EXPECT_THROW(this->atPos(7.000001), std::invalid_argument);

}


TEST_F(SequenceTest, ChangeElements) {

    // set sequence manually
    this->length(1.0);
    this->emplace_back(1.0, 7);
    this->emplace_back(1.0, 1);
    this->emplace_back(1.0, 2);
    this->emplace_back(1.0, 5);
    this->emplace_back(1.0, 3);
    this->emplace_back(1.0, 6);

    this->atPos(2.2).element = 10;

    EXPECT_EQ(7,  *this->entries()[0].element);
    EXPECT_EQ(10, *this->entries()[1].element);
    EXPECT_EQ(2,  *this->entries()[2].element);

}
