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
// Created by Jens Klimke on 2019-01-18.
//

#ifndef SIMMAP_SERVER_TRACK_H
#define SIMMAP_SERVER_TRACK_H

#include <list>
#include <graph/Graph.h>

namespace simmap {
namespace server {

    typedef graph::Graph::EdgeList edge_list_t;


    /**
     * class Track
     * @author Jens Klimke jens.klimke@rwth-aachen.de
     * TODO: use flag for closed track or open track
     */
    class Track : public edge_list_t {

    public:

        typedef graph::Graph::OrientedEdge TrackElement;

        Track() = default;
        ~Track() = default;

        explicit Track(const edge_list_t &tr);

        void reverseTrack();

    };

}} // namespace ::simmap::server


#endif // SIMMAP_SERVER_TRACK_H
