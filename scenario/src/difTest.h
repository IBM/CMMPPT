#ifndef LgFrDataInterfaceImplementationForTest_h
#define LgFrDataInterfaceImplementationForTest_h
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

// This file contains the global functions for:
// LgFrDataInterfaceImplementationForTest

// There is no need to have a special class for this case,
// We just need to instantiate a LgFrDataInterfaceImplementation 
// object and then register all LgFrDataInterfaceImplementationForTest
// global functions into that object when we want to use it for the test.


#include <wit/src/wit.h>

#include <scenario/src/idnumber.h>
#include <scenario/src/error.h>
#include <scenario/src/generVc.h>


//---------------------------------------------------------------------------------------
//
// Forward references
//
//---------------------------------------------------------------------------------------
class LgFrDataInterfaceImplementation;
class LgFrDataInterfaceEssentials;           

//---------------------------------------------------------------------------------------
//
// All gobal functions declarations
//
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// To register all global functions for this class
//---------------------------------------------------------------------------------------
void
registerDifTestImpFunctions(LgFrDataInterfaceImplementation *dImpTst);

//---------------------------------------------------------------------------------------
// To create a LgFrDataInterfaceImplementation object for difTest
//---------------------------------------------------------------------------------------
LgFrDataInterfaceImplementation * 
createDifTestImp();

//---------------------------------------------------------------------------------------
//
// Global functions to get "parts"
//

//---------------------------------------------------------------------------------------
// difTestGetRawPart global function (registering in the attribute table).
// Get raw parts and insert them into set_of_part
// Return: a LgFrPartVector object (vector of raw parts).
//---------------------------------------------------------------------------------------
void *
difTestGetRawParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------------
// difTestGetProductPart global function (registering in the attribute table).
// Get product parts and insert it into set_of_part
// Return: a LgFrPartVector object (vector of producible parts)
//---------------------------------------------------------------------------------------
void*
difTestGetProductParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------------
// difTestGetCapacityParts global function (registering in the attribute table).
// Get capacity parts and insert it into set_of_part
// Return: a LgFrPartVector object (vector of capacities).
//---------------------------------------------------------------------------------------
void*
difTestGetCapacityParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0); 

