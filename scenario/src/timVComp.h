#ifndef LGFR_TIME_VEC_COMPARE_H
#define LGFR_TIME_VEC_COMPARE_H

#include <scenario/src/idnumber.h>

// Class for implementing methods of comparing time vectors.
// This class is used by the LgFrScheduleSensitivePairCompareStrategy class.


// Forward References
class LgFrTimeVecFloat;

// -------------------------------------------------------------------------
//
// LgFrTimeVecCompareFloat- base class.
// TimeVecs are compared using LgFrTimeVecFloat::compareTo method.
//
// -------------------------------------------------------------------------

class LgFrTimeVecCompareFloat {
public:

  // Compare two TimeVecPairFloat's
  // return 0 if equal
  //        <0 if first less than second
  //        >0 if first greater than second
  //
  // This is implemented as LgFrTimeVecFloat::compareTo in this base class.
  virtual
    int
      compare(
         const LgFrTimeVecFloat &,
         const LgFrTimeVecFloat &)
            const;

  // Make clone copy of this object
  virtual
     LgFrTimeVecCompareFloat *
        clone() 
           const;

  // Return Comparison Strategy Identifier
  virtual
     isAReturnType
        isA()
           const;

  // Assignment operator
  LgFrTimeVecCompareFloat &
    operator=(
	      const LgFrTimeVecCompareFloat& rhs);

  // Copy constructor
  LgFrTimeVecCompareFloat (
     const LgFrTimeVecCompareFloat &);

  // Default Constructor
  LgFrTimeVecCompareFloat ();

  // Destructor
  ~LgFrTimeVecCompareFloat ();

  // test this class
  static void test();

private:

  const int id_;
};


// -------------------------------------------------------------------------
//
// LgFrMaximumElementTimeVecCompareFloat- derived class.
// TimeVecs are compared by comparing their maximum element values.
//
// -------------------------------------------------------------------------

class LgFrMaximumElementTimeVecCompareFloat :
   public LgFrTimeVecCompareFloat{
public:

  // Compare two TimeVecPairFloat's
  // return 0 if maximum elements are equal
  //        <0 if maximum element of first less than second
  //        >0 if maximum element of first greater than second
  virtual
    int
      compare(
         const LgFrTimeVecFloat &,
         const LgFrTimeVecFloat &)
            const;

  // Make clone copy of this object
  virtual
     LgFrTimeVecCompareFloat *
        clone() 
           const;

  // Return Comparison Strategy Identifier
  virtual
     isAReturnType
        isA()
           const;

  // Assignment operator
  LgFrTimeVecCompareFloat &
    operator=(
	      const LgFrMaximumElementTimeVecCompareFloat& rhs);

  // Copy constructor
  LgFrMaximumElementTimeVecCompareFloat (
     const LgFrMaximumElementTimeVecCompareFloat &);

  // Default Constructor
  LgFrMaximumElementTimeVecCompareFloat ();

  // Destructor
  ~LgFrMaximumElementTimeVecCompareFloat ();

  // test this class
  static void test();

private:

  const int id_;
};


// -------------------------------------------------------------------------
//
// LgFrMinimumElementTimeVecCompareFloat- derived class.
// TimeVecs are compared by comparing their maximum element values.
//
// -------------------------------------------------------------------------

class LgFrMinimumElementTimeVecCompareFloat :
   public LgFrTimeVecCompareFloat{
public:

  // Compare two TimeVecPairFloat's
  // return 0 if maximum elements are equal
  //        <0 if maximum element of first less than second
  //        >0 if maximum element of first greater than second
  virtual
    int
      compare(
         const LgFrTimeVecFloat &,
         const LgFrTimeVecFloat &)
            const;

  // Make clone copy of this object
  virtual
     LgFrTimeVecCompareFloat *
        clone() 
           const;

  // Return Comparison Strategy Identifier
  virtual
     isAReturnType
        isA()
           const;

  // Assignment operator
  LgFrTimeVecCompareFloat &
    operator=(
	      const LgFrMinimumElementTimeVecCompareFloat& rhs);

  // Copy constructor
  LgFrMinimumElementTimeVecCompareFloat (
     const LgFrMinimumElementTimeVecCompareFloat &);

  // Default Constructor
  LgFrMinimumElementTimeVecCompareFloat ();

  // Destructor
  ~LgFrMinimumElementTimeVecCompareFloat ();

  // test this class
  static void test();

private:

  const int id_;
};

#endif
