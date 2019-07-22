// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

#include <iostream>

#include <scenario/src/boolFS.h>
#include <scenario/src/sortSchd.h>
#include <scenario/src/prodSS.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/demdNmFS.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/machdep.h>

//-------------------------------------------------------------------------
//
// LgFrDiadicFilterStrategy Methods
//
//-------------------------------------------------------------------------

// Set left hand side filter strategy
void
LgFrDiadicFilterStrategy::leftHandSideFilterStrategy(
   const LgFrFilterStrategy & lhs)
{
   assert( leftHandSideFilterStrategyPtr_ != 0 );
   leftHandSideFilterStrategyPtr_->removeDependent( this );
   delete leftHandSideFilterStrategyPtr_;

   leftHandSideFilterStrategyPtr_ = lhs.clone();
   leftHandSideFilterStrategyPtr_->addDependent( this );
}


// Get left hand side filter strategy
const LgFrFilterStrategy &
LgFrDiadicFilterStrategy::leftHandSideFilterStrategy()
const
{
   assert (leftHandSideFilterStrategyPtr_ != 0);
   return *leftHandSideFilterStrategyPtr_;
}


// Set right hand side filter strategy
void
LgFrDiadicFilterStrategy::rightHandSideFilterStrategy(
   const LgFrFilterStrategy & rhs)
{
   assert( rightHandSideFilterStrategyPtr_ != 0 );
   rightHandSideFilterStrategyPtr_->removeDependent( this );
   delete rightHandSideFilterStrategyPtr_;

   rightHandSideFilterStrategyPtr_ = rhs.clone();
   rightHandSideFilterStrategyPtr_->addDependent( this );
}


// Get right hand side filter strategy
const LgFrFilterStrategy &
LgFrDiadicFilterStrategy::rightHandSideFilterStrategy()
const
{
   assert (rightHandSideFilterStrategyPtr_ != 0);
   return *rightHandSideFilterStrategyPtr_;
}

// Assignment operator
LgFrDiadicFilterStrategy &
LgFrDiadicFilterStrategy::operator=(
			  const LgFrDiadicFilterStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self

     (this->LgFrFilterStrategy::operator= ) (rhs);

     assert( leftHandSideFilterStrategyPtr_ != 0 );
     leftHandSideFilterStrategyPtr_->removeDependent( this );
     delete leftHandSideFilterStrategyPtr_;
     leftHandSideFilterStrategyPtr_  = 
        rhs.leftHandSideFilterStrategyPtr_->clone();
     leftHandSideFilterStrategyPtr_->addDependent( this );

     assert( rightHandSideFilterStrategyPtr_ != 0 );
     rightHandSideFilterStrategyPtr_->removeDependent( this );
     delete rightHandSideFilterStrategyPtr_;
     rightHandSideFilterStrategyPtr_ = 
        rhs.rightHandSideFilterStrategyPtr_->clone();
     rightHandSideFilterStrategyPtr_->addDependent( this );

  }
  return *this;
}


// Copy constructor
LgFrDiadicFilterStrategy::
   LgFrDiadicFilterStrategy (
      const LgFrDiadicFilterStrategy & source)
:  
   LgFrFilterStrategy( source ),
   leftHandSideFilterStrategyPtr_ ( 0 ),
   rightHandSideFilterStrategyPtr_( 0 )
{
   leftHandSideFilterStrategyPtr_  = 
      source.leftHandSideFilterStrategyPtr_->clone();
   leftHandSideFilterStrategyPtr_->addDependent( this );

   rightHandSideFilterStrategyPtr_ = 
      source.rightHandSideFilterStrategyPtr_->clone();
   rightHandSideFilterStrategyPtr_->addDependent( this );
}

// Copy constructor with new Scenario
LgFrDiadicFilterStrategy::
   LgFrDiadicFilterStrategy (
      const LgFrDiadicFilterStrategy & source,
      LgFrScenario & newScenario)
