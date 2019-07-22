// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.
#include <math.h>

/*#if defined (WIN32) || defined (__OS2__) || defined (__WINDOWS__)
#include <strstrea.h>
#else
#include <strstream.h>
#endif*/

#include <sstream>

#include <scenario/src/timeVecs.h>
#include <scenario/src/machdep.h>

#if defined (WIN32) || defined (__OS2__) || defined (__WINDOWS__)
#undef max
#undef min
#endif

// RW2STL - implement (RWGVector, LgFrTimeVecFloat)

// RW2STL - implement (RWGOrderedVector, LgFrTimeVecFloat)



// Create and initialize emptyVec
const LgFrTimeVecFloat LgFrTimeVecFloat::emptyVec;

// Return vector length
size_t 
LgFrTimeVecFloat::length()
const 
{
   return length_;
}

void
LgFrTimeVecFloat::reshape(
  size_t n)
{
   length_ = n;
   if ( storageMethod_ == FULL ) vecValPtr_->reserve(n);
}

// compareTo method.
// should return 0  if each element of *this "is equal to" source
//               >0 if *this "is larger" than source.
//               <0 if *this "is smaller" than source.
//
//               Each element of the vectors are compared starting
//               at the 0'th. 
//               If t is the first element which is not equal then
//               >0 if this[t] > source[t]
//               <0 if this[t] < source[t]
int 
LgFrTimeVecFloat::compareTo(
const LgFrTimeVecFloat & source) 
const
{
   const float fltEps = .00001f;
   assert( this->length() == source.length() );
   int i = 0; // Pulled out of the for below by RW2STL
   for ( i=0; i<this->length(); i++ )
       if ( fabs((*this)[i] - source[i]) >= fltEps )
          return ( (*this)[i] > source[i] ? 1 : -1);
   return 0;
}


// data().
const float *
LgFrTimeVecFloat::data()
const
{
   // Make a local version of this that's not a pointer-to-const.
   // localThis cast's away const-ness.  This allows data() to be invoked
   // on "const LgFrTimeVecFloat" object.
   // See Effective C++, Scot Meyers, item 21, page 77.
   LgFrTimeVecFloat * const localThis = (LgFrTimeVecFloat * const) this;

   delete [] dataPtr_;
   localThis->dataPtr_ = new float[ this->length() ];
   size_t i = 0; // Pulled out of the for below by RW2STL
   for ( i=0; i<this->length(); i++ ) dataPtr_[i] = (*this)[i];
   return dataPtr_;      
}

// intData().
const int *
LgFrTimeVecFloat::intData()
const
{
   // Make a local version of this that's not a pointer-to-const.
   // localThis cast's away const-ness.  This allows data() to be invoked
   // on "const LgFrTimeVecFloat" object.
   // See Effective C++, Scot Meyers, item 21, page 77.
   LgFrTimeVecFloat * const localThis = (LgFrTimeVecFloat * const) this;

   delete [] intDataPtr_;
   localThis->intDataPtr_ = new int[ this->length() ];
   size_t i = 0; // Pulled out of the for below by RW2STL
   for ( i=0; i<this->length(); i++ ) {
     float val = (*this)[i];
     // Test for division by zero
     if ( val == 0.0 )
       intDataPtr_[i] = 0;
     else
       intDataPtr_[i] = (int) (val + ( fabs(val) / val ) * .5);
   }   
   return intDataPtr_;      
}


// Destructor
LgFrTimeVecFloat::~LgFrTimeVecFloat()
{
   delete [] dataPtr_;
   delete [] intDataPtr_;
   delete vecValPtr_;
}


// Constructor, including default
LgFrTimeVecFloat::LgFrTimeVecFloat(
   const size_t n,
   const float  val) 
: 
  dataPtr_(0),
  intDataPtr_(0),
  storageMethod_(CONSTANT),
  vecValPtr_(0),
  constant_(val),
  length_(n)
{
   // Nothing to do here
}

// Another Constructor
LgFrTimeVecFloat::LgFrTimeVecFloat(
   const size_t n, 
   const float * const valVec )
: 
  dataPtr_(0),
  intDataPtr_(0),
  storageMethod_(CONSTANT),
  vecValPtr_(0),
  constant_(0),
  length_(n)
{
   bool constant = true;
   // Check to see if this is a full storage vector
   if ( n>0 )  {
      float elemZero = valVec[0];
      size_t i = 0; // Pulled out of the for below by RW2STL
      for ( i=1; i<n; i++ ) {
         if ( elemZero != valVec[i] ) {
            constant = false;
            break;
         }
      }
   }
   if ( constant ) {
      storageMethod_ = CONSTANT;
      if (n>0) constant_ = valVec[0];
      else     constant_ = 0.0;
   }
   else {
      storageMethod_ = FULL;
      vecValPtr_ = new LgFrFloatVector;
      size_t i = 0; // Pulled out of the for below by RW2STL
      for ( i=0; i<n; i++ ) vecValPtr_->push_back( valVec[i] );
   }
}

// Yet Another Constructor
LgFrTimeVecFloat::LgFrTimeVecFloat(
   const size_t len, 
   const std::string& stringVec,
   const char * ws )
