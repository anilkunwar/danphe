#ifndef THERMOTRANSPORTPARAMETER_H
#define THERMOTRANSPORTPARAMETER_H

#include "Material.h"

//Forward Declarations
class ThermotransportParameter;

template<>
InputParameters validParams<ThermotransportParameter>();

/**
 * Calculated properties for two component phase field model for thermotransport
 */
class ThermotransportParameter : public Material
{
public:
  ThermotransportParameter(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  ///Variable values
  VariableValue & _c;
  VariableValue & _T;

  ///Mateiral property declarations
  MaterialProperty<Real> & _Mq;
  MaterialProperty<RealGradient> & _grad_Mq;

  //MaterialProperty<Real> & _kappa; 
  //declared as secondary parameters in dot C files and have to be evaluated from the input parameters
  MaterialProperty<Real> & _B1;
  MaterialProperty<Real> & _B2;
  //MaterialProperty<Real> & _Qstar;
  //MaterialProperty<Real> & _D;

  ///Input parameters via the dot i input file
  Real _B0_1;
  Real _B0_2;
  Real _E1;
  Real _E2;
  Real _Qh1;
  Real _Qh2;
  //Real _surface_energy;
  //Real _int_width;
  //Real _length_scale;
  //Real _time_scale;

  //Input as values in the dot C file
  const Real _kb;
  //const Real _JtoeV;
  const Real _R;
};

#endif //THERMOTRANSPORTPARAMETER_H
