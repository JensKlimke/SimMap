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
// Created by Jens Klimke on 2019-01-19.
//

#ifndef SIMMAP_SERVER_LANESEPARATION_H
#define SIMMAP_SERVER_LANESEPARATION_H

#include <memory>
#include <graph/Neighbored.h>

namespace simmap {
namespace server {

    /**
     * class Separation
     * @author Jens Klimke jens.klimke@rwth-aachen.de
     */
    struct LaneSeparation {

        /** An enumeration for the separation type */
        enum class SeparationType { NONE, AREA, LINE, STRUCTURAL };


        /** An enumeration for crossing rule */
        enum class CrossingRule { ALLOWED, RESTRICTED, CAUTION };


        double width = 0.0; //!< The width of the separation area
        SeparationType type = SeparationType::NONE; //!< The type of the separation
        CrossingRule  rule = CrossingRule::ALLOWED; //!< The rule to cross the separation


        /**
         * Default constructor
         */
        LaneSeparation() = default;


        /**
         * Default destructor
         */
        virtual ~LaneSeparation() = default;

    };


}
}

#endif // SIMMAP_SERVER_LANESEPARATION_H
