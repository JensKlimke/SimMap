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
// Created by Jens Klimke on 2019/01/25
//

#ifndef SIMMAP_ODRADAPTER_LANESECTION_H
#define SIMMAP_ODRADAPTER_LANESECTION_H


#include "ODREdge.h"
#include <memory>
#include <iostream>

class LaneSection;

struct ODRRoad;

class InterfaceLaneSection {

public:

    typedef std::vector<ODREdge*> LanesVector;

    struct Linkage {

        // TODO: make this const?
        LaneSection *predecessorRight;
        LaneSection *predecessorLeft;
        LaneSection *successorRight;
        LaneSection *successorLeft;
        LaneSection *predecessorCenter;
        LaneSection *successorCenter;

    };


    /**
     * Constructor
     */
    InterfaceLaneSection() = default;


    /**
     * Destructor
     */
    virtual ~InterfaceLaneSection() = default;


    /**
     * Returns the reference to the vector of right lanes
     * @return Vector of right lanes
     */
    virtual const LanesVector * right() const = 0;


    /**
     * Returns the reference to the vector of left lanes
     * @return Vector of left lanes
     */
    virtual const LanesVector * left() const = 0;


    /**
     * Returns the reference to the center lane
     * @return Reference to the center lane
     */
    virtual ODREdge * center() const = 0;


    /**
     * Returns the side of the lane section
     * @return Side of the lane section
     */
    virtual base::Side side() const = 0;


    /**
     * Returns the linkage of the lane section
     * @return Linkage
     */
    virtual Linkage links() const = 0;


    /**
     * Returns the lane with the given ID
     * @param id ID of the lane
     * @return Lane
     */
    ODREdge *lane(int id) {

        if (id == 0)
            return center();
        else if (id > 0)
            return left()->at(static_cast<unsigned long>(id - 1));
        else if (id < 0)
            return right()->at(static_cast<unsigned long>(-id - 1));
        else
            return nullptr;

    }

};



class LaneSection : public InterfaceLaneSection {


private:

    LanesVector _lanesLeft{};
    LanesVector _lanesRight{};

    ODREdge *_center = nullptr;

    base::Side _side = base::Side::NONE;
    Linkage _links{};


public:

    unsigned int _id{};
    const ODRRoad *_road = nullptr;


    LaneSection(const LaneSection&) = delete;
    LaneSection(LaneSection&&) = default;
    ~LaneSection() override = default;

    /**
     * Creates a lane section for the given side and creates the required vectors
     * @param side Side to be set
     */
    explicit LaneSection(base::Side side) { _side = side; }


    const LanesVector* right() const override { return &_lanesRight; };


    const LanesVector* left() const override { return &_lanesLeft; };


    ODREdge * center() const override { return _center; };


    base::Side side() const override { return _side; };


    Linkage links() const override { return _links; }


    /**
     * Returns the lanes vector for right hand side
     * @return Lanes vector (right)
     */
    LanesVector &right() { return _lanesRight; }


    /**
     * Returns the lanes vector for left hand side
     * @return Lanes vector (left)
     */
    LanesVector &left() { return _lanesLeft; }


    /**
     * Sets the center lane
     * @param center The center lane to be set
     */
    void center(ODREdge* center) { _center = center; }


    /**
     * Returns a non-const reference to the center lane
     * @return
     */
    ODREdge* center() { return _center; }


    /**
     * Returns the links of the lane section
     * @return
     */
    Linkage &links() { return _links; }


    /**
     * Add a new lane with the given id
     * @param id ID of the lane
     * @return Lane that was created
     */
    void createLane(int id, ODREdge *edge) {

        if (id == 0) {

            // set center lane
            center(edge);

        } else if (id > 0) {

            // update size
            if(left().size() < id)
                left().resize((size_t) id);

            // create new lane and add
            left()[id - 1] = edge;

        } else if (id < 0) {

            // update size
            if (right().size() < -id)
                right().resize((size_t) -id);

            // create new lane and add
            right()[-id - 1] = edge;

        }

    }




};


class CrossSection : public InterfaceLaneSection {

private:

    const LaneSection *_lanesLeft   = nullptr;
    const LaneSection *_lanesRight  = nullptr;
    const LaneSection *_lanesCenter = nullptr;

    double _sLeft{};
    double _sRight{};
    double _sCenter{};


public:


    CrossSection() = default;


    ~CrossSection() override = default;


    const LanesVector* right() const override { return _lanesRight->right(); };


    const LanesVector* left() const override { return _lanesLeft->left(); };


    ODREdge * center() const override { return _lanesCenter->center(); };


    base::Side side() const override {

        if(_lanesLeft != nullptr && _lanesRight != nullptr)
            return base::Side::BOTH;
        else if(_lanesRight != nullptr)
            return base::Side::RIGHT;
        else if(_lanesLeft != nullptr)
            return base::Side::LEFT;
        else
            return base::Side::NONE;

    };


    Linkage links() const override {

        return Linkage{
            _lanesRight->links().predecessorRight,
            _lanesLeft->links().predecessorLeft,
            _lanesRight->links().successorRight,
            _lanesLeft->links().successorLeft,
            _lanesCenter->links().predecessorCenter,
            _lanesCenter->links().successorCenter
        };

    }


    /**
     * Sets the left lanes vector and the position
     * @param s Position
     * @param left Vector of lanes
     */
    void setLeft(double s, const LaneSection *left) {

        _lanesLeft = left;
        _sLeft = s;

    }


    /**
     * Sets the right lanes vector and the position
     * @param s Position
     * @param right Vector of lanes
     */
    void setRight(double s, const LaneSection *right) {

        _lanesRight = right;
        _sRight = s;

    }


    /**
     * Sets the center lane and the position
     * @param s Position
     * @param center Center lane
     */
    void setCenter(double s, const LaneSection *center) {

        _lanesCenter = center;
        _sCenter = s;

    }


    /**
     * Returns the position of the left lanes
     * @return Position
     */
    double sLeft() const { return _sLeft; }


    /**
     * Returns the position of the right lanes
     * @return Position
     */
    double sRight() const { return _sRight; }


    /**
     * Returns the position of the center lanes
     * @return Position
     */
    double sCenter() const { return _sCenter; }


    /**
     * Returns the right hand side lane section object
     * @return Lane section on the right hand side
     */
    const LaneSection* laneSectionRight() const { return _lanesRight; }


    /**
     * Returns the left hand side lane section object
     * @return Lane section on the left hand side
     */
    const LaneSection* laneSectionLeft() const { return _lanesLeft; }


    /**
     * Returns the center lane section object
     * @return Lane section on the center
     */
    const LaneSection* laneSectionCenter() const { return _lanesCenter; }


};



#endif // SIMMAP_ODRADAPTER_LANESECTION_H
