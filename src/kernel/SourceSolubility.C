
#include "SourceSolubility.h"

template <>
InputParameters
validParams<SourceSolubility>()
{
  InputParameters params = validParams<BodyForce>();
  //params.addCoupledVar("elec", "Electric potential for joule heating.");
  //params.addCoupledVar("args", "Vector of arguments of the diffusivity");
  params.addParam<Real>("volume", 1.0, "volume of the domain in m^3");
  params.addParam<MaterialPropertyName>("C_svar", "C_sat", "Temperature dependent solubility of Cu in Sn");
  return params;
}

SourceSolubility::SourceSolubility(const InputParameters & parameters)
  :BodyForce(parameters),
  _volume(getParam<Real>("volume")),
  _C_svart(getMaterialProperty<Real>("C_svar"))
{}


Real
SourceSolubility::computeQpResidual()
{
  return -(_C_svart[_qp] /_volume) * _test[_i][_qp];
}


