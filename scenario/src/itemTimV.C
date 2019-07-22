// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.
#include <iostream>
#include <stdio.h>

#include <scenario/src/itemTimV.h>
#include <scenario/src/demand.h>

// RW2STL - implement (RWGVector, LgFrItemTimeVecPairFloat)

// RW2STL - implement (RWGOrderedVector, LgFrItemTimeVecPairFloat)


// Create and initialize emptyVec
const LgFrTimeVecFloat LgFrItemTimeVecPairFloat::emptyVec;

// Print out the timeVec data
void 
LgFrItemTimeVecPairFloat::print() const
{
  std::cout << "\n************************************";
  std::cout << "\n Item data for this LgFrItemTimeVecPairFloat: ";
  this->item_->print();
  std::cout << "\n timeVecFloat is\n";
  int i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<timeVecFloat_.length(); i++ )
     std::cout <<"   " <<timeVecFloat_[i];
  std::cout <<std::endl;
}

// Format an itemTimeVec for display.
// Returns a string representing the itemTimeVecFloat.
std::string 
LgFrItemTimeVecPairFloat::format(
   const LgFrTimeVecFloat & highLight,
   const bool truncate,
   const char highLightFlag,
   const int width,
   const int precision )//,
   //const long iosFlags )
const
{
   assert( item_ != 0 );
   return 
     item_->printingName(truncate) + 
     std::string(" ") + 
     timeVecFloat_.format( highLight, 
                           highLightFlag, 
                           width, 
                           precision );//, 
                           //iosFlags ); 
}

// isEqual method, returns true if they have the same part and timeVecFloat
bool
LgFrItemTimeVecPairFloat::isEqual(
		   const LgFrItemTimeVecPairFloat* t
		 ) const
{
  const LgFrItemTimeVecPairFloat *rhs = (const LgFrItemTimeVecPairFloat*) t;
  return ( this->compareTo(rhs) == 0 );
}

// compareTo method, uses part and TimeVecFloat to do the comparison.
// should return 0  if *this "is equal to" *source.
//               >0 if *this "is larger" than *source.
//               <0 if *this "is smaller" than *source.

int 
LgFrItemTimeVecPairFloat::compareTo(
		    const LgFrItemTimeVecPairFloat* source
		    ) const
{
  const LgFrItemTimeVecPairFloat* sourceITVFPPtr = 
    (const LgFrItemTimeVecPairFloat*)source;
  int majorCompare 
    = (  (this->item_) -> compareTo(
				     sourceITVFPPtr->item_ )
       );
  if (majorCompare != 0)	// Can use major key
    return majorCompare;
  else    
    return ( this->timeVecFloat_.compareTo( sourceITVFPPtr->timeVecFloat_ ) );
}

// This function is only used to query the item, not used to set it.
const LgFrItem&
LgFrItemTimeVecPairFloat::item() const
{
  return (*item_);
}


// This function is only used to query the part, not used to set it.
// If the item is really a part, get it, otherwise bomb out on assert.
const LgFrPart& 
LgFrItemTimeVecPairFloat::part () 
     const
{
  assert (item_->isAPart());
  return (* (LgFrPart*) item_);
}


// This function is only used to query the demand, not used to set it.
// If the item is really a demand, get it, otherwise bomb out on assert.
const LgFrDemand& 
LgFrItemTimeVecPairFloat::demand () 
     const
{
  assert (item_->isADemand() );
  return (* (LgFrDemand*) item_);
}


// This function is only used to set the part
void
LgFrItemTimeVecPairFloat::item(const LgFrItem& item)
{
  item_ = static_cast<LgFrItem*>( item.Clone() );
}

// This function is only used to query the timeVecFloat, not used to set it.
const LgFrTimeVecFloat&
LgFrItemTimeVecPairFloat::timeVecFloat() const
{
  return (timeVecFloat_);
}


// This function is only used to set the timeVecFloat
void
LgFrItemTimeVecPairFloat::timeVecFloat(const LgFrTimeVecFloat& tvf)
{
  timeVecFloat_ = tvf;
}


// Equality test
int
LgFrItemTimeVecPairFloat::operator== (
   const LgFrItemTimeVecPairFloat& rhs)
const
{
  return ( this->isEqual (&rhs) );
}

// Default constructor
LgFrItemTimeVecPairFloat::LgFrItemTimeVecPairFloat()
: item_ (0), timeVecFloat_()
{
  // Nothing to do but return
}

