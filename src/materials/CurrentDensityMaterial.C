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
  params.addRequiredParam<RealVectorValue>("j_vector", "The material current_density tensor (in A/m^2). E.g. '-1000 0 0'");
  params.addClassDescription("Material that holds the current density vector and computes the drift velocity");
  return params;
}

CurrentDensityMaterial::CurrentDensityMaterial(const InputParameters & parameters) :
    Material(parameters),
    _j_mater_vol(getParam<RealVectorValue>("j_vector")),
    _current_density(declareProperty<RealVectorValue>("current_density"))
       
{
}


void
CurrentDensityMaterial::computeQpProperties()
{
  _current_density[_qp] = _j_mater_vol;
}
