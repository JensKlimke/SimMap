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
// Created by Jens Klimke on 2019-01-05.
//

#ifndef SIMMAP_GRAPH_PATH_H
#define SIMMAP_GRAPH_PATH_H

#include <vector>
#include <list>
#include <cmath>
#include "Edge.h"

namespace graph {

    class Object;

    /**
     * class Path
     * @author Jens Klimke jens.klimke@rwth-aachen.de
     */
    class Path {


    protected:

        /** Type definition for a list of edges. */
        typedef std::list<const Edge *> edge_vector_t;

        /** Type definition for an iterator of edge lists. */
        typedef edge_vector_t::const_iterator iterator_t;

        /**
         * Structure to store an edge list iterator and a position.
         * An object of this type is called path position.
         */
        struct position_t {
            iterator_t it;
            double s{};
        };

        /** Type definition to store a vector of path positions. */
        typedef std::vector<position_t> position_vector_t;



        edge_vector_t _segments; //!< The container to store the edges of the path

        iterator_t _it{}; //!< An iterator to store the edge of the path, in which the current position is defined TODO: use position_t to store position
        double _s = 0.0;  //!< The position in the current edge

        double _backPos = 0.0; //!< The position in the last edge, where the path ends
        double _headPos = 0.0; //!< The position in the first edge, where the path begins


    public:

        /** Type definition for object list (only a reference to the edge object list type definition) */
        typedef Edge::ObjectsList ObjectsList;


        /**
         * Default constructor
         */
        Path() = default;


        /**
         * Copy constructor
         * @param p Path to be copied
         */
        Path(const Path &p);


        /**
         * Default destructor
         */
        virtual ~Path() = default;


        /**
         * Calculates the length of the path (from the current point to the end)
         * @return Length of the path
         */
        double distanceToHead() const;


        /**
         * Calculates the length of the path from the current position to the back
         * @return Length of the path
         */
        double distanceToBack() const;


        /**
         * Updates the current position by a long. position in the path
         * @param s New position
         */
        void position(double s);


        /**
         * Returns a list of objects belonging to the path
         * @return List of objects
         */
        ObjectsList objects() const;


        /**
         * Stream the path information to the string
         * @param os Out stream
         * @return The stream
         */
        virtual std::ostream & streamTo(std::ostream &os) const;



    protected:


        /**
         * Sets the position to the ith segment and the local position to s. Additionally the back and head are set to the
         * given local position on the first and last segment respectively.
         * @param i The index of the segment, the position will be set to
         * @param s The local position
         * @param back The back position
         * @param head The head position
         */
        void _set(size_t i, double s, double back, double head);


        /**
         * Returns the index of the element at the given position
         * @param s Position (will be updated to the local position in final edge)
         */
        position_t index(double s) const;


        /**
         * Returns the index of the element at the given position (forward direction)
         * @param ds Position (will be updated to the local position in final edge)
         */
        iterator_t index_fw(double &s) const;


        /**
         * Returns the index of the element at the given position (backward direction)
         * @param ds Position (will be updated to the local position in final edge)
         */
        iterator_t index_bw(double &s) const;


        /**
         * Returns the current position in the path
         * @return Position in the path
         */
        position_t _position() const;


        /**
         * Returns the head of the path
         * @return Head of the path as position struct
         */
        position_t _head() const;


        /**
         * Returns the head of the path
         * @return Head of the path as position struct
         */
        position_t _back() const;


        /**
         * Returns the front part (body) of the path. The position elements store the iterator of the regarding segments
         * and the length from the position to the end of the segment (or distance to head position for the last segment)
         * @return Vector of position objects
         */
        position_vector_t _body(double stopAt = INFINITY) const;


        /**
         * Returns the back part (tail) of the path. The position elements store the iterator of the regarding segments
         * and the length from the position to the start of the segment (or distance to back position for the last segment)
         * The sign is positive for each distance
         * @return Vector of position objects
         */
        position_vector_t _tail(double stopAt = INFINITY) const;

    };

}


std::ostream & operator<<(std::ostream &os, const graph::Path& p);


#endif // SIMMAP_GRAPH_PATH_H
