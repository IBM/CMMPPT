#ifndef ScenarioBuilderForATP_H
#define ScenarioBuilderForATP_H

// build a scenario for ATP

#include <rw/cstring.h>
#include <scenario/src/scenBld.h>
#include <scenario/src/abFctyAp.h>

class LgFrApplicationAbstractFactory;
class LgFrScenarioForATP;
class LgFrScheduleFactory;

class LgFrScenarioBuilderForATP : public LgFrScenarioBuilder
{
public:
  LgFrScenarioBuilderForATP();
  ~LgFrScenarioBuilderForATP();

  void buildScenario(const LgFrDataInterfaceAbstract& dif, const LgFrCalendar& calendar);
  LgFrScenarioForATPSmartPointer getScenario();
  
  void factoryType() const;  

  static void test();

protected:
  virtual
  LgFrInitialProblemAndParameters* createIPP(LgFrScenarioSmartPointer scenario) const;
  virtual
  LgFrScheduleFactory* createScheduleFactory(LgFrScenarioSmartPointer scenario) const;

private:
  LgFrScenarioForATPSmartPointer scenarioPtr_;
  LgFrScenarioBuilderForATP(const LgFrScenarioBuilderForATP&);
  LgFrScenarioBuilderForATP& operator=(const LgFrScenarioBuilderForATP&);  
};

#endif
