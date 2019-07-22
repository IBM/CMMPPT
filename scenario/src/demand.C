// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.
// File: LgFrDemand.C
// Author: jmt
// Created: 3 December 1993 9:09:27 pm
// System: WITUSE

#include <iostream>

#include <scenario/src/demand.h>
#include <scenario/src/machdep.h>

//----------------------------------------------------------------------
//
//   Demand
//
//----------------------------------------------------------------------

static const std::string guiSepChars (",\n  ");

// Print out the demand data
void 
LgFrDemand::print() const
{
  std::cout << "\n demand name  = " << name_
       << "\n part data for this demand: ";
  partPtr_ ->print();
  std::cout << "\n printing part/demand name: " << printingName();
}

// Display for xcdb
void 
LgFrDemand::xcdb() const
{
  std::cerr << "\n demand name  = " << name_;
  std::cerr << "\n printing part/demand name: " << printingName();
  partPtr_ ->xcdb();
}

// Display for xcdb
void 
LgFrDemand::xcdb()
{
  (  (const LgFrDemand*) this  )     -> xcdb();
}

// isEqual method, returns true if they have the same demand
// and part name
bool
LgFrDemand::isEqual(
		   const LgFrItem* t
		 ) const
{
  bool d = t->isADemand();
  assert (t->isADemand());
  return ( *this == * ( (const LgFrDemand*) t) ? true : false );
}

// compareTo method, uses demand name and part name to do the comparison.
// should return 0  if *this "is equal to" *source.
//               >0 if *this "is larger" than *source.
//               <0 if *this "is smaller" than *source.
// C lib. function memcmp() is used to do the comparison.

int 
LgFrDemand::compareTo(
		    const LgFrItem* source
		    ) const
{
  assert (source->isADemand());
  const LgFrDemand* sourceDemandPtr = (const LgFrDemand*)source;
  int majorCompare = name_.compare( sourceDemandPtr->name_ );
  if (majorCompare == 0)	// Have to use minor key
    return partPtr_->compareTo( sourceDemandPtr->partPtr()  );
  else    
    return majorCompare;
}

// Default constructor
// Initialize demand name to null string
// and part pointer to null.
LgFrDemand::LgFrDemand()
: name_ (), partPtr_ (0)
{
  // All the work is done in the initialization part
}

// Constructor
LgFrDemand::LgFrDemand(const std::string& n, const LgFrPart* p)
: name_ (n), partPtr_ (p)
{
  // All the work is done in the initialization part
}

//Copy constructor
LgFrDemand::LgFrDemand (const LgFrDemand& source)
: name_ (source.name()), partPtr_ (source.partPtr())
{
  // All the work is done in the initialization part
}

// Assignment
LgFrDemand&
LgFrDemand::operator=(const LgFrDemand& rhs)
{
  // Don't have to call operator= for base class
  name_ = rhs.name();
  partPtr_ = rhs.partPtr();
  return (*this);
}

// Another Assignment
LgFrItem&
LgFrDemand::operator=(const LgFrItem& rhs)
{
  assert( rhs.isADemand() );
  (this->LgFrDemand::operator=) ( (const LgFrDemand &)rhs );
  return (*this);
}


// Destructor
LgFrDemand::~LgFrDemand()
{
  // Nothing to do but reutrn.
}

// This function returns a printing name: part name-demand name
std::string
LgFrDemand::printingName(
			 bool trunc
			 ) const
{
  int dnchars = 12;
  std::string printingDemandName = name_;
  std::string printingPartName   = partPtr()->printingName(trunc);
  if ( trunc ) 
    printingDemandName.resize(dnchars);
  else 
    printingDemandName.resize(size_tMax(printingDemandName.length(),dnchars));
  return (printingPartName
	  + "-"
	  + printingDemandName
	  );
}

// This function is only to query the demand name, not used to set it.
std::string
LgFrDemand::uniqueName() const
{
  return (name_
	  + "\n"
	  + (partPtr()->name())
	  );
}