:  
   LgFrFilterStrategy( source, newScenario ),
   leftHandSideFilterStrategyPtr_ ( 0 ),
   rightHandSideFilterStrategyPtr_( 0 )
{

   leftHandSideFilterStrategyPtr_  = 
      source.leftHandSideFilterStrategyPtr_->clone(newScenario);
   leftHandSideFilterStrategyPtr_->addDependent( this );

   rightHandSideFilterStrategyPtr_  = 
      source.rightHandSideFilterStrategyPtr_->clone(newScenario);
   rightHandSideFilterStrategyPtr_->addDependent( this );

}


// Default Constructor
LgFrDiadicFilterStrategy::
   LgFrDiadicFilterStrategy ()
:  
   LgFrFilterStrategy()
{

   leftHandSideFilterStrategyPtr_ = new LgFrFilterStrategy;
   leftHandSideFilterStrategyPtr_->addDependent( this );

   rightHandSideFilterStrategyPtr_ = new LgFrFilterStrategy;
   rightHandSideFilterStrategyPtr_->addDependent( this );

}


// Another Constructor
LgFrDiadicFilterStrategy::
   LgFrDiadicFilterStrategy (
      LgFrFilterStrategy & leftHandSideFilterStrategy,
      LgFrFilterStrategy & rightHandSideFilterStrategy ) 
:  
   LgFrFilterStrategy(),
   leftHandSideFilterStrategyPtr_ ( 0 ),
   rightHandSideFilterStrategyPtr_ ( 0 )
{
   leftHandSideFilterStrategyPtr_ = leftHandSideFilterStrategy.clone();
   leftHandSideFilterStrategyPtr_->addDependent( this );

   rightHandSideFilterStrategyPtr_ = rightHandSideFilterStrategy.clone();
   rightHandSideFilterStrategyPtr_->addDependent( this );
}


// Destructor
LgFrDiadicFilterStrategy::
   ~LgFrDiadicFilterStrategy ()
{

  leftHandSideFilterStrategyPtr_->removeDependent( this );
  delete leftHandSideFilterStrategyPtr_;

  rightHandSideFilterStrategyPtr_->removeDependent( this );
  delete rightHandSideFilterStrategyPtr_;

}



//-------------------------------------------------------------------------
//
// LgFrAndFilterStrategy Methods
//
//-------------------------------------------------------------------------

// Do Filtering
bool
LgFrAndFilterStrategy::exclude(
   const LgFrItemTimeVecPairFloat & itvpf)
const
{

   // Evaluate left and right filter strategies
   bool lhs(  leftHandSideFilterStrategy().exclude( itvpf ) );
   bool rhs( rightHandSideFilterStrategy().exclude( itvpf ) );

   return ( lhs && rhs );

}


// Make clone copy of object
LgFrFilterStrategy *
LgFrAndFilterStrategy::clone()
const
{
  LgFrAndFilterStrategy* newInst = 
      new LgFrAndFilterStrategy(*this);
  return (newInst);
}

// Make clone copy of object with new Scenario
LgFrFilterStrategy *
LgFrAndFilterStrategy::clone(LgFrScenario & newScenario)
const
{
  LgFrAndFilterStrategy* newInst = 
      new LgFrAndFilterStrategy(*this, newScenario);
  return (newInst);
}

// Assignment operator
LgFrAndFilterStrategy &
LgFrAndFilterStrategy::operator=(
			  const LgFrAndFilterStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
     (this->LgFrDiadicFilterStrategy::operator= ) (rhs);
  }
  return *this;
}


// Copy constructor
LgFrAndFilterStrategy::
   LgFrAndFilterStrategy (
      const LgFrAndFilterStrategy & source)
:  
   LgFrDiadicFilterStrategy( source ),
   id_(__LGFRANDFILTERSTRATEGY)
{
}

// Copy constructor with new Scenario
LgFrAndFilterStrategy::
   LgFrAndFilterStrategy (
      const LgFrAndFilterStrategy & source,
      LgFrScenario & newScenario)
