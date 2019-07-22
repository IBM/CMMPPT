#include <iostream.h>

#include <scenario/src/demdBgPC.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/demdSchd.h>
#include <scenario/src/demVolSS.h>
#include <scenario/src/scenario.h>
#include <scenario/src/machdep.h>

// Do comparison
int
LgFrDemandBacklogTimesGrossRevenuePairCompare::itemTimeVecPairCompareFloat(
   const LgFrItemTimeVecPairFloat & itvpf1,
   const LgFrItemTimeVecPairFloat & itvpf2)
const
{
   assert( itvpf1.item().isA() == __LGFRDEMAND );
   assert( itvpf2.item().isA() == __LGFRDEMAND );
   
   // Get backlog volumes
   LgFrTimeVecFloat backlog1 ( 
            implodePtr_->lastProblemSolved().
                backlogVolume( &(itvpf1.demand()) )  );
   LgFrTimeVecFloat backlog2 ( 
            implodePtr_->lastProblemSolved().
                backlogVolume( &(itvpf2.demand()) )  );

   // Get Gross Revenues
   float grossRev1 ( 
            lastProblemSolved().grossRev( &(itvpf1.demand()) )  );
   float grossRev2 ( 
            lastProblemSolved().grossRev( &(itvpf2.demand()) )  );

   // Compare demand priorities
   int majorKey = ( grossRev1*backlog1 ).compareTo( grossRev2*backlog2 );

   if ( majorKey != 0 )
      return majorKey;
   else    // Demand priorities are equal, so compare itemTimeVecs
     return itvpf1.compareTo(&itvpf2);
}

// Make clone copy of object
LgFrPairCompareFloat *
LgFrDemandBacklogTimesGrossRevenuePairCompare::clone()
const
{
  LgFrDemandBacklogTimesGrossRevenuePairCompare* newInst = 
      new LgFrDemandBacklogTimesGrossRevenuePairCompare(*this);
  return (newInst);
}

// Make clone copy of object with new Scenario
LgFrPairCompareFloat *
LgFrDemandBacklogTimesGrossRevenuePairCompare::clone(
             LgFrScenario & newScenario)
const
{
  LgFrDemandBacklogTimesGrossRevenuePairCompare* newInst = 
      new LgFrDemandBacklogTimesGrossRevenuePairCompare(*this, newScenario);
  return (newInst);
}

// Return comparison strategy identifier
isAReturnType
LgFrDemandBacklogTimesGrossRevenuePairCompare::isA()
const
{
   return id_;
}

// Assignment operator
LgFrDemandBacklogTimesGrossRevenuePairCompare &
LgFrDemandBacklogTimesGrossRevenuePairCompare::operator=(
   const LgFrDemandBacklogTimesGrossRevenuePairCompare& rhs)
{
  assert( id_ == rhs.id_ );
  if (this != &rhs) {		// Check for assignment to self
     (this->LgFrAttributeSensitivePairCompareFloat::operator= ) (rhs);
     implodePtr_ = rhs.implodePtr_;
  }
  return *this;
}


// Copy constructor
LgFrDemandBacklogTimesGrossRevenuePairCompare::
   LgFrDemandBacklogTimesGrossRevenuePairCompare (
      const LgFrDemandBacklogTimesGrossRevenuePairCompare & source)
:  LgFrAttributeSensitivePairCompareFloat(source),
   implodePtr_( source.implodePtr_ ),
   id_(__LGFRDEMANDBACKLOGTIMESGROSSREVENUEPAIRCOMPARE) 
{
  assert( id_ == source.id_ );
}

// Copy constructor with newScenario
LgFrDemandBacklogTimesGrossRevenuePairCompare::
   LgFrDemandBacklogTimesGrossRevenuePairCompare (
      const LgFrDemandBacklogTimesGrossRevenuePairCompare & source,
      LgFrScenario & newScenario)
:  LgFrAttributeSensitivePairCompareFloat(
        source,
        newScenario, 
        &implode(newScenario) ),
   implodePtr_( &implode(newScenario) ),
   id_(__LGFRDEMANDBACKLOGTIMESGROSSREVENUEPAIRCOMPARE) 

{
  assert( id_ == source.id_ );
}

// Constructor
LgFrDemandBacklogTimesGrossRevenuePairCompare::
   LgFrDemandBacklogTimesGrossRevenuePairCompare (
      LgFrScenario & scenario )
: LgFrAttributeSensitivePairCompareFloat( 
        scenario, 
        &implode(scenario) ),
   implodePtr_( &implode(scenario) ),
   id_(__LGFRDEMANDBACKLOGTIMESGROSSREVENUEPAIRCOMPARE) 
{
   // Nothing to do here
}

// Default Constructor
// Private member function
LgFrDemandBacklogTimesGrossRevenuePairCompare::
   LgFrDemandBacklogTimesGrossRevenuePairCompare ()
:  // LgFrAttributeSensitivePairCompareFloat( LgFrScenario() ),
   implodePtr_( NULL ),
   id_(0)
{
  // Nothing to do here
}

// Destructor
LgFrDemandBacklogTimesGrossRevenuePairCompare::
   ~LgFrDemandBacklogTimesGrossRevenuePairCompare ()
{
}



