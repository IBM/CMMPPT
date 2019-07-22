#ifndef LgFrDataInterfaceImplementationPerfectWorldCMRP_h
#define LgFrDataInterfaceImplementationPerfectWorldCMRP_h
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

// This file contains the global functions for:
//    LgFrDataInterfaceImplementationPerfectWorldCMRP

// There is no need to have a special class for this case,
// We just need to instantiate a LgFrDataInterfaceImplementation 
// object and then register all LgFrDataInterfaceImplementationPerfectWorldCMRP
// global functions into that object when we want to use it for the test.

#include <wit/src/wit.h>

#include <scenario/src/idnumber.h>
#include <scenario/src/error.h>
#include <scenario/src/generVc.h>
#include <scenario/src/dif.h>

//---------------------------------------------------------------------------
//
// Forward references
//
//---------------------------------------------------------------------------
class LgFrDataInterfaceImplementation;
class LgFrDataInterfaceEssentials;           
class LgFrScenario;

//---------------------------------------------------------------------------
//
// All gobal functions declarations
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// To register all global functions 
//---------------------------------------------------------------------------
void
registerPWcmrpDifImpFunctions(LgFrDataInterfaceImplementation *dImpPW);

//---------------------------------------------------------------------------
// To create a LgFrDataInterfaceImplementation object for Perfect_World CMRP
//---------------------------------------------------------------------------
LgFrDataInterfaceImplementation * 
createPWcmrpDifImp();

//---------------------------------------------------------------------------
//
// Global functions to get "parts"
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// difPWcmrpGetRawPart global function (registering in the attribute table).
// Get raw parts for the Perfect_World CMRP and insert them into set_of_part
// Return: a LgFrPartVector object (vector of raw parts).
//---------------------------------------------------------------------------
void *
difPWcmrpGetRawParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------
// difPWcmrpGetRawPart global function (registering in the attribute table).
// Get product parts for the Perfect_World CMRP and insert it into set_of_part
// Return: a LgFrPartVector object (vector of producible parts)
//---------------------------------------------------------------------------
void*
difPWcmrpGetProductParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------
// difPWcmrpGetRawPart global function (registering in the attribute table).
// Get capacity parts for the Perfect_World CMRP and insert it into set_of_part
// Return: a LgFrPartVector object (vector of capacities).
//---------------------------------------------------------------------------
void*
difPWcmrpGetCapacityParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0); 