//---------------------------------------------------------------------------------------
//
// Global functions to get part attributes.
//
//---------------------------------------------------------------------------------------
// difTestGetRawUnitCost global function (registered in the attribute table)
// it returns a LgFrFloatVector object.
//---------------------------------------------------------------------------------------
void *
difTestGetRawUnitCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------------
// difTestGetRawProcCost global function (registered in the attribute table).
// It returns a LgFrFloatVector object.
//---------------------------------------------------------------------------------------
void *
difTestGetRawProcCost(
    const LgFrDataInterfaceImplementation * difImp,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------------
// difTestGetRawConstrained global function (registered in the attribute table).
// It returns a LgFrIntVector object.
//---------------------------------------------------------------------------------------
void *
difTestGetRawConstrained(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);               

//---------------------------------------------------------------------------------------
// difTestGetPartProcLeadTime global function (registered in the attribute table).
// It returns a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difTestGetRawProcLeadTime(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------------
// difTestGetPartObj1ScrapCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difTestGetRawObj1ScrapCost(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//---------------------------------------------------------------------------------------
// difTestGetPartObj1StockCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difTestGetRawObj1StockCost(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------------
// difTestGetPartSupplyVol global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difTestGetRawSupplyVol(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//--------------------------------------------------------------------
//  
//   Global functions to obtain producible part attributes
//
//-------------------------------------------------------------------


//---------------------------------------------------------------------------------------
// difTestGetProductProcCost global function (registered in the attribute table).
// It returns a LgFrFloatVector object.
//---------------------------------------------------------------------------------------
void *
difTestGetProductProcCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//---------------------------------------------------------------------------------------
// difTestGetProductUnitCost global function (registered in the attribute table)
// it returns a LgFrFloatVector object.
//---------------------------------------------------------------------------------------
void *
difTestGetProductUnitCost(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//---------------------------------------------------------------------------------------
// difTestGetProductConstrained global function (registered in the attribute table).
// It returns a LgFrIntVector object
//---------------------------------------------------------------------------------------
void *
difTestGetProductConstrained(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//---------------------------------------------------------------------------------------
// difTestGetProductYield global function (registered in the attribute table).
// Return: a LgFrFloatVector object
//---------------------------------------------------------------------------------------
void *
difTestGetProductYield(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//---------------------------------------------------------------------------------------
// difTestGetPartCycleTime global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difTestGetPartCycleTime(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//---------------------------------------------------------------------------------------
// difTestGetProductProcLeadTime global function (registered in the attribute table).
// It returns a LgFrTimeVecFloatVector object
//---------------------------------------------------------------------------------------
void *
difTestGetProductProcLeadTime(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//---------------------------------------------------------------------------------------
// difTestGetPartObj1ProdCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difTestGetPartObj1ProdCost(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//---------------------------------------------------------------------------------------
// difTestGetProductObj1ScrapCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difTestGetProductObj1ScrapCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//---------------------------------------------------------------------------------------
// difTestGetProductObj1StockCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difTestGetProductObj1StockCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//---------------------------------------------------------------------------------------
// difTestGetProductSupplyVol global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difTestGetProductSupplyVol(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//---------------------------------------------------------------------------------------
// difTestGetProductMinLotSize global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void*
difTestGetProductMinLotSize(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//--------------------------------------------------------------------
//  
//   Methods to obtain capacity attributes
//
//-------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// difTestGetCapacityUnitCost global function (registered in the attribute table)
// it returns a LgFrFloatVector object.
//---------------------------------------------------------------------------------------
void *
difTestGetCapacityUnitCost(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//---------------------------------------------------------------------------------------
//difTestGetCapacityConstrained global function (registered in the attribute table).
// It returns a LgFrIntVector objec
//---------------------------------------------------------------------------------------
void *
difTestGetCapacityConstrained(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//---------------------------------------------------------------------------------------
//difTestGetCapacityObj1ScrapCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difTestGetCapacityObj1ScrapCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//---------------------------------------------------------------------------------------
//difTestGetCapacitySupplyVol global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difTestGetCapacitySupplyVol(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//--------------------------------------------------------------------
//  
//   Global functions to obtain demands
//
//-------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// difTestGetCommittedDemands global function (registering in the attribute table).
// Get committed demands and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of committed demands).
//---------------------------------------------------------------------------------------
void *
difTestGetCommittedDemands(
  const LgFrDataInterfaceImplementation * /* dImp,not used */,
  const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//---------------------------------------------------------------------------------------
// difTestGetNewOpportunityDemands global function (registering in the attribute table).
// Get new opportunity demands and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of new opportunity demands).
//---------------------------------------------------------------------------------------
void *
difTestGetNewOpportunityDemands(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//---------------------------------------------------------------------------------------
// difTestGetFirmForecastDemands global function (registering in the attribute table).
// Get firm forcast demands and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of firm forcast demands).
//---------------------------------------------------------------------------------------
void *
difTestGetFirmForecastDemands(
    const LgFrDataInterfaceImplementation * /* dImp,not used */,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//---------------------------------------------------------------------------------------
// difTestGetRiskForecastDemands global function (registering in the attribute table).
// Get risk forcast demands and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of risk forcast demands).
//---------------------------------------------------------------------------------------
void *
difTestGetRiskForecastDemands(
    const LgFrDataInterfaceImplementation * /* dImp,not used */,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//--------------------------------------------------------------------
//  
//   Global functions to obtain demand attributes
//
//-------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// difTestGetDemandGrossRev global function (registering in the attribute table).
// Compute total demand gross revenue
// Return: a LgFrFloatVector object (vector of demand gross revenues).
//---------------------------------------------------------------------------------------
void *
difTestGetDemandGrossRev(
    const LgFrDataInterfaceImplementation * /* difImp,  not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//---------------------------------------------------------------------------------------
// difTestGetDemandVol global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difTestGetDemandVol(
    const LgFrDataInterfaceImplementation * /* difImp,  not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//---------------------------------------------------------------------------------------
// difTestGetDemanObj1CumShipReward global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difTestGetDemandObj1CumShipReward(
    const LgFrDataInterfaceImplementation * /* difImp,  not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//---------------------------------------------------------------------------------------
// difTestGetDemanObj1ShipReward global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difTestGetDemandObj1ShipReward(
    const LgFrDataInterfaceImplementation * /* difImp,  not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//--------------------------------------------------------------------
//  
//   Global function to define BOM
//
//-------------------------------------------------------------------

void*
difTestDefineBom(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector*, // itemPtrOrdVecPtr,
    LgFrError &, // status, 
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//---------------------------------------------------------------------------------------
// Tests this case.  Only returns if all tests passed
// Expects data testScenario1 to have come from
// /u/implode/wit/data/examples/diner12/wit.data, and expects
// testScenario2 to have been made by copying testScenario1
//---------------------------------------------------------------------------------------

void
difTestContextTest(
   LgFrScenario        		& testScenario1,
   LgFrLastProblemSolved       	& /* lps */,      // testScenario1's
   LgFrDataInterface	       	& ts1dif,	// testScenario1's
   LgFrScenario 	       	& testScenario1dup,
   LgFrDataInterface	       	& ts1dupDif);	// testScenario1dup's


#endif
