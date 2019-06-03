//
// Created by klimke on 09.05.2019.
//

#include <map>
#include "ODRRoad.h"
#include "ODRJunction.h"
#include <odr/odr1_5_structure.h>


void parseLinks(const odr1_5::OpenDRIVE *odr,
                std::map<std::string, std::shared_ptr<ODRRoad>> &roads,
                const std::map<std::string, std::shared_ptr<ODRJunction>> &juncs) {

    for (auto &r : odr->sub_road) {

        // get road
        auto road = roads.at(*r._id);

        // get predecessor
        auto pred = r.sub_link->sub_predecessor;
        if (pred) {

            // get id and contact point
            auto &id = *pred->_elementId;

            // get element type
            auto &et = *pred->_elementType;
            if (et == "road") {

                // get road and start lane section
                auto pr = roads.at(*pred->_elementId).get();
                auto ls = road->lanes.crossSection(def::ContactPoint::START);

                // iterate over lanes
                for (auto ln : *ls.laneSectionRight()->right()) {

                    // iterate over predecessors
                    for (auto lid : ln->_pred) {

                        // get contact point and lane
                        auto cp = *pred->_contactPoint == "end" ? def::ContactPoint::END : def::ContactPoint::START;
                        auto lsc = pr->lanes.lane(cp, lid);

                        // link lane
                        ln->link(lsc, true);

                    }

                }


                // link center lanes
                if(ls.laneSectionCenter()->center() != nullptr) {

                    // get lane
                    auto ln = ls.laneSectionCenter()->center();

                    // iterate over predecessors
                    for (auto lid : ln->_pred) {

                        // get contact point and lane
                        auto cp = *pred->_contactPoint == "end" ? def::ContactPoint::END : def::ContactPoint::START;
                        auto lsc = pr->lanes.lane(cp, lid);

                        // link lane
                        ln->prev(lsc, def::ContactPoint::START, false);

                    }

                }


                // iterate over lanes
                for (auto ln : *ls.laneSectionLeft()->left()) {

                    // iterate over predecessors
                    for (auto lid : ln->_pred) {

                        // get contact point and lane
                        auto cp = *pred->_contactPoint == "end" ? def::ContactPoint::END : def::ContactPoint::START;
                        auto lsc = pr->lanes.lane(cp, lid);

                        // link lane
                        ln->link(lsc, true);

                    }

                }

            }

        }

        // get successor
        auto succ = r.sub_link->sub_successor;
        if (succ) {

            // get id and contact point
            auto &id = *succ->_elementId;

            // get element type
            auto &et = *succ->_elementType;
            if (et == "road") {

                // get road and start lane section
                auto pr = roads.at(*succ->_elementId).get();
                auto ls = road->lanes.crossSection(def::ContactPoint::END);

                // iterate over lanes
                for (auto ln : *ls.laneSectionRight()->right()) {

                    // iterate over successors
                    for (auto lid : ln->_succ) {

                        // get contact point and lane
                        auto cp = *succ->_contactPoint == "end" ? def::ContactPoint::END : def::ContactPoint::START;
                        auto lsc = pr->lanes.lane(cp, lid);

                        // link lane
                        ln->link(lsc, false);

                    }

                }


                // link center lanes
                if(ls.laneSectionCenter()->center() != nullptr) {

                    // get lane
                    auto ln = ls.laneSectionCenter()->center();

                    // iterate over predecessors
                    for (auto lid : ln->_succ) {

                        // get contact point and lane
                        auto cp = *succ->_contactPoint == "end" ? def::ContactPoint::END : def::ContactPoint::START;
                        auto lsc = pr->lanes.lane(cp, lid);

                        // link lane
                        ln->next(lsc, def::ContactPoint::END, false);

                    }

                }


                // iterate over lanes
                for (auto ln : *ls.laneSectionLeft()->left()) {

                    // iterate over successors
                    for (auto lid : ln->_succ) {

                        // get contact point and lane
                        auto cp = *succ->_contactPoint == "end" ? def::ContactPoint::END : def::ContactPoint::START;
                        auto lsc = pr->lanes.lane(cp, lid);

                        // link lane
                        ln->link(lsc, false);

                    }

                }

            }

        }

    }

}
