// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>


#include <scenario/src/sortSchd.h>
#include <scenario/src/sortSchP.h>
#include <scenario/src/schdStrt.h>
#include <scenario/src/partSS.h>
#include <scenario/src/pairComp.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/machdep.h>


// Global variable used by qsort compare function
LgFrSortingScheduleFloat *LgFrSortingScheduleFloatThisGlobalPtr;
LgFrPairCompareFloat     *LgFrSortingScheduleFloatCompareStrategyGlobalPtr;

// RW2STL - implement (RWGVector, LgFrSortingScheduleFloatSmartPointer) 

// RW2STL - implement (RWGOrderedVector, LgFrSortingScheduleFloatSmartPointer) 



// Regular function passed to qsort to perform comparison
extern "C"
int
LgFrSortingScheduleFloatQsortCompare(
   const void *i1ptr,
   const void *i2ptr )
{
   assert( LgFrSortingScheduleFloatThisGlobalPtr != 0 );
   assert( LgFrSortingScheduleFloatCompareStrategyGlobalPtr != 0 );
   const size_t index1 (*    (  (const size_t *) i1ptr  )  
			), 
                index2 (*    (  (const size_t *) i2ptr  )  
			);
   return
      LgFrSortingScheduleFloatCompareStrategyGlobalPtr->
         itemTimeVecPairCompareFloat(
            LgFrSortingScheduleFloatThisGlobalPtr->
               LgFrScheduleFloat::operator[](index1),
            LgFrSortingScheduleFloatThisGlobalPtr->
               LgFrScheduleFloat::operator[](index2) );
}


// Do sort
void
LgFrSortingScheduleFloat::sort(
   const LgFrPairCompareFloat* const comparisonStrategy,
   const bool ascending)
{
  // Save compareStrategy
  compareStrategy( comparisonStrategy );
  ascending_ = ascending;

  if ( compareStrategyPtr_ != 0 ) {

     // Create permutation vector to be used by qsort
     size_t *pv = new size_t[permutations_.size()];
     size_t i = 0; // Pulled out of the for below by RW2STL
     for ( i=0; i<permutations_.size(); i++ )
        pv[i] = permutations_[i];

     // Get lock on global variables.
     // This function currently just returns.  The locking
     // mechanism must be added if LgFr is running with multiple
     // processes.
     prepareToSort();

     // Assign global variables
     LgFrSortingScheduleFloatThisGlobalPtr = this;
     LgFrSortingScheduleFloatCompareStrategyGlobalPtr = compareStrategyPtr_;

     // Invoke qsort
     qsort( 
        pv,
        permutations_.size(), 
        sizeof(size_t),
        LgFrSortingScheduleFloatQsortCompare );

     // Set global variables to 0
     LgFrSortingScheduleFloatThisGlobalPtr = 0;
     LgFrSortingScheduleFloatCompareStrategyGlobalPtr = 0;

     // Release lock on global variables.
     // This function currently just returns.  The locking
     // mechanism must be added if LgFr is running with multiple
     // processes.
     doneSorting();

     // Assign sorted order to permutation vector
     for ( i=0; i<permutations_.size(); i++ )
        permutations_[i] = pv[i];
     delete [] pv;

  }
  else
  {
     // Return to original unsorted order
     size_t i = 0; // Pulled out of the for below by RW2STL
     for ( i=0; i<permutations_.size(); i++ )
        permutations_[i] = i;
  }

  // If sort order is not ascending then reverse order of permuation
  // vector.
  if ( !ascending ) {
     size_t i,j,temp;
     if ( permutations_.size() >0 ) {
        for ( i=0, j=permutations_.size() - 1; 
              i<j; 
              i++, j-- ) {
              temp = permutations_[i];
              permutations_[i] = permutations_[j];
              permutations_[j] = temp;
        }
     }
  }

}

// Set comparison strategy
void
LgFrSortingScheduleFloat::compareStrategy (
   const LgFrPairCompareFloat * const cs )
{
   if ( compareStrategyPtr_ != cs ) {
      if ( compareStrategyPtr_ != 0 ) 
         compareStrategyPtr_->removeDependent( this );
      delete compareStrategyPtr_;
      if ( cs != 0 )
         compareStrategyPtr_ = cs->clone();
      else
         compareStrategyPtr_ = 0;
      if ( compareStrategyPtr_ != 0 ) 
         compareStrategyPtr_->addDependent( this );
   }
}