: 
  dataPtr_(0),
  intDataPtr_(0),
  storageMethod_(FULL),
  vecValPtr_(0),
  constant_(0),
  length_(len)
{

 SCETokenizer next(stringVec);
 std::string token;
 bool result;

 vecValPtr_ = new LgFrFloatVector(len,0.0);

 double f;
 size_t i = 0;
 bool constant = true;
 float vecValZero = 0.0;

 while ((i<len) && (! (token = next(ws)).empty()))   {
     result = sce::fromString( token, f ); 
    if (result == true) {
       if ( i==0 ) vecValZero = (float)f;
       (*vecValPtr_)[i] = (float)f;
       if ( vecValZero != f ) constant = false;
       i++;
       }

    }
 if ( i != len ) constant = false;

 // If vector is constant, then convert from full storage.
 if ( constant ) {
    delete vecValPtr_;
    vecValPtr_ = 0;
    constant_ = vecValZero;
    storageMethod_ = CONSTANT;
 }

}

// Copy Constructor
LgFrTimeVecFloat::LgFrTimeVecFloat(
   const LgFrTimeVecFloat &source)
: 
  dataPtr_(0),
  intDataPtr_(0),
  storageMethod_(source.storageMethod_),
  vecValPtr_(0),
  constant_(source.constant_),
  length_(source.length_)
{
   if ( source.vecValPtr_ != 0 ) 
      vecValPtr_ = new LgFrFloatVector( *(source.vecValPtr_) );
}

// Assignment operator. 
// Assignement when rhs is LgFrTimeVecFloat.
LgFrTimeVecFloat & 
LgFrTimeVecFloat::operator=(
const LgFrTimeVecFloat & rhs)
{
  if (this != &rhs) {           // Check for assignment to self

    delete [] dataPtr_;
    dataPtr_ = 0;

    delete [] intDataPtr_;
    intDataPtr_ = 0;
    
    storageMethod_ = rhs.storageMethod_;

    if ( vecValPtr_ != 0 ) {

      if ( rhs.vecValPtr_ != 0 ) {
        // vecValPtr_ & rhs.vecValPtr_ are not null
        assert( vecValPtr_->size() == length_ );
        assert( rhs.vecValPtr_->size() == rhs.length_ );
        if ( length_ == rhs.length_ ) {
          // OK to use storage already allocated to vecValPtr_
          *vecValPtr_ = *(rhs.vecValPtr_);
          }
        else {
          delete vecValPtr_;
          vecValPtr_ = new LgFrFloatVector( *(rhs.vecValPtr_) );
        }
      }
      else {
        // vecValPtr_ is not null, rhs.vecValPtr_ is null
        delete vecValPtr_;
        vecValPtr_ = 0;
      }

    }
    else  {

      if ( rhs.vecValPtr_ != 0 ) {
        // vecValPtr_ is null, rhs.vecValPtr_ is not null
        vecValPtr_ = new LgFrFloatVector( *(rhs.vecValPtr_) );
      }
      else {
        // vecValPtr_ & rhs.vecValPtr_ are null
        // Nothing needs to be done here
      }

    }

    constant_ = rhs.constant_;

    length_ = rhs.length_;

  }

  // Test assignment operator result
  assert (this->length() == rhs.length());

  return *this;
}

// Assignment operator
// Assignement when rhs is float.
LgFrTimeVecFloat & 
LgFrTimeVecFloat::operator=(
const float & rhs)
{

  delete [] dataPtr_;
  dataPtr_ = 0;

  delete [] intDataPtr_;
  intDataPtr_ = 0;
  
  storageMethod_ = CONSTANT;

  delete vecValPtr_;
  vecValPtr_ = 0;

  constant_ = rhs;

  return *this;
}

// Return the i'th element
float
LgFrTimeVecFloat::operator[]( const size_t i )
const
{
   if (storageMethod_ == CONSTANT ) return constant_;
   else return (*vecValPtr_)[i] ; 
}

float &
LgFrTimeVecFloat::operator[]( const size_t i )
{
   // If constant then switch to full storage method.
   if (storageMethod_ == CONSTANT ) {
      storageMethod_ = FULL;
      vecValPtr_ = new LgFrFloatVector( length_, constant_ );
      constant_ = 0.0;
   } 
   return (*vecValPtr_)[i] ; 
}


// Format a timeVec for display.
// Return a string representing the TimeVecFloat.
std::string 
LgFrTimeVecFloat::format(
   const LgFrTimeVecFloat & highLight,
   const char hlf,
   const int width,
   const int precision,
   const std::ios_base::fmtflags iosFlags /*= fixed | showpoint | right*/ )
const
{
  char highLightFlag;
  std::ostringstream buf;
  buf.precision(precision);
  buf.flags( iosFlags );

  // Loop once for each element of timeVec
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<length(); i++ ) {

     // format number
     buf.width( width );
     buf << operator[]( i );

     // Add highlighting flag
     highLightFlag = ' ';
     if ( i<highLight.length() )
        if ( highLight[i] != 0.0 ) highLightFlag = hlf;
     buf << highLightFlag;

  }

  std::string retVal( buf.str() );

  return retVal;
}

std::string LgFrTimeVecFloat::formatAsCsv() const
{
  std::ostringstream buf;
  
  // make sure output does not use scientific notation
  std::fixed( buf );
  //buf.setf( ostrstream::fixed );

  // Loop once for each element of timeVec
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<length(); i++ ) {

     if ( i != 0 ) buf <<",";

     // format number
     buf << operator[]( i );

  }

  std::string retVal( buf.str() );

  return retVal;
}



// Perform diadic vector operation.
// Returned vector is: 
//    this[t] "oper" source[t]
//  where t is 0,1,...,length of vectors
//        "oper" is function which has 2 float parameter and returns float
LgFrTimeVecFloat 
LgFrTimeVecFloat::op(
      const LgFrTimeVecFloat& source,
      float (*scalarOperation) (const float, const float) )
   const
{
   assert( this->length() == source.length() );
   LgFrTimeVecFloat retVal( source.length() );
   size_t t = 0; // Pulled out of the for below by RW2STL
   for ( t=0; t<source.length(); t++ )
      retVal[t] = (scalarOperation)( (*this)[t], source[t] );
   return retVal;
}

