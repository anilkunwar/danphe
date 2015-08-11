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

#ifndef CURRENTDENSITY_H
#define CURRENTDENSITY_H

#include "AuxKernel.h"

//Forward Declarations
class CurrentDensity;

template<>
InputParameters validParams<CurrentDensity>();

/**
 * Constant auxiliary value
 */
class CurrentDensity : public AuxKernel
{
public:
  CurrentDensity(const InputParameters & parameters);

  virtual ~CurrentDensity() {}

protected:
  /**
   * AuxKernels MUST override computeValue.  computeValue() is called on
   * every quadrature point.  For Nodal Auxiliary variables those quadrature
   * points coincide with the nodes.
   */
  virtual Real computeValue();

  /// Will hold 0,1,2 for x,y,z
  int _component;

  /// The gradient of a coupled variable
  VariableGradient & _potential_gradient;

  /// Holds the conductivity and viscosity from the material system
  const MaterialProperty<Real> & _conductivity;
  //const MaterialProperty<Real> & _viscosity;
};

#endif //CURRENTDENSITY_H
