#ifndef LGFR_ITEM_TIME_VEC_PAIR_H
#define LGFR_ITEM_TIME_VEC_PAIR_H
// RW2STL -- inserted:
#include <string>
#include <vector>
// End of RW2STL -- inserted includes.
#include <ostream>


#include <scenario/src/timeVecs.h>

// This class represents a pair of Items and TimeVecFloat.
// The intended items are Part and Demand.

class LGFR_DECLSPEC LgFrItemTimeVecPairFloat { 

private:
  
  static const LgFrTimeVecFloat emptyVec; // Used for default arg because SUN
                                          // compiler fails when passed
                                          // LgFrTimeVecFloat()


public:

  // Returns true if compareTo returns 0.
  virtual
    bool
      isEqual(const LgFrItemTimeVecPairFloat*) 
	const;

  // compareTo method, uses ItemTimeVecPairFloat item->name and timeVecFloat
  // to do the comparison.
  // should return 0  if this->name_ "is equal to" source->name_ and
  //                     this->timeVecFloat "is equal" to source->timeVecFloat
  //               >0 if this->name_ "is larger" than source->name_.
  //               <0 if this->name_ "is smaller" than source->name_.
  //    
  //               If the name's are equal then the timeVecFloats are compared. 
  //               >0 if this->timeVecFloat "is larger" than
  //                     source->timeVecFloat.
  //               <0 if this->timeVecFloat "is smaller" than
  //                     source->timeVecFloat.
  
  virtual int compareTo(const LgFrItemTimeVecPairFloat*) const;

  // Set and Get methods
  // Item
  void  item (const LgFrItem&);	                // Set
  const LgFrItem& item () const;	        // Get
  const LgFrPart& part () const;		// If the item is really
						// a part, get it, otherwise
						// bomb out on assert
  const LgFrDemand& demand () const;		// If the item is really
						// a demand, get it, otherwise
						// bomb out on assert

  // timeVecFloat
  virtual  void timeVecFloat (const LgFrTimeVecFloat&);    // Set
  virtual  const LgFrTimeVecFloat& timeVecFloat () const;  // Get
  
  // Constructor that sets both parts
  LgFrItemTimeVecPairFloat(const LgFrItem& item, const LgFrTimeVecFloat& p);

  // default constructor
  LgFrItemTimeVecPairFloat();
  
  // Copy constructor
  LgFrItemTimeVecPairFloat (const LgFrItemTimeVecPairFloat& source);
  
  // destructor
  ~LgFrItemTimeVecPairFloat();
  
  // Assignment
  virtual
    LgFrItemTimeVecPairFloat& 
      operator=(const LgFrItemTimeVecPairFloat& rhs);
  
  // Equality
  virtual
    int
      operator==(const LgFrItemTimeVecPairFloat&)
	const;
  
   // Format an itemTimeVec for display.
   // Returns a string representing the itemTimeVecFloat.
   //
   // highLight:
   //   If highLight[i] != 0.0 then the i'th element of the timeVec
   //   is emphasized by following the number with an "*",
   //   or a client specified character.
   //
   // truncate: true indicates that item names all have the same length
   //           and are truncated if they can not fit in the alloted
   //           width.
   //
   // highLightFlag: character to follow a highlighted timeVec entry.
   //
   // width: ios width used to format each timeVec entry.
   //
   // precision: ios precision used to format each timeVec entry.
   //
   // iosFlags: ios formating flags used to format each timeVec entry.
   //
   std::string format(
      const LgFrTimeVecFloat & highLight = LgFrItemTimeVecPairFloat::emptyVec,
      const bool truncate = true,
      const char highLightFlag = '*',
      const int width = 10,
      const int precision = 1)//,
      //const long iosFlags = ios::fixed | ios::showpoint | ios::right )
   const;

  // Print Class
  virtual void print() const;
  
  // test this class
  static void test();

private:

  LgFrItem*          item_;
  LgFrTimeVecFloat   timeVecFloat_;
};


// RW2STL - declare (RWGVector,LgFrItemTimeVecPairFloat);

// RW2STL - declare (RWGOrderedVector,LgFrItemTimeVecPairFloat);

typedef std::vector<LgFrItemTimeVecPairFloat> LgFrItemTimeVecPairFloatVector; // RW2STL - typedef RWGOrderedVector(LgFrItemTimeVecPairFloat) LgFrItemTimeVecPairFloatVector;


#endif
