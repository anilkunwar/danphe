/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef SPLITCHVOLTAGE_H
#define SPLITCHVOLTAGE_H

#include "Kernel.h"

//Forward Declaration
class SplitCHVoltage;

template<>
InputParameters validParams<SplitCHVoltage>();
/**
 * SplitCHVoltage adds the soret effect in the split form of the Cahn-Hilliard
 * equation.
 */
class SplitCHVoltage : public Kernel
{
public:
  SplitCHVoltage(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  virtual Real computeQpCJacobian();

  /// int label for Voltage variable
  unsigned int _volt_var;

  /// Coupled variable for the Voltage
  VariableValue & _volt;

  /// Variable gradient for Voltage
  VariableGradient & _grad_volt;

  /// int label for the Concentration
  unsigned int _c_var;

  /// Variable value for the concentration
  VariableValue & _c;

  /// Diffusivity material property
  const MaterialProperty<Real> & _D;

  /// Temperature material property
  const MaterialProperty<Real> & _T;


  /// Heat of transport material property
  /// Effective charge number of the species
  const MaterialProperty<Real> & _z;

  /// Boltzmann constant
  const Real _kb;

  /// Charge of an electron
  const Real _eo;

  /// Temperature in kelvin scale
  //const Real _T;
};

#endif //SPLITCHVOLTAGE_H
