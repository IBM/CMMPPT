// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/demdSchd.h>
#include <scenario/src/demdSchP.h>
#include <scenario/src/machdep.h>
#include <scenario/src/demVolSS.h>

// Set Schedule Strategy
void
LgFrSortingDemandScheduleFloat::attributeScheduleStrategy(
const LgFrDemandScheduleStrategy & attributeScheduleStrategy )
{
   LgFrSortingScheduleFloat::attributeScheduleStrategy( attributeScheduleStrategy );
}

// Get Schedule Strategy
// If this object ceases to exist or the attributeScheduleStrategy is changed,
// then the returned reference will be dangling
const LgFrDemandScheduleStrategy *
LgFrSortingDemandScheduleFloat::attributeScheduleStrategy()
const
{
   return (const LgFrDemandScheduleStrategy *)
      LgFrSortingScheduleFloat::attributeScheduleStrategy();
}

// Add ItemTimeVecPairFloat to demand schedule
void
LgFrSortingDemandScheduleFloat::insert (
       const LgFrItemTimeVecPairFloat itvpf )
{
   assert( &(itvpf.item()) != 0 );
   assert( itvpf.item().isADemand() );
   LgFrSortingScheduleFloat::insert( itvpf );
}

// Add demand (demand) and timeVecFloat(tvf) to the end of the collection
void
LgFrSortingDemandScheduleFloat::insert (
       const LgFrDemand& demand,
       const LgFrTimeVecFloat tvf)
{
   insert( LgFrItemTimeVecPairFloat( demand, tvf ) );
}

// Insert ItemTimeVecPairFloat to demand schedule
void
LgFrSortingDemandScheduleFloat::insertAt (
       const size_t index,
       const LgFrItemTimeVecPairFloat itvpf )
{
   assert( itvpf.item().isADemand() );
   LgFrSortingScheduleFloat::insertAt( index, itvpf );
}

// Add demand (demand) and timeVecFloat(tvf) to the end of the collection
void
LgFrSortingDemandScheduleFloat::insertAt (
       const size_t index,
       const LgFrDemand& demand,
       const LgFrTimeVecFloat tvf)
{
   insertAt( index, LgFrItemTimeVecPairFloat( demand, tvf ) );
}
  
// Insert Demand if it isn't already in the schedule.  In any case,
// associate timeVec with demand.
void
LgFrSortingDemandScheduleFloat::insertKeyAndValue(const LgFrDemand& demand,
					 const LgFrTimeVecFloat& value)
{
  this -> LgFrSortingScheduleFloat::insertKeyAndValue (demand, value);
}


// Return demand at location i
LgFrDemand
LgFrSortingDemandScheduleFloat::demand (const size_t i)
     const
{
  assert (  i < this->size()  );
  return (  (*this)[i]  . demand()  );
}

// Make clone copy of object
LgFrSortingScheduleFloat *
LgFrSortingDemandScheduleFloat::clone()
const
{
  LgFrSortingDemandScheduleFloat* newInst = 
      new LgFrSortingDemandScheduleFloat(*this);
  return (newInst);
}

// Make clone copy of object with newScenario
LgFrSortingScheduleFloat *
LgFrSortingDemandScheduleFloat::clone(LgFrScenario& newScenario)
const
{ 
  LgFrSortingDemandScheduleFloat* newInst = 
      new LgFrSortingDemandScheduleFloat(*this, newScenario);
  return (newInst);
}

LgFrItemTimeVecPairFloat &
LgFrSortingDemandScheduleFloat::operator[](const LgFrDemand& demand)
{
  return (this->LgFrSortingScheduleFloat::operator[](demand));
}

const LgFrItemTimeVecPairFloat &
LgFrSortingDemandScheduleFloat::operator[](const LgFrDemand& demand)
const
{
  return (this->LgFrSortingScheduleFloat::operator[](demand));
}

  
LgFrItemTimeVecPairFloat
LgFrSortingDemandScheduleFloat::operator[](const size_t& i) const
{
  return (this->LgFrSortingScheduleFloat::operator[](i));
}

  
LgFrItemTimeVecPairFloat &
LgFrSortingDemandScheduleFloat::operator[](const size_t& i)
{
  return (this->LgFrSortingScheduleFloat::operator[](i));
}
  
// This should not be used by clients of this class.  Since it is a
// virtual method in the base class, it is used by the base
// class when *this is a LgFrSortingDemandScheduleFloat.
LgFrItemTimeVecPairFloat &
LgFrSortingDemandScheduleFloat::operator[](const LgFrItem& item)
{
  assert (item.isADemand());
  return this->LgFrSortingScheduleFloat::operator[](item);
}

