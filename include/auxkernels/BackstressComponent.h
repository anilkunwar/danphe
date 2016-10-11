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

#ifndef BackstressComponent_H
#define BackstressComponent_H

#include "AuxKernel.h"

//Forward Declarations
class BackstressComponent;

template<>
InputParameters validParams<BackstressComponent>();

/**
 * Constant auxiliary value
 */
class BackstressComponent : public AuxKernel
{
public:
  BackstressComponent(const InputParameters & parameters);

  virtual ~BackstressComponent() {}

protected:
  /**
   * AuxKernels MUST override computeValue.  computeValue() is called on
   * every quadrature point.  For Nodal Auxiliary variables those quadrature
   * points coincide with the nodes.
   */
  virtual Real computeValue();

  /// Will hold 0,1,2 for x,y,z
  int _component;
 
  /// int label for stress variable
  unsigned int _stress_var;

 /// Coupled variable for the temperature
  const VariableValue & _stress;

  /// Variable gradient for temperature
  const VariableGradient & _grad_stress;

  /// Diffusivity material property
  const MaterialProperty<Real> & _D;

  /// Will be set from the input file
  Real _kb;
  Real _omega;
  Real _T_c;
};

#endif //BackstressComponent_H

