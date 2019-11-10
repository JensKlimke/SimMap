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

#ifndef SIMMAP_GRAPH_H
#define SIMMAP_GRAPH_H

#include <string>
#include <list>
#include <tuple>
#include <map>
#include <memory>
#include <base/definitions.h>


namespace graph {

    // pre-define edge class
    class Edge;


    /**
     * class Graph
     * @author Jens Klimke jens.klimke@rwth-aachen.de
     */
    struct Graph : public std::map<std::string, std::shared_ptr<Edge>> {


        /** A short definition of an orientation stored together with and edge */
        typedef std::pair<def::Orientation, const graph::Edge *> OrientedEdge;

        /** A typedef to store a list of oriented edges */
        typedef std::list<OrientedEdge> EdgeList;


        /**
         * Default constructor
         */
        Graph() = default;


        /**
         * Default destructor
         */
        virtual ~Graph() = default;


        /**
         * Constructor fro copying String-Edge maps
         * @param e String-Edge map
         */
        explicit Graph(std::map<std::string, std::shared_ptr<Edge>> &&e)
            : std::map<std::string, std::shared_ptr<Edge>>(e) {}

        /**
         * Connects the given sequences of edges such that they have their correct links and
         * @param edges Edges to be connected
         */
        static void autoConnect(const std::vector<std::pair<double, std::vector<Edge *>>> &edges);


    };

}


#endif // SIMMAP_GRAPH_H
