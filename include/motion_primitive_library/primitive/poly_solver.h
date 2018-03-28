/**
 * @file poly_solver.h
 * @brief Trajectory generator back-end
 */
#ifndef POLY_SOLVER_H
#define POLY_SOLVER_H

#include <stdio.h>
#include <iostream>
#include <memory>
#include <Eigen/Core>
#include <Eigen/StdVector>
#include <Eigen/LU>
#include <motion_primitive_library/primitive/poly_traj.h>

/**
 * @brief Trajectory generator back-end class
 *
 * Given intermediate waypoints and associated time allocation, generate the n-th order polynomials
 */
template <int Dim>
class PolySolver {
  public:
    /**
     * @brief Simple constructor
     * @param smooth_derivative_order The max derivative we want continuous
     * @param minimize_derivative The derivative to minimize
     */
    PolySolver(unsigned int smooth_derivative_order, unsigned int minimize_derivative, bool debug = false);
    /**
     * @brief Solve the trajector as defined in constructor
     * @param waypoints Intermediate waypoints that the trajectory pass through
     * @param dts Time allocation for each segment
     *
     * Note that the element in dts is the time for that segment
     */
    bool solve(const vec_E<Waypoint<Dim>>& waypoints, const std::vector<decimal_t> &dts);

    bool gradient_descent(const vec_E<Waypoint<Dim>>& waypoints,
        const std::vector<double>& dts,
        int m_sample);
 
    void set_obs(const vec_Vecf<Dim>& obs) { obs_ = obs; }

    ///Get the solved trajectory
    std::shared_ptr<PolyTraj<Dim>> getTrajectory();

  private:
    std::pair<MatDf, MatDf> get_Jc(const MatDf& T, const VecDf& deltaT, const MatDf& V, const MatDf& P, const MatDf& Lpp);
    double get_c(const Vecf<Dim>& pt, const Vecf<Dim>& ref_pt);
    Vecf<Dim> get_c_derivative(const Vecf<Dim>& pt, const vec_Vecf<Dim>& ref_pts);
    VecDf getT(int N, double t);
    ///Number of coefficients of a polynomial
    unsigned int N_;
    ///Order of derivative to optimize
    unsigned int R_;
    ///Enble output on screen
    bool debug_;
    ///Solved trajectory
    std::shared_ptr<PolyTraj<Dim>> ptraj_;
    vec_E<MatDf> B_;
    vec_Vecf<Dim> obs_;
};

///PolySolver for 2D
typedef PolySolver<2> PolySolver2;

///PolySolver for 3D
typedef PolySolver<3> PolySolver3;
#endif
