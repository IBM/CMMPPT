#ifndef ScenarioBuilderForDefault_H
#define ScenarioBuilderForDefault_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

// build a scenario for Default Scenario

#include <scenario/src/calendar.h>
#include <scenario/src/scenBld.h>
#include <scenario/src/abFctyAp.h>
#include <scenario/src/scenario.h>

class LgFrScenarioBuilderForDefault : public LgFrScenarioBuilder
{
public:
  LgFrScenarioBuilderForDefault();
  ~LgFrScenarioBuilderForDefault();

  void buildScenario(
      const LgFrDataInterfaceAbstract& dif, 
      const LgFrCalendar& calendar);

  LgFrScenarioSmartPointer getScenario();

  
  void factoryType() const;  

  static void test();

protected:
  LgFrInitialProblemAndParameters* createIPP(LgFrScenarioSmartPointer scenario) const;
  LgFrScheduleFactory* createScheduleFactory(LgFrScenarioSmartPointer scenario) const;

private:
  LgFrScenarioSmartPointer scenarioPtr_;
  LgFrScenarioBuilderForDefault(const LgFrScenarioBuilderForDefault&);
  LgFrScenarioBuilderForDefault& operator=(const LgFrScenarioBuilderForDefault&);
};

#endif
