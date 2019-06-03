//
// Created by Jens Klimke on 2019-02-05.
//

#include "LaneSectionSequence.h"

LaneSection* LaneSectionSequence::create(double s, def::Side side) {

    if(nestedseq::sections().empty()) {

        auto sub = nestedseq::create(s);
        return sub->first.emplace(s, LaneSection(side));

    } else {

        // get last element
        auto entry = at(s);
        auto both = entry.first.element != nullptr && entry.first.element->side() == def::Side::BOTH;

        LaneSection *ls = nullptr;

        // get element
        if (both != (side == def::Side::BOTH)) {

            // create sub-section and add lane section
            auto res = nestedseq::create(s);

            // differ between right/both and left
            if(side == def::Side::BOTH || side == def::Side::RIGHT)
                ls = res->first.emplace(s, LaneSection(side));
            else
                ls = res->second.emplace(s, LaneSection(side));

        } else if(side == def::Side::RIGHT || side == def::Side::BOTH) {

            // add lane section to the right side
            auto sec = nestedseq::sections().atPos(s);
            ls = sec.element.first.emplace(s, LaneSection(side));

        } else if(side == def::Side::LEFT) {

            // add lane section to the left side
            auto sec = nestedseq::sections().atPos(s);
            ls = sec.element.second.emplace(s, LaneSection(side));

        }

        return ls;

    }

}


LaneSectionSequence::LaneEntry LaneSectionSequence::lane(const ODREdge *lane) const {

    // check if indexed
    if(_lane_index.empty())
        index();

    return *_lane_index.at(lane);

}


ODREdge* LaneSectionSequence::lane(def::ContactPoint cp, int id) const {

    auto cs = crossSection(cp);
    return cs.lane(id);

}


const std::vector<LaneSectionSequence::LaneEntry> & LaneSectionSequence::lanes() const {

    // check if indexed
    if(_lanes.empty())
        index();

    return _lanes;

}


const std::vector<LaneSectionSequence::LaneSectionEntry> &LaneSectionSequence::laneSections() const {

    // check if indexed
    if(_lane_sections.empty())
        index();

    return _lane_sections;

}


CrossSection LaneSectionSequence::crossSection(double s) const {

    auto entries = lane_section_sequence_t::at(s);

    CrossSection cs{};
    if(entries.second.element == nullptr) {

        // set all sides
        cs.setLeft(entries.first.position, entries.first.element);
        cs.setRight(entries.first.position, entries.first.element);
        cs.setCenter(entries.first.position, entries.first.element);

    } else {

        // set right and left
        cs.setRight(entries.first.position, entries.first.element);
        cs.setLeft(entries.second.position, entries.second.element);

        // set center either from right or left section
        if(entries.first.element->center() != nullptr)
            cs.setCenter(entries.first.position, entries.first.element);
        else
            cs.setCenter(entries.second.position, entries.second.element);

    }

    return cs;

}


CrossSection LaneSectionSequence::crossSection(def::ContactPoint cp) const {

    lane_section_sequence_t::EntryPair entries;
    if(cp == def::ContactPoint::START)
        entries = lane_section_sequence_t::front();
    else
        entries = lane_section_sequence_t::back();


    CrossSection cs{};
    if(entries.second.element == nullptr) {

        // set all sides
        cs.setLeft(entries.first.position, entries.first.element);
        cs.setRight(entries.first.position, entries.first.element);
        cs.setCenter(entries.first.position, entries.first.element);

    } else {

        // set right and left
        cs.setRight(entries.first.position, entries.first.element);
        cs.setLeft(entries.second.position, entries.second.element);

        // set center either from right or left section
        if(entries.first.element->center() != nullptr)
            cs.setCenter(entries.first.position, entries.first.element);
        else
            cs.setCenter(entries.second.position, entries.second.element);

    }

    return cs;

}


