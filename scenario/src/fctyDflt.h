#ifndef DefaultFactory_H
#define DefaultFactory_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/abFctyAp.h>

class LgFrDefaultFactory: public LgFrApplicationAbstractFactory
{
public:
  LgFrDefaultFactory();
  ~LgFrDefaultFactory();
  virtual LgFrInitialProblemAndParameters* createIPP(LgFrScenarioSmartPointer scenario) const;
  virtual LgFrScheduleFactory* createScheduleFactory(LgFrScenarioSmartPointer scenario) const;
  void test();
  virtual void factoryType() const;
  
private:
  LgFrDefaultFactory(const LgFrDefaultFactory&);
  LgFrDefaultFactory& operator=(const LgFrDefaultFactory&);
};

#endif
