#ifndef LgFrDataInterfaceImplementationVolkswagenCMRP_h
#define LgFrDataInterfaceImplementationVolkswagenCMRP_h


// This file contains the global functions for:
//    LgFrDataInterfaceImplementationVolkswagenCMRP

// There is no need to have a special class for this case,
// We just need to instantiate a LgFrDataInterfaceImplementation 
// object and then register all LgFrDataInterfaceImplementationVolkswagenCMRP
// global functions into that object when we want to use it for the test.

#include <wit/src/wit.h>

#include <scenario/src/idnumber.h>
#include <scenario/src/error.h>
#include <scenario/src/generVc.h>

//---------------------------------------------------------------------
//
//   Data interface Implementation VW CMRP
//
//---------------------------------------------------------------------

// Forward references

class LgFrDataInterfaceImplementation;
class LgFrDataInterfaceEssentials;           
class LgFrScenario;

//---------------------------------------------------------------------
//
// To create a LgFrDataInterfaceImplementation object for VW CMRP
//
//---------------------------------------------------------------------

LgFrDataInterfaceImplementation * createVWcmrDifImp();

//---------------------------------------------------------------------
//
// To register all global functions 
//
//---------------------------------------------------------------------

void
registerVWcmrDifImpFunctions(LgFrDataInterfaceImplementation *);


//-------------------------------------------------------------------
//  
//   Global functions to obtain the defined parts and capacities.
//
//-------------------------------------------------------------------

// difVWcmrGetRawPart global function (registering in the attribute table).
// Get raw parts for the VW CMRP and insert them into set_of_part
// Return: a LgFrOrderedVectorPart object (vector of raw parts).

void *
difVWcmrGetRawParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrOrderedVectorItemPtr*,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetRawPart global function (registering in the attribute table).
// Get raw parts for the VW CMRP and insert them into set_of_part
// Return: a LgFrOrderedVectorPart object (vector of product parts).

void*
difVWcmrGetProductParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrOrderedVectorItemPtr* ,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetRawPart global function (registering in the attribute table).
// Get raw parts for the VW CMRP and insert them into set_of_part
// Return: a LgFrOrderedVectorPart object (vector of product parts).

void*
difVWcmrGetCapacityParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrOrderedVectorItemPtr* ,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = NULL); 

//-----------------------------------------------------------------------------
//  
//  Global functions to obtain raw material attributes
//
//-----------------------------------------------------------------------------



// difVWcmrGetRawUnitCost global function (registered in the attribute table)
// it returns a LgFrVectorFloat object.

void *
difVWcmrGetRawUnitCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetRawProcCost global function (registered in the attribute table).
// It returns a LgFrVectorFloat object.

