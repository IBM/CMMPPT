#include <iostream.h>

#include <scenario/src/demdVgPC.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/demdSchd.h>
#include <scenario/src/demdSchP.h>
#include <scenario/src/scenario.h>
#include <scenario/src/machdep.h>

// Do comparison
int
LgFrDemandVolumeTimesGrossRevenuePairCompare::itemTimeVecPairCompareFloat(
   const LgFrItemTimeVecPairFloat & itvpf1,
   const LgFrItemTimeVecPairFloat & itvpf2)
const
{
   assert( itvpf1.item().isA() == __LGFRDEMAND );
   
   // Get demand volumes
   LgFrTimeVecFloat demVol1 ( 
            workingImplosionProblem().timeVec( itvpf1.demand() )  );
            // lastProblemSolved().demandVolume( &(itvpf1.demand()) )  );
   LgFrTimeVecFloat demVol2 ( 
            workingImplosionProblem().timeVec( itvpf2.demand() )  );
            // lastProblemSolved().demandVolume( &(itvpf2.demand()) )  );

   // Get Gross Revenues
   float grossRev1 ( 
            lastProblemSolved().grossRev( &(itvpf1.demand()) )  );
   float grossRev2 ( 
            lastProblemSolved().grossRev( &(itvpf2.demand()) )  );

   // Compare demand priorities
   int majorKey = ( grossRev1*demVol1 ).compareTo( grossRev2*demVol2 );

   if ( majorKey != 0 )
      return majorKey;
   else   // Demand priorities are equal, so compare itemTimeVecs
     return itvpf1.compareTo(&itvpf2);
}

// Make clone copy of object
LgFrPairCompareFloat *
LgFrDemandVolumeTimesGrossRevenuePairCompare::clone()
const
{
  LgFrDemandVolumeTimesGrossRevenuePairCompare* newInst = 
      new LgFrDemandVolumeTimesGrossRevenuePairCompare(*this);
  return (newInst);
}

// Make clone copy of object with new Scenario
LgFrPairCompareFloat *
LgFrDemandVolumeTimesGrossRevenuePairCompare::clone(
          LgFrScenario & newScenario )
const
{
  LgFrDemandVolumeTimesGrossRevenuePairCompare* newInst = 
      new LgFrDemandVolumeTimesGrossRevenuePairCompare(*this, newScenario );
  return (newInst);
}

// Return comparison strategy identifier
isAReturnType
LgFrDemandVolumeTimesGrossRevenuePairCompare::isA()
const
{
   return id_;
}

// Assignment operator
LgFrDemandVolumeTimesGrossRevenuePairCompare &
LgFrDemandVolumeTimesGrossRevenuePairCompare::operator=(
   const LgFrDemandVolumeTimesGrossRevenuePairCompare& rhs)
{
  assert( id_ == rhs.id_ );
  if (this != &rhs) {		// Check for assignment to self
     // Sun CC does not like following line, so do it differently
     (this->LgFrAttributeSensitivePairCompareFloat::operator= ) (rhs);
  }
  return *this;
}


// Copy constructor
LgFrDemandVolumeTimesGrossRevenuePairCompare::
   LgFrDemandVolumeTimesGrossRevenuePairCompare (
      const LgFrDemandVolumeTimesGrossRevenuePairCompare & source)
:  LgFrAttributeSensitivePairCompareFloat(source),
   id_(__LGFRDEMANDVOLUMETIMESGROSSREVENUEPAIRCOMPARE) 
{
  assert( id_ == source.id_ );
}

// Copy constructor with new Scenario
LgFrDemandVolumeTimesGrossRevenuePairCompare::
   LgFrDemandVolumeTimesGrossRevenuePairCompare (
      const LgFrDemandVolumeTimesGrossRevenuePairCompare & source,
      LgFrScenario & newScenario )
:  LgFrAttributeSensitivePairCompareFloat(source, newScenario),
   id_(__LGFRDEMANDVOLUMETIMESGROSSREVENUEPAIRCOMPARE) 
{
  assert( id_ == source.id_ );
}


