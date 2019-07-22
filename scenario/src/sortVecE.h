// SortedVectorEnhanced
#ifndef LgFrSortedVectorEnhanced_h
#define LgFrSortedVectorEnhanced_h

// This class has three primary responsibilities:
// 1. Fullfill all of the responsibilities of an RWSortedVector.
// 2. Return the index of the first item in the set that is greater than 
// or equal to any given item.
//
// This is a reference-based collection.  Insertions into it take the same
// amount of time that insertions into an RWSortedVector take.  The most
// efficient way to insert a lot of things into this set is to insert them
// in sorted order.  A good way to do this is (probably) to insert the 
// objects into a b-tree first and then insert them into this collection
// by traversing the b-tree

#include <rw/sortvec.h>

class LgFrSortedVectorEnhanced
: public   RWSortedVector		// Used to provide the interface
					// and most of the implementation
{
  // isA(), newSpecies(), copy(), and some other things are declared
  // by this macro
  RWDECLARE_COLLECTABLE(LgFrSortedVectorEnhanced)

public:

  // Returns the index of the first member of the collection that
  // is greater than or equal to the argument.  If there is no such element,
  // return the index of the greatest element.
  virtual
  size_t
  indexGreaterThanOrEqual(
			  const RWCollectable*)
  const; 


  // Note that this class defines neither it's destructor nor certain constructors.
  // This is not in keeping with our convention that all classes define them
  // so that we are using canonical form.  I decided that since this class
  // does not have any instance variables and probaly never will, I would
  // not do it this time. --- Wally

  // Default constructor and constructor that allows you to set size
  LgFrSortedVectorEnhanced( size_t size = RWCollection::DEFAULT_CAPACITY );


  // test this class
  static void test();
};

#endif