void *
difVWcmrGetRawProcCost(
    const LgFrDataInterfaceImplementation * difImp,
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetRawConstrained global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difVWcmrGetRawConstrained(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetRawProcLeadTime global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difVWcmrGetRawProcLeadTime(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetRawObj1ScrapCost global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difVWcmrGetRawObj1ScrapCost(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetRawObj1StockCost global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difVWcmrGetRawObj1StockCost(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetRawSupplyVol global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difVWcmrGetRawSupplyVol(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = NULL);


//-----------------------------------------------------------------------------
//  
//  Global functions to obtain producible part attributes
//
//-----------------------------------------------------------------------------

// difVWcmrGetProductUnitCost global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difVWcmrGetProductUnitCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetProductProcCost global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difVWcmrGetProductProcCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetProductConstrained global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difVWcmrGetProductConstrained(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetProducYieldt global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difVWcmrGetProductYield(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetProductProcLeadTime global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difVWcmrGetProductProcLeadTime(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetProductCycleTime global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difVWcmrGetProductCycleTime(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetObj1ProdCost global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difVWcmrGetObj1ProdCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetProductObj1ScrapCost global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difVWcmrGetProductObj1ScrapCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetProductObj1StockCost global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difVWcmrGetProductObj1StockCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetProductSupplyVol global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difVWcmrGetProductSupplyVol(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetProductMinLotSize global function (registered in the attribute 
// table). It returns a LgFrVectorTimeVecFloat object.
void *
difVWcmrGetProductMinLotSize(
  const LgFrDataInterfaceImplementation * /* difImp, not used */,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);


//-----------------------------------------------------------------------------
//  
//  Global functions to obtain capacity attributes
//
//-----------------------------------------------------------------------------

// difVWcmrGetCapacityUnitCost global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difVWcmrGetCapacityUnitCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetCapacityConstrained global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difVWcmrGetCapacityConstrained(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetCapacityObj1ScrapCost global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difVWcmrGetCapacityObj1ScrapCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetCapacitySupplyVol global function (registered in the attribute 
// table).  It returns a LgFrVectorInt object.

void *
difVWcmrGetCapacitySupplyVol(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);  

//-----------------------------------------------------------------------------
//  
//  Global functions  to obtain demands
//
//-----------------------------------------------------------------------------


// difVWcmrGetCommittedDemands global function (registering in the attribute 
// table).  Get committed demands for the VW CMRP and insert
// them into set_of_demand.  Return: a LgFrOrderedVectorDemand object (vector 
// of committed demands).

void *
difVWcmrGetCommittedDemands(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetNewOpportunityDemands global function (registering in the attribute 
// table).  Get committed demands for the VW CMRP and insert
// them into set_of_demand.  Return: a LgFrOrderedVectorDemand object (vector 
// of committed demands).

void *
difVWcmrGetNewOpportunityDemands(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetFirmForecastDemands global function (registering in the attribute 
// table).  Get committed demands for the VW CMRP and insert
// them into set_of_demand.  Return: a LgFrOrderedVectorDemand object (vector 
// of committed demands).

void *
difVWcmrGetFirmForecastDemands(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetRiskForecastDemands global function (registering in the attribute 
// table).  Get committed demands for the VW CMRP and insert
// them into set_of_demand.  Return: a LgFrOrderedVectorDemand object (vector 
// of committed demands).

void *
difVWcmrGetRiskForecastDemands(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

//-----------------------------------------------------------------------------
//  
//  Global functions to obtain demand attributes
//
//-----------------------------------------------------------------------------

// difVWcmrGetDemandGrossRev global function (registering in the attribute 
// table).  Get committed demands for the VW CMRP and insert
// them into set_of_demand.  Return: a LgFrOrderedVectorDemand object (vector 
// of committed demands).

void *
difVWcmrGetDemandGrossRev(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetDemandVol global function (registering in the attribute 
// table).  Get committed demands for the VW CMRP and insert
// them into set_of_demand.  Return: a LgFrOrderedVectorDemand object (vector 
// of committed demands).

void *
difVWcmrGetDemandVol(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetDemandObj1CumShipReward global function (registering in the attribute 
// table).  Get committed demands for the VW CMRP and insert
// them into set_of_demand.  Return: a LgFrOrderedVectorDemand object (vector 
// of committed demands).

void *
difVWcmrGetDemandObj1CumShipReward(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);

// difVWcmrGetDemandObj1ShipReward global function (registering in the attribute 
// table).  Get committed demands for the VW CMRP and insert
// them into set_of_demand.  Return: a LgFrOrderedVectorDemand object (vector 
// of committed demands).

void *
difVWcmrGetDemandObj1ShipReward(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = NULL);
   
//---------------------------------------------------------------------
// Define BOM
//---------------------------------------------------------------------
void*
difVWcmrDefineBom(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrOrderedVectorItemPtr*, // itemPtrOrdVecPtr,
    LgFrError &, // status, 
    const LgFrDataInterfaceEssentials * difEssenPtr = NULL);


//---------------------------------------------------------------------
// Context test
//---------------------------------------------------------------------
void
difVWcmrContextTest(
   LgFrDataInterface    	& ts1dif,	// testScenario1's
   LgFrDataInterface    	& ts1dupDif,	// testScenario1dup's
   const LgFrScenario          * scenPtr);     // testScenario1's

#endif





























