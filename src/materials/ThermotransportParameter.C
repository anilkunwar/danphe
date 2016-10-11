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
  //params.addRequiredParam<Real>("rho", "molar density of the tin mixture or reciprocal of molar volume in mol./m^3");
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
    _E1(getParam<Real>("E1")),
    _E2(getParam<Real>("E2")),
    _Qh1(getParam<Real>("Qh1")),
    _Qh2(getParam<Real>("Qh2")),
    //_rho(getParam<Real>("rho")),
    //_surface_energy(getParam<Real>("surface_energy")),
    //_JtoeV(6.24150974e18), // joule to eV conversion
    _JtoeV(1.0),  //this means no conversion from joules to ev
    //_int_width(getParam<Real>("int_width")),
    //_length_scale(getParam<Real>("length_scale")),
    //_time_scale(getParam<Real>("time_scale")),
    //_kb(8.617343e-5), // Boltzmann constant in eV/K
    _kb(1.38e-23), // Boltzmann constant in J/k
    _R(8.31),  // Universal gas constant in J/mol K
    //_length(1.0e9) //length parameter from meters to nanometers
    //_time(1.0e-9)  // time in nanoseconds scale 
    _length(1.0)  // this means no conversion from meters to millimeters
{
}

void
ThermotransportParameter::computeQpProperties()
{
  // Convert mobility from m^2/s to the length and time scale and nanometers in this context
  //Real D0_c = _D0*_time_scale/(_length_scale*_length_scale);
  Real B0_1 = _B0_1*_length*_length;  //*_time;
  Real B0_2 = _B0_2*_length*_length;  //*_time;
  //Real rhon = _rho/(_length*_length*_length); //molar density in moles/nm^3

  Real RT = _R*_T[_qp];

  //Compute atomic mobility of the species 1 and species 2
  _B1[_qp] = B0_1 * std::exp(-_E1/RT);
  _B2[_qp] = B0_2 *std::exp(-_E2/RT);

  // Convert surface energy from J/m2 to eV/length_scale
  //Real surf_energy = _surface_energy*_JtoeV*_length_scale*_length_scale;

  //Convert heat of transport from J/mol to ev/mol
  Real Qh1 = _Qh1*_JtoeV;
  Real Qh2 = _Qh2*_JtoeV;
  
  //Define the heat of transport of each species
  //_Qstar[_qp] = -4.0; // eV
  
  //In this context the heat of transport of the species are obtained from the input file
  //Compute the net thermotransport factor
   _Mq[_qp] =   _B2[_qp]*Qh2 - _B1[_qp]*Qh1;
   _grad_Mq[_qp] = 0.0;
}

