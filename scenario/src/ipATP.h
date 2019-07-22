// File:   ipATP.h
// Author: grace and tom

#ifndef LGFRINITIALPROBLEMFORATPAPP_H
#define LGFRINITIALPROBLEMFORATPAPP_H

// Initial problem for the Available to Promise(ATP) application.

#include <scenario/src/initProb.h>
#include <scenario/src/scenATP.h>

// forward references
class LgFrSetOfParts;
class LgFrSetOfDemands;
class LgFrDataInterface;

class LgFrInitialProblemForATP: public LgFrInitialProblemAndParameters {

public:
  
  // Populates the LgFrWitRunVeneer's data structures using the data 
  // from a dataInterface object.  It also populates any ATP-specific
  // setOfDemands.
virtual
void
feedInto(LgFrLastProblemSolved & lps);

  // Add a new NewOpp demand into the system.  
virtual  
RWBoolean
addNewNewOppDemand(LgFrLastProblemSolved & lps,
                   const RWCString & demandName,
                   const LgFrPart & part );
  
  // Returns a description of the problem that is suitable for
  // use as a window heading. This can not be used to set the
  // title; only to query it.
virtual
RWCString
title(const LgFrDataInterfaceEssentials* difEssenPtr = NULL)
       const;

  // Make a clone copy of this object with new Scenario
virtual
LgFrInitialProblemAndParameters*
clone ( LgFrScenarioSmartPointer )
        const;

// the preferred constructor
LgFrInitialProblemForATP(LgFrScenarioSmartPointer);      
       
  // Destructor
~LgFrInitialProblemForATP();

  // Assignment operator.
LgFrInitialProblemForATP&
operator=( const LgFrInitialProblemForATP& rhs );


  // Copy constructor
LgFrInitialProblemForATP( const LgFrInitialProblemForATP& source );
  
  // Default Constructor
LgFrInitialProblemForATP();


  
  // Tests this class.  Only returns if all tests passed
static
void
test ();

  
// Context sensitive test:
// The first parameter must be an IPP that belongs to an ATP scenario that
// was created using a DIF that used registerDifTestImpFunctions.
// The second parameter must be an IPP that belongs to and ATP scenario that
// was created using a DIF taht used registerPWatpDifImpFunctions
static
void
contextTest(LgFrInitialProblemForATP     & ippTest,
            LgFrInitialProblemForATP  & ippPWatp);

protected:

virtual
LgFrScenarioForATPSmartPointer
ATPscenario()
       const;

private:

  int newOppPriority_;

};

#endif