// This should not be used by clients of this class.  Since it is a
// virtual method in the base class, it is used by the base
// class when *this is a LgFrSortingDemandScheduleFloat.
const LgFrItemTimeVecPairFloat &
LgFrSortingDemandScheduleFloat::operator[](const LgFrItem& item)
const
{
  assert (item.isADemand());
  return this->LgFrSortingScheduleFloat::operator[](item);
}


// default constructor
LgFrSortingDemandScheduleFloat::LgFrSortingDemandScheduleFloat(const size_t n)
: LgFrSortingScheduleFloat ( n ) //(size_tMax (n, 1)) )
{
   // Nothing to do here
}
  
// Copy constructor
LgFrSortingDemandScheduleFloat::LgFrSortingDemandScheduleFloat (
   const LgFrSortingDemandScheduleFloat& source)
: LgFrSortingScheduleFloat ( source )
// : LgFrSortingScheduleFloat ( (LgFrSortingScheduleFloat)(source) )
{
   // Nothing to do here
}
  
// Copy constructor with new Scenario
LgFrSortingDemandScheduleFloat::LgFrSortingDemandScheduleFloat (
   const LgFrSortingDemandScheduleFloat& source,
   LgFrScenario & newScenario)
: LgFrSortingScheduleFloat ( source, newScenario )
{
   // Nothing to do here
}
  
// destructor
LgFrSortingDemandScheduleFloat::~LgFrSortingDemandScheduleFloat()
{
   // Nothing to do here
}
  
// Assignment 
LgFrSortingDemandScheduleFloat &
LgFrSortingDemandScheduleFloat::operator=(
    const LgFrSortingDemandScheduleFloat& rhs)
{
  if (this != &rhs) {           // Check for assignment to self
    (this->LgFrSortingScheduleFloat::operator= ) (rhs);
  }

  // Test assignment operator result
  assert (this->size() == rhs.size());

  return *this;
}

// Perform diadic schedule operation.
LgFrSortingDemandScheduleFloat
LgFrSortingDemandScheduleFloat::op(
   LgFrTimeVecFloatOpFunction vectorOp,
   const LgFrSortingDemandScheduleFloat& rhs,
   float defaultValue )
const
{
   LgFrSortingDemandScheduleFloat retVal;

   kernalOfOp( vectorOp, rhs, retVal, defaultValue );

   return retVal;
}


// Perform diadic schedule operation.
LgFrSortingDemandScheduleFloat
LgFrSortingDemandScheduleFloat::op(
   LgFrTimeVecFloatRhsScalarOpFunction scalarOp, 
   const float rhs )
const
{
   LgFrSortingDemandScheduleFloat retVal;

   kernalOfOp( scalarOp, rhs, retVal );

   return retVal;
}


// Perform diadic schedule operation.
LgFrSortingDemandScheduleFloat
LgFrSortingDemandScheduleFloat::op(
   const float lhs,
   LgFrTimeVecFloatLhsScalarOpFunction scalarOp )
const
{
   LgFrSortingDemandScheduleFloat retVal;

   kernalOfOp( lhs, scalarOp, retVal );

   return retVal;
}


// perform scalar operation on column (period) in schedule
LgFrSortingDemandScheduleFloat
LgFrSortingDemandScheduleFloat::columnOp(
  const size_t period,
  float (*scalarOperation) (const float, const float),
  const float rhs )
const
{
   LgFrSortingDemandScheduleFloat retVal;

   kernalOfColumnOp(  period, scalarOperation, rhs, retVal );

   return retVal;
}
  
// Addition operator
// (schedule + schedule)
LgFrSortingDemandScheduleFloat
operator+ (
   const LgFrSortingDemandScheduleFloat& lhs, 
   const LgFrSortingDemandScheduleFloat& rhs)
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
LgFrSortingDemandScheduleFloat
operator- (
   const LgFrSortingDemandScheduleFloat& lhs,
   const LgFrSortingDemandScheduleFloat& rhs)
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
LgFrSortingDemandScheduleFloat
operator* (
   const LgFrSortingDemandScheduleFloat& lhs,
   const float rhs)
{
  return lhs.op ( operator*, rhs );
}
// (float * schedule)
LgFrSortingDemandScheduleFloat
operator* (
   const float lhs,
   const LgFrSortingDemandScheduleFloat& rhs)
{
  return rhs.op ( lhs, operator*);
}


