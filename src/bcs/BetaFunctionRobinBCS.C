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

#include "BetaFunctionRobinBCS.h"
#include "Function.h"

template<>
InputParameters validParams<BetaFunctionRobinBCS>()
{
  InputParameters params = validParams<IntegratedBC>();

  // Here we are adding a parameter that will be extracted from the input file by the Parser
  //params.addParam<Real>("alpha", 1.0, "Value of convection coefficient multiplied by the coupled value on the boundary");
  params.addRequiredParam<FunctionName>("hfunction", "The function for representation of convection coefficient.");
  params.addRequiredParam<FunctionName>("betafunction", "The function for representation of ambient temperature beta.");
  //params.addParam<Real>("beta", 1.0, "Value of T_infinity multiplied by the coupled value on the boundary");
  //params.addRequiredCoupledVar("some_var", "Flux Value at the Boundary");
  return params;
}

BetaFunctionRobinBCS::BetaFunctionRobinBCS(const InputParameters & parameters) :
    IntegratedBC(parameters),
    //_alpha(getParam<Real>("alpha")),
    _hfunc(getFunction("hfunction")),
    _betafunc(getFunction("betafunction"))
    //_func(getFunction("betafunction"))
    //_some_var_val(coupledValue("some_var"))
{}

Real
BetaFunctionRobinBCS::computeQpResidual()
{
  return -_test[_i][_qp]*_hfunc.value(_t, _q_point[_qp])*(_betafunc.value(_t, _q_point[_qp]) -_u[_qp]);
}

Real
BetaFunctionRobinBCS::computeQpJacobian()
{
  return _test[_i][_qp] * (-_hfunc.value(_t, _q_point[_qp])) * _phi[_j][_qp];
}


