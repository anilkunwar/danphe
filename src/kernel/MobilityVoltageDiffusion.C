/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "MobilityVoltageDiffusion.h"
#include "MooseVariable.h" 
// As the Kernel uses "_var.number()),", MooseVariable.h should be included to prevent the following error
//error: invalid use of incomplete  type "class MooseVariable"
//https://groups.google.com/forum/#!msg/moose-users/alUZJJQkdI0/cbCp2JvsBwAJ
//https://groups.google.com/forum/?utm_medium=email&utm_source=footer#!msg/moose-users/Zy_Ul5dfzcQ/wIT3R4LsBwAJ
template <>
InputParameters
validParams<MobilityVoltageDiffusion>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Add Electromigration Effect to Split formulation Cahn-Hilliard Kernel");
  params.addRequiredCoupledVar("volt", "Voltage"); //phi = volt
  params.addCoupledVar("c", "Concentration");
  // Add a required parameter.  If this isn't provided in the input file MOOSE will error.
  params.addRequiredParam<Real>("porefactor","The porefactor depending upon the shape and thermal conductivity ratios"); //F.A. Nichols date 1979
    // Add a parameter with a default value.  This value can be overriden in the input file.
  //params.addParam<Real>("porefactor",1.0, "The porefactor depending upon the shape and thermal conductivity ratios");
params.addRequiredParam<Real>("z", "Effective charge number for void in Sn");
  // Add a parameter with a default value.  This value can be overriden in the input file.
  params.addParam<Real>("kb", 1.38e-23, "The Boltzmann constant in J/K"); // Write it in eV/K
  params.addParam<Real>("e", 1.6e-19, "electronic charge in coulomb");
  params.addParam<Real>("rho", 5.5e-7, "electric resistivity of liquid tin in ohm m"); // value along the direction denoted by angle theta with c-axis of tin crystal
  params.addRequiredParam<Real>("T_c","Temperature of the medium");
  params.addRequiredParam<MaterialPropertyName>("mobility_name","The mobility used with the kernel");
  //params.addParam<MaterialPropertyName>("Q_name", "Qheat", "The material name for the heat of transport");
  return params;
}

MobilityVoltageDiffusion::MobilityVoltageDiffusion(const InputParameters & parameters)
  : Kernel(parameters),
    _volt_var(coupled("volt")),
    _volt(coupledValue("volt")),
    _grad_volt(coupledGradient("volt")),
    _is_coupled(isCoupled("c")),
    _c_var(_is_coupled ? coupled("c") : _var.number()),
    _c(_is_coupled ? coupledValue("c") : _u),
    _pf(getParam<Real>("porefactor")),
    _z(getParam<Real>("z")),
    _kb(getParam<Real>("kb")),
    _e(getParam<Real>("e")),
    _rho(getParam<Real>("rho")),
    _T_c(getParam<Real>("T_c")),
    _M(getMaterialProperty<Real>("mobility_name")),
    //_Q(getMaterialProperty<Real>("Q_name")),
    _kB(8.617343e-5) // Boltzmann constant in eV/K
{
}
// The MobilityVoltageDiffusion kernel uses R = McQ*(gradT)/T instead of R = DcQ*(gradT)/(k_b*T^2)
Real
MobilityVoltageDiffusion::computeQpResidual()
{
  const Real volt_term = _M[_qp] *_pf* _z*_e*_rho * _c[_qp] / (_kb*_T_c);
  return volt_term * _grad_volt[_qp] * _grad_test[_i][_qp];
}

Real
MobilityVoltageDiffusion::computeQpJacobian()
{
  return _is_coupled ? 0.0 : computeQpCJacobian();
}

Real
MobilityVoltageDiffusion::computeQpOffDiagJacobian(unsigned int jvar)
{
  // c Off-Diagonal Jacobian
  if (_c_var == jvar)
    return computeQpCJacobian();

  // T Off-Diagonal Jacobian
  if (_volt_var == jvar)
    return _M[_qp] *_pf* _z*_e*_rho * _c[_qp] * _grad_test[_i][_qp] *
           (_grad_phi[_j][_qp]) / (_kb*_T_c);

  return 0.0;
}

Real
MobilityVoltageDiffusion::computeQpCJacobian()
{
  // Calculate the Jacobian for the c variable
  return _M[_qp] *_pf* _z*_e*_rho * _phi[_j][_qp] * _grad_volt[_qp] / (_kb*_T_c) *
         _grad_test[_i][_qp];
}
