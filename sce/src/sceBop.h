// file: sceBop.h
// author: tom & robin

#ifndef  LGFRSCEBOP_H
#define  LGFRSCEBOP_H

// RW2STL - inserted:
#include <vector>
#include <string>
// End of RW2STL - inserted includes.

// #include <stdlib.h>
#include <wit/src/wit.h>
#include <assert.h>



class LgFrSceBop
{
public:
  // default constructor. Don't use.
  LgFrSceBop (); 

  // better constructor
  LgFrSceBop (float defaultPreference,
              int defaultEarly,
              int defaultLate,
              std::string & defaultOpName,
              int defaultBopIndex );

  // get "preference"
  const
  float
  preference () const ;
  
  // set "preference"
  void
  preference (const float pref);
  // get "early"
  const
  int
  early () const ;
  
  // set "early"
  void
  early (const int earliestPeriod);

  // get "late"
  const
  int
  late () const ;
  
  // set "late"
  void
  late (const int latestPeriod);

  // get "opName"
  const
  std::string&
  opName () const ;
  
  // set "opName"
  void
  opName (const std::string & operationName);

  // get "bopIndex"
  const
  int
  bopIndex () const ;
  
  // set "bopIndex"
  void
  bopIndex (const int bopIndexNumber);

  // split bop
  // Split the specified bop at the given date.
  // The specified bop becomes the early "half" and 
  // the later "half" is returned as a new bop.
  // The bop is split both in WIT and in the object.
  LgFrSceBop 
  splitBop ( WitRun * const theWitRun,
             int endingDateOfFirstHalf );
  
  // copy constructor
  LgFrSceBop(const LgFrSceBop& source);

  // assignment operator
  LgFrSceBop&
  operator=(const LgFrSceBop& rhs);

  // destructor
  virtual
  ~LgFrSceBop();

  // == operator
  // Note: two  bops are EQUAL if their bopIndex's are equal
  //      ????  (this is used by OrderedVector to find a "match"
  //      not sure we'll even use it
  int
  operator==(const LgFrSceBop& rhs);
  
  
private:
  float  preference_ ;
  int    early_ ;
  int    late_ ;
  std::string opName_ ;
  int    bopIndex_ ;
};

// Here's the declaration for the compare function
 int bopCompare(const LgFrSceBop * a, const LgFrSceBop * b);







// ============================
// SCE Part Set of Bops
// ============================

// This object is a simple wrapper around RWOrderedVector(LgFrSceBop)
// It is convenient because it'll make the client code MUCH cleaner.

// It only really provides two functions:
//  1) construction:  You pass in the witRun and the partName, and it
//     creates a sorted vector of sceBop objects for that part.
//  2) indexing operator.  This gives a reference to the i'th
//     LgFrSceBop item which you can modify, since its a reference.
//
//  Note: if you need more function, we can easily implement whatever
//  is available via RWOrderedVector(val).  Some of the key ones are ...
//  insertAt, and removeAt.  However, if I understand, the size of the
//  SetOfBops stays fixed with the original number of bops in Wit.
//  In other words, once its created, you never need to modify the
//  sorted vector itself -- only the objects within it.

// here's the necessary RW setups for a typical "generic.h" Class

// RW2STL - declare (RWGVector, LgFrSceBop)

// RW2STL - declare (RWGOrderedVector, LgFrSceBop)

typedef std::vector<LgFrSceBop> LgFrOrderedVectorSceBop; // RW2STL - typedef RWGOrderedVector(LgFrSceBop) LgFrOrderedVectorSceBop;


class LgFrScePartSetOfBops {
public:

// preferred  constructor
// This constructor automatically populates the setOfBops for
// the given partname and Puts them into Sorted Order.
LgFrScePartSetOfBops(WitRun * const theWitRun, std::string & partName);
  
// Here's how you get access to one of the Bops se as a reference!
// from here you can modify at your leisure via SplitBop or whatever
LgFrSceBop &
operator[](size_t i);

  
// copy constructor
LgFrScePartSetOfBops(const LgFrScePartSetOfBops& source);


// destructor
virtual
~LgFrScePartSetOfBops();

// default constructor -- DON'T USE
LgFrScePartSetOfBops();

// assignment operator
LgFrScePartSetOfBops&
operator=(const LgFrScePartSetOfBops& rhs);

// test method
// RLH: Ask about "static-ness" of the test
// and the use of "this" pointer  with static.
void test();

private:
  LgFrOrderedVectorSceBop    setOfBops_;
};


#endif