// Division operator
// (schedule / float)
LgFrSortingDemandScheduleFloat
operator/ (
   const LgFrSortingDemandScheduleFloat& lhs,
   const float rhs)
{
  return lhs.op ( operator/, rhs );
}
// (float / schedule)
LgFrSortingDemandScheduleFloat
operator/ (
   const float lhs,
   const LgFrSortingDemandScheduleFloat& rhs)
{
  return rhs.op( lhs, operator/ );
}


// Addition operator
// (schedule + float)
LgFrSortingDemandScheduleFloat
operator+ (
   const LgFrSortingDemandScheduleFloat& lhs,
   const float rhs)
{
  return lhs.op ( operator+, rhs);
}
// (float + schedule)
LgFrSortingDemandScheduleFloat
operator+ (
   const float lhs,
   const LgFrSortingDemandScheduleFloat& rhs)
{
  return rhs.op ( lhs, operator+);
}


// Subtraction operator
// (schedule - float)
LgFrSortingDemandScheduleFloat
operator- (
   const LgFrSortingDemandScheduleFloat& lhs,
   const float rhs)
{
  return lhs.op ( operator-, rhs );
}
// (float - schedule)
LgFrSortingDemandScheduleFloat
operator- (
   const float lhs,
   const LgFrSortingDemandScheduleFloat& rhs)
{
  return rhs.op ( lhs, operator- );
}

  

#ifdef NDEBUG
#undef NDEBUG
#endif

