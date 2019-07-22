// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.

#include <scenario/src/partSchd.h>
#include <scenario/src/partSchP.h>
#include <scenario/src/partSS.h>
#include <scenario/src/scenario.h>
#include <scenario/src/supplySS.h>
#include <scenario/src/prodSS.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/machdep.h>

#include <iostream>

// Set Schedule Strategy
void
LgFrSortingPartScheduleFloat::attributeScheduleStrategy(
const LgFrPartScheduleStrategy & attributeScheduleStrategy )
{
   LgFrSortingScheduleFloat::attributeScheduleStrategy( attributeScheduleStrategy );
}

// Get Schedule Strategy
// If this object ceases to exist or the attributeScheduleStrategy is changed,
// then the returned reference will be dangling
const LgFrPartScheduleStrategy *
LgFrSortingPartScheduleFloat::attributeScheduleStrategy()
const
{
   return (const LgFrPartScheduleStrategy *)
      LgFrSortingScheduleFloat::attributeScheduleStrategy();
}

// Add ItemTimeVecPairFloat to part schedule
void
LgFrSortingPartScheduleFloat::insert (
       const LgFrItemTimeVecPairFloat itvpf )
{
   assert( &(itvpf.item()) != 0 );
   assert( itvpf.item().isAPart());
   LgFrSortingScheduleFloat::insert( itvpf );
}

// Insert ItemTimeVecPairFloat to part schedule
void
LgFrSortingPartScheduleFloat::insertAt (
       const size_t index,
       const LgFrItemTimeVecPairFloat itvpf )
{
   assert( itvpf.item().isAPart());
   LgFrSortingScheduleFloat::insertAt( index, itvpf );
}


// Insert Part if it isn't already in the schedule.  In any case,
// associate timeVec with part.
void
LgFrSortingPartScheduleFloat::insertKeyAndValue(const LgFrPart& part,
					 const LgFrTimeVecFloat& value)
{
  this -> LgFrSortingScheduleFloat::insertKeyAndValue (part, value);
}


// Make clone copy of object
LgFrSortingScheduleFloat *
LgFrSortingPartScheduleFloat::clone()
const
{
  LgFrSortingPartScheduleFloat* newInst = 
      new LgFrSortingPartScheduleFloat(*this);
  return (newInst);
}

// Make clone copy of object with newScenario
LgFrSortingScheduleFloat *
LgFrSortingPartScheduleFloat::clone(LgFrScenario& newScenario)
const
{ 
  LgFrSortingPartScheduleFloat* newInst = 
      new LgFrSortingPartScheduleFloat(*this, newScenario);
  return (newInst);
}

// Return part at location i
LgFrPart
LgFrSortingPartScheduleFloat::part (const size_t i)
     const
{
  assert (  i < this->size()  );
  return (  (*this)[i]  . part()  );
}

LgFrItemTimeVecPairFloat &
LgFrSortingPartScheduleFloat::operator[](const LgFrPart& part)
{
  return (this->LgFrSortingScheduleFloat::operator[](part));
}

const LgFrItemTimeVecPairFloat &
LgFrSortingPartScheduleFloat::operator[](const LgFrPart& part)
const
{
  return (this->LgFrSortingScheduleFloat::operator[](part));
}

  
LgFrItemTimeVecPairFloat
LgFrSortingPartScheduleFloat::operator[](const size_t& i) const
{
  return (this->LgFrSortingScheduleFloat::operator[](i));
}

  
LgFrItemTimeVecPairFloat &
LgFrSortingPartScheduleFloat::operator[](const size_t& i)
{
  return (this->LgFrSortingScheduleFloat::operator[](i));
}
  
// This should not be used by clients of this class.  Since it is a
// virtual method in the base class, it is used by the base
// class when *this is a LgFrSortingPartScheduleFloat.
LgFrItemTimeVecPairFloat &
LgFrSortingPartScheduleFloat::operator[](const LgFrItem& item)
{
  assert (item.isAPart());
  return this->LgFrSortingScheduleFloat::operator[](item);
}

// This should not be used by clients of this class.  Since it is a
// virtual method in the base class, it is used by the base
// class when *this is a LgFrSortingPartScheduleFloat.
const LgFrItemTimeVecPairFloat &
LgFrSortingPartScheduleFloat::operator[](const LgFrItem& item)
const
{
  assert (item.isAPart());
  return this->LgFrSortingScheduleFloat::operator[](item);
}


