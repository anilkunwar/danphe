/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "CHConvection.h"

/**
 * This function defines the valid parameters for
 * this Kernel and their default values
 */
template <>
InputParameters
validParams<CHConvection>()
{
  InputParameters params = validParams<Kernel>();
 // params.addRequiredParam<RealVectorValue>("velocity", "Velocity Vector");
   params.addCoupledVar(
      "velocity_x", 0, "The variable containing the x-component of the velocity front.");
  params.addCoupledVar(
      "velocity_y", 0, "The variable containing the y-component of the velocity front.");
  params.addCoupledVar(
	"velocity_z", 0, "The variable containing the z-component of the velocity front.");
  return params;
}

CHConvection::CHConvection(const InputParameters & parameters)
  : // You must call the constructor of the base class first
    Kernel(parameters),
 //   _velocity(getParam<RealVectorValue>("velocity")),
    _velocity_x(coupledValue("velocity_x")),
    _velocity_y(coupledValue("velocity_y")),
	_velocity_z(coupledValue("velocity_z")),
	_x_vel_var(coupled("velocity_x")),
    _y_vel_var(coupled("velocity_y")),
	_z_vel_var(coupled("velocity_z"))
{
//  _velocity(0) = _velocity_x[_qp];
 // _velocity(1) = _velocity_y[_qp];
 // _velocity(2) = _velocity_z[_qp];
}


Real
CHConvection::computeQpResidual()
{
  _velocity(0) = _velocity_x[_qp];
  _velocity(1) = _velocity_y[_qp];
  _velocity(2) = _velocity_z[_qp];
  
//	const RealVectorValue vec(_velocity_x[_qp], _velocity_y[_qp], _velocity_z[_qp]);	
  // velocity * _grad_u[_qp] is actually doing a dot product
  return _test[_i][_qp] * (_velocity * _grad_u[_qp]);
}

Real
CHConvection::computeQpJacobian()
{
	_velocity(0) = _velocity_x[_qp];
  _velocity(1) = _velocity_y[_qp];
  _velocity(2) = _velocity_z[_qp];
//	const RealVectorValue vec(_velocity_x[_qp], _velocity_y[_qp], _velocity_z[_qp]);
  // the partial derivative of _grad_u is just _grad_phi[_j]
  return _test[_i][_qp] * (_velocity * _grad_phi[_j][_qp]);
 // return _test[_i][_qp] * (vec * _grad_phi[_j][_qp]);
}
