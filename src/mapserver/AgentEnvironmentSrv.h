// Copyright (c) 2020 Jens Klimke (jens.klimke@rwth-aachen.de). All rights reserved.
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
// Created by Jens Klimke on $YEAR-$MONTH-16.
//


#ifndef SIMMAP_AGENTENVIRONMENTSRV_H
#define SIMMAP_AGENTENVIRONMENTSRV_H

#include <iostream>
#include <memory>
#include <string>

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <mapserver/simmap.grpc.pb.h>
#include <simmap/simmap.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using std::chrono::system_clock;


class AgentEnvironmentSrv final : public simmap::envrionment::manager::AgentEnvironmentServer::Service {

    unsigned long map_id{};

public:

    /**
     * @brief Starts the service
     * Loads a given map into the memory.
     * @param map
     */
    explicit AgentEnvironmentSrv(const std::string &map) {

        // load map
        simmap::loadMap(map.c_str(), map_id);

    }


    /**
     * @brief Ends the service
     */
    ~AgentEnvironmentSrv() override {

        // reset everything
        simmap::clear();

    }

    Status registerAgent(::grpc::ServerContext *context, const ::simmap::envrionment::manager::AgentMap *request,
                         ::simmap::envrionment::manager::Void *response) override {

        // execute
        if (simmap::registerAgent(request->agent().id(), request->map().id()) != 0)
            return Status::CANCELLED;

        // ok
        return Status::OK;

    }

    Status unregisterAgent(::grpc::ServerContext *context, const ::simmap::envrionment::manager::AgentInstance *request,
                           ::simmap::envrionment::manager::Void *response) override {

        // execute
        if (simmap::unregisterAgent(request->id()) != 0)
            return Status::CANCELLED;

        // ok
        return Status::OK;

    }

    Status setTrack(::grpc::ServerContext *context, const ::simmap::envrionment::manager::AgentTrack *request,
                    ::simmap::envrionment::manager::Void *response) override {

        // prepare track
        auto n = request->track().roads_size();
        const char *roads[n];

        // set elements
        for (int i = 0; i < n; ++i)
            roads[i] = request->track().roads(i).c_str();

        // execute
        if (simmap::setTrack(request->agent().id(), roads, n) != 0)
            return Status::CANCELLED;

        // ok
        return Status::OK;

    }

    Status getPosition(::grpc::ServerContext *context, const ::simmap::envrionment::manager::AgentInstance *request,
                       ::simmap::envrionment::manager::Position *response) override {

        // prepare data
        simmap::Position pos{};

        // execute
        if (simmap::getPosition(request->id(), pos) != 0)
            return Status::CANCELLED;

        // set data
        response->set_x(pos.x);
        response->set_y(pos.y);
        response->set_z(pos.z);
        response->set_phi(pos.phi);
        response->set_kappa(pos.kappa);

        // ok
        return Status::OK;

    }

    Status
    setMapPosition(::grpc::ServerContext *context, const ::simmap::envrionment::manager::AgentMapPosition *request,
                   ::simmap::envrionment::manager::TrackLengths *response) override {

        // prepare map position
        simmap::MapPosition mapPos{};
        mapPos.edgeID = request->mapposition().edgeid().c_str();
        mapPos.latPos = request->mapposition().latpos();
        mapPos.longPos = request->mapposition().longpos();

        // variables
        double lenFront{}, lenBack{};

        // execute
        if (simmap::setMapPosition(request->agent().id(), mapPos, lenFront, lenBack) != 0)
            return Status::CANCELLED;

        // set response
        response->set_front(lenFront);
        response->set_back(lenBack);

        // ok
        return Status::OK;

    }

    Status getMapPosition(::grpc::ServerContext *context, const ::simmap::envrionment::manager::AgentInstance *request,
                          ::simmap::envrionment::manager::MapPosition *response) override {

        // prepare map position
        simmap::MapPosition mapPos{};

        // execute
        if (simmap::getMapPosition(request->id(), mapPos) != 0)
            return Status::CANCELLED;

        // set response data
        response->set_edgeid(mapPos.edgeID);
        response->set_latpos(mapPos.latPos);
        response->set_longpos(mapPos.longPos);

        // ok
        return Status::OK;

    }

    Status match(::grpc::ServerContext *context, const ::simmap::envrionment::manager::AgentPositionUpdate *request,
                 ::simmap::envrionment::manager::MapPosition *response) override {

        // prepare input data
        simmap::Position pos{};
        pos.x = request->position().x();
        pos.y = request->position().y();
        pos.z = request->position().z();
        pos.phi = request->position().phi();
        pos.kappa = request->position().kappa();

        // prepare map position
        simmap::MapPosition mapPos{};

        // execute
        if (simmap::match(request->agent().id(), pos, request->distance(), mapPos) != 0)
            return Status::CANCELLED;

        // set response data
        response->set_edgeid(mapPos.edgeID);
        response->set_latpos(mapPos.latPos);
        response->set_longpos(mapPos.longPos);

        // ok
        return Status::OK;

    }

    Status move(::grpc::ServerContext *context, const ::simmap::envrionment::manager::AgentDisplacement *request,
                ::simmap::envrionment::manager::TrackLengths *response) override {

        // variables
        double lenFront{}, lenBack{};

        // execute
        if (simmap::move(request->agent().id(), request->distance(), request->lateralposition(), lenFront, lenBack) !=
            0)
            return Status::CANCELLED;

        // set response
        response->set_front(lenFront);
        response->set_back(lenBack);

        // ok
        return Status::OK;

    }

