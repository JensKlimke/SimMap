//
// Created by Jens Klimke on 2019-01-03.
//

#ifndef SIMMAP_GRAPH_EDGE_H
#define SIMMAP_GRAPH_EDGE_H

#include <vector>
#include <tuple>
#include "Oriented.h"
#include "Neighbored.h"

namespace graph {

    class Object;

    /**
     * class Edge
     * @author Jens Klimke jens.klimke@rwth-aachen.de
     */
    struct Edge : public Oriented, public Neighbored {


        /** The short type name for a vector of position-object pairs */
        typedef std::vector<std::pair<double, const Object *>> ObjectsList;


        std::string _id; //!< The ID of the edge


        /**
         * Default constructor
         */
        Edge() = default;


        /**
         * Default destructor
         */
        ~Edge() override = default;


        /**
         * Returns the ID of the edge
         * @return ID of the edge
         */
        const std::string &id() const;


        /**
         * Returns the stored direction of the edge
         * @return Storage direction
         */
        bool isForward() const override;


        /**
         * Returns a list of objects, e.g. traffic signs, block marks,
         * @return List of objects
         */
        virtual ObjectsList objects() const = 0;


    };

}



#endif // SIMMAP_GRAPH_EDGE_H
