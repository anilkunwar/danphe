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
#include "TinSheet.h"

template<>
InputParameters validParams<TinSheet>()
{
  InputParameters params = validParams<Material>();

  // Add a parameter to get the radius of the balls in the column (used later to interpolate conductivity).
  //params.addParam<Real>("ball_radius", "The radius of the steel balls that are packed in the column.  Used to interpolate _conductivity.");

  return params;
}


TinSheet::TinSheet(const std::string & name, InputParameters parameters) :
    Material(name, parameters),

    // Get the one parameter from the input file
    //_ball_radius(getParam<Real>("ball_radius")),

    // Declare two material properties.  This returns references that we
    // hold onto as member variables
    _conductivity(declareProperty<Real>("conductivity"))
    //_viscosity(declareProperty<Real>("viscosity"))
{
  // Sigh: Still can't depend on C++11....
  //std::vector<Real> ball_sizes(2);
  //ball_sizes[0] = 1;
  //ball_sizes[1] = 3;

  // From the paper: Table 1
  //std::vector<Real> conductivity(2);
  //conductivity[0] = 0.8451e-9;
  //conductivity[1] = 8.968e-9;

  // Set the x,y data on the LinearInterpolation object.
  //_conductivity_interpolation.setData(ball_sizes, conductivity);
}

void
TinSheet::computeQpProperties()
{
  //_viscosity[_qp] = 7.98e-4; // (Pa*s) Water at 30 degrees C (Wikipedia)

  // Sample the LinearInterpolation object to get the conductivity for the ball size
  _conductivity[_qp] = 73; //W/m k _conductivity_interpolation.sample(_ball_radius);
}
