//
// Created by Jens Klimke on 2019-01-03.
//

#ifndef SIMMAP_GRAPH_H
#define SIMMAP_GRAPH_H

#include <vector>
#include <list>
#include <map>
#include <memory>
#include <base/definitions.h>


namespace graph {

    class Edge;


    /**
     * class Graph
     * @author Jens Klimke jens.klimke@rwth-aachen.de
     */
    struct Graph : public std::map<std::string, std::shared_ptr<Edge>> {


        typedef std::pair<def::Orientation, const graph::Edge *> OrientedEdge;
        typedef std::list<OrientedEdge> EdgeList;


        Graph() = default;
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
