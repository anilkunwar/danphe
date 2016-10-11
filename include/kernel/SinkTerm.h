/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef SINKTERM_H
#define SINKTERM_H

#include "BodyForce.h"

//Forward Declarations
class SinkTerm;

template<>
InputParameters validParams<SinkTerm>();

class SinkTerm : public BodyForce
{
public:
  SinkTerm(const InputParameters & parameters);
};

#endif  //SINKTERM_H

