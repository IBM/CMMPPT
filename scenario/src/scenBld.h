#ifndef ScenarioBuilder_H
#define ScenarioBuilder_H
// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.


class LgFrDataInterfaceAbstract;
class LgFrCalendar;
class LgFrScenario;
class LgFrApplicationAbstractFactory;
class LgFrInitialProblemAndParameters;
class LgFrLastProblemSolved;
class LgFrScheduleFactory;
class LgFrScenarioSmartPointer;


// abstract base class for LgFrScenarioBuilderForATP, LgFrScenarioBuilderForCMRP, ...
class LgFrScenarioBuilder
{
public:

  // This method creates and initializes a scenario.
  virtual
  void
  buildScenario(const LgFrDataInterfaceAbstract& dif,
                const LgFrCalendar& calendar) = 0;

  
  virtual void factoryType() const = 0;

  LgFrScenarioBuilder(LgFrApplicationAbstractFactory*);
  virtual ~LgFrScenarioBuilder();
  
  static void test();

protected:

  virtual
  LgFrInitialProblemAndParameters* createIPP(LgFrScenarioSmartPointer scenario) const = 0;

  virtual
  LgFrScheduleFactory* createScheduleFactory(LgFrScenarioSmartPointer scenario) const = 0;


  //////////////////////////////////////////////////////////////////////////////
  //                                                                          //
  // The following method does initialization of "typical" scenarios, such    //
  // as LgFrScenario, LgFrScenarioForCMRP, and LgFrTestScenario.  It is pro-  //
  // vided for the convenience of subclasses of LgFrScenarioBuilder but       //
  // someday there may be subclasses of LgFrScenario that can be built using  //
  // the Builder pattern but without using this particular member function.   //
  //                                                                          //
  //////////////////////////////////////////////////////////////////////////////

  void finishBuildingScenario (LgFrScenarioSmartPointer);
  
  //////////////////////////////////////////////////////////////////////////////
  //                                                                          //
  //  The following methods are provided for accessing protected and private  //
  //  member functions in LgFrScenario.  They are for the convenience of      //
  //  the people writting new scenario-builders (i.e. subclasses of this      //
  //  class).  This way, these people don't have to make their new scenario-  //
  //  builders friends of LgFrScenario.  (So they don't have to change their  //
  //  scenario.h code when they get new releases.)                            //
  //                                                                          //
  //////////////////////////////////////////////////////////////////////////////
  
  void
  scenariosInitialProblemAndParameters (LgFrScenarioSmartPointer, // Set
                                        LgFrInitialProblemAndParameters *);
  LgFrInitialProblemAndParameters&
  scenariosInitialProblemAndParameters (LgFrScenarioSmartPointer); // Get

  LgFrLastProblemSolved&
  scenariosLastProblemSolved (LgFrScenarioSmartPointer);            // Get

  void
  scenariosScheduleFactory  (LgFrScenarioSmartPointer,             // Set
                             LgFrScheduleFactory *);

  void
  scenariosUniqueName (LgFrScenarioSmartPointer,                  // Set
                       std::string
                       );
  std::string
  scenariosDeriveUniqueName (LgFrScenarioSmartPointer);            // Compute and Get
       
  LgFrApplicationAbstractFactory* factoryPtr_;
};

#endif
