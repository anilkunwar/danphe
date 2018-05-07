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

#ifndef INTERFACIALANGLENEUMANNBC_H
#define INTERFACIALANGLENEUMANNBC_H

#include "IntegratedBC.h"

//Forward Declarations
class InterfacialAngleNeumannBC;

template<>
InputParameters validParams<InterfacialAngleNeumannBC>();

/**
 * Boundary condition of a Neumann style whose value is computed by a user-defined function
 */
class InterfacialAngleNeumannBC : public IntegratedBC
{
public:
  InterfacialAngleNeumannBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();

  /// The wetting angle used for the Neumann wall potential condition
  //const Real & _value;
private:
// The cosine of the wetting angle for the Neumann wall potential condition
Real  _trigonometricvalue;
};

#endif // INTERFACIALANGLENEUMANNBC_H
