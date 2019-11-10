//
// Created by Jens Klimke on 06.08.18.
//

#ifndef SIMMAP_LIB_H
#define SIMMAP_LIB_H

#include <cstdint>
#include "types.h"

#ifdef _WIN32
#ifdef SHARED_EXPORT
        #define SHARED_EXPORT __declspec(dllexport)
    #else
        #define SHARED_EXPORT __declspec(dllimport)
    #endif
#else
#define SHARED_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
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


    // TODO: error codes


    /**
     * Clears the complete library storage. This leads to a state which is equal to the state when the library is
     * right loaded
     * @return Error code (0 = no error)
     */
    err_type_t SHARED_EXPORT clear();


    /**
     * Load map by passing the file name
     * @param filename File name
     * @param id ID of the segment
     * @return Error code (0 = no error)
     */
    err_type_t SHARED_EXPORT loadMap(const char *filename, id_type_t *id);


    /**
     * Unload map
     * @param id Map ID
     * @return Error code (0 = no error)
     */
    err_type_t SHARED_EXPORT unloadMap(id_type_t id);


    /**
     * Registers an agent
     * @param agentID ID of the agent
     * @param mapID ID of the associated map segment
     * @return Error code (0 = no error)
     */
    err_type_t SHARED_EXPORT registerAgent(id_type_t agentID, id_type_t mapID);


    /**
     * Unregisters the given agent
     * @param agentID ID of the agent
     * @return Error code (0 = no error)
     */
    err_type_t SHARED_EXPORT unregisterAgent(id_type_t agentID);


    /**
     * Sets the agent's track
     * @param agentID Agent ID
     * @param roadIds Road IDs as an array of cstrings. A minus denotes backward direction
     * @param n Number of roads in track
     * @return Error code
     */
    err_type_t SHARED_EXPORT setTrack(id_type_t agentID, const char **roadIds, unsigned long n);


    /**
     * Returns the absolute position in the world based on the road, the long. position in the road and the lane.
     * @param agentID Agent ID
     * @param mapPos Map position (Road ID, long. position, lane ID)
     * @param pos Position in global coordinates (will be updated)
     * @return Error code (0 = no error)
     */
    err_type_t SHARED_EXPORT getPosition(id_type_t agentID, Position *pos);


    /**
     * Sets the map position of the agent
     * @param agentID Agent ID
     * @param mapPos Map position to be set
     * @param lenFront Length to the front of the path (value is used for update)
     * @param lenBack Length to the back of the path (value is used for update)
     * @return Error code (0 = no error)
     */
    err_type_t SHARED_EXPORT setMapPosition(id_type_t agentID, MapPosition mapPos, double *lenFront, double *lenBack);


    /**
     * Returns the map position of the given agent
     * @param agentID Agent ID
     * @param mapPos Map position
     * @return Error code (0 = no error)
     */
    err_type_t SHARED_EXPORT getMapPosition(id_type_t agentID, MapPosition *mapPos);


    /**
     * Returns the map position based on the given global coordinates
     * @param agentID Agent ID
     * @param pos Absolute global position
     * @param ds Distance travelled since last update
     * @param mapPos New position in path
     * @return Error code (0 = no error)
     */
    err_type_t SHARED_EXPORT match(id_type_t agentID, Position pos, double ds, MapPosition *mapPos);


    /**
     * Moves the given agent by the distance of ds
     * @param agentID Agent id
     * @param ds Distance to be moved
     * @param t Lateral position in lane
     * @param lenFront Length to the front of the path (value is used for update)
     * @param lenBack Length to the back of the path (value is used for update)
     * @return Error code (0 = no error)
     */
    err_type_t SHARED_EXPORT move(id_type_t agentID, double ds, double t, double *lenFront, double *lenBack);


    /**
     * Calculates the horizon
     * @param agentID Agent ID
     * @param knots Knots where the horizon shall be calculated (relative to current position)
     * @param horizon Horizon to be calculated, an array of n entries
     * @param n Number of knots
     * @return Error code (0 = no error)
     */
    err_type_t SHARED_EXPORT horizon(id_type_t agentID, const double *knots, HorizonInformation *horizon, unsigned long n);


    /**
     * Returns a list of objects on the current path
     * @param agentID Agent ID
     * @param obj Object information vector
     * @param n Number of objects (pre-set for maximum number)
     * @return Error code (0 = no error)
     */
    err_type_t SHARED_EXPORT objects(id_type_t agentID, ObjectInformation *obj, unsigned long *n);


    /**
     * Returns a list of lanes neighbored to the current lane
     * @param agentID Agent ID
     * @param lanes Lane objects to be returned
     * @param n Number of lanes (pre-set for maximum number)
     * @return Error code (0 = no error)
     */
    err_type_t SHARED_EXPORT lanes(id_type_t agentID, LaneInformation *lanes, unsigned long *n);


    /**
     * Returns a list of of targets on the path of the agent
     * @param agentID Agent ID
     * @param targets Target objects
     * @param n Number of targets (pre-set for maximum number)
     * @return Error code (0 = no error)
     */
    err_type_t SHARED_EXPORT targets(id_type_t agentID, TargetInformation *targets, unsigned long *n);

}

#ifdef __cplusplus
}
#endif

#endif // SIMMAP_LIB_H

