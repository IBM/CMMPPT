// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/demdPerd.h>

// Author: wally

// Print out the demand data
void 
LgFrDemandPeriodPair::print() const
{
  std::cout << "\n************************************";
  std::cout << "\n Demand data for this LgFrDemandPeriodPair: ";
  this->demand_.print();
  std::cout << "\n period       = " << period_ << "\n";
}

// isEqual method, returns true if they have the same demand and period
bool
LgFrDemandPeriodPair::isEqual(
		   const LgFrDemandPeriodPair* t
		 ) const
{
  const LgFrDemandPeriodPair *rhs = (const LgFrDemandPeriodPair*) t;
  return (    this->demand_ == rhs->demand_
	  &&  this->period_ == rhs->period_);
}

// compareTo method, uses demand and period to do the comparison.
// should return 0  if *this "is equal to" *source.
//               >0 if *this "is larger" than *source.
//               <0 if *this "is smaller" than *source.
// C lib. function memcmp() is used to do the comparison.

int 
LgFrDemandPeriodPair::compareTo(
		    const LgFrDemandPeriodPair* source
		    ) const
{
  const LgFrDemandPeriodPair* sourceDPPPtr = 
    (const LgFrDemandPeriodPair*)source;
  int majorCompare 
    = (  (this->demand_) . compareTo(
				     &(sourceDPPPtr->demand_) )
       );
  if (majorCompare != 0)	// Can use major key
    return majorCompare;
  else    
    return (this->period_ - sourceDPPPtr->period_);
}

// This function is only used to query the demand, not used to set it.
const LgFrDemand&
LgFrDemandPeriodPair::demand() const
{
  return (demand_);
}


// This function is only used to set the demand
void
LgFrDemandPeriodPair::demand(const LgFrDemand& item)
{
  demand_ = item;
}

// This function is only used to query the period, not used to set it.
const int&
LgFrDemandPeriodPair::period() const
{
  return (period_);
}


// This function is only used to set the period
void
LgFrDemandPeriodPair::period(const int& p)
{
  period_ = p;
}


// Equality test
int
LgFrDemandPeriodPair::operator== (const LgFrDemandPeriodPair& rhs)
const
{
  return (this->isEqual (&rhs));
}

// Default constructor
LgFrDemandPeriodPair::LgFrDemandPeriodPair()
: demand_ (), period_ (0)
{
  // All the work is done in the initialization part
}

// Constructor
LgFrDemandPeriodPair::LgFrDemandPeriodPair(const LgFrDemand& item, 
					   const int& p)
: demand_ (item), period_ (p)
{
  // All the work is done in the initialization part
}

//Copy constructor
LgFrDemandPeriodPair::LgFrDemandPeriodPair (const LgFrDemandPeriodPair& source)
: demand_ (source.demand_), period_ (source.period_)
{
  // All the work is done in the initialization part
}

// Destructor
LgFrDemandPeriodPair::~LgFrDemandPeriodPair()
{
  // Nothing to do but reutrn.
}

// Assignment
LgFrDemandPeriodPair&
LgFrDemandPeriodPair::operator=(const LgFrDemandPeriodPair& rhs)
{
  if (this != &rhs) {
    demand_ = rhs.demand_;
    period_ = rhs.period_;
  }
  return (*this);
}



#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrDemandPeriodPair
void
LgFrDemandPeriodPair::test()
{
  std::string n1 = "demand1";
  const LgFrPart p1 ("part1");
  LgFrDemand d1(n1,&p1);
  std::string n2 = "demand2";
  const LgFrPart p2("part2");
  LgFrDemand d2(n2,&p2);

  // Test constructors
  LgFrDemandPeriodPair dDef; 
  LgFrDemandPeriodPair dInit1 (d1, 10);
  LgFrDemandPeriodPair dInit2 (d2, 10);
  LgFrDemandPeriodPair dInit3 (d2, 20);

  // Test assignment
  dDef = dInit1;
  assert (dDef.demand() == dInit1.demand()
	  &&  dDef.period() == dInit1.period()  );
  
  // Test comparison
  assert (dDef   == dInit1);
  assert (! (dInit1 == dInit2));
  assert (! (dDef   == dInit2));
  assert (! (dInit3 == dInit2));

  // Test hashing, isEqual, compareTo
  assert (dDef.isEqual(&dInit1));
  assert (dDef.compareTo(&dInit1) == 0);
  assert (dInit1.compareTo(&dInit2) < 0);
  assert (dInit2.compareTo(&dInit3) < 0);
}
