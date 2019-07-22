#ifndef TESTSCENARIO_H
#define TESTSCENARIO_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

// Used for testing.
// This class is a subclass of Scenario and allows LgFr*::test().
// methods access to the scenarios lastProblemSolved,
// setOfParts and setOfDemands.

#include <scenario/src/scenario.h>

class LgFrTestScenarioSmartPointer;

class LgFrTestScenario : public LgFrScenario
{
  
public:

  virtual int GetScenarioType() const { return __LGFRTESTSCENARIO; }

  friend class LgFrTestScenarioSmartPointerToConst;
  friend class LgFrTestScenarioSmartPointer;

  // Return the setOfParts which belongs to this scenario
  LgFrSetOfParts&
     setOfParts(int k = 0);

  // Return the setOfDemands which belongs to this scenario
  LgFrSetOfDemands&
     setOfDemands(int k = 0);

  // Return the LastProblemSolved which belongs to this scenario
  LgFrLastProblemSolved&
     lastProblemSolved();

  virtual
  const LgFrLastProblemSolved&
     lastProblemSolved()
         const;
  
  // Constructor
  LgFrTestScenario (const LgFrDataInterfaceAbstract&,
                    const LgFrCalendar&);

  
  // Destructor
  ~LgFrTestScenario();

  // Default Constructor
  LgFrTestScenario();


  // Self-test
  static void test();

  protected:

  // Copy constructor
  LgFrTestScenario(
               const LgFrTestScenario& source);

  private:

  // Assignment operator.
  LgFrTestScenario&
    operator=(
              const LgFrTestScenario& rhs);

  // identifier to identify actual scenario type
  int id_;
};

#endif
