// RW2STL -- inserted:
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/usrInMod.h>
#include <scenario/src/machdep.h>
#include <scenario/src/scenario.h>


// updateFrom - This method is invoked when the algorithm solution has
// been updated.  We only actually want something to happen if the user
// has updated things.  If the output solution changes, we don't care as
// only input schedules should be subscribing to objects of this class.
void
LgFrUserInputModification::updateFrom (
   LgFrModel * ,
   void * a )
{
  if (a != 0)  {
    LgFrAlgorithm * algorithm = (LgFrAlgorithm *)a;
    
    if ( algorithm->GetAlgorithmType() == __LGFRUSERINPUTMODIFICATION ) 
      changed( 0 );
  }
}

// Get Working Implosion Problem
LgFrWorkingImplosionProblem &
LgFrUserInputModification::workingImplosionProblem()
const
{
   assert (wipPtr_ != 0);
   return *wipPtr_;
}


// Return pointer to the implode algorithm in the specified scenario.
LgFrAlgorithm *
LgFrUserInputModification::counterPart(
LgFrScenario & scenario)
const
{
   return &( scenario.userInputModification() );
}

// Default constructor
LgFrUserInputModification::LgFrUserInputModification()
: 
  LgFrAlgorithm(),
  wipPtr_(0),
  id_(__LGFRUSERINPUTMODIFICATION)
{
  assert (this->numberDependents() == 0);
  // All the work is done in the initialization part
}

// A Constructor
LgFrUserInputModification::LgFrUserInputModification(
   LgFrWorkingImplosionProblem & wip )
: 
  LgFrAlgorithm(),
  wipPtr_(&wip),
  id_(__LGFRUSERINPUTMODIFICATION)
{
  assert (this->numberDependents() == 0);
  // Register with WIP to be informed when working implosion problem changes
  wipPtr_->addDependent( this );
}


//Copy constructor
LgFrUserInputModification::LgFrUserInputModification 
   (const LgFrUserInputModification& source)
: 
  LgFrAlgorithm(source),
  wipPtr_(source.wipPtr_),
  id_(__LGFRUSERINPUTMODIFICATION)
{
  assert (this->numberDependents() == 0);
  // Register with WIP to be informed when working implosion problem changes
  if (wipPtr_!=0) wipPtr_->addDependent( this );
}

// Destructor
LgFrUserInputModification::~LgFrUserInputModification()
{
  // Unregister with WIP
  if (wipPtr_!=0) wipPtr_->removeDependent( this );
}

// Assignment
LgFrUserInputModification&
LgFrUserInputModification::operator=
(const LgFrUserInputModification& )
{
  // Difficult to implement, so wait till it is necessary
  abort();
  return *this;
}

#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrUserInputModification
void
LgFrUserInputModification::test()
{

  // Test default constructor
  LgFrUserInputModification defaultImplodeCtrl;
  assert (defaultImplodeCtrl.wipPtr_ == 0);

}

//
void
LgFrUserInputModification::contextTest(LgFrWorkingImplosionProblem &wip)
{
  // The data for this test should have come from
  // "/u/implode/wit/data/examples/diner12/wit.data"

  // Test non-default constructors
  LgFrUserInputModification anImplodeCtrl (wip);
  LgFrUserInputModification copiedImplodeCtrl (anImplodeCtrl);
  assert ((copiedImplodeCtrl.wipPtr_)->implosionMethod()
          == (anImplodeCtrl.wipPtr_)->implosionMethod());
}
