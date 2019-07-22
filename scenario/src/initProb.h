#ifndef LGFRINITIALPROBLEMANDPARAMETERS_H
#define LGFRINITIALPROBLEMANDPARAMETERS_H

// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.
// File: LgFrInitialProblemAndParameters.h
// Author: wally
// Created: 17 November 1993 7:20:21 am
// System: WITUSE


#include <scenario/src/constant.h>
#include <scenario/src/lastPrbS.h>
#include <scenario/src/sOfParts.h>
#include <scenario/src/sOfDemds.h>
#include <scenario/src/scenP.h>

class LgFrDataInterface;
class LgFrDataInterfaceAbstract;
class LgFrDataInterfaceEssentials;


// Instances of this class derived from this classs
// are able to create the initial data that this
// scenario is based on.
// This is an abstract base class.
class LgFrInitialProblemAndParameters {

public:
  // Put the problem's data into the LgFrWitRunVeneer's data
  // structures.
  // This is pure virtual.
  virtual
    void
      feedInto(
               LgFrLastProblemSolved & lps)
	  = 0;

  // Returns a description of the problem that is suitable for
  // use as a window heading. This can not be used to set the
  // title; only to query it.
  virtual
    std::string
      title(const LgFrDataInterfaceEssentials * = 0)
	const
	  = 0;
  
  // Make clone copy of this object with new Scenario
virtual
LgFrInitialProblemAndParameters*
clone ( LgFrScenarioSmartPointer )
       const
	  = 0;
  

  // Destructor
virtual
~LgFrInitialProblemAndParameters();
  
  // default Constructor
LgFrInitialProblemAndParameters();

  // Constructor
LgFrInitialProblemAndParameters(LgFrScenarioSmartPointer scenarioPtr);
  
  // Copy Constructor
LgFrInitialProblemAndParameters(const LgFrInitialProblemAndParameters&);
  
  // assignment operator
LgFrInitialProblemAndParameters&
operator=(const LgFrInitialProblemAndParameters&);

protected:
  
  // return the scenarioPtr
  virtual
  LgFrScenarioSmartPointer
  scenario()
       const;
       

  // return the scenarios LgFrDataInterface
  virtual
  LgFrDataInterfaceAbstract *
  dataInterfacePtr()
       const;

  // Return a non-const setOfParts which belongs to this
  // LgFrInitialProblemAndParameters scenario
  virtual 
     LgFrSetOfParts&
        setOfParts(int k = LGFR_DEFAULT_SET);

  // Return a non-const setOfDemands which belongs to this
  // LgFrInitialProblemAndParameters scenario
  virtual
     LgFrSetOfDemands&
        setOfDemands(int k= LGFR_DEFAULT_SET);

private:
  LgFrScenarioSmartPointer scenarioPtr_;
};

#endif
