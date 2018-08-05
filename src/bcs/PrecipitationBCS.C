/****************************************************************/
/*
DO NOT MODIFY THIS HEADER
*/
/* MOOSE - Multiphysics Object Oriented Simulation Environment */
/*
*/
/*
/****************************************************************/
#include "PrecipitationBCS.h"
template<>
InputParameters validParams<PrecipitationBCS>()
{
InputParameters params = validParams<IntegratedBC>();
// Here we are adding a parameter that will be extracted from the input file by the Parser
//params.addParam<Real>("alpha", 1.0, "Value multiplied by the coupled value on the boundary");//params.addRequiredCoupledVar("some_var", "Flux Value at the Boundary");
//params.addParam<Real>("rate_constant", 1.0, "Value multiplied by the coupled value on the boundary");
params.addParam<Real>("volume", 1.0, "volume of the domain in m^3");
params.addParam<Real>("area", 1.0, "cross section area of the boundary in m^2");
params.addParam<Real>("C_const", 1.0, "Initial solubility of Cu in liquid Sn");
params.addParam<MaterialPropertyName>("rate_constant", "k_prcpt", "Temperature dependent rate constant for precipitation of Cu in Sn");
params.addParam<MaterialPropertyName>("C_svar", "C_sat", "Temperature dependent solubility of Cu in Sn");
return params;
}
                                      
PrecipitationBCS::PrecipitationBCS(const InputParameters & parameters) :
IntegratedBC(parameters),
//_kr(getParam<Real>("rate_constant")),
_volume(getParam<Real>("volume")),
_area(getParam<Real>("area")),
_C_o(getParam<Real>("C_const")),
_k_rc(getMaterialProperty<Real>("rate_constant")),
_C_svart(getMaterialProperty<Real>("C_svar"))
//_density(getMaterialProperty<Real>("density_name"))
//_some_var_val(coupledValue("some_var"))
{}

                                      Real
PrecipitationBCS::computeQpResidual()
{
return -_test[_i][_qp] * _k_rc[_qp] *_area*(_C_o-_C_svart[_qp])/_volume;
}
