// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.
#include <assert.h>
#include <iostream>


#include <scenario/src/sOfParts.h>
#include <scenario/src/machdep.h>
#include <scenario/src/hashUtil.h>

using std::cout;
using std::cerr;

// Return a part with name partName.
// If the part is not in the SetOfParts, then insert it.
LgFrPart
LgFrSetOfParts::newPart( const std::string& partName )
{
  const LgFrPart * itemPtr = 0;
  itemPtr = this->findValue(partName);
  if (itemPtr == 0) {
    LgFrPart item( partName );
    insert( item );
    itemPtr = &operator[]( size() - 1 );
  }
  return *itemPtr;
}

// Return true if part is contained in set
bool
LgFrSetOfParts::contains( const LgFrPart part )
const
{
  NameItemDictionary::const_iterator it = nameToItem_.find( part.name() );
  return ( it != nameToItem_.end() );
}


// Find part from partName
const LgFrPart*
LgFrSetOfParts::findValue (
			   const std::string& partName
			   )
     const
{
  NameItemDictionary::const_iterator it = nameToItem_.find( partName );
  if( it != nameToItem_.end() )
  {
      NameItemPair pr = *it;
    const LgFrPart * partPtr1 = &(*this)[0];
    return (*it).second;
  }
  else
    return 0;
    
//    const std::string itemName (partName);
//  const LgFrPart * retVal = ((LgFrPart *) nameToItem_.findValue (&itemName));
//  return retVal;
}


void
LgFrSetOfParts::dictionaryInsert (const size_t i)
{
  LgFrPart * partPtr = &(*this)[i];

  nameToItem_.insert( NameItemPair( partPtr->uniqueName(), partPtr ) );
  
}


//-----------------------------------------------------------------
//
// entries
//
//-----------------------------------------------------------------

// Number of parts currently in set
size_t
LgFrSetOfPartsWithMovableParts::size()
const
{
  return partVec_.size();
}

// Number of parts currently in set
size_t
LgFrSetOfPartsWithUnmovableParts::size()
const
{
  return partPtrVec_.size();
}
//-----------------------------------------------------------------
//
// operator[]
//
//-----------------------------------------------------------------

// Return the i'th part in the set
const LgFrPart &
LgFrSetOfParts::operator[]
  ( size_t i )
const
{
  // OK to cast away const'ness of this, since returning a const
  // reference
  LgFrSetOfParts* const nonConstThis = (LgFrSetOfParts* const) this;

  return (*nonConstThis)[ i ];
}

// Return the i'th part in the set
LgFrPart &
LgFrSetOfPartsWithMovableParts::operator[]
  ( size_t i )
{
  return partVec_[i];
}


// Return the i'th part in the set
LgFrPart &
LgFrSetOfPartsWithUnmovableParts::operator[]
  ( size_t i )
{
  return *( partPtrVec_[i] );
}


//-----------------------------------------------------------------
//
// insert, sequencedSetInsert
//
//-----------------------------------------------------------------


// Add new part item to the end of the collection
void
LgFrSetOfParts::insert (const LgFrPart item)
{
  assert( !contains( item ) );
  if ( contains( item ) ) return;
  else 
  {
    // Put it in the sequenced set
    bool setMoved = sequencedSetInsert(item);
    
    this->dictionaryInsert (  size() - 1  );
    
    if( setMoved )
        refreshDictionary();
   
  }
}

void LgFrSetOfPartsWithMovableParts::refreshDictionary()
{
  clearDictionary();
  int j = 0; // Pulled out of the for below by RW2STL
  for (j = 0;  j < size();  ++j)
    this->dictionaryInsert(j);
}


void LgFrSetOfPartsWithUnmovableParts::refreshDictionary()
{
    // Nothing to refresh here since the parts never move
}



// Add new part item to the end of the sequenced set
bool
LgFrSetOfPartsWithMovableParts::sequencedSetInsert (const LgFrPart item)
{
    bool moved = partVec_.capacity() == partVec_.size();
    partVec_.push_back(item);

    return moved;
}


