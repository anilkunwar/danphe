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

#include "ConstantTensorElectricConvection.h"
#include "Material.h"

template<>
InputParameters validParams<ConstantTensorElectricConvection>()
{
  InputParameters params = validParams<Kernel>();

  //params.addRequiredCoupledVar("Temperature", "The variable representing the temperature.");
  // Add a required parameter.  If this isn't provided in the input file MOOSE will error.
  params.addRequiredParam<MaterialPropertyName>("D_name", "The diffusivity used with the kernel");
  //params.addRequiredParam<std::vector<Real> >("j_vector","current density (j) in A/m^2");
  //params.addRequiredParam<std::vector<Real> >("slip_factor","Fraction of calculated slip");
  params.addRequiredParam<Real>("z", "Effective charge number for Cu");
  // Add a parameter with a default value.  This value can be overriden in the input file.
  params.addParam<Real>("kb", 1.38e-23, "The Boltzmann constant in J/K");
  params.addParam<Real>("e", 1.6e-19, "electronic charge in coulomb");
  params.addParam<Real>("rho", 5.5e-7, "electric resistivity of liquid tin in ohm m");
  params.addParam<Real>("T_c", 623.0, "Temperature of the solder medium in K");
  params.addClassDescription("Constant j Electromigration Flux.  v.nabla_c , where v = D*z*e*rho*j_ij/kT");


  return params;
}

ConstantTensorElectricConvection::ConstantTensorElectricConvection(const InputParameters & parameters) :
    Kernel(parameters),
    // Save off the coupled variable identifier for use in
    // computeQpOffDiagJacobian
    //_T_var(coupled("T")),
    // Save off the coupled value for use in Residual 
    //_T(coupledValue("T")),
    // Couple to the gradient of the pressure
    //_grad_T(coupledGradient("T")),
    // Grab necessary material properties
    _D(getMaterialProperty<Real>("D_name")),
    //the current density tensor has already been defined in CurrentDensityMaterial file
    _current_density(getMaterialProperty<RealTensorValue>("current_density")),
    //std::vector<Real> current_density = params.get<std::vector<Real> >("j_vector");
    //std::vector<Real> slip_factor = params.get<std::vector<Real> >("slip_factor");
    _z(getParam<Real>("z")),
    _kb(getParam<Real>("kb")),
    _e(getParam<Real>("e")),
    _rho(getParam<Real>("rho")),
    _T_c(getParam<Real>("T_c"))
{
}
  
Real
ConstantTensorElectricConvection::computeQpResidual()
{
  // See also: E. Majchrzak and L. Turchan, "The Finite Difference
  // Method For Transient Convection Diffusion", Scientific Research
  // of the Institute of Mathematics and Computer Science, vol. 1,
  // no. 11, 2012, pp. 63-72.
  Real electrical_velocity = -_D[_qp]*_z *_e * _rho* _current_density[_qp]/(_kb* _T_c);

  return electrical__velocity * _grad_u[_qp] * _test[_i][_qp];
}

Real
ConstantTensorElectricConvection::computeQpJacobian()
{
  Real electrical_velocity = -_D[_qp]*_z *_e * _rho* _current_density[_qp]/(_kb* _T_c);

  return electrical_velocity * _grad_phi[_j][_qp] * _test[_i][_qp];
}


//only one variable and thus no computeQpOffDiagJacobian
//Real
//ConstantTensorElectricConvection::computeQpOffDiagJacobian(unsigned int jvar)
//{
//  if (jvar == _T_var)
 // {
  //  _D[_qp]*_Qh* (_kb[_qp/_T[_qp]*_T[_qp])*(2*_grad_T[_qp]*_phi[_j][_qp]/ _T[_qp] -
  //         _grad_phi[_j][_qp])*_test[_i][_qp];
  //}

 // return 0.0;
//}
