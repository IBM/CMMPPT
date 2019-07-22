#ifndef ScenarioDirector_H
#define ScenarioDirector_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/dif.h>
#include <scenario/src/calendar.h>
#include <scenario/src/scenBld.h>


// the director for the builder pattern used for creating scenarios
// the user should:
//   instantiate an application specific subclass of LgFrScenarioBuilder
//   pass a pointer to this instance to the LgFrScenarioDirector
//      constructor
//   instantiate a subclass of LgFrDataInterface
//   instantiate a LgFrCalendar object
//   call the LgFrScenarioDirector::construct() method with
//      the LgFrDataInterface and LgFrCalendar objects as parameters
//   extract the application specific scenario using the getScenario
//      method of the LgFrScenarioBuilder
// for an example see the LgFrScenarioDirector::test() method in
//   scenDrct.C
//

class LgFrScenarioDirector
{
public:
  LgFrScenarioDirector(LgFrScenarioBuilder* builder);
  ~LgFrScenarioDirector();
  void construct(const LgFrDataInterfaceAbstract& dif, const LgFrCalendar& calendar);
  static void test();

private:
  LgFrScenarioBuilder* builderPtr_;
  
  LgFrScenarioDirector(const LgFrScenarioDirector&); // not allowed
  LgFrScenarioDirector& operator=(const LgFrScenarioDirector&);  // not allowed
};
#endif