// Add new part item to the end of the sequenced set
bool
LgFrSetOfPartsWithUnmovableParts::sequencedSetInsert (const LgFrPart item)
{
  LgFrPart * partPtr = new LgFrPart( item );

  partPtrVec_.push_back(partPtr);

  return false; // The addresses of the LgFrSetOfPartsWithUnmovableParts class never change by definition
}

//-----------------------------------------------------------------
//
// insertAt, sequencedSetInsertAt
//
//-----------------------------------------------------------------


// Add new part item to the collection at position index.
// Everything afterwards
// is shifted to a higher index.
void
LgFrSetOfParts::insertAt (const size_t index, 
			  const LgFrPart item)
{
  assert( !contains( item ) );
  // Test for valid index
  assert (index < size());
  // Put it in the sequenced set
  sequencedSetInsertAt(index, item);

  this->dictionaryInsert (  index  );
    
  refreshDictionary();
}

// Add new part item to the sequenced set at position index.
// Everything afterwards is shifted to a higher index.
bool
LgFrSetOfPartsWithMovableParts::sequencedSetInsertAt (const size_t index, const LgFrPart item)
{
  bool moved = partVec_.capacity() == partVec_.size();
  partVec_.insert(partVec_.begin()+index, item);
  // This will tell to regenerate the dictionary to point to the parts in their new positions
  return moved;
}

// Add new part item to the sequenced set at position index.
// Everything afterwards is shifted to a higher index.
bool
LgFrSetOfPartsWithUnmovableParts::sequencedSetInsertAt (const size_t index, 
			  const LgFrPart item)
{
  LgFrPart * partPtr = new LgFrPart( item );

  partPtrVec_.insert(partPtrVec_.begin()+index, partPtr);

  return true;
}

//-----------------------------------------------------------------
//
// resize, sequencedSetResize
//
//-----------------------------------------------------------------

// Change the size of the set

void
LgFrSetOfParts::reserve (const size_t newCapacity)
{
  const size_t nEntries ( size() );
  if (newCapacity > nEntries) {
    // Change the size of the vector
    this -> sequencedSetReserve(newCapacity);

    minBuckets_ = newCapacity;
  }
}

void
LgFrSetOfPartsWithMovableParts::reserve (const size_t newCapacity)
{
  const size_t nEntries ( size() );
  if (newCapacity > nEntries) {
    // Clear dictionary entries so resize will not be burdened with them
    this -> clearDictionary();
    // Perform base class resizing
    LgFrSetOfParts::reserve( newCapacity );
    // The parts were copied so regenerate the dictionary to point
    // to their new locations
    int j = 0; // Pulled out of the for below by RW2STL
    for (j = 0;  j < nEntries;  ++j)
      this->dictionaryInsert(j);
  }
}

void
LgFrSetOfPartsWithMovableParts::sequencedSetReserve (const size_t newCapacity)
{
  partVec_.reserve(newCapacity);
}

void
LgFrSetOfPartsWithUnmovableParts::sequencedSetReserve (const size_t newCapacity)
{
  partPtrVec_.reserve(newCapacity);
}

#if 0
//-----------------------------------------------------------------
//
// dictionaryInsert
//
//-----------------------------------------------------------------


void
LgFrSetOfPartsWithUnmovableParts::dictionaryInsert (const size_t i)
{
  LgFrPart * partPtr = partPtrVec_[i];
  
  std::string* namePtr =
    new std::string (partPtr->uniqueName());

    nameToItem_.insertKeyAndValue (namePtr, partPtr);

}
#endif
//-----------------------------------------------------------------
//
// operator=
//
//-----------------------------------------------------------------

LgFrSetOfParts &
LgFrSetOfParts::operator=(
			  const LgFrSetOfParts& rhs)
{
  if (this != &rhs) {		// Check for assignment to self  

    size_t newSize = this->size();
    size_t newCapacity = rhs.minBuckets_;
    assert (newSize == rhs.size());      // Test assignment operator result

    this -> clearDictionary();

    int j = 0; // Pulled out of the for below by RW2STL
    for (j = 0;  j < newSize;  ++j) {
      this->dictionaryInsert(j);
    }
  }
  return *this;
}

