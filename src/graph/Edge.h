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

#ifndef SIMMAP_GRAPH_EDGE_H
#define SIMMAP_GRAPH_EDGE_H

#include <vector>
#include <tuple>
#include <string>
#include "Oriented.h"
#include "Neighbored.h"

namespace graph {

    class Object;

    /**
     * class Edge
     * @author Jens Klimke jens.klimke@rwth-aachen.de
     */
    class Edge : public Oriented, public Neighbored {

    public:

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
