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

#ifndef SIMMAP_OBJECT_H
#define SIMMAP_OBJECT_H

namespace graph {

    class Edge;

    /**
     * An interface to define objects
     * @author Jens Klimke jens.klimke@rwth-aachen.de
     */
    struct Object {


        /**
         * Default constructor
         */
        Object() = default;


        /**
         * Default destructor
         */
        virtual ~Object() = default;


        /**
         * Returns the type of the object
         * @return Type of the object
         */
        virtual const std::string &getType() const = 0;


        /**
         * Returns the label of the object
         * @return Label of the object
         */
        virtual const std::string &getLabel() const = 0;


        /**
         * Returns the ID of the object
         * @return ID of the object
         */
        virtual const std::string &getID() const = 0;


        /**
         * Returns the value of the object
         * @return Value of the object
         */
        virtual double getValue() const = 0;


    };


} // namespace graph

#endif //SIMMAP_OBJECT_H