// scan method modeled after APL scan \ operator.
LgFrTimeVecFloat 
LgFrTimeVecFloat::scan(
      float (*scalarOperation) (const float, const float) )
   const
{
   LgFrTimeVecFloat retVal( *this );
   size_t t = 0; // Pulled out of the for below by RW2STL
   for ( t=1; t<retVal.length(); t++ )
      retVal[t] = (scalarOperation)( retVal[t-1], retVal[t] );
   return retVal;
}

// reduce method modeled after APL reduce / operator.
float 
LgFrTimeVecFloat::reduce(
      float (*scalarOperation) (const float, const float) )
   const
{
   assert( this->length() > 0 );
   float retVal( (*this)[0] );
   size_t t = 0; // Pulled out of the for below by RW2STL
   for ( t=1; t<this->length(); t++ )
      retVal = (scalarOperation)( retVal, (*this)[t] );
   return retVal;
}


// Addition
LgFrTimeVecFloat
LgFrTimeVecFloat::add(
   const LgFrTimeVecFloat& rhs)
const
{
  return (*this) + rhs;
}

// Subtraction
LgFrTimeVecFloat
LgFrTimeVecFloat::subtract(
   const LgFrTimeVecFloat& rhs)
const
{
  return (*this) - rhs;
}

// Multiplication
LgFrTimeVecFloat
LgFrTimeVecFloat::multiply(
   const LgFrTimeVecFloat& rhs)
const
{
  return (*this) * rhs;
}

// Division
LgFrTimeVecFloat
LgFrTimeVecFloat::divide(
   const LgFrTimeVecFloat& rhs)
const
{
  return (*this) / rhs;
}

// Maxium
LgFrTimeVecFloat
LgFrTimeVecFloat::maximum(
   const LgFrTimeVecFloat& rhs)
const
{
  return op( rhs, LgFrFloatMax );
}

// undoCum
// If *this is:   1 3 6 10
// then return:   1 2 3 4
LgFrTimeVecFloat
LgFrTimeVecFloat::undoCum()
const
{
  return (*this) - ( (*this) >> 1 );
}

// Percentage Of, with 0/0 being defined as 0%
LgFrTimeVecFloat
LgFrTimeVecFloat::percentageOf(
    const LgFrTimeVecFloat& rhs)
const
{
 if (this->length() == 0)
     return *(this);
 assert (this->length() == rhs.length());

 LgFrTimeVecFloat retVec(this->length(), 0.0);
 size_t len = this->length();
 float l;	// current element from *this
 float r;	// current element from rhs
 int i = 0; // Pulled out of the for below by RW2STL
 for (i=0;  i<len;  i++) {
     l = (*this)[i];
     r = rhs[i];
     if (r == 0  &&  l == 0)
	 retVec[i] = 0;
     else {
	 if (r == 0){
	     std::cout << "Warning: division by zero "
		 << "requested in LgFrTimeVecFloat::percentageOf" << std::endl;
	     retVec[i] = 0;
	 }
	 else
	     retVec[i] = l*100/r;
     }
 }
 return retVec;
}


// cumulativeNet ("net out" another timevec of data)
// This is designed for the "netting" of real order demands
// from a forecast demand.  The resulting timeVec is non-negative
// and any net quatnity that is greater than what's available is
// rolled to the next period.
LgFrTimeVecFloat
LgFrTimeVecFloat::cumulativeNet(
      const LgFrTimeVecFloat & consumer )
   const
{
 if (this->length() == 0)
     return *(this);
 assert (this->length() == consumer.length());

 // need  a copy of the consumer because were going to
 // change it to do "rollover"
 LgFrTimeVecFloat consumerTVF(consumer);
 
 float remainder;

 LgFrTimeVecFloat retVec(this->length(), 0.0);
 int i = 0; // Pulled out of the for below by RW2STL
 for (i=0; i<this->length(); i++) {
              // if offset negative, don't apply
   remainder = (*this)[i] - consumerTVF[i];

   // if you consume more than is available, then roll
   // the remainder into the next period (unless you're
   // already at the last period).  
   if (remainder < 0)  {
     retVec[i] = 0.0;
     if (i < this->length() - 1)
        consumerTVF[i+1] -= remainder;
   }
   else
        retVec[i] = remainder;
  }
 return retVec;
}

// Net qty out of elements of *this that are after element
// at startIndex.  (The element at startIndex is not effected.)
// This may change both *this and the qty argument.
void
LgFrTimeVecFloat::takeQtyFromPositiveElementsAfterIndex
(float & qty,
 const size_t startIndex,
 const float fltEps)
{
  if (qty < -fltEps) {
    std::cerr << "WARNING: LgFrTimeVecFloat::takeQtyFromPositiveElementsAfterIndex"
      << "\ncalled with a negative quantity.  Call ignored\n";
  }
  else {
    size_t j = startIndex+1;
    const size_t n = this->length();
    while (j < n  &&  qty > fltEps) {
      if ((*this)[j] > fltEps) {
	if ((*this)[j] >= qty) {
	  // subtract qty from both
	  (*this)[j] -= qty;  qty = 0;
	  if (fabs((*this)[j]) < fltEps)  (*this)[j] = 0;
	}
	else {
	  // subtract (*this)[j] from both
	  qty -= (*this)[j];  (*this)[j] = 0;
	}
      }
      ++j;
    }
  }
}

