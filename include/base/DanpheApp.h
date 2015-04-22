#ifndef DANPHEAPP_H
#define DANPHEAPP_H

#include "MooseApp.h"

class DanpheApp;

template<>
InputParameters validParams<DanpheApp>();

class DanpheApp : public MooseApp
{
public:
  DanpheApp(const std::string & name, InputParameters parameters);
  virtual ~DanpheApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* DANPHEAPP_H */
