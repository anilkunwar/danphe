#ifndef VOLTPFPARAMSPOLYFREEENERGY_H
#define VOLTPFPARAMSPOLYFREEENERGY_H

#include "Material.h"

//Forward Declarations
class VoltPFParamsPolyFreeEnergy;

template<>
InputParameters validParams<VoltPFParamsPolyFreeEnergy>();

/**
 * Calculated properties for a single component phase field model using polynomial free energies
 */
class VoltPFParamsPolyFreeEnergy : public Material
{
public:
  VoltPFParamsPolyFreeEnergy(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  ///Variable values
  const VariableValue & _c;
  //const VariableValue & _T;
  const VariableValue & _volt;
  

  ///Mateiral property declarations
  MaterialProperty<Real> & _M;
  MaterialProperty<RealGradient> & _grad_M;

  MaterialProperty<Real> & _kappa;
  MaterialProperty<Real> & _c_eq;
  MaterialProperty<Real> & _W;
  //MaterialProperty<Real> & _Qstar;
  MaterialProperty<Real> & _zeff;
  //MaterialProperty<Real> & _T;
  MaterialProperty<Real> & _D;

  ///Input parameters
  Real _T;
  Real _int_width;
  Real _length_scale;
  Real _time_scale;
  MooseEnum _order;
  Real _D0;
  Real _Em;
  Real _Ef;
  Real _surface_energy;

  const Real _JtoeV;
  const Real _kb;
  const Real _echarge;
};

#endif //VOLTPFPARAMSPOLYFREEENERGY_H
