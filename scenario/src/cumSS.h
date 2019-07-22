#ifndef LGFR_CUMULATIVE_SCHEDULE_STRATEGY_H
#define LGFR_CUMULATIVE_SCHEDULE_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/schdSnSS.h>

// This Schedule Strategy is sensitive to another
// schedule.  A cumulative schedule is made from
// the "sensitive to" or independent schedule.
//
// The 4 kinds of cumulative schedule strategies
// defined here are:
//
// LgFrCumTimeVecPartScheduleStrategy:
//    The independent schedule is a part schedule.
//    The resulting dependent schedule is a part schedule with the
//    timeVecs cumulated.
//
//    for example:
//      indepenent schedule: part1  1  2  3
//                           part2  1  1  1
//                           part3  1  3  5
//
//      dependent schedule:  part1  1  3  6
//                           part2  1  2  3
//                           part3  1  4  9
//
// LgFrCumTimeVecDemandScheduleStrategy:
//    The independent schedule is a Demand schedule.
//    The resulting dependent schedule is a Demand schedule with the
//    timeVecs cumulated.
//
//    for example:
//      indepenent schedule: demand1 part1  1  2  3
//                           demand2 part1  1  1  1
//                           demand3 part2  1  3  5
//
//      dependent schedule:  demand1 part1  1  3  6
//                           demand2 part1  1  2  3
//                           demand3 part2  1  4  9
//
// LgFrCumDemandsPartScheduleStrategy:
//    The independent schedule is a demand schedule.
//    The resulting dependent schedule is a part schedule with the
//    the demands for the same part summed.
//
//    for example:
//      indepenent schedule: demand1 part1  1  2  3
//                           demand2 part1  1  1  1
//                           demand3 part2  1  3  5
//
//      dependent schedule:  part1  2  3  4
//                           part2  1  3  5
//
// LgFrPartSummaryScheduleStrategy:
//    The independent schedule is a part schedule.
//    The resulting dependent schedule is a part schedule such that
//    every itemTimeVec in it has the sum by part of all the timevecs.
//    You should only have one part in this schedule since any more will
//    have the same data.  It is used to cumulate an entire  part
//    schedule into one timevec.  Hence the one part in this schedule
//    should be a dummy part.
//
//    for example:
//      indepenent schedule: part1  1  2  3
//                           part1  1  1  1
//                           part2  1  3  5
//
//      dependent schedule:  partDummy  3  6  9
//      where partDummy is from the setOfParts associated with this
//      schedule.
//
// LgFrDemandSummaryScheduleStrategy:
//    Analogous to  LgFrPartSummaryScheduleStrategy except part is replaced
//    with demand.
//
// LgFrCompundInterestScheduleStrategy:
//    The independent schedule is a part schedule that (should) represent some
//    dollar amounts.  A private data member holds the interest rate and 
//    the resulting dependent schedule is a part schedule with the
//    timeVecs such that if part x is in the schedule then TV(x,t) = the compound interest 
//    at time t by investing dependentTV(x,i) in period i, for i=0,1,...,t.

// This class follows the Strategy Pattern

// Forward References
class LgFrSortingScheduleFloat;
class LgFrSortingScheduleFloatSmartPointerToConst;
class LgFrSortingScheduleFloatSmartPointer;

class LgFrSortingPartScheduleFloatSmartPointerToConst;
class LgFrSortingPartScheduleFloatSmartPointer;

class LgFrSortingDemandScheduleFloatSmartPointerToConst;
class LgFrSortingDemandScheduleFloatSmartPointer;


//-----------------------------------------------------------------------
//
//  Definition of LgFrCumTimeVecPartScheduleStrategy
//
//-----------------------------------------------------------------------

