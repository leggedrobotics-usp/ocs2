/*
 * AnymalKinematics.h
 *
 *  Created on: Aug 11, 2017
 *      Author: farbod
 */

#ifndef ANYMAL_ANYMALKINEMATICS_H_
#define ANYMAL_ANYMALKINEMATICS_H_

#include <c_switched_model_interface/KinematicsModelBase.h>

#include <iit/robots/anymal/transforms.h>
#include <iit/robots/anymal/jacobians.h>

namespace anymal {

class AnymalKinematics : public KinematicsModelBase<12>
{
public:
	enum { LF=0,  RF=1,  LH=2,  RH=3 };

	/**
	 * calculate foot position in Base frame
	 */
	void footPositionBaseFrame(const size_t& footIndex, Eigen::Vector3d& footPosition) override;

	/**
	 * calculate foot Jacobian in Base frame
	 */
	void footJacobainBaseFrame(const size_t& footIndex, Eigen::Matrix<double,6,12>& footJacobian) override;

private:
	// RBD homogeneous transforms of feet
	iit::ANYmal::HomogeneousTransforms::Type_fr_trunk_X_fr_LF_foot fr_trunk_X_fr_LF_foot_;
	iit::ANYmal::HomogeneousTransforms::Type_fr_trunk_X_fr_RF_foot fr_trunk_X_fr_RF_foot_;
	iit::ANYmal::HomogeneousTransforms::Type_fr_trunk_X_fr_LH_foot fr_trunk_X_fr_LH_foot_;
	iit::ANYmal::HomogeneousTransforms::Type_fr_trunk_X_fr_RH_foot fr_trunk_X_fr_RH_foot_;

	// RBD Jacobian of feet
	iit::ANYmal::Jacobians::Type_fr_trunk_J_fr_LF_foot fr_trunk_J_fr_LF_foot_;
	iit::ANYmal::Jacobians::Type_fr_trunk_J_fr_RF_foot fr_trunk_J_fr_RF_foot_;
	iit::ANYmal::Jacobians::Type_fr_trunk_J_fr_LH_foot fr_trunk_J_fr_LH_foot_;
	iit::ANYmal::Jacobians::Type_fr_trunk_J_fr_RH_foot fr_trunk_J_fr_RH_foot_;

};

}  // end of anymal namespace

#include "implementation/AnymalKinematics.h"

#endif /* ANYMAL_ANYMALKINEMATICS_H_ */