// test this class
void LgFrSortingDemandScheduleFloat::test()
{
  // Make some demand for the test
  LgFrPart p1( "part1" );
  LgFrPart p2( "part2" );
  LgFrPart p3( "part3" );
  LgFrDemand d1( "demand1", &p1 );
  LgFrDemand d2( "demand2", &p2 );
  LgFrDemand d3( "demand3", &p3 );

  // Make some timeVecFloat for the test
  LgFrTimeVecFloat tvf1(10,1.11f);
  LgFrTimeVecFloat tvf2(10,2.22f);

  // Make some demand timeVecFloat pairs for the test
  LgFrItemTimeVecPairFloat dtvfp1( d1, tvf1 );
  LgFrItemTimeVecPairFloat dtvfp2( d2, tvf1 );
  LgFrItemTimeVecPairFloat dtvfp3( d3, tvf2 );
  LgFrItemTimeVecPairFloat dtvfp4( d1, tvf2 );

  // Test the insertion methods
  LgFrSortingDemandScheduleFloat ss (1);	// Make it a little small
  				// to test automatic resizing
  ss.insert(dtvfp2);
  ss.insert(d3, tvf2);          // equivalent to ss.insert(ptvfp3)
  ss.insertAt(0, d1, tvf1);     // equivalent to ss.insertAt(0, ptvfp1);
  assert (3 == ss.size());
  assert (ss[0].item().name() == "demand1");
  assert (ss[0].demand().partPtr() == &p1);
  assert (ss[2].item().name() == "demand3");
  assert (ss[2].demand().partPtr() == &p3);
  assert (ss[1].item().name() == "demand2");

  // Test the assignment operator and the copy constructor
  LgFrSortingDemandScheduleFloat ss1 (1);	// Make it a little small
  				// to test resizing at assignment time
  ss1.insert(dtvfp4);		
  ss1 = ss;
  LgFrSortingDemandScheduleFloat ss2 (ss);

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

  // Test to ensure that an item without a defined type cannot be 
  // inserted.
  // ss.push_back( LgFrItemTimeVecPairFloat() );  // <-- assert in insert 
                                               //     should fail 
  // Test indexing by item
  assert( ss[d3] == dtvfp3 );
  const LgFrSortingScheduleFloat constSs(ss);
  assert( constSs[d3] == dtvfp3 );

  // Test resize ... no more, we let the tools do their job
  //LgFrSortingDemandScheduleFloat ss3(0);
  //ss3.resize(3);
  //assert (ss3.size() == 0);

  // Test op method
  {
  LgFrSortingDemandScheduleFloat lhs,rhs,diff;
  float v1[]={ 1., 2., 3., 4. };
  float v2[]={ 4., 3., 2., 1. };
  LgFrTimeVecFloat vec1(4,v1), vec2(4,v2);
  lhs.insertKeyAndValue( d1, vec1 );
  lhs.insertKeyAndValue( d2, vec1 );
  rhs.insertKeyAndValue( d2, vec2 );
  rhs.insertKeyAndValue( d3, vec2 );
  diff = lhs.op(
#ifdef WIN32
  // Other compilers don't need this cast-not req'd for MSVC4, req'd for MSVC2
  (LgFrTimeVecFloatOpFunction)
#endif
  &LgFrTimeVecFloat::subtract, rhs );
  // std::cout <<"diff" <<std::endl;
  // std::cout <<diff.format() <<std::endl;
  assert( diff.size() == 3 );
  assert( diff[0].item() == d1 );
  assert( diff[1].item() == d2 );
  assert( diff[2].item() == d3 );
  assert( diff[0].timeVecFloat() == vec1 );
  assert( diff[1].timeVecFloat() == vec1-vec2 );
  assert( diff[2].timeVecFloat() == 0.0 -vec2 );
  }
  {  // empty rhs
  LgFrSortingDemandScheduleFloat lhs,rhs,diff;
  float v1[]={ 1., 2., 3., 4. };
  float v2[]={ 4., 3., 2., 1. };
  LgFrTimeVecFloat vec1(4,v1), vec2(4,v2);
  lhs.insertKeyAndValue( d1, vec1 );
  lhs.insertKeyAndValue( d2, vec2 );
  diff = lhs.op(
#ifdef WIN32
  // Other compilers don't need this cast-not req'd for MSVC4, req'd for MSVC2
  (LgFrTimeVecFloatOpFunction)
#endif
  &LgFrTimeVecFloat::subtract, rhs );
  // std::cout <<"diff" <<std::endl;
  // std::cout <<diff.format() <<std::endl;
  assert( diff.size() == 2 );
  assert( diff[0].item() == d1 );
  assert( diff[1].item() == d2 );
  assert( diff[0].timeVecFloat() == vec1 );
  assert( diff[1].timeVecFloat() == vec2 );
  }
  {   // empty lhs
  LgFrSortingDemandScheduleFloat lhs,rhs,diff;
  float v1[]={ 1., 2., 3., 4. };
  float v2[]={ 4., 3., 2., 1. };
  LgFrTimeVecFloat vec1(4,v1), vec2(4,v2);
  rhs.insertKeyAndValue( d2, vec1 );
  rhs.insertKeyAndValue( d3, vec2 );
  diff = lhs.op(
#ifdef WIN32
  // Other compilers don't need this cast-not req'd for MSVC4, req'd for MSVC2
  (LgFrTimeVecFloatOpFunction)
#endif
  &LgFrTimeVecFloat::subtract, rhs );
  // std::cout <<"diff" <<std::endl;
  // std::cout <<diff.format() <<std::endl;
  assert( diff.size() == 2 );
  assert( diff[0].item() == d2 );
  assert( diff[1].item() == d3 );
  assert( diff[0].timeVecFloat() == 0.0-vec1 );
  assert( diff[1].timeVecFloat() == 0.0 -vec2 );
  }
  {   // empty lhs and rhs
  LgFrSortingDemandScheduleFloat lhs,rhs,diff;
  float v1[]={ 1., 2., 3., 4. };
  float v2[]={ 4., 3., 2., 1. };
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

  // Test operation- method
  {
  LgFrSortingDemandScheduleFloat lhs,rhs,diff;
  float v1[]={ 1., 2., 3., 4. };
  float v2[]={ 4., 3., 2., 1. };
  LgFrTimeVecFloat vec1(4,v1), vec2(4,v2);
  lhs.insertKeyAndValue( d1, vec1 );
  lhs.insertKeyAndValue( d2, vec1 );
  rhs.insertKeyAndValue( d2, vec2 );
  rhs.insertKeyAndValue( d3, vec2 );
  diff = lhs - rhs;
  // std::cout <<"diff" <<std::endl;
  // std::cout <<diff.format() <<std::endl;
  assert( diff.size() == 3 );
  assert( diff[0].item() == d1 );
  assert( diff[1].item() == d2 );
  assert( diff[2].item() == d3 );
  assert( diff[0].timeVecFloat() == vec1 );
  assert( diff[1].timeVecFloat() == vec1-vec2 );
  assert( diff[2].timeVecFloat() == 0.0 -vec2 );
  }

  // Test operation+ method
  {
  LgFrSortingDemandScheduleFloat lhs,rhs,result;
  float v1[]={ 1., 2., 3., 4. };
  float v2[]={ 4., 3., 2., 1. };
  LgFrTimeVecFloat vec1(4,v1), vec2(4,v2);
  lhs.insertKeyAndValue( d1, vec1 );
  lhs.insertKeyAndValue( d2, vec1 );
  rhs.insertKeyAndValue( d2, vec2 );
  rhs.insertKeyAndValue( d3, vec2 );
  result = lhs + rhs;
  // std::cout <<"result" <<std::endl;
  // std::cout <<result.format() <<std::endl;
  assert( result.size() == 3 );
  assert( result[0].item() == d1 );
  assert( result[1].item() == d2 );
  assert( result[2].item() == d3 );
  assert( result[0].timeVecFloat() == vec1 );
  assert( result[1].timeVecFloat() == vec1+vec2 );
  assert( result[2].timeVecFloat() == 0.0 +vec2 );
  }

  // Test scalar operation+,-,*,/ methods
  {
  LgFrSortingDemandScheduleFloat x,y,z,r;
  float v1[]={ 1., 2., 3., 4. };
  float v2[]={ 4., 3., 2., 1. };
  LgFrTimeVecFloat vec1(4,v1), vec2(4,v2);
  x.insertKeyAndValue( d1, vec1 );
  x.insertKeyAndValue( d2, vec2 );
  y.insertKeyAndValue( d1, 2.34f*vec1 );
  y.insertKeyAndValue( d2, 2.34f*vec2 );
  z.insertKeyAndValue( d1, 5.6f+vec1 );
  z.insertKeyAndValue( d2, 5.6f+vec2 );
  r.insertKeyAndValue( d1, 7.8f/vec1 );
  r.insertKeyAndValue( d2, 7.8f/vec2 );

  assert( 2.34f*x == y );
  assert( x*2.34f == y );

  assert( y/2.34f == x );
  assert( 7.8f/x == r );

  assert( 5.6f+x == z );
  assert( x+5.6f == z );

  assert( z-5.6f == x );
  assert( 3.13f-x == 3.13f + (-1)*x );
  }
}

// test LgFrSortingDemandScheduleFloat methods.
// Expects the data in testScenario to come from
// "/u/implode/wit/data/examples/diner12/wit.data"
void
LgFrSortingDemandScheduleFloat::contextTest(LgFrScenario& testScenario)
{
  // see if all schedules are properly updated when wip is updated

  LgFrSortingDemandScheduleFloatSmartPointer schedule1;
  LgFrSortingDemandScheduleFloatSmartPointer schedule2;
  LgFrDemandVolumeScheduleStrategy ss1(testScenario);
  LgFrDemandVolumeScheduleStrategy ss2(ss1);
  schedule1->attributeScheduleStrategy(ss1);
  schedule2->attributeScheduleStrategy(ss2);

  // populate schedules
  ss1.populate( *schedule1 );
  ss2.populate( *schedule2 );  

  // change a schedule
  LgFrDemand aDemand = schedule1->demand(0);

  std::cout << std::endl << "schedule2 before schedule 1 changes" << std::endl
      << schedule2->format() << std::endl;
  
  schedule1->insertKeyAndValue( aDemand,
				(*schedule1)[0].timeVecFloat() + 2.5
			      );
  std::cout << std::endl << "schedule1 after schedule 1 changes" << std::endl
      << schedule1->format() << std::endl;

  std::cout << std::endl << "schedule2 after schedule 1 changes" << std::endl
      << schedule2->format() << std::endl;

  
  // Test columnOp
  {
  LgFrSortingDemandScheduleFloat schedA;
  float vA1[]={ 1.f, 2.f, 3.f, 4.f };
  float vA2[]={ 4.f, 3.f, 2.f, 1.f };
  LgFrTimeVecFloat vecA1(4,vA1), vecA2(4,vA2);
  schedA.insertKeyAndValue( schedule1->demand(0), vecA1 );
  schedA.insertKeyAndValue( schedule1->demand(1), vecA2 );
  LgFrSortingDemandScheduleFloat schedB = schedA.columnOp( 2, ::LgFrAssign, 3.14f );
  LgFrTimeVecFloat vecB1 = vecA1;
  vecB1[2] = 3.14f;
  LgFrTimeVecFloat vecB2 = vecA2;
  vecB2[2] = 3.14f;
  assert( schedB.size() == 2 );
  assert( schedB[0].item() == schedule1->demand(0) );
  assert( schedB[1].item() == schedule1->demand(1) );
  assert( schedB[0].timeVecFloat() == vecB1 );
  assert( schedB[1].timeVecFloat() == vecB2 );

  LgFrSortingDemandScheduleFloat schedC =
    schedB.columnOp( 2, ::LgFrMultiply, 2.0 );
  LgFrTimeVecFloat vecC1 = vecB1;
  vecC1[2] = 2 * vecB1[2];
  LgFrTimeVecFloat vecC2 = vecB2;
  vecC2[2] = 2 * vecB2[2];
  assert( schedC.size() == 2 );
  assert( schedC[0].item() == schedule1->demand(0) );
  assert( schedC[1].item() == schedule1->demand(1) );
  assert( schedC[0].timeVecFloat() == vecC1 );
  assert( schedC[1].timeVecFloat() == vecC2 );
  }
}