class LgFrCumTimeVecPartScheduleStrategy : 
   public LgFrPartScheduleSensitiveScheduleStrategy {

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

  // Make clone copy of this object with new Scenario
  virtual
     LgFrAttributeScheduleStrategy *
        clone(LgFrScenario &) 
           const;

  // Assignment operator
  LgFrCumTimeVecPartScheduleStrategy &
    operator=(
	      const LgFrCumTimeVecPartScheduleStrategy& rhs);

  // Copy constructor
  LgFrCumTimeVecPartScheduleStrategy (
     const LgFrCumTimeVecPartScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrCumTimeVecPartScheduleStrategy (
     const LgFrCumTimeVecPartScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrCumTimeVecPartScheduleStrategy (
     LgFrScenario &);

  // Destructor
  virtual
     ~LgFrCumTimeVecPartScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrCumTimeVecPartScheduleStrategy methods.
  // Expects data from both testScenario to come from
  // /u/implode/wit/data/examples/diner12/wit.data
  static
  void
  contextTest (LgFrScenario                & testScnario,
               LgFrSortingPartScheduleFloatSmartPointer supply);
private:

  const int id_;

  // Default Constructor
  LgFrCumTimeVecPartScheduleStrategy ();
};

//---------------------------------------------------------------------------
//
//    Description of LgFrCumTimeVecDemandScheduleStrategy
//
//---------------------------------------------------------------------------

class LgFrCumTimeVecDemandScheduleStrategy : 
   public LgFrDemandScheduleSensitiveScheduleStrategy {

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

  // Make clone copy of this object with new Scenario
  virtual
     LgFrAttributeScheduleStrategy *
        clone(LgFrScenario &) 
           const;

  // Assignment operator
  LgFrCumTimeVecDemandScheduleStrategy &
    operator=(
	      const LgFrCumTimeVecDemandScheduleStrategy& rhs);

  // Copy constructor
  LgFrCumTimeVecDemandScheduleStrategy (
     const LgFrCumTimeVecDemandScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrCumTimeVecDemandScheduleStrategy (
     const LgFrCumTimeVecDemandScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrCumTimeVecDemandScheduleStrategy (
     LgFrScenario &);

  // Destructor
  virtual
     ~LgFrCumTimeVecDemandScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrCumTimeVecDemandScheduleStrategy methods.
  // Expects data from both testScenario to come from
  // /u/implode/wit/data/examples/diner12/wit.data
  static
  void
  contextTest (LgFrScenario                & testScnario,
               LgFrSortingDemandScheduleFloatSmartPointer demand);
private:

  const int id_;

  // Default Constructor
  LgFrCumTimeVecDemandScheduleStrategy ();
};

//-----------------------------------------------------------------------
//
//  Definition of LgFrCumDemandsPartScheduleStrategy
//
//-----------------------------------------------------------------------

class LgFrCumDemandsPartScheduleStrategy : 
   public LgFrPartScheduleSensitiveScheduleStrategy {

public:

  // Populate Schedule with itemTimeVecs
  virtual
     void
        kernalOfPopulate(
           LgFrSortingScheduleFloat& )
              const;
  
  // Abort if this method is used
  virtual
     void
        insertPartSchedule(
           LgFrSortingPartScheduleFloatSmartPointer );

  // Abort if this method is used more than once
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
  LgFrCumDemandsPartScheduleStrategy &
    operator=(
	      const LgFrCumDemandsPartScheduleStrategy& rhs);

  // Copy constructor
  LgFrCumDemandsPartScheduleStrategy (
     const LgFrCumDemandsPartScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrCumDemandsPartScheduleStrategy (
     const LgFrCumDemandsPartScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrCumDemandsPartScheduleStrategy (
     LgFrScenario &);

  // Destructor
  virtual
     ~LgFrCumDemandsPartScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrCumDemandsPartScheduleStrategy methods.
  // Expects data from both testScenario to come from
  // /u/implode/wit/data/brenda/denise/wit.data
  static
  void
  contextTest (LgFrScenario                & testScnario,
               LgFrSortingDemandScheduleFloatSmartPointer demand);
private:

  const int id_;

  // Default Constructor
  LgFrCumDemandsPartScheduleStrategy ();
};


//-----------------------------------------------------------------------
//
//  Definition of LgFrPartSummaryScheduleStrategy
//
//-----------------------------------------------------------------------

class LgFrPartSummaryScheduleStrategy : 
   public LgFrPartScheduleSensitiveScheduleStrategy {

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

  // Make clone copy of this object with new Scenario
  virtual
     LgFrAttributeScheduleStrategy *
        clone(LgFrScenario &) 
           const;

  // Assignment operator
  LgFrPartSummaryScheduleStrategy &
    operator=(
	      const LgFrPartSummaryScheduleStrategy& rhs);

  // Copy constructor
  LgFrPartSummaryScheduleStrategy (
     const LgFrPartSummaryScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrPartSummaryScheduleStrategy (
     const LgFrPartSummaryScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrPartSummaryScheduleStrategy (
     LgFrScenario &);

  // Destructor
  virtual
     ~LgFrPartSummaryScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrPartSummaryScheduleStrategy methods.
  // Expects data from both testScenario to come from
  // /u/implode/wit/data/examples/diner12/wit.data
  static
  void
  contextTest (LgFrScenario                & testScnario,
               LgFrSortingPartScheduleFloatSmartPointer supply);

protected:

  // Estimate size of schedule using this ass.
  // The populate method uses this to set an initial size of the
  // schedule.
  virtual
     size_t
        estimatedSize(
           const LgFrSortingScheduleFloat& )
              const;

  // Default Constructor
  LgFrPartSummaryScheduleStrategy ();

private:

  const int id_;

};

//-----------------------------------------------------------------------
//
//  Definition of LgFrDemandSummaryScheduleStrategy
//
//-----------------------------------------------------------------------

class LgFrDemandSummaryScheduleStrategy : 
   public LgFrDemandScheduleSensitiveScheduleStrategy {

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

  // Make clone copy of this object with new Scenario
  virtual
     LgFrAttributeScheduleStrategy *
        clone(LgFrScenario &) 
           const;

  // Assignment operator
  LgFrDemandSummaryScheduleStrategy &
    operator=(
	      const LgFrDemandSummaryScheduleStrategy& rhs);

  // Copy constructor
  LgFrDemandSummaryScheduleStrategy (
     const LgFrDemandSummaryScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrDemandSummaryScheduleStrategy (
     const LgFrDemandSummaryScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrDemandSummaryScheduleStrategy (
     LgFrScenario &);

  // Destructor
  virtual
     ~LgFrDemandSummaryScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrDemandSummaryScheduleStrategy methods.
  // Expects data from both testScenario to come from
  // /u/implode/wit/data/examples/diner12/wit.data
  static
  void
  contextTest (LgFrScenario                & testScnario,
               LgFrSortingDemandScheduleFloatSmartPointer supply);

protected:

  // Estimate size of schedule using this ass.
  // The populate method uses this to set an initial size of the
  // schedule.
  virtual
     size_t
        estimatedSize(
           const LgFrSortingScheduleFloat& )
              const;


private:

  const int id_;

  // Default Constructor
  LgFrDemandSummaryScheduleStrategy ();
};


//-----------------------------------------------------------------------
//
//  Definition of LgFrCompoundInterestScheduleStrategy
//
//-----------------------------------------------------------------------

class LgFrCompoundInterestScheduleStrategy : 
   public LgFrPartScheduleSensitiveScheduleStrategy {

public:

  // Populate Schedule with itemTimeVecs
  virtual
     void
        kernalOfPopulate(
           LgFrSortingScheduleFloat& )
              const;

  // Set the interestRate
  virtual
     void
        interestRate(float theRate);

  // Get the interestRate
  virtual
     float
        interestRate()
           const;
     
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
  LgFrCompoundInterestScheduleStrategy &
    operator=(
	      const LgFrCompoundInterestScheduleStrategy& rhs);

  // Copy constructor
  LgFrCompoundInterestScheduleStrategy (
     const LgFrCompoundInterestScheduleStrategy &);

  // Copy constructor with new Scenario
  LgFrCompoundInterestScheduleStrategy (
     const LgFrCompoundInterestScheduleStrategy &,
     LgFrScenario &);

  // Constructor
  LgFrCompoundInterestScheduleStrategy (
     LgFrScenario &);

  // Destructor
  virtual
     ~LgFrCompoundInterestScheduleStrategy ();

  // test this class
  static void test();

  // test LgFrCompoundInterestScheduleStrategy methods.
  // Expects data from both testScenario to come from
  // /u/implode/wit/data/examples/diner12/wit.data
  static
  void
  contextTest (LgFrScenario                & testScnario,
               LgFrSortingPartScheduleFloatSmartPointer supply);
private:

float interestRate_;
  const int id_;

  // Default Constructor
  LgFrCompoundInterestScheduleStrategy ();
};

#endif
