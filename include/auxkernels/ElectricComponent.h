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

#ifndef ELECTRICCOMPONENT_H
#define ELECTRICCOMPONENT_H

#include "AuxKernel.h"
//#include "MaterialRealVectorValueAux.h"

//Forward Declarations
class ElectricComponent;

template<>
InputParameters validParams<ElectricComponent>();

/**
 * Constant auxiliary value
 */
class ElectricComponent : public AuxKernel
{
public:
  ElectricComponent(const InputParameters & parameters);

  virtual ~ElectricComponent() {}

protected:
  /**
   * AuxKernels MUST override computeValue.  computeValue() is called on
   * every quadrature point.  For Nodal Auxiliary variables those quadrature
   * points coincide with the nodes.
   */
  virtual Real computeValue();
 
  /// Will hold 0,1,2 for x,y,z
  int _component;

  /// Diffusivity material property
  const MaterialProperty<Real> & _D;

  /// current density material property
  const MaterialProperty<RealVectorValue> & _current_density;

  /// Will be set from the input file
  Real _z;
  Real _kb;
  Real _e;
  Real _rho;
  Real _T_c;

};

#endif //ELECTRICCOMPONENT_H
