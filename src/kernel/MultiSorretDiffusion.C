/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "MultiSoretDiffusion.h"
template<>
InputParameters validParams<SoretDiffusion>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Add Soret effect to Split formulation Cahn-Hilliard Kernel");
  params.addRequiredCoupledVar("T", "Temperature");
  params.addRequiredCoupledVar("c", "Concentration");
  params.addRequiredParam<MaterialPropertyName>("diffname_1", "The diffusivity of Sn or Element 1 used with the kernel");
  params.addRequiredParam<MaterialPropertyName>("diffname_2", "The diffusivity of Cu or Element 2 used with the kernel");
  params.addParam<MaterialPropertyName>("Qname_1", "Qheat_1", "The material 1 name for the heat of transport");
  params.addParam<MaterialPropertyName>("Qname_2", "Qheat_2", "The material 2 name for the heat of transport");
  return params;
}

MultiSoretDiffusion::MultiSoretDiffusion(const InputParameters & parameters) :
    Kernel(parameters),
    _T_var(coupled("T")),
    _T(coupledValue("T")),
    _grad_T(coupledGradient("T")),
    _c_var(coupled("c")),
    _c(coupledValue("c")),
    _D1(getMaterialProperty<Real>("diffname_1")),
    _D2(getMaterialProperty<Real>("diffname_2")),
    _Q1(getMaterialProperty<Real>("Qname_1")),
    _Q2(getMaterialProperty<Real>("Qname_2")),
    _kb(8.617343e-5) // Boltzmann constant in eV/K
{
}

Real
MultiSoretDiffusion::computeQpResidual()
{
  # use the term to represent the differences of D1,D2 and Q1 and Q2 (or something like this)
  # should create the expression Mq*(1/T)*gradT
  # where Mq = rho *c (1-c)[ba Qa - bb Qb]
  # this code has -[D *Q *c /(k*T*T)]*gradT
  Real T_term = _D[_qp] * _Q[_qp] * _c[_qp] / (_kb * _T[_qp] * _T[_qp]);

  return T_term * _grad_T[_qp] * _grad_test[_i][_qp];
}

Real
MultiSoretDiffusion::computeQpJacobian()
{
  if (_c_var == _var.number()) //Requires c jacobian
    return computeQpCJacobian();

  return 0.0;
}

Real
MultiSoretDiffusion::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (_c_var == jvar) //Requires c jacobian
    return computeQpCJacobian();
  else if (_T_var == jvar) //Requires T jacobian
    return _D[_qp] * _Q[_qp] * _c[_qp] * _grad_test[_i][_qp] *
           (_grad_phi[_j][_qp]/(_kb * _T[_qp] * _T[_qp]) - 2.0 * _grad_T[_qp] * _phi[_j][_qp] / (_kb * _T[_qp] * _T[_qp] * _T[_qp]));

  return 0.0;
}

Real
MultiSoretDiffusion::computeQpCJacobian()
{
  //Calculate the Jacobian for the c variable
  return _D[_qp] * _Q[_qp] * _phi[_j][_qp] * _grad_T[_qp] / (_kb * _T[_qp] * _T[_qp]) * _grad_test[_i][_qp];
}

