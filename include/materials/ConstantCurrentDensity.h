/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef CONSTANTCURRENTDENSITY_H
#define CONSTANTCURRENTDENSITY_H

#include "Material.h"

class ConstantCurrentDensity;

template<>
InputParameters validParams<ConstantCurrentDensity>();

/**
 * ConstantCurrentDensity provides a simple RealTensorValue type
 * MaterialProperty that can be used as a mobility in a phase field simulation.
 */
class ConstantCurrentDensity : public Material
{
public:
  ConstantCurrentDensity(const InputParameters & parameters);

protected:
  virtual void computeProperties() {};
  virtual void initialSetup();

  /// raw tensor values as passed in from the input file
  std::vector<Real> _J_values;

  /// Name of the mobility tensor material property
  MaterialPropertyName _J_name;
  MaterialProperty<RealTensorValue> & _J;
};

#endif //CONSTANTCURRENTDENSITY_H
