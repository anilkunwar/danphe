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

#ifndef PRECIPITATIONBCS_H
#define PRECIPITATIONBCS_H

#include "IntegratedBC.h"

//Forward Declarations
class PrecipitationBCS;
class Function;

template<>
InputParameters validParams<PrecipitationBCS>();

/**
 * Implements a simple constant Neumann BC where grad(u)=alpha * v on the boundary.
 * Uses the term produced from integrating the diffusion operator by parts.
 */
class PrecipitationBCS : public IntegratedBC
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  PrecipitationBCS(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  /// The function being used for setting the value
  Function & _func;

  virtual Real computeQpJacobian();

private:
  /**
   * Multiplier on the boundary.
   */
  // Real _alpha;
  Real _beta;

  /**
   * Holds the values at the quadrature points
   * of a coupled variable.
   */
  //VariableValue & _some_var_val;
};

#endif //PrecipitationBCS_H