// Get comparison strategy
const LgFrPairCompareFloat * 
LgFrSortingScheduleFloat::compareStrategy ()
const
{
   return compareStrategyPtr_;
}

// Return true if schedule is in original unsorted order.
bool
LgFrSortingScheduleFloat::isInNaturalOrder()
const
{
   if ( compareStrategyPtr_ == 0 && ascending_ ) return true;
   else return false;
#if 0
   size_t i = 0; // Pulled out of the for below by RW2STL
   for ( i=0; i<permutations_.size(); i++ )
      if ( permutations_[i] != i ) return false;
   return true;
#endif
}

// Return true if schedule is sorted in ascending order
bool
LgFrSortingScheduleFloat::isSortAscending()
const
{
   return ascending_;
}

// Fill in schedule using attributeScheduleStrategy

// this has to be made a smart pointer method!!
void
LgFrSortingScheduleFloat::populate()
{
   assert ((attributeScheduleStrategyPtr_ != 0) && ("ERROR: cannot populate() a schedule without a ScheduleStragety assigned to it!"));
   attributeScheduleStrategyPtr_->populate( *this );
}

// Set Filter Strategy
void
LgFrSortingScheduleFloat::filterStrategy(
const LgFrFilterStrategy & filterStrategy )
{
   assert( filterStrategyPtr_ != 0 );
   filterStrategyPtr_->removeDependent( this );
   delete filterStrategyPtr_;
   filterStrategyPtr_ = filterStrategy.clone();
   filterStrategyPtr_->addDependent( this );
}



// Get Filter Strategy
// If this object ceases to exist or the attributeScheduleStrategy is changed,
// then the returned reference will be dangling
const LgFrFilterStrategy &
LgFrSortingScheduleFloat::filterStrategy()
const
{
   assert (filterStrategyPtr_ != 0);
   return *filterStrategyPtr_;
}


bool
LgFrSortingScheduleFloat::isMutable ()
{
  return (
	  (attributeScheduleStrategyPtr_ == 0)
	  ? true
	  : this->attributeScheduleStrategy()->isMutable()
	  );
}

LgFrValidity
LgFrSortingScheduleFloat::validity ( const LgFrTimeVecFloat& proposedTimeVec )
{
  return (
	  (attributeScheduleStrategyPtr_ == 0)
	  ? INVALID
	  : this->attributeScheduleStrategy()->validity( proposedTimeVec )
	  );
}

// Add new item to the end of the collection
void
LgFrSortingScheduleFloat::insert (const LgFrItemTimeVecPairFloat pair)
{
  assert (this->isMutable());
  this->localInsert (pair);
  this->propagateUpdate(pair); 
  this->changed(0);
}

// Add new item to the end of the collection without propagating the change 
// to other parts of the system
void
LgFrSortingScheduleFloat::localInsert (const LgFrItemTimeVecPairFloat & pair)
{
  if ( permutations_.size() == permutations_.capacity() )
      permutations_.reserve( 2*capacity() + 1 ); // LgFrScheduleFloat::insert adjusts its own capacity

  LgFrScheduleFloat::insert( pair );
  permutations_.push_back( permutations_.size() );
  assert( LgFrScheduleFloat::size() == permutations_.size() );
}


// Add new item to the collection at position index.  Everything afterwards
// is shifted to a higher index.
void
LgFrSortingScheduleFloat::insertAt (const size_t index, 
				    const LgFrItemTimeVecPairFloat pair)
{
  assert (this->isMutable());
  this->localInsertAt (index, pair);
  this->propagateUpdate(pair);
  this->changed(0);
}