    Status switchLane(::grpc::ServerContext *context, const ::simmap::envrionment::manager::AgentLane *request,
                      ::simmap::envrionment::manager::Void *response) override {

        // execute
        if (simmap::switchLane(request->agent().id(), request->laneoffset()) != 0)
            return Status::CANCELLED;

        // ok
        return Status::OK;

    }

    Status horizon(::grpc::ServerContext *context, const ::simmap::envrionment::manager::AgentGridPoints *request,
                   ::simmap::envrionment::manager::Horizon *response) override {

        // prepare horizon
        auto n = request->gridpoints_size();
        simmap::HorizonInformation hor[n];

        // execute
        if (simmap::horizon(request->agent().id(), request->gridpoints().data(), hor, n) != 0)
            return Status::CANCELLED;

        // iterate over horizon points
        for (int i = 0; i < n; ++i) {

            // create point
            auto pnt = response->add_point();

            // set data
            pnt->set_s(hor[i].s);
            pnt->set_x(hor[i].x);
            pnt->set_y(hor[i].y);
            pnt->set_psi(hor[i].psi);
            pnt->set_kappa(hor[i].kappa);
            pnt->set_egolanewidth(hor[i].egoLaneWidth);
            pnt->set_leftlanewidth(hor[i].leftLaneWidth);
            pnt->set_rightlanewidth(hor[i].rightLaneWidth);

        }

        // ok
        return Status::OK;

    }

    Status objects(::grpc::ServerContext *context, const ::simmap::envrionment::manager::AgentInstance *request,
                   ::simmap::envrionment::manager::ObjectList *response) override {

        // namespace
        using namespace ::simmap::envrionment::manager;

        // prepare object list
        unsigned long n = response->maxnoofelements();
        simmap::ObjectInformation objects[n];

        // execute
        if (simmap::objects(request->id(), objects, n) != 0)
            return Status::CANCELLED;

        // iterate over horizon points
        for (int i = 0; i < n; ++i) {

            // create point
            auto obj = response->add_object();

            // enum types
            auto type = ObjectInformation_ObjectType_UNKNOWN;
            if (objects[i].type == simmap::ObjectType::SPEED_LIMIT)
                type = ObjectInformation_ObjectType_SPEED_LIMIT;
            else if (objects[i].type == simmap::ObjectType::STOP_SIGN)
                type = ObjectInformation_ObjectType_STOP_SIGN;

            // set data
            obj->set_id(objects[i].id);
            obj->set_distance(objects[i].distance);
            obj->set_type(type);
            obj->set_value(objects[i].value);

        }

        // ok
        return Status::OK;

    }

    Status lanes(::grpc::ServerContext *context, const ::simmap::envrionment::manager::AgentInstance *request,
                 ::simmap::envrionment::manager::LaneList *response) override {

        // namespace
        using namespace ::simmap::envrionment::manager;

        // prepare lane list
        unsigned long n = response->maxnoofelements();
        simmap::LaneInformation lanes[n];

        // execute
        if (simmap::lanes(request->id(), lanes, n) != 0)
            return Status::CANCELLED;

        // iterate over horizon points
        for (int i = 0; i < n; ++i) {

            // create point
            auto lane = response->add_lane();

            // access
            auto access = LaneInformation_Access_NOT_POSSIBLE;
            if (lanes[i].access == simmap::Access::ALLOWED)
                access = LaneInformation_Access_ALLOWED;
            else if (lanes[i].access == simmap::Access::NOT_ALLOWED)
                access = LaneInformation_Access_NOT_ALLOWED;

            // direction
            auto direction = LaneInformation_Direction_FORWARDS;
            if (lanes[i].direction == simmap::Direction::BACKWARDS)
                direction = LaneInformation_Direction_BACKWARDS;

            // set data
            lane->set_id(lanes[i].id);
            lane->set_s(lanes[i].s);
            lane->set_index(lanes[i].index);
            lane->set_lengthontrack(lanes[i].lengthOnTrack);
            lane->set_lengthtoclosed(lanes[i].lengthToClosed);
            lane->set_access(access);
            lane->set_direction(direction);

        }

        // ok
        return Status::OK;

    }

    Status targets(::grpc::ServerContext *context, const ::simmap::envrionment::manager::AgentInstance *request,
                   ::simmap::envrionment::manager::TargetList *response) override {

        // prepare target list
        unsigned long n = response->maxnoofelements();
        simmap::TargetInformation targets[n];

        // execute
        if (simmap::targets(request->id(), targets, n) != 0)
            return Status::CANCELLED;

        // iterate over horizon points
        for (int i = 0; i < n; ++i) {

            // create point
            auto target = response->add_target();

            // set data
            target->set_id(targets[i].id);
            target->set_lane(targets[i].lane);
            target->set_rel_pos_x(targets[i].x);
            target->set_rel_pos_y(targets[i].y);
            target->set_distance(targets[i].distance);
            target->set_latoffset(targets[i].latOffset);

        }

        // ok
        return Status::OK;

    }

};


#endif //SIMMAP_AGENTENVIRONMENTSRV_H