// default constructor
LgFrSortingPartScheduleFloat::LgFrSortingPartScheduleFloat(const size_t n)
: LgFrSortingScheduleFloat ( n ) //(size_tMax (n, 1)) )
{
   // Nothing to do here
}
  
// Copy constructor
LgFrSortingPartScheduleFloat::LgFrSortingPartScheduleFloat (
   const LgFrSortingPartScheduleFloat& source)
: LgFrSortingScheduleFloat ( source )
// : LgFrSortingScheduleFloat ( (LgFrSortingScheduleFloat)(source) )
{
   // Nothing to do here
}
  
// Copy constructor with new Scenario
LgFrSortingPartScheduleFloat::LgFrSortingPartScheduleFloat (
   const LgFrSortingPartScheduleFloat& source,
   LgFrScenario & newScenario)
: LgFrSortingScheduleFloat ( source, newScenario )
{
   // Nothing to do here
}
  
// destructor
LgFrSortingPartScheduleFloat::~LgFrSortingPartScheduleFloat()
{
   // Nothing to do here
}
  
// Assignment 
LgFrSortingPartScheduleFloat &
LgFrSortingPartScheduleFloat::operator=(
    const LgFrSortingPartScheduleFloat& rhs)
{
  if (this != &rhs) {           // Check for assignment to self
    // Sun CC does not like following line, so do it differently
    (this->LgFrSortingScheduleFloat::operator= ) (rhs);
  }

    // Test assignment operator result
    assert (this->size() == rhs.size());

    return *this;
}

// Perform diadic schedule operation.
LgFrSortingPartScheduleFloat
LgFrSortingPartScheduleFloat::op(
   LgFrTimeVecFloatOpFunction vectorOp,
   const LgFrSortingPartScheduleFloat& rhs,
   float defaultValue )
const
{
   LgFrSortingPartScheduleFloat retVal;

   kernalOfOp( vectorOp, rhs, retVal, defaultValue );

   return retVal;
}



// Perform diadic schedule operation.
LgFrSortingPartScheduleFloat
LgFrSortingPartScheduleFloat::op(
   LgFrTimeVecFloatRhsScalarOpFunction scalarOp,
   const float rhs )
const
{
   LgFrSortingPartScheduleFloat retVal;

   kernalOfOp( scalarOp, rhs, retVal );

   return retVal;
}


// Perform diadic schedule operation.
LgFrSortingPartScheduleFloat
LgFrSortingPartScheduleFloat::op(
   const float lhs,
   LgFrTimeVecFloatLhsScalarOpFunction scalarOp )
const
{
   LgFrSortingPartScheduleFloat retVal;

   kernalOfOp( lhs, scalarOp, retVal );

   return retVal;
}

// perform scalar operation on column (period) in schedule
LgFrSortingPartScheduleFloat
LgFrSortingPartScheduleFloat::columnOp(
  const size_t period,
  float (*scalarOperation) (const float, const float),
  const float rhs )
const
{
   LgFrSortingPartScheduleFloat retVal;

   kernalOfColumnOp(  period, scalarOperation, rhs, retVal );

   return retVal;
}

  
// Addition operator
// (schedule + schedule)
LgFrSortingPartScheduleFloat
operator+ (
   const LgFrSortingPartScheduleFloat& lhs, 
   const LgFrSortingPartScheduleFloat& rhs)
{
  return lhs.op(
#ifdef WIN32
  // Other compilers don't need this cast-not req'd for MSVC4, req'd for MSVC2
  (LgFrTimeVecFloatOpFunction)
#endif
  &LgFrTimeVecFloat::add,rhs);
}

// Subtraction operator
// (schedule - schedule)
LgFrSortingPartScheduleFloat
operator- (
   const LgFrSortingPartScheduleFloat& lhs, 
   const LgFrSortingPartScheduleFloat& rhs)
{
  return lhs.op(
#ifdef WIN32
  // Other compilers don't need this cast-not req'd for MSVC4, req'd for MSVC2
  (LgFrTimeVecFloatOpFunction)
#endif
  &LgFrTimeVecFloat::subtract,rhs);
}


