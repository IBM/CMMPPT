#ifndef LGFR_DEMAND_PARTNAME_PAIR_COMPARE_H
#define LGFR_DEMAND_PARTNAME_PAIR_COMPARE_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/pairComp.h>

// Comparison Strategy for SortingSchedule's.
//
// This strategy sorts demand itemTimeVec's on part name and then demand name.

// This class follows the Strategy Pattern


class LgFrDemandPartNamePairCompare : public LgFrPairCompareFloat {
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

  // Assignment operator
  LgFrDemandPartNamePairCompare &
    operator=(
	      const LgFrDemandPartNamePairCompare& rhs);

  // Copy constructor
  LgFrDemandPartNamePairCompare (
     const LgFrDemandPartNamePairCompare &);

  // Copy constructor with new Scenario
  LgFrDemandPartNamePairCompare (
     const LgFrDemandPartNamePairCompare &,
     LgFrScenario &);

  // Default Constructor
  LgFrDemandPartNamePairCompare ();

  // Destructor
  ~LgFrDemandPartNamePairCompare ();

  // test this class
  static void test();

private:

  const int id_;
};

#endif
