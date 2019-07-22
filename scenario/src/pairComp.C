// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/scenario.h>
#include <scenario/src/pairComp.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/partSchd.h>
#include <scenario/src/machdep.h>

// Do comparison
int
LgFrPairCompareFloat::itemTimeVecPairCompareFloat(
   const LgFrItemTimeVecPairFloat & itvpf1,
   const LgFrItemTimeVecPairFloat & itvpf2)
const
{
   return itvpf1.compareTo(&itvpf2);
}

// Make clone copy of object
LgFrPairCompareFloat *
LgFrPairCompareFloat::clone()
const
{
  LgFrPairCompareFloat* newInst = 
      new LgFrPairCompareFloat(*this);
  return (newInst);
}

// Make clone copy of object with new Scenario
LgFrPairCompareFloat *
LgFrPairCompareFloat::clone(LgFrScenario & newScenario)
const
{
  LgFrPairCompareFloat* newInst = 
      new LgFrPairCompareFloat(*this, newScenario);
  return (newInst);
}

// Return the setOfParts which belongs to a scenario
const LgFrSetOfParts&
LgFrPairCompareFloat::constSetOfParts(
   int k,
   LgFrScenario & scenario)
{
   return scenario.setOfParts(k);
}

// Return the setOfDemands which belongs to a scenario
const LgFrSetOfDemands&
LgFrPairCompareFloat::constSetOfDemands(
   int k,
   LgFrScenario & scenario)
{
   return scenario.setOfDemands(k);
}

// Return the LastProblemSolved which belongs to a scenario
LgFrLastProblemSolved&
LgFrPairCompareFloat::lastProblemSolved(
   LgFrScenario & scenario)
{
   return scenario.lastProblemSolved();
}

// Return the WorkingImplosionProblem which belongs to a scenario
LgFrWorkingImplosionProblem&
LgFrPairCompareFloat::workingImplosionProblem(
   LgFrScenario & scenario)
{
   return scenario.workingImplosionProblem();
}

// Return the Implode which belongs to a scenario
LgFrImplode&
LgFrPairCompareFloat::implode(
   LgFrScenario & scenario)
{
   return scenario.implode();
}

// Return the focussed shortage schedule which belongs to a scenario
LgFrFocussedShortageScheduleControl&
LgFrPairCompareFloat::focussedShortageScheduleControl(
   LgFrScenario & scenario)
{
   return scenario.focussedShortageScheduleControl();
}

// Assignment operator
LgFrPairCompareFloat &
LgFrPairCompareFloat::operator=(
			  const LgFrPairCompareFloat& rhs)
{
  if (this != &rhs) {		// Check for assignment to self

     (this->LgFrSubscription::operator= ) (rhs);

     if ( algorithmPtr_ != 0 ) algorithmPtr_->removeDependentFromHeterogeneousList( this );
     algorithmPtr_ = rhs.algorithmPtr_;
     if ( algorithmPtr_ != 0 ) algorithmPtr_->addDependent( this );

  }
  return *this;
}


// Copy constructor
LgFrPairCompareFloat::
   LgFrPairCompareFloat (
      const LgFrPairCompareFloat & source)
:  
   LgFrSubscription(),
   algorithmPtr_(source.algorithmPtr_),
   id_(__LGFRPAIRCOMPAREFLOAT)
{
  if ( algorithmPtr_ != 0 ) algorithmPtr_->addDependent( this );
}

// Copy constructor associated with a new Scenario
LgFrPairCompareFloat::
   LgFrPairCompareFloat (
      const LgFrPairCompareFloat & source,
      LgFrScenario & ,
      LgFrModel * algorithmPtr )
:  
   LgFrSubscription(),
   algorithmPtr_(algorithmPtr),
   id_(__LGFRPAIRCOMPAREFLOAT)
{
  if ( algorithmPtr_ != 0 ) algorithmPtr_->addDependent( this );
}


// Default Constructor
LgFrPairCompareFloat::
   LgFrPairCompareFloat (
      LgFrModel * algorithmPtr )
:  
   LgFrSubscription(),
   algorithmPtr_(algorithmPtr),
   id_(__LGFRPAIRCOMPAREFLOAT)
{
  if ( algorithmPtr_ != 0 ) algorithmPtr_->addDependent( this );
}

// Destructor
LgFrPairCompareFloat::
   ~LgFrPairCompareFloat ()
{
  if ( algorithmPtr_ != 0 ) algorithmPtr_->removeDependentFromHeterogeneousList( this );
}


#ifdef NDEBUG
#undef NDEBUG
#endif

// test LgFrPairCompareFloat methods.
void
LgFrPairCompareFloat::test()
{
  int i,t;

  // Test default constructor
  LgFrPairCompareFloat cs1;

  // Test copy constructor
  LgFrPairCompareFloat cs2(cs1);

  // Test assignment operator
  LgFrPairCompareFloat cs3;
  cs3 = cs2;

  // Test clone method
  LgFrPairCompareFloat *cs4Ptr = cs1.clone();

  // Test destructor
  delete cs4Ptr;

  // Test itemTimeVecPairCompareFloat
  // method by doing sort and testing result

  // Create a part schedule to sort
  const size_t psSize(100);
  LgFrSortingPartScheduleFloat ps(psSize); 
  const LgFrSortingPartScheduleFloat & constps = ps;
  std::string partName;
  LgFrPart part;
  LgFrTimeVecFloat tvf(10);
  srand( 1 );
  char chars[]="abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUV";
  int nChars = sizeof(chars)-1;
  for ( i=0; ps.size()<psSize; i++ ) {
     // Generate 2 character partname
     partName = std::string(1,chars[rand()%nChars]) + 
                std::string(1,chars[rand()%nChars]);
     // Create part
     part = LgFrPart(partName);
     // Make sure part was not already created
     if ( ! (ps.contains(part)) ) {
       // Generate random timeVecFloat
       for ( t=0; t<tvf.length(); t++ ) tvf[t] = rand()/10.f; 
       // Insert part and timeVecFloat in schedule
       ps.insertKeyAndValue( part, tvf );
     }
  }

  // Display unsorted schedule
  // cout <<"$$$$$$$$$$ Unsorted $$$$$$$$$$$$$$$" <<endl;
  // ps.print();

  // Set compare strategy and Sort the part schedule
  ps.sort( &cs1 );

  // Display sorted schedule
  // cout <<"$$$$$$$$$$  Sorted  $$$$$$$$$$$$$$$" <<endl;
  // ps.print();

  // Test to insure schedule is sorted
  for ( i=1; i<ps.size(); i++ ) {
     LgFrItemTimeVecPairFloat itvf (constps[i]);
     assert( constps[i-1].compareTo(& itvf) <= 0 );
  }

  
}
