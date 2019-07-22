
#include <math.h>
#include <assert.h>

#include <scenario/src/collflt.h>
#include <scenario/src/idnumber.h>

float LgFrCollectableFloat::fltEps = .00001f;



bool
LgFrCollectableFloat::operator==(
  const LgFrCollectableFloat& c) 
const
{ 
  return isEqual(&c);
}

int
LgFrCollectableFloat::compareTo(
  const LgFrCollectableFloat* c) 
const
{ 
  return
    (
      ( fabs( value() - ((const LgFrCollectableFloat*)c)->value() ) < fltEps )
     ? 0
     : ( ( value() < ((const LgFrCollectableFloat*)c)->value() )
	? -1 
	: 1
	)
     );
}


bool
LgFrCollectableFloat::isEqual(
  const LgFrCollectableFloat* c) 
const
{ 
  return compareTo(c) == 0;
}


float
LgFrCollectableFloat::value()
const
{ 
  return value_;
}

float
LgFrCollectableFloat::value(
  float val)
{ 
  float temp = value_;
  value_ = val;
  return temp;
}


// Destructor
LgFrCollectableFloat::~LgFrCollectableFloat()
{
  // Nothing to do here
}

// Assignment operator.
LgFrCollectableFloat&
LgFrCollectableFloat::operator=(
  const LgFrCollectableFloat& rhs)
{
  if (this != &rhs) {           // Check for assignment to self
     
     value_ = rhs.value_;

  }
  return *this;
}

// Copy constructor
LgFrCollectableFloat::LgFrCollectableFloat(
  const LgFrCollectableFloat& source)
:
   value_(source.value_)
{
  // Nothing to do here
}


// Preferred Constructor
LgFrCollectableFloat::LgFrCollectableFloat(
   float val )
:
   value_( val )
{
   // Nothing to do here
}

// Default Constructor
LgFrCollectableFloat::LgFrCollectableFloat()
:
   value_(0)
{
   // Nothing to do here
}     

#ifdef NDEBUG
#undef NDEBUG
#endif
// Tests this class.  Only returns if all tests passed
void
LgFrCollectableFloat::test()
{

  LgFrCollectableFloat zero;
  LgFrCollectableFloat one(1.0);
  LgFrCollectableFloat two(2.0);
  LgFrCollectableFloat twoC(two);

  assert( two.compareTo(&twoC) == 0 );
  assert( one.compareTo(&two)  < 0 );
  assert( one.compareTo(&zero) > 0 );


  assert( two.isEqual(&twoC) );
  assert(!two.isEqual(&zero) );

  assert( two==twoC );

  assert( fabs( two.value() - 2. ) < fltEps );

}
