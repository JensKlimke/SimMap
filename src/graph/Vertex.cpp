//
// Created by Jens Klimke on 2019-01-03.
//

#include "Vertex.h"

namespace graph {

    void Vertex::connectTo(Vertex *destination, Edge *edge) {

        // set link
        _out.push_back(Link{edge, destination});
        destination->_in.push_back(Link{edge, this});

        // connect edge to outgoing edges of destination vertex
        for (const auto &n : destination->outLinks())
            edge->next(n.first);

        // connect edge to incoming edges of source vertex
        for (const auto &n : this->inLinks())
            edge->prev(n.first);

    }

    const Vertex::LinkVector &Vertex::outLinks() const {

        return _out;

    }

    const Vertex::LinkVector &Vertex::inLinks() const {

        return _in;

    }

}