:  
   LgFrDiadicFilterStrategy( source, newScenario ),
   id_(__LGFRANDFILTERSTRATEGY)
{
}


// Constructor
LgFrAndFilterStrategy::
   LgFrAndFilterStrategy ()
:  
   LgFrDiadicFilterStrategy(),
   id_(__LGFRANDFILTERSTRATEGY)
{
  // Nothing to do here
}

// Another Constructor
LgFrAndFilterStrategy::
   LgFrAndFilterStrategy (
     LgFrFilterStrategy & leftHandSideFilterStrategy,
     LgFrFilterStrategy & rightHandSideFilterStrategy )
:  
   LgFrDiadicFilterStrategy(
      leftHandSideFilterStrategy,
      rightHandSideFilterStrategy),
   id_(__LGFRANDFILTERSTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrAndFilterStrategy::
   ~LgFrAndFilterStrategy ()
{
   // Nothing to do here
}





//-------------------------------------------------------------------------
//
// LgFrOrFilterStrategy Methods
//
//-------------------------------------------------------------------------

// Do Filtering
bool
LgFrOrFilterStrategy::exclude(
   const LgFrItemTimeVecPairFloat & itvpf)
const
{

   // Evaluate left and right filter strategies
   bool lhs(  leftHandSideFilterStrategy().exclude( itvpf ) );
   bool rhs( rightHandSideFilterStrategy().exclude( itvpf ) );

   return ( lhs || rhs );

}


// Make clone copy of object
LgFrFilterStrategy *
LgFrOrFilterStrategy::clone()
const
{
  LgFrOrFilterStrategy* newInst = 
      new LgFrOrFilterStrategy(*this);
  return (newInst);
}

// Make clone copy of object with new Scenario
LgFrFilterStrategy *
LgFrOrFilterStrategy::clone(LgFrScenario & newScenario)
const
{
  LgFrOrFilterStrategy* newInst = 
      new LgFrOrFilterStrategy(*this, newScenario);
  return (newInst);
}

// Assignment operator
LgFrOrFilterStrategy &
LgFrOrFilterStrategy::operator=(
			  const LgFrOrFilterStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
     (this->LgFrDiadicFilterStrategy::operator= ) (rhs);
  }
  return *this;
}


// Copy constructor
LgFrOrFilterStrategy::
   LgFrOrFilterStrategy (
      const LgFrOrFilterStrategy & source)
:  
   LgFrDiadicFilterStrategy( source ),
   id_(__LGFRORFILTERSTRATEGY)
{
}

// Copy constructor with new Scenario
LgFrOrFilterStrategy::
   LgFrOrFilterStrategy (
      const LgFrOrFilterStrategy & source,
      LgFrScenario & newScenario)
:  
   LgFrDiadicFilterStrategy( source, newScenario ),
   id_(__LGFRORFILTERSTRATEGY)
{
}


// Constructor
LgFrOrFilterStrategy::
   LgFrOrFilterStrategy ()
:  
   LgFrDiadicFilterStrategy(),
   id_(__LGFRORFILTERSTRATEGY)
{
  // Nothing to do here
}

// Another Constructor
LgFrOrFilterStrategy::
   LgFrOrFilterStrategy (
     LgFrFilterStrategy & leftHandSideFilterStrategy,
     LgFrFilterStrategy & rightHandSideFilterStrategy )
