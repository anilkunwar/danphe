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

#include "CurrentDensity.h"

template<>
InputParameters validParams<CurrentDensity>()
{
  InputParameters params = validParams<AuxKernel>();

  // Add a required parameter.  If this isn't provided in the input file MOOSE will error.
  //params.addRequiredParam<Real>("conductivity", "The conductivity (sigma) of the metal");

  // Declare the options for a MooseEnum.
  // These options will be presented to the user in Peacock
  // and if something other than these options is in the input file
  // an error will be printed
  MooseEnum component("x y z");

  // Use the MooseEnum to add a parameter called "component"
  params.addRequiredParam<MooseEnum>("component", component, "The desired component of current density.");

  // Add a "coupling paramater" to get a variable from the input file.
  params.addRequiredCoupledVar("electric_potential", "The potential field.");

  return params;
}

CurrentDensity::CurrentDensity(const std::string & name, InputParameters parameters) :
    AuxKernel(name, parameters),

    // This will automatically convert the MooseEnum to an integer
    _component(getParam<MooseEnum>("component")),

    // Get the gradient of the variable
    _potential_gradient(coupledGradient("electric_potential")),

    // Snag conductivity from the Material system.
    // Only AuxKernels operating on Elemental Auxiliary Variables can do this
    _conductivity(getMaterialProperty<Real>("conductivity"))

    // Snag viscosity from the Material system.
    // Only AuxKernels operating on Elemental Auxiliary Variables can do this
    //_viscosity(getMaterialProperty<Real>("viscosity"))
{
}

Real
CurrentDensity::computeValue()
{
  // Access the gradient of the potential at this quadrature point
  // Then pull out the "component" of it we are looking for (x, y or z)
  // Note that getting a particular component of a gradient is done using the
  // parenthesis operator
  return -_conductivity[_qp]*_potential_gradient[_qp](_component);
}
