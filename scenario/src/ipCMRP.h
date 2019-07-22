#ifndef LGFRINITIALPROBLEMFORCMRPAPP_H
#define LGFRINITIALPROBLEMFORCMRPAPP_H

// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.
// File:   ipCMRP.h
// Author: tom

// Initial problem for the Constrained MRP (CMRP) application. 

// There are two classes defined in this file.  They are:
//    LgFrInitialProblemForCMRP
//    LgFrInitialProblemForCMRPwithPrioritiesFromDif
//
//    LgFrInitialProblemForCMRP sets priorites based on the demand type
//    (committed, newOpportunity, firmForecast, or riskForecast).  The
//    committed demands are given a higher priority then all other demands.
//
//     LgFrInitialProblemForCMRPwithPrioritesFromDif sets the priorities
//     by obtaining them from the dif class.
//
//     LgFrInitialProblemForCMRPwithPrioritesFromDif inherits from
//     LgFrInitialProblemForCMRP.

//---------------------------------------------------------------------------
//
//  InitialProblemForCMRP and InitialProblemForCMRPWithPrioritiesFromDif
//
//  - Wit ObjChoice is set to 1.
//
//  - Wit Part attribute unitCost is set.  Wit uses this attribute with
//    objChoice 2.  Since objChoice is 1, unitCost is not used by when
//    doing an optImplode.  The unitCost is used by CMRP to compute the
//    financial impact.
//
//  InitialProblemForCMRP
//
//  - Demand priorites are: 10 for committed demands and
//    100 for other demands like forecast.;
//
//  - Objective function:
//      - obj1ShipReward=100 for all committed demands.
//        obj1ShipReward=10 for all other demands.
//      - obj1CumShipReward=.001*obj1ShipReward for all demands.
//
//  InitialProblemForCMRPWithPrioritiesFromDif
//
//  - Demand priorities are obtained from DIF.
//
//  - Objective function:
//      - obj1ShipReward is set with value obtained from dif.
//      - obj1CumShipReward=.001*obj1ShipReward for all demands.
//
//---------------------------------------------------------------------------

#include <scenario/src/initProb.h>
#include <scenario/src/scCMRPP.h>


// forward references
class LgFrDataInterface;
class LgFrDataInterfaceEssencials;

class LgFrInitialProblemForCMRP: public LgFrInitialProblemAndParameters {

public:
  
  // Populates the LgFrWitRunVeneer's data structures using the data 
  // from a dataInterface object.  It also populates any CMRP-specific
  // setOfParts.
virtual
void
feedInto(LgFrLastProblemSolved & lps);
  
  // Returns a description of the problem that is suitable for
  // use as a window heading. This can not be used to set the
  // title; only to query it.
virtual
std::string
title(const LgFrDataInterfaceEssentials* difEssenPtr = 0)
     const;

  // Make a clone copy of this object with new Scenario
virtual
LgFrInitialProblemAndParameters*
clone ( LgFrScenarioSmartPointer )
        const;

// the preferred constructor
LgFrInitialProblemForCMRP(LgFrScenarioSmartPointer);      
       
  // Destructor
~LgFrInitialProblemForCMRP();

  // Default Constructor
LgFrInitialProblemForCMRP();


  
  // Tests this class.  Only returns if all tests passed
static
void
test ();

  
// Context sensitive test
static
void
contextTest(  LgFrInitialProblemForCMRP          & ippTest,
            LgFrInitialProblemForCMRP          & ippPWcmrp
            );

protected:


// we're going to give out the actual pointer.  Be careful!!
virtual
LgFrScenarioForCMRPSmartPointer
CMRPscenario()
       const;

private:

virtual
void
setDemandPriorities(
  LgFrLastProblemSolved & lps);

virtual
void
setDemandObj1ShipReward(
  LgFrLastProblemSolved & lps);

};


class LgFrInitialProblemForCMRPwithPrioritiesFromDif: 
  public LgFrInitialProblemForCMRP {

public:

  // Make a clone copy of this object with new Scenario
virtual
LgFrInitialProblemAndParameters*
clone ( LgFrScenarioSmartPointer )
        const;

// the preferred constructor
LgFrInitialProblemForCMRPwithPrioritiesFromDif(LgFrScenarioSmartPointer);      
       
  // Destructor
~LgFrInitialProblemForCMRPwithPrioritiesFromDif();

  // Default Constructor
LgFrInitialProblemForCMRPwithPrioritiesFromDif();

  
  // Tests this class.  Only returns if all tests passed
static
void
test ();

  
// Context sensitive test
static
void
contextTest(LgFrInitialProblemForCMRPwithPrioritiesFromDif   & ippTest,
            LgFrDataInterface				   & testDif
            );


#if 0
protected:

virtual
LgFrScenarioForCMRPwithPrioritiesFromDif*
CMRPscenario()
       const;
#endif

private:

virtual
void
setDemandPriorities(
  LgFrLastProblemSolved & lps);

virtual
void
setDemandObj1ShipReward(
  LgFrLastProblemSolved & lps);

};
#endif
