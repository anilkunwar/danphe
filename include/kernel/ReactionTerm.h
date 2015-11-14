
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

#ifndef REACTIONTERM_H
#define REACTIONTERM_H

#include "Kernel.h"

// Forward Declaration
class ReactionTerm;

template<>
InputParameters validParams<ReactionTerm>();

/**
 * Kernel which implements the convective term in the transient heat
 * conduction equation, and provides coupling with the Darcy pressure
 * equation.
 */
class ReactionTerm : public Kernel
{
public:
  ReactionTerm(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  /// int label for temperature variable
  unsigned int _T_var;

  /// Coupled variable for the temperature
  VariableValue & _T;

  /// Variable gradient for temperature
  VariableGradient & _grad_T;

  /// Diffusivity material property
  const MaterialProperty<Real> & _cs;

  /// Will be set from the input file
  //Real _Qh;
  Real _kc;
};

#endif //REACTIONTERM_H
