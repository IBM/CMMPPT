#ifndef Dif_Testing_Factory_H
#define Dif_Testing_Factory_H

#include <scenario/src/abFctyAp.h>

class LgFrDifTestingFactory : public LgFrApplicationAbstractFactory
{
public:

  LgFrDifTestingFactory();

  ~LgFrDifTestingFactory();

  virtual 
     LgFrInitialProblemAndParameters* 
        createIPP(
           LgFrScenarioSmartPointer scenario) 
              const;

  virtual 
     LgFrScheduleFactory* 
        createScheduleFactory(
           LgFrScenarioSmartPointer scenario) 
              const;

  void 
     test();

  virtual 
     void 
        factoryType() 
           const;

private:
  LgFrDifTestingFactory(const LgFrDifTestingFactory&);
  LgFrDifTestingFactory& operator=(const LgFrDifTestingFactory&);  

};
#endif
