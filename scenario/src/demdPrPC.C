#include <iostream.h>

#include <scenario/src/demdPrPC.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/demdSchd.h>
#include <scenario/src/lastPrbS.h>
#include <scenario/src/machdep.h>
#include <scenario/src/demdSchP.h>

// Do comparison
int
LgFrDemandPriorityPairCompare::itemTimeVecPairCompareFloat(
   const LgFrItemTimeVecPairFloat & itvpf1,
   const LgFrItemTimeVecPairFloat & itvpf2)
const
{
   assert( itvpf1.item().isA() == __LGFRDEMAND );
   
   // Get demand priorities
   LgFrTimeVecFloat prior1 ( 
            lastProblemSolved().priority( &(itvpf1.demand()) )  );
   LgFrTimeVecFloat prior2 ( 
            lastProblemSolved().priority( &(itvpf2.demand()) )  );

   // Compare demand priorities
   int majorKey = prior1.compareTo(prior2);

   if ( majorKey != 0 )
      return majorKey;
   else	   // Demand priorities are equal, so compare itemTimeVecs
     return itvpf1.compareTo(&itvpf2);
}

// Make clone copy of object
LgFrPairCompareFloat *
LgFrDemandPriorityPairCompare::clone()
const
{
  LgFrDemandPriorityPairCompare* newInst = 
      new LgFrDemandPriorityPairCompare(*this);
  return (newInst);
}

// Make clone copy of object with new Scenario
LgFrPairCompareFloat *
LgFrDemandPriorityPairCompare::clone(LgFrScenario & newScenario)
const
{
  LgFrDemandPriorityPairCompare* newInst = 
      new LgFrDemandPriorityPairCompare(*this, newScenario);
  return (newInst);
}

// Return comparison strategy identifier
isAReturnType
LgFrDemandPriorityPairCompare::isA()
const
{
   return id_;
}

// Assignment operator
LgFrDemandPriorityPairCompare &
LgFrDemandPriorityPairCompare::operator=(
			  const LgFrDemandPriorityPairCompare& rhs)
{
  assert( id_ == rhs.id_ );
  if (this != &rhs) {		// Check for assignment to self
     (this->LgFrAttributeSensitivePairCompareFloat::operator= ) (rhs);
  }
  return *this;
}


// Copy constructor
LgFrDemandPriorityPairCompare::
   LgFrDemandPriorityPairCompare (
      const LgFrDemandPriorityPairCompare & source)
:  LgFrAttributeSensitivePairCompareFloat(source),
   id_(__LGFRDEMANDPRIORITYPAIRCOMPARE) 
{
  assert( id_ == source.id_ );
}

// Copy constructor with new Scenario
LgFrDemandPriorityPairCompare::
   LgFrDemandPriorityPairCompare (
      const LgFrDemandPriorityPairCompare & source,
      LgFrScenario & newScenario)
:  LgFrAttributeSensitivePairCompareFloat(source, newScenario),
   id_(__LGFRDEMANDPRIORITYPAIRCOMPARE) 
{
  assert( id_ == source.id_ );
}

// Constructor
LgFrDemandPriorityPairCompare::
   LgFrDemandPriorityPairCompare (
      LgFrScenario & scenario )
: LgFrAttributeSensitivePairCompareFloat( scenario ),
   id_(__LGFRDEMANDPRIORITYPAIRCOMPARE) 
{
  // Nothing to do here
}

// Default Constructor
LgFrDemandPriorityPairCompare::
   LgFrDemandPriorityPairCompare ()
:  LgFrAttributeSensitivePairCompareFloat(),
   id_(0)
{
  // Nothing to do here
}

// Destructor
LgFrDemandPriorityPairCompare::
   ~LgFrDemandPriorityPairCompare ()
{
   // Nothing to do here
}



#include <scenario/src/scenario.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrDemandPriorityPairCompare methods.
void
LgFrDemandPriorityPairCompare::test()
{
}

// test LgFrDemandPriorityPairCompare methods.
// Expects the data in testScenario to come from
// "/u/implode/wit/data/examples/diner12/wit.data"
void
LgFrDemandPriorityPairCompare::contextTest
  (LgFrScenario 	          & testScenario,
   LgFrLastProblemSolved          & lps,		// testScenario's 
   LgFrSetOfDemands               & sod,		// testScenario's
   LgFrSortingDemandScheduleFloatSmartPointer demandSchedule)	// testScenario's
{
  int i,t;
  
  // Test constructor
  LgFrDemandPriorityPairCompare cs1( testScenario );
  assert( cs1.id_ == __LGFRDEMANDPRIORITYPAIRCOMPARE );
  assert( cs1.lastProblemSolved().numberOfPeriods() == lps.numberOfPeriods() );

  // Test isA method
  assert( cs1.isA() == __LGFRDEMANDPRIORITYPAIRCOMPARE );

  // Test copy constructor
  LgFrDemandPriorityPairCompare cs2(cs1);
  assert( cs2.isA() == __LGFRDEMANDPRIORITYPAIRCOMPARE );
  assert( cs1.lastProblemSolved().numberOfPeriods() ==
          cs2.lastProblemSolved().numberOfPeriods()  );
  assert( cs1.workingImplosionProblem().implosionMethod() ==
          cs2.workingImplosionProblem().implosionMethod()  );

  // Test assignment operator
  LgFrDemandPriorityPairCompare cs3( testScenario );
  cs3 = cs2;
  assert( cs3.isA() == cs2.isA() );
  assert( cs3.workingImplosionProblem().equitability() ==
          cs2.workingImplosionProblem().equitability()  );

  {
    // Test clone method
    LgFrPairCompareFloat *cs4Ptr = cs1.clone();
    assert( cs4Ptr->isA() == cs1.isA() );
    assert( ((LgFrDemandPriorityPairCompare *)cs4Ptr)->
                workingImplosionProblem().timeVec( sod[0] ) ==
                cs1.workingImplosionProblem().timeVec( sod[0] ) );

    // Test destructor
    delete cs4Ptr;
  }

  // Set demand priorities to something interesting
  srand(1);
  LgFrTimeVecFloat randVec( cs1.lastProblemSolved().numberOfPeriods() );
  for ( i=1; i<sod.entries(); i++ ) {

     // Generate random timeVecFloat
     for ( t=0; t<randVec.length(); t++ ) randVec[t] = rand()/10.f;

     lps.priority( &(sod[i]), randVec );
  }

  const LgFrSortingDemandScheduleFloat & constDemandSchedule = *demandSchedule;
  // Test itemTimeVecPairCompareFloat
  // method by doing sort and testing result

  // Display unsorted schedule
  // cout <<"$$$$$$$$$$ Unsorted $$$$$$$$$$$$$$$" <<endl;
  // demandSchedule->print();

  // Sort the demandSchedule
  demandSchedule->sort( &cs1 );

  // Display sorted demandSchedule
  // cout <<"$$$$$$$$$$  Sorted  $$$$$$$$$$$$$$$" <<endl;
  // demandSchedule->print();

  // Test to insure demandSchedule is sorted
  for ( i=1; i<demandSchedule->entries(); i++ ) {
  //    cout <<"Priority " 
  //         <<i <<": "
  //         <<lps.priority( &((*demandSchedule)[i-1].demand()) ).format()
  //         <<endl;
     assert( lps.priority( &(constDemandSchedule[i-1].demand()) ) . compareTo (  
             lps.priority( &(constDemandSchedule[ i ].demand()) ) )  <= 0  );
  }
}
