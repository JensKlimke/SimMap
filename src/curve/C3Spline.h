//
// Created by Jens Klimke on 14.04.18.
//

#ifndef SIMMAP_CURVE_C3SPLINE_H
#define SIMMAP_CURVE_C3SPLINE_H

#include <base/sequence.h>
#include <base/poly1.h>

namespace simmap {
namespace curve {

    /**
     * class C3Spline
     * @author Jens Klimke <jens.klimke@rwth-aachen.de>
     * TODO: comments
     */
    class C3Spline : public base::sequence<base::poly1> {


    private:

        void fromDefinition(const Eigen::RowVectorXd& s, const Eigen::Matrix<double, 4, Eigen::Dynamic>& p);
        void fromValues(const Eigen::RowVectorXd& s, const Eigen::RowVectorXd& w);
        void fromValuesAndDerivatives(const Eigen::RowVectorXd& s, const Eigen::RowVectorXd& w, const Eigen::RowVectorXd& der);

    public:

        C3Spline() = default;
        C3Spline(const C3Spline& ps) = default;
        C3Spline(const Eigen::RowVectorXd& s, const Eigen::MatrixXd& p);
        explicit C3Spline(double width);

        ~C3Spline() = default;

        void insert(double s, double p0, double p1, double p2, double p3);

        Eigen::RowVectorXd operator() (const Eigen::RowVectorXd& s) const;
        double operator() (double s) const;

        Eigen::RowVectorXd der(const Eigen::RowVectorXd& s) const;
        double der(double s) const;

        Eigen::RowVectorXd steps(double dPhi_max, double s_max) const;

    };

}} // namespace ::simmap::curve

#endif // SIMMAP_CURVE_C3SPLINE_H
