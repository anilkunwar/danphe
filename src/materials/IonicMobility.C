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
#include "IonicMobility.h"

template<>
InputParameters validParams<IonicMobility>()
{
  InputParameters params = validParams<Material>();

  // Add a parameter to get the radius of the balls in the column (used later to interpolate permeability).
  //params.addParam<Real>("ball_radius", "The radius of the steel balls that are packed in the column.  Used to interpolate _permeability.");
  params.addParam<Real>("z", "Ion charge number for the transported species.");
  params.addParam<Real>("F", 96500, "Faraday's constant in C.");
  params.addParam<Real>("R", 8.31, "Universal gas constant in J/mol K.");
  params.addParam<Real>("T", 298, "Temperature of the liquid or solvent in K.");
  return params;
}


IonicMobility::IonicMobility(const InputParameters & parameters) :
    Material(parameters),
    // Declare two material properties.  This returns references that we
    // hold onto as member variables
    _M(declareProperty<Real>("M_name")),
    //_viscosity(declareProperty<Real>("viscosity"))

    // Get the one parameter from the input file
    _z(getParam<Real>("z")),
    _F(getParam<Real>("F")),
    _R(getParam<Real>("R")),
    _T(getParam<Real>("T"))
{
}

void
IonicMobility::computeQpProperties()
{
  //_viscosity[_qp] = 7.98e-4; // (Pa*s) Water at 30 degrees C (Wikipedia)

  // Sample the LinearInterpolation object to get the permeability for the ball size
  _M[_qp] = (_z * _F)/(_R*_T);
}
