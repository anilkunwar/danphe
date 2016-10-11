/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "DriftVelocity.h"

template<>
InputParameters validParams<DriftVelocity>()
{
  InputParameters params = validParams<AuxKernel>();
  //params.addRequiredCoupledVar("j_x", "Current Density");
  //params.addRequiredCoupledVar("momentum", "Momentum (conserved form)");
  params.addRequiredParam<MaterialPropertyName>("D_name", "The diffusivity used with the kernel");
  params.addRequiredParam<Real>("j_x", "scalar x_component  of current density vector");
  params.addRequiredParam<Real>("z", "Effective charge number for Cu");
  // Add a parameter with a default value.  This value can be overriden in the input file.
  params.addParam<Real>("kb", 1.38e-23, "The Boltzmann constant in J/K");
  params.addParam<Real>("e", 1.6e-19, "electronic charge in coulomb");
  params.addParam<Real>("rho", 5.5e-7, "electric resistivity of liquid tin in ohm m");
  params.addParam<Real>("T_c", 623.0, "Temperature of the solder medium in K");
  return params;
  
}

DriftVelocity::DriftVelocity(const InputParameters & parameters) :
    AuxKernel(parameters),
    //_current_density(coupledValue("j_x")),
    //_momentum(coupledValue("momentum")),
    // Grab necessary material properties
    _D(getMaterialProperty<Real>("D_name")),
    //current_density(getParam<RealVectorValue>("current_density")),
    //_current_density(getMaterialProperty<RealVectorValue>("current_density")),
    _j_x(getParam<Real>("j_x")),
    _z(getParam<Real>("z")),
    _kb(getParam<Real>("kb")),
    _e(getParam<Real>("e")),
    _rho(getParam<Real>("rho")),
    _T_c(getParam<Real>("T_c"))
{
}

Real
DriftVelocity::computeValue()
{
  return _D[_qp]*_z *_e * _rho*_j_x/(_kb* _T_c); ;
}


