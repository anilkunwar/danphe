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

#ifndef COUPLEDPOTENTIAL_H
#define COUPLEDPOTENTIAL_H

#include "Kernel.h"

// Forward Declaration
class CoupledPotential;

template<>
InputParameters validParams<CoupledPotential>();

/**
 * Simple class to demonstrate off diagonal Jacobian contributions.
 */
class CoupledPotential : public Kernel
{
public:
  CoupledPotential(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  /// The gradient of potential
  VariableGradient & _potential_gradient;

  /// Coupling identifier for the potential.  This is used to uniquely identify a coupled variable
  unsigned int _potential_var;

  //Getting the material property
  const Param<Real> & _conductivity;
  //const MaterialProperty<Real> & _conductivity;
  const MaterialProperty<Real> & _mob;
};

#endif //COUPLEDPOTENTIAL_H
