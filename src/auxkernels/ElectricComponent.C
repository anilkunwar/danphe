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

#include "ElectricComponent.h"

template<>
InputParameters validParams<ElectricComponent>()
{
  InputParameters params = validParams<AuxKernel>();
  
  // Declare the options for a MooseEnum.
  // These options will be presented to the user in Peacock
  // and if something other than these options is in the input file
  // an error will be printed
  MooseEnum component("x y z");

  // Use the MooseEnum to add a parameter called "component"
  params.addRequiredParam<MooseEnum>("component", component, "The desired component of velocity.");
  params.addRequiredParam<MaterialPropertyName>("D_name", "The diffusivity used with the kernel");
  params.addRequiredParam<Real>("z", "Effective charge number for Cu");
  // Add a parameter with a default value.  This value can be overriden in the input file.
  params.addParam<Real>("kb", 1.38e-23, "The Boltzmann constant in J/K");
  params.addParam<Real>("e", 1.6e-19, "electronic charge in coulomb");
  params.addParam<Real>("rho", 5.5e-7, "electric resistivity of liquid tin in ohm m");
  params.addParam<Real>("T_c", 623.0, "Temperature of the solder medium in K");
  return params;
}

ElectricComponent::ElectricComponent(const InputParameters & parameters) :
    AuxKernel(parameters),
    // This will automatically convert the MooseEnum to an integer
    _component(getParam<MooseEnum>("component")),
    // Grab necessary material properties
    _D(getMaterialProperty<Real>("D_name")),
    //current_density(getParam<RealVectorValue>("current_density")),
    _current_density(getMaterialProperty<RealVectorValue>("current_density")),
    _z(getParam<Real>("z")),
    _kb(getParam<Real>("kb")),
    _e(getParam<Real>("e")),
    _rho(getParam<Real>("rho")),
    _T_c(getParam<Real>("T_c"))
{
}

Real
ElectricComponent::computeValue()
{
  // Access the gradient of the temperature at this quadrature point
  // Then pull out the "component" of it we are looking for (x, y or z)
  // Note that getting a particular component of a gradient is done using the
  // parenthesis operator
  //Real ElectricalVelocity = -_D[_qp]*_z *_e * _rho/(_kb* _T_c); 
  //return ElectricalVelocity*MaterialRealVectorValueAux::computeValue();
  return -(_D[_qp]*_z *_e * _rho/(_kb* _T_c))*_current_density[_qp](_component); 
  //return -_D[_qp]* _Qh * (1.0/(_kb*_T[_qp]*_T[_qp])) * _grad_T[_qp](_component);
}
