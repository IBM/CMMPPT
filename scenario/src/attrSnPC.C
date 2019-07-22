#include <scenario/src/lastPrbS.h>
#include <scenario/src/wrkgProb.h>
#include <scenario/src/attrSnPC.h>

// Get LastProblemSolved
const LgFrLastProblemSolved &
LgFrAttributeSensitivePairCompareFloat::lastProblemSolved()
const
{
   assert (lpsPtr_ != NULL);
   return *lpsPtr_;
}

// Get WorkingImplosionProblem
const LgFrWorkingImplosionProblem &
LgFrAttributeSensitivePairCompareFloat::workingImplosionProblem()
const
{
   assert (wipPtr_ != NULL);
   return *wipPtr_;
}


// Assignment operator
LgFrAttributeSensitivePairCompareFloat &
LgFrAttributeSensitivePairCompareFloat::operator=(
			  const LgFrAttributeSensitivePairCompareFloat& rhs)
{
  if (this != &rhs) {		// Check for assignment to self

     (this->LgFrPairCompareFloat::operator= ) (rhs);

      lpsPtr_ = rhs.lpsPtr_;
      wipPtr_ = rhs.wipPtr_;
  }
  return *this;
}


// Copy constructor
LgFrAttributeSensitivePairCompareFloat::
   LgFrAttributeSensitivePairCompareFloat (
      const LgFrAttributeSensitivePairCompareFloat & source)
:  
   LgFrPairCompareFloat( source ),
   lpsPtr_( source.lpsPtr_ ),
   wipPtr_( source.wipPtr_ )
{
  // Nothing to do here
}

// Copy constructor with new Scenario
LgFrAttributeSensitivePairCompareFloat::
   LgFrAttributeSensitivePairCompareFloat (
      const LgFrAttributeSensitivePairCompareFloat & source,
      LgFrScenario & newScenario, 
      LgFrModel * algorithmPtr )
:  
   LgFrPairCompareFloat( source, newScenario, algorithmPtr ),
   lpsPtr_(&(LgFrPairCompareFloat::lastProblemSolved(newScenario))),
   wipPtr_(&(LgFrPairCompareFloat::workingImplosionProblem(newScenario)))
{
  // Nothing to do here
}

// Constructor
LgFrAttributeSensitivePairCompareFloat::
   LgFrAttributeSensitivePairCompareFloat (
     LgFrScenario & scenario,
     LgFrModel * algorithmPtr )
:   
   LgFrPairCompareFloat(algorithmPtr),
   lpsPtr_( &LgFrPairCompareFloat::lastProblemSolved(scenario) ),
   wipPtr_( &LgFrPairCompareFloat::workingImplosionProblem(scenario) )
{
   assert( lpsPtr_ != NULL );
   assert( wipPtr_ != NULL );
}


// Default Constructor
// This is a private member function.
LgFrAttributeSensitivePairCompareFloat::
   LgFrAttributeSensitivePairCompareFloat ()
:   
   LgFrPairCompareFloat(),
   lpsPtr_( NULL ),
   wipPtr_( NULL )
{
  // Nothing to do here
}

// Destructor
LgFrAttributeSensitivePairCompareFloat::
   ~LgFrAttributeSensitivePairCompareFloat ()
{
   // Nothing to do here
}