// Multiplcation operator
// (schedule * float)
LgFrSortingPartScheduleFloat
operator* (
   const LgFrSortingPartScheduleFloat& lhs,
   const float rhs)
{
  return lhs.op ( operator*, rhs);
}
// (float * schedule)
LgFrSortingPartScheduleFloat
operator* (
   const float lhs,
   const LgFrSortingPartScheduleFloat& rhs)
{
  return rhs.op ( lhs, operator*);
}


// Division operator
// (schedule / float)
LgFrSortingPartScheduleFloat
operator/ (
   const LgFrSortingPartScheduleFloat& lhs,
   const float rhs)
{
  return lhs.op ( operator/, rhs );
}
// (float / schedule)
LgFrSortingPartScheduleFloat
operator/ (
   const float lhs,
   const LgFrSortingPartScheduleFloat& rhs)
{
  return rhs.op( lhs, operator/ );
}

// Addition operator
// (schedule + float)
LgFrSortingPartScheduleFloat
operator+ (
   const LgFrSortingPartScheduleFloat& lhs,
   const float rhs)
{
  return lhs.op ( operator+, rhs);
}
// (float + schedule)
LgFrSortingPartScheduleFloat
operator+ (
   const float lhs,
   const LgFrSortingPartScheduleFloat& rhs)
{
  return rhs.op ( lhs, operator+);
}