// Add new item to the collection at position index without propagating the
// change to other parts of the system.  Everything afterwards
// is shifted to a higher index.
void
LgFrSortingScheduleFloat::localInsertAt (const size_t index, 
					 const LgFrItemTimeVecPairFloat& pair)
{
  if ( permutations_.size() == permutations_.capacity() )
      permutations_.reserve( 2*capacity() + 1 ); // LgFrScheduleFloat::insert adjusts its own capacity

   LgFrScheduleFloat::insertAt( index, pair );
   size_t i = 0; // Pulled out of the for below by RW2STL
   for ( i=0; i<permutations_.size(); i++ )
      if ( permutations_[i] >= index ) permutations_[i]++;
   assert (index < permutations_.size());
   permutations_.insert(permutations_.begin()+index, index );
}


// Insert Item if it isn't already in the schedule.  In any case,
// associate timeVec with item.
void
LgFrSortingScheduleFloat::insertKeyAndValue(
   const LgFrItem& item, 
   const LgFrTimeVecFloat& value)
{
  assert (this->isMutable());
  localInsertKeyAndValue (item, value);
  LgFrItemTimeVecPairFloat pair (item, value);
  this->propagateUpdate(pair);
  this->changed(0);
}

// Insert Item if it isn't already in the schedule without propagating
// the change to other parts of the system.  In any case,
// associate timeVec with item.
void
LgFrSortingScheduleFloat::localInsertKeyAndValue(
   const LgFrItem& item, 
   const LgFrTimeVecFloat& value)
{
  if (  LgFrScheduleFloat::contains(item)   ) {
    // get the itemTimeVec and change it's timeVec part
    (LgFrScheduleFloat::operator[](item)) . timeVecFloat (value);    
  }
  else {
    this->localInsert (LgFrItemTimeVecPairFloat(item, value) );
  }
}


// Make clone copy of object
LgFrSortingScheduleFloat *
LgFrSortingScheduleFloat::clone()
const
{
  LgFrSortingScheduleFloat* newInst = 
      new LgFrSortingScheduleFloat(*this);
  return (newInst);
}

// Make clone copy of object with new Scenario
LgFrSortingScheduleFloat *
LgFrSortingScheduleFloat::clone(LgFrScenario & newScenario)
const
{
  LgFrSortingScheduleFloat* newInst = 
      new LgFrSortingScheduleFloat(*this, newScenario);
  return (newInst);
}











// Return the i'th ItemTimeVecPairFloat
LgFrItemTimeVecPairFloat
LgFrSortingScheduleFloat::operator[](const size_t& i )
const
{  
   return LgFrScheduleFloat::operator[](permutations_[i]);
}

LgFrItemTimeVecPairFloat &
LgFrSortingScheduleFloat::operator[](const size_t& i )
{  
   return LgFrScheduleFloat::operator[](permutations_[i]);
}


// Return the LgFrItemTimeVecPairFloat whose item matches item.
// If there is no such LgFrItemTimeVecPairFloat, a new one is created
// (using the default LgFrTimeVecFloat constructor) and inserted at
// the end.
LgFrItemTimeVecPairFloat &
LgFrSortingScheduleFloat::operator[](const LgFrItem& item)
{
  if (  LgFrScheduleFloat::contains(item)   ) {
    return LgFrScheduleFloat::operator[](item);
  }
  else {
    this->insert (LgFrItemTimeVecPairFloat(item,
					   LgFrTimeVecFloat())  // default t-v
		  );
    return (*this)[permutations_[ permutations_.size() - 1 ] ];
  }
}

// Return the LgFrItemTimeVecPairFloat whose item matches item.
// If there is no such LgFrItemTimeVecPairFloat, raise an assert error.
const LgFrItemTimeVecPairFloat &
LgFrSortingScheduleFloat::operator[](const LgFrItem& item)
const
{
  assert ( LgFrScheduleFloat::contains(item) );
  return LgFrScheduleFloat::operator[](item);
}


#if 0
// Remove all pairs from schedule without propagating
// the change to other parts of the system.
// Pairs are PairsOfItemTimeVecFloat
void
LgFrSortingScheduleFloat::clear()
{
   assert (this->isMutable());
   // Propagate the change through the attribute schedule strategy	
   if (attributeScheduleStrategyPtr_ != 0) {
      if ( this->size() > 0 ) {
         LgFrTimeVecFloat zeroTV( (*this)[0].timeVecFloat().length(), 
                                   0.0 );
         int i = 0; // Pulled out of the for below by RW2STL
         for (i = 0;  i < this->size(); ++i) {
            LgFrItemTimeVecPairFloat pair ((*this)[i].part(),
    	                                   zeroTV );
            this->propagateUpdate(pair);	
         }
      }
   }
   this->localClear();
}
#endif

