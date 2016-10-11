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

#include "RobinBCS.h"

template<>
InputParameters validParams<RobinBCS>()
{
  InputParameters params = validParams<IntegratedBC>();

  // Here we are adding a parameter that will be extracted from the input file by the Parser
  params.addParam<Real>("alpha", 1.0, "Value of convection coefficient multiplied by the coupled value on the boundary");
  params.addParam<Real>("beta", 1.0, "Value of T_infinity multiplied by the coupled value on the boundary");
  //params.addRequiredCoupledVar("some_var", "Flux Value at the Boundary");
  return params;
}

RobinBCS::RobinBCS(const InputParameters & parameters) :
    IntegratedBC(parameters),
    _alpha(getParam<Real>("alpha")),
    _beta(getParam<Real>("beta"))
    //_some_var_val(coupledValue("some_var"))
{}

Real
RobinBCS::computeQpResidual()
{
  return -_test[_i][_qp]*_alpha*(_beta -_u[_qp]);
}

Real
RobinBCS::computeQpJacobian()
{
  return _test[_i][_qp] * (-_alpha) * _phi[_j][_qp];
}

