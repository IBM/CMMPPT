// RW2STL -- inserted:
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/implode.h>
#include <scenario/src/machdep.h>
#include <scenario/src/scenario.h>


// updateFrom - This method is invoked when the algorthm solution has
// been updated.
void
LgFrImplode::updateFrom (
   LgFrModel * ,
   void * a )
{
  if (a != 0)  {
    LgFrAlgorithm * algorithm = (LgFrAlgorithm *)a;

    if ( algorithm->GetAlgorithmType() == __LGFRIMPLODE )
      changed( 0 );
  }
}

// Get Last Problem Solved
LgFrLastProblemSolved &
LgFrImplode::lastProblemSolved()
const
{
   assert (lpsPtr_ != 0);
   return *lpsPtr_;
}


// Return pointer to the implode algorithm in the specified scenario.
LgFrAlgorithm *
LgFrImplode::counterPart(
LgFrScenario & scenario)
const
{
   return &( scenario.implode() );
}

// Default constructor
LgFrImplode::LgFrImplode()
: 
  LgFrAlgorithm(),
  lpsPtr_(0),
  id_(__LGFRIMPLODE)
{
  assert (this->numberDependents() == 0);
  // All the work is done in the initialization part
}

// A Constructor
LgFrImplode::LgFrImplode(
   LgFrLastProblemSolved & lps )
: 
  LgFrAlgorithm(),
  lpsPtr_(&lps),
  id_(__LGFRIMPLODE)
{
  assert (this->numberDependents() == 0);
  // Register with LPS to be informed when implosion solution changes
  lpsPtr_->addDependent( this );
}


//Copy constructor
LgFrImplode::LgFrImplode 
   (const LgFrImplode& source)
: 
  LgFrAlgorithm(source),
  lpsPtr_(source.lpsPtr_),
  id_(__LGFRIMPLODE)
{
  assert (this->numberDependents() == 0);
  // Register with LPS to be informed when implosion solution changes
  if (lpsPtr_!=0) lpsPtr_->addDependent( this );
}

// Destructor
LgFrImplode::~LgFrImplode()
{
  // Unregister with LPS
  if (lpsPtr_!=0) lpsPtr_->removeDependent( this );
}

// Assignment
LgFrImplode&
LgFrImplode::operator=
(const LgFrImplode& )
{
  // Difficult to implement, so wait till it is necessary
  abort();
  return *this;
}

#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrImplode
void
LgFrImplode::test()
{

  // Test default constructor
  LgFrImplode defaultImplodeCtrl;
  assert (defaultImplodeCtrl.lpsPtr_ == 0);

}

//
void
LgFrImplode::contextTest(LgFrLastProblemSolved       &lps,
                         LgFrSetOfParts              &sop)
{
  // The data for this test should have come from
  // "/u/implode/wit/data/examples/diner12/wit.data"

  // Test non-default constructors
  LgFrImplode anImplodeCtrl (lps);
  assert(sop[0].name() == "Skillet");
  LgFrImplode copiedImplodeCtrl (anImplodeCtrl);
  assert ((copiedImplodeCtrl.lpsPtr_)->numberOfPeriods()
          == (anImplodeCtrl.lpsPtr_)->numberOfPeriods());
  assert ((anImplodeCtrl.lpsPtr_)->numberOfPeriods() == 2);
}
