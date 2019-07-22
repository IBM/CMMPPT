#ifndef ScenarioBuilderForSce_H
#define ScenarioBuilderForSce_H

// build a scenario for Sce

#include <scenario/src/scenBld.h>
#include <scenario/src/abFctyAp.h>

#include <sce/src/scenSceP.h>

class LgFrApplicationAbstractFactory;
class LgFrScenarioForSce;
class LgFrScheduleFactory;

class LgFrScenarioBuilderForSce : public LgFrScenarioBuilder
{
public:
  LgFrScenarioBuilderForSce();
  ~LgFrScenarioBuilderForSce();

  void buildScenario(const LgFrDataInterfaceAbstract& dif, const LgFrCalendar& calendar);
  LgFrScenarioForSceSmartPointer getScenario();
  
  void factoryType() const;  

  static void test();

protected:
  virtual
  LgFrInitialProblemAndParameters* createIPP(LgFrScenarioSmartPointer scenario) const;
  virtual
  LgFrScheduleFactory* createScheduleFactory(LgFrScenarioSmartPointer scenario) const;

private:
  LgFrScenarioForSceSmartPointer scenarioPtr_;
  LgFrScenarioBuilderForSce(const LgFrScenarioBuilderForSce&);
  LgFrScenarioBuilderForSce& operator=(const LgFrScenarioBuilderForSce&);  
};

#endif