// Constructor
LgFrDemandVolumeTimesGrossRevenuePairCompare::
   LgFrDemandVolumeTimesGrossRevenuePairCompare (
      LgFrScenario & scenario )
: LgFrAttributeSensitivePairCompareFloat( scenario ),
   id_(__LGFRDEMANDVOLUMETIMESGROSSREVENUEPAIRCOMPARE) 
{
  // Nothing to do here
}

// Default Constructor
LgFrDemandVolumeTimesGrossRevenuePairCompare::
   LgFrDemandVolumeTimesGrossRevenuePairCompare ()
:  LgFrAttributeSensitivePairCompareFloat(),
   id_(0)
{
  // Nothing to do here
}

// Destructor
LgFrDemandVolumeTimesGrossRevenuePairCompare::
   ~LgFrDemandVolumeTimesGrossRevenuePairCompare ()
{
   // Nothing to do here
}



#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrDemandVolumeTimesGrossRevenuePairCompare methods.
void
LgFrDemandVolumeTimesGrossRevenuePairCompare::test()
{
}


// test LgFrDemandBacklogTimesGrossRevenuePairCompare methods.
// Expects the data in testScenario to come from
// "/u/implode/wit/data/examples/diner12/wit.data"
void
LgFrDemandVolumeTimesGrossRevenuePairCompare::contextTest
  (LgFrScenario 	          & testScenario,
   LgFrLastProblemSolved          & lps,		// testScenario's 
   LgFrSetOfDemands               & sod,		// testScenario's
   LgFrSortingDemandScheduleFloatSmartPointer demandSchedule)	// testScenario's
{
  int i;

  // Test constructor
  LgFrDemandVolumeTimesGrossRevenuePairCompare cs1( testScenario );
  assert( cs1.id_ == __LGFRDEMANDVOLUMETIMESGROSSREVENUEPAIRCOMPARE );
  assert( cs1.lastProblemSolved().numberOfPeriods() == lps.numberOfPeriods() );

  // Test isA method
  assert( cs1.isA() == __LGFRDEMANDVOLUMETIMESGROSSREVENUEPAIRCOMPARE );

  // Test copy constructor
  LgFrDemandVolumeTimesGrossRevenuePairCompare cs2(cs1);
  assert( cs2.isA() == __LGFRDEMANDVOLUMETIMESGROSSREVENUEPAIRCOMPARE );
  assert( cs1.lastProblemSolved().numberOfPeriods() ==
          cs2.lastProblemSolved().numberOfPeriods()  );
  assert( cs1.workingImplosionProblem().implosionMethod() ==
          cs2.workingImplosionProblem().implosionMethod()  );

  // Test assignment operator
  LgFrDemandVolumeTimesGrossRevenuePairCompare cs3( testScenario );
  cs3 = cs2;
  assert( cs3.isA() == cs2.isA() );
  assert( cs3.workingImplosionProblem().equitability() ==
          cs2.workingImplosionProblem().equitability()  );

  {
    // Test clone method
    LgFrPairCompareFloat *cs4Ptr = cs1.clone();
    assert( cs4Ptr->isA() == cs1.isA() );

    // Test destructor
    delete cs4Ptr;
  }

  const LgFrSortingDemandScheduleFloat & constDemandSchedule = *demandSchedule;
  // Test itemTimeVecPairCompareFloat
  // method by doing sort and testing result

  // Display unsorted demandSchedule
  // cout <<"$$$$$$$$$$ Unsorted $$$$$$$$$$$$$$$" <<endl;
  // demandSchedule->print();

  // Sort the part schedule
  demandSchedule->sort( &cs1 );

  // Display sorted schedule
  // cout <<"$$$$$$$$$$  Sorted  $$$$$$$$$$$$$$$" <<endl;
  // demandSchedule->print();

  // Test to insure demandSchedule is sorted
  for ( i=1; i<demandSchedule->entries(); i++ ) {

     // Get demand volumes
     LgFrTimeVecFloat demVol1 ( 
            lps.demandVolume( &(constDemandSchedule[i-1].demand()) )  );
     LgFrTimeVecFloat demVol2 ( 
            lps.demandVolume( &(constDemandSchedule[ i ].demand()) )  );

  }
}