:  
   LgFrDiadicFilterStrategy(
      leftHandSideFilterStrategy,
      rightHandSideFilterStrategy),
   id_(__LGFRORFILTERSTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrOrFilterStrategy::
   ~LgFrOrFilterStrategy ()
{
   // Nothing to do here
}




//-------------------------------------------------------------------------
//
// LgFrNotFilterStrategy Methods
//
//-------------------------------------------------------------------------

// Do Filtering
bool
LgFrNotFilterStrategy::exclude(
   const LgFrItemTimeVecPairFloat & itvpf)
const
{

   // Evaluate filter strategies
   bool fs(  filterStrategy().exclude( itvpf ) );

   return !fs;

}


// Set filter strategy
void
LgFrNotFilterStrategy::filterStrategy(
   const LgFrFilterStrategy & fs)
{
   assert( filterStrategyPtr_ != 0 );
   filterStrategyPtr_->removeDependent( this );
   delete filterStrategyPtr_;

   filterStrategyPtr_ = fs.clone();
   filterStrategyPtr_->addDependent( this );
}

// Get filter strategy
const LgFrFilterStrategy &
LgFrNotFilterStrategy::filterStrategy()
const
{
   assert (filterStrategyPtr_ != 0);
   return *filterStrategyPtr_;
}


// Make clone copy of object
LgFrFilterStrategy *
LgFrNotFilterStrategy::clone()
const
{
  LgFrNotFilterStrategy* newInst = 
      new LgFrNotFilterStrategy(*this);
  return (newInst);
}

// Make clone copy of object with new Scenario
LgFrFilterStrategy *
LgFrNotFilterStrategy::clone(LgFrScenario & newScenario)
const
{
  LgFrNotFilterStrategy* newInst = 
      new LgFrNotFilterStrategy(*this, newScenario);
  return (newInst);
}

// Assignment operator
LgFrNotFilterStrategy &
LgFrNotFilterStrategy::operator=(
			  const LgFrNotFilterStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
     (this->LgFrFilterStrategy::operator= ) (rhs);

     assert( filterStrategyPtr_ != 0 );
     filterStrategyPtr_->removeDependent( this );
     delete filterStrategyPtr_;
     filterStrategyPtr_  = 
        rhs.filterStrategyPtr_->clone();
     filterStrategyPtr_->addDependent( this );

  }
  return *this;
}


// Copy constructor
LgFrNotFilterStrategy::
   LgFrNotFilterStrategy (
      const LgFrNotFilterStrategy & source)
:  
   LgFrFilterStrategy( source ),
   filterStrategyPtr_( 0 ),
   id_(__LGFRNOTFILTERSTRATEGY)
{
  filterStrategyPtr_  = source.filterStrategyPtr_->clone();
  filterStrategyPtr_->addDependent( this );
}


// Copy constructor with new Scenario
LgFrNotFilterStrategy::
   LgFrNotFilterStrategy (
      const LgFrNotFilterStrategy & source,
      LgFrScenario & newScenario)
:  
   LgFrFilterStrategy( source, newScenario ),
   id_(__LGFRNOTFILTERSTRATEGY)
{
  filterStrategyPtr_  = source.filterStrategyPtr_->clone(newScenario);
  filterStrategyPtr_->addDependent( this );
}


// Constructor
LgFrNotFilterStrategy::
   LgFrNotFilterStrategy ()
:  
   LgFrFilterStrategy(),
   id_(__LGFRNOTFILTERSTRATEGY)
{
   filterStrategyPtr_ = new LgFrFilterStrategy;
   filterStrategyPtr_->addDependent( this );
}

// Another Constructor
LgFrNotFilterStrategy::
   LgFrNotFilterStrategy (
     const LgFrFilterStrategy & filterStrategy )
:  
   LgFrFilterStrategy(),
   filterStrategyPtr_ ( 0 ),
   id_(__LGFRNOTFILTERSTRATEGY)
{
   filterStrategyPtr_ = filterStrategy.clone();
   filterStrategyPtr_->addDependent( this );
}

// Destructor
LgFrNotFilterStrategy::
   ~LgFrNotFilterStrategy ()
{
   assert( filterStrategyPtr_ != 0 );
   filterStrategyPtr_->removeDependent( this );
   delete filterStrategyPtr_;
}

//-------------------------------------------------------------------------
//
// Test Methods for all the classes in this file
//
//-------------------------------------------------------------------------

#ifdef NDEBUG
#undef NDEBUG
#endif

