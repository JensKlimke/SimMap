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
// Created by Jens Klimke on 06.08.18.
//

#include <string>
#include <map>
#include <list>

#include <server/Map.h>
#include <server/Path.h>
#include <server/MapCoordinate.h>
#include <base/functions.h>
#include <odradapter/ODRAdapter.h>
#include <simmap/simmap.h>

#ifndef MATCH_WIDTH_FACTOR
#define MATCH_WIDTH_FACTOR 1.1
#endif


namespace simmap {

    using namespace simmap::server;

    struct Agent {
        Map *map = nullptr;
        Path path;
        Track track{};
    };

    static std::map<id_type_t, Map *>   _maps;   // Segment ID -> Map Segment
    static std::map<id_type_t, Agent *> _agents; // Agent ID   -> Map Segment

    static id_type_t _seg_id_counter = 0;


    err_type_t _basicCheckAgent(id_type_t agentID, Agent **ag) {

        try {

            // check if agent exists
            if (_agents.find(agentID) == _agents.end())
                return 2;

            // check segment of agent
            if (_agents.at(agentID)->map == nullptr)
                return 3;

            // return agent
            if (ag != nullptr)
                *ag = _agents.at(agentID);

            return 0;

        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return 4;
        }

    }


    err_type_t _basicCheckEdge(const Agent *ag, const std::string &edgeID, const LaneEdge **edge) {

        // get edge
        try {
            *edge = ag->map->getEdge(edgeID);
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return 20;
        }

        return 0;

    }


    err_type_t _basicMapCoordinate(id_type_t agentID, const MapPosition &mapPos, Agent **ag, MapCoordinate *mc) {

        // check if agent already registered
        auto err = _basicCheckAgent(agentID, ag);
        if (err != 0)
            return err;

        // get edge
        const LaneEdge *edge;
        err = _basicCheckEdge(*ag, mapPos.edgeID, &edge);
        if (err != 0)
            return err;

        // create map coordinate
        *mc = MapCoordinate(edge, mapPos.longPos, mapPos.latPos);
        return 0;

    }

    std::pair<ObjectType, int> _getObjectType(const graph::Object *obj) {

        if (obj->getType() != "signal")
            return {ObjectType::UNKNOWN, 0};


        if (obj->getLabel() == "206") {

            return {ObjectType::STOP_SIGN, 0};

        } else if (obj->getLabel() == "274.1") {

            return {ObjectType::SPEED_LIMIT, 30};

        } else if (obj->getLabel() == "274.1-20") {

            return {ObjectType::SPEED_LIMIT, 20};

        } else if (obj->getLabel() == "274.2") {

            return {ObjectType::SPEED_LIMIT, -30};

        } else if (obj->getLabel() == "274.2-20") {

            return {ObjectType::SPEED_LIMIT, -20};

        } else if (obj->getLabel().substr(0, 4) == "274-") {

            int val;

            try { val = std::stoi(obj->getLabel().substr(4)); }
            catch (...) { val = 0; }

            return {ObjectType::SPEED_LIMIT, val};

        } else if (obj->getLabel().substr(0, 4) == "278-") {

            int val;

            try { val = std::stoi(obj->getLabel().substr(4)); }
            catch (...) { val = 0; }

            return {ObjectType::SPEED_LIMIT, -val};

        } else if (obj->getLabel() == "282") {

            return {ObjectType::SPEED_LIMIT, -1};

        } else {

            return {ObjectType::UNKNOWN, 0};

        }

    }


    void _getTargetsOnPath(std::list<std::pair<id_type_t, Agent *>> &pool, std::vector<TargetInformation> &tars,
                           const Path &path, int pathIndex, bool sameDir) {

        // TODO: can agents be remove from the pool, when once added?

        // iterate over targets and check on current path
        for (auto const &tar : pool) {

            // get map coordinate and distance
            auto mc = tar.second->path.position();
            auto ds = path.distance(mc);

            // get relative position
            auto rel = base::toLocal(path.position().absolutePosition(), mc.absolutePosition().position);

            // set information
            for (auto dse : ds)
                tars.emplace_back(
                        TargetInformation{tar.first, rel.x, rel.y, (sameDir ? 1.0 : -1.0) * dse, mc.d(), pathIndex});

        }

    }


