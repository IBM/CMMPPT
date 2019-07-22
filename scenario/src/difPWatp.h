#ifndef LgFrDataInterfaceImplementationPerfectWorldATP_h
#define LgFrDataInterfaceImplementationPerfectWorldATP_h


// This file contains the global functions for: 
//    LgFrDataInterfaceImplementationPerfectWorldATP

#include <wit/src/wit.h>

#include <scenario/src/idnumber.h>
#include <scenario/src/error.h>
#include <scenario/src/generVc.h>
#include <scenario/src/scenario.h>

//-----------------------------------------------------------------------------
//
// Forward references
//
//-----------------------------------------------------------------------------
class LgFrDataInterfaceImplementation;
class LgFrDataInterfaceEssentials;           
// class LgFrScenario;

//---------------------------------------------------------------------
//
// To create a LgFrDataInterfaceImplementation object for Perfect_World ATP
//
//---------------------------------------------------------------------

LgFrDataInterfaceImplementation * createPWatpDifImp();

//---------------------------------------------------------------------
//
// To register all global functions 
//
//---------------------------------------------------------------------

void
registerPWatpDifImpFunctions(LgFrDataInterfaceImplementation *);


//-------------------------------------------------------------------
//  
//   Global functions to obtain the defined parts and capacities.
//
//-------------------------------------------------------------------

// difPWatpGetRawPart global function (registering in the attribute table).
// Get raw parts for the Perfect_World ATP and insert them into set_of_part
// Return: a LgFrOrderedVectorPart object (vector of raw parts).

void *
difPWatpGetRawParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrOrderedVectorItemPtr*,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetRawPart global function (registering in the attribute table).
// Get raw parts for the Perfect_World ATP and insert them into set_of_part
// Return: a LgFrOrderedVectorPart object (vector of product parts).

void*
difPWatpGetProductParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrOrderedVectorItemPtr* ,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetRawPart global function (registering in the attribute table).
// Get raw parts for the Perfect_World ATP and insert them into set_of_part
// Return: a LgFrOrderedVectorPart object (vector of product parts).

void*
difPWatpGetCapacityParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrOrderedVectorItemPtr* ,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = NULL); 






//-----------------------------------------------------------------------------
//  
//  Global functions to obtain raw material attributes
//
//-----------------------------------------------------------------------------



// difPWatpGetRawUnitCost global function (registered in the attribute table)
// it returns a LgFrVectorFloat object.

void *
difPWatpGetRawUnitCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetRawProcCost global function (registered in the attribute table).
// It returns a LgFrVectorFloat object.

void *
difPWatpGetRawProcCost(
    const LgFrDataInterfaceImplementation * difImp,
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetRawConstrained global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difPWatpGetRawConstrained(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetRawProcLeadTime global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difPWatpGetRawProcLeadTime(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetRawObj1ScrapCost global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difPWatpGetRawObj1ScrapCost(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetRawObj1StockCost global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difPWatpGetRawObj1StockCost(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetRawSupplyVol global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difPWatpGetRawSupplyVol(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

//-----------------------------------------------------------------------------
//  
//  Global functions to obtain producible part attributes
//
//-----------------------------------------------------------------------------

// difPWatpGetProductUnitCost global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difPWatpGetProductUnitCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetProductProcCost global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difPWatpGetProductProcCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetProductConstrained global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difPWatpGetProductConstrained(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetProducYieldt global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difPWatpGetProductYield(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetProductProcLeadTime global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difPWatpGetProductProcLeadTime(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetProductCycleTime global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difPWatpGetProductCycleTime(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetObj1ProdCost global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difPWatpGetObj1ProdCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetProductObj1ScrapCost global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difPWatpGetProductObj1ScrapCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetProductObj1StockCost global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difPWatpGetProductObj1StockCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetProductSupplyVol global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difPWatpGetProductSupplyVol(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

//-----------------------------------------------------------------------------
//  
//  Global functions to obtain capacity attributes
//
//-----------------------------------------------------------------------------

// difPWatpGetCapacityUnitCost global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difPWatpGetCapacityUnitCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetCapacityConstrained global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difPWatpGetCapacityConstrained(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetCapacityObj1ScrapCost global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difPWatpGetCapacityObj1ScrapCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetCapacitySupplyVol global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difPWatpGetCapacitySupplyVol(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);  


//-----------------------------------------------------------------------------
//  
//  Global functions  to obtain demands
//
//-----------------------------------------------------------------------------


// difPWatpGetCommittedDemands global function (registering in the attribute 
// table).  Get committed demands for the Perfect_World ATP and insert
// them into set_of_demand.  Return: a LgFrOrderedVectorDemand object (vector 
// of committed demands).

void *
difPWatpGetCommittedDemands(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetNewOpportunityDemands global function (registering in the attribute 
// table).  Get committed demands for the Perfect_World ATP and insert
// them into set_of_demand.  Return: a LgFrOrderedVectorDemand object (vector 
// of committed demands).

void *
difPWatpGetNewOpportunityDemands(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetFirmForecastDemands global function (registering in the attribute 
// table).  Get committed demands for the Perfect_World ATP and insert
// them into set_of_demand.  Return: a LgFrOrderedVectorDemand object (vector 
// of committed demands).

void *
difPWatpGetFirmForecastDemands(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetRiskForecastDemands global function (registering in the attribute 
// table).  Get committed demands for the Perfect_World ATP and insert
// them into set_of_demand.  Return: a LgFrOrderedVectorDemand object (vector 
// of committed demands).

void *
difPWatpGetRiskForecastDemands(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);


//-----------------------------------------------------------------------------
//  
//  Global functions to obtain demand attributes
//
//-----------------------------------------------------------------------------

// difPWatpGetDemandGrossRev global function (registering in the attribute 
// table).  Get committed demands for the Perfect_World ATP and insert
// them into set_of_demand.  Return: a LgFrOrderedVectorDemand object (vector 
// of committed demands).

void *
difPWatpGetDemandGrossRev(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetDemandVol global function (registering in the attribute 
// table).  Get committed demands for the Perfect_World ATP and insert
// them into set_of_demand.  Return: a LgFrOrderedVectorDemand object (vector 
// of committed demands).

void *
difPWatpGetDemandVol(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetDemandObj1CumShipReward global function (registering in the attribute 
// table).  Get committed demands for the Perfect_World ATP and insert
// them into set_of_demand.  Return: a LgFrOrderedVectorDemand object (vector 
// of committed demands).

void *
difPWatpGetDemandObj1CumShipReward(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difPWatpGetDemandObj1ShipReward global function (registering in the attribute 
// table).  Get committed demands for the Perfect_World ATP and insert
// them into set_of_demand.  Return: a LgFrOrderedVectorDemand object (vector 
// of committed demands).

void *
difPWatpGetDemandObj1ShipReward(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);
  
//---------------------------------------------------------------------
// Define BOM
//---------------------------------------------------------------------
void*
difPWatpDefineBom(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);


//---------------------------------------------------------------------
// Context test
//---------------------------------------------------------------------
void
difPWatpContextTest(
   LgFrDataInterface    	& ts1dif,	// testScenario1's
   LgFrDataInterface    	& ts1dupDif,	// testScenario1dup's
   const LgFrScenario           * scenPtr       // testScenario1's
   );

#endif

