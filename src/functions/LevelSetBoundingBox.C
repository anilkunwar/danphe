//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

// MOOSE includes
#include "LevelSetBoundingBox.h"

template <>
InputParameters
validParams<LevelSetBoundingBox>()
{
  InputParameters params = validParams<Function>();
  params.addClassDescription("Implementation of 'Box' ranging from 0 to 1.");
  params.addParam<RealVectorValue>(
      "bottomleftcorner", RealVectorValue(0.0, 0.0, 0.0), "The bottom corner of the box.");
  params.addParam<RealVectorValue>(
      "toprightcorner", RealVectorValue(0.05, 0.05, 0.0), "The top corner of the box.");
  params.addParam<Real>("thickness", 0.15, "The initial length of the electroplated material.");
  params.addParam<Real>("epsilon", 0.01, "The interface thickness.");
  return params;
}

LevelSetBoundingBox::LevelSetBoundingBox(const InputParameters & parameters)
  : Function(parameters),
    _bottom(getParam<RealVectorValue>("bottomleftcorner")),
    _top(getParam<RealVectorValue>("toprightcorner")),
    _width(getParam<Real>("thickness")),
    _epsilon(getParam<Real>("epsilon"))
{
}

Real
LevelSetBoundingBox::value(Real /*t*/, const Point & p)
{
  //const Real x = ((p - _bottom).size() - _width) / _epsilon; //p(0) represent x-coordinate of p vector
  const Real x = ((p(0) - _bottom(0)) - _width) / _epsilon;
  return 1.0 / (1 + std::exp(x));
}

RealGradient
LevelSetBoundingBox::gradient(Real /*t*/, const Point & p)
{
  //Real norm = (p - _bottom).size();
  Real norm = (p(0) - _bottom(0));
  Real g = (norm - _width) / _epsilon;
  RealGradient output;

  Real g_prime;
  for (unsigned int i = 0; i < LIBMESH_DIM; ++i)
  {
    g_prime = (p(i) - _bottom(i)) / (_epsilon * norm);
    output(i) = (g_prime * std::exp(g)) / ((std::exp(g) + 1) * (std::exp(g) + 1));
  }
  return output;
}
