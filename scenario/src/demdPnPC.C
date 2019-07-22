// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/demdPnPC.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/partSchd.h>
#include <scenario/src/demdSchd.h>
#include <scenario/src/machdep.h>
#include <scenario/src/idnumber.h>

// Do comparison
int
LgFrDemandPartNamePairCompare::itemTimeVecPairCompareFloat(
   const LgFrItemTimeVecPairFloat & itvpf1,
   const LgFrItemTimeVecPairFloat & itvpf2)
const
{
   int majorCompare = ( itvpf1.demand().partPtr()->compareTo(
                        itvpf2.demand().partPtr() )   );
  if (majorCompare == 0)        // Have to use minor key
    return itvpf1.demand().compareTo( &itvpf2.demand() );
  else
    return majorCompare;
}

// Make clone copy of object
LgFrPairCompareFloat *
LgFrDemandPartNamePairCompare::clone()
const
{
  LgFrDemandPartNamePairCompare* newInst = 
      new LgFrDemandPartNamePairCompare(*this);
  return (newInst);
}

// Make clone copy of object with new Scenario
LgFrPairCompareFloat *
LgFrDemandPartNamePairCompare::clone(LgFrScenario & newScenario)
const
{
  LgFrDemandPartNamePairCompare* newInst = 
      new LgFrDemandPartNamePairCompare(*this, newScenario);
  return (newInst);
}

// Assignment operator
LgFrDemandPartNamePairCompare &
LgFrDemandPartNamePairCompare::operator=(
			  const LgFrDemandPartNamePairCompare& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
     // nothing to do here
  }
  return *this;
}


// Copy constructor
LgFrDemandPartNamePairCompare::
   LgFrDemandPartNamePairCompare (
      const LgFrDemandPartNamePairCompare & source)
:  LgFrPairCompareFloat(source),
   id_(__LGFRDEMANDPARTNAMEPAIRCOMPARE)
{
}

// Copy constructorwith new Scenario
LgFrDemandPartNamePairCompare::
   LgFrDemandPartNamePairCompare (
      const LgFrDemandPartNamePairCompare & source,
      LgFrScenario & newScenario)
:  LgFrPairCompareFloat(source, newScenario),
   id_(__LGFRDEMANDPARTNAMEPAIRCOMPARE)
{
}


// Default Constructor
LgFrDemandPartNamePairCompare::
   LgFrDemandPartNamePairCompare ()
:  LgFrPairCompareFloat(),
   id_(__LGFRDEMANDPARTNAMEPAIRCOMPARE)
{
  // Nothing to do here
}

// Destructor
LgFrDemandPartNamePairCompare::
   ~LgFrDemandPartNamePairCompare ()
{
   // Nothing to do here
}


// test LgFrDemandPartNamePairCompare methods.
void
LgFrDemandPartNamePairCompare::test()
{
  int i,t;

  // Test default constructor
  LgFrDemandPartNamePairCompare cs1;

  // Test copy constructor
  LgFrDemandPartNamePairCompare cs2(cs1);

  // Test assignment operator
  LgFrDemandPartNamePairCompare cs3;
  cs3 = cs2;

  // Test clone method
  LgFrPairCompareFloat *cs4Ptr = cs1.clone();

  // Test destructor
  delete cs4Ptr;

  // Test itemTimeVecPairCompareFloat
  // method by doing sort and testing result

  // Create a demand schedule to sort
  const size_t nParts(5);
  const std::string parts[nParts]={"ddd","bbb","ccc","eee","aaa"};
  const size_t nDemands(5);
  const std::string demands[nDemands]=
      {"demand1","demand3","demand2","demand5","demand4"};
  LgFrSortingDemandScheduleFloat ds(nParts*nDemands);
  const LgFrSortingDemandScheduleFloat & constds = ds;
  LgFrPart part[nParts];
  LgFrDemand demand;
  LgFrTimeVecFloat tvf(10);
  srand(1);
  int p = 0; // Pulled out of the for below by RW2STL
  for ( p=0; p<nParts; p++ ) {
     // Create part
     part[p] = LgFrPart(parts[p]);
  
     int d = 0; // Pulled out of the for below by RW2STL
     for ( d=0; d<nDemands; d++ ) {
        // Create demand
        demand = LgFrDemand(demands[d], &(part[p]) );
        // Generate random timeVecFloat
        for ( t=0; t<tvf.length(); t++ ) tvf[t] = rand()/10.f; 
        // Insert demand and timeVecFloat in schedule
        ds.insertKeyAndValue( demand, tvf );
     }
  }

  // Display unsorted schedule
  // cout <<"$$$$$$$$$$ Unsorted $$$$$$$$$$$$$$$" <<endl;
  // ds.print();

  // Sort
  ds.sort( &cs1 );

  // Display sorted schedule
  // cout <<"$$$$$$$$$$  Sorted  $$$$$$$$$$$$$$$" <<endl;
  // ds.print();

  // Test to insure schedule is sorted
  for ( i=1; i<ds.size(); i++ ) {
     assert( constds[i-1].demand().partPtr()->compareTo(
             constds[ i ].demand().partPtr() ) <= 0  );
  }

  
}
