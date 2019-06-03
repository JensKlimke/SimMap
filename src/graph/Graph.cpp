//
// Created by Jens Klimke on 2019-01-03.
//

#include "Graph.h"
#include "Edge.h"

namespace graph {


    void Graph::autoConnect(const std::vector<std::pair<double, std::vector<Edge *>>> &edges) {

        // iterate over sequences
        for (auto it = edges.begin(); it != edges.end(); ++it) {

            // get entry
            auto &seq = *it;

            // initialize variables
            auto s0 = seq.first;
            Edge *previous = nullptr;

            // get next iterator
            auto it2 = std::next(it);

            // iterate over edges
            for (auto &edge : seq.second) {

                // link edges
                if (previous != nullptr)
                    previous->link(edge, false);

                // link neighbors
                if (it2 != edges.end()) {

                    // get entry and start point
                    auto &seq2 = *it2;
                    auto s02 = seq2.first;

                    // iterate over edges
                    for (auto &edge2 : seq2.second) {

                        // calculate start point in edge
                        edge->addNeighbor(s02 - s0, edge2);

                        // next s
                        s02 += edge2->length();

                    }

                }


                // next s and previous edge
                s0 += edge->length();
                previous = edge;

            }


        }

    }


}