//---------------------------------------------------------------------------
//
// Global functions to get raw material attributes.
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// difPWcmrpGetRawUnitCost global function (registered in the attribute table)
// it returns a LgFrFloatVector object.
//---------------------------------------------------------------------------
void *
difPWcmrpGetRawUnitCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------
// difPWcmrpGetRawProcCost global function (registered in the attribute table).
// It returns a LgFrFloatVector object.
//---------------------------------------------------------------------------
void *
difPWcmrpGetRawProcCost(
    const LgFrDataInterfaceImplementation * difImp,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------
// difPWcmrpGetRawConstrained global function (registered in the attribute table).
// It returns a LgFrIntVector objec
//---------------------------------------------------------------------------
void *
difPWcmrpGetRawConstrained(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);               

//---------------------------------------------------------------------------
// difPWcmrpGetRawProcLeadTime global function (registered in the attribute table).
// It returns a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------
void *
difPWcmrpGetRawProcLeadTime(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------
// difPWcmrpGetRawObj1ScrapCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------
void *
difPWcmrpGetRawObj1ScrapCost(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------
// difPWcmrpGetRawObj1StockCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------
void *
difPWcmrpGetRawObj1StockCost(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------
// difPWcmrpGetRawSupplyVol global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------
void *
difPWcmrpGetRawSupplyVol(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------
//
// Global functions to get product attributes.
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// difPWcmrpGetProductUnitCost global function (registered in the attribute table)
// it returns a LgFrFloatVector object.
//---------------------------------------------------------------------------
void *
difPWcmrpGetProductUnitCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------
// difPWcmrpGetProductProcCost global function (registered in the attribute table).
// It returns a LgFrFloatVector object.
//---------------------------------------------------------------------------
void *
difPWcmrpGetProductProcCost(
    const LgFrDataInterfaceImplementation * difImp,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------
// difPWcmrpGetProductConstrained global function (registered in the attribute table).
// It returns a LgFrIntVector objec
//---------------------------------------------------------------------------
void *
difPWcmrpGetProductConstrained(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);               

//---------------------------------------------------------------------------
// difPWcmrpGetProductProcLeadTime global function (registered in the attribute table).
// It returns a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------
void *
difPWcmrpGetProductProcLeadTime(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------
// difPWcmrpGetProductYield global function (registered in the attribute table).
// Return: a LgFrFloatVector object
//---------------------------------------------------------------------------
void *
difPWcmrpGetProductYield(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0
    );

//---------------------------------------------------------------------------
// difPWcmrpGetProductCycleTime global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------
void *
difPWcmrpGetProductCycleTime(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0
    );

//---------------------------------------------------------------------------
// difPWcmrpGetObj1ProdCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------
void *
difPWcmrpGetObj1ProdCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0
    );

//---------------------------------------------------------------------------
// difPWcmrpGetProductObj1ScrapCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------
void *
difPWcmrpGetProductObj1ScrapCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0
    );

//---------------------------------------------------------------------------
// difPWcmrpGetProductObj1StockCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------
void *
difPWcmrpGetProductObj1StockCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0
    );

//---------------------------------------------------------------------------
// difPWcmrpGetProductSupplyVol global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------
void *
difPWcmrpGetProductSupplyVol(
    const LgFrDataInterfaceImplementation * /* difImp,  not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------
// difPWcmrpGetProductMinLotSize global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------
void*
difPWcmrpGetProductMinLotSize(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0
    );

//---------------------------------------------------------------------------
//  
//   Global functions to obtain capacity attributes
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// difPWcmrpGetCapacityUnitCost global function (registered in the attribute table)
// it returns a LgFrFloatVector object.
//---------------------------------------------------------------------------
void *
difPWcmrpGetCapacityUnitCost(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0
    );

//---------------------------------------------------------------------------
// difPWcmrpGetCapacityConstrained global function (registered in the attribute table).
// It returns a LgFrIntVector object
//---------------------------------------------------------------------------
void *
difPWcmrpGetCapacityConstrained(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0
    );

//---------------------------------------------------------------------------
// difPWcmrpGetCapacityObj1ScrapCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------
void *
difPWcmrpGetCapacityObj1ScrapCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0
    );

//---------------------------------------------------------------------------
// difPWcmrpGetCapacitySupplyVol global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------
void *
difPWcmrpGetCapacitySupplyVol(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0
    );

//---------------------------------------------------------------------------
//  
//   Global functions to obtain demands
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// difPWcmrpGetCommittedDemands global function (registering in the attribute table).
// Get committed demands for the Perfect_World CMRP and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of committed demands).
//---------------------------------------------------------------------------
void *
difPWcmrpGetCommittedDemands(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0); 

//---------------------------------------------------------------------------
// difPWcmrpGetNewOpportunityDemands global function (registering in the attribute table).
// Get new opportunity demands for the Perfect_World CMRP and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of new opportunity demands).
//---------------------------------------------------------------------------
void *
difPWcmrpGetNewOpportunityDemands(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0); 

//---------------------------------------------------------------------------
// difPWcmrpGetFirmForecastDemands global function (registering in the attribute table).
// Get firm forcast demands for the Perfect_World CMRP and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of firm forcast demands).
//---------------------------------------------------------------------------
void *
difPWcmrpGetFirmForecastDemands(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------
// difPWcmrpGetRiskForecastDemands global function (registering in the attribute table).
// Get risk forcast demands for the Perfect_World CMRP and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of risk forcast demands).
//---------------------------------------------------------------------------
void *
difPWcmrpGetRiskForecastDemands(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//-------------------------------------------------------------
//
// Global functions to obtain demands attributes
//
//-------------------------------------------------------------

//---------------------------------------------------------------------------
// difPWcmrpGetDemandGrossRev global function (registering in the attribute table).
// Compute total demand gross revenue
// Return: a LgFrFloatVector object (vector of demand gross revenues).
//---------------------------------------------------------------------------
void *
difPWcmrpGetDemandGrossRev(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------
// difPWcmrpGetDemandVol global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------
void *
difPWcmrpGetDemandVol(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------
// difPWcmrpGetDemanObj1CumShipReward global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------
void *
difPWcmrpGetDemandObj1CumShipReward(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------
// difPWcmrpGetDemanObj1ShipReward global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------
void *
difPWcmrpGetDemandObj1ShipReward(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------
// Define BOM
//---------------------------------------------------------
void*
difPWcmrpDefineBom(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------
// Context test
//---------------------------------------------------------
void
difPWcmrpContextTest(
    LgFrDataInterface    	& ts1dif,	// testScenario1's
    LgFrDataInterface    	& ts1dupDif,	// testScenario1dup's
    const LgFrScenario          * scenPtr       // testScenario1's
    ); 


#endif

