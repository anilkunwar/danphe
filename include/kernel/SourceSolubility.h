#ifndef SOURCESOLUBILITY_H
#define SOURCESOLUBILITY_H

#include "BodyForce.h"


// Forward Declarations
class SourceSolubility;

template <>
InputParameters validParams<SourceSolubility>();

/**
 * This kernel calculates the heat source term corresponding to joule heating,
 * Q = J * E = elec_cond * grad_phi * grad_phi, where phi is the electrical potenstial.
 */
class SourceSolubility : public BodyForce
{
public:
  SourceSolubility(const InputParameters & parameters);
  //virtual void initialSetup();

protected:
  virtual Real computeQpResidual();
  //virtual Real computeQpJacobian();
  //virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  //const VariableGradient & _grad_elec;
  //const unsigned int _elec_var;
  //Multipliers or dividers in the kernel
  Real _volume;
  //Material property
  const MaterialProperty<Real> & _C_svart;
  //const MaterialProperty<Real> & _delec_cond_dT;
  //std::vector<const MaterialProperty<Real> *> _delec_cond_darg;
};

#endif // SOURCESOLUBILITY_H
