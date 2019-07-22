#ifndef LgFrSetOfParts_h
#define LgFrSetOfParts_h

// RW2STL -- inserted:
#include <map>
#include <string>
// End of RW2STL -- inserted includes.
// Set of Parts
// Authors: Wally and Jung-Mu

// This class has three primary responsibilities:
// 1. Hold a sequenced set of all of the parts
// 2. Provide a mapping from part-name to part.
// 3. Provide a mapping from part-index to part.
// Parts are inserted into this set by another object (LgFrScenario,
// generally); the other object is responsible for putting the parts
// into the sequenced set so that they are at the right place.

// The class LgFrSetOfParts is an abstract base class for
// LgFrSetOfPartsWithMovableParts and LgFrSetOfPartsWithUnmovableParts.
  
// LgFrSetOfPartsWithMovableParts:
// This is a value-based set.  (See Tools.h++ manual for definition of
// value-based. It has better performance than LgFrSetOfPartsWithUnmovableParts
// so it should be used when possible.

// LgFrSetOfPartsWithUnmovableParts:
// This set is used for the universalSetOfParts in scenario.h.  The parts
// stored by this set are not moved in memory, so it is OK to maintain a
// pointer to a part obtained from this set.  Part pointers should be obtained
// from this set when creating a demand.



#include <scenario/src/generVc.h>


class  LgFrSetOfParts
{
public:

  // Create a new part and insert it in the collection.
  virtual
     LgFrPart
        newPart( 
           const std::string& );

  // Returns true if part is in set.
  virtual
     bool
       contains( 
         const LgFrPart )
           const; 
   
  // Add part p to the end of the collection
  virtual
    void
      insert (const LgFrPart p);

  // Add part p to the collection at position index.  Everything afterwards
  // is shifted to a higher index.
  virtual
    void
      insertAt (const size_t index,
                const LgFrPart p);

  // Return a pointer to the part whose name is partName.
  // You can call this member function
  // with a char* or a std::string.
  // Note: Having this return a LgFrPart* is consistent with Rogue Waves'
  virtual
    const LgFrPart*
      findValue (const std::string& partName)
        const;

  // Increase the amount of storage allocated to the set
  virtual
    void
      reserve (const size_t newCapacity);

  // Number of parts currently in set
  virtual
    size_t
      size()
        const = 0;

  // Return the i'th part in the set
  const LgFrPart & 
    operator[]
      ( size_t i )
        const;
  virtual
    LgFrPart &
      operator[]
        ( size_t i )
          = 0;

  // Display part data for xcdb()
  virtual void xcdb() const = 0;
  virtual void xcdb() = 0;

  // Destructor
  virtual
    ~LgFrSetOfParts();

    // Assignment operator
  LgFrSetOfParts &
    operator=(
	      const LgFrSetOfParts& rhs);

  // Copy constructor
  LgFrSetOfParts (const LgFrSetOfParts &);

  // Constructor for a set expected to have up to n parts
  // (and default constructor)
  LgFrSetOfParts (const size_t n = DEFAULT_VECTOR_CAPACITY);

protected:

  // Remove the entries in the dictionary and free the keys
  virtual
    void
      clearDictionary();

  // Refresh the dictionary
  virtual
    void
      refreshDictionary() = 0;

  // Insert the part at partVec_[i] into the dictionary.
  virtual
    void
      dictionaryInsert (const size_t index);

private:
  
  // Add part p to the end of the sequenced set. Returns true if the vector had to move in memory
  virtual
    bool
      sequencedSetInsert (const LgFrPart p) = 0;

  // Add part p to the sequenced set at position index.  Everything afterwards
  // is shifted to a higher index.
  virtual
    bool
      sequencedSetInsertAt (const size_t index,
	                    const LgFrPart p) = 0;

  // Resize the sequenced set
  virtual
    void
      sequencedSetReserve (const size_t newCapacity) = 0;
  
  size_t minBuckets_;           // Minimum number of buckets in dictionary
                                // and actual capacity of this set.

  typedef std::pair<std::string,LgFrPartPtr> NameItemPair;
  typedef std::map<std::string,LgFrPartPtr> NameItemDictionary;
  NameItemDictionary nameToItem_;	// Used to implement the mapping from 
					// part-name to part.  The key is an 
					// std::string* and the value
					// is a "part*"
  

};



class LgFrSetOfPartsWithMovableParts
:  public   LgFrSetOfParts
  
{
public:

  // Increase the amount of storage allocated to the set
  void
    reserve (const size_t newCapacity);

  // Number of parts currently in set
  virtual
    size_t
      size()
        const;
  
  // Return the i'th part in the set
  virtual
    LgFrPart &
      operator[]
        ( size_t i );
  
  // Display part data for xcdb()
  void xcdb() const;
  void xcdb();

  // Destructor
  virtual
    ~LgFrSetOfPartsWithMovableParts();

  // Assignment operator
  LgFrSetOfPartsWithMovableParts &
    operator=(
	      const LgFrSetOfPartsWithMovableParts& rhs);

  // Copy constructor
  LgFrSetOfPartsWithMovableParts (const LgFrSetOfPartsWithMovableParts &);

  // Constructor for a set expected to have up to n parts (and default constructor)
  LgFrSetOfPartsWithMovableParts (const size_t n = DEFAULT_VECTOR_CAPACITY);

  // test this class
  static void test();

 
private:

  // Refresh the dictionary
  virtual
    void
      refreshDictionary();

  // Add part p to the end of the collection
  bool
    sequencedSetInsert (const LgFrPart p);

  // Add part p to the collection at position index.  Everything afterwards
  // is shifted to a higher index.
  bool
    sequencedSetInsertAt (const size_t index,
	      const LgFrPart p);

  // Resize the sequenced set
  void
    sequencedSetReserve (const size_t newCapacity);
  
  LgFrPartVector	partVec_;       // Used to store parts in this set.
                                        // This is implements
                                        // the sequenced set of parts.

					
};


class LgFrSetOfPartsWithUnmovableParts
:  public   LgFrSetOfParts
  
{
public:

  // Number of parts currently in set
  virtual
    size_t
      size()
        const;
  
  // Return the i'th part in the set
  virtual
    LgFrPart &
      operator[]
        ( size_t i );
  
  // Display part data for xcdb()
  void xcdb() const;
  void xcdb();

  // Destructor
  virtual
    ~LgFrSetOfPartsWithUnmovableParts();

  // Assignment operator
  LgFrSetOfPartsWithUnmovableParts &
    operator=(
	      const LgFrSetOfPartsWithUnmovableParts& rhs);

  // Copy constructor
  LgFrSetOfPartsWithUnmovableParts (const LgFrSetOfPartsWithUnmovableParts &);

  // Constructor for a set expected to have up to n parts (and default constructor)
  LgFrSetOfPartsWithUnmovableParts (const size_t n = DEFAULT_VECTOR_CAPACITY);

  // test this class
  static void test();


private:

  // Refresh the dictionary
  virtual
    void
      refreshDictionary();

  // Add part p to the end of the collection
  bool
    sequencedSetInsert (const LgFrPart p);

  // Add part p to the collection at position index.  Everything afterwards
  // is shifted to a higher index.
  bool
    sequencedSetInsertAt (const size_t index,
	      const LgFrPart p);

  // Resize the sequenced set
  void
    sequencedSetReserve (const size_t newCapacity);
  
  LgFrPartPtrVector partPtrVec_;    // Used to store pointers to
                                           // unmoveable parts in this set.
                                           // This is implements
                                           // the sequenced set of parts.
					
};

#endif