#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrDemandBacklogTimesGrossRevenuePairCompare methods.
void
LgFrDemandBacklogTimesGrossRevenuePairCompare::test()
{
}


// test LgFrDemandBacklogTimesGrossRevenuePairCompare methods.
// Expects the data in testScenario to come from
// "/u/implode/wit/data/examples/diner12/wit.data"
void
LgFrDemandBacklogTimesGrossRevenuePairCompare::contextTest
  (
   LgFrScenario 	       & testScenario,
   LgFrWorkingImplosionProblem & wip,			// testScenario's 
   LgFrLastProblemSolved       & lps,			// testScenario's 
   LgFrSetOfDemands            & sod)			// testScenario's 
{
  int i;
  
  // Test constructor
  LgFrDemandBacklogTimesGrossRevenuePairCompare cs1( testScenario );
  assert( cs1.id_ == __LGFRDEMANDBACKLOGTIMESGROSSREVENUEPAIRCOMPARE );
  assert( cs1.lastProblemSolved().numberOfPeriods() == lps.numberOfPeriods() );

  // Test isA method
  assert( cs1.isA() == __LGFRDEMANDBACKLOGTIMESGROSSREVENUEPAIRCOMPARE );

  // Test copy constructor
  LgFrDemandBacklogTimesGrossRevenuePairCompare cs2(cs1);
  assert( cs2.isA() == __LGFRDEMANDBACKLOGTIMESGROSSREVENUEPAIRCOMPARE );
  assert( cs1.lastProblemSolved().numberOfPeriods() ==
          cs2.lastProblemSolved().numberOfPeriods()  );
  assert( cs1.workingImplosionProblem().implosionMethod() ==
          cs2.workingImplosionProblem().implosionMethod()  );

  // Test assignment operator
  LgFrDemandBacklogTimesGrossRevenuePairCompare cs3( testScenario );
  cs3 = cs2;
  assert( cs3.isA() == cs2.isA() );
  assert( cs3.workingImplosionProblem().equitability() ==
          cs2.workingImplosionProblem().equitability()  );

  {
    // Test clone method
    LgFrPairCompareFloat *cs4Ptr = cs1.clone();
    assert( cs4Ptr->isA() == cs1.isA() );
    assert( ( ( (LgFrDemandBacklogTimesGrossRevenuePairCompare *) cs4Ptr)
              ->
              workingImplosionProblem().timeVec( sod[0] ) )
            ==
            cs1.workingImplosionProblem().timeVec( sod[0] ) );

    // Test destructor
    delete cs4Ptr;
  }

  // Create a demand schedule to sort
  LgFrDemandVolumeScheduleStrategy demVolSS(testScenario);
  LgFrSortingDemandScheduleFloat schedule;
  schedule.attributeScheduleStrategy( demVolSS );
  schedule.populate();
  const LgFrSortingDemandScheduleFloat & constSchedule = schedule;

  // Do implosion to have backlog to sort on
  testScenario.newImplosionSolution();

  // Test itemTimeVecPairCompareFloat
  // method by doing sort and testing result

  // Display unsorted schedule
  // cerr <<"$$$$$$$$$$ Unsorted $$$$$$$$$$$$$$$" <<endl
  //      << schedule.format() <<endl;

  // Sort the part schedule
  schedule.sort( &cs1 );

  // Display sorted schedule
  // cerr <<"$$$$$$$$$$  Sorted  $$$$$$$$$$$$$$$" <<endl
  //      << schedule.format() <<endl;

  // Test to insure schedule is sorted
  for ( i=1; i<schedule.entries(); i++ ) {

     // Get demand volumes
     LgFrTimeVecFloat demVol1 ( 
            lps.demandVolume( &(constSchedule[i-1].demand()) )  );
     LgFrTimeVecFloat demVol2 ( 
            lps.demandVolume( &(constSchedule[ i ].demand()) )  );

     // Get shipment volumes
     LgFrTimeVecFloat shipVol1 ( 
              lps.shipmentVolume( &(constSchedule[i-1].demand()) )  );
     LgFrTimeVecFloat shipVol2 ( 
            lps.shipmentVolume( &(constSchedule[ i ].demand()) )  );

     // Compute backlog volumes
     LgFrTimeVecFloat backlog1 ( 
              demVol1.scan(LgFrAdd) - shipVol1.scan(LgFrAdd)  );
     LgFrTimeVecFloat backlog2 ( 
              demVol2.scan(LgFrAdd) - shipVol2.scan(LgFrAdd)  );

  }

  // Test that schedule gets resorted after an implode
  assert( schedule.entries()>5 );
  assert( !(constSchedule[2] == constSchedule[4]) );
  // switch 2nd and 4th entry in schedule
  LgFrItemTimeVecPairFloat s2 ( constSchedule[2] );
  LgFrItemTimeVecPairFloat s4 ( constSchedule[4] );
  schedule.localInsertAt (2, s4);
  schedule.localInsertAt (4, s2);

  // do same implosion again
  lps.implode(wip);
  // see if implosion caused schedule to be sorted correctly
  assert( constSchedule[2] == s2 );
  assert( constSchedule[4] == s4 );
}
