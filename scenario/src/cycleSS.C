// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/cycleSS.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/sortSchd.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/scenario.h>
#include <scenario/src/lastPrbS.h>
#include <scenario/src/machdep.h>
#include <scenario/src/partSchP.h>
#include <wit/src/wit.h>


// Populate Schedule with itemTimeVecs
void
LgFrCycleTimeScheduleStrategy::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  LgFrPart part;
  // Loop once for each part
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<setOfParts().size(); i++ ) {
     part = setOfParts()[i];

     // Is i'th part a product?
     if ( this->lastProblemSolved(*scenarioPtr_) . category( &part )  ==  
          LgFrPRODUCT ) {

	 LgFrItemTimeVecPairFloat itv (
	     part, 
	     workingImplosionProblem(*scenarioPtr_) . cycleTime( part ) 
	     );

	 // Check to see if i'th itemTimeVec is to be filtered out
	 if ( schedule.filterStrategy().exclude( itv ) ) continue;

	 schedule.localInsert( itv );	// Insert itemTimeVec into schedule
     }
  }

}


// Update (propogate) a supply change to wip 
void 
LgFrCycleTimeScheduleStrategy::update(
          const LgFrItemTimeVecPairFloat & newITV)
{
  ( this -> workingImplosionProblem(*scenarioPtr_) )
  . cycleTime(newITV.part(), newITV.timeVecFloat());
}


// CycleTime Volumes are not mutable
bool
LgFrCycleTimeScheduleStrategy::isMutable()
const
{
 return true;
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrCycleTimeScheduleStrategy::clone()
const
{
  LgFrCycleTimeScheduleStrategy* newInst = 
      new LgFrCycleTimeScheduleStrategy(*this);
  return (newInst);
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrCycleTimeScheduleStrategy::clone(LgFrScenario& newScenario)
const
{  LgFrCycleTimeScheduleStrategy* newInst = 
      new LgFrCycleTimeScheduleStrategy(*this, newScenario);
  return (newInst);
}


// Assignment operator
LgFrCycleTimeScheduleStrategy &
LgFrCycleTimeScheduleStrategy::operator=(
	  const LgFrCycleTimeScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrPartScheduleStrategy::operator= ) (rhs);
     scenarioPtr_ = rhs.scenarioPtr_;
  }
  return *this;
}


// Copy constructor
LgFrCycleTimeScheduleStrategy::
   LgFrCycleTimeScheduleStrategy (
      const LgFrCycleTimeScheduleStrategy & source)
:  LgFrPartScheduleStrategy( source ), 
   scenarioPtr_( source.scenarioPtr_ ),
   id_(__LGFRCYCLETIMESCHEDULESTRATEGY)
{
}

