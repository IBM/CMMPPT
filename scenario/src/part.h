#ifndef LGFRPART_H
#define LGFRPART_H

// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.
// File: LgFrPart.h
// Author: jmt
// Created: 3 December 1993 9:08:46 pm
// System: WITUSE

#include <assert.h>


#include <scenario/src/item.h>
//#include <scenario/src/idnumber.h>

// Typedef used to identify a part category
typedef enum
  {
  LgFrRAW,
  LgFrCAPACITY,
  LgFrPRODUCT
  }
LgFrPartCategory;

// This part class contains part name/number
class LGFR_DECLSPEC LgFrPart : public LgFrItem {

public:

        // isEqual method, returns true if they have the same name.
        virtual bool isEqual(const LgFrItem*) const;

        // compareTo method, uses part name to to the comparison.
	// should return 0  if part1->name_ "is equal to" part2->name_.
	//               >0 if part1->name_ "is larger" than part2->name_.
	//               <0 if part1->name_ "is smaller" than part2->name_.

        virtual int compareTo(const LgFrItem*) const;

	// Default constructor
	LgFrPart();

	// Copy constructor
	LgFrPart(const LgFrPart&);

	// Constructor
	LgFrPart(const std::string&);

	// Destructor
        virtual
	   ~LgFrPart();

	// Assignment 
        LgFrPart&
	operator=(const LgFrPart&);

	// Assignment 
        LgFrItem&
	operator=(const LgFrItem&);

        // Get a printing name for part
        // This function returns a printing name for the part.
        // If the trunc parameter is set to true then part name will be
        // truncated/padded to pnchars. Otherwise the full part 
        // name will be used.
        // Defaults: trunc = true, 
        //           part name is truncated/padded to be 18 chars
        std::string 
	  printingName(bool trunc = true) const;

        virtual LgFrItem* Clone() const { return new LgFrPart( *this ); }

        virtual bool isAPart() const { return true; }


        // Get a unique name for the part
        std::string uniqueName() const;

        // Get the part name
        std::string name() const;
        
        // Get the part name for gui use
        std::string guiName() const;

	// Equality
	virtual
        int
	operator==(const LgFrItem&)
	const;

        // Print part data
        virtual void print() const;

        // Display part data for xcdb()
        void xcdb() const;
        void xcdb();

	// test this class
	static void test();

private:
        std::string name_;    // part name
};
#endif
