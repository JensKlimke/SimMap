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
// Created by Jens Klimke on 2019-02-05.
//

#ifndef SIMMAP_ODRADAPTER_LANESECTIONSEQ_H
#define SIMMAP_ODRADAPTER_LANESECTIONSEQ_H

#include <map>
#include <vector>
#include <base/nestedseq.h>
#include "LaneSection.h"

typedef base::nestedseq<LaneSection> lane_section_sequence_t;
class LaneSectionSequence : public lane_section_sequence_t {

public:


    struct LaneEntry {
        ODREdge *lane;
        LaneSection *laneSection;
    };

    struct LaneSectionEntry {
        double position{};
        double length{};
        LaneSection *laneSection{};
    };


private:

    mutable std::map<const ODREdge *, LaneEntry *> _lane_index{};
    mutable std::vector<LaneEntry> _lanes{};
    mutable std::vector<LaneSectionEntry> _lane_sections{};


public:


    /**
     * Creates a lane section with a given side
     * @param s Position of the lane section
     * @param side Side of the lane section
     * @return The created lane section
     */
    LaneSection *create(double s, base::Side side);


    /**
     * Returns the vector of lanes in the lane section
     * @return Vector of lane information structs
     */
    const std::vector<LaneEntry> & lanes() const;


    /**
     * Returns the ID of the given lane
     * @return Lane information struct
     */
    LaneEntry lane(const ODREdge* lane) const;


    /**
     * Returns the lane with the given ID at the front or back of the lane section
     * @param cp Contact point
     * @param id ID of the lane
     * @return Pointer to lane
     */
    ODREdge *lane(base::ContactPoint cp, int id) const;


    /**
     * Returns a list of lane section entries
     * @return List of lane section entries
     */
    const std::vector<LaneSectionEntry> &laneSections() const;


    /**
     * Returns the cross section of the lane section at the given position
     * @param s Position
     * @return Cross section of the lane section
     */
    CrossSection crossSection(double s) const;


    /**
     * Returns the cross section of the lane section at the given contact point
     * @param cp Contact point (can be START or END)
     * @return Cross section of the lane section
     */
    CrossSection crossSection(base::ContactPoint cp) const;


    /**
     * Sets the length of the lane section sequence (and updates all sub-elements like lanes)
     * @param length Length to be set
     */
    void length(double length) override;


    /**
     * Returns the overall length of the lane section sequence
     * @return Length of the lane section sequence
     */
    double length() const override;


    /**
     * Index the lanes of the lane section
     */
    void index(bool generateIDs = false) const;

};


#endif // SIMMAP_ODRADAPTER_LANESECTIONSEQ_H
