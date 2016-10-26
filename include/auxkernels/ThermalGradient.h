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

#ifndef THERMALGRADIENT_H
#define THERMALGRADIENT_H

#include "AuxKernel.h"

//Forward Declarations
class ThermalGradient;

template<>
InputParameters validParams<ThermalGradient>();

/**
 * Constant auxiliary value
 */
class ThermalGradient : public AuxKernel
{
public:
  ThermalGradient(const InputParameters & parameters);

  virtual ~ThermalGradient() {}

protected:
  /**
   * AuxKernels MUST override computeValue.  computeValue() is called on
   * every quadrature point.  For Nodal Auxiliary variables those quadrature
   * points coincide with the nodes.
   */
  virtual Real computeValue() override;

  /// Will hold 0,1,2 for x,y,z
  int _component;

  /// The gradient of a coupled variable
  const VariableGradient & _T_gradient;

  /// Holds the permeability and viscosity from the material system
  //const MaterialProperty<Real> & _permeability;
  //const MaterialProperty<Real> & _viscosity;
};

#endif //THERMALGRADIENT_H
