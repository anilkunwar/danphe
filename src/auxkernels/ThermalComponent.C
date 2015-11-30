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

#include "ThermalComponent.h"

template<>
InputParameters validParams<ThermalComponent>()
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
  params.addRequiredCoupledVar("Temperature", "The variable representing the temperature.");
  // Add a required parameter.  If this isn't provided in the input file MOOSE will error.
  params.addRequiredParam<MaterialPropertyName>("D_name", "The diffusivity used with the kernel");
  params.addRequiredParam<Real>("Q_asterik", "The heat of transport of Cu (Q*) in Sn at T in J/mol");

  // Add a parameter with a default value.  This value can be overriden in the input file.
  params.addParam<Real>("kb", 1.38e-23, "The Boltzmann constant in J/K");


  return params;
}

ThermalComponent::ThermalComponent(const InputParameters & parameters) :
    AuxKernel(parameters),

    // This will automatically convert the MooseEnum to an integer
    _component(getParam<MooseEnum>("component")),

    // Save off the coupled variable identifier for use in
    // computeQpOffDiagJacobian
    _T_var(coupled("Temperature")),
    // Save off the coupled value for use in Residual 
    _T(coupledValue("Temperature")),
    // Couple to the gradient of the pressure
    _grad_T(coupledGradient("Temperature")),
    // Grab necessary material properties
    _D(getMaterialProperty<Real>("D_name")),
    _Qh(getParam<Real>("Q_asterik")),
    _kb(getParam<Real>("kb"))
{
}

Real
ThermalComponent::computeValue()
{
  // Access the gradient of the temperature at this quadrature point
  // Then pull out the "component" of it we are looking for (x, y or z)
  // Note that getting a particular component of a gradient is done using the
  // parenthesis operator
  return -_D[_qp]* _Qh * (1.0/(_kb*_T[_qp]*_T[_qp])) * _grad_T[_qp](_component);
}
