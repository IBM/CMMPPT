// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.
#include <assert.h>
#include <iostream>


#include <scenario/src/sOfDemds.h>
#include <scenario/src/machdep.h>
#include <scenario/src/hashUtil.h>

typedef LgFrDemandVector LgFrDemandVector;


// Return a demand with name demandName for the specified part.
// If the demand is not in the SetOfDemands, then insert it.
LgFrDemand
LgFrSetOfDemands::newDemand( 
   const std::string& demandName,
   const LgFrPart * const demandedPart )
{
  const LgFrDemand * itemPtr;
  if (demandedPart == 0) {
      std::cerr << "Error in " << __FILE__ << " at line " << __LINE__ << '\n'
      << "Trying to create a demand with a 0 part-pointer.\n"
      << "The name of the demand being created was " << demandName << std::endl;
    abort();
  }
  itemPtr = this->findValue(demandName, demandedPart->name());
  if (itemPtr == 0) {
    LgFrDemand item( demandName, demandedPart );
    insert( item );
    itemPtr = & ( (*this) [(this->size()) - 1] );
  }
  return *itemPtr;
}

// Return true if demand is contained in set
bool
LgFrSetOfDemands::contains( const LgFrDemand demand )
const
{
  const std::string itemName (demand.name()
                                      + LgFrDemand::uniqueSeparator()
                                      + demand.partPtr()->name() );

  NameItemDictionary::const_iterator it = nameToItem_.find( itemName );
  return ( it != nameToItem_.end() );


  //return nameToItem_.contains( &itemName );
}


// Add new demand item to the end of the collection
void
LgFrSetOfDemands::insert (const LgFrDemand item)
{
  assert( !contains( item ) );
  if ( contains( item ) ) return;
  else {
    // If the vector is at full capacity, we know it will have to move
      // and the dictionary addresses will become stale.
    bool moved = capacity() == size(); 

    // Put it in the inherited ordered vector
    LgFrDemandVector::push_back(item);
    // Put it into the dictionary
    if( moved )
    {
        refreshDictionary();
    }
    else
    {
        int index = (this->size()) - 1;
        this->dictionaryInsert (  index  );
    }
  }
}

// Add new demand item to the collection at position index.  Everything afterwards
// is shifted to a higher index.
void
LgFrSetOfDemands::insertAt (const size_t index, 
			  const LgFrDemand item)
{
  assert( !contains( item ) );
  // Test for valid index
  assert (index < this->size());
  // Put it in the inherited ordered vector
  LgFrDemandVector::insert( begin() + index, item);

  refreshDictionary();
}


// Change the size of the set
void
LgFrSetOfDemands::reserve (const size_t newCapacity)
{
  const size_t nEntries (LgFrDemandVector::size());
  if (newCapacity > nEntries) 
  {
    // Change the size of the vector
    LgFrDemandVector::reserve(newCapacity);
    // Record the new capacity
    minBuckets_ = newCapacity;
    // The demands were copied so regenerate the dictionary to point
    // to their new locations
    refreshDictionary();
  }
}

LgFrSetOfDemands &
LgFrSetOfDemands::operator=(
			  const LgFrSetOfDemands& rhs)
{
  if (this != &rhs) 
  {		// Check for assignment to self
#if 0    
    (this->LgFrDemandVector::operator= ) (rhs);
#else
    // Try doing assignment w/o operator= because OrderDemandVector does not
    // support operator=.
    {
      // Clear the vector contents and change its size
      LgFrDemandVector::clear();
      LgFrDemandVector::reserve( rhs.size() );
      int j = 0; // Pulled out of the for below by RW2STL
      for (j = 0;  j < rhs.size();  ++j) {
        LgFrDemandVector::push_back( rhs[j] );
        // this->operator[](j) = rhs[j];
      }        
    }
#endif     

    size_t newSize = this->size();
    size_t newCapacity = rhs.minBuckets_;
    assert (newSize == rhs.size());      // Test assignment operator result

    refreshDictionary();
  }
  return *this;
}


// Copy constructor
LgFrSetOfDemands::LgFrSetOfDemands (const LgFrSetOfDemands & source)
: LgFrDemandVector ( (const LgFrDemandVector &)(source) ),
  minBuckets_ (source.minBuckets_)//,
  //nameToItem_ (minBuckets_)
{
  const size_t newSize = source.size();
  int j = 0; // Pulled out of the for below by RW2STL
  for (j = 0;  j < newSize;  ++j) {
    this->dictionaryInsert(j);
  }
}


