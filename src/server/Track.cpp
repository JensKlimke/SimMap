//
// Created by Jens Klimke on 2019-01-18.
//

#include "Track.h"
#include <base/functions.h>


namespace simmap {
namespace server {


    Track::Track(const edge_list_t &tr) : edge_list_t(tr) {}


    void Track::reverseTrack() {

        // do the super reverse
        edge_list_t::reverse();

        // switch orientations
        for (auto &o : *this)
            o.first = ::base::flip(o.first);

    }

}}