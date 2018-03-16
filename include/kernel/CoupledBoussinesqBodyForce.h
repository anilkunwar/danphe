//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

//Modified from INSBase.h  and INSMomentumBase.h  kernels

#ifndef COUPLEDBOUSSINESQBODYFORCE_H
#define COUPLEDBOUSSINESQBODYFORCE_H

#include "Kernel.h"

// Forward Declarations
class CoupledBoussinesqBodyForce;
class Function;

template <>
InputParameters validParams<CoupledBoussinesqBodyForce>();

class CoupledBoussinesqBodyForce : public Kernel
{
public:
  CoupledBoussinesqBodyForce(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

 // components of velocity as 0,1 or 2
  unsigned _component;
// Coupled variables
  const VariableValue & _u_vel;
  const VariableValue & _v_vel;
  const VariableValue & _w_vel;
  const VariableValue & _p;

 // Gradients
  const VariableGradient & _grad_u_vel;
  const VariableGradient & _grad_v_vel;
  const VariableGradient & _grad_w_vel;
  const VariableGradient & _grad_p;

 // Variable numberings
  unsigned _u_vel_var_number;
  unsigned _v_vel_var_number;
  unsigned _w_vel_var_number;
  unsigned _p_var_number;

  RealVectorValue _gravity;


  // Parameters
  //const Real _acceleration;
  // Materials Properties
  //const MaterialProperty<Real> & _mu;
  const MaterialProperty<Real> & _rho_bq;
  const MaterialProperty<Real> & _mask;
};

#endif // COUPLEDBOUSSINESQBODYFORCE_H
