#ifndef LGFR_REQUIREMENT_SCHEDULE_STRATEGY_H
#define LGFR_REQUIREMENT_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/partSS.h>
#include <scenario/src/lastPrbS.h>

//---------------------------------------------------------------------------
//
// This file contains class definitions for:
//   LgFrRequirementScheduleStrategy
//   LgFrCmrpRequirementScheduleStrategy
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
//   LgFrRequirementScheduleStrategy
//
//---------------------------------------------------------------------------

// Schedule Strategy for a requirement schedule.
// The items are parts with category PRODUCT.
// The timeVecs are the parts' requirementVolume.

// Schedules using this strategy will be updated with the new
// production solution after running an explode.

// This class follows the Strategy Pattern

// Forward references
class LgFrExplode;
class LgFrSortingPartScheduleFloat;
class LgFrLastProblemSolved;
class LgFrWorkingImplosionProblem;

class LgFrRequirementScheduleStrategy : 
   public LgFrPartScheduleStrategy {


public:

  // Populate Schedule with itemTimeVecs
  virtual
     void
        kernalOfPopulate(
           LgFrSortingScheduleFloat& )
              const;


  // Make clone copy of this object
  virtual
     LgFrAttributeScheduleStrategy *
        clone() 
           const;

  // Make clone copy of this object
  virtual
     LgFrAttributeScheduleStrategy *
        clone(LgFrScenario &) 
           const;

  // Assignment operator
  LgFrRequirementScheduleStrategy &
    operator=(
	      const LgFrRequirementScheduleStrategy& rhs);

  // Copy constructor
  LgFrRequirementScheduleStrategy (
     const LgFrRequirementScheduleStrategy &);

  // Copy constructor with newScenario
  LgFrRequirementScheduleStrategy (
     const LgFrRequirementScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrRequirementScheduleStrategy (
     LgFrScenario & );

  // Destructor
  virtual
     ~LgFrRequirementScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrRequirementScheduleStrategy methods.
  // testScenario should have been generated using the diner12 dataset
  static
  void   
  contextTest     (LgFrScenario          & testScenario,
		   LgFrWorkingImplosionProblem & wip,
                   LgFrLastProblemSolved & lps,
                   LgFrSetOfParts        & sop);

private:

  LgFrExplode * explodePtr_;
  const int id_;

  // Default Constructor
  LgFrRequirementScheduleStrategy ();
};


//---------------------------------------------------------------------------
//
//   LgFrCmrpRequirementScheduleStrategy
//
//---------------------------------------------------------------------------

// Schedule Strategy for a MRP Production Volume schedule.
// The items are parts with category PRODUCT.
// The timeVecs are the parts' mrpProdVol.

// Schedules using this strategy will be updated with the new
// production solution after running an explode.


class LgFrCmrpRequirementScheduleStrategy : 
   public LgFrPartScheduleStrategy {


public:

  // Populate Schedule with itemTimeVecs
  virtual
     void
        kernalOfPopulate(
           LgFrSortingScheduleFloat& )
              const;


  // Make clone copy of this object
  virtual
     LgFrAttributeScheduleStrategy *
        clone() 
           const;

  // Make clone copy of this object
  virtual
     LgFrAttributeScheduleStrategy *
        clone(LgFrScenario &) 
           const;

  // Assignment operator
  LgFrCmrpRequirementScheduleStrategy &
    operator=(
	      const LgFrCmrpRequirementScheduleStrategy& rhs);

  // Copy constructor
  LgFrCmrpRequirementScheduleStrategy (
     const LgFrCmrpRequirementScheduleStrategy &);

  // Copy constructor with newScenario
  LgFrCmrpRequirementScheduleStrategy (
     const LgFrCmrpRequirementScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrCmrpRequirementScheduleStrategy (
     LgFrScenario & );

  // Destructor
  virtual
     ~LgFrCmrpRequirementScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrCmrpRequirementScheduleStrategy methods.
  // testScenario should have been generated using the diner12 dataset
  static
  void   
  contextTest     (LgFrScenario          & testScenario,
		   LgFrWorkingImplosionProblem & wip,
                   LgFrLastProblemSolved & lps,
                   LgFrSetOfParts        & sop);

private:

  LgFrExplode * explodePtr_;
  const int id_;

  // Default Constructor
  LgFrCmrpRequirementScheduleStrategy ();
};

#endif

