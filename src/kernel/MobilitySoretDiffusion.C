/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "MobilitySoretDiffusion.h"
#include "MooseVariable.h"
template <>
InputParameters
validParams<MobilitySoretDiffusion>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Add Soret effect to Split formulation Cahn-Hilliard Kernel");
  params.addRequiredCoupledVar("T", "Temperature");
  params.addCoupledVar("c", "Concentration");
  params.addRequiredParam<MaterialPropertyName>("mobility_name",
                                                "The mobility used with the kernel");
  params.addParam<MaterialPropertyName>(
      "Q_name", "Qheat", "The material name for the heat of transport");
  return params;
}

MobilitySoretDiffusion::MobilitySoretDiffusion(const InputParameters & parameters)
  : Kernel(parameters),
    _T_var(coupled("T")),
    _T(coupledValue("T")),
    _grad_T(coupledGradient("T")),
    _is_coupled(isCoupled("c")),
    _c_var(_is_coupled ? coupled("c") : _var.number()),
    _c(_is_coupled ? coupledValue("c") : _u),
    _M(getMaterialProperty<Real>("mobility_name")),
    _Q(getMaterialProperty<Real>("Q_name")),
    _kB(8.617343e-5) // Boltzmann constant in eV/K
{
}
// The MobilitySoretDiffusion kernel uses R = McQ*(gradT)/T instead of R = DcQ*(gradT)/(k_b*T^2)
Real
MobilitySoretDiffusion::computeQpResidual()
{
  const Real T_term = _M[_qp] * _Q[_qp] * _c[_qp] / ( _T[_qp]);
  return T_term * _grad_T[_qp] * _grad_test[_i][_qp];
}

Real
MobilitySoretDiffusion::computeQpJacobian()
{
  return _is_coupled ? 0.0 : computeQpCJacobian();
}

Real
MobilitySoretDiffusion::computeQpOffDiagJacobian(unsigned int jvar)
{
  // c Off-Diagonal Jacobian
  if (_c_var == jvar)
    return computeQpCJacobian();

  // T Off-Diagonal Jacobian
  if (_T_var == jvar)
    return _M[_qp] * _Q[_qp] * _c[_qp] * _grad_test[_i][_qp] *
           (_grad_phi[_j][_qp] - _grad_T[_qp] * _phi[_j][_qp] / _T[_qp]) /
           (_T[_qp]);

  return 0.0;
}

Real
MobilitySoretDiffusion::computeQpCJacobian()
{
  // Calculate the Jacobian for the c variable
  return _M[_qp] * _Q[_qp] * _phi[_j][_qp] * _grad_T[_qp] / (_T[_qp]) *
         _grad_test[_i][_qp];
}
