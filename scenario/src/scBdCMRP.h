#ifndef ScenarioBuilderForCMRP_H
#define ScenarioBuilderForCMRP_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

// build a scenario for CMRP

#include <scenario/src/scenBld.h>
#include <scenario/src/abFctyAp.h>
#include <scenario/src/scCMRPP.h>

class LgFrApplicationAbstractFactory;
class LgFrScenarioForCMRP;
// class LgFrScenarioForCMRPwithDif;
class LgFrScheduleFactory;

class LgFrScenarioBuilderForCMRP : public LgFrScenarioBuilder
{
public:
  LgFrScenarioBuilderForCMRP();
  ~LgFrScenarioBuilderForCMRP();

  void buildScenario(
      const LgFrDataInterfaceAbstract& dif, 
      const LgFrCalendar& calendar);

  LgFrScenarioForCMRPSmartPointer getScenario();
  
  void factoryType() const;  

  static void test();

protected:
  virtual
  LgFrInitialProblemAndParameters* createIPP(LgFrScenarioSmartPointer scenario) const;
  virtual
  LgFrScheduleFactory* createScheduleFactory(LgFrScenarioSmartPointer scenario) const;

private:
  LgFrScenarioForCMRPSmartPointer scenarioPtr_;
  LgFrScenarioBuilderForCMRP(const LgFrScenarioBuilderForCMRP&);
  LgFrScenarioBuilderForCMRP& operator=(const LgFrScenarioBuilderForCMRP&);  
};



class LgFrScenarioBuilderForCMRPwithPrioritiesFromDif : 
   public LgFrScenarioBuilder
{
public:
  LgFrScenarioBuilderForCMRPwithPrioritiesFromDif();
  ~LgFrScenarioBuilderForCMRPwithPrioritiesFromDif();

  void buildScenario(
     const LgFrDataInterfaceAbstract& dif, 
     const LgFrCalendar& calendar);

  LgFrScenarioForCMRPSmartPointer getScenario();

  void factoryType() const;

  static void test();

protected:
  virtual
  LgFrInitialProblemAndParameters* createIPP(LgFrScenarioSmartPointer scenario) const;

  virtual
  LgFrScheduleFactory* createScheduleFactory(LgFrScenarioSmartPointer scenario) const;

private:
  LgFrScenarioForCMRPSmartPointer scenarioPtr_;
  LgFrScenarioBuilderForCMRPwithPrioritiesFromDif(
     const LgFrScenarioBuilderForCMRPwithPrioritiesFromDif&);
  LgFrScenarioBuilderForCMRPwithPrioritiesFromDif& operator=(
     const LgFrScenarioBuilderForCMRPwithPrioritiesFromDif&);

};

#endif
