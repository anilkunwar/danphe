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

#ifndef WETTINGANGLENEUMANNBC_H
#define WETTINGANGLENEUMANNBC_H

#include "IntegratedBC.h"

//Forward Declarations
class WettingAngleNeumannBC;

template<>
InputParameters validParams<WettingAngleNeumannBC>();

/**
 * Boundary condition of a Neumann style whose value is computed by a user-defined function
 */
class WettingAngleNeumannBC : public IntegratedBC
{
public:
  WettingAngleNeumannBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();

  /// The wetting angle used for the Neumann wall potential condition
  const Real & _value;
};

#endif // WETTINGANGLENEUMANNBC_H
