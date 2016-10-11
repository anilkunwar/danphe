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

#include "BackstressComponent.h"

template<>
InputParameters validParams<BackstressComponent>()
{
  InputParameters params = validParams<AuxKernel>();

  // Declare the options for a MooseEnum.
  // These options will be presented to the user in Peacock
  // and if something other than these options is in the input file
  // an error will be printed
  MooseEnum component("x y z");

  // Use the MooseEnum to add a parameter called "component"
  params.addRequiredParam<MooseEnum>("component", component, "The desired component of velocity.");

  // Add a "coupling paramater" to get a variable from the input file.
  params.addRequiredCoupledVar("hydrostatic", "The variable representing the stress sigma.");
  // Add a required parameter.  If this isn't provided in the input file MOOSE will error.
  params.addRequiredParam<MaterialPropertyName>("D_name", "The diffusivity used with the kernel");
  

  // Add a parameter with a default value.  This value can be overriden in the input file.
  params.addParam<Real>("kb", 1.38e-23, "The Boltzmann constant in J/K");
  params.addParam<Real>("omega", 2.71e-23, "Atomic volume of tin in m^3");
  params.addParam<Real>("T_c", 523.0, "Temperature of the solder medium in K");


  return params;
}

BackstressComponent::BackstressComponent(const InputParameters & parameters) :
    AuxKernel(parameters),

    // This will automatically convert the MooseEnum to an integer
    _component(getParam<MooseEnum>("component")),
    // Save off the coupled variable identifier for use in
    // computeQpOffDiagJacobian
    _stress_var(coupled("hydrostatic")),
    // Save off the coupled value for use in Residual 
    _stress(coupledValue("hydrostatic")),
    // Couple to the gradient of the pressure
    _grad_stress(coupledGradient("hydrostatic")),
    // Grab necessary material properties
    _D(getMaterialProperty<Real>("D_name")),
    _kb(getParam<Real>("kb")),
    _omega(getParam<Real>("omega")),
    _T_c(getParam<Real>("T_c"))
{
}

Real
BackstressComponent::computeValue()
{
  // Access the gradient of the temperature at this quadrature point
  // Then pull out the "component" of it we are looking for (x, y or z)
  // Note that getting a particular component of a gradient is done using the
  // parenthesis operator
  return -_D[_qp]* _omega * (1.0/(_kb*_T_c)) * _grad_stress[_qp](_component);
}

