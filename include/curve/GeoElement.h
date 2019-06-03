//
// Created by Jens Klimke on 30.03.18.
//

#ifndef SIMMAP_CURVE_GEOELEMENT_H
#define SIMMAP_CURVE_GEOELEMENT_H

#include <vector>
#include <Eigen/Core>
#include <base/definitions.h>


namespace simmap {
namespace curve {


/**
 * class GeoElement
 * @author Jens Klimke jens.klimke@rwth-aachen.de
 */
    class GeoElement {

    private:

        Eigen::Vector3d r0 = {0.0, 0.0, 0.0};
        double phi0 = 0.0;
        double len = 0.0;

    public:

        enum class Type {
            LINE, ARC, SPIRAL, POLY_3, PARAM_POLY_3, CURVE
        };


        /**
         * Default destructor
         */
        virtual ~GeoElement() = default;


        /**
         * Returns the type of the geo element
         * @return Type
         */
        virtual Type type() const = 0;


        /**
         * Returns the parameters of the geo element
         * @return Parameters
         */
        virtual Eigen::RowVectorXd parameters() const = 0;


        /**
         * Sets the start point of the geo element
         * @param pos Start point
         */
        virtual void startPoint(const def::CurvePoint &pos);


        /**
         * Returns the start point of the geo element
         * @return Start point
         */
        virtual def::CurvePoint startPoint() const;


        /**
         * Returns the end point of the geo element
         * @return
         */
        virtual def::CurvePoint endPoint() const;

        /**
         * Returns the start curvature of the geo element
         * @return start curvature
         */
        virtual double startCurvature() const = 0;


        /**
         * Returns the end curvature of the geo element
         * @return end curvature
         */
        virtual double endCurvature() const = 0;


        /**
         * Returns the curvature at the given position
         * @param s Position
         * @return Curvature
         */
        virtual double curvature(double s) const = 0;


        /**
         * Sets the length of the geo element
         * @param l Length
         */
        virtual void length(double l);


        /**
         * Returns the length of the geo element
         * @return
         */
        virtual double length() const;


        /**
         * Returns the curve positions at the given positions
         * @param v Positions
         * @return Curve positions
         */
        virtual std::vector<def::CurvePoint> positions(const Eigen::RowVectorXd &v) const;


        /**
         * Returns the curve positions at the given positions
         * @param v Positions
         * @return Curve positions
         */
        virtual std::vector<def::CurvePoint> operator()(const Eigen::RowVectorXd &v) const;


        /**
         * Returns the curve position at the given position
         * @param s Position
         * @return Curve position
         */
        virtual def::CurvePoint operator()(double s) const;


        /**
         * Returns the curve position at the given position
         * @param s Position
         * @return Curve position
         */
        virtual def::CurvePoint pos(double s) const = 0;


        /**
         * Returns the curve position at the given position with respect to a given offset
         * @param s Position
         * @param d Offset
         * @return Curve position
         */
        def::CurvePoint pos(double s, double d) const;


        /**
         * Calculates the step size, which results in a maximum angle diff and maximum step size (start and end point of
         * geo element are always included)
         * @param dPhi_max Maximum angle diff
         * @param s_max Maximum step size
         * @return
         */
        virtual Eigen::RowVectorXd steps(double dPhi_max, double s_max) const;


        /**
         * Returns a geo element which is equal to the given geo element but reversed in terms of the process variable s
         * @param geo Reversed geo element to be stored in (must be same type as original or compatible)
         */
        virtual void reverse(GeoElement *geo) const = 0;


        /**
         * Creates a blank objects of the derived class
         * @return created object
         */
        virtual GeoElement *createBlank() const = 0;

    };

}} // namespace ::simmap::server

#endif // SIMMAP_SERVER_GEOELEMENT_H