void LaneSectionSequence::length(double length) {

    // set length of overall sequence
    nestedseq::length(length);

    /*// iterate over sub-sequences
    for(auto const &sub : entries()) {

        // set center length
        if(sub.element->center() != nullptr)
            sub.element->center()->_length = sub.length;

        // update right lanes
        for(auto &ln : sub.element->right())
            ln->length(sub.length);

        // update left lanes
        for(auto &ln : sub.element->left())
            ln->length(sub.length);

    }*/

}


double LaneSectionSequence::length() const {

    return nestedseq::length();

}


void LaneSectionSequence::index(bool generateIDs) const {

     // empty containers
    _lane_sections.clear();
    _lanes.clear();
    _lane_index.clear();


    // iterate over sub-sequences
    for(auto const &sub : entries()) {

        // save entry
        LaneSectionEntry entry{};
        entry.position    = sub.position;
        entry.length      = sub.length;
        entry.laneSection = sub.element;

        double s0 = entry.position;
        double s1 = entry.position + entry.length;

        // get side
        auto side = sub.element->side();

        // both sides
        if(side == def::Side::BOTH) {

            // iterate over left lanes
            int i = 0;
            for (auto ln : sub.element->left()) {

                _lanes.emplace_back(LaneEntry{ln, sub.element});
                ln->_s0 = s0;
                ln->_s1 = s1;

                if (generateIDs)
                    ln->_laneID = ++i;

            }

            // iterate over right lanes
            i = 0;
            for (auto ln : sub.element->right()) {

                _lanes.emplace_back(LaneEntry{ln, sub.element});
                ln->_s0 = s0;
                ln->_s1 = s1;

                if (generateIDs)
                    ln->_laneID = --i;

            }

        } else if(side == def::Side::LEFT) {

            // iterate over lanes
            int i = 0;
            for (auto ln : sub.element->left()) {

                _lanes.emplace_back(LaneEntry{ln, sub.element});
                ln->_s0 = s0;
                ln->_s1 = s1;

                if (generateIDs)
                    ln->_laneID = ++i;

            }

        }  else if(side == def::Side::RIGHT) {

            // iterate over lanes
            int i = 0;
            for (auto ln : sub.element->right()) {

                _lanes.emplace_back(LaneEntry{ln, sub.element});
                ln->_s0 = s0;
                ln->_s1 = s1;

                if (generateIDs)
                    ln->_laneID = --i;

            }

        }

        // save center lane
        auto ln = sub.element->center();
        if (ln != nullptr) {

            _lanes.emplace_back(LaneEntry{ln, sub.element});
            ln->_s0 = s0;
            ln->_s1 = s1;

            if (generateIDs)
                ln->_laneID = 0;

        }

        // save lane section
        _lane_sections.emplace_back(entry);

    }



    // link lane sections

    LaneSection *prevRight  = nullptr;
    LaneSection *prevCenter = nullptr;
    LaneSection *prevLeft   = nullptr;

    for(auto const &ls : _lane_sections) {


        // link lefts
        if(ls.laneSection->side() == def::Side::LEFT || ls.laneSection->side() == def::Side::BOTH) {

            // link predecessor
            ls.laneSection->links().predecessorLeft = prevLeft;

            // link successor
            if(prevLeft != nullptr)
                prevLeft->links().successorLeft = ls.laneSection;

            // update previous
            prevLeft = ls.laneSection;

        }


        // link rights
        if(ls.laneSection->side() == def::Side::RIGHT || ls.laneSection->side() == def::Side::BOTH) {

            // link predecessor
            ls.laneSection->links().predecessorRight = prevRight;

            // link successor
            if(prevRight != nullptr)
                prevRight->links().successorRight = ls.laneSection;

            // update previous
            prevRight = ls.laneSection;

        }


        // link center
        if(ls.laneSection->center() != nullptr) {

            // link predecessor
            ls.laneSection->links().predecessorCenter = prevCenter;

            // link successors
            if (prevCenter != nullptr)
                prevCenter->links().successorCenter = ls.laneSection;

            // update previous
            prevCenter = ls.laneSection;

        }


    }


    // create lane index
    for (auto &e0 : _lanes)
        _lane_index[e0.lane] = &e0;


    // link successors and predecessors
    for (auto &e0 : _lanes) {

        // get predecessor and successor lane section
        LaneSection *pred;
        LaneSection *succ;

        if (e0.lane->_laneID > 0) {
            pred = e0.laneSection->links().predecessorLeft;
            succ = e0.laneSection->links().successorLeft;
        } else if (e0.lane->_laneID == 0) {
            pred = e0.laneSection->links().predecessorCenter;
            succ = e0.laneSection->links().successorCenter;
        } else {
            pred = e0.laneSection->links().predecessorRight;
            succ = e0.laneSection->links().successorRight;
        }

        // link successors
        if (succ != nullptr) {

            // iterate over successor lanes
            for (auto s : e0.lane->_succ)
                e0.lane->link(succ->lane(s), false);

        }

        // link predecessors
        if (pred != nullptr) {

            // iterate over predecessor lanes
            for (auto s : e0.lane->_pred)
                e0.lane->link(pred->lane(s), true);

        }

    }



    // link inners and outers
    for (auto &e0 : _lanes) {

        // get data of one edge ...
        auto i0 = e0.lane->_laneID; // lane ID
        auto s0 = e0.lane->_s0;     // start of lane in road

        // set inner lane
        for (auto &e1 : _lanes) {

            // only when both lanes in same lane section
            if (e1.laneSection != e0.laneSection)
                continue;

            // get data of another edge
            auto i1 = e1.lane->_laneID; // lane ID

            // check if lane1 is inner of lane0
            if (i0 > 1 && i1 == i0 - 1)
                e0.lane->_inner = e1.lane;
            else if (i0 < -1 && i1 == i0 + 1)
                e0.lane->_inner = e1.lane;
            else if (i0 >= 1 && i1 == i0 + 1)
                e0.lane->_outer = e1.lane;
            else if (i0 <= -1 && i1 == i0 - 1)
                e0.lane->_outer = e1.lane;

        }

    }



    // link neighbors
    for (auto &e0 : _lanes) {

        // get data of one edge ...
        auto i0 = e0.lane->_laneID; // lane ID
        auto s0 = e0.lane->_s0;     // start of lane in road

        // ignore non-drivable lanes
        if (e0.lane->_drivingType != ODREdge::DrivingType::DRIVABLE)
            continue;

        // iterate over all lanes again
        for (auto &e1 : _lanes) {

            // get data of another edge
            auto i1 = e1.lane->_laneID; // lane ID
            auto s1 = e1.lane->_s0;     // start of lane in road

            // calculate start point
            auto s = s1 - s0;

            // set neighbor
            if(i0 < -1 && e0.lane->_inner == e1.lane) {

                // find first drivable inner lane
                auto in = e1.lane;
                while(in != nullptr && in->_drivingType != ODREdge::DrivingType::DRIVABLE)
                    in = const_cast<ODREdge*>(in->_inner);

                // check if inner lane exists and add as neighbor
                if(in != nullptr)
                    e0.lane->addNeighbor(s, in);

            } else if(i0 >= 1 && e0.lane->_outer == e1.lane) {

                // find first drivable outer lane
                auto ou = e1.lane;
                while(ou != nullptr && ou->_drivingType != ODREdge::DrivingType::DRIVABLE)
                    ou = const_cast<ODREdge*>(ou->_outer);

                // check if outer lane exists and add as neighbor
                if(ou != nullptr)
                    e0.lane->addNeighbor(s, ou);

            } else if(i0 == -1 && i1 == 1) {

                // find first drivable outer lane
                auto ou = e1.lane;
                while(ou != nullptr && ou->_drivingType != ODREdge::DrivingType::DRIVABLE)
                    ou = const_cast<ODREdge*>(ou->_outer);

                // only geometrically neighbored lanes
                if (ou != nullptr && s > -e1.lane->length() && s < e0.lane->length())
                    e0.lane->addNeighbor(s, ou);

            }

        }

    }

}


