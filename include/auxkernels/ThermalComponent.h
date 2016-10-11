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

#ifndef THERMALCOMPONENT_H
#define THERMALCOMPONENT_H

#include "AuxKernel.h"

//Forward Declarations
class ThermalComponent;

template<>
InputParameters validParams<ThermalComponent>();

/**
 * Constant auxiliary value
 */
class ThermalComponent : public AuxKernel
{
public:
  ThermalComponent(const InputParameters & parameters);

  virtual ~ThermalComponent() {}

protected:
  /**
   * AuxKernels MUST override computeValue.  computeValue() is called on
   * every quadrature point.  For Nodal Auxiliary variables those quadrature
   * points coincide with the nodes.
   */
  virtual Real computeValue();

  /// Will hold 0,1,2 for x,y,z
  int _component;
 
  /// int label for temperature variable
  unsigned int _T_var;

 /// Coupled variable for the temperature
  const VariableValue & _T;

  /// Variable gradient for temperature
  const VariableGradient & _grad_T;

  /// Diffusivity material property
  const MaterialProperty<Real> & _D;

  /// Will be set from the input file
  Real _Qh;
  Real _kb;
};

#endif //THERMALCOMPONENT_H
//
