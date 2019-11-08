//
// Created by Jens Klimke on 20.12.18.
//

#ifndef SIMMAP_GRAPH_NEIGHBORED_H
#define SIMMAP_GRAPH_NEIGHBORED_H

#include <base/sequence.h>

namespace graph {


    /**
     * class Neighbored
     * @author Jens Klimke jens.klimke@rwth-aachen.de
     */
    class Neighbored {

        /** A typedef to store a sequence of Element  of implementing Neighbored,
         * @see: base::sequence
         */
        typedef base::sequence<const Neighbored*> NeighborSeq;

        NeighborSeq _right; //!< The sequence of right hand side neighbors
        NeighborSeq _left;  //!< The sequence of left hand side neighbors


    public:

        /**
         * Enum representing the sides right and left
         */
        enum class Side { RIGHT, LEFT };


        /**
         * Default constructor
         */
        Neighbored() = default;


        /**
         * Default destructor
         */
        virtual ~Neighbored() = default;


        /**
         * Adds a neighbor on the left side in reference direction (not depending on this orientation). That means that
         * if this object is backwards oriented, the neighbored object is added to the right side, if forward oriented
         * it's added to the left side. Also the position is referred to the reference direction and recalculated
         * accordingly. It's added bi-directional, so this object is also added to the neighbored object.
         * @param s Position to be added
         * @param obj Object
         */
        void addNeighbor(double s, Neighbored *obj);


        /**
         * Adds a right neighbor
         * @param s Position where the neighbor starts
         * @param right Neighbor edge
         */
        void addRight(double s, Neighbored *right, bool bi = true);


        /**
         * Adds a left neighbor
         * @param s Position where the neighbor starts
         * @param left Neighbor edge
         */
        void addLeft(double s, Neighbored *left, bool bi = true);


        /**
         * Returns the neighbor edge
         * @param s Position in edge (an edge can have multiple neighbors, also with the same offset index)
         * @param side The side of the neighbor
         * @param number Number of neighbors to the given side
         * @return The neighbor edge
         */
        std::pair<double, const Neighbored *> neighbor(double s, Side side, size_t number) const;


        /**
         * Returns a pointer to the left neighbor sequence
         * @return Left neighbor sequence
         */
        const NeighborSeq & left() const;


        /**
         * Returns a pointer to the right neighbor sequence
         * @return Right neighbor sequence
         */
        const NeighborSeq & right() const;


        /**
         * Returns the length of the object
         * @return length
         */
        virtual double length() const = 0;


        /**
         * Returns a flag which defines the object to be forward oriented
         * @return Flag
         */
        virtual bool isForward() const = 0;


        /**
         * Switches the given side
         * @param sd Side to be switched
         * @return Switched side
         */
        static Side switchSide(const Side &sd);


    };

};


#endif // SIMMAP_GRAPH_NEIGHBORED_H