// Remove all pairs from schedule.
// Pairs are PairsOfItemTimeVecFloat
void
LgFrSortingScheduleFloat::localClear()
{
   LgFrScheduleFloat::clear();
   permutations_.clear();
}

// Change the size of the schedule
void
LgFrSortingScheduleFloat::reserve (const size_t newCapacity)
{
   assert (newCapacity  >=  this->size());	// Not done yet: have to do 
                                                // propagateUpdate on deleted
                                                // entries
   LgFrScheduleFloat::reserve(newCapacity);
   permutations_.reserve(newCapacity);
}


// Assignment operator
LgFrSortingScheduleFloat &
LgFrSortingScheduleFloat::operator=(
			  const LgFrSortingScheduleFloat& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrScheduleFloat::operator= ) (rhs);

    permutations_ = rhs.permutations_;
    assert (permutations_.size() == rhs.permutations_.size());

    if ( compareStrategyPtr_ != 0 ) 
       compareStrategyPtr_->removeDependent( this );
    delete compareStrategyPtr_;
    if ( rhs.compareStrategyPtr_ != 0 )
       compareStrategyPtr_ = rhs.compareStrategyPtr_->clone();
    else
       compareStrategyPtr_ = 0;
    if ( compareStrategyPtr_ != 0 ) 
       compareStrategyPtr_->addDependent( this );

    if ( attributeScheduleStrategyPtr_ != 0 ) 
       attributeScheduleStrategyPtr_->removeDependent( this );
    delete attributeScheduleStrategyPtr_;
    if ( rhs.attributeScheduleStrategyPtr_ != 0 )
       attributeScheduleStrategyPtr_ = rhs.attributeScheduleStrategyPtr_->clone();
    else
       attributeScheduleStrategyPtr_ = 0;
    if ( attributeScheduleStrategyPtr_ != 0 ) 
       attributeScheduleStrategyPtr_->addDependent( this );

    assert( filterStrategyPtr_ != 0 );
    assert( rhs.filterStrategyPtr_ != 0 );
    filterStrategyPtr_->removeDependent( this );
    delete filterStrategyPtr_;
    filterStrategyPtr_ = rhs.filterStrategyPtr_->clone();
    filterStrategyPtr_->addDependent( this );

    ascending_ = rhs.ascending_;
  }
  return *this;
}


// Copy constructor
LgFrSortingScheduleFloat::LgFrSortingScheduleFloat (
   const LgFrSortingScheduleFloat & source)
:   LgFrScheduleFloat(source),
    permutations_(source.permutations_),
    compareStrategyPtr_(0),
    attributeScheduleStrategyPtr_(0),
    filterStrategyPtr_(0),
    ascending_(source.ascending_)
{
    if ( source.compareStrategyPtr_ != 0 )
       compareStrategyPtr_ = source.compareStrategyPtr_->clone();
    else
       compareStrategyPtr_ = 0;
    if ( compareStrategyPtr_ != 0 ) 
       compareStrategyPtr_->addDependent( this );

    if ( source.attributeScheduleStrategyPtr_ != 0 )
       attributeScheduleStrategyPtr_ = source.attributeScheduleStrategyPtr_->clone();
    else
       attributeScheduleStrategyPtr_ = 0;
    if ( attributeScheduleStrategyPtr_ != 0 ) 
       attributeScheduleStrategyPtr_->addDependent( this );

    assert( source.filterStrategyPtr_ != 0 );
    filterStrategyPtr_ = source.filterStrategyPtr_->clone();
    filterStrategyPtr_->addDependent( this );

}

// Copy constructor with new Scenario
LgFrSortingScheduleFloat::LgFrSortingScheduleFloat (
   const LgFrSortingScheduleFloat & source,
   LgFrScenario & newScenario)
