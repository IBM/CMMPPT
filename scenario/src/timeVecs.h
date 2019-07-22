#ifndef LgFrTimVecs_h
#define LgFrTimVecs_h
// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
#include <vector>
// End of RW2STL -- inserted includes.

// All of the time vector classes should be defined here

//#include <strstrea.h>

#include <scenario/src/generVc.h>

#if defined (__OS2__) || defined (WIN32) || defined (__WINDOWS__)
#undef max
#undef min
#endif 

enum LgFrSchedulePrintStyle
  {
    TEXT,		// Blank-separated variables, aka TEXT or TXT
    CSV 		// Comma-separated variables
  };


class LGFR_DECLSPEC LgFrTimeVecFloat  {

private:

  static const LgFrTimeVecFloat emptyVec; // Used for default arg because SUN
                                          // compiler fails when passed
                                          // LgFrTimeVecFloat()

public:

   // Return vector length
   size_t 
      length() 
         const;  

   //  Change length of vector
   void 
      reshape(
         size_t n);   

   // compareTo method.
   // should return 0  if each element of *this "is equal to" source
   //               >0 if *this "is larger" than source.
   //               <0 if *this "is smaller" than source.
   //
   //               Each element of the vectors are compared starting
   //               at the 0'th.  If the elements are within .00001
   //               of each other, then they are said to be equal.
   //
   //               0  if all elements are equal.
   //
   //               If t is the first element which is not equal then 
   //               >0 if this[t] > source[t]
   //               <0 if this[t] < source[t]
   virtual int compareTo(const LgFrTimeVecFloat &) const;

   // Perform diadic vector operation.
   // Returned vector is: 
   //    result[t] = this[t] "scalarOp" source[t]
   // where t is 0,1,...,length of vectors
   //     "scalarOp" is function which has 2 float parameter and returns float
   //
   // This function allows the client to define operations on TimeVecFloat's.
   // For example:
   //
   //   float userFunc(const float a, const float b) const {
   //      return ( 2.0*a + b );
   //   }
   //   LgFrTimeVecFloat tv1(5,3.0), tv2(5,4.0), tv3;
   //   tv3 = tv1.op(tv2,userFunc);
   //   assert( tv3 == ( 2.0*tv1 + tv2 ) );
   //
   LgFrTimeVecFloat  
   op(
      const LgFrTimeVecFloat& source,
      float (*scalarOp) ( const float, const float) )
   const;

   // scan method.
   // Named after APL scan operator \.
   //
   // Returned vector is:
   //      result[0] = this[0]
   //      result[1] = this[0] "scalarOp" this[1]
   //      result[2] = this[0] "scalarOp" this[1] "scalarOp" this[2]
   //      ...
   //
   // This method allows clients to form cumlative timeVecs.
   //
   // For example:
   //    float v   []={ 1., 3., 5.,  7., 11., 13. }
   //    float cumv[]={ 1., 4., 9., 16., 27., 40. }
   //    LgFrTimeVecFloat    vec(6,   v);
   //    LgFrTimeVecFloat cumVec = vec.scan(LgFrAdd);
   //    assert( cumVec == LgFrTimeVecFloat(6,cumv) ); 
   //
   LgFrTimeVecFloat  
   scan(
      float (*scalarOp) ( const float, const float) )
   const;

   // reduce method.
   // Named after APL reduce operator /.
   //
   // Original purpose is to be used to implement methods
   // for returning the sum of a timeVec and the maximum value of a timeVec.
   //
   // Returned float is:
   //      result = this[0] "scalarOp" this[1] "scalarOp" this[2] ...
   //
   // For example:
   //    float v   []={ 1., 3., 5.,  7., 11., 13. }
   //    LgFrTimeVecFloat    vec(6,   v);
   //    assert( fabs( vec.reduce(LgFrAdd) - 40.0 ) <= fltEps  ); 
   //    assert( fabs( vec.reduce(LgFrFloatMax) - 13.0 ) <= fltEps  ); 
   //
   float  
   reduce(
      float (*scalarOp) ( const float, const float) )
   const;

   // Addition
   LgFrTimeVecFloat
   add(
      const LgFrTimeVecFloat & source )
   const;

   // Subtraction
   LgFrTimeVecFloat
   subtract(
      const LgFrTimeVecFloat & source )
   const;

   // Multiplication
   LgFrTimeVecFloat
   multiply(
      const LgFrTimeVecFloat & source )
   const;

   // Division
   LgFrTimeVecFloat
   divide(
      const LgFrTimeVecFloat & source )
   const;

   // Maximum
   // returnedVector[i] = maximum( (*this)[i], source[i] );
   LgFrTimeVecFloat
   maximum(
      const LgFrTimeVecFloat & source )
   const;


   // undoCum
   // If *this is:   1 3 6 10
   // then return:   1 2 3 4
   LgFrTimeVecFloat
     undoCum()
       const;