// applyOffset (Apply a cycletime or lead time offset)
LgFrTimeVecFloat
LgFrTimeVecFloat::applyOffset(
      const LgFrTimeVecFloat & offsetFTV )
   const
{
 if (this->length() == 0)
     return *(this);
 assert (this->length() == offsetFTV.length());

 int intOffset;
 int shiftToPeriod;
 double fraction, ipart;

 LgFrTimeVecFloat retVec(this->length(), 0.0);
 int i = 0; // Pulled out of the for below by RW2STL
 for (i=0; i<this->length(); i++) {
              // if offset negative, don't apply
     if (offsetFTV[i] > -0.0001) {
        fraction = modf(offsetFTV[i], &ipart);
        intOffset = (int) ipart;
        if (fraction > 0.5)
           intOffset++;
        shiftToPeriod = i + intOffset;
        // stuff that rolls off the end is put into the last bucket
        if (shiftToPeriod >= this->length())
           shiftToPeriod = this->length() - 1;
        retVec[shiftToPeriod] += this->operator[](i);
     }
 }
 return retVec;
}

// Return the rhs (very simple)
LgFrTimeVecFloat
LgFrTimeVecFloat::returnRHS(
   const LgFrTimeVecFloat& rhs)
const
{
  return rhs;
}

// Sumation
float
LgFrTimeVecFloat::sum()
const
{
  return this->reduce(LgFrAdd);
}

// Maximum
float
LgFrTimeVecFloat::max()
const
{
  return this->reduce(LgFrFloatMax);
}

// Minimum
float
LgFrTimeVecFloat::min()
const
{
  return this->reduce(LgFrFloatMin);
}

// Scalar floating point add which operates on single vector element
float 
LgFrAdd( const float lhs, const float rhs ) 
{
   return (lhs+rhs);
}

// Scalar floating point subtract which operates on single vector element
float 
LgFrSubtract( const float lhs, const float rhs )
{
   return (lhs-rhs);
}

// Scalar floating point multiply which operates on single vector element
float 
LgFrMultiply( const float lhs, const float rhs )
{
   return (lhs*rhs);
}

// Scalar floating point divide which operates on single vector element
float 
LgFrDivide( const float lhs, const float rhs )
{
   return (lhs/rhs);
}

// Scalar floating point assignment ( lhs = rhs ).
float
LgFrAssign( const float lhs, const float rhs )
{
   return rhs;
}

// returns ths highest priority (minimum number that's still > 0 )
float
minNonZeroFloat (const float a, const float b)
{
    return (a <= b
       ? (a != 0  ?  a  :  b)
       : (b != 0  ?  b  :  a)
      );
}

// Function used by LgFrTimeVecFloat::test
float LgFrTimeVecFloatUserFunc(const float a, const float b) {
     return ( 2.0f*a + b );
}


// Addition operator
LgFrTimeVecFloat
operator+ (const LgFrTimeVecFloat& lhs, const LgFrTimeVecFloat& rhs)
{ 
  return lhs.op(rhs,LgFrAdd);
}
LgFrTimeVecFloat
operator+ (const LgFrTimeVecFloat& lhs, const float& rhs)
{ 
  return lhs.op( LgFrTimeVecFloat(lhs.length(),rhs), LgFrAdd);
}
LgFrTimeVecFloat
operator+ (const float& lhs, const LgFrTimeVecFloat& rhs)
{ 
  return rhs+lhs;
}

// Subtraction operator
LgFrTimeVecFloat
operator- (const LgFrTimeVecFloat& lhs, const LgFrTimeVecFloat& rhs)
{
  return lhs.op(rhs,LgFrSubtract);
}
LgFrTimeVecFloat
operator- (const LgFrTimeVecFloat& lhs, const float& rhs)
{ 
  return lhs.op( LgFrTimeVecFloat(lhs.length(),rhs), LgFrSubtract);
}
LgFrTimeVecFloat
operator- (const float& lhs, const LgFrTimeVecFloat& rhs)
{
  return ( LgFrTimeVecFloat(rhs.length(),lhs) ).op(rhs,LgFrSubtract);
}

// Multiplication operator
LgFrTimeVecFloat
operator* (const LgFrTimeVecFloat& lhs, const LgFrTimeVecFloat& rhs)
{
  return lhs.op(rhs,LgFrMultiply);
}
LgFrTimeVecFloat
operator* (const LgFrTimeVecFloat& lhs, const float& rhs)
{ 
  return lhs.op( LgFrTimeVecFloat(lhs.length(),rhs), LgFrMultiply);
}
LgFrTimeVecFloat
operator* (const float& lhs, const LgFrTimeVecFloat& rhs)
{ 
  return rhs*lhs;
}

// Division operator
LgFrTimeVecFloat
operator/ (const LgFrTimeVecFloat& lhs, const LgFrTimeVecFloat& rhs)
{
  return lhs.op(rhs,LgFrDivide);
}
LgFrTimeVecFloat
operator/ (const LgFrTimeVecFloat& lhs, const float& rhs)
{ 
  return lhs.op( LgFrTimeVecFloat(lhs.length(),rhs), LgFrDivide);
}
LgFrTimeVecFloat
operator/ (const float& lhs, const LgFrTimeVecFloat& rhs)
{ 
  return ( LgFrTimeVecFloat(rhs.length(),lhs) ).op(rhs,LgFrDivide);
}

