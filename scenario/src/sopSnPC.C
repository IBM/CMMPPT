#include <iostream.h>

#include <scenario/src/timeVecs.h>
#include <scenario/src/sortSchd.h>
#include <scenario/src/sortSchP.h>
#include <scenario/src/timVComp.h>
#include <scenario/src/sopSnPC.h>
#include <scenario/src/scenario.h>
#include <scenario/src/machdep.h>
#include <scenario/src/partSchP.h>

// Do comparison
// The logic for this method is as follows:
// switch:
// case (only one of the items is in the set of parts)
//	it precedes the other item
// case (neither item is in the set of parts)
//	do comparison based on the itemTimeVec's themselves
// case (both items are in the set of parts)
//	do the comparison based on the timeVec's in the schedule
// endswitch
int
LgFrSetOfPartsSensitivePairCompareFloat::itemTimeVecPairCompareFloat(
   const LgFrItemTimeVecPairFloat & itvpf1,
   const LgFrItemTimeVecPairFloat & itvpf2)
const
{
  assert( setOfPartsPtr_ != NULL );

  LgFrPairCompareFloat basePairCompare;
  RWBoolean item1InSop;	// true iff item 1 is in *setOfPartsPtr_
  RWBoolean item2InSop;	// true iff item 2 is in *setOfPartsPtr_

  // Set values of item*InSop and return the correct answer if
  // both items are in the schedule
  item1InSop  =  setOfPartsPtr_->contains( itvpf1.part() );
  item2InSop  =  setOfPartsPtr_->contains( itvpf2.part() );
  if (item1InSop) {
    
    if (item2InSop) {
      // Both items are in set of parts.
      // Compare the items
      return basePairCompare.itemTimeVecPairCompareFloat( itvpf1, itvpf2 );
    }
  }
     

  // If both items are not in schedule then just compare itemTimeVec's
  if ( !item1InSop && !item2InSop )
     return basePairCompare.itemTimeVecPairCompareFloat( itvpf1, itvpf2 );

  // Only one item is in schedule, so it preceeds the other
  assert ( ( item1InSop && !item2InSop ) || 
	   (!item1InSop &&  item2InSop )   );
  if   ( item1InSop ) return -1;
  else                return  1;
}

// Set Set Of Parts to be used for sorting
void
LgFrSetOfPartsSensitivePairCompareFloat::setOfParts(
   const LgFrSetOfParts & sop )
{
   setOfPartsPtr_ = &sop;
}
   

// Get Set Of Parts
const LgFrSetOfParts &
LgFrSetOfPartsSensitivePairCompareFloat::setOfParts()
const
{
   return *setOfPartsPtr_;
}


// Make clone copy of object - deep copy
LgFrPairCompareFloat *
LgFrSetOfPartsSensitivePairCompareFloat::clone()
const
{
  return new LgFrSetOfPartsSensitivePairCompareFloat(*this);
}

// Make clone copy of object with new Scenario
LgFrPairCompareFloat *
LgFrSetOfPartsSensitivePairCompareFloat::clone(LgFrScenario & newScenario)
const
{
  return new LgFrSetOfPartsSensitivePairCompareFloat(*this, newScenario);
}

// Return comparison strategy identifier
isAReturnType
LgFrSetOfPartsSensitivePairCompareFloat::isA()
const
{
   return id_;
}

// Assignment operator
LgFrSetOfPartsSensitivePairCompareFloat &
LgFrSetOfPartsSensitivePairCompareFloat::operator=(
   const LgFrSetOfPartsSensitivePairCompareFloat& rhs)
{
  assert( id_ == rhs.id_ );
  if (this != &rhs) {		// Check for assignment to self
     (this->LgFrPairCompareFloat::operator= ) (rhs);

     setOfPartsPtr_ = rhs.setOfPartsPtr_;

  }
  return *this;
}


// Copy constructor
LgFrSetOfPartsSensitivePairCompareFloat::
   LgFrSetOfPartsSensitivePairCompareFloat (
      const LgFrSetOfPartsSensitivePairCompareFloat & source)
:  LgFrPairCompareFloat(source),
   setOfPartsPtr_( source.setOfPartsPtr_ ),
   id_(__LGFRSETOFPARTSSENSITIVEPAIRCOMPARE)
{
  assert( id_ == source.id_ );
}


// Copy constructor with new Scenario
LgFrSetOfPartsSensitivePairCompareFloat::
   LgFrSetOfPartsSensitivePairCompareFloat (
      const LgFrSetOfPartsSensitivePairCompareFloat & source,
      LgFrScenario & newScenario)