// Subtraction operator
// (schedule - float)
LgFrSortingPartScheduleFloat
operator- (
   const LgFrSortingPartScheduleFloat& lhs,
   const float rhs)
{
  return lhs.op ( operator-, rhs );
}
// (float - schedule)
LgFrSortingPartScheduleFloat
operator- (
   const float lhs,
   const LgFrSortingPartScheduleFloat& rhs)
{
  return rhs.op ( lhs, operator-);
}


  
#ifdef NDEBUG
#undef NDEBUG
#endif
// test this class
void LgFrSortingPartScheduleFloat::test()
{
  // Make some parts for the test
  LgFrPart p1( "part1" );
  LgFrPart p2( "part2" );
  LgFrPart p3( "part3" );

  // Make some timeVecFloat for the test
  LgFrTimeVecFloat tvf1(10,1.11f);
  LgFrTimeVecFloat tvf2(10,2.22f);

  // Make some part timeVecFloat pairs for the test
  LgFrItemTimeVecPairFloat ptvfp1( p1, tvf1 );
  LgFrItemTimeVecPairFloat ptvfp2( p2, tvf1 );
  LgFrItemTimeVecPairFloat ptvfp3( p3, tvf2 );
  LgFrItemTimeVecPairFloat ptvfp4( p1, tvf2 );

  // Test the insertion methods
  LgFrSortingPartScheduleFloat ss (1);	// Make it a little small
  				// to test automatic resizing
  ss.insert(ptvfp2);
  ss.insertKeyAndValue(p3, tvf2); // equivalent to ss.insert(ptvfp3)
  ss.insertAt(0, ptvfp1);
  assert (3 == ss.size());
  assert (ss[0].item().name() == "part1");
  assert (ss[2].item().name() == "part3");
  assert (ss[1].item().name() == "part2");

  // Test the assignment operator and the copy constructor
  LgFrSortingPartScheduleFloat ss1 (1);	// Make it a little small
  				// to test resizing at assignment time
  ss1.insert(ptvfp4);		
  ss1 = ss;
  LgFrSortingPartScheduleFloat ss2 (ss);

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < ss.size(); i++) {
    // Test result of assignment
    assert (ss1[i] == ss[i]);
    // Test result of copy
    assert (ss2[i] == ss[i]);
    // Print the contents of partSchedule
    // ss[i].print();
    // std::cout << std::endl;
  }
  // ss.print();

  // Test to ensure that an item without a defined type cannot be 
  // inserted.
  // ss.push_back( LgFrItemTimeVecPairFloat() );  // <-- assert in insert 
                                               //     should fail 
  // Test assignment using subscripting
  LgFrTimeVecFloat tvf3(10,3.33f);
  ss[0].timeVecFloat (tvf3);	// ss[0] must return an lvalue for this to
  				// have the desired effect
  assert (ss[0].timeVecFloat()[0]  ==  tvf3[0]);   // test for desired effect

  // Test indexing by part
  assert( ss[p3] == ptvfp3 );
  const LgFrSortingPartScheduleFloat constSs(ss);
  assert( constSs[p3] == ptvfp3 );

  // Test resize .. no more. We let the tools do their job
  //LgFrSortingPartScheduleFloat ss3(0);
  //ss3.resize(3);
  //assert (ss3.size() == 0);

  // Test op method
  {
  LgFrPart p1 ("IBMPS2mod95");
  LgFrPart p2 ("IBMPS2mod90serial45conf78");
  LgFrSortingPartScheduleFloat lhs,rhs,diff;
  float v1[]={ 1., 2., 3., 4. };
  float v2[]={ 4., 3., 2., 1. };
  LgFrTimeVecFloat vec1(4,v1), vec2(4,v2);
  lhs.insertKeyAndValue( p1, vec1 );
  lhs.insertKeyAndValue( p2, vec1 );
  rhs.insertKeyAndValue( p2, vec2 );
  rhs.insertKeyAndValue( p3, vec2 );
  diff = lhs.op(
#ifdef WIN32
  // Other compilers don't need this cast-not req'd for MSVC4, req'd for MSVC2
  (LgFrTimeVecFloatOpFunction)
#endif
  &LgFrTimeVecFloat::subtract, rhs );
  // std::cout <<"diff" <<std::endl;
  // std::cout <<diff.format() <<std::endl;
  assert( diff.size() == 3 );
  assert( diff[0].item() == p1 );
  assert( diff[1].item() == p2 );
  assert( diff[2].item() == p3 );
  assert( diff[0].timeVecFloat() == vec1 );
  assert( diff[1].timeVecFloat() == vec1-vec2 );
  assert( diff[2].timeVecFloat() == 0.0 -vec2 );
  }
  {  // empty rhs
  LgFrSortingPartScheduleFloat lhs,rhs,diff;
  float v1[]={ 1., 2., 3., 4. };
  float v2[]={ 4., 3., 2., 1. };
  LgFrTimeVecFloat vec1(4,v1), vec2(4,v2);
  lhs.insertKeyAndValue( p1, vec1 );
  lhs.insertKeyAndValue( p2, vec2 );
  diff = lhs.op(
#ifdef WIN32
  // Other compilers don't need this cast-not req'd for MSVC4, req'd for MSVC2
  (LgFrTimeVecFloatOpFunction)
#endif
  &LgFrTimeVecFloat::subtract, rhs );
  // std::cout <<"diff" <<std::endl;
  // std::cout <<diff.format() <<std::endl;
  assert( diff.size() == 2 );
  assert( diff[0].item() == p1 );
  assert( diff[1].item() == p2 );
  assert( diff[0].timeVecFloat() == vec1 );
  assert( diff[1].timeVecFloat() == vec2 );
  }
  {   // empty lhs
  LgFrSortingPartScheduleFloat lhs,rhs,diff;
  float v1[]={ 1., 2., 3., 4. };
  float v2[]={ 4., 3., 2., 1. };
  LgFrTimeVecFloat vec1(4,v1), vec2(4,v2);
  rhs.insertKeyAndValue( p2, vec1 );
  rhs.insertKeyAndValue( p3, vec2 );
  diff = lhs.op(
#ifdef WIN32
  // Other compilers don't need this cast-not req'd for MSVC4, req'd for MSVC2
  (LgFrTimeVecFloatOpFunction)
#endif
  &LgFrTimeVecFloat::subtract, rhs );
  // std::cout <<"diff" <<std::endl;
  // std::cout <<diff.format() <<std::endl;
  assert( diff.size() == 2 );
  assert( diff[0].item() == p2 );
  assert( diff[1].item() == p3 );
  assert( diff[0].timeVecFloat() == 0.0-vec1 );
  assert( diff[1].timeVecFloat() == 0.0 -vec2 );
  }
  {   // empty lhs and rhs
  LgFrSortingPartScheduleFloat lhs,rhs,diff;
  diff = lhs.op(
#ifdef WIN32
  // Other compilers don't need this cast-not req'd for MSVC4, req'd for MSVC2
  (LgFrTimeVecFloatOpFunction)
#endif
  &LgFrTimeVecFloat::subtract, rhs );
  // std::cout <<"diff" <<std::endl;
  // std::cout <<diff.format() <<std::endl;
  assert( diff.size() == 0 );
  }

  // Test operation- 
  // (schedule - schedule)
  {
  LgFrPart p1 ("IBMPS2mod95");
  LgFrPart p2 ("IBMPS2mod90serial45conf78");
  LgFrSortingPartScheduleFloat lhs,rhs,diff;
  float v1[]={ 1., 2., 3., 4. };
  float v2[]={ 4., 3., 2., 1. };
  LgFrTimeVecFloat vec1(4,v1), vec2(4,v2);
  lhs.insertKeyAndValue( p1, vec1 );
  lhs.insertKeyAndValue( p2, vec1 );
  rhs.insertKeyAndValue( p2, vec2 );
  rhs.insertKeyAndValue( p3, vec2 );
  diff = lhs-rhs;
  // std::cout <<"diff" <<std::endl;
  // std::cout <<diff.format() <<std::endl;
  assert( diff.size() == 3 );
  assert( diff[0].item() == p1 );
  assert( diff[1].item() == p2 );
  assert( diff[2].item() == p3 );
  assert( diff[0].timeVecFloat() == vec1 );
  assert( diff[1].timeVecFloat() == vec1-vec2 );
  assert( diff[2].timeVecFloat() == 0.0 -vec2 );
  }

  // Test operation+
  // (schedule + schedule)
  {
  LgFrPart p1 ("IBMPS2mod95");
  LgFrPart p2 ("IBMPS2mod90serial45conf78");
  LgFrSortingPartScheduleFloat lhs,rhs,result;
  float v1[]={ 1., 2., 3., 4. };
  float v2[]={ 4., 3., 2., 1. };
  LgFrTimeVecFloat vec1(4,v1), vec2(4,v2);
  lhs.insertKeyAndValue( p1, vec1 );
  lhs.insertKeyAndValue( p2, vec1 );
  rhs.insertKeyAndValue( p2, vec2 );
  rhs.insertKeyAndValue( p3, vec2 );
  result = lhs+rhs;
  // std::cout <<"result" <<std::endl;
  // std::cout <<result.format() <<std::endl;
  assert( result.size() == 3 );
  assert( result[0].item() == p1 );
  assert( result[1].item() == p2 );
  assert( result[2].item() == p3 );
  assert( result[0].timeVecFloat() == vec1 );
  assert( result[1].timeVecFloat() == vec1+vec2 );
  assert( result[2].timeVecFloat() == 0.0 +vec2 );
  }

  // Test operation*
  // (schedule * float) and (float * schedule)
  {
  LgFrPart p1 ("IBMPS2mod95");
  LgFrPart p2 ("IBMPS2mod90serial45conf78");
  LgFrSortingPartScheduleFloat lhs,rhs,result;
  float v1[]={ 1.f, 2.f, 3.f, 4.f };
  float v2[]={ 4.f, 3.f, 2.f, 1.f };
  LgFrTimeVecFloat vec1(4,v1), vec2(4,v2);
  lhs.insertKeyAndValue( p1, vec1 );
  lhs.insertKeyAndValue( p2, vec2 );
  result = lhs * 3.14f;
  // std::cout <<"result" <<std::endl;
  // std::cout <<result.format() <<std::endl;
  assert( result.size() == 2 );
  assert( result[0].item() == p1 );
  assert( result[1].item() == p2 );
  assert( result[0].timeVecFloat() == vec1*3.14f );
  assert( result[1].timeVecFloat() == vec2*3.14f );
  assert( result == ( 3.14f * lhs ) );
  }


  // Test operation/
  // (schedule / float)
  {
  LgFrPart p1 ("IBMPS2mod95");
  LgFrPart p2 ("IBMPS2mod90serial45conf78");
  LgFrSortingPartScheduleFloat lhs,rhs,result;
  float v1[]={ 1., 2., 3., 4. };
  float v2[]={ 4., 3., 2., 1. };
  LgFrTimeVecFloat vec1(4,v1), vec2(4,v2);
  lhs.insertKeyAndValue( p1, vec1 );
  lhs.insertKeyAndValue( p2, vec2 );
  result = lhs / 3.14f;
  // std::cout <<"result" <<std::endl;
  // std::cout <<result.format() <<std::endl;
  assert( result.size() == 2 );
  assert( result[0].item() == p1 );
  assert( result[1].item() == p2 );
  assert( result[0].timeVecFloat() == vec1 / 3.14f );
  assert( result[1].timeVecFloat() == vec2 / 3.14f);
  }


  // Test operation/
  // (float / schedule)
  {
  LgFrPart p1 ("IBMPS2mod95");
  LgFrPart p2 ("IBMPS2mod90serial45conf78");
  LgFrSortingPartScheduleFloat lhs,rhs,result;
  float v1[]={ 1., 2., 3., 4. };
  float v2[]={ 4., 3., 2., 1. };
  LgFrTimeVecFloat vec1(4,v1), vec2(4,v2);
  rhs.insertKeyAndValue( p1, vec1 );
  rhs.insertKeyAndValue( p2, vec2 );
  result = 123. / rhs;
  // std::cout <<"result" <<std::endl;
  // std::cout <<result.format() <<std::endl;
  assert( result.size() == 2 );
  assert( result[0].item() == p1 );
  assert( result[1].item() == p2 );
  assert( result[0].timeVecFloat() == 123. / vec1 );
  assert( result[1].timeVecFloat() == 123. / vec2 );

  // Test zero numerator
  assert( ( 0. / rhs )[0].timeVecFloat() == 0. );
  assert( ( 0. / rhs )[1].timeVecFloat() == 0. );
  }


  // Test operation+
  // (schedule + float) and (float + schedule)
  {
  LgFrPart p1 ("IBMPS2mod95");
  LgFrPart p2 ("IBMPS2mod90serial45conf78");
  LgFrSortingPartScheduleFloat lhs,rhs,result;
  float v1[]={ 1., 2., 3., 4. };
  float v2[]={ 4., 3., 2., 1. };
  LgFrTimeVecFloat vec1(4,v1), vec2(4,v2);
  lhs.insertKeyAndValue( p1, vec1 );
  lhs.insertKeyAndValue( p2, vec2 );
  result = lhs + 3.14f;
  // std::cout <<"result" <<std::endl;
  // std::cout <<result.format() <<std::endl;
  assert( result.size() == 2 );
  assert( result[0].item() == p1 );
  assert( result[1].item() == p2 );
  assert( result[0].timeVecFloat() == vec1+3.14f );
  assert( result[1].timeVecFloat() == vec2+3.14f );
  assert( result == ( 3.14f + lhs ) );
  }

  // Test operation/
  // (schedule - float)
  {
  LgFrPart p1 ("IBMPS2mod95");
  LgFrPart p2 ("IBMPS2mod90serial45conf78");
  LgFrSortingPartScheduleFloat lhs,rhs,result;
  float v1[]={ 1., 2., 3., 4. };
  float v2[]={ 4., 3., 2., 1. };
  LgFrTimeVecFloat vec1(4,v1), vec2(4,v2);
  lhs.insertKeyAndValue( p1, vec1 );
  lhs.insertKeyAndValue( p2, vec2 );
  result = lhs - 3.14f;
  // std::cout <<"result" <<std::endl;
  // std::cout <<result.format() <<std::endl;
  assert( result.size() == 2 );
  assert( result[0].item() == p1 );
  assert( result[1].item() == p2 );
  assert( result[0].timeVecFloat() == vec1 - 3.14f);
  assert( result[1].timeVecFloat() == vec2 - 3.14f );
  }


  // Test operation/
  // (float - schedule)
  {
  LgFrPart p1 ("IBMPS2mod95");
  LgFrPart p2 ("IBMPS2mod90serial45conf78");
  LgFrSortingPartScheduleFloat lhs,rhs,result;
  float v1[]={ 1., 2., 3., 4. };
  float v2[]={ 4., 3., 2., 1. };
  LgFrTimeVecFloat vec1(4,v1), vec2(4,v2);
  rhs.insertKeyAndValue( p1, vec1 );
  rhs.insertKeyAndValue( p2, vec2 );
  result = 123. - rhs;
  // std::cout <<"result" <<std::endl;
  // std::cout <<result.format() <<std::endl;
  assert( result.size() == 2 );
  assert( result[0].item() == p1 );
  assert( result[1].item() == p2 );
  assert( result[0].timeVecFloat() == 123. - vec1 );
  assert( result[1].timeVecFloat() == 123. - vec2 );
  }


}


