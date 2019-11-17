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
// Created by Jens Klimke on 2018/10/17
//

#ifndef SIMMAP_SERVER_PATH_H
#define SIMMAP_SERVER_PATH_H


#include <graph/Path.h>
#include <graph/Graph.h>
#include "LaneEdge.h"
#include "Track.h"

namespace simmap {
namespace server {


class MapCoordinate;
class Track;


/**
 * class Path
 * @author Jens Klimke jens.klimke@rwth-aachen.de
 */
class Path : public ::graph::Path {

public:


    struct NeighborInformation {

        int    index;
        double offset;
        bool   accessible;
        bool   allowed;
        bool   sameDir;

    };

    typedef std::pair<NeighborInformation, Path> Neighbor;


protected:


    struct Matcher {

        const Path *path;
        const Eigen::Vector3d *xyz;

        def::CurvePoint operator()(double s, double &d) const;
        Eigen::Vector2d bounds() const;

    };


    /* Attributes */

    double _d = 0.0;


public:




    /**
     * Constructor
     */
    Path() = default;


    /**
     * Destructor
     */
    ~Path() override = default;


    /**
     * Updates the track by adding edges along the track until the lengths are reached
     * TODO: move to Path (create a generic edge validity checker)
     * @param lenHead Length of the path to the head
     * @param lenBack Length of the path to the back
     * @param track Track
     */
    void updatePath(double lenHead, double lenBack, Track &track);


    /**
     * Updates the current position by a long. position in the path
     * @param s New position
     * @param d Lateral offset
     */
    void position(double s, double d = 0.0);


    /**
     * Returns a pointer to the position in the path
     * @return Position
     */
    MapCoordinate position() const;


    /**
     * Returns the position in the path relative to the current position
     * @param s The relative position from the current position
     * @param d Lateral offset
     * @return Position
     */
    MapCoordinate positionAt(double s, double d = 0.0) const;


    /**
     * Returns the head position of the path
     * @return Position
     */
    MapCoordinate head() const;


    /**
     * Returns the back position of the path
     * @return Position
     */
    MapCoordinate back() const;


    /**
     * Returns the track of the path
     * @return The track
     */
    Track track() const;


    /** Checks whether the track element belongs to the path with respect to the direction
     * @param trackElement A track element
     * @return Flag if the edge is part of the track element
     * TODO: rename
     */
    bool hasRoad(const Track::TrackElement &trackElement) const;


    /**
     * Creates a path along the
     * @param position Start coordinate
     * @param lenHead Length of the path to the front (positive)
     * @param lenBack Length of the path to the back (also positive!)
     * @param track The track on which the path shall be followed
     * @return The created path
     */
    static void create(Path &path, Track &track, double lenHead, double lenBack, const MapCoordinate &position);


    /**
     * Creates all pathes beginning from the given point. The function also checks for loops
     * @param paths Vector in which the paths are stored
     * @param edge Edge to be started
     * @param len The maximum length of the paths
     */
    static void createRecursively(std::list<Path*> &paths, const LaneEdge *edge, double len);


    /**
     * Matches the given xyz-Coordinate on the path
     * @param xyz Absolute position in the world
     * @param s Stores the longitudinal distance from the current position (can be preset to estimate a close result)
     * @param radius Sets the initial search radius around s
     * @return Error of the matching
     */
    double match(const Eigen::Vector3d &xyz, double &s, double radius = INFINITY) const;


    /**
     * Creates a list of neighbored paths along the track
     * @return List of paths
     */
    std::list<Neighbor> neighboredPaths(Track &track) const;


    /**
     * Calculates the distances to the given map coordinate. If the map coordinate is not part of the path, then an
     * empty vector is returned. Note that one map coordinate can be several times part of a path, if the path is
     * a circle for example.
     * @param mc Map coordinate
     * @return Distance to the current position
     */
    std::vector<double> distance(const MapCoordinate &mc) const;



protected:


    /**
     * Searches for the next track road, the map coordinate is part of and moves the previous roads the the end of the track
     * @param track Track
     * @param coordinate Map coordinate
     * @return
     */
    static void updateTrack(const MapCoordinate &coordinate, Track &track);


    /**
     * Searches and returns the track element (road) corresponding to the given positon
     * @param coordinate The coordinate to be searched for
     * @param track The track to be searched in
     * @return The track index
     */
    static size_t findTrackElement(const MapCoordinate &coordinate, const Track &track);


    /**
     * Extends the path to reach the distance
     * @param ds Distance
     * @param track Track
     * @param trackIndex Index of the current track element
     * TODO: test this extensively (some changes done)
     */
    void extendPath(double ds, const Track &track, size_t &trackIndex);


    /**
     * Extends the given path recursively by creating an copy and adding segments
     * @param paths Vector in which the paths are stored
     * @param len Maximum length
     */
    static void extendRecursively(std::list<Path *> &paths, double len);


    /**
     * Executes a match algorithm
     * @param matcher Matcher struct
     * @param s Initial position (will be updated)
     * @param s_eps Minimum step residuum
     * @param f_eps Minimum function residuum
     * @param max_f Maximum number of evaluations
     * @return Final error
     */
    static double matchExec(const Path::Matcher *matcher, double *s, double s_eps, double f_eps, size_t max_f);


};

}} // namespace ::simmap::server



#endif // SIMMAP_SERVER_PATH_H
