#ifndef ScenarioBuilderForTest_H
#define ScenarioBuilderForTest_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

// build a scenario for Test Scenario

#include <scenario/src/scenBld.h>
#include <scenario/src/abFctyAp.h>
#include <scenario/src/testScen.h>
#include <scenario/src/scTestP.h>

class LgFrScenarioBuilderForTest : public LgFrScenarioBuilder
{
public:
  LgFrScenarioBuilderForTest();
  ~LgFrScenarioBuilderForTest();

  virtual void buildScenario(
      const LgFrDataInterfaceAbstract& dif, const LgFrCalendar& calendar);

  // This method returns the scenario that was created.
  // The caller of the method becomes the owner of the scenario and has
  // all memory-management responsibilities associated therewith.
  // The builder forgets the scenario ever existed.
  
  LgFrTestScenarioSmartPointer getScenario();

  virtual void factoryType() const;  

  static void test();
  
protected:
  virtual
  LgFrInitialProblemAndParameters* createIPP(LgFrScenarioSmartPointer scenario) const;
  virtual
  LgFrScheduleFactory* createScheduleFactory(LgFrScenarioSmartPointer scenario) const;

private:
  LgFrScenarioBuilderForTest(const LgFrScenarioBuilderForTest&);  // no copy constructor
    LgFrScenarioBuilderForTest& operator=(const LgFrScenarioBuilderForTest&); // no assignment operator
    
  

  LgFrTestScenarioSmartPointer scenarioPtr_;
};

#endif