:   LgFrScheduleFloat(source),
    permutations_(source.permutations_),
    compareStrategyPtr_(0),
    attributeScheduleStrategyPtr_(0),
    filterStrategyPtr_(0),
    ascending_(source.ascending_)
{
    if ( source.compareStrategyPtr_ != 0 )
       compareStrategyPtr_ = source.compareStrategyPtr_->clone(newScenario);
    else
       compareStrategyPtr_ = 0;
    if ( compareStrategyPtr_ != 0 ) 
       compareStrategyPtr_->addDependent( this );

    if ( source.attributeScheduleStrategyPtr_ != 0 )
       attributeScheduleStrategyPtr_ = 
           source.attributeScheduleStrategyPtr_->clone(newScenario);
    else
       attributeScheduleStrategyPtr_ = 0;
    if ( attributeScheduleStrategyPtr_ != 0 ) 
       attributeScheduleStrategyPtr_->addDependent( this );

    assert( source.filterStrategyPtr_ != 0 );
    filterStrategyPtr_ = source.filterStrategyPtr_->clone(newScenario);
    filterStrategyPtr_->addDependent( this );

}


// Constructor for a set expected to have up to n parts
LgFrSortingScheduleFloat::LgFrSortingScheduleFloat (const size_t n)
: LgFrScheduleFloat(n),
  compareStrategyPtr_(0),
  attributeScheduleStrategyPtr_(0),
  filterStrategyPtr_(0),
  ascending_(true)
{
   permutations_.reserve(n);
   filterStrategyPtr_ = new LgFrFilterStrategy;
   filterStrategyPtr_->addDependent( this );
}

// Destructor
LgFrSortingScheduleFloat::~LgFrSortingScheduleFloat ()
{
  if ( compareStrategyPtr_ != 0 ) 
     compareStrategyPtr_->removeDependent( this );
  delete compareStrategyPtr_;

  if ( attributeScheduleStrategyPtr_ != 0 ) 
     attributeScheduleStrategyPtr_->removeDependent( this );
  delete attributeScheduleStrategyPtr_;

  filterStrategyPtr_->removeDependent( this );
  delete filterStrategyPtr_;

}

// Prepare to sort
void
LgFrSortingScheduleFloat::prepareToSort()
{
  // Currently do nothing.

  // If and when LgFr is running in a multiprocess environment
  // then this routine will implement a lock on the global
  // variables used to implement sorting.
}

// Finished sorting
void
LgFrSortingScheduleFloat::doneSorting()
{
  // Currently do nothing.

  // If and when LgFr is running in a multiprocess environment
  // then this routine will implement an unlock on the global
  // variables used to implement sorting.
}


// Print member data in class
void
LgFrSortingScheduleFloat::print()
const
{
   std::cout <<"************************************************" <<std::endl;
   std::cout <<"LgSortingFrSchedule" << std::endl;
   int i = 0; // Pulled out of the for below by RW2STL
   for ( i=0; i<size(); i++ )  {
   std::cout <<" *********************************" <<std::endl;
      std::cout <<" entry number " <<i <<":";
      ((*this)[i]) . print();
   }
   std::cout <<"************************************************" <<std::endl;
}

// Set Schedule Strategy
void
LgFrSortingScheduleFloat::attributeScheduleStrategy(
const LgFrAttributeScheduleStrategy & attributeScheduleStrategy )
{
   if ( attributeScheduleStrategyPtr_ != 0 ) 
      attributeScheduleStrategyPtr_->removeDependent( this );
   delete attributeScheduleStrategyPtr_;
   attributeScheduleStrategyPtr_ = attributeScheduleStrategy.clone();
   if ( attributeScheduleStrategyPtr_ != 0 ) 
      attributeScheduleStrategyPtr_->addDependent( this );
}


// Get Schedule Strategy
// If this object ceases to exist or the attributeScheduleStrategy is changed,
// then the returned reference will be dangling
const LgFrAttributeScheduleStrategy *
LgFrSortingScheduleFloat::attributeScheduleStrategy()
const
{
   return attributeScheduleStrategyPtr_;
}

