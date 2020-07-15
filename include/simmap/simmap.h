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

/**
 * @file lib.h
 *
 * Collection of publicly accessible function of the SimMap framework.
 *
 * This header file defines the access to the library functions.
 */

#ifndef SIMMAP_SIMMAP_H
#define SIMMAP_SIMMAP_H

#include <cstdint>
#include "simmap_types.h"

#ifdef MAKE_SHARED
#ifdef LIBRARY_EXPORT
#define SHARED_EXPORT __declspec(dllexport)
#else
#define SHARED_EXPORT __declspec(dllimport)
#endif
#else
#define SHARED_EXPORT
#endif


namespace simmap {

    // typedefs
    typedef unsigned int id_type_t;
    typedef unsigned int err_type_t;


    // C compatibility
    typedef struct Position Position;
    typedef struct MapPosition MapPosition;
    typedef struct ObjectInformation ObjectInformation;
    typedef struct LaneInformation LaneInformation;
    typedef struct TargetInformation TargetInformation;
    typedef struct HorizonInformation HorizonInformation;


    // \todo: error codes


    /**
     * Clears the complete library storage. This leads to a state which is equal to the state when the library is
     * right loaded
     * @return Error code (0 = no error)
     */
    SHARED_EXPORT err_type_t clear();


    /**
     * Load map by passing the file name
     * @param filename File name
     * @param id ID of the segment
     * @return Error code (0 = no error)
     */
    SHARED_EXPORT err_type_t loadMap(const char *filename, id_type_t *id);


    /**
     * Unload map
     * @param id Map ID
     * @return Error code (0 = no error)
     */
    SHARED_EXPORT err_type_t unloadMap(id_type_t id);


    /**
     * Registers an agent
     * @param agentID ID of the agent
     * @param mapID ID of the associated map segment
     * @return Error code (0 = no error)
     */
    SHARED_EXPORT err_type_t registerAgent(id_type_t agentID, id_type_t mapID);


    /**
     * Unregisters the given agent
     * @param agentID ID of the agent
     * @return Error code (0 = no error)
     */
    SHARED_EXPORT err_type_t unregisterAgent(id_type_t agentID);


    /**
     * Sets the agent's track
     * @param agentID Agent ID
     * @param roadIds Road IDs as an array of cstrings. A minus denotes backward direction
     * @param n Number of roads in track
     * @return Error code
     */
    SHARED_EXPORT err_type_t setTrack(id_type_t agentID, const char **roadIds, unsigned long n);


    /**
     * Returns the absolute position in the world based on the road, the long. position in the road and the lane.
     * @param agentID Agent ID
     * @param mapPos Map position (Road ID, long. position, lane ID)
     * @param pos Position in global coordinates (will be updated)
     * @return Error code (0 = no error)
     */
    SHARED_EXPORT err_type_t getPosition(id_type_t agentID, Position *pos);


    /**
     * Sets the map position of the agent
     * @param agentID Agent ID
     * @param mapPos Map position to be set
     * @param lenFront Length to the front of the path (value is used for update)
     * @param lenBack Length to the back of the path (value is used for update)
     * @return Error code (0 = no error)
     */
    SHARED_EXPORT err_type_t setMapPosition(id_type_t agentID, MapPosition mapPos, double *lenFront, double *lenBack);


    /**
     * Returns the map position of the given agent
     * @param agentID Agent ID
     * @param mapPos Map position
     * @return Error code (0 = no error)
     */
    SHARED_EXPORT err_type_t getMapPosition(id_type_t agentID, MapPosition *mapPos);


    /**
     * Returns the map position based on the given global coordinates
     * @param agentID Agent ID
     * @param pos Absolute global position
     * @param ds Distance travelled since last update
     * @param mapPos New position in path
     * @return Error code (0 = no error)
     */
    SHARED_EXPORT err_type_t match(id_type_t agentID, Position pos, double ds, MapPosition *mapPos);


    /**
     * Moves the given agent by the distance of ds
     * @param agentID Agent id
     * @param ds Distance to be moved
     * @param t Lateral position in lane
     * @param lenFront Length to the front of the path (value is used for update)
     * @param lenBack Length to the back of the path (value is used for update)
     * @return Error code (0 = no error)
     */
    SHARED_EXPORT err_type_t move(id_type_t agentID, double ds, double t, double *lenFront, double *lenBack);


    /**
     * Sets the lane of the agent (relatively to the current)
     * @param agentID Agent ID
     * @param laneOffset Lane offset (0: same lane, +n: n lanes to the right, -n: n lanes to the left)
     * @return Error code (0 = no error)
     */
    SHARED_EXPORT err_type_t switchLane(id_type_t agentID, int laneOffset);


    /**
     * Calculates the horizon
     * @param agentID Agent ID
     * @param gridPoints Grid points where the horizon shall be calculated (relative to current position)
     * @param horizon Horizon to be calculated, an array of n entries
     * @param n Number of grid points
     * @return Error code (0 = no error)
     */
    SHARED_EXPORT err_type_t horizon(id_type_t agentID, const double *gridPoints, HorizonInformation *horizon, unsigned long n);


    /**
     * Returns a list of objects on the current path
     * @param agentID Agent ID
     * @param obj Object information vector
     * @param n Number of objects (pre-set for maximum number)
     * @return Error code (0 = no error)
     */
    SHARED_EXPORT err_type_t objects(id_type_t agentID, ObjectInformation *obj, unsigned long *n);


    /**
     * Returns a list of lanes neighbored to the current lane
     * @param agentID Agent ID
     * @param lanes Lane objects to be returned
     * @param n Number of lanes (pre-set for maximum number)
     * @return Error code (0 = no error)
     */
    SHARED_EXPORT err_type_t lanes(id_type_t agentID, LaneInformation *lanes, unsigned long *n);


    /**
     * Returns a list of of targets on the path of the agent
     * @param agentID Agent ID
     * @param targets Target objects
     * @param n Number of targets (pre-set for maximum number)
     * @return Error code (0 = no error)
     */
    SHARED_EXPORT err_type_t targets(id_type_t agentID, TargetInformation *targets, unsigned long *n);

}

#endif // SIMMAP_SIMMAP_H

