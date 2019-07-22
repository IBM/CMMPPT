#ifndef ATPFactory_H
#define ATPFactory_H

#include <scenario/src/abFctyAp.h>

class LgFrATPFactory : public LgFrApplicationAbstractFactory
{
public:
  LgFrATPFactory();
  ~LgFrATPFactory();
  virtual LgFrInitialProblemAndParameters* createIPP(LgFrScenarioSmartPointer scenario) const;
  virtual LgFrScheduleFactory* createScheduleFactory(LgFrScenarioSmartPointer scenario) const;
  void test();
  virtual void factoryType() const;
  
private:
  LgFrATPFactory(const LgFrATPFactory&);
  LgFrATPFactory& operator=(const LgFrATPFactory&);
};

#endif
