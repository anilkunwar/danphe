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

#ifndef WETTINGNEUMANNBC_H
#define WETTINGNEUMANNBC_H

#include "IntegratedBC.h"

//Forward Declarations
class WettingNeumannBC;
class Function;

template<>
InputParameters validParams<WettingNeumannBC>();

/**
 * Boundary condition of a Neumann style whose value is computed by a user-defined function
 */
class WettingNeumannBC : public IntegratedBC
{
public:
  WettingNeumannBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();

  /// The function being used for setting the value
  Function & _func;
};

#endif // WETTINGNEUMANNBC_H
