#ifndef SCALEDPOLYNOMIALFREEENERGY_H
#define SCALEDPOLYNOMIALFREEENERGY_H

#include "DerivativeParsedMaterialHelper.h"
#include "ExpressionBuilder.h"

// Forward Declarations
class ScaledPolynomialFreeEnergy;

template<>
InputParameters validParams<ScaledPolynomialFreeEnergy>();

/**
 * Derivative free energy material defining polynomial free energies for single component materials, with derivatives from ExpressionBuilder
 */
class ScaledPolynomialFreeEnergy : public DerivativeParsedMaterialHelper,
                             public ExpressionBuilder
{
public:
  ScaledPolynomialFreeEnergy(const InputParameters & parameters);
  //ScaledPolynomialFreeEnergy(const std::string & deprecated_name, InputParameters parameters); // DEPRECATED CONSTRUCTOR

protected:
  ///Concentration variable used in the free energy expression
  EBTerm _c;

  ///Equilibrium concentration
  EBTerm _a;

  ///Barrier height
  EBTerm _W;

  ///Polynomial order
  MooseEnum _order;
};

#endif //SCALEDPOLYNOMIALFREEENERGY_H
