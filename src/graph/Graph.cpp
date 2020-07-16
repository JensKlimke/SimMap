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

#include "Graph.h"
#include "Edge.h"
#include <iostream>

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

                        // do not set neighbors out of bounds
                        if(s02 - s0 + edge2->length() > 1e-15 && s02 - s0 < edge->length() - 1e-15)
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