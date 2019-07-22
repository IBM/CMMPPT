#ifndef ScenarioBuilderForDifTesting_H
#define ScenarioBuilderForDifTesting_H

// build a scenario for Default Scenario

#include <rw/cstring.h>
#include <scenario/src/calendar.h>
#include <scenario/src/scenBld.h>
#include <scenario/src/abFctyAp.h>
#include <scenario/src/scenario.h>
#include <scenario/src/scDifTP.h>

class LgFrScenarioBuilderForDifTesting : public LgFrScenarioBuilder
{
public:
  LgFrScenarioBuilderForDifTesting();
  ~LgFrScenarioBuilderForDifTesting();

  void buildScenario(
      const LgFrDataInterfaceAbstract& dif, 
      const LgFrCalendar& calendar);

  LgFrScenarioForDifTestingSmartPointer getScenario();

  
  void factoryType() const;  

protected:
  LgFrInitialProblemAndParameters* createIPP(LgFrScenarioSmartPointer scenario) const;
  LgFrScheduleFactory* createScheduleFactory(LgFrScenarioSmartPointer scenario) const;

private:
  LgFrScenarioForDifTestingSmartPointer scenarioPtr_;
  LgFrScenarioBuilderForDifTesting(const LgFrScenarioBuilderForDifTesting&);
  LgFrScenarioBuilderForDifTesting& operator=(const LgFrScenarioBuilderForDifTesting&);
};

#endif
