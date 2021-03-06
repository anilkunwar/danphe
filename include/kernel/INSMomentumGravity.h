/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef INSMOMENTUMGRAVITY_H
#define INSMOMENTUMGRAVITY_H

#include "Kernel.h"

// Forward Declarations
class INSMomentumGravity;

template<>
InputParameters validParams<INSMomentumGravity>();

/**
 * This class computes momentum equation residual and Jacobian
 * contributions for the incompressible Navier-Stokes momentum
 * equation.
 */
class INSMomentumGravity : public Kernel
{
public:
  INSMomentumGravity(const InputParameters & parameters);

  virtual ~INSMomentumGravity(){}

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // Coupled variables
  const VariableValue& _u_vel;
  const VariableValue& _v_vel;
  const VariableValue& _w_vel;
  const VariableValue& _p;

  // Gradients
  const VariableGradient& _grad_u_vel;
  const VariableGradient& _grad_v_vel;
  const VariableGradient& _grad_w_vel;

  // Variable numberings
  unsigned _u_vel_var_number;
  unsigned _v_vel_var_number;
  unsigned _w_vel_var_number;
  unsigned _p_var_number;

  // Material properties
  // MaterialProperty<Real> & _dynamic_viscosity;
  Real _mu;
  Real _rho;
  RealVectorValue _gravity;

  // Parameters
  unsigned _component;
};


#endif // INSMOMENTUMGRAVITY_H
