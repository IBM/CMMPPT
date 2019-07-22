#ifndef LGFR_PART_CATEGORY_PAIR_COMPARE_H
#define LGFR_PART_CATEGORY_PAIR_COMPARE_H

#include <scenario/src/attrSnPC.h>

// Comparison Strategy for sorting PartSchedule's.
//
// This strategy sorts the ItemTimeVec's using the part category as
// the major sort key.  ItemTimeVec::compareTo is used as the minor
// sort key.

// The order of the part category is the order in enum LgFrPartCategory.

class LgFrPartCategoryPairCompareFloat : 
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
  LgFrPartCategoryPairCompareFloat &
    operator=(
	      const LgFrPartCategoryPairCompareFloat& rhs);

  // Copy constructor
  LgFrPartCategoryPairCompareFloat (
     const LgFrPartCategoryPairCompareFloat &);

  // Copy constructor with new Scenario
  LgFrPartCategoryPairCompareFloat (
     const LgFrPartCategoryPairCompareFloat &, 
     LgFrScenario &);

  // Constructor
  LgFrPartCategoryPairCompareFloat (
     LgFrScenario & );

  // Destructor
  ~LgFrPartCategoryPairCompareFloat ();

  // test this class
  static void test();

  // test this class
  // Expects the data in testScenario to come from
  // "/u/implode/wit/data/examples/diner12/wit.data"
  static
  void
  contextTest
    (LgFrScenario               & testScenario,
     LgFrLastProblemSolved      & lps,			// testScenario's 
     LgFrSetOfParts             & sop);			// testScenario's 

private:

  const int id_;

  // Default Constructor
  LgFrPartCategoryPairCompareFloat ();

};

#endif
