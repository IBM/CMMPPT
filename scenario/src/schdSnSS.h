#ifndef LGFR_SCHEDULE_SENSITIVE_SCHEDULE_STRATEGY_H
#define LGFR_SCHEDULE_SENSITIVE_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/sortSchd.h>
#include <scenario/src/sortSchP.h>
#include <scenario/src/partSS.h>
#include <scenario/src/demandSS.h>


// Part and demand attribute schedule strategy.
// These schedules are derived from one or more other schedules.

// This base class Schedule strategy returns a combination
// (default is sum) of all the independent schedules.  The combination
// can be calculated by applying any LgFrTimeVecFloatOpFunction to
// matching rows of the schedule.

// A few words about ownership: Depending on the ownTheSchedules_ attribute,
// these class may or may not own the schedules they are sensitive to.
// If ownTheSchedules_ is true, this class will delete the schedules when
// it's destructor is called; otherwise the schedules that are inserted
// into instances of this class should not be deleted until the instances
// of this class are deleted.

// Forward references
class LgFrSortingPartScheduleFloat;
class LgFrSortingPartScheduleFloatSmartPointer;
class LgFrSortingDemandScheduleFloat;
class LgFrSortingDemandScheduleFloatSmartPointer;

// ----------------------------------------------------------------
//
//  Description of LgFrPartScheduleSensitiveScheduleStrategy
//
// ----------------------------------------------------------------

class LgFrPartScheduleSensitiveScheduleStrategy : 
   public LgFrPartScheduleStrategy {
public:


  // Set the combining function.  This is the function that is used
  // to combine timeVecs in the schedules.
  virtual
  void
  combiningFunction (
      LgFrTimeVecFloatOpFunction func
      );
  
  // Populate Schedule with itemTimeVecs
  virtual
     void
        kernalOfPopulate(
           LgFrSortingScheduleFloat& )
              const;

  // Insert a Schedule containing data to be used to compute this schedule.
  // If ownTheScedules(), a pointer to a clone of this schedule is kept by
  // this object.  Otherwise, a pointer to the schedule itself is kept and
  // the life of the schedule had better be greater than the live of this
  // object.
  virtual
     void
        insertPartSchedule(
           LgFrSortingPartScheduleFloatSmartPointer );
  virtual
     void
        insertDemandSchedule(
           LgFrSortingDemandScheduleFloatSmartPointer );

  // Make clone copy of this object
  virtual
     LgFrAttributeScheduleStrategy *
        clone() 
           const;

  // Make clone copy of this object with new Scenario
  virtual
     LgFrAttributeScheduleStrategy *
        clone(LgFrScenario &) 
           const;

  // Assignment operator
  LgFrPartScheduleSensitiveScheduleStrategy &
    operator=(
	      const LgFrPartScheduleSensitiveScheduleStrategy& rhs);

  // Copy constructor
  LgFrPartScheduleSensitiveScheduleStrategy (
     const LgFrPartScheduleSensitiveScheduleStrategy &);

  // Copy constructor with newScenario
  LgFrPartScheduleSensitiveScheduleStrategy (
     const LgFrPartScheduleSensitiveScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrPartScheduleSensitiveScheduleStrategy (
     LgFrScenario &);

  // Destructor
  ~LgFrPartScheduleSensitiveScheduleStrategy ();

  // test this class
  static void test();

  // Test LgFrPartScheduleSensitiveScheduleStrategy methods.
  // Expects testScenario to contain data from
  // /u/implode/wit/data/brenda/denise/wit.data
  // Expects supplySchedule to be a supply schedule for testScenario
  // (containing a LgFrSupplyScheduleStrategy).
  static
  void
  contextTest ( LgFrScenario & testScenario,
                LgFrSortingPartScheduleFloatSmartPointer supplySchedule
               );

protected:

  // Default Constructor
  LgFrPartScheduleSensitiveScheduleStrategy ();

  // Estimate size of schedule using this ass.
  // The populate method uses this to set an initial size of the
  // schedule.
  virtual
     size_t
        estimatedSize(
           const LgFrSortingScheduleFloat& )
              const;

   // Return indepenedent schedules
   const LgFrOrderedVectorSortingScheduleFloatSmartPointer &
      schedules()
         const;

  // If ownTheScedules() is true, insert a pointer to a copy of the argument.
  // Otherwise insert a pointer to the argument itself.
  virtual
     void
        localInsert(
           LgFrSortingScheduleFloatSmartPointer );

  LgFrOrderedVectorSortingScheduleFloatSmartPointer  independentSchedules_;

private:
  LgFrTimeVecFloatOpFunction 	     		     combiningFunction_;
  const int id_;
};




// ----------------------------------------------------------------
//
//  Description of LgFrDemandScheduleSensitiveScheduleStrategy
//
// ----------------------------------------------------------------

class LgFrDemandScheduleSensitiveScheduleStrategy : 
   public LgFrDemandScheduleStrategy {
public:

  // Populate Schedule with itemTimeVecs
  virtual
     void
        kernalOfPopulate(
           LgFrSortingScheduleFloat& )
              const;

  // Insert a Schedule containing data to be used to compute this schedule.
  // If ownTheScedules(), a pointer to a clone of this schedule is kept by
  // this object.  Otherwise, a pointer to the schedule itself is kept and
  // the life of the schedule had better be greater than the live of this
  // object.
  virtual
     void
        insertPartSchedule(
           LgFrSortingPartScheduleFloatSmartPointer );
  virtual
     void
        insertDemandSchedule(
           LgFrSortingDemandScheduleFloatSmartPointer );

  // Make clone copy of this object
  virtual
     LgFrAttributeScheduleStrategy *
        clone() 
           const;

  // Make clone copy of this object with new Scenario
  virtual
     LgFrAttributeScheduleStrategy *
        clone(LgFrScenario &) 
           const;

  // Assignment operator
  LgFrDemandScheduleSensitiveScheduleStrategy &
    operator=(
	      const LgFrDemandScheduleSensitiveScheduleStrategy& rhs);

  // Copy constructor
  LgFrDemandScheduleSensitiveScheduleStrategy (
     const LgFrDemandScheduleSensitiveScheduleStrategy &);

  // Copy constructor with newScenario
  LgFrDemandScheduleSensitiveScheduleStrategy (
     const LgFrDemandScheduleSensitiveScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrDemandScheduleSensitiveScheduleStrategy (
     LgFrScenario &);

  // Destructor
  ~LgFrDemandScheduleSensitiveScheduleStrategy ();

  // test this class
  static void test();

  // Test LgFrDemandScheduleSensitiveScheduleStrategy methods.
  // Expects testScenario to contain data from
  // /u/implode/wit/data/brenda/denise/wit.data
  // Expects supplySchedule to be a supply schedule for testScenario
  // (containing a LgFrSupplyScheduleStrategy).
  static
  void
  contextTest ( LgFrScenario & testScenario,
                LgFrSortingDemandScheduleFloatSmartPointer demandVolSchedule
               );

protected:

  // Default Constructor
  LgFrDemandScheduleSensitiveScheduleStrategy ();

  // Estimate size of schedule using this ass.
  // The populate method uses this to set an initial size of the
  // schedule.
  virtual
     size_t
        estimatedSize(
           const LgFrSortingScheduleFloat& )
              const;

   // Return indepenedent schedules
   const LgFrOrderedVectorSortingScheduleFloatSmartPointer &
      schedules()
         const;

  virtual
     void
        localInsert(
           LgFrSortingScheduleFloatSmartPointer );

  LgFrOrderedVectorSortingScheduleFloatSmartPointer  independentSchedules_;

private:
  const int id_;
};

#endif
