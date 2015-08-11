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

#include "ElectricPotential.h"


template<>
InputParameters validParams<ElectricPotential>()
{
  // Start with the parameters from our parent
  InputParameters params = validParams<Diffusion>();

  // Now add any extra parameters this class needs:

  // Add a required parameter.  If this isn't provided in the input file MOOSE will error.
  params.addRequiredParam<Real>("conductivity", "The conductivity (sigma) of the metal");

  // Add a parameter with a default value.  This value can be overriden in the input file.
  //params.addParam<Real>("viscosity", 7.98e-4, "The viscosity (mu) of the fluid.  Default is for 30 degrees C.");

  return params;
}


ElectricPotential::ElectricPotential(const InputParameters & parameters) :
    Diffusion(parameters),

    // Get the parameters from the input file
    _conductivity(getParam<Real>("conductivity"))
    //_viscosity(getParam<Real>("viscosity"))
{
}

ElectricPotential::~ElectricPotential()
{
}

Real
ElectricPotential::computeQpResidual()
{
  // sigma * grad_u * grad_phi[i]
  return (_conductivity) * Diffusion::computeQpResidual();
}

Real
ElectricPotential::computeQpJacobian()
{
  // sigma * grad_phi[j] * grad_phi[i]
  return (_conductivity) * Diffusion::computeQpJacobian();
}
