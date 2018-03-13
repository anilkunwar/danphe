//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef LEVELSETBOUNDINGBOX_H
#define LEVELSETBOUNDINGBOX_H

// MOOSE includes
#include "Function.h"

class LevelSetBoundingBox;

template <>
InputParameters validParams<LevelSetBoundingBox>();

/**
 * Modified code of LevelSetOlssonBubble that Implements the "bubble" function from Olsson and Kreiss (2005).
 */
class LevelSetBoundingBox : public Function
{
public:
  LevelSetBoundingBox(const InputParameters & parameters);

  virtual Real value(Real /*t*/, const Point & p) override;

  virtual RealGradient gradient(Real /*t*/, const Point & p) override;

protected:
  /// The 'bottomLeft' and 'topRight' of the box
  //const RealVectorValue & _center;
  const RealVectorValue & _bottom;
  const RealVectorValue & _top;

  /// The thickness or dx(width) of the bubble
  //const Real & _radius;
  const Real & _width;

  /// The interface thickness
  const Real & _epsilon;
};

#endif // LEVELSETBOUNDINGBOX_H
