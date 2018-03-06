//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
// This kernel is for computation of NSGravityBodyForce in general
// For BoussinesqApproximation, INSMomentumGravity kernel can be utilized
// Modified from INSBase.C and INSMomentumBase.C kernels

#include "BoussinesqBodyForce.h"

template <>
InputParameters
validParams<BoussinesqBodyForce>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("This class computes the gravity force contribution for boussinesq approximation.");
  // Coupled variables
   params.addRequiredParam<unsigned>("component", "The velocity component that this is applied to.");
  params.addCoupledVar("u", 0, "x-velocity");
  params.addCoupledVar("v", 0, "y-velocity"); // only required in 2D and 3D
  params.addCoupledVar("w", 0, "z-velocity"); // only required in 3D
  params.addCoupledVar("p", 0,"pressure");

  params.addParam<RealVectorValue>(
      "gravity", RealVectorValue(0, 0, 0), "Direction of the gravity vector");

  //params.addParam<MaterialPropertyName>("mu_name", "mu", "The name of the dynamic viscosity");
  // The strength of the acceleration in the _component direction.  Make this
  // value negative if you want force in the -_component direction.
  //params.addRequiredParam<Real>("acceleration", "The body force vector component.");
  params.addRequiredParam<MaterialPropertyName>("rho_boussinesq", "density name as a function of alpha and T_grad"); //use of DerivativeParsedMaterial type required
  return params;
}

BoussinesqBodyForce::BoussinesqBodyForce(const InputParameters & parameters)
  : Kernel(parameters), 
   _component(getParam<unsigned>("component")),
 // Coupled variables
    _u_vel(coupledValue("u")),
    _v_vel(coupledValue("v")),
    _w_vel(coupledValue("w")),
    _p(coupledValue("p")),

    // Gradients
    _grad_u_vel(coupledGradient("u")),
    _grad_v_vel(coupledGradient("v")),
    _grad_w_vel(coupledGradient("w")),
    _grad_p(coupledGradient("p")),

    // Variable numberings
    _u_vel_var_number(coupled("u")),
    _v_vel_var_number(coupled("v")),
    _w_vel_var_number(coupled("w")),
    _p_var_number(coupled("p")),

    _gravity(getParam<RealVectorValue>("gravity")),

  // Material properties
  //  _mu(getMaterialProperty<Real>("mu_name")),
 //_acceleration(getParam<Real>("acceleration")),
  _rho_bq(getMaterialProperty<Real>("rho_boussinesq"))
 
{
}

Real
BoussinesqBodyForce::computeQpResidual()
{
  // -rho * g * phi
  //RealVectorValue gravity_term = _gravity;
  //return -gravity_term*_rho_bq[_qp] * _test[_i][_qp];
  return -_rho_bq[_qp] * _gravity(_component) * _test[_i][_qp];
}

Real
BoussinesqBodyForce::computeQpJacobian()
{
  return 0.0;
}

Real
BoussinesqBodyForce::computeQpOffDiagJacobian(unsigned int jvar)
{
  //if (jvar == _rho_bq_var_number)
  //  return -_phi[_j][_qp] * _acceleration * _test[_i][_qp];
// the density in BoussinesqBodyForce kernel is brought
//from Material block type

  return 0.0;
}
