#ifndef LGFR_SCHEDULE_MIX_IN_FOR_ATP_H
#define LGFR_SCHEDULE_MIX_IN_FOR_ATP_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/scATPP.h>

class LgFrScheduleMixInForATP
{
public:

  // Get the scenario
  virtual LgFrScenarioForATPSmartPointer scenario();
  virtual LgFrScenarioForATPSmartPointerToConst scenario() const;

  // Set the scenario
  virtual void scenario(LgFrScenarioForATPSmartPointerToConst);

  // Get the desired state
  virtual int desiredState() const;
  
  // Set the desired state
  virtual void desiredState(int);

/**************************************************************
   the below methods should come from the interface to the
   state machine and probably will go away when my stuff
   is tested
   ****************************************************************/

  // Get the current state of the state machine
  virtual int currentState() const;
  virtual void sendToken(int);
/*****************************************************************/

  // Assignment operator
  LgFrScheduleMixInForATP&
  operator=(
            const LgFrScheduleMixInForATP&);

  // Copy constructor
  LgFrScheduleMixInForATP(const LgFrScheduleMixInForATP&);
  
  // Constructor
  LgFrScheduleMixInForATP(LgFrScenarioSmartPointerToConst, int);

  // Destructor
  virtual ~LgFrScheduleMixInForATP();

protected:
  // Default Constructor
  LgFrScheduleMixInForATP();
  
private:
  LgFrScenarioForATPSmartPointer scenarioPtr_;
  int desiredState_;  // this should be a typedef
};

#endif