    err_type_t clear() {


        const int ERR = 10;

        try {

            // delete networks
            for (auto &nw : _maps)
                delete nw.second;

            // delete agents
            for (auto &ag : _agents)
                delete ag.second;

            // clear containers
            _maps.clear();
            _agents.clear();

            // reset ID counter
            _seg_id_counter = 0;

        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return ERR + 9;
        }

        return 0;

    }


    err_type_t loadMap(const char *filename, id_type_t *id) {

        const int ERR = 20;

        try {

            *id = 0;
            simmap::odra::ODRAdapter *map;

            try {

                // load map file
                map = new odra::ODRAdapter;
                _maps[++_seg_id_counter] = map;

            } catch (const std::exception &e) {
                std::cerr << e.what() << std::endl;
                return ERR + 5;
            }


            try {

                // create network
                map->loadFile(filename);

            }  catch (const std::exception &e) {

                // print error
                std::cerr << e.what() << std::endl;

                // delete map
                delete _maps[_seg_id_counter];
                _maps.erase(_seg_id_counter);

                // reduce counter
                --_seg_id_counter;

                return ERR + 6;
            }

            // return segment id
            *id = _seg_id_counter;

        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return ERR + 9;
        }

        return 0;

    }


    err_type_t unloadMap(id_type_t id) {

        const int ERR = 30;

        try {

            // get segment
            if (_maps.find(id) == _maps.end())
                return ERR + 5;

            // get map
            auto seg = _maps.at(id);

            // iterate over agents
            auto it = _agents.begin();
            while (it != _agents.end()) {

                if (it->second->map == seg) {

                    // get agent id and increase iterator
                    auto aid = it->first;
                    it++;

                    // unregister agent
                    unregisterAgent(aid);

                } else {

                    // increase iterator
                    it++;

                }

            }

            delete seg;
            _maps.erase(id);

        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return ERR + 9;
        }

        // return segment id
        return 0;

    }


    err_type_t registerAgent(id_type_t agentID, id_type_t mapID) {

        const int ERR = 40;

        try {

            // check if agent already registered
            if (_agents.find(agentID) != _agents.end())
                return ERR + 5;

            // check if map exists
            if (_maps.find(mapID) == _maps.end())
                return ERR + 6;


            // add agent to list
            _agents[agentID] = new Agent;
            _agents[agentID]->map = _maps.at(mapID);


        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return ERR + 9;
        }

        // everything ok
        return 0;

    }


    err_type_t unregisterAgent(id_type_t agentID) {

        const int ERR = 50;

        try {

            // check if agent already registered
            Agent *ag = nullptr;
            auto err = _basicCheckAgent(agentID, &ag);
            if (err != 0)
                return ERR + err;

            // erase agent
            delete _agents[agentID];
            _agents.erase(agentID);

        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return ERR + 9;
        }

        // everything ok
        return 0;

    }


    err_type_t setTrack(id_type_t agentID, const char **trackElements, unsigned long n) {

        const int ERR = 60;

        try {

            // check if agent already registered
            Agent *ag = nullptr;
            auto err = _basicCheckAgent(agentID, &ag);
            if (err != 0)
                return ERR + err;

            // get track and empty
            ag->track.clear();

            // get segment
            auto *map = _agents.at(agentID)->map;

            // iterate over roads
            for (size_t i = 0; i < n; ++i) {

                // get road id and
                std::string id(trackElements[i]);
                base::Orientation ori;

                // get orientation and remove sign
                if (id.substr(0, 1) == "-") {
                    id = id.substr(1);
                    ori = base::Orientation::BACKWARDS;
                } else if (id.substr(0, 1) == "+") {
                    id = id.substr(1);
                    ori = base::Orientation::FORWARDS;
                } else {
                    ori = base::Orientation::FORWARDS;
                }

                // check if road exists
                if (map->_roadNetwork.find(id) == map->_roadNetwork.end())
                    return ERR + 5;

                // get road and add to track
                auto road = map->_roadNetwork.at(id);
                ag->track.push_back(simmap::server::Track::TrackElement{ori, road.get()});

            }

        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return ERR + 9;
        }

        return 0;

    }


