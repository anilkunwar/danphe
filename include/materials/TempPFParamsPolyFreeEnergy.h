#ifndef TEMPPFPARAMSPOLYFREEENERGY_H
#define TEMPPFPARAMSPOLYFREEENERGY_H

#include "Material.h"

//Forward Declarations
class TempPFParamsPolyFreeEnergy;

template<>
InputParameters validParams<TempPFParamsPolyFreeEnergy>();

/**
 * Calculated properties for a single component phase field model using polynomial free energies
 */
class TempPFParamsPolyFreeEnergy : public Material
{
public:
//  TempPFParamsPolyFreeEnergy(const std::string & name,
//                         InputParameters parameters);
 TempPFParamsPolyFreeEnergy(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  ///Variable values
  const VariableValue & _c;
  const VariableValue & _T;

  ///Mateiral property declarations
  MaterialProperty<Real> & _M;
  MaterialProperty<RealGradient> & _grad_M;

  MaterialProperty<Real> & _kappa;
  MaterialProperty<Real> & _c_eq;
  MaterialProperty<Real> & _W;
  MaterialProperty<Real> & _Qstar;
  MaterialProperty<Real> & _D;

  ///Input parameters
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
};

#endif //TEMPPFPARAMSPOLYFREEENERGY_H
