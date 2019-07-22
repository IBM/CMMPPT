#ifndef SCENARIOFORDIFTESTING
#define SCENARIOFORDIFTESTING

#include <scenario/src/scenario.h>


// forward declarations
class LgFrInitialProblemAndParameters;

class LgFrScenarioForDifTesting : public LgFrScenario
{

  friend class LgFrInitialProblemForDifTesting;
  friend class LgFrScenarioForDifTestingSmartPointerToConst;
  friend class LgFrScenarioForDifTestingSmartPointer;
  
  public:

  virtual
     isAReturnType
        isA()
           const;

  // For Testing Make DIF available
  LgFrDataInterfaceAbstract*
    dif();

  // Constructor
  LgFrScenarioForDifTesting ( 
      const LgFrDataInterfaceAbstract&,
      const LgFrCalendar&);

  // Destructor
  ~LgFrScenarioForDifTesting();

  // Default Constructor
  LgFrScenarioForDifTesting();

  // Self-test
  static void test();

 protected:
  // Copy constructor
  LgFrScenarioForDifTesting(
               const LgFrScenarioForDifTesting& source);

 private:

  // identifier for checking acutal scenario type
 int id_;

  // Assignment operator.
  LgFrScenarioForDifTesting&
    operator=(
              const LgFrScenarioForDifTesting& rhs);

};

#endif