    err_type_t getPosition(id_type_t agentID, Position *pos) {

        const int ERR = 70;

        try {

            // check if agent already registered
            Agent *ag = nullptr;
            auto err = _basicCheckAgent(agentID, &ag);
            if (err != 0)
                return ERR + err;

            // get agent and check track
            if (ag->track.empty())
                return ERR + 5; // track not set

            // get map position and absolute position
            auto mc = ag->path.position();
            auto p = mc.absolutePosition();

            // save result
            pos->x = p.position.x;
            pos->y = p.position.y;
            pos->z = p.position.z;
            pos->phi = p.angle;
            pos->kappa = p.curvature;


        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return ERR + 9;
        }

        return 0;

    }


    err_type_t setMapPosition(id_type_t agentID, MapPosition mapPos, double *lenFront, double *lenBack) {

        const int ERR = 80;

        try {

            // get agent and position
            MapCoordinate mc{};
            Agent *ag;

            auto err = _basicMapCoordinate(agentID, mapPos, &ag, &mc);
            if (err != 0)
                return ERR + err;

            // set path
            ag->path = Path();


            try {

                // create path
                Path::create(ag->path, ag->track, *lenFront, *lenBack, mc);

                // set lengths
                *lenFront = ag->path.distanceToHead();
                *lenBack = ag->path.distanceToBack();

            } catch (const std::exception &e) {
                std::cerr << e.what() << std::endl;
                return ERR + 5;
            }

            // return 0
            return 0;

        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return ERR + 9;
        }

    }


    err_type_t getMapPosition(id_type_t agentID, MapPosition *mapPos) {

        const int ERR = 90;

        try {

            // check if agent already registered
            Agent *ag = nullptr;
            auto err = _basicCheckAgent(agentID, &ag);
            if (err != 0)
                return ERR + err;

            // get map coordinate
            auto mc = ag->path.position();

            // copy position
            mapPos->edgeID = mc.edge()->id().c_str();
            mapPos->latPos = mc.d();
            mapPos->longPos = mc.s();

        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return ERR + 9;
        }

        return 0;

    }


    err_type_t match(id_type_t agentID, Position pos, double ds, MapPosition *mapPos) {

        const int ERR = 100;

        try {

            // check if agent already registered
            Agent *ag = nullptr;
            auto err = _basicCheckAgent(agentID, &ag);
            if (err != 0)
                return ERR + err;

            // get doubled travel distance
            double ds2 = std::abs(ds) * MATCH_WIDTH_FACTOR;

            // matching
            base::Vector3 vec{pos.x, pos.y, pos.z};
            ag->path.match(vec, ds, ds2);

            // update path position
            auto mc = ag->path.positionAt(ds);
            auto edge = mc.edge();

            // save map position
            mapPos->edgeID  = edge->id().c_str();
            mapPos->longPos = mc.s();

            // transform real position into matched positions cs
            auto loc = base::toLocal(mc.absolutePosition(), vec);
            mapPos->latPos = loc.y;

        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return ERR + 9;
        }

        return 0;

    }


    err_type_t move(id_type_t agentID, double distance, double lateralPosition, double *lenFront, double *lenBack) {

        const int ERR = 110;

        try {

            // check if agent already registered
            Agent *ag = nullptr;
            auto err = _basicCheckAgent(agentID, &ag);
            if (err != 0)
                return ERR + err;

            // check length
            if (ag->path.distanceToHead() < distance || -ag->path.distanceToBack() > distance)
                return ERR + 5;

            try {

                // set path
                ag->path.position(distance, lateralPosition);

            } catch (const std::exception &e) {
                std::cerr << e.what() << std::endl;
                return ERR + 6;
            }

            try {

                // update path
                ag->path.updatePath(*lenFront, *lenBack, ag->track);

            } catch (const std::exception &e) {
                std::cerr << e.what() << std::endl;
                return ERR + 7;
            }

            // update lengths
            *lenFront = ag->path.distanceToHead();
            *lenBack = ag->path.distanceToBack();


        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return ERR + 9;
        }

        return 0;

    }


