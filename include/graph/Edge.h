//
// Created by Jens Klimke on 2019-01-03.
//

#ifndef SIMMAP_GRAPH_EDGE_H
#define SIMMAP_GRAPH_EDGE_H

#include "Oriented.h"
#include "Neighbored.h"

namespace graph {

    class Object;

    /**
     * class Edge
     * @author Jens Klimke jens.klimke@rwth-aachen.de
     */
    struct Edge : public Oriented, public Neighbored {


        std::string _id;
        typedef std::vector<std::pair<double, const Object *>> ObjectsList;


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
         * Returns a flag which defines if the given direction is compatible to the lane
         * @param ori Orientation to be test
         * @return Flag
         */
        bool isDirectionalCompatible(def::Orientation ori) const;


        /**
         * Returns a list of objects
         * @return List of objects
         */
        virtual ObjectsList objects() const = 0;


    protected:


        /**
         * Returns a ObjectList from the given vector of objects
         * @tparam T Object class
         * @param obj Object vector
         * @return An ObjectList
         */
        template<typename T>
        static inline ObjectsList _objects(const std::vector<std::pair<double, T>> &obj) {

            ObjectsList lst{};
            for (const auto &o : obj)
                lst.push_back({o.first, &o.second});

            return lst;

        }


    };

}



#endif // SIMMAP_GRAPH_EDGE_H