// This function is only to query the demand name, not used to set it.
std::string
LgFrDemand::name() const
{
  return (name_);
}

// This function is only to query the demand name, not used to set it.
// For gui use
std::string
LgFrDemand::guiName() const
{
  return ((partPtr()->name())
	  + guiSepChars
	  + name_
	  );
}

std::string
LgFrDemand::guiSeparator()
{
  return guiSepChars;
}

// This function is only used to query the part pointer, not used to set it.
const LgFrPart *
LgFrDemand::partPtr() const
{
  return (partPtr_);
}

// Equality test
int
LgFrDemand::operator== (const LgFrItem& rhs)
const
{
  return ( compareTo( &rhs ) == 0 );
}


#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrSetOfDemands methods.
void
LgFrDemand::test()
{
 // Test constructors 
  LgFrDemand dDef; 
  std::string n1 = "demand1";
  const LgFrPart p1 ("IBMPS2model9508ext2conf");
  LgFrDemand d1(n1,&p1);
  std::string n2 = "IBMNorthAmerican";
  const LgFrPart p2("IBMPS2mod85A");
  LgFrDemand d2(n2,&p2);
  std::string n3 = "IBMAsia";
  const LgFrPart p3("IBMRIOS6K");
  LgFrDemand d3(n3,&p3);
  std::string n4 = "IBMAsia";
  const LgFrPart p4("IBMRIOS6KX");
  LgFrDemand d4(n4,&p4);

  assert( dDef.isADemand() && !dDef.isAPart() );

  // Test assignment
  dDef = d1;
  assert (dDef.name() == d1.name());
  
  // Test comparison
  assert (dDef   == d1);
  assert (!(d1   == d2));
  assert (!(dDef == d2));
  assert (!(dDef == d3));
  assert (!(d2   == d3));
  assert (!(d1   == d3));
  assert (!(d3   == d4));

  // Test hashing, isEqual, compareTo
  assert (dDef.isEqual(&d1));
  assert (dDef.compareTo(&d1) == 0);
  assert (d2.compareTo(&d3) != 0);
  assert (d3.compareTo(&d4) != 0);

  // Test printingName method
  std::cout << "\nPrinting dDef:    \n" << dDef.printingName(); 
  std::cout << "\nPrinting demand1: \n" << d1.printingName(); 
  std::cout << "\nPrinting demand2: \n" << d2.printingName(); 
  std::cout << "\nPrinting demand3: \n" << d3.printingName();
  std::cout << "\nPrinting demand4: \n" << d4.printingName();

   // Test printingName method without truncation
  std::cout << "\nPrinting dDef:    \n" << dDef.printingName(false); 
  std::cout << "\nPrinting demand1: \n" << d1.printingName(false); 
  std::cout << "\nPrinting demand2: \n" << d2.printingName(false); 
  std::cout << "\nPrinting demand3: \n" << d3.printingName(false);
  std::cout << "\nPrinting demand4: \n" << d4.printingName(false);

  // Test uniqueName method
  std::cout << "\nUnique name for dDef:    \n" << dDef.uniqueName(); 
  std::cout << "\nUnique name for demand1: \n" << d1.uniqueName(); 
  std::cout << "\nUnique name for demand2: \n" << d2.uniqueName(); 
  std::cout << "\nUnique name for demand3: \n" << d3.uniqueName(); 
  std::cout << "\nUnique name for demand4: \n" << d4.uniqueName();
  
  // Test guiName method
  std::cout << "\nGui name for dDef:    \n" << dDef.guiName(); 
  std::cout << "\nGui name for demand1: \n" << d1.guiName(); 
  std::cout << "\nGui name for demand2: \n" << d2.guiName(); 
  std::cout << "\nGui name for demand3: \n" << d3.guiName(); 
  std::cout << "\nGui name for demand4: \n" << d4.guiName(); 

}
