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
  #params.addParam<MaterialPropertyName>("mob_name", "L", "The mobility used with the kernel");
  params.addParam<MaterialPropertyName>("net_thermotransport", "Mq","The mobility of thermotransport");
  return params;
}

MultiSoretDiffusion::MultiSoretDiffusion(const InputParameters & parameters) :
    Kernel(parameters),
    _T_var(coupled("T")),
    _T(coupledValue("T")),
    _grad_T(coupledGradient("T")),
    _c_var(coupled("c")),
    _Mq(getMaterialProperty<Real>("net_thermotransport")),
    _kb(8.617343e-5) // Boltzmann constant in eV/K
{
}

Real
MultiSoretDiffusion::computeQpResidual()
{
  Real T_term = _Mq[_qp]*(- _c[_qp]*_c[_qp] + _c[-qp]) / ( _T[_qp] );
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
    //return _D[_qp] * _Q[_qp] * _c[_qp] * _grad_test[_i][_qp] *
     //      (_grad_phi[_j][_qp]/(_kb * _T[_qp] * _T[_qp]) - 2.0 * _grad_T[_qp] * _phi[_j][_qp] / (_kb * _T[_qp] * _T[_qp] * _T[_qp]));

  return 0.0;
}

Real
MultiSoretDiffusion::computeQpCJacobian()
{
  //Calculate the Jacobian for the c variable
  //return _D[_qp] * _Q[_qp] * _phi[_j][_qp] * _grad_T[_qp] / (_kb * _T[_qp] * _T[_qp]) * _grad_test[_i][_qp];
  return _Mq[_qp]*_grad_test[_i][_qp]*(_grad_T[_qp]/_T[_qp]- 2.0 *_phi[_j][_qp]*_grad_T[_qp]/_T[_qp])
}

