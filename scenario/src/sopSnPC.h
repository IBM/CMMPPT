#ifndef LGFR_SET_PARTS_SENSITIVE_PAIR_COMPARE_H
#define LGFR_SET_PARTS_SENSITIVE_PAIR_COMPARE_H

#include <scenario/src/pairComp.h>
#include <scenario/src/idnumber.h>
#include <scenario/src/sortSchP.h>

// This is a Class for implementing 
// Comparison Strategies used for sorting
// LgFrSortingSchedule.

// This class is used when sorting ItemTimeVecPairs.
// This class has is associated with a set of parts.
// Membership in this set of parts used as the major sort key. 

// Forward references
class LgFrSortingScheduleFloat;
class LgFrSortingScheduleFloatSmartPointerToConst;
class LgFrSortingScheduleFloatSmartPointer;
class LgFrTimeVecCompareFloat;
class LgFrSortingPartScheduleFloat;
class LgFrSortingPartScheduleFloatSmartPointerToConst;
class LgFrSortingPartScheduleFloatSmartPointer;

class LgFrSetOfPartsSensitivePairCompareFloat : 
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
           const;
           
  // Set Set Of Parts.
  // A pointer to this sop is kept by this object.
  // The life of the sop must be greater than the life of this object.
  virtual
     void
        setOfParts(
           const LgFrSetOfParts & );

  // Get Set Of Parts
  virtual
     const LgFrSetOfParts &
        setOfParts()
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
  LgFrSetOfPartsSensitivePairCompareFloat &
    operator=(
	      const LgFrSetOfPartsSensitivePairCompareFloat& rhs);

  // Copy constructor
  LgFrSetOfPartsSensitivePairCompareFloat (
     const LgFrSetOfPartsSensitivePairCompareFloat &);

  // Copy constructor with new Scenario
  LgFrSetOfPartsSensitivePairCompareFloat (
     const LgFrSetOfPartsSensitivePairCompareFloat &,
     LgFrScenario &);

  // Constructor
  LgFrSetOfPartsSensitivePairCompareFloat ();

  // Destructor
  ~LgFrSetOfPartsSensitivePairCompareFloat ();

  // test this class
  static void test();

private:

  const LgFrSetOfParts * setOfPartsPtr_;
  const isAReturnType          id_;

#if 0
  // Copy constructor used by clone
  LgFrSetOfPartsSensitivePairCompareFloat (
     const LgFrSetOfPartsSensitivePairCompareFloat*);
#endif  
};

#endif
