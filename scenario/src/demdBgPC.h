#ifndef LGFR_DEMAND_BACKLOG_TIMES_GROSS_REV_PAIR_COMPARE_H
#define LGFR_DEMAND_BACKLOG_TIMES_GROSS_REV_PAIR_COMPARE_H

#include <scenario/src/attrSnPC.h>

// Comparison Strategy for sorting DemandSchedule's on 
// demand backlog volumes times gross revenue.
//
// This strategy sorts the ItemTimeVec's using the demand's backlog
// volume timeVec times the demand's gross revenue as
// the major sort key.  ItemTimeVec::compareTo is used as the minor
// sort key.


// ModelClients dependent on instances of this Model object will
// be notified when the backlog might have changed due to a new
// implosion solution.

// Forward References
class LgFrImplode;

class LgFrDemandBacklogTimesGrossRevenuePairCompare : 
  public LgFrAttributeSensitivePairCompareFloat {

public:

  // Compare two ItemTimeVecPairFloat's
  // return 0 if equal
  //        <0 if first less than second
  //        >0 if first greater than second
  virtual
    int
      itemTimeVecPairCompareFloat(
         const LgFrItemTimeVecPairFloat &,
         const LgFrItemTimeVecPairFloat &)
            const;

  // Make clone copy of this object
  virtual
     LgFrPairCompareFloat *
        clone() 
           const;

  // Make clone copy of this object with a new Scenario
  virtual
     LgFrPairCompareFloat *
        clone(LgFrScenario &) 
           const;

  // Return Comparison Strategy Identifier
  virtual
     isAReturnType
        isA()
           const;

  // Assignment operator
  LgFrDemandBacklogTimesGrossRevenuePairCompare &
    operator=(
	      const LgFrDemandBacklogTimesGrossRevenuePairCompare& rhs);

  // Copy constructor
  LgFrDemandBacklogTimesGrossRevenuePairCompare (
     const LgFrDemandBacklogTimesGrossRevenuePairCompare &);

  // Copy constructor with a new Scenario
  LgFrDemandBacklogTimesGrossRevenuePairCompare (
     const LgFrDemandBacklogTimesGrossRevenuePairCompare &,
     LgFrScenario &);

  // Constructor
  LgFrDemandBacklogTimesGrossRevenuePairCompare (
     LgFrScenario & );

  // Destructor
  ~LgFrDemandBacklogTimesGrossRevenuePairCompare ();

  // test this class
  static void test();

  static
  void
  contextTest
    (
     LgFrScenario 	         & testScenario,
     LgFrWorkingImplosionProblem & wip,			// testScenario's 
     LgFrLastProblemSolved       & lps,			// testScenario's 
     LgFrSetOfDemands            & sod);		// testScenario's 

private:

   const LgFrImplode * implodePtr_;
   const int id_;

   // Default Constructor
   LgFrDemandBacklogTimesGrossRevenuePairCompare ();

};

#endif
