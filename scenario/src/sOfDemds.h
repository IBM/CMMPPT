#ifndef LgFrSetOfDemands_h
#define LgFrSetOfDemands_h

// RW2STL -- inserted:
#include <map>
#include <string>
// End of RW2STL -- inserted includes.
// Set of Demands
// Authors: Wally and Jung-Mu

// This class has three primary responsibilities:
// 1. Hold a sequenced set of all of the demands
// 2. Provide a mapping from demand-name to demand.
// 3. Provide a mapping from demand-index to demand.
// Demands are inserted into this set by another object (LgFrScenario,
// generally); the other object is responsible for putting the demands
// into the OrderedVector so that they are at the right place.
// This is a value-based set.  (See Tools.h++ manual for definition of
// value-based.


#include <scenario/src/generVc.h>

class LgFrSetOfDemands
: public LgFrDemandVector	// Used to provide the interface
					// for the mapping from demand
  					// index to demand
{
public:

  // Create a new demand and insert it in the collection.
  LgFrDemand
    newDemand(
      const std::string& demandName,
        const LgFrPart * const demandedPart );

  // Returns true if demand is in set.
  bool
    contains( 
      const LgFrDemand )
        const; 

  // Add demand d to the end of the collection
  void
    insert (const LgFrDemand d);

  // Add demand d to the collection at position index.  Everything afterwards
  // is shifted to a higher index.
  void
    insertAt (const size_t index,
	      const LgFrDemand d);

  // Return a pointer to the demand whose name is demandName.
  // You can call this member function 
  // with a char* or a std::string.
  // Note: Having this return a LgFrPart* is consistent with Rogue Waves'
  const LgFrDemand*
    findValue (const std::string& demandName,
	       const std::string& partName
	       )
      const;

  // Increase the amount of storage allocated to the set
  void
    reserve (const size_t newCapacity);

  // Display part data for xcdb()
  void xcdb() const;
  void xcdb();

  // Destructor
  virtual
    ~LgFrSetOfDemands();

  // Assignment operator
  LgFrSetOfDemands &
    operator=(
	      const LgFrSetOfDemands& rhs);

  // Copy constructor
  LgFrSetOfDemands (const LgFrSetOfDemands &);

  // Constructor for a set expected to have up to n demands (and default constructor)
  LgFrSetOfDemands (const size_t n = DEFAULT_VECTOR_CAPACITY);

  // test this class
  static void test();

private:
  // Insert the demand at demandVec_[i] into the dictionary.
  void
    dictionaryInsert (const size_t index);

  // Refresh the dictionary
  virtual
    void
      refreshDictionary();

  // Remove the entries in the dictionary
  void
    clearDictionary();

  size_t minBuckets_;           // Minimum number of buckets in dictionary
                                // and actual capacity of this set.

  typedef std::pair<std::string,LgFrDemandPtr> NameItemPair;
  typedef std::map<std::string,LgFrDemandPtr> NameItemDictionary;
  NameItemDictionary nameToItem_;	// Used to implement the mapping from 
					// demand-name to demand.  The key is an 
					// std::string* and the value
					// is a "demand*".
};

#endif
