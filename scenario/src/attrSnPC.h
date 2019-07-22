#ifndef LGFR_ATTRIBUTE_SENSITIVE_PAIR_COMPARE_H
#define LGFR_ATTRIBUTE_SENSITIVE_PAIR_COMPARE_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/pairComp.h>
#include <scenario/src/idnumber.h>

// This is an Abstract Base Class for implementing 
// Comparison Strategies used for sorting
// by LgFrSortingSchedule.

// This class is used when sorting on attributes of an ItemTimeVecPair
// rather than just the data stored in an ItemTimeVecPair.

// This class has member functions for obtaining the LastProblemSolved
// and WorkingImplosionProblem.  This provides access to the attribute
// data of the ItemTimeVecPair.

// Forward references
class LgFrLastProblemSolved;
class LgFrWorkingImplosionProblem;

class LgFrAttributeSensitivePairCompareFloat : 
   public LgFrPairCompareFloat    {
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
            const = 0;

  // Get LastProblemSolved
  virtual
     const LgFrLastProblemSolved &
        lastProblemSolved()
           const;

  // Get WorkingImplosionProblem
  virtual
     const LgFrWorkingImplosionProblem &
        workingImplosionProblem()
           const;

  // Make clone copy of this object
  virtual
     LgFrPairCompareFloat *
        clone() 
           const = 0;

  // Make clone copy of this object with a new Scenario
  virtual
     LgFrPairCompareFloat *
        clone(LgFrScenario &) 
           const = 0;

  // Assignment operator
  LgFrAttributeSensitivePairCompareFloat &
    operator=(
	      const LgFrAttributeSensitivePairCompareFloat& rhs);

  // Copy constructor
  LgFrAttributeSensitivePairCompareFloat (
     const LgFrAttributeSensitivePairCompareFloat &);

  // Copy constructor with new Scenario
  LgFrAttributeSensitivePairCompareFloat (
     const LgFrAttributeSensitivePairCompareFloat &,
     LgFrScenario &,
     LgFrModel * algorithmPtr = NULL );

  // Constructor
  LgFrAttributeSensitivePairCompareFloat (
     LgFrScenario &,
     LgFrModel * algorithmPtr = NULL );

  // Destructor
  ~LgFrAttributeSensitivePairCompareFloat ();

  // test this class
  // static void test();

protected:

  // Default Constructor
  // This constructor should not be used.  It is would be declared private,
  // but then there are problems implementing the derived classes private
  // default constructors.
  LgFrAttributeSensitivePairCompareFloat ();

private:

  const LgFrLastProblemSolved       * lpsPtr_;
  const LgFrWorkingImplosionProblem * wipPtr_;


};

#endif
