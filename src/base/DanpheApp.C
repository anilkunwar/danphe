#include "DanpheApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"

//Kernels
#include "ElectricPotential.h"
//Auxkernels
#include "CurrentDensity.h"
//Materials
#include "TinSheet.h"

template<>
InputParameters validParams<DanpheApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

DanpheApp::DanpheApp(const std::string & name, InputParameters parameters) :
    MooseApp(name, parameters)
{
  srand(processor_id());

  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  DanpheApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
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
  registerKernel(ElectricPotential);
  registerAux(CurrentDensity);
  registerMaterial(TinSheet);
}

void
DanpheApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
}