// test LgFrNotFilterStrategy methods.
void
LgFrNotFilterStrategy::test()
{

  // Test default constructor
  LgFrNotFilterStrategy fs1;

  // Test copy constructor
  LgFrNotFilterStrategy fs2(fs1);

  // Test assignment operator
  LgFrNotFilterStrategy fs3;
  fs3 = fs2;

  // Test clone method
  LgFrFilterStrategy *fs4Ptr = fs1.clone();

  // Test destructor
  delete fs4Ptr;

  // Test setting/getting filter strategy 
  fs1.filterStrategy( LgFrZeroTimeVecFilterStrategy() );

  // Test alternate constructor
  LgFrZeroTimeVecFilterStrategy zeroTvFS;
  LgFrNotFilterStrategy fs5( zeroTvFS );

  // Test exclude
  LgFrItemTimeVecPairFloat itvp1( LgFrPart("abc"), LgFrTimeVecFloat(2,2.) );
  assert( fs5.exclude( itvp1 ) );

  itvp1.timeVecFloat( LgFrTimeVecFloat(2, 0.0) );
  assert( !fs5.exclude( itvp1 ) );

}


// test LgFrAndFilterStrategy methods.
void
LgFrAndFilterStrategy::test()
{

  // Test default constructor
  LgFrAndFilterStrategy fs1;

  // Test copy constructor
  LgFrAndFilterStrategy fs2(fs1);

  // Test assignment operator
  LgFrAndFilterStrategy fs3;
  fs3 = fs2;

  // Test clone method
  LgFrFilterStrategy *fs4Ptr = fs1.clone();

  // Test destructor
  delete fs4Ptr;

  fs1.leftHandSideFilterStrategy( LgFrZeroTimeVecFilterStrategy() );

  // Test setting/getting rhs filter strategy which are methods of 
  // the abstract base class LgFrDiadicFilterStrategy
  fs1.rightHandSideFilterStrategy( LgFrDemandNameFilterStrategy() );

  // Test alternate constructor
  LgFrFilterStrategy fs;
  LgFrZeroTimeVecFilterStrategy zeroTvFS;
  LgFrAndFilterStrategy fs5( fs, zeroTvFS );

  // Test exclude
  LgFrItemTimeVecPairFloat itvp1( LgFrPart("abc"), LgFrTimeVecFloat(2,2.) );
  assert( !fs5.exclude( itvp1 ) );

  itvp1.timeVecFloat( LgFrTimeVecFloat(2, 0.0) );
  assert( !fs5.exclude( itvp1 ) );

  fs5.leftHandSideFilterStrategy( zeroTvFS );
  assert( fs5.exclude( itvp1 ) );

}


// test LgFrOrFilterStrategy methods.
void
LgFrOrFilterStrategy::test()
{

  // Test default constructor
  LgFrOrFilterStrategy fs1;

  // Test copy constructor
  LgFrOrFilterStrategy fs2(fs1);

  // Test assignment operator
  LgFrOrFilterStrategy fs3;
  fs3 = fs2;

  // Test clone method
  LgFrFilterStrategy *fs4Ptr = fs1.clone();

  // Test destructor
  delete fs4Ptr;

  // Test setting/getting lhs filter strategy which are methods of 
  // the abstract base class
  fs1.leftHandSideFilterStrategy( LgFrZeroTimeVecFilterStrategy() );

  // Test setting/getting rhs filter strategy which are methods of 
  // the abstract base class
  fs1.rightHandSideFilterStrategy( LgFrDemandNameFilterStrategy() );

  // Test alternate constructor
  LgFrFilterStrategy fs;
  LgFrZeroTimeVecFilterStrategy zeroTvFS;
  LgFrOrFilterStrategy fs5( fs, zeroTvFS );

  // Test exclude
  LgFrItemTimeVecPairFloat itvp1( LgFrPart("abc"), LgFrTimeVecFloat(2,2.) );
  assert( !fs5.exclude( itvp1 ) );

  itvp1.timeVecFloat( LgFrTimeVecFloat(2, 0.0) );
  assert( fs5.exclude( itvp1 ) );

}