// Copy constructor with newScenario
LgFrCycleTimeScheduleStrategy::
   LgFrCycleTimeScheduleStrategy (
      const LgFrCycleTimeScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrPartScheduleStrategy( 
         source, 
         newScenario ),
   id_(__LGFRCYCLETIMESCHEDULESTRATEGY)
{
}

// Constructor
LgFrCycleTimeScheduleStrategy::
   LgFrCycleTimeScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleStrategy( 
      LgFrAttributeScheduleStrategy::setOfParts(scenario), 0),
   scenarioPtr_( &(scenario) ),
   id_(__LGFRCYCLETIMESCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrCycleTimeScheduleStrategy::
   LgFrCycleTimeScheduleStrategy ()
:  LgFrPartScheduleStrategy( LgFrSetOfPartsWithMovableParts() ),
   scenarioPtr_(0),
   id_(__LGFRCYCLETIMESCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrCycleTimeScheduleStrategy::
   ~LgFrCycleTimeScheduleStrategy ()
{
   // Nothing to do here
}

// ---------------------------
// LgFrFrozenZoneScheduleStrategy methods
// ------------------------------------



// Populate Schedule with itemTimeVecs
void
LgFrFrozenZoneScheduleStrategy::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  LgFrPart part;
  LgFrItemTimeVecPairFloat itv;
  float fltEps = 0.0001f;

  // Loop once for each part
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<setOfParts().size(); i++ ) {
     part = setOfParts()[i];

     // Is i'th part a product?
     if ( this->lastProblemSolved(*scenarioPtr_) . category( &part )  ==  
          LgFrPRODUCT ) {

        // Make itemTimeVec
        itv.item( part );
        LgFrTimeVecFloat fz(scenarioPtr_->numberOfPeriods(), 0.0);
        const LgFrTimeVecFloat & ct
	    = this->workingImplosionProblem(*scenarioPtr_) . cycleTime(part);
        int t = 0; // Pulled out of the for below by RW2STL
        for (t=0; t<fz.length(); t++)  {
           if ((ct[t] > t + 1 - fltEps) || (ct[t] < -fltEps))
              fz[t] = 1.0;
           else
              break;
         }
        itv.timeVecFloat(fz);

        // Check to see if i'th itemTimeVec is to be filtered out
        if ( schedule.filterStrategy().exclude( itv ) ) continue;

        // Insert itemTimeVec into schedule
        schedule.localInsert( itv );

     }
  }

}

// CycleTime Volumes are not mutable
bool
LgFrFrozenZoneScheduleStrategy::isMutable()
const
{
 return false;
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrFrozenZoneScheduleStrategy::clone()
const
{
  LgFrFrozenZoneScheduleStrategy* newInst = 
      new LgFrFrozenZoneScheduleStrategy(*this);
  return (newInst);
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrFrozenZoneScheduleStrategy::clone(LgFrScenario& newScenario)
const
{  LgFrFrozenZoneScheduleStrategy* newInst = 
      new LgFrFrozenZoneScheduleStrategy(*this, newScenario);
  return (newInst);
}


// Assignment operator
LgFrFrozenZoneScheduleStrategy &
LgFrFrozenZoneScheduleStrategy::operator=(
	  const LgFrFrozenZoneScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrPartScheduleStrategy::operator= ) (rhs);
     scenarioPtr_ = rhs.scenarioPtr_;
  }
  return *this;
}


// Copy constructor
LgFrFrozenZoneScheduleStrategy::
   LgFrFrozenZoneScheduleStrategy (
      const LgFrFrozenZoneScheduleStrategy & source)
:  LgFrPartScheduleStrategy( source ), 
   scenarioPtr_( source.scenarioPtr_ ),
   id_(__LGFRFROZENZONESCHEDULESTRATEGY)
{
}

// Copy constructor with newScenario
LgFrFrozenZoneScheduleStrategy::
   LgFrFrozenZoneScheduleStrategy (
      const LgFrFrozenZoneScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrPartScheduleStrategy( 
         source, 
         newScenario ),
   id_(__LGFRFROZENZONESCHEDULESTRATEGY)
{
}

// Constructor
LgFrFrozenZoneScheduleStrategy::
   LgFrFrozenZoneScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleStrategy( 
      LgFrAttributeScheduleStrategy::setOfParts(scenario), 0),
   scenarioPtr_( &scenario ),
   id_(__LGFRFROZENZONESCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrFrozenZoneScheduleStrategy::
   LgFrFrozenZoneScheduleStrategy ()
:  LgFrPartScheduleStrategy( LgFrSetOfPartsWithMovableParts() ),
   scenarioPtr_(0),
   id_(__LGFRFROZENZONESCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrFrozenZoneScheduleStrategy::
   ~LgFrFrozenZoneScheduleStrategy ()
{
   // Nothing to do here
}




// ---------------------------------------------------------
//            test methods
// ---------------------------------------------------------


#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrCycleTimeScheduleStrategy methods.
void
LgFrCycleTimeScheduleStrategy::test()
{
}


// test LgFrCycleTimeScheduleStrategy methods.
// Expects data for both testScenario and testScenario2 to come from
// /u/implode/wit/data/examples/diner12/wit.data, but expects them to
// be two different scenarios.
void
LgFrCycleTimeScheduleStrategy::contextTest
    (LgFrScenario                & testScenario,
     LgFrWorkingImplosionProblem & wip,            // from testScenario
     LgFrLastProblemSolved       & lps,            // from testScenario
     LgFrSetOfParts              & sop)            // from testScenario
{
  int i;
  float fltEps = .0001f;

  // Test constructor
  LgFrCycleTimeScheduleStrategy cycleTimeSS1(testScenario);

  // Test copy constructor
  LgFrCycleTimeScheduleStrategy cycleTimeSS1copy(cycleTimeSS1);

  // Test assignment operator
  {
    LgFrCycleTimeScheduleStrategy cycleTimeSS3;
    cycleTimeSS3 = cycleTimeSS1copy;
  }

  // Test clone method
  {
    LgFrAttributeScheduleStrategy *cycleTimeSS1ClonePtr = cycleTimeSS1.clone();

    // Test destructor
    delete cycleTimeSS1ClonePtr;
  }

  LgFrPart EggOpart = *(sop.findValue("Plain_Omlet"));
  

  // Test populate
  LgFrSortingPartScheduleFloatSmartPointer schedule;
  const  LgFrSortingPartScheduleFloat & constSchedule = *schedule;
  cycleTimeSS1.populate( *schedule );
  // cout <<endl <<"LgFrCycleTimeScheduleStrategy" <<endl
  //      <<schedule->format() <<endl;

  assert( wip.cycleTime(EggOpart) == constSchedule[EggOpart].timeVecFloat() );

  // Test update
  LgFrTimeVecFloat newCT(2, 2.0);
  cycleTimeSS1.update (  LgFrItemTimeVecPairFloat (EggOpart, newCT)  );// update it
  testScenario.newImplosionSolution();		// make it get propagated
  assert( lps.cycleTime(&EggOpart) == newCT );	// check it

  // Test populate with filter
  LgFrZeroTimeVecFilterStrategy zeroFilter;
  schedule->filterStrategy( zeroFilter );
  
  cycleTimeSS1.populate( *schedule );

  for ( i=0; i<schedule->size(); i++ ) {
     assert(  !(constSchedule[i].timeVecFloat() == 0.0)  );
  }

}





// test LgFrFrozenZoneScheduleStrategy methods.
void
LgFrFrozenZoneScheduleStrategy::test()
{
}


// test LgFrFrozenZoneScheduleStrategy methods.
// Expects data for both testScenario and testScenario2 to come from
// /u/implode/wit/data/examples/diner12/wit.data, but expects them to
// be two different scenarios.
void
LgFrFrozenZoneScheduleStrategy::contextTest
    (LgFrScenario                & testScenario,
     LgFrWorkingImplosionProblem & wip,	            // from testScenario
     LgFrSetOfParts              & sop)            // from testScenario
{
  int i;
  float fltEps = .0001f;

  // Test constructor
  LgFrFrozenZoneScheduleStrategy cycleTimeSS1(testScenario);

  // Test copy constructor
  LgFrFrozenZoneScheduleStrategy cycleTimeSS1copy(cycleTimeSS1);

  // Test assignment operator
  {
    LgFrFrozenZoneScheduleStrategy cycleTimeSS3;
    cycleTimeSS3 = cycleTimeSS1copy;
  }

  // Test clone method
  {
    LgFrAttributeScheduleStrategy *cycleTimeSS1ClonePtr = cycleTimeSS1.clone();

    // Test destructor
    delete cycleTimeSS1ClonePtr;
  }

  LgFrPart EggOpart = *(sop.findValue("Plain_Omlet"));
  

  // Test populate
  LgFrSortingPartScheduleFloatSmartPointer schedule;
  const  LgFrSortingPartScheduleFloat & constSchedule = *schedule;
  cycleTimeSS1.populate( *schedule );
  // cout <<endl <<"LgFrFrozenZoneScheduleStrategy" <<endl
  //      <<schedule.format() <<endl;

  assert( wip.cycleTime(EggOpart) == constSchedule[EggOpart].timeVecFloat() );

  // Test populate with filter
  LgFrZeroTimeVecFilterStrategy zeroFilter;
  schedule->filterStrategy( zeroFilter);
  
  cycleTimeSS1.populate( *schedule );

  for ( i=0; i<schedule->size(); i++ ) {
     assert(  !(constSchedule[i].timeVecFloat() == 0.0)  );
  }


}

