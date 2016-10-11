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
// this kernel constitutes the sum of backstress convection(nabla C * nabla S and c*laplacian(S))
// -alpha*nabla(C)*nabla(S) – alpha*C*nabla^2(S) = -alpha*nabla(C*nabla(S))
// where, alpha = D_0*Omega/(RT).
//Note that in this kernel S is the main variable and c is the coupled variable
#include "LaplacianStress.h"

template<>
InputParameters validParams<LaplacianStress>()
{
  InputParameters params = validParams<Kernel>();

  params.addRequiredCoupledVar("concentration", "The variable representing the stress tr(sigma_ij)/3.");
  // Add a required parameter.  If this isn't provided in the input file MOOSE will error.
  //params.addRequiredParam<MaterialPropertyName>("D_name", "The diffusivity used with the kernel");
  params.addRequiredParam<MaterialPropertyName>("Youngs_modulus", "Isotropic young's modulus of tin");
  params.addRequiredParam<MaterialPropertyName>("Poissons_ratio", "Poisson's ratio of tin");
  //params.addRequiredParam<Real>("Q_asterik", "The heat of transport of Cu (Q*) in Sn at T in J/mol");

  // Add a parameter with a default value.  This value can be overriden in the input file.
  params.addParam<Real>("kb", 1.38e-23, "The Boltzmann constant in J/K");
  params.addParam<Real>("omega", 2.71e-23, "Atomic volume of tin in m^3");
  //params.addParam<Real>("e", 1.6e-19, "electronic charge in coulomb");
  //params.addParam<Real>("rho", 5.5e-7, "electric resistivity of liquid tin in ohm m");
  params.addParam<Real>("T_c", 523.0, "Temperature of the solder medium in K");


  return params;
}

LaplacianStress::LaplacianStress(const InputParameters & parameters) :
    Kernel(parameters),
    // Save off the coupled variable identifier for use in
    // computeQpOffDiagJacobian
    _c_var(coupled("concentration")),
    // Save off the coupled value for use in Residual 
    _c(coupledValue("concentration")),
    // Couple to the gradient of the pressure
    _grad_c(coupledGradient("concentration")),
    // Grab necessary material properties
    //_D(getMaterialProperty<Real>("D_name")),
    _E(getMaterialProperty<Real>("Youngs_modulus")),
    _nu(getMaterialProperty<Real>("Poissons_ratio")),
    _kb(getParam<Real>("kb")),
    _omega(getParam<Real>("omega")),
    _T_c(getParam<Real>("T_c"))
{
}

Real
LaplacianStress::computeQpResidual()
{
  // See also: E. Majchrzak and L. Turchan, "The Finite Difference
  // Method For Transient Convection Diffusion", Scientific Research
  // of the Institute of Mathematics and Computer Science, vol. 1,
  // no. 11, 2012, pp. 63-72.
  // http://srimcs.im.pcz.pl/2012_1/art_07.pdf

  // http://en.wikipedia.org/wiki/Superficial_velocity
  // Add the codes to describe the diffusion of backstress
  //RealVectorValue stress_velocity =
   //_D[_qp]* _omega * (1.0/(_kb*_T_c)) * _grad_stress[_qp];

  return (_grad_u[_qp] + (2.0*_E[_qp]* _omega/(9.0*(1.0-_nu[_qp]))) * _grad_c[_qp] )* _grad_test[_i][_qp];
}

Real
LaplacianStress::computeQpJacobian()
{
  //RealVectorValue stress_velocity =
    //2.0*_E[_qp]* _omega * 1.0/(9.0(1.0-_nu[_qp])) * _grad_stress[_qp];

  return _grad_phi[_j][_qp] * _grad_test[_i][_qp];
}

Real
LaplacianStress::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _c_var)
  {
    (2.0*_E[_qp]* _omega/(9.0*(1.0-_nu[_qp])))*
           _grad_phi[_j][_qp]* _grad_test[_i][_qp];
  }

  return 0.0;
}
