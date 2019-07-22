#include <iostream.h>

#include <scenario/src/timVComp.h>
#include <scenario/src/timeVecs.h>
#include <scenario/src/machdep.h>

#if defined (WIN32) || defined (__OS2__) || defined (__WINDOWS__)
#undef max
#undef min
#endif


// ---------------------------------------------------------------------------
//
// LgFrTimeVecCompareFloat methods.
//
// ---------------------------------------------------------------------------

// Do comparison
int
LgFrTimeVecCompareFloat::compare(
   const LgFrTimeVecFloat & tvf1,
   const LgFrTimeVecFloat & tvf2)
const
{
   return tvf1.compareTo(tvf2);
}


// Make clone copy of object
LgFrTimeVecCompareFloat *
LgFrTimeVecCompareFloat::clone()
const
{
  LgFrTimeVecCompareFloat* newInst = 
      new LgFrTimeVecCompareFloat(*this);
  return (newInst);
}

// Return comparison strategy identifier
isAReturnType
LgFrTimeVecCompareFloat::isA()
const
{
   return id_;
}

// Assignment operator
LgFrTimeVecCompareFloat &
LgFrTimeVecCompareFloat::operator=(
			  const LgFrTimeVecCompareFloat& rhs)
{
  assert( id_ == rhs.id_ );
  if (this != &rhs) {		// Check for assignment to self
     // Nothing to do here
  }
  return *this;
}


// Copy constructor
LgFrTimeVecCompareFloat::
   LgFrTimeVecCompareFloat (
      const LgFrTimeVecCompareFloat & source)
:
   id_(__LGFRTIMEVECCOMPAREFLOAT)
{
  assert( id_ == source.id_ );
}


// Default Constructor
LgFrTimeVecCompareFloat::
   LgFrTimeVecCompareFloat ()
:  
   id_(__LGFRTIMEVECCOMPAREFLOAT)
{
  // Nothing to do here
}

// Destructor
LgFrTimeVecCompareFloat::
   ~LgFrTimeVecCompareFloat () {
   // Nothing to do here
}



// ---------------------------------------------------------------------------
//
// LgFrMaximumElementTimeVecCompareFloat methods.
//
// ---------------------------------------------------------------------------

// Do comparison
int
LgFrMaximumElementTimeVecCompareFloat::compare(
   const LgFrTimeVecFloat & tvf1,
   const LgFrTimeVecFloat & tvf2)
const
{
   float tvf1Max( tvf1.max() );
   float tvf2Max( tvf2.max() );

   if      ( tvf1Max < tvf2Max ) return -1;
   else if ( tvf1Max > tvf2Max ) return 1;
   else return 0;
}


// Make clone copy of object
LgFrTimeVecCompareFloat *
LgFrMaximumElementTimeVecCompareFloat::clone()
const
{
  LgFrMaximumElementTimeVecCompareFloat* newInst = 
      new LgFrMaximumElementTimeVecCompareFloat(*this);
  return (newInst);
}

// Return comparison strategy identifier
isAReturnType
LgFrMaximumElementTimeVecCompareFloat::isA()
const
{
   return id_;
}

// Assignment operator
LgFrTimeVecCompareFloat &
LgFrMaximumElementTimeVecCompareFloat::operator=(
	  const LgFrMaximumElementTimeVecCompareFloat& rhs)
{
  assert( id_ == rhs.id_ );
  if (this != &rhs) {		// Check for assignment to self
     // Nothing to do here
  }
  return *this;
}


// Copy constructor
LgFrMaximumElementTimeVecCompareFloat::
   LgFrMaximumElementTimeVecCompareFloat (
      const LgFrMaximumElementTimeVecCompareFloat & source)
:
   id_(__LGFRMAXIMUMELEMENTTIMEVECCOMPAREFLOAT)
{
  assert( id_ == source.id_ );
}


// Default Constructor
LgFrMaximumElementTimeVecCompareFloat::
   LgFrMaximumElementTimeVecCompareFloat ()
:  
   id_(__LGFRMAXIMUMELEMENTTIMEVECCOMPAREFLOAT)
{
  // Nothing to do here
}

// Destructor
LgFrMaximumElementTimeVecCompareFloat::
   ~LgFrMaximumElementTimeVecCompareFloat () {
   // Nothing to do here
}


// ---------------------------------------------------------------------------
//
// LgFrMinimumElementTimeVecCompareFloat methods.
//
// ---------------------------------------------------------------------------

// Do comparison
int
LgFrMinimumElementTimeVecCompareFloat::compare(
   const LgFrTimeVecFloat & tvf1,
   const LgFrTimeVecFloat & tvf2)
const
{
   float tvf1Min( tvf1.min() );
   float tvf2Min( tvf2.min() );

   if      ( tvf1Min < tvf2Min ) return -1;
   else if ( tvf1Min > tvf2Min ) return 1;
   else return 0;
}


// Make clone copy of object
LgFrTimeVecCompareFloat *
LgFrMinimumElementTimeVecCompareFloat::clone()
const
{
  LgFrMinimumElementTimeVecCompareFloat* newInst = 
      new LgFrMinimumElementTimeVecCompareFloat(*this);
  return (newInst);
}

// Return comparison strategy identifier
isAReturnType
LgFrMinimumElementTimeVecCompareFloat::isA()
const
{
   return id_;
}

// Assignment operator
LgFrTimeVecCompareFloat &
LgFrMinimumElementTimeVecCompareFloat::operator=(
	  const LgFrMinimumElementTimeVecCompareFloat& rhs)
{
  assert( id_ == rhs.id_ );
  if (this != &rhs) {		// Check for assignment to self
     // Nothing to do here
  }
  return *this;
}


