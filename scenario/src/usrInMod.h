#ifndef LGFR_USER_INPUT_MODIFICATION_H
#define LGFR_USER_INPUT_MODIFICATION_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/algorthm.h>
#include <scenario/src/supplySS.h>
#include <scenario/src/supSSATP.h>
#include <scenario/src/demVolSS.h>
#include <scenario/src/demSSATP.h>
#include <scenario/src/cycleSS.h>

// This class represents the implosion algorithm

// Forward references
class LgFrWorkingImplosionProblem;

class LgFrUserInputModification : public LgFrAlgorithm {

   // This is a list of methods which obtain the results of a user
   // updating wip by obtaining this objects wipPtr_ and then accessing the
   // wip directly.

   // The updated wip results used by these methods are
   // supply, demandVolume, and cycleTime

  
   // ------------------------------------------------------------------------
   // Attribute Schedule Strategies:
   // ------------------------------------------------------------------------
   friend 
      void
         LgFrSupplyScheduleStrategy::kernalOfPopulate(
            LgFrSortingScheduleFloat& schedule )
               const;

   friend 
      void
         LgFrSupplyScheduleStrategyForATP::kernalOfPopulate(
            LgFrSortingScheduleFloat& schedule )
               const;

   friend 
      void
         LgFrDemandVolumeScheduleStrategy::kernalOfPopulate(
            LgFrSortingScheduleFloat& schedule )
               const;

   friend 
      void
         LgFrDemandVolumeScheduleStrategyForATP::kernalOfPopulate(
            LgFrSortingScheduleFloat& schedule )
               const;

   friend 
      void
         LgFrCycleTimeScheduleStrategy::kernalOfPopulate(
            LgFrSortingScheduleFloat& schedule )
               const;

public:

  int virtual GetAlgorithmType() const { return __LGFRUSERINPUTMODIFICATION; }

  // updateFrom - This method is invoked when the algorthm solution has
  // been updated.
  virtual
     void
        updateFrom (
           LgFrModel * p,
           void * d );

  // Return pointer to the implode algorithm in the specified scenario.
  virtual
     LgFrAlgorithm *
        counterPart(
           LgFrScenario & )
              const;

  // Constructor.
   LgFrUserInputModification(
       LgFrWorkingImplosionProblem & wip );

  // Copy constructor
  LgFrUserInputModification (
     const LgFrUserInputModification& source);
  
  // destructor
  ~LgFrUserInputModification();
  
  // test this class
  static void test();

  // Test this class in the context of it's owner(s).
  // The data for this test should have come from
  // "/u/implode/wit/data/examples/diner12/wit.data"
  static void contextTest(LgFrWorkingImplosionProblem       &wip);


private:

  LgFrWorkingImplosionProblem  * wipPtr_;

  const int id_;

  // default constructor
  LgFrUserInputModification();

  // Assignment 
  LgFrUserInputModification& 
    operator=(const LgFrUserInputModification& rhs);

  // Get Working Implosion Problem associated with this alogrithm.
  // If the owner of the workingImplosionProblem is deleted by its owner then
  // then the returned reference will be dangling
  virtual
     LgFrWorkingImplosionProblem &
        workingImplosionProblem()
           const;

};

#endif
