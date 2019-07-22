#ifndef SceFactory_H
#define SceFactory_H

#include <scenario/src/abFctyAp.h>

class LgFrSceFactory : public LgFrApplicationAbstractFactory
{
public:
  LgFrSceFactory();
  ~LgFrSceFactory();
  virtual LgFrInitialProblemAndParameters* createIPP(LgFrScenarioSmartPointer scenario) const;
  virtual LgFrScheduleFactory* createScheduleFactory(LgFrScenarioSmartPointer scenario) const;

#if 0
  void test();
#endif

  virtual void factoryType() const;

private:
  LgFrSceFactory(const LgFrSceFactory&);
  LgFrSceFactory& operator=(const LgFrSceFactory&);  

};

#endif
