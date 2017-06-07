/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef MOBILITYSORETDIFFUSION_H
#define MOBILITYSORETDIFFUSION_H

#include "Kernel.h"

// Forward Declaration
class MobilitySoretDiffusion;

template <>
InputParameters validParams<MobilitySoretDiffusion>();
/**
 * MobilitySoretDiffusion adds the soret effect in the split form of the Cahn-Hilliard
 * equation.
 */
class MobilitySoretDiffusion : public Kernel
{
public:
  MobilitySoretDiffusion(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  virtual Real computeQpCJacobian();

  /// int label for temperature variable
  unsigned int _T_var;

  /// Coupled variable for the temperature
  const VariableValue & _T;

  /// Variable gradient for temperature
  const VariableGradient & _grad_T;

  /// is the kernel used in a coupled form?
  const bool _is_coupled;

  /// int label for the Concentration
  unsigned int _c_var;

  /// Variable value for the concentration
  const VariableValue & _c;

  /// Mobility material property
  const MaterialProperty<Real> & _M;

  /// Heat of transport material property
  const MaterialProperty<Real> & _Q;

  /// Boltzmann constant
  const Real _kB;
};

#endif // MOBILITYSORETDIFFUSION_H