// Shift operators
LgFrTimeVecFloat
operator<< (const LgFrTimeVecFloat& source, const int& rhs)
{
  if (rhs == 0)
    return source;			// Return by value makes copy
  else if (rhs < 0) 
    return (source >> (-rhs));
  else {
    assert (rhs > 0);
    LgFrTimeVecFloat result(source);		// Copy source
    int resultI, sourceI;			// Indexes
    const int n = source.length();
    resultI = 0;
    sourceI = rhs;
    // Invarient for rest of this routine:
    // For all i such that i < resultI: result[i] has the right value
    while (sourceI < n) 
      result [resultI++] = source [sourceI++];
    while (resultI < n)
      result [resultI++] = 0.0;
    return result;
  }
}

LgFrTimeVecFloat
operator>> (const LgFrTimeVecFloat& source, const int& rhs)
{
  if (rhs == 0)
    return source;				// Return by makes copy
  else if (rhs < 0) 
    return (source << (-rhs));
  else {
    assert (rhs > 0);
    LgFrTimeVecFloat result(source);		// Copy source
    int resultI, sourceI;			// Indexes
    const int n = source.length();
    resultI = 0;
    // Invarient for rest of this routine:
    // For all i such that i < resultI: result[i] has the right value
    while (resultI < rhs)
      result [resultI++] = 0.0;
    sourceI = 0;
    while (resultI < n) 
      result [resultI++] = source [sourceI++];
    return result;
  }  
}

// Equality operator
int operator== (const LgFrTimeVecFloat& lhs, const LgFrTimeVecFloat& rhs)
{
  return lhs.compareTo( rhs ) == 0;
}
int operator== (const LgFrTimeVecFloat& lhs, const float& rhs)
{
  return lhs.compareTo( LgFrTimeVecFloat(lhs.length(),rhs) ) == 0;
}
int operator== (const float& lhs, const LgFrTimeVecFloat& rhs)
{ 
  return rhs==lhs;
}


// Inequality operator
int operator!= (const LgFrTimeVecFloat& lhs, const LgFrTimeVecFloat& rhs)
{
  return lhs.compareTo( rhs ) != 0;
}
int operator!= (const LgFrTimeVecFloat& lhs, const float& rhs)
{
  return lhs.compareTo( LgFrTimeVecFloat(lhs.length(),rhs) ) != 0;
}
int operator!= (const float& lhs, const LgFrTimeVecFloat& rhs)
{ 
  return rhs!=lhs;
}


// Less than operator
int operator< (const LgFrTimeVecFloat& lhs, const LgFrTimeVecFloat& rhs)
{
  return lhs.compareTo( rhs ) < 0;
}
int operator< (const LgFrTimeVecFloat& lhs, const float& rhs)
{
  return lhs.compareTo( LgFrTimeVecFloat(lhs.length(),rhs) ) < 0;
}
int operator< (const float& lhs, const LgFrTimeVecFloat& rhs)
{ 
  return ( LgFrTimeVecFloat(rhs.length(),lhs) ).compareTo(rhs) < 0;
}

int operator<=(const LgFrTimeVecFloat& lhs, const LgFrTimeVecFloat& rhs)
{
  return lhs.compareTo( rhs ) <= 0;
}
int operator<=(const LgFrTimeVecFloat& lhs, const float& rhs)
{
  return lhs.compareTo( LgFrTimeVecFloat(lhs.length(),rhs) ) <= 0;
}
int operator<=(const float& lhs, const LgFrTimeVecFloat& rhs)
{ 
  return ( LgFrTimeVecFloat(rhs.length(),lhs) ).compareTo(rhs) <= 0;
}

// Greater than operator
int operator> (const LgFrTimeVecFloat& lhs, const LgFrTimeVecFloat& rhs)
{
  return lhs.compareTo( rhs ) > 0;
}
int operator> (const LgFrTimeVecFloat& lhs, const float& rhs)
{
  return lhs.compareTo( LgFrTimeVecFloat(lhs.length(),rhs) ) > 0;
}
int operator> (const float& lhs, const LgFrTimeVecFloat& rhs)
{ 
  return ( LgFrTimeVecFloat(rhs.length(),lhs) ).compareTo(rhs) > 0;
}

int operator>=(const LgFrTimeVecFloat& lhs, const LgFrTimeVecFloat& rhs)
{
  return lhs.compareTo( rhs ) >=0;
}
int operator>=(const LgFrTimeVecFloat& lhs, const float& rhs)
{
  return lhs.compareTo( LgFrTimeVecFloat(lhs.length(),rhs) ) >=0;
}
int operator>=(const float& lhs, const LgFrTimeVecFloat& rhs)
{ 
  return ( LgFrTimeVecFloat(rhs.length(),lhs) ).compareTo(rhs) >=0;
}

