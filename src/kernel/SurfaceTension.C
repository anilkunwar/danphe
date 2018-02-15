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

#include "SurfaceTension.h"
//\\ This kernel acts like a source kernel
template<>
InputParameters validParams<SurfaceTension>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Implements a source term proportional to the value of a coupled "
                             "variable. Weak form: $(\\psi_i, -\\sigma v)$.");
  //params.addRequiredCoupledVar("Temperature", "The variable representing the temperature.");
  params.addRequiredCoupledVar("v", "The coupled variable which provides the force");
  // Add a required parameter.  If this isn't provided in the input file MOOSE will error.
  //params.addRequiredParam<MaterialPropertyName>("c_sat", "The saturated solubility used with the kernel");
  //params.addRequiredParam<Real>("k_chem", "the product of chemical reaction constant and ration s/v");
  params.addParam<MaterialPropertyName>("function_name", "func_name", "The name of the function");
  //params.addRequiredParam<MaterialPropertyName>("func_name", "The name of the function");
  //params.addParam<Real>("coef", 1.0, "Coefficent ($\\sigma$) multiplier for the coupled force term.");
  params.addParam<Real>("sigmacoef", 1.0, "Coefficent ($\\sigma$) multiplier for the coupled force term.");
  //params.addRequiredParam<Real>("coef",  "Coefficent ($\\sigma$) multiplier for the coupled force term.");


  // Add a parameter with a default value.  This value can be overriden in the input file.
  //params.addParam<Real>("kb", 1.38e-23, "The Boltzmann constant in J/K");


  return params;
}

SurfaceTension::SurfaceTension(const InputParameters & parameters) :
    Kernel(parameters),
    // Save off the coupled variable identifier for use in
    // computeQpOffDiagJacobian
    _v_var(coupled("v")),
    // Save off the coupled value for use in Residual 
    _v(coupledValue("v")),
    // Couple to the gradient of the pressure
    //_grad_T(coupledGradient("Temperature")),
    // Grab necessary material properties
    //_cs(getMaterialProperty<Real>("c_sat")),
    //_kc(getParam<Real>("k_chem"))
    _gammafn(getMaterialProperty<Real>("function_name")),
    //_coef(getParam<Real>("coef"))
    _sigmazero(getParam<Real>("sigmacoef"))
    //_kb(getParam<Real>("coef"))
{
}

Real
SurfaceTension::computeQpResidual()
{
  //return _kc *(_cs[_qp]- _u[_qp]) * _test[_i][_qp];
  //return -_kc * _u[_qp] * _test[_i][_qp];
  return -_sigmazero * _gammafn[_qp] * _v[_qp] * _test[_i][_qp];
}

Real
SurfaceTension::computeQpJacobian()
{
 //return  -_kc *  _phi[_j][_qp] * _test[_i][_qp]; 
 return 0;
}

Real
SurfaceTension::computeQpOffDiagJacobian(unsigned int jvar)
//{
  //if (jvar == _T_var)
  //{
   //   0.0;
  //  _D[_qp]*_Qh* (-_kb/_T[_qp]*_T[_qp])*(2*_grad_T[_qp]*_phi[_j][_qp]/ _T[_qp] +
  //         _grad_phi[_j][_qp])*_grad_u[_qp] * _test[_i][_qp];
  //}

  //return 0.0;
//}
{
  if (jvar == _v_var)
    return -_sigmazero * _gammafn[_qp] * _phi[_j][_qp] * _test[_i][_qp];
  return 0.0;
}
