#ifndef CMRPFactory_H
#define CMRPFactory_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/abFctyAp.h>

class LgFrCMRPFactory : public LgFrApplicationAbstractFactory
{
public:
  LgFrCMRPFactory();
  ~LgFrCMRPFactory();
  virtual LgFrInitialProblemAndParameters* createIPP(LgFrScenarioSmartPointer scenario) const;
  virtual LgFrScheduleFactory* createScheduleFactory(LgFrScenarioSmartPointer scenario) const;

  void test();
  virtual void factoryType() const;

private:
  LgFrCMRPFactory(const LgFrCMRPFactory&);
  LgFrCMRPFactory& operator=(const LgFrCMRPFactory&);  

};




class LgFrCMRPwithPrioritiesFromDifFactory : 
   public LgFrApplicationAbstractFactory
{
public:
  LgFrCMRPwithPrioritiesFromDifFactory();

  ~LgFrCMRPwithPrioritiesFromDifFactory();

  virtual LgFrInitialProblemAndParameters* 
     createIPP(LgFrScenarioSmartPointer scenario) const;

  virtual LgFrScheduleFactory* 
     createScheduleFactory(LgFrScenarioSmartPointer scenario) const;

  void test();

  virtual void factoryType() const;

private:
  LgFrCMRPwithPrioritiesFromDifFactory(
     const LgFrCMRPwithPrioritiesFromDifFactory&);

  LgFrCMRPwithPrioritiesFromDifFactory& operator=(
     const LgFrCMRPwithPrioritiesFromDifFactory&);

};

#endif