// test this class
// Expects the data in testScenario to come from
// "/u/implode/wit/data/examples/diner12/wit.data"
void LgFrSortingPartScheduleFloat::contextTest
  (
   LgFrScenario			& testScenario,
   LgFrWorkingImplosionProblem	& wip,			// testScenario's
   LgFrSetOfParts		& sop			// testScenario's
   )
{
  size_t i = 0;

  //Test set/get of schedule strategy and populate method
  LgFrSortingPartScheduleFloat schedule;
  LgFrSupplyScheduleStrategy supSS(testScenario);
  LgFrProductionScheduleStrategy prodSS(testScenario);
  assert( schedule.attributeScheduleStrategy() == 0 );
  schedule.attributeScheduleStrategy( supSS );
  schedule.attributeScheduleStrategy( prodSS );
  schedule.attributeScheduleStrategy( supSS );

  schedule.populate();

  for ( i=0; i<sop.size(); i++ ) {
    assert( sop[i] == schedule[i].part() );
    assert( wip.timeVec(sop[i]) == schedule[i].timeVecFloat() );
  }
  
  // Test setting of schedules filter strategy
  {
    LgFrSortingPartScheduleFloat schedule;
    LgFrSupplyScheduleStrategy supSS(testScenario);
    LgFrZeroTimeVecFilterStrategy ztvFS;
    schedule.attributeScheduleStrategy( supSS );
    assert( schedule.attributeScheduleStrategy() != 0 );
    schedule.filterStrategy(ztvFS);
  }

  {
    // see if all schedules are properly updated when wip is updated

    LgFrSortingPartScheduleFloatSmartPointer schedule1;
    LgFrSortingPartScheduleFloatSmartPointer schedule2;
    LgFrSupplyScheduleStrategy ss1(testScenario);
    LgFrSupplyScheduleStrategy ss2(ss1);
    schedule1->attributeScheduleStrategy(ss1);
    schedule2->attributeScheduleStrategy(ss2);

    // populate schedules
    ss1.populate( *schedule1 );
    ss2.populate( *schedule2 );  

    // change a schedule
    LgFrPart aPart = schedule1->part(0);

    std::cout << std::endl << "schedule2 before schedule 1 changes" << std::endl
         << schedule2->format() << std::endl;
    
    schedule1->insertKeyAndValue( aPart, 
				  (*schedule1)[0].timeVecFloat() + 2.5
				);
    std::cout << std::endl << "schedule1 after schedule 1 changes" << std::endl
         << schedule1->format() << std::endl;

    std::cout << std::endl << "schedule2 after schedule 1 changes" << std::endl
         << schedule2->format() << std::endl;
  }

  
  // Test columnOp
  {
  LgFrSortingPartScheduleFloat schedA;
  float vA1[]={ 1., 2., 3., 4. };
  float vA2[]={ 4., 3., 2., 1. };
  LgFrTimeVecFloat vecA1(4,vA1), vecA2(4,vA2);
  schedA.insertKeyAndValue( schedule[0].part(), vecA1 );
  schedA.insertKeyAndValue( schedule[1].part(), vecA2 );
  LgFrSortingPartScheduleFloat schedB = schedA.columnOp( 2, ::LgFrAssign, 3.14f );
  LgFrTimeVecFloat vecB1 = vecA1;
  vecB1[2] = 3.14f;
  LgFrTimeVecFloat vecB2 = vecA2;
  vecB2[2] = 3.14f;
  assert( schedB.size() == 2 );
  assert( schedB[0].item() == schedule[0].part() );
  assert( schedB[1].item() == schedule[1].part() );
  assert( schedB[0].timeVecFloat() == vecB1 );
  assert( schedB[1].timeVecFloat() == vecB2 );

  LgFrSortingPartScheduleFloat schedC = schedB.columnOp( 2, ::LgFrMultiply, 2.0 );
  LgFrTimeVecFloat vecC1 = vecB1;
  vecC1[2] = 2 * vecB1[2];
  LgFrTimeVecFloat vecC2 = vecB2;
  vecC2[2] = 2 * vecB2[2];
  assert( schedC.size() == 2 );
  assert( schedC[0].item() == schedule[0].part() );
  assert( schedC[1].item() == schedule[1].part() );
  assert( schedC[0].timeVecFloat() == vecC1 );
  assert( schedC[1].timeVecFloat() == vecC2 );
  }
}
