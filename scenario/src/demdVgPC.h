#ifndef LGFR_DEMAND_VOLUME_TIMES_GROSS_REV_PAIR_COMPARE_H
#define LGFR_DEMAND_VOLUME_TIMES_GROSS_REV_PAIR_COMPARE_H

#include <scenario/src/attrSnPC.h>

// Comparison Strategy for sorting DemandSchedule's on 
// demand volumes times gross revenue.
//
// This strategy sorts the ItemTimeVec's using the demand's 
// volume timeVec times the demand's gross revenue as
// the major sort key.  ItemTimeVec::compareTo is used as the minor
// sort key.

// Forward declarations
class LgFrSortingDemandScheduleFloat;
class LgFrSortingDemandScheduleFloatSmartPointer;

class LgFrDemandVolumeTimesGrossRevenuePairCompare : 
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

  // Make clone copy of this object with new Scenario
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
  LgFrDemandVolumeTimesGrossRevenuePairCompare &
    operator=(
	      const LgFrDemandVolumeTimesGrossRevenuePairCompare& rhs);

  // Copy constructor
  LgFrDemandVolumeTimesGrossRevenuePairCompare (
     const LgFrDemandVolumeTimesGrossRevenuePairCompare &);

  // Copy constructor with new Scenario
  LgFrDemandVolumeTimesGrossRevenuePairCompare (
     const LgFrDemandVolumeTimesGrossRevenuePairCompare &,
     LgFrScenario & );

  // Constructor
  LgFrDemandVolumeTimesGrossRevenuePairCompare (
     LgFrScenario & );

  // Destructor
  ~LgFrDemandVolumeTimesGrossRevenuePairCompare ();

  // test this class
  static void test();

  // test LgFrDemandBacklogTimesGrossRevenuePairCompare methods.
  // Expects the data in testScenario to come from
  // "/u/implode/wit/data/examples/diner12/wit.data"
  static
  void
  contextTest
    (LgFrScenario 	         & testScenario,
     LgFrLastProblemSolved          & lps,		// testScenario's 
     LgFrSetOfDemands               & sod,		// testScenario's
     LgFrSortingDemandScheduleFloatSmartPointer demandSchedule);	// testScenario's

private:

  const int id_;

  // Default Constructor
  LgFrDemandVolumeTimesGrossRevenuePairCompare ();

};

#endif