LgFrSetOfPartsWithMovableParts &
LgFrSetOfPartsWithMovableParts::operator=(
			  const LgFrSetOfPartsWithMovableParts& rhs)
{
  if (this != &rhs) {		// Check for assignment to self

    
    // Clear the vector contents and change its size
    partVec_.clear();
    partVec_.reserve( rhs.size() );
    const LgFrSetOfParts & rhsSop = rhs;
    int j = 0; // Pulled out of the for below by RW2STL
    for (j = 0;  j < rhs.size();  ++j) {
      partVec_.push_back( rhsSop[j] );
    }          

    // invoking base class operator= method must follow the setting
    // up of the sequenced set partVec_
    (this->LgFrSetOfParts::operator= ) (rhs);
  }
  return *this;
}


LgFrSetOfPartsWithUnmovableParts &
LgFrSetOfPartsWithUnmovableParts::operator=(
			  const LgFrSetOfPartsWithUnmovableParts& rhs)
{
  if (this != &rhs) {		// Check for assignment to self

    
    // Clear the vector contents and change its size
    int j = 0; // Pulled out of the for below by RW2STL
    for ( j=0; j < size(); ++j ) delete partPtrVec_[j];
    partPtrVec_.clear();
    partPtrVec_.reserve( rhs.size() );
    const LgFrSetOfParts & rhsSop = rhs;
    for (j = 0;  j < rhs.size();  ++j) {
      insert( rhsSop[j] );
    }          

    // invoking base class operator= method must follow the setting
    // up of the sequenced set partPtrVec_
    (this->LgFrSetOfParts::operator= ) (rhs);
  }
  return *this;
}

//-----------------------------------------------------------------
//
// Copy Constructor
//
//-----------------------------------------------------------------

// Copy constructor
LgFrSetOfParts::LgFrSetOfParts (
  const LgFrSetOfParts & source)
: 
  minBuckets_ (source.minBuckets_)//,
  //nameToItem_ (minBuckets_)
{
  // Nothing to do here 
}

// Copy constructor
LgFrSetOfPartsWithMovableParts::LgFrSetOfPartsWithMovableParts (
  const LgFrSetOfPartsWithMovableParts & source)
:
  LgFrSetOfParts( source ),
  partVec_    (source.partVec_)
  
{
  const size_t newSize = source.size();
  int j = 0; // Pulled out of the for below by RW2STL
  for (j = 0;  j < newSize;  ++j) {
    this->dictionaryInsert(j);
  }
  
}

// Copy constructor
LgFrSetOfPartsWithUnmovableParts::LgFrSetOfPartsWithUnmovableParts (
  const LgFrSetOfPartsWithUnmovableParts & source)
:
  LgFrSetOfParts( source ),
  partPtrVec_    (source.partPtrVec_)
  
{
  // partPtrVec points to parts which belong to source.
  // Make copies which belong to *this and update partPtrVec_
  // to point to the copies
  const size_t newSize = source.size();
  partPtrVec_.clear();
  const LgFrSetOfParts & sourceSop = source;
  int j = 0; // Pulled out of the for below by RW2STL
  for (j = 0;  j < newSize;  ++j) insert( sourceSop[j] );
  
}

//-----------------------------------------------------------------
//
// Constructor with specified initial size
//
//-----------------------------------------------------------------

// Constructor for a set expected to have up to n parts
LgFrSetOfParts::LgFrSetOfParts (const size_t n)
:
  minBuckets_ (goodSize (n))//,
  //nameToItem_ (minBuckets_)
{
  // Nothing to do in here
}

// Constructor for a set expected to have up to n parts
LgFrSetOfPartsWithMovableParts::LgFrSetOfPartsWithMovableParts (const size_t n)
:
  LgFrSetOfParts( n )//,
  //partVec_ (   goodSize (n)   ) 	        // Not all new's do well
						// with 0-length arrays
{
  // Nothing to do in here
}

// Constructor for a set expected to have up to n parts
LgFrSetOfPartsWithUnmovableParts::LgFrSetOfPartsWithUnmovableParts(
 const size_t n )
