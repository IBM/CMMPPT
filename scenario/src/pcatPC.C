#include <iostream.h>

#include <scenario/src/scenario.h>
#include <scenario/src/pcatPC.h>
#include <scenario/src/lastPrbS.h>
#include <scenario/src/wrkgProb.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/partSchd.h>
#include <scenario/src/machdep.h>

// Do comparison
int
LgFrPartCategoryPairCompareFloat::itemTimeVecPairCompareFloat(
   const LgFrItemTimeVecPairFloat & itvpf1,
   const LgFrItemTimeVecPairFloat & itvpf2)
const
{
   assert( itvpf1.item().isA() == __LGFRPART );
   
   // Get part categories
   LgFrPartCategory pcat1 = lastProblemSolved().category( &(itvpf1.part()) );
   LgFrPartCategory pcat2 = lastProblemSolved().category( &(itvpf2.part()) );

   // Compare part categories
   if      ( pcat1 < pcat2 ) return -1;
   else if ( pcat1 > pcat2 ) return  1;

   // Part categories are equal, so compare itemTimeVecs
   else                      return itvpf1.compareTo(&itvpf2);
}

// Make clone copy of object
LgFrPairCompareFloat *
LgFrPartCategoryPairCompareFloat::clone()
const
{
  LgFrPartCategoryPairCompareFloat* newInst = 
      new LgFrPartCategoryPairCompareFloat(*this);
  return (newInst);
}

// Make clone copy of object with new Scenario
LgFrPairCompareFloat *
LgFrPartCategoryPairCompareFloat::clone(LgFrScenario & newScenario)
const
{
  LgFrPartCategoryPairCompareFloat* newInst = 
      new LgFrPartCategoryPairCompareFloat(*this, newScenario);
  return (newInst);
}

// Return comparison strategy identifier
isAReturnType
LgFrPartCategoryPairCompareFloat::isA()
const
{
   return id_;
}

// Assignment operator
LgFrPartCategoryPairCompareFloat &
LgFrPartCategoryPairCompareFloat::operator=(
			  const LgFrPartCategoryPairCompareFloat& rhs)
{
  assert( id_ == rhs.id_ );
  if (this != &rhs) {		// Check for assignment to self
     (this->LgFrAttributeSensitivePairCompareFloat::operator= ) (rhs);
  }
  return *this;
}


// Copy constructor
LgFrPartCategoryPairCompareFloat::
   LgFrPartCategoryPairCompareFloat (
      const LgFrPartCategoryPairCompareFloat & source)
:  LgFrAttributeSensitivePairCompareFloat(source),
   id_(__LGFRPARTCATEGORYPAIRCOMPAREFLOAT) 
{
  assert( id_ == source.id_ );
}

// Copy constructor with new Scenario
LgFrPartCategoryPairCompareFloat::
   LgFrPartCategoryPairCompareFloat (
      const LgFrPartCategoryPairCompareFloat & source,
      LgFrScenario & newScenario)
:  LgFrAttributeSensitivePairCompareFloat(source, newScenario),
   id_(__LGFRPARTCATEGORYPAIRCOMPAREFLOAT) 
{
  assert( id_ == source.id_ );
}

// Constructor
LgFrPartCategoryPairCompareFloat::
   LgFrPartCategoryPairCompareFloat (
      LgFrScenario & scenario )
: LgFrAttributeSensitivePairCompareFloat( scenario ),
   id_(__LGFRPARTCATEGORYPAIRCOMPAREFLOAT) 
{
  // Nothing to do here
}

// Default Constructor
LgFrPartCategoryPairCompareFloat::
   LgFrPartCategoryPairCompareFloat ()
:  LgFrAttributeSensitivePairCompareFloat(),
   id_(0)
{
  // Nothing to do here
}

// Destructor
LgFrPartCategoryPairCompareFloat::
   ~LgFrPartCategoryPairCompareFloat ()
{
   // Nothing to do here
}


#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrPartCategoryPairCompareFloat methods.
void
LgFrPartCategoryPairCompareFloat::test()
{
}

