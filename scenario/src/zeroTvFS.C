// RW2STL -- inserted:
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/zeroTvFS.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/machdep.h>

// Do Filtering
bool
LgFrZeroTimeVecFilterStrategy::exclude(
   const LgFrItemTimeVecPairFloat & itvpf)
const
{
    return itvpf.timeVecFloat() == 0.0 ? true : false;
}

// Make clone copy of object
LgFrFilterStrategy *
LgFrZeroTimeVecFilterStrategy::clone()
const
{
  LgFrZeroTimeVecFilterStrategy* newInst = 
      new LgFrZeroTimeVecFilterStrategy(*this);
  return (newInst);
}

// Make clone copy of object with new Scenario
LgFrFilterStrategy *
LgFrZeroTimeVecFilterStrategy::clone(LgFrScenario & newScenario)
const
{
  LgFrZeroTimeVecFilterStrategy* newInst = 
      new LgFrZeroTimeVecFilterStrategy(*this, newScenario);
  return (newInst);
}

// Assignment operator
LgFrZeroTimeVecFilterStrategy &
LgFrZeroTimeVecFilterStrategy::operator=(
			  const LgFrZeroTimeVecFilterStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
     (this->LgFrFilterStrategy::operator= ) (rhs);
  }
  return *this;
}


// Copy constructor
LgFrZeroTimeVecFilterStrategy::
   LgFrZeroTimeVecFilterStrategy (
      const LgFrZeroTimeVecFilterStrategy & source)
:  
   LgFrFilterStrategy( source ),
   id_(__LGFRZEROTIMEVECFILTERSTRATEGY)
{
}

// Copy constructor with new Scenario
LgFrZeroTimeVecFilterStrategy::
   LgFrZeroTimeVecFilterStrategy (
      const LgFrZeroTimeVecFilterStrategy & source,
      LgFrScenario & newScenario)
:  
   LgFrFilterStrategy( source, newScenario ),
   id_(__LGFRZEROTIMEVECFILTERSTRATEGY)
{
}


// Default Constructor
LgFrZeroTimeVecFilterStrategy::
   LgFrZeroTimeVecFilterStrategy ()
:  
   LgFrFilterStrategy(),
   id_(__LGFRZEROTIMEVECFILTERSTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrZeroTimeVecFilterStrategy::
   ~LgFrZeroTimeVecFilterStrategy ()
{
   // Nothing to do here
}


// test LgFrZeroTimeVecFilterStrategy methods.
void
LgFrZeroTimeVecFilterStrategy::test()
{

  // Test default constructor
  LgFrZeroTimeVecFilterStrategy cs1;

  LgFrZeroTimeVecFilterStrategy cs2(cs1);

  // Test assignment operator
  LgFrZeroTimeVecFilterStrategy cs3;
  cs3 = cs2;

  // Test clone method
  LgFrFilterStrategy *cs4Ptr = cs1.clone();

  // Test destructor
  delete cs4Ptr;

  // Test exclude
  LgFrItemTimeVecPairFloat itvp1( LgFrPart("abc"), LgFrTimeVecFloat(2,2.) );
  assert( !cs1.exclude( itvp1 ) );

  itvp1.timeVecFloat( LgFrTimeVecFloat(2, 0.0) );
  assert( cs1.exclude( itvp1 ) );

}