:
  LgFrSetOfParts( n )//,
  //partPtrVec_ (   goodSize (n)   ) 	        // Not all new's do well
						// with 0-length arrays
{
      partPtrVec_.reserve( n );
  // Nothing to do in here
}

//---------------------------------------------------------------------------
//
// Destructor
//
//---------------------------------------------------------------------------

// Destructor
LgFrSetOfParts::~LgFrSetOfParts()
{
  clearDictionary();
}

// Destructor
LgFrSetOfPartsWithMovableParts::~LgFrSetOfPartsWithMovableParts()
{
  // Nothing to do here

}

// Destructor
LgFrSetOfPartsWithUnmovableParts::~LgFrSetOfPartsWithUnmovableParts()
{
  size_t i = 0; // Pulled out of the for below by RW2STL
  for( i=0; i<size(); i++ ) delete partPtrVec_[i];
}

// Remove the entries in the dictionary and free the keys
void
LgFrSetOfParts::clearDictionary()
{
    nameToItem_.clear();
}

//-----------------------------------------------------------------
//
// Xcdb methods
//
//-----------------------------------------------------------------


// Display for xcdb
void 
LgFrSetOfParts::xcdb() const
{
  cerr << "\n";
  cerr << "Set of parts:\tminBuckets_ = " << minBuckets_ << "\n";
  cerr << "\t\tsize() = " << this->size() << "\n";
  int i = 0; // Pulled out of the for below by RW2STL
  for (i = 0; i < this->size(); ++i)
    cerr << i << ": " << (*this)[i].printingName() << "\n";
  cerr << std::endl;
}

// Display for xcdb
void 
LgFrSetOfParts::xcdb()
{
  (  (const LgFrSetOfParts*) this  )     -> xcdb();
}

// Display for xcdb
void 
LgFrSetOfPartsWithMovableParts::xcdb() const
{
  (  (const LgFrSetOfParts*) this  )     -> xcdb();
}

// Display for xcdb
void 
LgFrSetOfPartsWithMovableParts::xcdb()
{
  (  (const LgFrSetOfParts*) this  )     -> xcdb();
}

// Display for xcdb
void 
LgFrSetOfPartsWithUnmovableParts::xcdb() const
{
  (  (const LgFrSetOfParts*) this  )     -> xcdb();
}

// Display for xcdb
void 
LgFrSetOfPartsWithUnmovableParts::xcdb()
{
  (  (const LgFrSetOfParts*) this  )     -> xcdb();
}




#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrSetOfPartsWithMovableParts methods.
void
LgFrSetOfPartsWithMovableParts::test()
{
  // Make some parts for the test
  int partIndex = 0;
  const char* n3char = "part3";
  LgFrPart p1( "part1");
  LgFrPart p2( "part2");
  LgFrPart p3( "part3");
  LgFrPart p4( "part4");

  // Test the insertion methods
  LgFrSetOfPartsWithMovableParts sop (2);	// Make it a little small
  				// to test automatic resizing
  sop.insert(p2);
  const LgFrPart* pPart2 = sop.findValue ( "part2" );
  pPart2 = sop.findValue ( "part2" );

  sop.insert(p3);
  pPart2 = sop.findValue ( "part2" );
  sop.insertAt(0, p1);
  assert (3 == sop.size());
  assert (sop[0].name() == "part1");
  assert (sop[2].name() == "part3");
  assert (sop[1].name() == "part2");

  // Test the assignment operator and the copy constructor
  LgFrSetOfPartsWithMovableParts sop1 (1);	// Make it a little small
  				// to test resizing at assignment time
  sop.insert(p4);		// Give it something that should go away
  sop1 = sop;
  LgFrSetOfPartsWithMovableParts sop2 (sop);
  pPart2 = sop.findValue ( "part2" );

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < sop.size(); i++) {
    // Test result of assignment
    assert (sop1[i] == sop[i]);
    // Test result of copy
    assert (sop2[i] == sop[i]);
    // Print the contents of the OrderedVector part of sop
    /*
    sop[i]->print();
    cout << endl;
    */
  }

  assert (sop[0].name() == "part1");
  assert (sop[1].name() == "part2");
  assert (sop[2].name() == "part3");

  // Test newPart method
  LgFrPart newPart = sop.newPart( "newPart" );
  int sopEntries( sop.size() );
  assert( newPart == sop[ sopEntries - 1 ] );
  sop.newPart( "newPart" );  // Test inserting a 2nd time

  assert (sop[0].name() == "part1");
  assert (sop[1].name() == "part2");
  assert (sop[2].name() == "part3");

  pPart2 = sop.findValue ( "part2" );

  sop.newPart( "part2" ); // Test inserting something that is not the
	                     // last thing in the set a second time
  assert( sopEntries == sop.size() );
  assert( newPart == sop[ sopEntries - 1 ] );

  // Test contains method
  assert( sop.contains( p4 ) );
  LgFrPart oddPart( "OddPart");
  assert( !sop.contains( oddPart ) );

  // Test the dictionary part
  const LgFrPart * foundPart = sop.findValue (n3char);
  assert (*foundPart == p3);
  foundPart = sop1.findValue (n3char);
  assert (*foundPart == p3);

  // Test resize
  //LgFrSetOfPartsWithMovableParts sop3(0);
  //sop3.resize(3);
  //assert (sop3.size() == 0);
  
}


