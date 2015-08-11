/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment */
/* */
/* All contents are licensed under LGPL V2.1 */
/* See LICENSE for full restrictions */
/****************************************************************/

//This kernel adds electromigration flux in the Cahn-Hilliard Equation
//Mathematically it is J_em=c*(D/kT)*z*eE
#include "SplitCHVoltage.h"
template<>
InputParameters validParams<SplitCHVoltage>()
{
InputParameters params = validParams<Kernel>();
params.addClassDescription("Add Electromigration Flux to Split formulation Cahn-Hilliard Kernel");
//params.addRequiredCoupledVar("T", "Temperature in Kelvin Scale");
params.addRequiredCoupledVar("volt", "Voltage");
params.addRequiredCoupledVar("c", "Concentration");
params.addRequiredParam<MaterialPropertyName>("diff_name", "The diffusivity used with the kernel");
params.addRequiredParam<MaterialPropertyName>("T", "The temperature used with the kernel");
params.addParam<MaterialPropertyName>("z_name", "zeff", "Effective charge number of the species");
//params.addParam<MaterialPropertyName>("e_name", "eo", "Charge of the electron");
return params;
}
SplitCHVoltage::SplitCHVoltage(const InputParameters & parameters) :
Kernel(parameters),
//_T_var(coupled("T")),
//_T(coupledValue("T")),
//_grad_T(coupledGradient("T")),
_volt_var(coupled("volt")),
_volt(coupledValue("volt")),
_grad_volt(coupledGradient("volt")),
_c_var(coupled("c")),
_c(coupledValue("c")),
_D(getMaterialProperty<Real>("diff_name")),
_T(getMaterialProperty<Real>("T")),
//_Q(getMaterialProperty<Real>("Q_name")),
_z(getMaterialProperty<Real>("z_name")),
//_T(getParam<Real>("T")),
_kb(8.617343e-5), // Boltzmann constant in eV/K
_eo(1.6e-19) // charge of an electron
{
}
Real
SplitCHVoltage::computeQpResidual()
{
Real volt_term = _D[_qp] * _z[_qp] *_eo * _c[_qp] / (_kb * _T[_qp] );
return volt_term * _grad_volt[_qp] * _grad_test[_i][_qp];
}

Real
SplitCHVoltage::computeQpJacobian()
{
  if (_c_var == _var.number()) //Requires c jacobian
    return computeQpCJacobian();

  return 0.0;
}

Real
SplitCHVoltage::computeQpOffDiagJacobian(unsigned int jvar)
{
if (_c_var == jvar)
return _D[_qp] * _z[_qp] * _eo * _phi[_j][_qp] * _grad_volt[_qp] / (_kb *  _T[_qp]) * _grad_test[_i][_qp];
else if (_volt_var == jvar)
return _D[_qp] * _z[_qp] * _eo * _c[_qp] * _grad_test[_i][_qp] *
(_grad_phi[_j][_qp]/(_kb * _T[_qp]) + _grad_volt[_qp] * _phi[_j][_qp] / (_kb * _T[_qp] * _T[_qp]));
return 0.0;
}

Real
SplitCHVoltage::computeQpCJacobian()
{
  //Calculate the Jacobian for the c variable
  return _D[_qp] * _z[_qp] * _phi[_j][_qp] * _grad_volt[_qp] / (_kb * _T[_qp]) * _grad_test[_i][_qp];
}