:  LgFrPairCompareFloat(source, newScenario),
   setOfPartsPtr_( source.setOfPartsPtr_ ),
   id_(__LGFRSETOFPARTSSENSITIVEPAIRCOMPARE)
{
  assert( id_ == source.id_ );
}

// Default Constructor
LgFrSetOfPartsSensitivePairCompareFloat::
   LgFrSetOfPartsSensitivePairCompareFloat ()
: LgFrPairCompareFloat(),
   setOfPartsPtr_(NULL),
   id_(__LGFRSETOFPARTSSENSITIVEPAIRCOMPARE)
{
  // Nothing to do here
}

// Destructor
LgFrSetOfPartsSensitivePairCompareFloat::
   ~LgFrSetOfPartsSensitivePairCompareFloat ()
{
  // Nothing to do here
}



#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrSetOfPartsSensitivePairCompareFloat methods.
void
LgFrSetOfPartsSensitivePairCompareFloat::test()
{
  int i,t;

  // Test default constructor
  LgFrSetOfPartsSensitivePairCompareFloat cs1;
  assert( cs1.id_ == __LGFRSETOFPARTSSENSITIVEPAIRCOMPARE );
  assert( cs1.setOfPartsPtr_==NULL );

  // Test isA method
  assert( cs1.isA() == __LGFRSETOFPARTSSENSITIVEPAIRCOMPARE );

  // Test copy constructor
  LgFrSetOfPartsSensitivePairCompareFloat cs2(cs1);
  assert( cs2.isA() == __LGFRSETOFPARTSSENSITIVEPAIRCOMPARE );
  assert( cs2.setOfPartsPtr_==NULL );

  // Test assignment operator
  LgFrSetOfPartsSensitivePairCompareFloat cs3;
  cs3 = cs2;
  assert( cs3.isA() == cs2.isA() );
  assert( cs3.setOfPartsPtr_==NULL );

  // Test clone method
  LgFrPairCompareFloat *cs4Ptr = cs1.clone();
  assert( cs4Ptr->isA() == cs1.isA() );
  assert( ((LgFrSetOfPartsSensitivePairCompareFloat*)cs4Ptr)->setOfPartsPtr_
          ==NULL );

  // Test destructor
  delete cs4Ptr;

  // Create a set of parts
  LgFrSetOfPartsWithUnmovableParts sop1, sop2;

  // Test setting / getting sop
  cs1.setOfParts( sop2 );
  assert( &sop2 == &cs1.setOfParts() );
  
  // Test itemTimeVecPairCompareFloat
  // method by doing sort and testing result

  // Create a part schedule to sort
  const size_t psSize(10);
  LgFrSortingPartScheduleFloat ps(psSize); 
  const LgFrSortingPartScheduleFloat & constps = ps;
  RWCString partName;
  LgFrPart part;
  LgFrTimeVecFloat tvf(10);
  srand( 1 );
  for ( i=0; ps.entries()<psSize; i++ ) {
     // Generate 2 character partname
     partName = RWCString("Part") +
                RWLocale::global().asString( (long)i );
     // Create part
     if ( i < (psSize-5) )
       part = sop1.newPart(partName);
     else
       part = sop2.newPart(partName);
     // Make sure part was not already created
     if ( ! (ps.contains(part)) ) {
       // Generate random timeVecFloat
       for ( t=0; t<tvf.length(); t++ ) tvf[t] = rand()/10.f; 
       // Insert part and timeVecFloat in schedule
       ps.insertKeyAndValue( part, tvf );
     }
  }

  // Display unsorted schedule
  // cout <<"$$$$$$$$$$ Unsorted $$$$$$$$$$$$$$$" <<endl;
  // cout <<ps.format();

  // Set compare strategy and Sort the part schedule
  ps.sort( &cs1 );

  // Display sorted schedule
  // cout <<"$$$$$$$$$$  Sorted  $$$$$$$$$$$$$$$" <<endl;
  // cout <<ps.format();

  // Test to insure schedule is sorted
  const LgFrSortingPartScheduleFloat & constPs = ps;
  for ( i=0; i<ps.entries(); i++ ) {
    if ( i < (psSize-5) ) {
      assert( sop2.contains( constPs[i].part() ) );
    }
    else {
      assert( sop1.contains( constPs[i].part() ) );
    }
  }

}
