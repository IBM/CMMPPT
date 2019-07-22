#ifndef LGFRDATAINTERFACE_ESSENTIALS_FROM_SCENARIO_H
#define LGFRDATAINTERFACE_ESSENTIALS_FROM_SCENARIO_H
// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.

#include <scenario/src/difEssen.h>

// forward declaration
class LgFrScenario;
class WitRun;

// This is a concrete class to serve as a bridge class between the
// initialProblem(InitProb) class and dataInterface(DIF) class 
// because some of the functions in the DIF class
// need informations (from scenario) which are not stored 
// (and can not be accessed) in the DIF class.
// These extra data need to be passed into DIF functions (whenever needed)
// when an InitProb object calls a DIF object to get data.
// The base class (LGFRDataInterfaceEssentials) defines the interface.
// All detail implementations will be defined in concrete subclasses
// and will be hidden from the user.
// An object of this class will be instantiated on the flight 
// (trantient object) and will be passed to those functions in a DIF 
// object when needed.
 
class LgFrDataInterfaceEssentialsFromScenario: public LgFrDataInterfaceEssentials
{
public:

  // Create a part and insert it to set_of_part
  virtual
    LgFrPart
      createAndInsertPart(const std::string& partName) const;

  // Create a demand and insert it to set_of_demand
  virtual
    LgFrDemand
      createAndInsertDemand(const std::string& demandName,
			    const std::string& partName) const;

  // Get a calander from the scenario
  virtual
    const LgFrCalendar& calendar() const;

  // Return a mutable pointer to LastSolvedProblem's WitRun.
  virtual
    WitRun*
      mutableWitRun();

  //--------------------------------------------------------------------
  //  
  //   Standard class methods
  //
  //-------------------------------------------------------------------

  // Make a clone copy of this object in same scenario
  virtual
    LgFrDataInterfaceEssentials *
        clone() const;

  // Make a clone copy of this object in new scenario
  virtual
    LgFrDataInterfaceEssentials *
        clone(const LgFrScenario*) const;

  // Copy constructor with same scenario
  LgFrDataInterfaceEssentialsFromScenario
    (const LgFrDataInterfaceEssentialsFromScenario& copy);

  // Copy constructor with new scenario
  LgFrDataInterfaceEssentialsFromScenario
    (const LgFrDataInterfaceEssentialsFromScenario& copy,
     const LgFrScenario* newScenPtr);


  // Default constructor
  LgFrDataInterfaceEssentialsFromScenario();

  // Constructor
  LgFrDataInterfaceEssentialsFromScenario(const LgFrScenario*);

  // Destructor
  virtual
    ~LgFrDataInterfaceEssentialsFromScenario();

private:
  // Assignment (not allowed at this time)
  LgFrDataInterfaceEssentialsFromScenario&
    operator=(const LgFrDataInterfaceEssentialsFromScenario& rhs);

private:
  const LgFrScenario* scenarioPtr_;

};
#endif

