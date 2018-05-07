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

#ifndef INTERFACIALNEUMANNBC_H
#define INTERFACIALNEUMANNBC_H

#include "IntegratedBC.h"

//Forward Declarations
class InterfacialNeumannBC;
class Function;

template<>
InputParameters validParams<WettingNeumannBC>();

/**
 * Boundary condition of a Neumann style whose value is computed by a user-defined function
 */
class InterfacialNeumannBC : public IntegratedBC
{
public:
  InterfacialNeumannBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();

  /// The function being used for setting the value
  Function & _func;
//private:
// constants 
//material-properties
};

#endif // INTERFACIALNEUMANNBC_H
