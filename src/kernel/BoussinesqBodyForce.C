//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "BoussinesqBodyForce.h"

template <>
InputParameters
validParams<BoussinesqBodyForce>()
{
  InputParameters params = validParams<NSKernel>();
  params.addClassDescription("This class computes the gravity force contribution.");
  // The strength of the acceleration in the _component direction.  Make this
  // value negative if you want force in the -_component direction.
  params.addRequiredParam<Real>("acceleration", "The body force vector component.");
  params.addRequiredParam<MaterialPropertyName>("rho_name", "density name as a function of alpha and T_grad"); //use of DerivativeParsedMaterial type required
  return params;
}

BoussinesqBodyForce::BoussinesqBodyForce(const InputParameters & parameters)
  : NSKernel(parameters), 
 _acceleration(getParam<Real>("acceleration")),
 _rho_bq(getMaterialProperty<Real>("rho_name"))
 
{
}

Real
BoussinesqBodyForce::computeQpResidual()
{
  // -rho * g * phi
  return -_rho_bq[_qp] * _acceleration * _test[_i][_qp];
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