    err_type_t switchLane(id_type_t agentID, int laneOffset) {

        const int ERR = 160;

        try {

            // check if agent already registered
            Agent *ag = nullptr;
            auto err = _basicCheckAgent(agentID, &ag);
            if (err != 0)
                return ERR + err;

            // iterate over neighbors
            // TODO: save neighbored lanes to efficiently search for targets (recalculate intelligently on position update)
            for (const auto &p : ag->path.neighboredPaths(ag->track)) {

                if(p.first.index == laneOffset) {

                    if(!p.first.sameDir)
                        return ERR + 7;

                    // not accessible
                    if(!p.first.accessible)
                        return ERR + 6;

                    // set lengths
                    double lenFront = ag->path.distanceToHead();
                    double lenBack = ag->path.distanceToBack();

                    // set new position
                    ag->path = Path();

                    try {

                        // create path
                        Path::create(ag->path, ag->track, lenFront, lenBack, p.second.position());

                    } catch (const std::exception &e) {
                        std::cerr << e.what() << std::endl;
                        return ERR + 5;
                    }

                    return 0;

                }

            }

            // no neighbor found
            return ERR + 8;

        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return ERR + 9;
        }

    }



    err_type_t horizon(id_type_t agentID, const double *gridPoints, HorizonInformation *horizon, unsigned long n) {

        const int ERR = 120;

        try {

            // check if agent already registered
            Agent *ag = nullptr;
            auto err = _basicCheckAgent(agentID, &ag);
            if (err != 0)
                return ERR + err;

            // get agent position
            auto pos = ag->path.position().absolutePosition();

            // calculate horizon
            for (size_t i = 0; i < n; ++i) {

                // save grid points
                auto s = gridPoints[i];
                horizon[i].s = s;

                // preset (in case it is aborted)
                horizon[i].x          = 0.0;
                horizon[i].y          = 0.0;
                horizon[i].psi        = 0.0;
                horizon[i].kappa      = 0.0;
                horizon[i].laneWidth  = 0.0;
                horizon[i].rightWidth = 0.0;
                horizon[i].leftWidth  = 0.0;

                // check if distance to head is reached
                if (s > ag->path.distanceToHead()) {
                    horizon[i].s = INFINITY;
                    continue;
                }

                // check if the distance to back is reached
                if (s < -ag->path.distanceToBack()) {
                    horizon[i].s = -std::numeric_limits<double>::infinity();
                    continue;
                }

                // calculate position
                MapCoordinate mc{};
                base::CurvePoint mp{};

                try {

                    mc = ag->path.positionAt(s);
                    mp = mc.absolutePosition();

                } catch (const std::exception &e) {
                    std::cerr << e.what() << std::endl;
                    return ERR + 6;
                }

                // get right lane
                MapCoordinate mcr{};
                MapCoordinate mcl{};
                try {

                    mcr = mc.right();
                    mcl = mc.left();

                } catch (const std::exception &e) {
                    std::cerr << e.what() << std::endl;
                    return ERR + 7;
                }

                // position angle and curvature
                horizon[i].x     = mp.position.x;
                horizon[i].y     = mp.position.y;
                horizon[i].psi   = mp.angle;
                horizon[i].kappa = mp.curvature;

                // widths of lanes, TODO: move that paths
                horizon[i].laneWidth  = mc.width();
                horizon[i].rightWidth = mcr.outOfRoad() ? 0.0 : mcr.width();
                horizon[i].leftWidth  = mcl.outOfRoad() ? 0.0 : mcl.width();

            }

        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return ERR + 9;
        }


        return 0;

    }


