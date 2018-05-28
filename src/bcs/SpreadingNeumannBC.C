//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "SpreadingNeumannBC.h"

//registerMooseObject("ExampleApp", SpreadingNeumannBC);
//This may be for newer applications

template <>
InputParameters
validParams<SpreadingNeumannBC>()
{
  InputParameters params = validParams<IntegratedBC>();

  // Here we are adding a parameter that will be extracted from the input file by the Parser
  params.addParam<Real>("phasefactor", 1.0, "Amplitude of the NeumannBC");
  //params.addRequiredCoupledVar("some_var", "Flux Value at the Boundary");
  return params;
}

SpreadingNeumannBC::SpreadingNeumannBC(const InputParameters & parameters)
  : IntegratedBC(parameters),
    _factor(getParam<Real>("phasefactor"))
    //_some_var_val(coupledValue("some_var"))
{
}

// the residual is formed according to the phase field method's boundary condtion
// k*dC/dn + f_w^', where f_w = factor*(c-c^3/3)

Real
SpreadingNeumannBC::computeQpResidual()
{
  return _test[_i][_qp] * _factor * (_u[_qp]*_u[_qp]-1.0);
}

Real
SpreadingNeumannBC::computeQpJacobian()
{
  return _test[_i][_qp] * _factor * _phi[_j][_qp] * 2.0 *_u[_qp];
}