// localUpdate - This method is invoked when the data used by the populate
// method has changed.
void
LgFrSortingScheduleFloat::localUpdate (
   LgFrModel * ,
   void * )
{
    populate();
}

void
LgFrSortingScheduleFloat::propagateUpdate(
					  const LgFrItemTimeVecPairFloat & pair)
{
  if (attributeScheduleStrategyPtr_ != 0) {
     attributeScheduleStrategyPtr_ -> update (pair);
   }
}



// Perform diadic schedule operation.
LgFrSortingScheduleFloat
LgFrSortingScheduleFloat::op(
   LgFrTimeVecFloatOpFunction vectorOp, 
   const LgFrSortingScheduleFloat& rhs,
   float defaultValue)
const
{
   LgFrSortingScheduleFloat retVal;

   kernalOfOp( vectorOp, rhs, retVal, defaultValue );

   return retVal;
}


// perform scalar operation on column (period) in schedule
LgFrSortingScheduleFloat
LgFrSortingScheduleFloat::columnOp(
  const size_t period,
  float (*scalarOperation) (const float, const float),
  const float rhs )
const
{
   LgFrSortingScheduleFloat retVal;

   kernalOfColumnOp(  period, scalarOperation, rhs, retVal );

   return retVal;
}

// Addition operator
LgFrSortingScheduleFloat
operator+ (
   const LgFrSortingScheduleFloat& lhs,
   const LgFrSortingScheduleFloat& rhs)
{
  return lhs.op(&LgFrTimeVecFloat::add,rhs);
}

// Subtraction operator
LgFrSortingScheduleFloat
operator- (
   const LgFrSortingScheduleFloat& lhs,
   const LgFrSortingScheduleFloat& rhs)
{
  return lhs.op(&LgFrTimeVecFloat::subtract,rhs);
}

#ifdef NDEBUG
#undef NDEBUG
#endif