   // Percentage Of, with 0/0 being defined as 0%
   LgFrTimeVecFloat
   percentageOf(
      const LgFrTimeVecFloat& rhs)
   const;

   // cumulativeNet ("net out" another timevec of data)
   // This is designed for the "netting" of real order demands
   // from a forecast demand.  The resulting timeVec is non-negative
   // and any net quatnity that is greater than what's available is
   // rolled to the next period.
   LgFrTimeVecFloat
   cumulativeNet(
      const LgFrTimeVecFloat & consumerTVF )
   const;

   // Net qty out of elements of *this that are after element
   // at startIndex.  (The element at startIndex is not effected.)
   // This may change both *this and the qty argument.
   // It handles epsilons carefully.
   void
   takeQtyFromPositiveElementsAfterIndex (float & qty,
					  const size_t startIndex = -1,
					  const float fltEps = .0001);

   // applyOffset (Apply a cycletime or lead time offset)
   LgFrTimeVecFloat
   applyOffset(
      const LgFrTimeVecFloat & offsetFTV )
   const;

   // returnRHS 
   LgFrTimeVecFloat
   returnRHS(
      const LgFrTimeVecFloat & source )
   const;

   // Sum. Return sum of all elements in timeVec.
   float
      sum()
         const;

   // Maximum. Return value of maximum element in timeVec
   float
      max()
         const;

   // Minimum. Return value of minimum element in timeVec
   float
      min()
         const;

   // data. Provides access to the timeVec represented as a "float *".
   // This datum is owned by LgFrTimeVecFloat and may not be deleted or
   // changed.  If the LgFrTimeVecFloat itself changes or goes out of scope,
   // the pointer value previously returned is invalid.
   // The length of the returned "float *" can be obtained with the length()
   // member function.
   const float *
      data()
         const;

  // intData. Similar to data() method except that an "int *" is returned.
  const int *
     intData()
        const;

   // Destructor
   ~LgFrTimeVecFloat();

   // Default Constructor.  
   // Set initial length of vector.
   // Set initial value of each vector element.
   LgFrTimeVecFloat(const size_t n = 0, const float val = 0.0 );

   // Another constructor.
   // len: length of vector
   // valVec: c style float vector containing inital values
   LgFrTimeVecFloat(const size_t n, const float * const valVec );

   // Yet Another constructor.
   // len: length of vector
   // stringVec: std::string representing a vector of floats
   // ws: White space list for parsing
   // Uses SCETokenizer and RWCLocale to parse string.  Result will be
   // padded with 0.0's or truncated as needed.
   LgFrTimeVecFloat(const size_t len, const std::string& stringVec, 
           const char * ws = " *\t\n");

   // Copy Constructor
   LgFrTimeVecFloat( const LgFrTimeVecFloat & );

   // Assignment operator
   virtual 
      LgFrTimeVecFloat & operator=(const LgFrTimeVecFloat & rhs);
   virtual 
      LgFrTimeVecFloat & operator=(const float & rhs);

   // Return i'th element
   virtual 
      float  operator[](const size_t i) const;
   virtual 
      float& operator[](const size_t i);

   // Format a timeVec for display.
   // Return a string representing the TimeVecFloat.
   //
   // highLight:
   //   If highLight[i] != 0.0 then the i'th number in the returned
   //   string is emphasized by following the number with an "*",
   //   or a client specified character.
   //
   // highLightFlag: character to follow a highlighted vector entry.
   //
   // width: ios width used to format each number in vector.
   //
   // precision: ios precision used to format each number in vector.
   //
   // iosFlags: ios formating flags used to format each number in vector.
   //

   std::string format( 
      const LgFrTimeVecFloat & highLight = LgFrTimeVecFloat::emptyVec,
      const char highLightFlag = '*',
      const int width = 10,
      const int precision = 1,
      const std::ios_base::fmtflags iosFlags = std::ios_base::fixed | std::ios_base::showpoint | std::ios_base::right )
     const;

   // format timeVec as a comma seperated variable string.
   std::string formatAsCsv() const;

   // test this class
   static void test();

private:

   typedef enum { FULL, CONSTANT } StorageMethod;

   float *    dataPtr_;
   int   * intDataPtr_;
   StorageMethod storageMethod_;
   LgFrFloatVector * vecValPtr_;
   float constant_;
   int length_;

};

// Addition Operator
LGFR_DECLSPEC LgFrTimeVecFloat 
   operator+ (const LgFrTimeVecFloat& lhs, const LgFrTimeVecFloat& rhs);
LGFR_DECLSPEC LgFrTimeVecFloat 
   operator+ (const LgFrTimeVecFloat& lhs, const float& rhs);
LGFR_DECLSPEC LgFrTimeVecFloat 
   operator+ (const float& lhs,            const LgFrTimeVecFloat& rhs);

// Subtraction Operator
LGFR_DECLSPEC LgFrTimeVecFloat 
   operator- (const LgFrTimeVecFloat& lhs, const LgFrTimeVecFloat& rhs);
