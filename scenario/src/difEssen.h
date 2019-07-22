#ifndef LGFRDATAINTERFACEESSENTIALS_H
#define LGFRDATAINTERFACEESSENTIALS_H
// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.

#include <scenario/src/calendar.h>
#include <scenario/src/part.h>
#include <scenario/src/demand.h>

// This is the virtual base class to be used as a bridge between 
// the initialProblem(InitProb) class and dataInterface(DIF) class 
// because some of the functions in the DIF class
// need informations (from other classes) 
// which are not stored (and can not be accessed) in the DIF class.
// These extra data need to be passed into DIF functions (whenever needed)
// when an InitProb object calls a DIF object to get data.
// This base class only defines the interface.
// All detail implementations will be defined in concrete subclasses
// and will be hidden from the user.
// An object of a concrete subclass derived from this base class
// will be instantiated on the flight (trantient object) 
// and will be passed to those functions in a DIF object when needed.
 
class LgFrDataInterfaceEssentials
{
public:

  // Create and insert a part
  virtual
    LgFrPart
      createAndInsertPart(const std::string& partName) const = 0;

  // Create and insert a demand
  virtual
    LgFrDemand
      createAndInsertDemand(const std::string& demandName, 
			    const std::string& partName) const = 0;

  // Get a calander
  virtual
    const LgFrCalendar& calendar() const = 0;

  //--------------------------------------------------------------------
  //  
  //   Standard class methods
  //
  //-------------------------------------------------------------------

  // Destructor
  virtual
    ~LgFrDataInterfaceEssentials();

};
#endif

