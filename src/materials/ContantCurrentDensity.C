/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
//#include "Constant_Anisotropic_Mobility.h" (reference)
#include "ConstantCurrentDensity.h"

template<>
InputParameters validParams<ConstantCurrentDensity>()
{
  InputParameters params = validParams<Material>();
  params.addClassDescription("Provide a constant current density tensor value");
  params.addRequiredParam<MaterialPropertyName>("J_name", "Name of j tensor porperty to generate");
  params.addRequiredRangeCheckedParam<std::vector<Real> >("tensor", "tensor_size=9", "Tensor values");
  return params;
}

ConstantCurrentDensity::ConstantCurrentDensity(const InputParameters & parameters) :
    Material(parameters),
    _J_values(getParam<std::vector<Real> >("tensor")),
    _J_name(getParam<MaterialPropertyName>("J_name")),
    _J(declareProperty<RealTensorValue>(_J_name))
{
}

void
ConstantCurrentDensity::initialSetup()
{
  _J.resize(_fe_problem.getMaxQps());
  for (unsigned int qp = 0; qp < _J.size(); ++qp)
    for (unsigned int a = 0; a < LIBMESH_DIM; ++a)
      for (unsigned int b = 0; b < LIBMESH_DIM; ++b)
        _J[qp](a,b) = _J_values[a*3 + b];
}
