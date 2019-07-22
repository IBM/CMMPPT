#include <scenario/src/sOfPaFS.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/scenario.h>
// #include <scenario/src/machdep.h>

// Do Filtering
RWBoolean
LgFrSetOfPartsFilterStrategy::exclude(
   const LgFrItemTimeVecPairFloat & itvpf)
const
{
   assert( this->scenarioPtr() != NULL );
   return ! (
	     setOfParts(this->scenarioPtr(),setOfPartsId_).contains( itvpf.part() ) 
	     );
}


// Identify the scenario containing the setOfParts to be used when filtering,
// and the setOfParts identifier (ie LGFR_UNIVERSAL_SET, LGFR_DEFAULT_SET, or
// values specific to scenario derived classes).
void
LgFrSetOfPartsFilterStrategy::setOfPartsIdentifier(
   const LgFrScenario & scenario,
   int setOfPartsId )
{
   this->scenario (scenario);
   setOfPartsId_  = setOfPartsId;
}


// Get setOfParts identifier
int 
LgFrSetOfPartsFilterStrategy::setOfPartsIdentifier()
const
{
   return setOfPartsId_;
}


// Make clone copy of object
LgFrFilterStrategy *
LgFrSetOfPartsFilterStrategy::clone()
const
{
  LgFrSetOfPartsFilterStrategy* newInst = 
      new LgFrSetOfPartsFilterStrategy(*this);
  return (newInst);
}

// Make clone copy of object with new Scenario
LgFrFilterStrategy *
LgFrSetOfPartsFilterStrategy::clone(LgFrScenario & newScenario)
const
{
  LgFrSetOfPartsFilterStrategy* newInst = 
      new LgFrSetOfPartsFilterStrategy(*this, newScenario);
  return (newInst);
}

// Return comparison strategy identifier
isAReturnType
LgFrSetOfPartsFilterStrategy::isA()
const
{
   return id_;
}

// Assignment operator
LgFrSetOfPartsFilterStrategy &
LgFrSetOfPartsFilterStrategy::operator=(
			  const LgFrSetOfPartsFilterStrategy& rhs)
{
  assert( id_ == rhs.id_ );
  if (this != &rhs) {		// Check for assignment to self
     (this->LgFrScenarioSensitiveFilterStrategy::operator= ) (rhs);
     setOfPartsId_ = rhs.setOfPartsId_;
  }
  return *this;
}


// Copy constructor
LgFrSetOfPartsFilterStrategy::
   LgFrSetOfPartsFilterStrategy (
      const LgFrSetOfPartsFilterStrategy & source)
:  
   LgFrScenarioSensitiveFilterStrategy( source ),
   setOfPartsId_(source.setOfPartsId_),
   id_(__LGFRSETOFPARTSFILTERSTRATEGY)
{
  assert( id_ == source.id_ );
}

// Copy constructor with new Scenario
LgFrSetOfPartsFilterStrategy::
   LgFrSetOfPartsFilterStrategy (
      const LgFrSetOfPartsFilterStrategy & source,
      LgFrScenario & newScenario)
:  
   LgFrScenarioSensitiveFilterStrategy( source, newScenario ),
   setOfPartsId_(source.setOfPartsId_),
   id_(__LGFRSETOFPARTSFILTERSTRATEGY)
{
  assert( id_ == source.id_ );
}


// Default Constructor
LgFrSetOfPartsFilterStrategy::
   LgFrSetOfPartsFilterStrategy ()
:  
   LgFrScenarioSensitiveFilterStrategy(),
   setOfPartsId_(0),
   id_(__LGFRSETOFPARTSFILTERSTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrSetOfPartsFilterStrategy::
   ~LgFrSetOfPartsFilterStrategy ()
{
   // Nothing to do here
}


#ifdef NDEBUG
#undef NDEBUG
#endif

// test LgFrSetOfPartsFilterStrategy methods.
void
LgFrSetOfPartsFilterStrategy::test()
{

  // Test default constructor
  LgFrSetOfPartsFilterStrategy cs1;
  assert( cs1.id_ == __LGFRSETOFPARTSFILTERSTRATEGY );

  // Test isA method
  assert( cs1.isA() == __LGFRSETOFPARTSFILTERSTRATEGY );

  // Test copy constructor
  LgFrSetOfPartsFilterStrategy cs2(cs1);
  assert( cs2.isA() == __LGFRSETOFPARTSFILTERSTRATEGY );

  // Test assignment operator
  LgFrSetOfPartsFilterStrategy cs3;
  cs3 = cs2;
  assert( cs3.isA() == cs2.isA() );

  // Test clone method
  LgFrFilterStrategy *cs4Ptr = cs1.clone();
  assert( cs4Ptr->isA() == cs1.isA() );

  // Test destructor
  delete cs4Ptr;

}

// test LgFrSetOfPartsFilterStrategy methods.
void
LgFrSetOfPartsFilterStrategy::contextTest(
   LgFrScenario & scenario,
   LgFrSetOfParts & univSop )
{
   LgFrSetOfPartsFilterStrategy cs1;

   // Test setting/getting FS's setOfParts
   cs1.setOfPartsIdentifier( scenario, LGFR_DEFAULT_SET );
   assert( cs1.setOfPartsIdentifier() == LGFR_DEFAULT_SET );
   cs1.setOfPartsIdentifier( scenario, LGFR_UNIVERSAL_SET );
   assert( cs1.setOfPartsIdentifier() == LGFR_UNIVERSAL_SET );

  // Test exclude
  LgFrItemTimeVecPairFloat itvp1( 
          univSop[0], 
          LgFrTimeVecFloat(2,2.) );
  assert( !cs1.exclude( itvp1 ) );

  // Create a part which is not in the universal set of parts
  LgFrPart part( itvp1.part().name() +
                 itvp1.part().name() +
                 itvp1.part().name()  );
  itvp1.item(part);
  assert( cs1.exclude( itvp1 ) );

}
