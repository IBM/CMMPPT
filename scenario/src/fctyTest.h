#ifndef TestFactory_H
#define TestFactory_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/abFctyAp.h>

class LgFrTestFactory: public LgFrApplicationAbstractFactory
{
public:
  LgFrTestFactory();
  ~LgFrTestFactory();
  virtual LgFrInitialProblemAndParameters* createIPP(LgFrScenarioSmartPointer scenario) const;
  virtual LgFrScheduleFactory* createScheduleFactory(LgFrScenarioSmartPointer scenario) const;
  void test();
  virtual void factoryType() const;
  
private:
  LgFrTestFactory(const LgFrTestFactory&);
  LgFrTestFactory& operator=(const LgFrTestFactory&);
};

#endif
