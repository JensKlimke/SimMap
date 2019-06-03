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
    LaneSection *create(double s, def::Side side);


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
    ODREdge *lane(def::ContactPoint cp, int id) const;


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
    CrossSection crossSection(def::ContactPoint cp) const;


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
