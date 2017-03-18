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
#ifndef OCULARMATERIALPROPERTIES_H
#define OCULARMATERIALPROPERTIES_H

#include "Material.h"

// A helper class from MOOSE that linear interpolates x,y data
//#include "LinearInterpolation.h"

class OcularMaterialProperties;

template<>
InputParameters validParams<OcularMaterialProperties>();

/**
 * Material objects inherit from Material and override computeQpProperties.
 *
 * Their job is to declare properties for use by other objects in the
 * calculation such as Kernels and BoundaryConditions.
 */
class OcularMaterialProperties : public Material
{
public:
  OcularMaterialProperties(const InputParameters & parameters);

protected:
  /**
   * Necessary override.  This is where the values of the properties
   * are computed.
   */
  virtual void computeQpProperties() override;
     /// The permeability (K)
   MaterialProperty<Real> & _permeability;

  /// The porosit 9epsilon)
   MaterialProperty<Real> & _porosity;
  
  /// The viscosity of the fluid (mu)
   MaterialProperty<Real> & _viscosity;

  /// The density (rho)
   MaterialProperty<Real> & _density;

  /// The diffusivity (rho)
   MaterialProperty<Real> & _diffusivity;

  // Ionic Mobility OF solute
  //MaterialProperty<Real> & _M;

  /// The radius of the balls in the column
  // const Real & _ball_radius;
  const Real & _z;
  const Real & _F;
  const Real & _R;
  const Real & _T;


  
};

#endif //OCULARMATERIALPROPERTIES_H
