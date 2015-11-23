
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

#ifndef CONSTANTTENSORELECTRICCONVECTION_H
#define CONSTANTTENSORELECTRICCONVECTION_H

#include "Kernel.h"

// Forward Declaration
class ConstantTensorElectricConvection;

template<>
InputParameters validParams<ConstantTensorElectricConvection>();

/**
 * Kernel which implements the convective term in the transient heat
 * conduction equation, and provides coupling with the Darcy pressure
 * equation.
 */
class ConstantTensorElectricConvection : public Kernel
{
public:
  ConstantTensorElectricConvection(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  //virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  /// int label for temperature variable
  //unsigned int _T_var;

  /// Coupled variable for the temperature
  //VariableValue & _T;

  /// Variable gradient for temperature
  //VariableGradient & _grad_T;
  
  /// Diffusivity material property
  const MaterialProperty<Real> & _D;
  
  /// Material current density assumed constant throughout the volume
  /// The tensor values are defined in the input file
  const MaterialProperty<RealTensorValue> &_current_density;

  /// Will be set from the input file
  Real _z;
  Real _kb;
  Real _e;
  Real _rho;
  Real _T_c;

  
};

#endif //CONSTANTTENSORELECTRICCONVECTION_H