// Copy constructor
LgFrMinimumElementTimeVecCompareFloat::
   LgFrMinimumElementTimeVecCompareFloat (
      const LgFrMinimumElementTimeVecCompareFloat & source)
:
   id_(__LGFRMAXIMUMELEMENTTIMEVECCOMPAREFLOAT)
{
  assert( id_ == source.id_ );
}


// Default Constructor
LgFrMinimumElementTimeVecCompareFloat::
   LgFrMinimumElementTimeVecCompareFloat ()
:  
   id_(__LGFRMAXIMUMELEMENTTIMEVECCOMPAREFLOAT)
{
  // Nothing to do here
}

// Destructor
LgFrMinimumElementTimeVecCompareFloat::
   ~LgFrMinimumElementTimeVecCompareFloat () {
   // Nothing to do here
}


// ---------------------------------------------------------------------------
//
// Test methods for the classes in this file.
//
// ---------------------------------------------------------------------------

#ifdef NDEBUG
#undef NDEBUG
#endif

// test LgFrMinimumElementTimeVecCompareFloat methods.
void
LgFrMinimumElementTimeVecCompareFloat::test()
{
  // Test default constructor
  LgFrMinimumElementTimeVecCompareFloat cs1;
  assert( cs1.id_ == __LGFRMAXIMUMELEMENTTIMEVECCOMPAREFLOAT );

  // Test isA method
  assert( cs1.isA() == __LGFRMAXIMUMELEMENTTIMEVECCOMPAREFLOAT );

  // Test copy constructor
  LgFrMinimumElementTimeVecCompareFloat cs2(cs1);
  assert( cs2.isA() == __LGFRMAXIMUMELEMENTTIMEVECCOMPAREFLOAT );

  // Test assignment operator
  LgFrMinimumElementTimeVecCompareFloat cs3;
  cs3 = cs2;
  assert( cs3.isA() == cs2.isA() );

  // Test clone method
  LgFrTimeVecCompareFloat *cs4Ptr = cs1.clone();
  assert( cs4Ptr->isA() == cs1.isA() );

  // Test destructor
  delete cs4Ptr;

  // Test timeVecCompareFloat
  // Test copy constructor
  float tv[]={0.f, 1.1f, 2.2f, 3.3f, 4.4f};
  LgFrTimeVecFloat tv5( 5, tv );
  LgFrTimeVecFloat tvCopy( tv5 );

  assert( cs1.compare( tvCopy, tv5 ) == 0 );
  tvCopy[2] = -40.f;
  assert( cs1.compare( tvCopy, tv5 ) < 0 );
  assert( cs1.compare( tv5, tvCopy ) > 0 );
  
}


// test LgFrTimeVecCompareFloat methods.
void
LgFrTimeVecCompareFloat::test()
{
  // Test default constructor
  LgFrTimeVecCompareFloat cs1;
  assert( cs1.id_ == __LGFRTIMEVECCOMPAREFLOAT );

  // Test isA method
  assert( cs1.isA() == __LGFRTIMEVECCOMPAREFLOAT );

  // Test copy constructor
  LgFrTimeVecCompareFloat cs2(cs1);
  assert( cs2.isA() == __LGFRTIMEVECCOMPAREFLOAT );

  // Test assignment operator
  LgFrTimeVecCompareFloat cs3;
  cs3 = cs2;
  assert( cs3.isA() == cs2.isA() );

  // Test clone method
  LgFrTimeVecCompareFloat *cs4Ptr = cs1.clone();
  assert( cs4Ptr->isA() == cs1.isA() );

  // Test destructor
  delete cs4Ptr;

  // Test timeVecCompareFloat
  // Test copy constructor
  float tv[]={0.f, 1.1f, 2.2f, 3.3f, 4.4f};
  LgFrTimeVecFloat tv5( 5, tv );
  LgFrTimeVecFloat tvCopy( tv5 );

  assert( cs1.compare( tvCopy, tv5 ) == 0 );
  tvCopy[2] = 40.;
  assert( cs1.compare( tvCopy, tv5 ) > 0 );
  assert( cs1.compare( tv5, tvCopy ) < 0 );
  
}

// test LgFrMaximumElementTimeVecCompareFloat methods.
void
LgFrMaximumElementTimeVecCompareFloat::test()
{
  // Test default constructor
  LgFrMaximumElementTimeVecCompareFloat cs1;
  assert( cs1.id_ == __LGFRMAXIMUMELEMENTTIMEVECCOMPAREFLOAT );

  // Test isA method
  assert( cs1.isA() == __LGFRMAXIMUMELEMENTTIMEVECCOMPAREFLOAT );

  // Test copy constructor
  LgFrMaximumElementTimeVecCompareFloat cs2(cs1);
  assert( cs2.isA() == __LGFRMAXIMUMELEMENTTIMEVECCOMPAREFLOAT );

  // Test assignment operator
  LgFrMaximumElementTimeVecCompareFloat cs3;
  cs3 = cs2;
  assert( cs3.isA() == cs2.isA() );

  // Test clone method
  LgFrTimeVecCompareFloat *cs4Ptr = cs1.clone();
  assert( cs4Ptr->isA() == cs1.isA() );

  // Test destructor
  delete cs4Ptr;

  // Test timeVecCompareFloat
  // Test copy constructor
  float tv[]={0.f, 1.1f, 2.2f, 3.3f, 4.4f};
  LgFrTimeVecFloat tv5( 5, tv );
  LgFrTimeVecFloat tvCopy( tv5 );

  assert( cs1.compare( tvCopy, tv5 ) == 0 );
  tvCopy[2] = 40.f;
  assert( cs1.compare( tvCopy, tv5 ) > 0 );
  assert( cs1.compare( tv5, tvCopy ) < 0 );
  
}
