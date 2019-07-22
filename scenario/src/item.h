#ifndef LGFRITEM_H
#define LGFRITEM_H

#include <string>
#include <assert.h>

#include <scenario/src/idnumber.h>

#include <scenario/src/dll.h>

// This LgFrItem class is an abstract base class for LgFrDemand and LgFrPart.

class LGFR_DECLSPEC LgFrItem 
{ 
public:
    // build
    virtual LgFrItem* Clone() const = 0;

    // Get the name
    virtual std::string name() const = 0;

    // Get the name for gui use
    virtual std::string guiName() const = 0;

    // Get name in printable form
    virtual std::string printingName(bool trunc = true) const = 0;

    // Assignment
    virtual LgFrItem& operator=(const LgFrItem& rhs) = 0;

    // Comparison
    virtual int operator==(const LgFrItem&) const = 0;
    virtual int compareTo( const LgFrItem* ) const = 0;
    virtual bool isEqual( const LgFrItem* ) const = 0;

    // Print part data
    virtual void print() const = 0;

    // Polymorphism: Exactly one of these two must be defined 
    // in the derived class and return true
    virtual bool isAPart() const { return false; }
    virtual bool isADemand() const { return false; }

};

#endif
