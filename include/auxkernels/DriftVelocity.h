/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef DRIFTVELOCITY_H
#define DRIFTVELOCITY_H

#include "AuxKernel.h"

//Forward Declarations
class DriftVelocity;

template<>
InputParameters validParams<DriftVelocity>();

/**
 * Velocity auxiliary value
 */
class DriftVelocity : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  DriftVelocity(const InputParameters & parameters);

  virtual ~DriftVelocity() {}

protected:
  virtual Real computeValue();

  //VariableValue & _current_density;
  //VariableValue & _momentum;

  /// Diffusivity material property
  const MaterialProperty<Real> & _D;

  /// current density material property
  // MaterialProperty<RealVectorValue> & _current_density;

  /// Will be set from the input file
  Real _j_x;
  Real _z;
  Real _kb;
  Real _e;
  Real _rho;
  Real _T_c;

};

#endif //DRIFTVELOCITY_H