// test LgFrSortingScheduleFloat methods.
void
LgFrSortingScheduleFloat::test()
{
  // Make some parts for the test
  LgFrPart p1( "part1" );
  LgFrPart p2( "part2" );
  LgFrPart p3( "part3" );
  LgFrPart p4( "part4" );

  // Make some timeVecFloat for the test
  LgFrTimeVecFloat tvf1(10,1.11f);
  LgFrTimeVecFloat tvf2(10,2.22f);

  // Make some part timeVecFloat pairs for the test
  LgFrItemTimeVecPairFloat ptvfp1( p1, tvf1 );
  LgFrItemTimeVecPairFloat ptvfp2( p2, tvf1 );
  LgFrItemTimeVecPairFloat ptvfp3( p3, tvf2 );
  LgFrItemTimeVecPairFloat ptvfp4( p4, tvf1 );

  // Test the insertion methods
  LgFrSortingScheduleFloat ss (1);	// Make it a little small
  				// to test automatic resizing
  ss.insert(ptvfp4);
  ss.insert(ptvfp3);
  ss.insertAt(0, ptvfp2);
  assert (3 == ss.size());
  assert (ss[0].item().name() == "part2");
  assert (ss[2].item().name() == "part3");
  assert (ss[1].item().name() == "part4");

  // Test the assignment operator and the copy constructor
  LgFrSortingScheduleFloat ss1 (1);	// Make it a little small
  				// to test resizing at assignment time
  ss.insertKeyAndValue(p1,tvf1);		
  ss1 = ss;
  LgFrSortingScheduleFloat ss2 (ss);

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < ss.size(); i++) {
    // Test result of assignment
    assert (ss1[i] == ss[i]);
    // Test result of copy
    assert (ss2[i] == ss[i]);
    // ss[i].print();
    // std::cout << std::endl;
  }

  // Test indexing by item
  assert( ss[p3] == ptvfp3 );
  assert( ((const LgFrSortingScheduleFloat)ss)[p3] == ptvfp3 );

  // Test resize
  /*LgFrSortingScheduleFloat ss3(0);
  ss3.resize(3);
  assert (ss3.size() == 0);*/

  // Test Getting Comparison Strategy
  assert( ss.compareStrategy() == 0 );
  assert( ss.isSortAscending() );

  // Test sort
  assert( ss.isInNaturalOrder() );
  // std::cout <<"Unsorted" <<std::endl;
  // ss.print();
  LgFrPairCompareFloat standardCompareStrategy;
  ss.sort(&standardCompareStrategy);
  assert( !ss.isInNaturalOrder() );
  assert( ss.isSortAscending() );

  // Again test getting Comparison strategy
  const LgFrPairCompareFloat * returnedCompareStrategy = ss.compareStrategy();
  assert( returnedCompareStrategy != 0 );

  assert( ss[p3] == ptvfp3 );       // Test indexing by part on sorted sched.
  // std::cout <<"Sorted" <<std::endl;
  // ss.print();

  // Test descending sort
  LgFrSortingScheduleFloat ds(ss);	
  assert( !ds.isInNaturalOrder() );
  ds.sort(&standardCompareStrategy,false);
  assert( !ds.isInNaturalOrder() );
  assert( !ds.isSortAscending() );
  {
  size_t i,j;
  for ( i=0, j=ds.size()-1;
        j<=0;
        i++, j-- )
     assert( ss[i] == ds[j] );
  }
  
  // Put back in natural unsorted order 
  ds.sort();
  assert( ds.isInNaturalOrder() );
  assert( ds.isSortAscending() );
  {
  size_t i;
  for ( i=0;
        i<ds.size();
        i++ )
     assert( ss1[i] == ds[i] );
  }

  // Test descending natural order
  ds.sort(0,false);
  assert( !ds.isInNaturalOrder() );
  assert( !ds.isSortAscending() );
  {
  size_t i,j;
  for ( i=0, j= ds.size()-1;
        i<ds.size();
        i++, j-- )
     assert( ss1[j] == ds[i] );
  }

  // Test set/get of filter strategy
  {
  LgFrFilterStrategy defaultFilter;
  LgFrZeroTimeVecFilterStrategy zeroFilter;
  ss.filterStrategy( zeroFilter );
  ss.filterStrategy( defaultFilter );
  }

  // Test operation-
  {
  LgFrPart p1 ("IBMPS2mod95");
  LgFrPart p2 ("IBMPS2mod90serial45conf78");
  LgFrSortingScheduleFloat lhs,rhs,diff;
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
  {
  LgFrPart p1 ("IBMPS2mod95");
  LgFrPart p2 ("IBMPS2mod90serial45conf78");
  LgFrSortingScheduleFloat lhs,rhs,result;
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

  // Test columnOp
  {
  LgFrSortingScheduleFloat schedA;
  float vA1[]={ 1., 2., 3., 4. };
  float vA2[]={ 4., 3., 2., 1. };
  LgFrTimeVecFloat vecA1(4,vA1), vecA2(4,vA2);
  schedA.insertKeyAndValue( p1, vecA1 );
  schedA.insertKeyAndValue( p2, vecA2 );
  LgFrSortingScheduleFloat schedB = schedA.columnOp( 2, ::LgFrAssign, 3.14f );
  LgFrTimeVecFloat vecB1 = vecA1;
  vecB1[2] = 3.14f;
  LgFrTimeVecFloat vecB2 = vecA2;
  vecB2[2] = 3.14f;
  assert( schedB.size() == 2 );
  assert( schedB[0].item() == p1 );
  assert( schedB[1].item() == p2 );
  assert( schedB[0].timeVecFloat() == vecB1 );
  assert( schedB[1].timeVecFloat() == vecB2 );

  LgFrSortingScheduleFloat schedC = schedB.columnOp( 2, ::LgFrMultiply, 2.0 );
  LgFrTimeVecFloat vecC1 = vecB1;
  vecC1[2] = 2 * vecB1[2];
  LgFrTimeVecFloat vecC2 = vecB2;
  vecC2[2] = 2 * vecB2[2];
  assert( schedC.size() == 2 );
  assert( schedC[0].item() == p1 );
  assert( schedC[1].item() == p2 );
  assert( schedC[0].timeVecFloat() == vecC1 );
  assert( schedC[1].timeVecFloat() == vecC2 );
  }
}
