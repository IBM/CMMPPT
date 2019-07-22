#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

// RW2STL -- inserted:
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/lastPrbS.h>
#include <scenario/src/explode.h>
#include <scenario/src/scenario.h>
#include <scenario/src/machdep.h>


// updateFrom - This method is invoked when the algorthm solution has
// been updated.
void
LgFrExplode::updateFrom (
   LgFrModel * ,
   void * a )
{
  if (a != 0)  {
    LgFrAlgorithm * algorithm = (LgFrAlgorithm *)a;

    if ( algorithm->GetAlgorithmType() == __LGFREXPLODE )
      changed( 0 );
  }
}


// Get Last Problem Solved
LgFrLastProblemSolved &
LgFrExplode::lastProblemSolved()
const
{
   assert (lpsPtr_ != 0);
   return *lpsPtr_;
}


// Return pointer to the explode algorithm in the specified scenario.
LgFrAlgorithm *
LgFrExplode::counterPart(
LgFrScenario & scenario)
const
{
   return &( scenario.explode() );
}


// Default constructor
LgFrExplode::LgFrExplode()
: 
  LgFrAlgorithm(),
  lpsPtr_(0),
  id_(__LGFREXPLODE)
{
  // All the work is done in the initialization part
}

// A Constructor
LgFrExplode::LgFrExplode(
   LgFrLastProblemSolved & lps )
: 
  LgFrAlgorithm(),
  lpsPtr_(&lps),
  id_(__LGFREXPLODE)
{
  assert (this->numberDependents() == 0);
  // Register with LPS to be informed when explosion solution changes
  lpsPtr_->addDependent( this );

}


//Copy constructor
LgFrExplode::LgFrExplode 
   (const LgFrExplode& source)
: 
  LgFrAlgorithm(source),
  lpsPtr_(source.lpsPtr_),
  id_(__LGFREXPLODE)
{
  assert (this->numberDependents() == 0);
  // Register with LPS to be informed when explosion solution changes
  if (lpsPtr_!=0) lpsPtr_->addDependent( this );
}

// Destructor
LgFrExplode::~LgFrExplode()
{
  // Unregister with LPS
  if (lpsPtr_!=0) lpsPtr_->removeDependent( this );
}

// Assignment
LgFrExplode&
LgFrExplode::operator=
(const LgFrExplode & )
{
  // Difficult to implement, so wait till it is necessary
  abort();
  return *this;
}


#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrExplode
void
LgFrExplode::test()
{
  // Test default constructor
  LgFrExplode defaultImplodeCtrl;
  assert (defaultImplodeCtrl.lpsPtr_ == 0);
}

//
void
LgFrExplode::contextTest(LgFrLastProblemSolved       &lps,
                         LgFrSetOfParts              &sop)
{
  // The data for this test should have come from
  // "/u/implode/wit/data/examples/diner12/wit.data"

  // Test non-default constructors
  LgFrExplode anExplodeCtrl (lps);
  assert(sop[0].name() == "Skillet");
  LgFrExplode copiedExplodeCtrl (anExplodeCtrl);
  assert ((copiedExplodeCtrl.lpsPtr_)->numberOfPeriods()
          == (anExplodeCtrl.lpsPtr_)->numberOfPeriods());
  assert ((anExplodeCtrl.lpsPtr_)->numberOfPeriods() == 2);

  // Test method to get an explode's lps
  assert( &(copiedExplodeCtrl.lastProblemSolved()) == &lps );
}
