//
// Created by klimke on 15.08.2018.
//

#include <base/definitions.h>
#include <Eigen/Core>
#include "C3Spline.h"

namespace simmap {
namespace curve {


    C3Spline::C3Spline(const Eigen::RowVectorXd& s, const Eigen::MatrixXd& p) {

        // if vector is empty, set to zero
        if(p.cols() == 0)
            return;

        if(p.rows() == 4)
            fromDefinition(s, p);
        else if(p.rows() == 2)
            fromValuesAndDerivatives(s, p.row(0), p.row(1));
        else if(p.rows() == 1)
            fromValues(s, p);
        else
            throw std::invalid_argument("Parameter vector p is invalid");

    }


    C3Spline::C3Spline(double width) {

        fromValues(Eigen::RowVector2d(0.0, 1.0), Eigen::RowVector2d(width, width));

    }


    void C3Spline::fromDefinition(const Eigen::RowVectorXd& s, const Eigen::Matrix<double, 4, Eigen::Dynamic>& p) {

        if (s.cols() - 1 != p.cols())
            throw std::invalid_argument("s must be +1 larger than p");

        // set length
        double s0 = -INFINITY;

        // iterate over vector
        for(int i = 0; i < p.cols(); ++i) {

            // check s
            if (s(i) <= s0)
                std::invalid_argument("s must increase monotonically");

            // create element such that derivatives are zero and values are as given
            insert(s(i), p(0,i), p(1,i), p(2,i), p(3,i));

            // save last s
            s0 = s(i);

        }

        length(s(s.size() - 1) - s(0));

    }


    void C3Spline::fromValues(const Eigen::RowVectorXd& s, const Eigen::RowVectorXd& w) {

        fromValuesAndDerivatives(s, w, Eigen::RowVectorXd::Zero(s.cols()));

    }


    void C3Spline::fromValuesAndDerivatives(const Eigen::RowVectorXd& s, const Eigen::RowVectorXd& w, const Eigen::RowVectorXd& der) {

        if (der.cols() != w.cols() || s.cols() != w.cols())
            throw std::invalid_argument("Vectors must have the same length");

        // iterate over vector
        for(int i = 1; i < w.cols(); ++i) {

            // check s
            if (s(i - 1) >= s(i))
                throw std::invalid_argument("s must increase strict monotonically");

            // create element such that derivatives are zero and values are as given
            sequence::emplace(s(i - 1), base::poly1::order3_fromValueAndDerivative(0.0,
                    s(i) - s(i - 1), w(i - 1), der(i - 1), w(i), der(i)));

        }

        // set length
        length(s(s.size() - 1) - s(0));

    }


    void C3Spline::insert(double s, double p0, double p1, double p2, double p3) {

        sequence::emplace(s, base::poly1(p0, p1, p2, p3));

    }


    Eigen::RowVectorXd C3Spline::operator() (const Eigen::RowVectorXd& s) const {

        // create zeros vector
        Eigen::RowVectorXd res = Eigen::RowVectorXd::Zero(s.cols());

        // check if spline has elements
        if(empty())
            return res;

        // iterate over elements and calculate result
        for(size_t i = 0; i < s.cols(); ++i)
            res(i) = operator()(s(i));

        return res;

    }


    double C3Spline::operator()(double s) const {

        // check if spline has elements
        if(empty())
            return 0.0;

        auto e = atPos(s);
        return e.element(e.position);

    }


    Eigen::RowVectorXd C3Spline::der(const Eigen::RowVectorXd& s) const {

        // get elements and create result vector
        Eigen::RowVectorXd res(s.size());

        // iterate over elements and calculate result
        for(size_t i = 0; i < s.cols(); ++i)
            res(i) = der(s(i));

        return res;

    }


    double C3Spline::der(double s) const {

        auto e = atPos(s);
        return e.element.der()(e.position);

    }


    Eigen::RowVectorXd C3Spline::steps(double dPhi_max, double s_max) const {

        // create vector
        std::vector<double> stps;
        double s = 0.0; double s0 = 0.0;

        // iterate over elements
        for(auto &e : entries()) {

            // get element and position
            const base::poly1* p = e.element;
            double s1 = e.position + e.length;

            while (s < s1) {

                // add s to vector
                stps.push_back(s);

                // calculate derivative
                base::poly1 der = p->der().der();

                // get ds and calculate s
                double k0 = fabs(der(s - s0));
                double sigma = fabs(der.der()(s - s0));

                // calculate step
                double t0 = dPhi_max / k0;
                double t1 = t0;

                // check sigma
                if (sigma > def::EPS_CURVATURE)
                    t0 = (1.0 / sigma) * (-1.0 * k0 + sqrt(k0 * k0 + 2.0 * dPhi_max * sigma));

                // add step to s
                s += std::min(std::min(t0, s1 - s0), s_max);

            }

            // add last s
            s = std::min(s, s1);
            s0 = s1;

        }

        // add last point
        stps.push_back(s);

        // put values to vector
        Eigen::RowVectorXd res(stps.size());
        for(size_t i = 0; i < stps.size(); ++i)
            res(i) = stps[i];

        return res;

    }

}}