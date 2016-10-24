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

#ifndef HEATCAPACITYMATERIAL_H
#define HEATCAPACITYMATERIAL_H

#include "Material.h"
#include "LinearInterpolation.h"

//Forward Declarations
class HeatCapacityMaterial;

template<>
InputParameters validParams<HeatCapacityMaterial>();

/**
 * Example material class that defines a few properties.
 */
class HeatCapacityMaterial : public Material
{
public:
  HeatCapacityMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

private:
  /**
   * This is the member reference that will hold the computed values
   * for the Real value property in this class.
   */
  MaterialProperty<Real> & _cprhokmu;

  /**
   * Computed values for the Gradient value property in this class.
   */
  MaterialProperty<RealGradient> & _convection_velocity;

  /**
   * This is the member reference that will hold the gradient
   * of the coupled variable
   */
  const VariableGradient & _T_grad;

  /**
   * This object returns a piecewise linear function based an a series
   * of points and their corresponding values
   */
  LinearInterpolation _piecewise_func;
};

#endif //HEATCAPACITYMATERIAL_H
