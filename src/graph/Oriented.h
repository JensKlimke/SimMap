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
// Created by Jens Klimke on 11.08.18.
//

#ifndef SIMMAP_GRAPH_ORIENTED_H
#define SIMMAP_GRAPH_ORIENTED_H

#include <vector>
#include <utility>
#include <algorithm>
#include <base/definitions.h>

namespace graph {

    /**
     * class Oriented
     * @author Jens Klimke jens.klimke@rwth-aachen.de
     */
    class Oriented {

    public:

        /** Type definition for pairs of a contact point and a oriented element */
        typedef std::pair<base::ContactPoint, Oriented *> OPair;

        /** Type definition for a vector of contact points with regarding oriented elements */
        typedef std::vector<OPair> Connections;


    protected:

        Connections _nexts; //!< The following elements (with contact points)
        Connections _prevs; //!< The previous elements (with contact points)


    public:


        /**
         * Constructor
         */
        Oriented() = default;


        /**
         * Destructor
         */
        virtual ~Oriented() = default;


        /**
         * Links the given object as next or previous element depending on the own direction. The function expects the
         * object to be the next object if the direction of this object is forward. The function also handles the contact
         * point of the linked object depending on its direction
         * @param obj Object to be linked
         * @param before Links the elements swapped
         */
        void link(Oriented *obj, bool before = false);


        /**
         * Sets the given object with the contact point as the successor of this element
         * @param obj Object to be set as successor
         * @param contactPoint Contact point of the successor to this element
         * @param bi Flag for bi-connecting the objects. If set to true, the given object is linked to this object accordingly
         */
        void next(Oriented *obj, base::ContactPoint contactPoint = base::ContactPoint::START, bool bi = true);


        /**
         * Sets the given object with the contact point as the predecessor of this element
         * @param obj Object to be set as predecessor
         * @param contactPoint Contact point of the predecessor to this element
         * @param bi Flag for bi-connecting the objects. If set to true, the given object is linked to this object accordingly
         */
        void prev(Oriented *obj, base::ContactPoint contactPoint = base::ContactPoint::END, bool bi = true);


        /**
         * Returns the successor connections of this element
         * @return Connection vector
         */
        const Connections & nexts() const;


        /**
         * Returns the predecessor connections of this element
         * @return Connection vector
         */
        const Connections & prevs() const;


        /**
         * Returns the orientation of this element
         * @return The orientation
         */
        virtual base::Orientation orientation() const = 0;


        /**
         * Returns a positive value if the element is forward oriented (or both)
         * @return Flag
         */
        virtual bool isForward() const;


        /**
         * Returns a positive value if the element is forward oriented (or both)
         * @return Flag
         */
        virtual bool isBackwards() const;


    private:


        /**
         * Checks whether the given object is already stored as a next object respecting the contact point
         * @param obj Object to be checked
         * @param contactPoint Contact point
         * @return Flag denoting the existance
         */
        bool isNext(Oriented *obj, base::ContactPoint contactPoint) const;


        /**
         * Checks whether the given object is already stored as a previous object respecting the contact point
         * @param obj Object to be checked
         * @param contactPoint Contact point
         * @return Flag denoting the existance
         */
        bool isPrev(Oriented *obj, base::ContactPoint contactPoint) const;

    };

};

#endif // SIMMAP_GRAPH_ORIENTED_H
