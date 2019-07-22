// SortedVectorEnhanced

#include <assert.h>

#include <rw/collstr.h>

#include <scenario/src/idnumber.h>
#include <scenario/src/sortVecE.h>

// isA(), newSpecies(), copy(), and some other things are defined
// by this macro
RWDEFINE_COLLECTABLE(LgFrSortedVectorEnhanced, __LGFRSORTEDVECTORENHANCED)

// Use *a's comparison function to compare it to *b
static int
comparison(const void* a, const void* b)
{
  return (*(RWCollectable**)a)->compareTo(*(RWCollectable**)b);
}

// Returns the index of the first member of the collection that
// is greater than or equal to the argument.  If there is no such element,
// return the index of the greatest element.
size_t
LgFrSortedVectorEnhanced::indexGreaterThanOrEqual(const RWCollectable* target)
const
{
  size_t indexResult = (this->index(target));
  if (indexResult != RW_NPOS) {
    return indexResult;
  }
  else {
    size_t searchIndex=0;	// should be set by RWbsearch
    RWbsearch(
	      &target,
	      (RWCollectable**)vec.data(), // collection's data (inherited)
	      this->entries(), 
	      sizeof(RWCollectable*),      // size of each candidate
	      comparison,		   // comparison function (inherited)
	      searchIndex);		   // returned index
    assert ( searchIndex == 0   
	     ||   this->at(searchIndex-1)->compareTo( target ) < 0  );
    size_t loopIndex = searchIndex;
    while (loopIndex < this->entries()  ) {
      if (  this->at(loopIndex)->compareTo( target )  >  0  )
	return loopIndex;
      ++loopIndex;
    }
    return this->entries()-1;
  }
}


LgFrSortedVectorEnhanced::LgFrSortedVectorEnhanced(size_t size)
  :RWSortedVector(size)
{
}

 
// test this class
void
LgFrSortedVectorEnhanced::test() {
  // Make some things to insert
  RWCollectableString ab("ab");
  RWCollectableString abc("abc");
  RWCollectableString cde("cde");
  
  // Make an LgFrSortedVectorEnhanced and populate it
  LgFrSortedVectorEnhanced v(3);
  v.insert( &abc );
  v.insert( &cde );
  v.insert( &ab );

  // Test exact matches
  assert (v.indexGreaterThanOrEqual( &abc ) == 1);
  assert (v.indexGreaterThanOrEqual( &ab ) == 0);
  assert (v.indexGreaterThanOrEqual( &cde ) == 2);

  // Test target smaller than anything in collection
  RWCollectableString a("a");
  assert (v.indexGreaterThanOrEqual( &a ) == 0);

  // Test targets greater than something in collection
  RWCollectableString aba("aba");
  RWCollectableString abcd("abcd");
  assert (v.indexGreaterThanOrEqual( &aba ) == 1);
  assert (v.indexGreaterThanOrEqual( &abcd ) == 2);

  // Test targets greater than anything in collection
  RWCollectableString z("z");
  assert (v.indexGreaterThanOrEqual( &z ) == 2);
};