#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrTimeVecFloat
void
LgFrTimeVecFloat::test()
{
  const float fltEps = .00001f;
  int i;

  // Test default constructor
  LgFrTimeVecFloat defTV; 
  assert( defTV.length() == 0 );

  // Test constructor with nonzero length
  LgFrTimeVecFloat tv10(10); 
  assert( tv10.length() == 10 );

  // Test constructor with nonzero length, and initial value
  LgFrTimeVecFloat tv5( 5, 3.1415f ); 
  assert( tv5.length() == 5 );
  for ( i=0; i<tv5.length(); i++ ) assert( fabs(tv5[i]-3.1415f) < fltEps );

  // Test operator[] as lvalue
  tv5[0]=0.0f;
  tv5[1]=1.1f;
  tv5[2]=2.2f;
  tv5[3]=3.3f;
  tv5[4]=4.4f;
  assert( fabs(tv5[0]-0.0f) < fltEps );
  assert( fabs(tv5[1]-1.1f) < fltEps );
  assert( fabs(tv5[2]-2.2f) < fltEps );
  assert( fabs(tv5[3]-3.3f) < fltEps );
  assert( fabs(tv5[4]-4.4f) < fltEps );

  // Test copy constructor
  LgFrTimeVecFloat tvCopy( tv5 ); 
  assert( tvCopy.length() == 5 );
  assert( fabs(tvCopy[0]-0.0f) < fltEps );
  assert( fabs(tvCopy[1]-1.1f) < fltEps );
  assert( fabs(tvCopy[2]-2.2f) < fltEps );
  assert( fabs(tvCopy[3]-3.3f) < fltEps );
  assert( fabs(tvCopy[4]-4.4f) < fltEps );

  // Test data method
  assert( fabs(tvCopy.data()[0]-0.0f) < fltEps );
  assert( fabs(tvCopy.data()[1]-1.1f) < fltEps );
  assert( fabs(tvCopy.data()[2]-2.2f) < fltEps );
  assert( fabs(tvCopy.data()[3]-3.3f) < fltEps );
  assert( fabs(tvCopy.data()[4]-4.4f) < fltEps );
  const float * data = tvCopy.data();
  assert( fabs(data[0]-0.0f) < fltEps );
  assert( fabs(data[1]-1.1f) < fltEps );
  assert( fabs(data[2]-2.2f) < fltEps );
  assert( fabs(data[3]-3.3f) < fltEps );
  assert( fabs(data[4]-4.4f) < fltEps );
  
  // Test intData method
  {
  LgFrTimeVecFloat tv(5,"-1.6 -1.1 0.0 2.1 3.8");
  assert( tv.intData()[0] == -2 );
  assert( tv.intData()[1] == -1 );
  assert( tv.intData()[2] ==  0 );
  assert( tv.intData()[3] ==  2 );
  assert( tv.intData()[4] ==  4 );
  const int * data = tv.intData();
  assert( data[0] == -2 );
  assert( data[1] == -1 );
  assert( data[2] ==  0 );
  assert( data[3] ==  2 );
  assert( data[4] ==  4 );
  }
  

  // Test compareTo, operator==, and operator!=
  assert( tvCopy.compareTo(tv5) == 0 );
  assert( tvCopy == tv5 );
  assert( ! (tvCopy != tv5) );
  tvCopy[1] = 1.11f;
  assert( tvCopy.compareTo(tv5) > 0 );
  assert( tv5.compareTo(tvCopy) < 0 );
  assert( !( tvCopy == tv5 ) );
  assert( tvCopy != tv5 );

  // Test operator<
  assert( tv5 < tvCopy );
  assert( -.5 < tvCopy );
  assert( tvCopy < 5. );

  // Test operator<=
  assert( tv5 <= tvCopy );
  assert( -.5 <= tvCopy );
  assert( tvCopy <= 5. );
  assert( tvCopy <= tvCopy );

  // Test operator >
  assert( tvCopy > tv5 );
  assert( tvCopy > -.5 );
  assert( 5. > tvCopy );

  // Test operator >=
  assert( tvCopy >= tv5 );
  assert( tvCopy >= -.5 );
  assert( 5. >= tvCopy );
  assert( tvCopy >= tvCopy );

  // Test Assignment to float
  tvCopy = 123.456f;
  assert( tvCopy.length() == 5 );
  for ( i=0; i<tvCopy.length(); i++ )
     assert( fabs(tvCopy[i]-123.456f) < fltEps );

  // Test Assignment to LgFgTimeVecFloat
  assert( !(tvCopy.compareTo(tv5) == 0) );
  tv5 = tvCopy;
  assert( tvCopy.compareTo(tv5) == 0 );
  assert( tv5.length() == 5 );
  for ( i=0; i<tv5.length(); i++ )
     assert( fabs(tv5[i]-123.456f) < fltEps );

  // Test resize
  tv10.reshape(15);
  assert( tv10.length() == 15 );

  // Test Alternative Constructor used when with WIT API returned values
  float fltVec[] = { 3.0, 2.0, 1.0 };
  LgFrTimeVecFloat tv3( 3, fltVec );
  assert( tv3.length() == 3 );
  assert( fabs(tv3[0]-3.0) < fltEps );
  assert( fabs(tv3[1]-2.0) < fltEps );
  assert( fabs(tv3[2]-1.0) < fltEps );

  // Test op method
  {
  LgFrTimeVecFloat tv1(5,3.0), tv2(5,4.0), tv3;
  tv3 = tv1.op(tv2,LgFrTimeVecFloatUserFunc);
  assert( tv3 == ( tv1 + tv1 + tv2 ) );
  }

  // Test addition operator
  float fltVecA[] = { 4.0, 3.0, 2.0 };
  LgFrTimeVecFloat tvA( 3, fltVecA );
  float fltVecB[] = {14.0,13.0,12.0 };
  LgFrTimeVecFloat tvB( 3, fltVecB );
  LgFrTimeVecFloat sum;
  sum = tv3 + tvA + tvB;
  assert( sum.length() == 3 );
  assert( fabs(sum[0]-21.0) < fltEps );
  assert( fabs(sum[1]-18.0) < fltEps );
  assert( fabs(sum[2]-15.0) < fltEps );
  sum = (tv3.add(tvA)).add(tvB);
  assert( fabs(sum[0]-21.0) < fltEps );
  assert( fabs(sum[1]-18.0) < fltEps );
  assert( fabs(sum[2]-15.0) < fltEps );

  // Test subtraction operator
  sum = sum - tv3 -tvB;
  assert ( sum == tvA );
  sum = tv3 + tvA + tvB;
  sum = sum.subtract(tv3).subtract(tvB);
  assert ( sum == tvA );

  // Test cumulativeNet
  LgFrTimeVecFloat forecastTV( 6, 100.0);
  float committed[] = {75., 100., 125., 150., 0., 150.};
  LgFrTimeVecFloat committedTV( 6, committed );
  LgFrTimeVecFloat nettedForecast;
  nettedForecast = forecastTV.cumulativeNet(committedTV);
  assert( nettedForecast.length() == 6 );
  assert( fabs(nettedForecast[0] - 25.0) < fltEps );
  assert( fabs(nettedForecast[1] - 0.0)  < fltEps );
  assert( fabs(nettedForecast[2] - 0.0)  < fltEps );
  assert( fabs(nettedForecast[3] - 0.0)  < fltEps );
  assert( fabs(nettedForecast[4] - 25.0) < fltEps );
  assert( fabs(nettedForecast[5] - 0.0)  < fltEps );

  // Test applyOffset operator
  float dataVec[] = { 5.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  LgFrTimeVecFloat dataTV( 5, dataVec );
  float offsetVec[] = {-1.0f, 1.0f, 0.4f, 0.8f, 1.0f};
  LgFrTimeVecFloat offsetTV( 5, offsetVec );
  LgFrTimeVecFloat shiftedTV;
  shiftedTV = dataTV.applyOffset(offsetTV);
  assert( shiftedTV.length() == 5 );
  assert( fabs(shiftedTV[0] - 0.0) < fltEps );
  assert( fabs(shiftedTV[1] - 0.0) < fltEps );
  assert( fabs(shiftedTV[2] - 7.0) < fltEps );
  assert( fabs(shiftedTV[3] - 0.0) < fltEps );
  assert( fabs(shiftedTV[4] - 3.0) < fltEps );


  // Test multiplication
  {
  LgFrTimeVecFloat tv1(10,100.f), tv2(10,.53f), tv3(10,53.f);
  assert( tv3 == tv1*tv2 );
  }

  // Test division
  {
  LgFrTimeVecFloat tv1(10,100.f), tv2(10,.53f), tv3(10,53.f);
  assert( tv2 == tv3/tv1 );
  }

  // Test maximum
  {
  LgFrTimeVecFloat tv1(5,"-2 -1 0  1 2" ), tv2(5,"2 1 0 -2 6");
  LgFrTimeVecFloat tv3(5," 2  1 0  1 6" );
  assert( tv3 == tv1.maximum( tv2 ) );
  }

  // Test "float operation TimeVecFloat" and "TimeVecFloat operation float"
  // where operation if +,-,*,==
  {
  LgFrTimeVecFloat tv1(50,100.0f), tv2(50,110.0f), tv3(50,90.0f), tv4(50,-90.0f);
  assert( tv1*1.1f == tv2 );
  assert( 1.1f*tv1 == tv2 );
  assert( tv2/1.1f == tv1 );
  assert( 11000.f/tv1 == tv2 );
  assert( tv1+10.f == tv2 );
  assert( 10.f+tv1 == tv2 );
  assert( tv1-10.f == tv3 );
  assert( 10.f-tv1 == tv4 );
  assert( tv1 == 100.0f );
  assert( 110.0f == tv2 );
  }

  // Test shift operators
  {
    LgFrTimeVecFloat tvshift (50);
    int i = 0; // Pulled out of the for below by RW2STL
    for (i = 0;  i < 50;  ++i)
      tvshift [i] = (float)i;
    assert (!(tvshift == (tvshift >> 1)));
    assert (  tvshift == ((tvshift << 1) >> 1) ); // shifts 0 out and back in
    assert (!(tvshift == ((tvshift >> 1) << 1))); // shifts 49 out and 0 in 
    for (i = 40; i < 50; ++i)
      tvshift [i] = 0;
    assert (  tvshift == ((tvshift >> 10) << 10) );
    assert (  tvshift == (((((tvshift>>3)>>7)<<4)<<4)<<2));  // check for 
							    //off-by-one errors
    assert (!(tvshift == ((tvshift >> 11) << 11)));
    assert (!(tvshift == ((tvshift << 10) << 10)));
  }

  // Test format method
  {
    float vec[]={1234567.f,123456.7f,12345.67f,1234.567f,123.4567f,12.34567f};
    LgFrTimeVecFloat tvfmt(6,vec);
    LgFrTimeVecFloat hl(6,0.0);
    LgFrTimeVecFloat hlAll(6,2.0);
    hl[2]=1.; hl[4]=-1.;
    assert (tvfmt.format() == 
	    " 1234567.0   123456.7    12345.7     1234.6      123.5       12.3 ");
    assert (tvfmt.format(hl) == 
	    " 1234567.0   123456.7    12345.7*    1234.6      123.5*      12.3 ");
    assert (tvfmt.format(hl>>2) ==
	    " 1234567.0   123456.7    12345.7     1234.6      123.5*      12.3 ");
    assert (tvfmt.format(hl,'$') ==
	    " 1234567.0   123456.7    12345.7$    1234.6      123.5$      12.3 ");
    assert (tvfmt.format(hl,'+',12) ==
	    "   1234567.0     123456.7      12345.7+      1234.6        123.5+        12.3 ");
    assert (tvfmt.format(hl,'!',12,0) ==
	    "    1234567.      123457.       12346.!       1235.         123.!         12. ");
    assert (tvfmt.format(hl,'*',12,0) ==
	    "    1234567.      123457.       12346.*       1235.         123.*         12. ");
    assert (tvfmt.format(hlAll,'*',12,0) ==
	    "    1234567.*     123457.*      12346.*       1235.*        123.*         12.*");
  }

  // Test scan method
  {
  float v   []={ 1., 3., 5.,  7., 11., 13. };
  float cumv[]={ 1., 4., 9., 16., 27., 40. };
  LgFrTimeVecFloat    vec(6,   v);
  LgFrTimeVecFloat cumVec = vec.scan(LgFrAdd);
  assert( cumVec == LgFrTimeVecFloat(6,cumv) );

  LgFrTimeVecFloat nullVec;
  assert( nullVec == nullVec.scan(LgFrAdd) );

  LgFrTimeVecFloat singletonVec(1,4.);
  LgFrTimeVecFloat cumSingletonVec( singletonVec.scan(LgFrAdd) );
  assert( singletonVec == cumSingletonVec );

  float difv[]={ 1., -2., -7., -14., -25., -38. };
  assert( vec.scan(LgFrSubtract) == LgFrTimeVecFloat(6,difv) );
  }

  // Test reduce, sum, max, and min methods
  {
  float v   []={ 1., 3., 5.,  7., 13., 11. };
  LgFrTimeVecFloat    vec(6,   v);
  assert( fabs( vec.reduce(LgFrAdd) - 40.0 ) <= fltEps  );
  assert( fabs( vec.reduce(LgFrFloatMax) - 13.0 ) <= fltEps  );
  assert( fabs( vec.sum() - 40.0 ) <= fltEps  );
  assert( fabs( vec.max() - 13.0 ) <= fltEps  );
  assert( fabs( vec.min() - 1.0 ) <= fltEps  );

  LgFrTimeVecFloat singletonVec(1,4.);
  assert( fabs( singletonVec.reduce(LgFrAdd) - 4. ) <= fltEps );
  assert( fabs( singletonVec.reduce(LgFrFloatMax) - 4. ) <= fltEps );
  assert( fabs( singletonVec.sum() - 4. ) <= fltEps );
  assert( fabs( singletonVec.max() - 4. ) <= fltEps );
  assert( fabs( singletonVec.min() - 4. ) <= fltEps );
  }

  // Test UndoCum
  {
  float v   []={ 1., 3., 5.,  7., 13., 11. };
  LgFrTimeVecFloat    vec(6,   v);
  LgFrTimeVecFloat cumVec = vec.scan(LgFrAdd);
  assert( cumVec.undoCum() == vec );
  }
  
  // Test percentageOf
  {
      float left [] = {0, 0, 1, 5.5, 17};
      float right[] = {0, 1, 2, 5.5, 8.5};
      LgFrTimeVecFloat lv (5, left);
      LgFrTimeVecFloat rv (5, right);
      LgFrTimeVecFloat result (lv.percentageOf(rv));
      assert (fabs (result[0])      < fltEps);
      assert (fabs (result[1])	    < fltEps);
      assert (fabs (result[2]-50)   < fltEps);
      assert (fabs (result[3]-100)  < fltEps);
      assert (fabs (result[4]-200)  < fltEps);
  }

  // Test std::string constructor with padding
  LgFrTimeVecFloat aVec(5, "0.0 1.0 2.0 3.0");
  assert( fabs(aVec[2] - 2.0) <= fltEps );
  assert( fabs(aVec[4] - 0.0) <= fltEps );

  // Test std::string constructor with truncation
  LgFrTimeVecFloat a1Vec(5, "0.0 1.0 2.0 3.0 4.0 5.0");
  assert( fabs(a1Vec[2] - 2.0) <= fltEps );
  assert( fabs(a1Vec[4] - 4.0) <= fltEps );
 
  // Test std::string constructor with different "white space"
  LgFrTimeVecFloat a2Vec(5, "0.0 1.0 $2.0 3.0 4.0" , " $\t\n");
  assert( fabs(a2Vec[2] - 2.0) <= fltEps );
  assert( fabs(a2Vec[4] - 4.0) <= fltEps );
 
  // Test std::string constructor with constant value
  {
  LgFrTimeVecFloat a2Vec(3, "1.0 1.0 1.0" );
  assert( fabs(a2Vec[0] - 1.0) <= fltEps );
  assert( fabs(a2Vec[1] - 1.0) <= fltEps );
  assert( fabs(a2Vec[2] - 1.0) <= fltEps );
  assert( a2Vec.length() == 3 );
  }
  {
  LgFrTimeVecFloat a2Vec(4, "1.0 1.0 1.0" );
  assert( fabs(a2Vec[0] - 1.0) <= fltEps );
  assert( fabs(a2Vec[1] - 1.0) <= fltEps );
  assert( fabs(a2Vec[2] - 1.0) <= fltEps );
  assert( fabs(a2Vec[3] - 0.0) <= fltEps );
  assert( a2Vec.length() == 4 );
  }
  {
  LgFrTimeVecFloat a2Vec(2, "1.0 1.0 1.0" );
  assert( fabs(a2Vec[0] - 1.0) <= fltEps );
  assert( fabs(a2Vec[1] - 1.0) <= fltEps );
  assert( a2Vec.length() == 2 );
  }
  {
  LgFrTimeVecFloat a2Vec(0, "1.0 1.0 1.0" );
  assert( a2Vec.length() == 0 );
  }
  {
  LgFrTimeVecFloat a2Vec(2, " " );
  assert( fabs(a2Vec[0] - 0.0) <= fltEps );
  assert( fabs(a2Vec[1] - 0.0) <= fltEps );
  assert( a2Vec.length() == 2 );
  }
  {
  LgFrTimeVecFloat a2Vec(0, " " );
  assert( a2Vec.length() == 0 );
  }
}
