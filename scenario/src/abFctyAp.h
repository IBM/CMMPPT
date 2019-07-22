#ifndef AbstractFactoryApplication_H
#define AbstractFactoryApplication_H

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

#include <scenario/src/initProb.h>
#include <scenario/src/scenario.h>
#include <scenario/src/schFctry.h>

class LgFrScenarioSmartPointer;

class LgFrApplicationAbstractFactory
{
public:
  virtual ~LgFrApplicationAbstractFactory();
  
  virtual LgFrInitialProblemAndParameters* createIPP(LgFrScenarioSmartPointer scenario) const = 0;
  virtual LgFrScheduleFactory* createScheduleFactory(LgFrScenarioSmartPointer scenario) const = 0;  
  virtual void factoryType() const = 0;  
};

#endif
