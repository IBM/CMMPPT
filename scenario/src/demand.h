#ifndef LGFRDEMAND_H
#define LGFRDEMAND_H
// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.
// File: LgFrDemand.h
// Author: jmt
// Created: 3 December 1993 9:09:27 pm
// System: WITUSE


#include <assert.h>


#include <scenario/src/item.h>
#include <scenario/src/idnumber.h>
#include <scenario/src/part.h>

// This class contains demand name and pointer to the part
// of this particular demand. 
class LGFR_DECLSPEC LgFrDemand : public LgFrItem  {

public:

        // isEqual method, returns true if they have the same name and
        // their parts' isEqual returns true  
        virtual bool isEqual(const LgFrItem*) const;

        // compareTo method, uses demand name to to the comparison.
	// should return 0  if *this "is equal to" *source
	//               >0 if *this "is larger" than *source
	//               <0 if *this "is smaller" than *source
        virtual int compareTo(const LgFrItem*) const;

	// Default constructor
	LgFrDemand();

	// Copy constructor
	LgFrDemand (const LgFrDemand&);

	// Constructor
	LgFrDemand(const std::string&, const LgFrPart* p);

	// Destructor
        virtual
	   ~LgFrDemand();

    virtual LgFrItem* Clone() const { return new LgFrDemand( *this ); }

    virtual bool isADemand() const { return true; }

	// Assignment 
        LgFrDemand& 
	operator=(const LgFrDemand&);

	// Assignment 
        LgFrItem&
	operator=(const LgFrItem&);

        // Get the string used to separate names in the unique string
        static const std::string uniqueSeparator();

        // Get a printing name for the demand and part
        // This function returns a printing name for the demand.
        // If the trunc parameter is set to true then part name will be
        // truncated/padded to pnchars and the demand name will be 
        // truncated/padded to dnchars.  Otherwise the full demand/part 
        // name will be used.
        // Defaults: trunc = true, 
        //           part name is truncated/padded to be 18 chars
        //           demand name is truncated/padded to be 12 chars
        std::string 
	  printingName(bool trunc = true) const;

        // Get a unique name for the demand
        std::string uniqueName() const;

        // Get the demand name
        std::string name() const;
        
        // Get the demand name for gui use
        std::string guiName() const;

        // Get the characters used to separate
        static std::string guiSeparator();

        // Get the part pointer
        const LgFrPart * partPtr() const;

	// Equality
	virtual
        int
	operator==(const LgFrItem&)
	const;

        // Print demand data
        virtual void print() const;

        // Display part data for xcdb()
        void xcdb() const;
        void xcdb();

        // test this class
        static void test();

private:
        std::string         name_;          // demand name
        const LgFrPart*   partPtr_;       // part pointer
};

inline
const
std::string
LgFrDemand::uniqueSeparator() 
{   return std::string("\n");   };

#endif
