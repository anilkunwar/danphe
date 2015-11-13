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

#include "ThermalConvection.h"

template<>
InputParameters validParams<ThermalConvection>()
{
  InputParameters params = validParams<Kernel>();

  params.addRequiredCoupledVar("Temperature", "The variable representing the temperature.");
  // Add a required parameter.  If this isn't provided in the input file MOOSE will error.
  params.addRequiredParam<MaterialPropertyName>("D_name", "The diffusivity used with the kernel");
  params.addRequiredParam<Real>("Q_asterik", "The heat of transport of Cu (Q*) in Sn at T in J/mol");

  // Add a parameter with a default value.  This value can be overriden in the input file.
  params.addParam<Real>("kb", 1.38e-23, "The Boltzmann constant in J/K");


  return params;
}

ThermalConvection::ThermalConvection(const InputParameters & parameters) :
    Kernel(parameters),
    // Save off the coupled variable identifier for use in
    // computeQpOffDiagJacobian
    _T_var(coupled("T")),
    // Save off the coupled value for use in Residual 
    _T(coupledValue("T")),
    // Couple to the gradient of the pressure
    _grad_T(coupledGradient("T")),
    // Grab necessary material properties
    _D(getMaterialProperty<Real>("D_name")),
    _Qh(getParam<Real>("Q_asterik")),
    _kb(getParam<Real>("kb"))
{
}

Real
ThermalConvection::computeQpResidual()
{
  // See also: E. Majchrzak and L. Turchan, "The Finite Difference
  // Method For Transient Convection Diffusion", Scientific Research
  // of the Institute of Mathematics and Computer Science, vol. 1,
  // no. 11, 2012, pp. 63-72.
  // http://srimcs.im.pcz.pl/2012_1/art_07.pdf

  // http://en.wikipedia.org/wiki/Superficial_velocity
  RealVectorValue thermal_velocity =
    _D[_qp]*_Qh * -(_kb/_T[_qp]*_T[_qp]) * _grad_T[_qp];

  return thermal_velocity * _grad_c[_qp] * _test[_i][_qp];
}

Real
ThermalConvection::computeQpJacobian()
{
  RealVectorValue thermal_velocity =
   _D[_qp]* _Qh * -(_kb/_T[_qp]*_T[_qp]) * _grad_T[_qp];

  return thermal_velocity * _grad_phi[_j][_qp] * _test[_i][_qp];
}

Real
ThermalConvection::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _T_var)
  {
    _D[_qp]*_Qh* (_kb[_qp/_T[_qp]*_T[_qp])*(2*_grad_T[_qp]*_phi[_j][_qp]/ _T[_qp] -
           _grad_phi[_j][_qp])*_test[_i][_qp];
  }

  return 0.0;
}