    err_type_t objects(id_type_t agentID, ObjectInformation *obj, unsigned long *n) {

        const int ERR = 130;

        try {

            // check if agent already registered
            Agent *ag = nullptr;
            auto err = _basicCheckAgent(agentID, &ag);
            if (err != 0)
                return ERR + err;

            // check n
            if (*n == 0)
                return ERR + 5;

            // copy n to store max
            auto max = *n;
            *n = 0;

            // iterate over objects
            for (const auto &o : ag->path.objects()) {

                auto object = dynamic_cast<const graph::Object*>(o.second);
                auto t = _getObjectType(object);

                obj[*n].id = object->getID().c_str();
                obj[*n].distance = o.first;
                obj[*n].type = t.first;
                obj[*n].value = t.second;

                if (++*n == max)
                    break;

            }


        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return ERR + 9;
        }

        return 0;


    }


    err_type_t lanes(id_type_t agentID, LaneInformation *lanes, unsigned long *n) {

        const int ERR = 140;

        try {

            // check if agent already registered
            Agent *ag = nullptr;
            auto err = _basicCheckAgent(agentID, &ag);
            if (err != 0)
                return ERR + err;

            // check n
            if (*n == 0)
                return ERR + 5;

            // copy n to store max
            auto max = *n;
            *n = 0;

            // iterate over neighbors
            // TODO: save neighbored lanes to efficiently search for targets (recalculate intelligently on position update)
            for (const auto &p : ag->path.neighboredPaths(ag->track)) {

                // check if lanes have same direction
                auto sd = p.first.sameDir;

                // create lane information
                lanes[*n] = LaneInformation{};
                auto li = &lanes[*n];

                // get edge
                auto pos = p.second.position();

                // save information
                li->width = p.second.position().width();
                li->index = p.first.index;
                li->lengthOnTrack = sd ? p.second.distanceToHead() : p.second.distanceToBack(); // TODO
                li->lengthToClosed = li->lengthOnTrack;
                li->access = p.first.accessible
                             ? (p.first.allowed ? Access::ALLOWED : Access::NOT_ALLOWED)
                             : Access::ALLOWED;
                li->direction = sd ? Direction::FORWARDS : Direction::BACKWARDS;
                li->id = pos.edge()->id().c_str();
                li->s  = pos.s();

                // check length
                if (++*n == max)
                    break;

            }


        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return ERR + 9;
        }

        return 0;

    }


    err_type_t targets(id_type_t agentID, TargetInformation *targets, unsigned long *n) {

        const int ERR = 150;

        try {

            // check if agent already registered
            Agent *ag = nullptr;
            auto err = _basicCheckAgent(agentID, &ag);
            if (err != 0)
                return ERR + err;

            // check n
            if (*n == 0)
                return ERR + 5;


            // create vector of agents
            std::list<std::pair<id_type_t, Agent *>> pool;
            for (auto const &tar : _agents) {

                // don't recognize itself
                if (tar.second == ag)
                    continue;

                // check if segment set and same segment
                if (ag->map != tar.second->map)
                    continue;

                // save pair to vector
                pool.emplace_back(tar);

            }


            // copy n to store max
            auto max = *n;
            *n = 0;

            // create target vector
            std::vector<TargetInformation> tars;
            tars.reserve(pool.size());

            // get targets on own path
            _getTargetsOnPath(pool, tars, ag->path, 0, true);

            // iterate over neighbored paths
            for (const auto &p : ag->path.neighboredPaths(ag->track))
                _getTargetsOnPath(pool, tars, p.second, p.first.index,
                                  p.second.position().edge()->isForward() == ag->path.position().edge()->isForward());

            // sort results by distance
            sort(tars.begin(), tars.end(),
                 [](const TargetInformation &a, const TargetInformation &b) -> bool {
                     return std::abs(a.distance) < std::abs(b.distance);
                 });

            // copy
            *n = static_cast<unsigned long>(tars.size() > max ? max : tars.size());
            for (size_t i = 0; i < *n; ++i)
                targets[i] = tars.at(i);


        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return ERR + 9;
        }

        return 0;


    }

} // namespace ::simmap

