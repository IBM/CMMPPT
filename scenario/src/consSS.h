#ifndef LGFR_MRP_CONSUMED_SCHEDULE_STRATEGY_H
#define LGFR_MRP_CONSUMED_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/partSS.h>
#include <scenario/src/lastPrbS.h>

//---------------------------------------------------------------------------
//
// This file contains class definitions for:
//   LgFrMrpConsVolScheduleStrategy
//   LgFrFullyExplodedMrpConsVolScheduleStrategy
//
//---------------------------------------------------------------------------


// Schedules using these strategy will be updated with new
// values after running an explode.

// Forward references
class LgFrExplode;
class LgFrSortingPartScheduleFloat;
class LgFrLastProblemSolved;
class LgFrWorkingImplosionProblem;



//---------------------------------------------------------------------------
//
//   LgFrMrpConsVolScheduleStrategy
//
//---------------------------------------------------------------------------

// Schedule Strategy for a MRP Consumption Volume schedule.
// The timeVecs are the parts' mrpConsVol.

// Schedules using this strategy will be updated with the new
// solution after running an explode.


class LgFrMrpConsVolScheduleStrategy : 
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
  LgFrMrpConsVolScheduleStrategy &
    operator=(
	      const LgFrMrpConsVolScheduleStrategy& rhs);

  // Copy constructor
  LgFrMrpConsVolScheduleStrategy (
     const LgFrMrpConsVolScheduleStrategy &);

  // Copy constructor with newScenario
  LgFrMrpConsVolScheduleStrategy (
     const LgFrMrpConsVolScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrMrpConsVolScheduleStrategy (
     LgFrScenario & );

  // Destructor
  virtual
     ~LgFrMrpConsVolScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrMrpConsVolScheduleStrategy methods.
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
  LgFrMrpConsVolScheduleStrategy ();
};


//---------------------------------------------------------------------------
//
//   LgFrFullyExplodedMrpConsVolScheduleStrategy
//
//---------------------------------------------------------------------------

// Schedule Strategy for a MRP Consumption Volume schedule.
// The timeVecs are the parts' fullyExplodedMrpConsVol.
// This is computed usint a utility in witutil.

// Schedules using this strategy will be updated with the new
// solution after running an explode.


class LgFrFullyExplodedMrpConsVolScheduleStrategy : 
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
  LgFrFullyExplodedMrpConsVolScheduleStrategy &
    operator=(
	      const LgFrFullyExplodedMrpConsVolScheduleStrategy& rhs);

  // Copy constructor
  LgFrFullyExplodedMrpConsVolScheduleStrategy (
     const LgFrFullyExplodedMrpConsVolScheduleStrategy &);

  // Copy constructor with newScenario
  LgFrFullyExplodedMrpConsVolScheduleStrategy (
     const LgFrFullyExplodedMrpConsVolScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrFullyExplodedMrpConsVolScheduleStrategy (
     LgFrScenario & );

  // Destructor
  virtual
     ~LgFrFullyExplodedMrpConsVolScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrFullyExplodedMrpConsVolScheduleStrategy methods.
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
  LgFrFullyExplodedMrpConsVolScheduleStrategy ();
};

#endif

