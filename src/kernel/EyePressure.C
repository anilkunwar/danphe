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

#include "EyePressure.h"


template<>
InputParameters validParams<EyePressure>()
{
  // Start with the parameters from our parent
  InputParameters params = validParams<Diffusion>();

  // No parameters are necessary here because we're going to get
  // permeability and viscosity from the Material
  // so we just return params...

  return params;
}


EyePressure::EyePressure(const InputParameters & parameters) :
    Diffusion(parameters),

    // Get the permeability and viscosity from the Material system
    // This returns a MaterialProperty<Real> reference that we store
    // in the class and then index into in computeQpResidual/Jacobian....
    _permeability(getMaterialProperty<Real>("permeability")),
    _viscosity(getMaterialProperty<Real>("viscosity"))
{
}

EyePressure::~EyePressure()
{
}

Real
EyePressure::computeQpResidual()
{
  // Use the MaterialProperty references we stored earlier
  return (_permeability[_qp]/_viscosity[_qp]) * Diffusion::computeQpResidual();
}

Real
EyePressure::computeQpJacobian()
{
  // Use the MaterialProperty references we stored earlier
  return (_permeability[_qp]/_viscosity[_qp]) * Diffusion::computeQpJacobian();
}