LGFR_DECLSPEC LgFrTimeVecFloat 
   operator- (const LgFrTimeVecFloat& lhs, const float& rhs);
LGFR_DECLSPEC LgFrTimeVecFloat 
   operator- (const float& lhs,            const LgFrTimeVecFloat& rhs);

// Multiplication Operator
LGFR_DECLSPEC LgFrTimeVecFloat 
   operator* (const LgFrTimeVecFloat& lhs, const LgFrTimeVecFloat& rhs);
LGFR_DECLSPEC LgFrTimeVecFloat 
   operator* (const LgFrTimeVecFloat& lhs, const float& rhs);
LGFR_DECLSPEC LgFrTimeVecFloat 
   operator* (const float& lhs,            const LgFrTimeVecFloat& rhs);

// Division Operator
LGFR_DECLSPEC LgFrTimeVecFloat 
   operator/ (const LgFrTimeVecFloat& lhs, const LgFrTimeVecFloat& rhs);
LGFR_DECLSPEC LgFrTimeVecFloat 
   operator/ (const LgFrTimeVecFloat& lhs, const float& rhs);
LGFR_DECLSPEC LgFrTimeVecFloat 
   operator/ (const float& lhs,            const LgFrTimeVecFloat& rhs);

// Shift Operators.  These do zero-filling.  If you need something else,
// You can implement it with this and operator+
LGFR_DECLSPEC LgFrTimeVecFloat 
   operator<< (const LgFrTimeVecFloat& lhs, const int& rhs);
LGFR_DECLSPEC LgFrTimeVecFloat 
   operator>> (const LgFrTimeVecFloat& lhs, const int& rhs);

// Equality
LGFR_DECLSPEC int operator==(const LgFrTimeVecFloat& lhs, const LgFrTimeVecFloat& rhs);
LGFR_DECLSPEC int operator==(const LgFrTimeVecFloat& lhs, const float& rhs);
LGFR_DECLSPEC int operator==(const float& lhs,            const LgFrTimeVecFloat& rhs);

// Inequality
LGFR_DECLSPEC int operator!=(const LgFrTimeVecFloat& lhs, const LgFrTimeVecFloat& rhs);
LGFR_DECLSPEC int operator!=(const LgFrTimeVecFloat& lhs, const float& rhs);
LGFR_DECLSPEC int operator!=(const float& lhs,            const LgFrTimeVecFloat& rhs);

// Less than
LGFR_DECLSPEC int operator< (const LgFrTimeVecFloat& lhs, const LgFrTimeVecFloat& rhs);
LGFR_DECLSPEC int operator< (const LgFrTimeVecFloat& lhs, const float& rhs);
LGFR_DECLSPEC int operator< (const float& lhs,            const LgFrTimeVecFloat& rhs);
LGFR_DECLSPEC int operator<=(const LgFrTimeVecFloat& lhs, const LgFrTimeVecFloat& rhs);
LGFR_DECLSPEC int operator<=(const LgFrTimeVecFloat& lhs, const float& rhs);
LGFR_DECLSPEC int operator<=(const float& lhs,            const LgFrTimeVecFloat& rhs);

// Greater than
LGFR_DECLSPEC int operator> (const LgFrTimeVecFloat& lhs, const LgFrTimeVecFloat& rhs);
LGFR_DECLSPEC int operator> (const LgFrTimeVecFloat& lhs, const float& rhs);
LGFR_DECLSPEC int operator> (const float& lhs,            const LgFrTimeVecFloat& rhs);
LGFR_DECLSPEC int operator>=(const LgFrTimeVecFloat& lhs, const LgFrTimeVecFloat& rhs);
LGFR_DECLSPEC int operator>=(const LgFrTimeVecFloat& lhs, const float& rhs);
LGFR_DECLSPEC int operator>=(const float& lhs,            const LgFrTimeVecFloat& rhs);

LGFR_DECLSPEC float LgFrAdd      ( const float, const float ); 
LGFR_DECLSPEC float LgFrSubtract ( const float, const float ); 
LGFR_DECLSPEC float LgFrMultiply ( const float, const float ); 
LGFR_DECLSPEC float LgFrDivide   ( const float, const float );
LGFR_DECLSPEC float LgFrAssign   ( const float, const float );


LGFR_DECLSPEC float minNonZeroFloat   ( const float, const float ); 

// RW2STL - declare (RWGVector, LgFrTimeVecFloat)

// RW2STL - declare (RWGOrderedVector, LgFrTimeVecFloat)

typedef std::vector<LgFrTimeVecFloat> LgFrTimeVecFloatVector; // RW2STL - typedef RWGVector(LgFrTimeVecFloat) LgFrTimeVecFloatVector;

typedef std::vector<LgFrTimeVecFloat> LgFrTimeVecFloatVector; // RW2STL - typedef RWGOrderedVector(LgFrTimeVecFloat) LgFrTimeVecFloatVector;


#endif
