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

#include "ReactionTerm.h"

template<>
InputParameters validParams<ReactionTerm>()
{
  InputParameters params = validParams<Kernel>();

  params.addRequiredCoupledVar("Temperature", "The variable representing the temperature.");
  // Add a required parameter.  If this isn't provided in the input file MOOSE will error.
  params.addRequiredParam<MaterialPropertyName>("c_sat", "The saturated solubility used with the kernel");
  params.addRequiredParam<Real>("k_chem", "the product of chemical reaction constant and ration s/v");

  // Add a parameter with a default value.  This value can be overriden in the input file.
  //params.addParam<Real>("kb", 1.38e-23, "The Boltzmann constant in J/K");


  return params;
}

ReactionTerm::ReactionTerm(const InputParameters & parameters) :
    Kernel(parameters),
    // Save off the coupled variable identifier for use in
    // computeQpOffDiagJacobian
    _T_var(coupled("Temperature")),
    // Save off the coupled value for use in Residual 
    _T(coupledValue("Temperature")),
    // Couple to the gradient of the pressure
    _grad_T(coupledGradient("Temperature")),
    // Grab necessary material properties
    _cs(getMaterialProperty<Real>("c_sat")),
    _kc(getParam<Real>("k_chem"))
    //_kb(getParam<Real>("kb"))
{
}

Real
ReactionTerm::computeQpResidual()
{
  //return _kc *(_cs[_qp]- _u[_qp]) * _test[_i][_qp];
  return -_kc * _u[_qp] * _test[_i][_qp];
}

Real
ReactionTerm::computeQpJacobian()
{
 return  -_kc *  _phi[_j][_qp] * _test[_i][_qp]; 
}

Real
ReactionTerm::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _T_var)
  {
      0.0;
  //  _D[_qp]*_Qh* (-_kb/_T[_qp]*_T[_qp])*(2*_grad_T[_qp]*_phi[_j][_qp]/ _T[_qp] +
  //         _grad_phi[_j][_qp])*_grad_u[_qp] * _test[_i][_qp];
  }

  return 0.0;
}
