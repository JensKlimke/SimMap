//
// Created by Jens Klimke on 2019-01-03.
//

#ifndef SIMMAP_GRAPH_VERTEX_H
#define SIMMAP_GRAPH_VERTEX_H

#include <vector>
#include "Edge.h"

namespace graph {


    /**
     * class Vertex
     * @author Jens Klimke jens.klimke@rwth-aachen.de
     */
    class Vertex {

        typedef std::pair<Edge *, Vertex *> Link;
        typedef std::vector<Link> LinkVector;

    private:

        LinkVector _out{};
        LinkVector _in{};


    public:

        /**
         * Default constructor
         */
        Vertex() = default;


        /**
         * Default deconstructor
         */
        virtual ~Vertex() = default;


        /**
         * Connects this vertex to another vertex by an given edge
         * @param destination Destination vertex
         * @param edge Edge to be set
         */
        void connectTo(Vertex *destination, Edge *edge);


        /**
         * Returns the outgoing links of this vertex
         * @return Outgoing links
         */
        const LinkVector &outLinks() const;


        /**
         * Returns the incoming links of this vertex
         * @return Incoming links
         */
        const LinkVector &inLinks() const;

    };

}

#endif // SIMMAP_GRAPH_VERTEX_H
