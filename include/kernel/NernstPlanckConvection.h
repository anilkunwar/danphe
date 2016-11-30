
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

#ifndef NERNSTPLANCKCONVECTION_H
#define NERNSTPLANCKCONVECTION_H

#include "Kernel.h"

// Forward Declaration
class NernstPlanckConvection;

template<>
InputParameters validParams<NernstPlanckConvection>();

/**
 * Kernel which implements the convective term in the transient heat
 * conduction equation, and provides coupling with the Darcy pressure
 * equation.
 */
class NernstPlanckConvection : public Kernel
{
public:
  NernstPlanckConvection(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  /// int label for temperature variable
  unsigned int _V_var;

  /// Coupled variable for the temperature
  const VariableValue & _V;

  /// Variable gradient for temperature
  const VariableGradient & _grad_V;

  /// Diffusivity material property
  const MaterialProperty<Real> & _M;

  /// Will be set from the input file
  //Real _Qh;
  //Real _kb;
};

#endif //NERNSTPLANCKCONVECTION_H
