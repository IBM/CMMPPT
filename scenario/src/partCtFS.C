// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/partCtFS.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/part.h>
#include <scenario/src/machdep.h>
#include <scenario/src/idnumber.h>
#include <scenario/src/scenario.h>
#include <scenario/src/lastPrbS.h>

// Do Filtering
bool
LgFrPartCategoryFilterStrategy::exclude(
   const LgFrItemTimeVecPairFloat & itvpf)
const
{
   assert( this -> scenarioPtr() != 0 );

#ifdef WIN32
   const LgFrPart (& thePart) (itvpf.part());
#else
   const LgFrPart & thePart = itvpf.part();
#endif

   const LgFrLastProblemSolved & lps 
     = this -> lastProblemSolved(   * (  this->scenarioPtr()  )   );
   if (lps.category(&thePart) == partCategory_)
       return true;

   return false;
}


LgFrPartCategory
LgFrPartCategoryFilterStrategy::partCategory()
  const
{
   return partCategory_;
}

// Make clone copy of object
LgFrFilterStrategy *
LgFrPartCategoryFilterStrategy::clone()
const
{
  LgFrPartCategoryFilterStrategy* newInst = 
      new LgFrPartCategoryFilterStrategy(*this);
  return (newInst);
}

// Make clone copy of object with new Scenario
LgFrFilterStrategy *
LgFrPartCategoryFilterStrategy::clone(LgFrScenario & newScenario)
const
{
  LgFrPartCategoryFilterStrategy* newInst = 
      new LgFrPartCategoryFilterStrategy(*this, newScenario);
  return (newInst);
}

// Assignment operator
LgFrPartCategoryFilterStrategy &
LgFrPartCategoryFilterStrategy::operator=(
			  const LgFrPartCategoryFilterStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
     (this->LgFrScenarioSensitiveFilterStrategy::operator= ) (rhs);
     partCategory_ = rhs.partCategory_;
  }
  return *this;
}


// Copy constructor
LgFrPartCategoryFilterStrategy::
   LgFrPartCategoryFilterStrategy (
      const LgFrPartCategoryFilterStrategy & source)
:  
   LgFrScenarioSensitiveFilterStrategy( source ),
   partCategory_(source.partCategory_),
   id_(__LGFRPARTCATEGORYFILTERSTRATEGY)
{
}

// Copy constructor with new Scenario
LgFrPartCategoryFilterStrategy::
   LgFrPartCategoryFilterStrategy (
      const LgFrPartCategoryFilterStrategy & source,
      LgFrScenario & newScenario )
:  
   LgFrScenarioSensitiveFilterStrategy( source, newScenario ),
   partCategory_(source.partCategory_),
   id_(__LGFRPARTCATEGORYFILTERSTRATEGY)
{
}

// Preferred Constructor
LgFrPartCategoryFilterStrategy::
   LgFrPartCategoryFilterStrategy (
          const LgFrPartCategory & pc,
	  const LgFrScenario & scenario)
:  
   LgFrScenarioSensitiveFilterStrategy(scenario), 
   partCategory_(pc),
   id_(__LGFRPARTCATEGORYFILTERSTRATEGY)
{
    // Nothing else to do
}


// Default Constructor -- this will match anything
LgFrPartCategoryFilterStrategy::
   LgFrPartCategoryFilterStrategy ()
:  
   LgFrScenarioSensitiveFilterStrategy(),
   partCategory_(LgFrPRODUCT),
   id_(__LGFRPARTCATEGORYFILTERSTRATEGY)
{
    std::cerr << "Warning: trying to construct a LgFrPartCategoryFilterStrategy"
	<< "\nwithout providing a scenario." << std::endl;
}

// Destructor
LgFrPartCategoryFilterStrategy::
   ~LgFrPartCategoryFilterStrategy ()
{
   // Nothing to do here
}


#ifdef NDEBUG
#undef NDEBUG
#endif

// test LgFrPartCategoryFilterStrategy methods.
void
LgFrPartCategoryFilterStrategy::test()
{
}

void
LgFrPartCategoryFilterStrategy::contextTest(LgFrScenario & testScenario)
{
  // Note: the tests of the scenarioPtr are redundant.  ScenarioPtr should 
  // (and is) tested by the class that provides it.  These tests are here 
  // anyway because the test for potential compiler bugs and incorrect
  // implementations of LgFrScenarioSensitiveFilterStrategy.

  // Test Preferred constructor
  LgFrPartCategoryFilterStrategy cs1(LgFrRAW, testScenario);
  LgFrPartCategoryFilterStrategy capacityFilter (LgFrCAPACITY, testScenario);
  assert( cs1.scenarioPtr() == &testScenario );
  assert( cs1.partCategory_ != capacityFilter.partCategory_ );

  // Test copy constructor
  LgFrPartCategoryFilterStrategy cs2(cs1);
  assert( cs2.partCategory_ == cs1.partCategory_);
  assert( cs2.scenarioPtr() == cs1.scenarioPtr() );

  // Test assignment operator
  LgFrPartCategoryFilterStrategy cs3(LgFrCAPACITY, testScenario);
  cs3 = cs2;
  assert( cs3.partCategory_ == cs2.partCategory_ );
  assert( cs3.scenarioPtr() == cs2.scenarioPtr() );

  // Test clone method
  LgFrFilterStrategy *cs4Ptr = cs1.clone();
  
  // Cast the clone back down to an instance of this class
  LgFrPartCategoryFilterStrategy *cs5Ptr = 
         (LgFrPartCategoryFilterStrategy *) cs4Ptr; 
  assert( cs1.partCategory_ == cs5Ptr->partCategory_ );
  assert( cs5Ptr->scenarioPtr() == cs1.scenarioPtr() );

  // Test exclude
  LgFrPart aPart ( *(testScenario.constSetOfParts().findValue("Skillet")  )
		  );
  LgFrTimeVecFloat aTVF(2,2.);
  LgFrItemTimeVecPairFloat itvp1( aPart, aTVF);
  assert( ! cs1.exclude( itvp1 ) );
  assert( ! cs3.exclude( itvp1 ) );
  assert( capacityFilter.exclude( itvp1 )  );
	  
  // Test partCategory()
  assert (cs1.partCategory() == cs1.partCategory_);
  assert (cs2.partCategory() == cs2.partCategory_);
  assert (cs3.partCategory() == cs3.partCategory_);
  assert (capacityFilter.partCategory() == capacityFilter.partCategory_);
  assert (cs1.partCategory() == cs1.partCategory_);
  assert (cs5Ptr->partCategory() ==  cs5Ptr->partCategory_);

  // Test destructor
  delete cs4Ptr;

}