// Constructor for a set expected to have up to n demands
LgFrSetOfDemands::LgFrSetOfDemands (const size_t n)
: //LgFrDemandVector (),	// Not all new's do well
						// with 0-length arrays
  minBuckets_ (goodSize (n))//,
  //nameToItem_ (minBuckets_)
{
  // Nothing to do in here
  reserve( n );
}

void LgFrSetOfDemands::refreshDictionary()
{
  clearDictionary();
  int j = 0; // Pulled out of the for below by RW2STL
  for (j = 0;  j < size();  ++j)
    this->dictionaryInsert(j);
}


void
LgFrSetOfDemands::dictionaryInsert (const size_t i)
{
  LgFrDemandPtr demandPtr = &at(i);
  nameToItem_.insert( NameItemPair( demandPtr->uniqueName(), demandPtr ) );

/*  for( NameItemDictionary::const_iterator it0 = nameToItem_.begin(); it0 != nameToItem_.end(); it0++ )
  {
      const LgFrDemand* p = (*it0).second;
        cout << "--" << (*it0).second->name() << std::endl;
  }*/

}


const LgFrDemand*
LgFrSetOfDemands::findValue (
			   const std::string& demandName,
			   const std::string& partName
			   )
     const
{
  const std::string itemName (demandName 
				      + LgFrDemand::uniqueSeparator()
				      + partName);

/*  for( NameItemDictionary::const_iterator it0 = nameToItem_.begin(); it0 != nameToItem_.end(); it0++ )
  {
      const LgFrDemand* p = (*it0).second;
        cout << "=-" << (*it0).second->name() << std::endl;
  }*/

  NameItemDictionary::const_iterator it = nameToItem_.find( itemName );

  if( it != nameToItem_.end() )
    return (*it).second;
  else
    return 0;

//  return ((LgFrDemand *) nameToItem_.findValue (&itemName));
}


// Display for xcdb
void 
LgFrSetOfDemands::xcdb() const
{
  std::cerr << "\n";
  std::cerr << "Set of demands:\n\tminBuckets_ = " << minBuckets_ << "\n";
  std::cerr << "\t\tsize() = " << this->size() << "\n";
  int i = 0; // Pulled out of the for below by RW2STL
  for (i = 0; i < this->size(); ++i)
    std::cerr << i << ": " << (*this)[i].printingName() << "\n";
  std::cerr << std::endl;
}


// Display for xcdb
void 
LgFrSetOfDemands::xcdb()
{
  (  (const LgFrSetOfDemands*) this  )     -> xcdb();
}


// Destructor
LgFrSetOfDemands::~LgFrSetOfDemands()
{
  clearDictionary();
}


// Remove the entries in the dictionary 
void
LgFrSetOfDemands::clearDictionary()
{
    nameToItem_.clear();
}


#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrSetOfDemands methods.
void
LgFrSetOfDemands::test()
{
  LgFrSetOfDemands sod(2);      // Make it a little small
  				// to test automatic resizing
  std::string n1 = "demand1";
  const LgFrPart p1 ("part1");
  LgFrDemand d1(n1,&p1);
  std::string n2 = "demand2";
  const LgFrPart p2("part2");
  LgFrDemand d2(n2,&p2);
  std::string n3 = "demand3";
  const LgFrPart p3("part3");
  LgFrDemand d3(n3,&p3);
  sod.insert(d1);
  sod.insert(d2);
  sod.insert(d3);
  assert (3 == sod.size());
  assert (sod[0].name() == n1);
  assert (sod[0].partPtr()->name() == "part1");
  assert (sod[2].name() == n3);
  assert (sod[2].partPtr()->name() == "part3");
  assert (sod[1].name() == n2);
  assert (sod[1].partPtr()->name() == "part2");

  // Test newDemand method
  LgFrDemand newDemand = sod.newDemand( "newDemand", &p2 );
  int sodEntries( sod.size() );
  assert( newDemand == sod[ sodEntries - 1 ] );
  sod.newDemand( "newDemand", &p2 );  // Test inserting a 2nd time
  sod.newDemand( n3, // Test inserting something that is not the
		 &p3); // last thing in the set a second time
  assert( sodEntries == sod.size() );
  assert( newDemand == sod[ sodEntries - 1 ] );

  // Test contains method
  assert( sod.contains( d3 ) );
  LgFrDemand oddDemand( "OddDemand", &p1 );
  assert( !sod.contains( oddDemand ) );

  // Print the OrderedVector part
  /*
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < sod.size(); i++) {
    sod[i]->print();
    cout << std::endl;
  }
  */

  // Test the dictionary part
  const char * n3char = "demand3";
  const LgFrDemand * foundDemand = sod.findValue (n3char, "part3");
  assert (*foundDemand  ==  d3);
}
