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
