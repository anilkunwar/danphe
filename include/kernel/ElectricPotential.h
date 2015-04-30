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

#ifndef ELECTRICPOTENTIAL_H
#define ELECTRICPOTENTIAL_H

// Including the "Diffusion" Kernel here so we can extend it
#include "Diffusion.h"

class ElectricPotential;

template<>
InputParameters validParams<ElectricPotential>();

/**
 * Represents sigma * grad_u * grad_phi
 *
 * We are inheriting from Diffusion instead of from Kernel because
 * the grad_u * grad_phi is already coded in there and all we
 * need to do is specialize that calculation by multiplying by K/mu
 */
class ElectricPotential : public Diffusion
{
public:
  ElectricPotential(const std::string & name, InputParameters parameters);
  virtual ~ElectricPotential();

protected:
  /**
   * Kernels _must_ override computeQpResidual()
   */
  virtual Real computeQpResidual();

  /**
   * This is optional (but recommended!)
   */
  virtual Real computeQpJacobian();

  /// Will be set from the input file
  Real _conductivity;
  //Real _viscosity;
};


#endif /* ELECTRICPOTENTIAL_H */
