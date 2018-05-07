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

#include "InterfacialAngleNeumannBC.h"


template<>
InputParameters validParams<InterfacialAngleNeumannBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addParam<Real>("cosineoftheta", 0.0, "The cosine of the wetting angle");
  // the expression cos(x) is utilized in parsed function's expression within moose framework
  return params;
}

InterfacialAngleNeumannBC::InterfacialAngleNeumannBC(const InputParameters & parameters):IntegratedBC(parameters),
    _trigonometricvalue(getParam<Real>("cosineoftheta"))
{
}

Real
InterfacialAngleNeumannBC::computeQpResidual()
{

	return -_test[_i][_qp] * _trigonometricvalue * sqrt(2) / 2 * (1 - (_u[_qp] * _u[_qp]));
}
