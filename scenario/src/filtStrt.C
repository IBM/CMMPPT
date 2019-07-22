#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/filtStrt.h>
#include <scenario/src/scenario.h>
#include <scenario/src/algorthm.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/machdep.h>

// Do Filtering
bool
LgFrFilterStrategy::exclude(
   const LgFrItemTimeVecPairFloat & )
const
{
   return false;
}

// Return the LastProblemSolved which belongs to a scenario
const LgFrLastProblemSolved&
LgFrFilterStrategy::lastProblemSolved(
   const LgFrScenario & scenario)
const
{
   return scenario.lastProblemSolved();
}

// updateFrom - This method is invoked when the data used by the exclude 
// filter method has changed.
/*
void
LgFrFilterStrategy::localUpdate (
   LgFrModel *,
   void * )
{
   // Inform all objects dependent on this strategy that there has been
   // a change in the data used in the exclude method
  cout << "object changing has address " << (unsigned long)this << endl;
}
*/

// Get a set of parts from scenario.
// Static protected member used by derived classes.  This function is a
// friend of scenario
const LgFrSetOfParts &
LgFrFilterStrategy::setOfParts(
   const LgFrScenario * scenPtr,
   int setOfPartsIdentifier )
{
   assert( scenPtr != 0 );
   return scenPtr->constSetOfParts( setOfPartsIdentifier );
}

// Make clone copy of object
LgFrFilterStrategy *
LgFrFilterStrategy::clone()
const
{
  LgFrFilterStrategy* newInst = 
      new LgFrFilterStrategy(*this);
  return (newInst);
}

// Make clone copy of object with new Scenario
LgFrFilterStrategy *
LgFrFilterStrategy::clone(LgFrScenario & newScenario)
const
{
  LgFrFilterStrategy* newInst = 
      new LgFrFilterStrategy(*this, newScenario);
  return (newInst);
}

// Assignment operator
LgFrFilterStrategy &
LgFrFilterStrategy::operator=(
			  const LgFrFilterStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
     (this->LgFrSubscription::operator= ) (rhs);
     if ( algorithmPtr_ != 0 ) algorithmPtr_->removeDependentFromHeterogeneousList( this );
     algorithmPtr_ = rhs.algorithmPtr_;
     if ( algorithmPtr_ != 0 ) algorithmPtr_->addDependent( this );
  }
  return *this;
}


// Copy constructor
LgFrFilterStrategy::
   LgFrFilterStrategy (
      const LgFrFilterStrategy & source)
: 
   LgFrSubscription(),       
   algorithmPtr_( source.algorithmPtr_ ),
   id_(__LGFRFILTERSTRATEGY)
{
   if ( algorithmPtr_ != 0 ) algorithmPtr_->addDependent( this );
}


// Copy constructor with newScenario
LgFrFilterStrategy::
   LgFrFilterStrategy (
      const LgFrFilterStrategy & source,
      LgFrScenario & newScenario )
: 
   LgFrSubscription(),         
   algorithmPtr_( 0 ),
   id_(__LGFRFILTERSTRATEGY)
{
   if ( source.algorithmPtr_ != 0 ) {
      algorithmPtr_ = source.algorithmPtr_ -> counterPart( newScenario );
      algorithmPtr_->addDependent( this );
   }
}


// Default Constructor
LgFrFilterStrategy::
   LgFrFilterStrategy (
      LgFrAlgorithm * algorithmPtr )
: 
   LgFrSubscription(),
   algorithmPtr_(algorithmPtr),
   id_(__LGFRFILTERSTRATEGY)
{
  if ( algorithmPtr_ != 0 ) algorithmPtr_->addDependent( this );
}

// Destructor
LgFrFilterStrategy::
   ~LgFrFilterStrategy ()
{
   if ( algorithmPtr_ != 0 ) algorithmPtr_->removeDependentFromHeterogeneousList( this );
}


#ifdef NDEBUG
#undef NDEBUG
#endif

// test LgFrFilterStrategy methods.
void
LgFrFilterStrategy::test()
{
  // Test default constructor
  LgFrFilterStrategy cs1;

  // Test copy constructor
  LgFrFilterStrategy cs2(cs1);

  // Test assignment operator
  LgFrFilterStrategy cs3;
  cs3 = cs2;

  // Test clone method
  LgFrFilterStrategy *cs4Ptr = cs1.clone();

  // Test destructor
  delete cs4Ptr;

  // Test exclude
  LgFrItemTimeVecPairFloat itvp1( LgFrPart("abc"), LgFrTimeVecFloat(2,2.) );
  assert( !cs1.exclude( itvp1 ) );

}
