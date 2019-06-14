//
// Created by Jens Klimke on 06.08.18.
//

#include <string>
#include <map>
#include <map>
#include <list>

#include "lib.h"

#include <server/Map.h>
#include <server/Path.h>
#include <server/MapCoordinate.h>
#include <server/LaneEdge.h>
#include <server/Track.h>
#include <base/functions.h>
#include <graph/Object.h>
#include <odradapter/ODRAdapter.h>

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

        } catch (...) {

            return 4;

        }

    }


    err_type_t _basicCheckEdge(const Agent *ag, const std::string &edgeID, const LaneEdge **edge) {

        // get edge
        try {
            *edge = ag->map->getEdge(edgeID);
        } catch (...) {
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

            // set information
            for (auto dse : ds)
                tars.emplace_back(TargetInformation{tar.first, (sameDir ? 1.0 : -1.0) * dse, mc.d(), pathIndex});

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


        } catch (...) {
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

            } catch (...) {
                return ERR + 5;
            }


            try {

                // create network
                map->loadFile(filename);

            } catch (...) {

                // delete map
                delete _maps[_seg_id_counter];
                _maps.erase(_seg_id_counter);

                // reduce counter
                --_seg_id_counter;

                return ERR + 6;
            }

            // return segment id
            *id = _seg_id_counter;

        } catch (...) {

            // unknown error
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

        } catch (...) {

            // unknown error
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


        } catch (...) {

            // unknown error
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

        } catch (...) {

            // unknown error
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
                def::Orientation ori;

                // get orientation and remove sign
                if (id.substr(0, 1) == "-") {
                    id = id.substr(1);
                    ori = def::Orientation::BACKWARDS;
                } else if (id.substr(0, 1) == "+") {
                    id = id.substr(1);
                    ori = def::Orientation::FORWARDS;
                } else {
                    ori = def::Orientation::FORWARDS;
                }

                // check if road exists
                if (map->_roadNetwork.find(id) == map->_roadNetwork.end())
                    return ERR + 5;

                // get road and add to track
                auto road = map->_roadNetwork.at(id);
                ag->track.push_back(simmap::server::Track::TrackElement{ori, road.get()});

            }

        } catch (...) {

            // unknown error
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
            pos->x = p.position.x();
            pos->y = p.position.y();
            pos->z = p.position.z();
            pos->phi = p.angle;
            pos->kappa = p.curvature;


        } catch (...) {

            // unknown error
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

            } catch (...) {
                return ERR + 5;
            }

            // get length and return 0
            *lenFront = ag->path.distanceToHead();
            return 0;

        } catch (...) {
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

        } catch (...) {
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
            double ds2 = fabs(ds) * MATCH_WIDTH_FACTOR;

            // check if path is long enough
            if (ag->path.distanceToBack() < ds2 || ag->path.distanceToHead() < ds2)
                return ERR + 5;

            // matching
            Eigen::Vector3d vec(pos.x, pos.y, pos.z);
            ag->path.match(vec, ds, ds2);

            // update path position
            auto mc = ag->path.positionAt(ds);
            auto edge = mc.edge();

            // save map position
            mapPos->edgeID  = edge->id().c_str();
            mapPos->longPos = mc.s();

            // transform real position into matched positions cs
            auto loc = base::toLocal(mc.absolutePosition(), vec);
            mapPos->latPos = loc.y();

        } catch (...) {
            return ERR + 9;
        }

        return 0;

    }


    err_type_t move(id_type_t agentID, double ds, double t, double *lenFront, double *lenBack) {

        const int ERR = 110;

        try {

            // check if agent already registered
            Agent *ag = nullptr;
            auto err = _basicCheckAgent(agentID, &ag);
            if (err != 0)
                return ERR + err;

            // check length
            if (ag->path.distanceToHead() < ds || -ag->path.distanceToBack() > ds)
                return ERR + 5;

            try {

                // set path
                ag->path.position(ds, t);

            } catch (...) {
                return ERR + 6;
            }

            try {

                // update path
                ag->path.updatePath(*lenFront, *lenBack, ag->track);

            } catch (...) {
                return ERR + 7;
            }

            // update lengths
            *lenFront = ag->path.distanceToHead();
            *lenBack = ag->path.distanceToBack();


        } catch (...) {
            return ERR + 9;
        }

        return 0;

    }


    err_type_t horizon(id_type_t agentID, const double *knots, double *horizon, unsigned long n) {

        const int ERR = 120;

        try {

            // check if agent already registered
            Agent *ag = nullptr;
            auto err = _basicCheckAgent(agentID, &ag);
            if (err != 0)
                return ERR + err;

            // define entry size
            const size_t N = 8;

            // get agent position
            auto pos = ag->path.position().absolutePosition();

            // calculate horizon
            for (size_t i = 0; i < n; ++i) {

                // save knots
                auto s = knots[i];
                horizon[i * N + 0] = s;

                // check if distance to head is reached
                if (s > ag->path.distanceToHead())
                    return ERR + 5;

                if (s < -ag->path.distanceToBack())
                    return ERR + 5;

                // calculate position
                MapCoordinate mc{};
                def::CurvePoint mp{};

                try {

                    mc = ag->path.positionAt(s);
                    mp = mc.absolutePosition();

                } catch (...) {
                    return ERR + 6;
                }

                // get right lane
                MapCoordinate mcr{};
                MapCoordinate mcl{};
                try {

                    mcr = mc.right();
                    mcl = mc.left();

                } catch (...) {
                    return ERR + 7;
                }

                // position angle and curvature
                horizon[i * N + 1] = mp.position.x();
                horizon[i * N + 2] = mp.position.y();
                horizon[i * N + 3] = mp.angle;
                horizon[i * N + 4] = mp.curvature;

                // widths of lanes
                horizon[i * N + 5] = mc.width();
                horizon[i * N + 6] = mcr.outOfRoad() ? 0.0 : mcr.width();
                horizon[i * N + 7] = mcl.outOfRoad() ? 0.0 : mcl.width();

            }

        } catch (...) {
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


        } catch (...) {
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

            // iterate over objects
            // TODO: save neighbored lanes to efficiently search for targets (recalculate intelligently on position update)
            for (const auto &p : ag->path.neighboredPaths(ag->track)) {

                // check if lanes have same direction
                bool sd = (p.second.position().edge()->isForward() == ag->path.position().edge()->isForward());

                // create lane information
                lanes[*n] = LaneInformation{};
                auto li = &lanes[*n];

                // save information
                li->width = p.second.position().width();
                li->index = p.first.index;
                li->lengthOnTrack = sd ? p.second.distanceToHead() : p.second.distanceToBack(); // TODO
                li->lengthToClosed = li->lengthOnTrack;
                li->access = p.first.accessible
                             ? (p.first.allowed ? Access::ALLOWED : Access::NOT_ALLOWED)
                             : Access::ALLOWED;
                li->direction = sd ? Direction::FORWARDS : Direction::BACKWARDS;
                li->id = p.second.position().edge()->id().c_str();

                // check length
                if (++*n == max)
                    break;

            }


        } catch (...) {
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
                     return fabs(a.distance) < fabs(b.distance);
                 });

            // copy
            *n = static_cast<unsigned long>(tars.size() > max ? max : tars.size());
            for (size_t i = 0; i < *n; ++i)
                targets[i] = tars.at(i);


        } catch (...) {
            return ERR + 9;
        }

        return 0;


    }

} // namespace ::simmap