// test LgFrSetOfPartsWithUnmovableParts methods.
void
LgFrSetOfPartsWithUnmovableParts::test()
{
  // Make some parts for the test
  int partIndex = 0;
  const char* n3char = "part3";
  LgFrPart p1( "part1");
  LgFrPart p2( "part2");
  LgFrPart p3( "part3");
  LgFrPart p4( "part4");

  // Test the insertion methods
  {
  LgFrSetOfPartsWithUnmovableParts sop (2);	// Make it a little small
  				// to test automatic resizing
  sop.insert(p2);
  sop.insert(p3);
  sop.insertAt(0, p1);
  assert (3 == sop.size());
  assert (sop[0].name() == "part1");
  assert (sop[2].name() == "part3");
  assert (sop[1].name() == "part2");

  // Test the assignment operator and the copy constructor
  {
  LgFrSetOfPartsWithUnmovableParts sop1 (1);	// Make it a little small
  				// to test resizing at assignment time
  sop.insert(p4);		// Give it something that should go away
  sop1 = sop;
  {
  LgFrSetOfPartsWithUnmovableParts sop2 (sop);

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < sop.size(); i++) {
    // Test result of assignment
    assert (sop1[i] == sop[i]);
    // Test result of copy
    assert (sop2[i] == sop[i]);
    // Print the contents of the OrderedVector part of sop
    /*
    sop[i]->print();
    cout << endl;
    */
  }

  // Test newPart method
  LgFrPart newPart = sop.newPart( "newPart" );
  int sopEntries( sop.size() );
  assert( newPart == sop[ sopEntries - 1 ] );
  sop.newPart( "newPart" );  // Test inserting a 2nd time
  sop.newPart( "part2" // Test inserting something that is not the
	       ); // last thing in the set a second time
  assert( sopEntries == sop.size() );
  assert( newPart == sop[ sopEntries - 1 ] );

  // Test contains method
  assert( sop.contains( p4 ) );
  LgFrPart oddPart( "OddPart");
  assert( !sop.contains( oddPart ) );

  // Test the dictionary part
  const LgFrPart * foundPart = sop.findValue (n3char);
  assert (*foundPart == p3);
  foundPart = sop1.findValue (n3char);
  assert (*foundPart == p3);


  
  // Test resize
  {
  //LgFrSetOfPartsWithUnmovableParts sop3(0);
  //sop3.resize(3);
  //assert (sop3.size() == 0);

  
  // Test that parts do not move when resizing
  {
    const LgFrPart * const partPtr = &sop[1];
    assert( partPtr == sop.findValue("part2") );
    sop.reserve( sop.size() * 25 );
    assert( partPtr == &sop[1] );
    assert( partPtr == sop.findValue("part2") );
  }

    
  }  // sop3 destructor invoked
  }  // sop2 destructor invoked
  }  // sop1 destructor invoked
  }  // sop  destructor invoked
}
