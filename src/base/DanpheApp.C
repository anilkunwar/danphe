#include "DanpheApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
//#include "ModulesApp.h"
//#include "PhaseFieldApp.h"
//#include "SolidMechanicsApp.h"
//#include "TensorMechanicsApp.h"
//#include "HeatConductionApp.h"
//#include "NavierStokesApp.h"
//#include "RichardsApp.h"
//#include "MiscApp.h"
#include "ChemicalReactionsApp.h"
//#include "CombinedApp.h"
#include "ContactApp.h"
#include "HeatConductionApp.h"
#include "LinearElasticityApp.h"
#include "MiscApp.h"
#include "NavierStokesApp.h"
#include "PhaseFieldApp.h"
#include "RichardsApp.h"
#include "SolidMechanicsApp.h"
#include "TensorMechanicsApp.h"
#include "WaterSteamEOSApp.h"
#include "XFEMApp.h"
//Kernels
#include "ElectricPotential.h"
//#include "CoupledPotential.h"
#include "SplitCHVoltage.h"
//#include "GasGeneration.h"
#include "MultiSoretDiffusion.h"
#include "ThermalConvection.h"
#include "SinkTerm.h"
#include "ReactionTerm.h"
#include "ConstantTensorElectricConvection.h"
#include "BackstressConvection.h"
#include "BackstressDiffusion.h"
#include "LaplacianStress.h"
#include "INSMomentumGravity.h"
//Auxkernels
#include "CurrentDensity.h"
#include "ThermalComponent.h"
#include "ElectricComponent.h"
#include "DriftVelocity.h"
#include "BackstressComponent.h"
//Boundary Conditions
#include "RobinBCS.h"
#include "FunctionRobinBCS.h"
//Materials
//#include "TinSheet.h" //see at ~/project/material_danphe/
#include "VoltPFParamsPolyFreeEnergy.h"
#include "TempPFParamsPolyFreeEnergy.h"
#include "ScaledPolynomialFreeEnergy.h"
#include "ThermotransportParameter.h"
#include "CurrentDensityMaterial.h"
#include "TemperatureDependentMaterial.h"
#include "HeatCapacityMaterial.h"
#include "DensityMaterial.h"
#include "ThermalConductivityMaterial.h"
//Initial Conditions
//#include "RndTrapezoidBoxIC.h"
//timesteppers
#include "TransientHalf.h"

template<>
InputParameters validParams<DanpheApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}
//Update the deprecated names
//DanpheApp::DanpheApp(const std::string & name, InputParameters parameters) :
   // MooseApp(name, parameters)
DanpheApp::DanpheApp(InputParameters parameters) :
    MooseApp(parameters)

{
  srand(processor_id());

  Moose::registerObjects(_factory);
  //ModulesApp::registerObjects(_factory);
  //PhaseFieldApp::registerObjects(_factory);
  //SolidMechanicsApp::registerObjects(_factory);
  //TensorMechanicsApp::registerObjects(_factory);
  //HeatConductionApp::registerObjects(_factory);
  //NavierStokesApp::registerObjects(_factory);
  //RichardsApp::registerObjects(_factory);
  //MiscApp::registerObjects(_factory);
  ChemicalReactionsApp::registerObjects(_factory);
  //CombinedApp::registerObjects(_factory);
  ContactApp::registerObjects(_factory);
  HeatConductionApp::registerObjects(_factory);
  LinearElasticityApp::registerObjects(_factory);
  MiscApp::registerObjects(_factory);
  NavierStokesApp::registerObjects(_factory);
  PhaseFieldApp::registerObjects(_factory);
  RichardsApp::registerObjects(_factory);
  SolidMechanicsApp::registerObjects(_factory);
  TensorMechanicsApp::registerObjects(_factory);
  WaterSteamEOSApp::registerObjects(_factory);
  XFEMApp::registerObjects(_factory);
  DanpheApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  //ModulesApp::associateSyntax(_syntax, _action_factory);
  //PhaseFieldApp::associateSyntax(_syntax, _action_factory);
  //SolidMechanicsApp::associateSyntax(_syntax, _action_factory);
  //TensorMechanicsApp::associateSyntax(_syntax, _action_factory);
  //HeatConductionApp::associateSyntax(_syntax, _action_factory);
  //NavierStokesApp::associateSyntax(_syntax, _action_factory);
  //RichardsApp::associateSyntax(_syntax, _action_factory);
  //MiscApp::associateSyntax(_syntax, _action_factory);
  ChemicalReactionsApp::associateSyntax(_syntax, _action_factory);
  //CombinedApp::associateSyntax(syntax, action_factory);
  ContactApp::associateSyntax(_syntax, _action_factory);
  HeatConductionApp::associateSyntax(_syntax, _action_factory);
  LinearElasticityApp::associateSyntax(_syntax, _action_factory);
  MiscApp::associateSyntax(_syntax, _action_factory);
  NavierStokesApp::associateSyntax(_syntax, _action_factory);
  PhaseFieldApp::associateSyntax(_syntax, _action_factory);
  RichardsApp::associateSyntax(_syntax, _action_factory);
  SolidMechanicsApp::associateSyntax(_syntax, _action_factory);
  TensorMechanicsApp::associateSyntax(_syntax, _action_factory);
  WaterSteamEOSApp::associateSyntax(_syntax, _action_factory);
  XFEMApp::associateSyntax(_syntax, _action_factory);
  DanpheApp::associateSyntax(_syntax, _action_factory);
}

DanpheApp::~DanpheApp()
{
}

void
DanpheApp::registerApps()
{
  registerApp(DanpheApp);
}

void
DanpheApp::registerObjects(Factory & factory)
{
  registerTimeStepper(TransientHalf);

  registerKernel(ElectricPotential);
  registerKernel(SplitCHVoltage);
  //registerKernel(GasGeneration);
  //registerKernel(CoupledPotential);
  registerKernel(MultiSoretDiffusion);
  registerKernel(ThermalConvection);
  registerKernel(SinkTerm);
  registerKernel(ReactionTerm);
  registerKernel(ConstantTensorElectricConvection);
  registerKernel(BackstressConvection);
  registerKernel(BackstressDiffusion);
  registerKernel(LaplacianStress);
  registerKernel(INSMomentumGravity);
  

  registerAux(CurrentDensity);
  registerAux(ThermalComponent);
  registerAux(ElectricComponent);
  registerAux(DriftVelocity);
  registerAux(BackstressComponent);

  registerBoundaryCondition(RobinBCS);
  registerBoundaryCondition(FunctionRobinBCS);

  //registerMaterial(TinSheet); //see at ~/project/material_danphe/
  registerMaterial(VoltPFParamsPolyFreeEnergy);
  registerMaterial(TempPFParamsPolyFreeEnergy);  
  registerMaterial(ScaledPolynomialFreeEnergy);
  registerMaterial(ThermotransportParameter);
  registerMaterial(CurrentDensityMaterial);
  registerMaterial(TemperatureDependentMaterial);
  registerMaterial(HeatCapacityMaterial);
  registerMaterial(DensityMaterial);
  registerMaterial(ThermalConductivityMaterial);

  //registerInitialCondition(RndTrapezoidBoxIC);
}

void
DanpheApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
}
