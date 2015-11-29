/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/


#ifndef CURRENTDENSITYMATERIAL_H
#define CURRENTDENSITYMATERIAL_H

#include "Material.h"

//Forward Declarations
class CurrentDensityMaterial;

template<>
InputParameters validParams<CurrentDensityMaterial>();

/**
 * Defines the permeability tensor used in Darcy flow
 */
class CurrentDensityMaterial : public Material
{
public:
  CurrentDensityMaterial(const InputParameters & parameters);

protected:

  virtual void computeQpProperties();

  /// current density tensor as entered by the user
  RealTensorValue _j_mater_vol;

  /// the Material property that this Material provides
  MaterialProperty<RealTensorValue> & _current_density;

};

#endif //CURRENTDENSITYMATERIAL_H