// Constructor
LgFrItemTimeVecPairFloat::LgFrItemTimeVecPairFloat(
   const LgFrItem& item, 
   const LgFrTimeVecFloat& tvf)
: item_ (0), timeVecFloat_ (tvf)
{
  item_ = static_cast<LgFrItem*>( item.Clone() );
}

//Copy constructor
LgFrItemTimeVecPairFloat::LgFrItemTimeVecPairFloat (
   const LgFrItemTimeVecPairFloat& source)
: item_ (0),  timeVecFloat_ (source.timeVecFloat_)
{
  if( source.item_ )
  {
    item_ = static_cast<LgFrItem*>( source.item_->Clone() );
  }
}

// Destructor
LgFrItemTimeVecPairFloat::~LgFrItemTimeVecPairFloat()
{
   delete item_;
}

// Assignment
LgFrItemTimeVecPairFloat&
LgFrItemTimeVecPairFloat::operator=(
   const LgFrItemTimeVecPairFloat& rhs)
{
  // Assertion should not be necessary, but caught a compiler bug 
  // in xlC.
  assert( &rhs != 0 );
  if (this != &rhs) {
     // If item_ was allocated then free it
     if ( item_ != 0 ) {
        delete item_;
        item_ = 0;
     }
     // If rhs item exists then allocate and assign
     if ( rhs.item_ != 0 ) {
        item_ = static_cast<LgFrItem*>( rhs.item_->Clone() );
        *item_  = *(rhs.item_);
     }
     // Assign timeVec
     timeVecFloat_ = rhs.timeVecFloat_;
  }
  return (*this);
}


#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrItemTimeVecPairFloat
void
LgFrItemTimeVecPairFloat::test()
{
  const LgFrPart p1 ("part1");
  const LgFrPart p2("part2");
  const LgFrTimeVecFloat timeVecFloat1(5,3.33f);
  const LgFrTimeVecFloat timeVecFloat2(5,5.55f);
  const LgFrDemand d1( "demand1", &p1 );
  const LgFrDemand d2( "demand2", &p1 );

  // Test constructors
  LgFrItemTimeVecPairFloat pDef; 
  LgFrItemTimeVecPairFloat pInit1 (p1, timeVecFloat1);
  LgFrItemTimeVecPairFloat pInit2 (p2, timeVecFloat1);
  LgFrItemTimeVecPairFloat pInit3 (p2, timeVecFloat2);
  LgFrItemTimeVecPairFloat dInit1 (d1, timeVecFloat2);
  LgFrItemTimeVecPairFloat dInit2 (d2, timeVecFloat1);

  // Test assignment
  pDef = pInit1;
  assert (pDef.item() == pInit1.item() );

  assert ( !(dInit1==dInit2) );
  dInit2 = dInit1;
  assert (dInit1.item() == dInit2.item() );
  LgFrTimeVecFloat tv1(  dInit1.timeVecFloat()); 
  LgFrTimeVecFloat tv2(  dInit2.timeVecFloat()); 
  assert ( tv1.length() == tv2.length() );
  int i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<tv1.length(); i++ ) assert( tv1[1]==tv2[i] );
  
  // Test comparison
  assert (dInit1 == dInit2 );
  // dInit1.item().print();
  // dInit2.item().print();
  assert (! (pInit1 == pInit2));
  assert (! (pDef   == pInit2));
  assert (! (pInit3 == pInit2));
  // assert (pInit3 == dInit1);   <-- This test correctly causes assert to
  //                                  fail in part::operator==

  // Test isEqual, compareTo
  assert (pDef.isEqual(&pInit1));
  assert (pDef.compareTo(&pInit1) == 0);
  assert (dInit1.isEqual(&dInit2));
  assert (dInit1.compareTo(&dInit2) == 0);
  assert (pInit1.compareTo(&pInit2) < 0);
  assert (pInit2.compareTo(&pInit3) < 0);

  // Test format method
  LgFrTimeVecFloat hlD(dInit1.timeVecFloat().length());
  LgFrTimeVecFloat hlP(pInit1.timeVecFloat().length());
  hlD[1] = hlP[2] = 1.;
  std::cout << dInit1.format() <<std::endl;
  std::cout << dInit1.format(hlD) <<std::endl;
  std::cout << pInit1.format() <<std::endl;
  std::cout << pInit1.format(hlP) <<std::endl;


}

