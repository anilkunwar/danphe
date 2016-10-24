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

#include "TemperatureDependentMaterial.h"

template<>
InputParameters validParams<TemperatureDependentMaterial>()
{
  InputParameters params = validParams<Material>();

  // Vectors for Linear Interpolation
  params.addRequiredParam<std::vector<Real> >("independent_vals", "The vector of indepedent values for building the piecewise function");
  params.addRequiredParam<std::vector<Real> >("dependent_vals", "The vector of depedent values for building the piecewise function");

  //params.addCoupledVar("diffusion_gradient", "The gradient of this variable will be used to compute a velocity vector property.");
  params.addCoupledVar("T_grad", "The tempertature variable will be used to compute a velocity vector property.");

  return params;
}

TemperatureDependentMaterial::TemperatureDependentMaterial(const InputParameters & parameters) :
    Material(parameters),
    // Declare that this material is going to provide a Real
    // valued property named "diffusivity" that Kernels can use.
    _cprhokmu(declareProperty<Real>("material property")),
    // material property is a temperature dependent property such as viscosity, density, heat capacity, thermal conductivity

    // Declare that this material is going to provide a RealGradient
    // valued property named "convection_velocity" that Kernels can use.
    _convection_velocity(declareProperty<RealGradient>("convection_velocity")),

    // Get the reference to the variable coupled into this Material
    _T_grad(isCoupled("T_grad") ? coupledGradient("T_grad") : _grad_zero),

    _piecewise_func(getParam<std::vector<Real> >("independent_vals"),
                    getParam<std::vector<Real> >("dependent_vals"))
{}

void
TemperatureDependentMaterial::computeQpProperties()
{
  // We will compute the diffusivity based on the Linear Interpolation of the provided vectors in the z-direction
  _cprhokmu[_qp] = _piecewise_func.sample(_q_point[_qp](2));

  _convection_velocity[_qp] = _T_grad[_qp];
}
