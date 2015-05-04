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

#include "CoupledPotential.h"

template<>
InputParameters validParams<CoupledPotential>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<std::string>("mob_name", "mobtemp", "The mobility used with the kernel");
  params.addRequiredCoupledVar("potential", "The variable representing the electrical potential.");
  return params;
}

CoupledPotential::CoupledPotential(const std::string & name, InputParameters parameters) :
    Kernel(name, parameters),

    // Couple to the gradient of the potential
    _potential_gradient(coupledGradient("potential")),

    // Save off the coupled variable identifier for use in computeQpOffDiagJacobian
    //_potential_var(coupled("potential")),

    // Grab necessary material properties
    _conductivity(getParam<Real>("conductivity"))

    // Get the mobility parameters
    _mob_name(getParam<std::string>("mob_name")),
    _mob(getMaterialProperty<Real>(_mob_name))
{
}

Real
CoupledPotential::computeQpResidual()
{
   return _mob[_qp] * _conductivity[_qp] * _potential_gradient[_qp] * _u[_qp] * _test[_i][_qp];
}

Real
CoupledPotential::computeQpJacobian()
{
  return _mob[_qp] *  _conductivity[_qp] * _potential_gradient[_qp] *_phi[_j][_qp] * _test[_i][_qp];
}
