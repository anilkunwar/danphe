//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef BOUSSINESQBODYFORCE_H
#define BOUSSINESQBODYFORCE_H

#include "NSKernel.h"

// Forward Declarations
class BoussinesqBodyForce;

template <>
InputParameters validParams<BoussinesqBodyForce>();

class BoussinesqBodyForce : public NSKernel
{
public:
  BoussinesqBodyForce(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  // Parameters
  const Real _acceleration;
  const MaterialProperty<Real> & _rho_bq;
};

#endif // BOUSSINESQBODYFORCE_H
