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

#include "BackstressConvection.h"

template<>
InputParameters validParams<BackstressConvection>()
{
  InputParameters params = validParams<Kernel>();

  params.addRequiredCoupledVar("hydrostatic", "The variable representing the stress tr(sigma_ij)/3.");
  // Add a required parameter.  If this isn't provided in the input file MOOSE will error.
  params.addRequiredParam<MaterialPropertyName>("D_name", "The diffusivity used with the kernel");
  //params.addRequiredParam<Real>("Q_asterik", "The heat of transport of Cu (Q*) in Sn at T in J/mol");

  // Add a parameter with a default value.  This value can be overriden in the input file.
  params.addParam<Real>("kb", 1.38e-23, "The Boltzmann constant in J/K");
  params.addParam<Real>("omega", 2.71e-23, "Atomic volume of tin in m^3");
  //params.addParam<Real>("e", 1.6e-19, "electronic charge in coulomb");
  //params.addParam<Real>("rho", 5.5e-7, "electric resistivity of liquid tin in ohm m");
  params.addParam<Real>("T_c", 523.0, "Temperature of the solder medium in K");


  return params;
}

BackstressConvection::BackstressConvection(const InputParameters & parameters) :
    Kernel(parameters),
    // Save off the coupled variable identifier for use in
    // computeQpOffDiagJacobian
    _stress_var(coupled("hydrostatic")),
    // Save off the coupled value for use in Residual 
    _stress(coupledValue("hydrostatic")),
    // Couple to the gradient of the pressure
    _grad_stress(coupledGradient("hydrostatic")),
    // Grab necessary material properties
    _D(getMaterialProperty<Real>("D_name")),
    _kb(getParam<Real>("kb")),
    _omega(getParam<Real>("omega")),
    _T_c(getParam<Real>("T_c"))
{
}

Real
BackstressConvection::computeQpResidual()
{
  // See also: E. Majchrzak and L. Turchan, "The Finite Difference
  // Method For Transient Convection Diffusion", Scientific Research
  // of the Institute of Mathematics and Computer Science, vol. 1,
  // no. 11, 2012, pp. 63-72.
  // http://srimcs.im.pcz.pl/2012_1/art_07.pdf

  // http://en.wikipedia.org/wiki/Superficial_velocity
  RealVectorValue stress_velocity =
   -_D[_qp]* _omega * (1.0/(_kb*_T_c)) * _grad_stress[_qp];

  return stress_velocity * _grad_u[_qp] * _test[_i][_qp];
}

Real
BackstressConvection::computeQpJacobian()
{
  RealVectorValue stress_velocity =
    -_D[_qp]* _omega * (1.0/(_kb*_T_c)) * _grad_stress[_qp];

  return stress_velocity * _grad_phi[_j][_qp] * _test[_i][_qp];
}

Real
BackstressConvection::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _stress_var)
  {
    -_D[_qp]* _omega * (1.0/(_kb*_T_c)) * _grad_u[_qp] *
           _grad_phi[_j][_qp]* _test[_i][_qp];
  }

  return 0.0;
}
