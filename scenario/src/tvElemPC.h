#ifndef LGFR_TIME_VEC_ELEMENT_PAIR_COMPARE_H
#define LGFR_TIME_VEC_ELEMENT_PAIR_COMPARE_H

#include <scenario/src/pairComp.h>

// Comparison Strategy for SortingSchedule's.
//
// This strategy sorts on the value of a t'th elememt (period) of the timeVec.
// The value t can be set be a method in this class.

// This class follows the Strategy Pattern


class LgFrTimeVecElementPairCompareFloat : public LgFrPairCompareFloat {
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

  // Set the element number (period) to sort on.
  virtual
     void
        period( 
           const size_t p );

  // Get the element number (period) which is set to sort on.
  virtual
     size_t
        period()
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
  LgFrTimeVecElementPairCompareFloat &
    operator=(
	      const LgFrTimeVecElementPairCompareFloat& rhs);

  // Copy constructor
  LgFrTimeVecElementPairCompareFloat (
     const LgFrTimeVecElementPairCompareFloat &);

  // Copy constructor with new Scenario
  LgFrTimeVecElementPairCompareFloat (
     const LgFrTimeVecElementPairCompareFloat &,
     LgFrScenario &);

  // Default Constructor
  LgFrTimeVecElementPairCompareFloat ();

  // Destructor
  ~LgFrTimeVecElementPairCompareFloat ();

  // test this class
  static void test();

private:

  const int id_;
  size_t    period_;
};

#endif
