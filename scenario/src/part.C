// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.
// File: LgFrPart.C
// Author: jmt
// Created: 3 December 1993 9:08:46 pm
// System: WITUSE
#include <iostream>

#include <scenario/src/part.h>
#include <scenario/src/machdep.h>

using std::cout;
using std::cerr;

//----------------------------------------------------------------------
//
//   Part
//
//----------------------------------------------------------------------

// Print out the part data
void 
LgFrPart::print() const
{
  cout << "\n part name  = " << name_;
  cout << "\n printing part name: " << printingName();
}

// Display for xcdb
void 
LgFrPart::xcdb() const
{
  cerr << "\n part name  = " << name_;
  cerr << "\n printing part name: " << printingName();
}


// Display for xcdb
void 
LgFrPart::xcdb()
{
  (  (const LgFrPart*) this  )     -> xcdb();
}


bool
LgFrPart::isEqual(
		   const LgFrItem* t
		 ) const
{
  assert (t->isAPart());
  return name_ == ((const LgFrPart*)t)->name_;
}

// compareTo method, uses part name to do the comparison.
// should return 0  if this->name_ "is equal to" source->name_.
//               >0 if this->name_ "is larger" than source->name_.
//               <0 if this->name_ "is smaller" than source->name_.
// C lib. function memcmp() is used to do the comparison.

int 
LgFrPart::compareTo(
		    const LgFrItem* source
		    ) const
{
  const LgFrPart* part = static_cast<const LgFrPart*>( source );
  return name_.compare( part->name_ );
}

// Default constructor
// Initialize part name to null string
LgFrPart::LgFrPart()
: name_()
{
  // All the work is done in the initialization part
}

// Constructor
// Create part with a string
LgFrPart::LgFrPart(const std::string& n)
: name_ (n)
{
  // All the work is done in the initialization part
}

// Copy constructor
LgFrPart::LgFrPart(const LgFrPart& source)
: name_ (source.name())
{
  // All the work is done in the initialization part
}

// Assignment
LgFrPart&
LgFrPart::operator=(const LgFrPart& rhs)
{
  // Don't have to call operator= for base class
  name_ = rhs.name();
  return (*this);
}

// Another Assignment
LgFrItem&
LgFrPart::operator=(const LgFrItem& rhs)
{
  assert( rhs.isAPart() );
  (this->LgFrPart::operator=) ( (const LgFrPart&)rhs );
  return (*this);
}


// Destructor
LgFrPart::~LgFrPart()
{
  // Nothing to do but reutrn.
}

// This function returns a printing name for the part.
std::string
LgFrPart::printingName(
		       bool trunc
		       ) const
{
  int pnchars = 18;
  std::string printingPartName = name_;
  if ( trunc ) printingPartName.resize(pnchars);
  else printingPartName.resize(size_tMax(printingPartName.length(),pnchars));
  return (printingPartName);
}

// This function is only to query the part name, not used to set it.
std::string
LgFrPart::uniqueName() const
{
  return (name_);
}

// This function is only to query the part name, not used to set it.
std::string
LgFrPart::name() const
{
  return (name_);
}

// This function is only to query the part name, not used to set it.
// For gui use
std::string
LgFrPart::guiName() const
{
  return (name_);
}


// Equality test
int
LgFrPart::operator== (const LgFrItem& rhs)
const
{
  return ( compareTo( &rhs ) == 0 );
}


#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrPart
void
LgFrPart::test()
{
  // Test constructors
  LgFrPart pDef; 
  LgFrPart pInit1 ("pInit1");
  LgFrPart pInit2 ("pInit2");

  assert( pDef.isAPart() && !pDef.isADemand() );
  // Test assignment
  pDef = pInit1;
  assert (pDef.name() == pInit1.name());
  
  // Test comparison
  assert (pDef   == pInit1);
  assert (! (pInit1 == pInit2));
  assert (! (pDef   == pInit2));

  // Test hashing, isEqual, compareTo
  assert (pDef.isEqual(&pInit1));
  assert (pDef.compareTo(&pInit1) == 0);
  assert (pInit1.compareTo(&pInit2) < 0);
/*
  // Test printingName method
  LgFrPart p1 ("IBMPS2mod95",3); 
  LgFrPart p2 ("IBMPS2mod90serial45conf78", 4);
  p1.print();
  p2.print();
*/
}
