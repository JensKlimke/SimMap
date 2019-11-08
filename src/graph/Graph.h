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
