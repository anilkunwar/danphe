
/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef BACKSTRESSCONVECTION_H
#define BACKSTRESSCONVECTION_H

#include "Kernel.h"

// Forward Declaration
class BackstressConvection;

template<>
InputParameters validParams<BackstressConvection>();

/**
 * Kernel which implements the convective term in the transient heat
 * conduction equation, and provides coupling with the Darcy pressure
 * equation.
 */
class BackstressConvection : public Kernel
{
public:
  BackstressConvection(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  /// int label for temperature variable
  unsigned int _stress_var;

  /// Coupled variable for the temperature
  VariableValue & _stress;

  /// Variable gradient for temperature
  VariableGradient & _grad_stress;

  /// Diffusivity material property
  const MaterialProperty<Real> & _D;

  /// Will be set from the input file
  Real _kb;
  Real _omega;
  Real _T_c;
  
};

#endif //BACKSTRESSCONVECTION_H
