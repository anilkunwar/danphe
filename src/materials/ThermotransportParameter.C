#include "ThermotransportParameter.h"

template<>
InputParameters validParams<ThermotransportParameter>()
{
  InputParameters params = validParams<Material>();
  params.addClassDescription("Phase field parameters for net thermotransport for two component alloy");
  params.addCoupledVar("T", "Temperature variable in Kelvin");
  params.addRequiredCoupledVar("c", "Concentration");
  params.addRequiredParam<Real>("B0_1", "Atomic mobility prefactor for species 1 in m^2/s");
  params.addRequiredParam<Real>("B0_2", "Atomic mobility prefactor for species 2 in m^2/s");
  params.addRequiredParam<Real>("E1", "Activation enthalpy of species 1 in J/mol");
  params.addRequiredParam<Real>("E2", "Activation enthalpy of species 2 in J/mol");
  params.addRequiredParam<Real>("Qh1", "Heat of transport for species 1 in J/mol");
  params.addRequiredParam<Real>("Qh2", "Heat of transport for species 2 in J/mol");
  //params.addRequiredParam<Real>("int_width", "The interfacial width of void surface in the lengthscale of the problem");
  //params.addParam<Real>("length_scale", 1.0e-9, "defines the base length scale of the problem in m");
  //params.addParam<Real>("time_scale", 1.0e-9, "defines the base time scale of the problem");
  return params;
}

ThermotransportParameter::ThermotransportParameter(const InputParameters & parameters) :
    Material(parameters),
    _c(coupledValue("c")),
    _T(coupledValue("T")),
    _Mq(declareProperty<Real>("Mq")),
    _grad_Mq(declareProperty<RealGradient>("grad_Mq")),
    _B1(declareProperty<Real>("B1")),
    _B2(declareProperty<Real>("B2")),
    //_Qstar(declareProperty<Real>("Qstar")),
    //_D(declareProperty<Real>("D")),
    _B0_1(getParam<Real>("B0_1")),
    _B0_2(getParam<Real>("B0_2")),
    _Qh1(getParam<Real>("Qh1")),
    _Qh2(getParam<Real>("Qh2")),
    _E1(getParam<Real>("E1")),
    _E2(getParam<Real>("E2")),
    //_surface_energy(getParam<Real>("surface_energy")),
    //_JtoeV(6.24150974e18), // joule to eV conversion
    //_int_width(getParam<Real>("int_width")),
    //_length_scale(getParam<Real>("length_scale")),
    //_time_scale(getParam<Real>("time_scale")),
    _kb(8.617343e-5) // Boltzmann constant in eV/K
    _R(8.31)  // Universal gas constant in J/mol K
{
}

void
ThermotransportParameter::computeQpProperties()
{
  // Convert mobility from m^2/s to the length and time scale
  //Real D0_c = _D0*_time_scale/(_length_scale*_length_scale);

  Real RT = _R*_T[_qp];

  //Compute atomic mobility of the species 1 and species 2
  _B1[_qp] = _B0_1* std::exp(-_E1/RT);
  _B2[_qp] = _B0_2*std::exp(-_E2/RT);

  // Convert surface energy from J/m2 to eV/length_scale
  //Real surf_energy = _surface_energy*_JtoeV*_length_scale*_length_scale;
  
  //Define the heat of transport of each species
  //_Qstar[_qp] = -4.0; // eV
  
  //In this context the heat of transport of the species are obtained from the input file
  //Compute the net thermotransport factor
   _Mq[_qp] = _B1[_qp]*_Qh1[_qp] - _B2[_qp]*_Qh2[_qp];
   _grad_Mq[_qp] = 0.0;
}

