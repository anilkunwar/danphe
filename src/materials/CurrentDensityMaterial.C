/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/


#include "CurrentDensityMaterial.h"



template<>
InputParameters validParams<CurrentDensityMaterial>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredParam<RealTensorValue>("j_tensor", "The material current_density tensor (in A/m^2)");
  params.addClassDescription("Material that holds the current density tensor");
  return params;
}

CurrentDensityMaterial::CurrentDensityMaterial(const InputParameters & parameters) :
    Material(parameters),
    _j_mater_vol(getParam<RealTensorValue>("j_tensor")),
    _current_density(declareProperty<RealTensorValue>("current_density"))
{
}


void
CurrentDensityMaterial::computeQpProperties()
{
  _current_density[_qp] = _j_mater_vol;
}
