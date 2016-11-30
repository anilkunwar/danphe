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

#include "NernstPlanckConvection.h"

template<>
InputParameters validParams<NernstPlanckConvection>()
{
  InputParameters params = validParams<Kernel>();

  params.addRequiredCoupledVar("Voltage", "The variable representing the voltage.");
  // Add a required parameter.  If this isn't provided in the input file MOOSE will error.
  params.addRequiredParam<MaterialPropertyName>("M_name", "The mobility used with the kernel");
  //params.addRequiredParam<Real>("Q_asterik", "The heat of transport of Cu (Q*) in Sn at T in J/mol");

  // Add a parameter with a default value.  This value can be overriden in the input file.
  //params.addParam<Real>("kb", 1.38e-23, "The Boltzmann constant in J/K");


  return params;
}

NernstPlanckConvection::NernstPlanckConvection(const InputParameters & parameters) :
    Kernel(parameters),
    // Save off the coupled variable identifier for use in
    // computeQpOffDiagJacobian
    _V_var(coupled("Voltage")),
    // Save off the coupled value for use in Residual 
    _V(coupledValue("Voltage")),
    // Couple to the gradient of the pressure
    _grad_V(coupledGradient("Voltage")),
    // Grab necessary material properties
    _M(getMaterialProperty<Real>("M_name"))
    //_Qh(getParam<Real>("Q_asterik")),
    //_kb(getParam<Real>("kb"))
{
}

Real
NernstPlanckConvection::computeQpResidual()
{
  // See also: E. Majchrzak and L. Turchan, "The Finite Difference
  // Method For Transient Convection Diffusion", Scientific Research
  // of the Institute of Mathematics and Computer Science, vol. 1,
  // no. 11, 2012, pp. 63-72.
  // http://srimcs.im.pcz.pl/2012_1/art_07.pdf

  // http://en.wikipedia.org/wiki/Superficial_velocity
  RealVectorValue ion_velocity =
   -_M[_qp]* _grad_V[_qp];

  return ion_velocity * _grad_u[_qp] * _test[_i][_qp];
}

Real
NernstPlanckConvection::computeQpJacobian()
{
  RealVectorValue ion_velocity =
   -_M[_qp]* _grad_V[_qp];

  return ion_velocity * _grad_phi[_j][_qp] * _test[_i][_qp];
}

Real
NernstPlanckConvection::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _V_var)
  {
    -_M[_qp]* _grad_phi[_j][_qp]*_grad_u[_qp] * _test[_i][_qp];
  }

  return 0.0;
}
