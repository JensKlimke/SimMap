//
// Created by Jens Klimke on 02.04.18.
//

#ifndef SIMMAP_CURVE_CURVE_H
#define SIMMAP_CURVE_CURVE_H


#include "GeoElement.h"
#include <base/sequence.h>
#include <base/definitions.h>
#include <Eigen/Core>


namespace simmap {
namespace curve {


class Curve : public GeoElement, public base::sequence<GeoElement*>
{

private:


    void _addElement(double s, double ds, double kappa0, double kappa1);


public:

    Curve() = default;
    ~Curve() override = default;

    GeoElement::Type type() const override;
    Eigen::RowVectorXd parameters() const override;

    double startCurvature() const override;
    double endCurvature() const override;
    double curvature(double s) const override;

    void curvature(const Eigen::RowVectorXd& s, const Eigen::RowVectorXd& kappa);
    void curvature(const Eigen::RowVectorXd& s, const Eigen::RowVectorXd& kappa0, const Eigen::RowVectorXd& kappa1);

    void startPoint(const def::CurvePoint &pos) override;
    def::CurvePoint startPoint() const override;

    Eigen::RowVectorXd steps(double dPhi_max, double s_max) const override;
    double length() const override;
    void length(double len) override;
    def::CurvePoint pos(double s) const override;

    void reverse(GeoElement *geo) const override;
    GeoElement* createBlank() const override;

};

}} // namespace ::simmap::server

std::ostream& operator<< (std::ostream& o, const simmap::curve::Curve& crv);

#endif // SIMMAP_CURVE_CURVE_H