// test LgFrPartCategoryPairCompareFloat methods.
// Expects the data in testScenario to come from
// "/u/implode/wit/data/examples/diner12/wit.data"
void
LgFrPartCategoryPairCompareFloat::contextTest
  (LgFrScenario               & testScenario,
   LgFrLastProblemSolved      & lps,			// testScenario's 
   LgFrSetOfParts             & sop)			// testScenario's 
{
  int i;

  // Now start testing of PartCategoryPairCompare
  
  // Test constructor
  LgFrPartCategoryPairCompareFloat cs1( testScenario );
  assert( cs1.id_ == __LGFRPARTCATEGORYPAIRCOMPAREFLOAT );
  assert( cs1.lastProblemSolved().numberOfPeriods() == 2 );

  // Test isA method
  assert( cs1.isA() == __LGFRPARTCATEGORYPAIRCOMPAREFLOAT );

  // Test copy constructor
  LgFrPartCategoryPairCompareFloat cs2(cs1);
  assert( cs2.isA() == __LGFRPARTCATEGORYPAIRCOMPAREFLOAT );
  assert( cs1.lastProblemSolved().numberOfPeriods() ==
          cs2.lastProblemSolved().numberOfPeriods()  );
  assert( cs1.workingImplosionProblem().implosionMethod() ==
          cs2.workingImplosionProblem().implosionMethod()  );

  // Test assignment operator
  LgFrPartCategoryPairCompareFloat cs3( testScenario );
  cs3 = cs2;
  assert( cs3.isA() == cs2.isA() );
  assert( cs1.workingImplosionProblem().equitability() ==
          cs2.workingImplosionProblem().equitability()  );

  {
    // Test clone method
    LgFrPairCompareFloat *cs4Ptr = cs1.clone();
    assert( cs4Ptr->isA() == cs1.isA() );
    LgFrTimeVecFloat cs4TV, cs1TV;
    {
      const LgFrLastProblemSolved &cs4LPS =
        ((LgFrPartCategoryPairCompareFloat *) cs4Ptr) ->
        lastProblemSolved();
      const LgFrLastProblemSolved &cs1LPS = cs1.lastProblemSolved();
      cs4TV  = cs4LPS.supplyVolume( &sop[0] );
      cs1TV =  cs1LPS.supplyVolume( &sop[0] );
      assert( cs1TV == cs4TV );

      cs4TV = ((LgFrPartCategoryPairCompareFloat *)cs4Ptr)->
        workingImplosionProblem().timeVec( sop[0] );
      cs1TV = cs1.workingImplosionProblem().timeVec( sop[0] );
      assert( cs1TV == cs4TV );
    }


    // Test destructor
    delete cs4Ptr;
  }
  
  // Test itemTimeVecPairCompareFloat
  // method by doing sort and testing result

  // Create a part schedule to sort
  const size_t psSize = sop.entries();
  LgFrSortingPartScheduleFloat ps(psSize);
  const LgFrSortingPartScheduleFloat & constps = ps;
  const LgFrLastProblemSolved &cs3LPS = cs3.lastProblemSolved();
  for ( i=0; i<psSize; i++ ) {
     // Insert part and timeVecFloat in schedule
     ps.insertKeyAndValue( 
           sop[i], 
           cs3LPS.supplyVolume( &sop[i] ) );
  }

  // Display unsorted schedule
  // cout <<"$$$$$$$$$$ Unsorted $$$$$$$$$$$$$$$" <<endl;
  // ps.print();

  // Sort the part schedule
  ps.sort( &cs1 );

  // Display sorted schedule
  // cout <<"$$$$$$$$$$  Sorted  $$$$$$$$$$$$$$$" <<endl;
  // ps.print();

  // Test to insure schedule is sorted
  for ( i=1; i<ps.entries(); i++ ) {
     LgFrPart pa (constps[i-1].part());
     LgFrPart pb (constps[i].part());
     assert( lps.category( &pa) <= lps.category( &pb));
  }
}
