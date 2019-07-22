#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.
#include <math.h>
#include <iostream>


#include <scenario/src/dif.h>
#include <scenario/src/difImpl.h>
#include <scenario/src/difTest.h>
#include <scenario/src/calendar.h>
#include <scenario/src/difEssSc.h>


//-------------------------------------------------------------------
//  
// Global functions to obtain the defined parts and capacities.
//
//-------------------------------------------------------------------

// difTestGetRawPart global function (registering in the attribute table).
// Get raw parts and insert them into set_of_part
// Return: a LgFrPartVector object (vector of raw parts).
void *
difTestGetRawParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  LgFrPartVector& retVal = *(new LgFrPartVector);
  LgFrPart part = difEssenPtr->createAndInsertPart("Raw");
  retVal.push_back(part);
  status = 0;
  return &retVal;
}

// difTestGetProductPart global function (registering in the attribute table).
// Get product parts from WIT and insert it into set_of_part
// Return: a LgFrPartVector object (vector of producible parts)
void*
difTestGetProductParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  LgFrPartVector& retVal = *(new LgFrPartVector);
  LgFrPart part = difEssenPtr->createAndInsertPart("Product");
  retVal.push_back(part);
  status = 0;
  return &retVal;
}

// difTestGetCapacityPart global function (registering in the attribute table).
// Get capacity parts from WIT and insert it into set_of_part
// Return: a LgFrPartVector object (vector of capacities).
void*
difTestGetCapacityParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  LgFrPartVector& retVal = *(new LgFrPartVector);
  LgFrPart part = difEssenPtr->createAndInsertPart("Capacity");
  retVal.push_back(part);  
  status = 0;
  return &retVal;
}

//--------------------------------------------------------------------
//  
//   Global functions to obtain raw part attributes
//
//-------------------------------------------------------------------

// difTestGetRawUnitCost global function (registered in the attribute table)
// it returns a LgFrFloatVector object.
void *
difTestGetRawUnitCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrFloatVector& retVal = *(new LgFrFloatVector);
  retVal.reserve ( pv.size() );

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    retVal.push_back( 123. );
  }

  status = 0;
  return &retVal;
}
  
// difTestGetPartProcCost global function (registered in the attribute table).
// It returns a LgFrFloatVector object.
void *
difTestGetRawProcCost(
    const LgFrDataInterfaceImplementation * difImp,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrFloatVector& retVal = *(new LgFrFloatVector);
  retVal.reserve ( pv.size() );

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    retVal.push_back( 103. );
  }

  status = 0;
  return &retVal;
}

// difTestGetPartConstrained global function (registered in the attribute table).
// It returns a LgFrIntVector object
void *
difTestGetRawConstrained(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)               
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrIntVector& retVal = *(new LgFrIntVector);
  retVal.reserve ( pv.size() );

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) 
    retVal.push_back( false );

  status = 0;
  return &retVal; 
} 

// difTestGetPartProcLeadTime global function (registered in the attribute table).
// It returns a LgFrTimeVecFloatVector object.
void *
difTestGetRawProcLeadTime(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrTimeVecFloatVector& retVal 
      = *(new LgFrTimeVecFloatVector);
  retVal.reserve ( pv.size() );

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    retVal.push_back( LgFrTimeVecFloat( nperiods , 1.00 ) );
  }

  status = 0;
  return &retVal;
}      


// difTestGetPartObj1ScrapCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difTestGetRawObj1ScrapCost(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrTimeVecFloatVector& retVal 
      = *(new LgFrTimeVecFloatVector);
  retVal.reserve ( pv.size() );

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    retVal.push_back( LgFrTimeVecFloat( nperiods ,2.34f ) );
  }

  status = 0;
  return &retVal;
}

// difTestGetPartObj1StockCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difTestGetRawObj1StockCost(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrTimeVecFloatVector& retVal 
      = *(new LgFrTimeVecFloatVector);
  retVal.reserve ( pv.size() );

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<pv.size(); i++ ) {
    retVal.push_back( LgFrTimeVecFloat( nperiods ,3.f ) );
  }

  status = 0;
  return &retVal;
}

// difTestGetPartSupplyVol global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difTestGetRawSupplyVol(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrTimeVecFloatVector& retVal 
      = *(new LgFrTimeVecFloatVector);
  retVal.reserve ( pv.size() );

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) 
  {
    retVal.push_back( LgFrTimeVecFloat( nperiods ,5.f ) );
  }

  status = 0;
  return &retVal;
}

//--------------------------------------------------------------------
//  
//   Global functions to obtain producible part attributes
//
//-------------------------------------------------------------------


// difTestGetProductProcCost global function (registered in the attribute table).
// It returns a LgFrFloatVector object.
void *
difTestGetProductProcCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * /* difEssenPtr, not used */
    )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrFloatVector& retVal = *(new LgFrFloatVector);
  retVal.reserve ( pv.size() );

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    retVal.push_back( 3.01f );
  }

  status = 0;
  return &retVal;
}


// difTestGetProductUnitCost global function (registered in the attribute table)
// it returns a LgFrFloatVector object.
void *
difTestGetProductUnitCost(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * /* difEssenPtr, not used */
    )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrFloatVector& retVal = *(new LgFrFloatVector);
  retVal.reserve ( pv.size() );

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    retVal.push_back( 3.21f );
  }

  status = 0;
  return &retVal;
}


// difTestGetProductConstrained global function (registered in the attribute table).
// It returns a LgFrIntVector object
void *
difTestGetProductConstrained(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * /* difEssenPtr, not used */
    )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrIntVector& retVal = *(new LgFrIntVector);
  retVal.reserve ( pv.size() );

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    retVal.push_back( true );    
  }

  status = 0;
  return &retVal;
}


// difTestGetProductYield global function (registered in the attribute table).
// Return: a LgFrIntVector object
void *
difTestGetProductYield(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * /* difEssenPtr, not used */
    )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrIntVector& retVal = *(new LgFrIntVector);
  retVal.reserve ( pv.size() );

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    retVal.push_back( 96 );
  }

  status = 0;
  return &retVal;
}


// difTestGetProductCycleTime global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difTestGetProductCycleTime(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrTimeVecFloatVector& retVal 
      = *(new LgFrTimeVecFloatVector);
  retVal.reserve ( pv.size() );

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    retVal.push_back( LgFrTimeVecFloat( nperiods , 1.2f ) );
  }

  status = 0;
  return &retVal;
}


// difTestGetProductProcLeadTime global function (registered in the attribute table).
// It returns a LgFrTimeVecFloatVector object.
void *
difTestGetProductProcLeadTime(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrTimeVecFloatVector& retVal 
      = *(new LgFrTimeVecFloatVector);
  retVal.reserve ( pv.size() );

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    retVal.push_back( LgFrTimeVecFloat( nperiods , -1.0f ) );
  }

  status = 0;
  return &retVal;
}


// difTestGetProductObj1ProdCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difTestGetProductObj1ProdCost(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrTimeVecFloatVector& retVal 
      = *(new LgFrTimeVecFloatVector);
  retVal.reserve ( pv.size() );

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    retVal.push_back( LgFrTimeVecFloat( nperiods , 3.4f ) );
  }

  status = 0;
  return &retVal;
}

// difTestGetProductObj1ScrapCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difTestGetProductObj1ScrapCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrTimeVecFloatVector& retVal
      = *(new LgFrTimeVecFloatVector);
  retVal.reserve ( pv.size() );

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    retVal.push_back( LgFrTimeVecFloat( nperiods , 5.6f ) );
  }

  status = 0;
  return &retVal;
}

// difTestGetProductObj1StockCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difTestGetProductObj1StockCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrTimeVecFloatVector& retVal
      = *(new LgFrTimeVecFloatVector);
  retVal.reserve ( pv.size() );

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    retVal.push_back( LgFrTimeVecFloat( nperiods , 7.8f ) );    
  }

  status = 0;
  return &retVal;
}

// difTestGetProductSupplyVol global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difTestGetProductSupplyVol(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrTimeVecFloatVector& retVal 
      = *(new LgFrTimeVecFloatVector);
  retVal.reserve ( pv.size() );

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    retVal.push_back( LgFrTimeVecFloat( nperiods , 9.0f ) );    
  }

  status = 0;
  return &retVal;
}

// difTestGetProductMinLotSize global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void*
difTestGetProductMinLotSize(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrTimeVecFloatVector& retVal
      = *(new LgFrTimeVecFloatVector);
  retVal.reserve ( pv.size() );

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    retVal.push_back( LgFrTimeVecFloat( nperiods , 1.0f ) );    
  }

  status = 0;
  return &retVal;
}

//--------------------------------------------------------------------
//  
//   Methods to obtain capacity attributes
//
//-------------------------------------------------------------------

// difTestGetCapacityUnitCost global function (registered in the attribute table)
// it returns a LgFrFloatVector object.
void *
difTestGetCapacityUnitCost(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * /* difEssenPtr, not used */
    )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrFloatVector& retVal = *(new LgFrFloatVector);
  retVal.reserve ( pv.size() );

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    retVal.push_back( 2.46f );
  }

  status = 0;
  return &retVal;
}


//difTestGetCapacityConstrained global function (registered in the attribute table).
// It returns a LgFrIntVector object
void *
difTestGetCapacityConstrained(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * /* difEssenPtr, not used */
    )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrIntVector& retVal = *(new LgFrIntVector);
  retVal.reserve ( pv.size() );

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    retVal.push_back( true );
  }

  status = 0;
  return &retVal;
}


//difTestGetCapacityObj1ScrapCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difTestGetCapacityObj1ScrapCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrTimeVecFloatVector& retVal
      = *(new LgFrTimeVecFloatVector);
  retVal.reserve ( pv.size() );

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    retVal.push_back( LgFrTimeVecFloat( nperiods , 78.f ) );
  }

  status = 0;
  return &retVal;
}


//difTestGetCapacitySupplyVol global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difTestGetCapacitySupplyVol(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr; 
  LgFrTimeVecFloatVector& retVal
      = *(new LgFrTimeVecFloatVector);
  retVal.reserve ( pv.size() );

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    retVal.push_back( LgFrTimeVecFloat( nperiods , 90.f ) );    
  }

  status = 0;
  return &retVal;
}


//--------------------------------------------------------------------
//  
//   Global functions to obtain demands
//
//-------------------------------------------------------------------

// difTestGetCommittedDemands global function (registering in the attribute table).
// Get committed demands and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of committed demands).
void *
difTestGetCommittedDemands(
  const LgFrDataInterfaceImplementation * /* dImp,not used */,
  const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  LgFrDemandVector& retVal = 
      *(new LgFrDemandVector);

  std::string dn = "Committed";
  std::string pn = "Product";

  LgFrDemand demand = 
      difEssenPtr->createAndInsertDemand(dn,pn);
  retVal.push_back(demand);

  status = 0;
  return &retVal;
}


// difTestGetNewOpportunityDemands global function (registering in the attribute table).
// Get new opportunity demands and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of new opportunity demands).
void *
difTestGetNewOpportunityDemands(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  LgFrDemandVector& retVal = 
      *(new LgFrDemandVector);

  std::string dn = "NewOpportunity";
  std::string pn = "Product";

  LgFrDemand demand = 
      difEssenPtr->createAndInsertDemand(dn,pn);
  retVal.push_back(demand);

  status = 0;
  return &retVal;
}


// difTestGetFirmForecastDemands global function (registering in the attribute table).
// Get firm forcast demands and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of firm forcast demands).
void *
difTestGetFirmForecastDemands(
    const LgFrDataInterfaceImplementation * /* dImp,not used */,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  LgFrDemandVector& retVal = 
      *(new LgFrDemandVector);

  std::string dn = "FirmForecast";
  std::string pn = "Product";

  LgFrDemand demand = 
      difEssenPtr->createAndInsertDemand(dn,pn);
  retVal.push_back(demand);

  status = 0;
  return &retVal;
}


// difTestGetRiskForecastDemands global function (registering in the attribute table).
// Get risk forcast demands and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of risk forcast demands).
void *
difTestGetRiskForecastDemands(
    const LgFrDataInterfaceImplementation * /* dImp,not used */,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  LgFrDemandVector& retVal = 
      *(new LgFrDemandVector);

  std::string dn = "RiskForecast";
  std::string pn = "Product";

  LgFrDemand demand = 
      difEssenPtr->createAndInsertDemand(dn,pn);
  retVal.push_back(demand);

  status = 0;
  return &retVal;
}


//--------------------------------------------------------------------
//  
//   Global functions to obtain demand attributes
//
//-------------------------------------------------------------------

// difTestGetDemandGrossRev global function (registering in the attribute table).
// Compute total demand gross revenue
// Return: a LgFrFloatVector object (vector of demand gross revenues).
void *
difTestGetDemandGrossRev(
    const LgFrDataInterfaceImplementation * /* difImp,  not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * /* difEssenPtr, not used */
    )
{
  LgFrItemPtrVector dv = *itemPtrOrdVecPtr; 
  LgFrFloatVector& retVal = *(new LgFrFloatVector);
  retVal.reserve( dv.size() );    
  
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < dv.size(); i++ ) {
    if      ( dv[i]->name() == "Committed"     )  retVal.push_back( 111.f );
    else if ( dv[i]->name() == "NewOpportunity" )  retVal.push_back( 222.f );
    else if ( dv[i]->name() == "FirmForecast"   )  retVal.push_back( 333.f );
    else if ( dv[i]->name() == "RiskForecast"   )  retVal.push_back( 444.f );
    else assert( 0 && "illegal demand name for Gross Revenue" );
  }
  status = 0;
  return &retVal;
}


// difTestGetDemandVol global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difTestGetDemandVol(
    const LgFrDataInterfaceImplementation * /* difImp,  not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrItemPtrVector dv = *itemPtrOrdVecPtr; 
  LgFrTimeVecFloatVector& retVal = 
      *(new LgFrTimeVecFloatVector);
  retVal.reserve( dv.size() );    

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < dv.size(); i++ ) {
    retVal.push_back( LgFrTimeVecFloat( nperiods, 111.f ) );
  }
  status = 0;
  return &retVal;
}


// difTestGetDemanObj1CumShipReward global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difTestGetDemandObj1CumShipReward(
    const LgFrDataInterfaceImplementation * /* difImp,  not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrItemPtrVector dv = *itemPtrOrdVecPtr; 
  LgFrTimeVecFloatVector& retVal = 
      *(new LgFrTimeVecFloatVector);
  retVal.reserve( dv.size() );    

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < dv.size(); i++ ) {
    retVal.push_back( LgFrTimeVecFloat( nperiods, 222.f ) );
  }
  status = 0;
  return &retVal;
}


// difTestGetDemanObj1ShipReward global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difTestGetDemandObj1ShipReward(
    const LgFrDataInterfaceImplementation * /* difImp,  not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrItemPtrVector dv = *itemPtrOrdVecPtr; 
  LgFrTimeVecFloatVector& retVal = 
      *(new LgFrTimeVecFloatVector);
  retVal.reserve( dv.size() );    

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<dv.size(); i++ ) {
    retVal.push_back( LgFrTimeVecFloat( nperiods, 333.f ) );
  }      
  status = 0;
  return &retVal;
}


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
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
   WitRun* eater = 
     ((LgFrDataInterfaceEssentialsFromScenario*)difEssenPtr)
     ->mutableWitRun();

   witAddBomEntry(eater,"Product","Raw");
   witAddBomEntry(eater,"Product","Capacity");
   return 0;
}

//---------------------------------------------------------------------------------------
// To create a LgFrDataInterfaceImplementation object for difTest
//---------------------------------------------------------------------------------------
LgFrDataInterfaceImplementation * 
createDifTestImp()
{
  return new LgFrDataInterfaceImplementation;
}


// -----------------------------------------------------------------------
//  register functions
// -----------------------------------------------------------------------

void
registerDifTestImpFunctions(
    LgFrDataInterfaceImplementation * dImp) 
{

  
    // Register all raw material attribute functions
    dImp->registerFunction("rawUnitCost", material, part,
			   vectorFloat, difTestGetRawUnitCost);
    dImp->registerFunction("rawProcCost", material, part,
			   vectorFloat, difTestGetRawProcCost);
    dImp->registerFunction("rawConstrained", material, part,
			   vectorInt, difTestGetRawConstrained);
    dImp->registerFunction("rawProcLeadTime", material, part,
			   timeVecFloat, difTestGetRawProcLeadTime);
    dImp->registerFunction("rawObj1ScrapCost", material, part,
			   timeVecFloat, difTestGetRawObj1ScrapCost);
    dImp->registerFunction("rawObj1StockCost", material, part,
			   timeVecFloat, difTestGetRawObj1StockCost);
    dImp->registerFunction("rawSupplyVol", material, part,
			   timeVecFloat, difTestGetRawSupplyVol);

    // Register all product attribute functions
    dImp->registerFunction("productUnitCost", material, part,
			   vectorFloat, difTestGetProductUnitCost);
    dImp->registerFunction("productProcCost", material, part,
			   vectorFloat, difTestGetProductProcCost);
    dImp->registerFunction("productConstrained", material, part,
			   vectorInt, difTestGetProductConstrained);
    dImp->registerFunction("productProcLeadTime", material, part,
       		   timeVecFloat, difTestGetProductProcLeadTime);
    dImp->registerFunction("productYield", material, part,
			   vectorInt, difTestGetProductYield);
    dImp->registerFunction("productCycleTime", material, part,
			   timeVecFloat, difTestGetProductCycleTime);
    dImp->registerFunction("obj1ProdCost", material, part,
			   timeVecFloat, difTestGetProductObj1ProdCost);
    dImp->registerFunction("productObj1ScrapCost", material, part,
			   timeVecFloat, difTestGetProductObj1ScrapCost);
    dImp->registerFunction("productObj1StockCost", material, part,
			   timeVecFloat, difTestGetProductObj1StockCost);
    dImp->registerFunction("productSupplyVol", material, part,
			   timeVecFloat, difTestGetProductSupplyVol);
    dImp->registerFunction("productMinLotSize", material, part,
			   timeVecFloat, difTestGetProductMinLotSize);

    // Register all capacity attribute functions
    dImp->registerFunction("capacityUnitCost", material, part,
			   vectorFloat, difTestGetCapacityUnitCost);
    dImp->registerFunction("capacityConstrained", material, part,
			   vectorInt, difTestGetCapacityConstrained);
    dImp->registerFunction("capacityObj1ScrapCost", material, part,
			   timeVecFloat, difTestGetCapacityObj1ScrapCost);
    dImp->registerFunction("capacitySupplyVol", material, part,
			   timeVecFloat, difTestGetCapacitySupplyVol);
   // Register all functions to obtain demands
    dImp->registerFunction("committedDemands", demandType, demand,
		   orderedVecDemand, difTestGetCommittedDemands);
    dImp->registerFunction("newOpportunityDemands", demandType, demand,
		   orderedVecDemand, difTestGetNewOpportunityDemands);
    dImp->registerFunction("firmForecastDemands", demandType, demand,
			   orderedVecDemand, difTestGetFirmForecastDemands);
    dImp->registerFunction("riskForecastDemands", demandType, demand,
			   orderedVecDemand, difTestGetRiskForecastDemands);

    // Register all demand attribute functions
    dImp->registerFunction("demandGrossRev", demandType, demand,
			   vectorFloat, difTestGetDemandGrossRev);
    dImp->registerFunction("demandVol", demandType, demand,
			   timeVecFloat, difTestGetDemandVol);
    dImp->registerFunction("demandObj1CumShipReward", demandType, demand,
			   timeVecFloat, difTestGetDemandObj1CumShipReward);
    dImp->registerFunction("demandObj1ShipReward", demandType, demand,
			   timeVecFloat, difTestGetDemandObj1ShipReward);
    // jmt 6/27/95 need to have partTable and demandTable (similar to attrTable)
    // Register all functions to obtain parts
    dImp->registerFunction("rawParts", material, part,
			   orderedVecPart, difTestGetRawParts);
    dImp->registerFunction("productParts", material, part,
			   orderedVecPart, difTestGetProductParts);

    dImp->registerFunction("capacityParts", material, part,
			   orderedVecPart, difTestGetCapacityParts);

    // Register define bom globle function
    dImp->registerFunction("defineBom", material, part,
			   timeVecFloat, difTestDefineBom);

}


#ifdef NDEBUG
#undef NDEBUG
#endif


// Tests this class.  Only returns if all tests passed
// Expects data in testScenario1 to have come from
// /u/implode/wit/data/examples/diner12/wit.data, and expects
// testScenario2 to have been made by copying testScenario1
// testSc.
// Right now, the only scenario that works correctly with a 
// LgFrTestDataInterface is LgFrScenarioforCMRP.  Although the above 
// comment says that data should have come from a file, it is probably
// misleading (WCD, 9/15/94).  The data from the file (which has already
// been put into WIT) is probably ignored.  This should be changed
// to take LgFrScenarioforCMRP's for the 1st and 6th arguments.
//
// TRE - 9/19/94:  The scenario being passed in here has already been
// "fedInto", and therefore has already used the dif being tested.  What
// makes this more confusing is that the dif can be used differently 
// depending on which scenario uses it (CMRP or ATP).  Hence I'm
// moving some of the testing done here into the ipCMRP and ipATP context
// tests.
//     Specifically, contextTest will not add parts to WIT, nor will it
// test its defineBOM() method with witGetPartNBOMEntries().  It will also
// not worry about how many parts (demands) are in setOfParts() (setOfDemands()).
// 

void
difTestContextTest
  (LgFrScenario        		& testScenario1,
   LgFrLastProblemSolved       	& /* lps */,      // testScenario1's
   LgFrDataInterface	       	& ts1dif,	// testScenario1's
   LgFrScenario 	       	& testScenario1dup,
   LgFrDataInterface	       	& ts1dupDif)	// testScenario1dup's
{
  const float fltEps = .0001f;
  LgFrCalendar& calendar = testScenario1.calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  LgFrDataInterfaceImplementation* ts1difImp
      = ts1dif.dataInterfaceImplementation();
  LgFrDataInterfaceImplementation* ts1dupDifImp
      = ts1dupDif.dataInterfaceImplementation();

  // Test title method
  assert( ts1difImp->title() == "Test problem for LgFrDataInterfaceImplementation" );
  assert( ts1dupDifImp->title()=="Test problem for LgFrDataInterfaceImplementation" );
  assert( ts1dif.title() == "Test problem for LgFrDataInterfaceImplementation");
  assert( ts1dupDif.title() == "Test problem for LgFrDataInterfaceImplementation");

  // Register all functions
  registerDifTestImpFunctions(ts1difImp); 
  registerDifTestImpFunctions(ts1dupDifImp);

  // Test Get Part Methods
  LgFrDataInterfaceEssentialsFromScenario difEssen(&testScenario1);
  LgFrPartVector pv;
  std::string itemSubTypeStr = "material";  
  LgFrPartVector* rawPartsPtr
    = (LgFrPartVector*) ts1dif.partAttribute(
	pv, itemSubTypeStr, "rawParts", &difEssen);
  LgFrPartVector raw = *rawPartsPtr;
  assert( raw.size() == 1 );
  assert( raw[0].name() == "Raw" );

  LgFrPartVector* productPartsPtr
    = (LgFrPartVector*) ts1dif.partAttribute(
	pv, itemSubTypeStr, "productParts", &difEssen);
  LgFrPartVector product = *productPartsPtr;
  assert( product.size() == 1 );
  assert( product[0].name() == "Product" );

  LgFrPartVector* capacityPartsPtr
    = (LgFrPartVector*) ts1dif.partAttribute(
	pv, itemSubTypeStr, "capacityParts", &difEssen);
  LgFrPartVector capacity = *capacityPartsPtr;
  assert( capacity.size() == 1 );
  assert( capacity[0].name() == "Capacity" );

  // Test get raw material attributes functions
  {
  LgFrFloatVector* unitCostPtr
    = (LgFrFloatVector*) ts1dif.partAttribute(
	raw, itemSubTypeStr, "rawUnitCost", &difEssen);
  LgFrFloatVector unitCost = *unitCostPtr;
  assert( unitCost.size() == 1 );
  assert( fabs(unitCost[0] - 123.f) < fltEps );


  LgFrIntVector* constrainedPtr
      = (LgFrIntVector*) ts1dif.partAttribute(
	  raw, itemSubTypeStr, "rawConstrained", &difEssen);
  LgFrIntVector constrained = *constrainedPtr;
  assert( constrained.size() == 1 );
  assert( !constrained[0] );

  LgFrTimeVecFloatVector* obj1ScrapCostPtr
      = (LgFrTimeVecFloatVector*) ts1dif.partAttribute(
	  raw, itemSubTypeStr, "rawObj1ScrapCost", &difEssen);
  LgFrTimeVecFloatVector obj1ScrapCost = *obj1ScrapCostPtr;
  assert( obj1ScrapCost.size() == 1 );
  assert( 
     obj1ScrapCost[0] 
       == LgFrTimeVecFloat( nperiods, 2.34f ) );

  
  LgFrTimeVecFloatVector* obj1StockCostPtr
      = (LgFrTimeVecFloatVector*) ts1dif.partAttribute(
	  raw, itemSubTypeStr, "rawObj1StockCost", &difEssen);
  LgFrTimeVecFloatVector obj1StockCost = *obj1StockCostPtr;
  assert( obj1StockCost.size() == 1 );
  assert( 
     obj1StockCost[0] 
       == LgFrTimeVecFloat( nperiods, 3.f ) );


  LgFrTimeVecFloatVector* supplyVolPtr
      = (LgFrTimeVecFloatVector*) ts1dif.partAttribute(
	  raw, itemSubTypeStr, "rawSupplyVol", &difEssen);
  LgFrTimeVecFloatVector supplyVol = *supplyVolPtr;
  assert( supplyVol.size() == 1 );
  assert( supplyVol[0] 
	    == LgFrTimeVecFloat( nperiods, 5.f ) );

  delete unitCostPtr;
  delete constrainedPtr;
  delete obj1ScrapCostPtr;
  delete obj1StockCostPtr;
  delete supplyVolPtr;
}

  delete rawPartsPtr;


  // Test get product attributes global functions
  {
  LgFrTimeVecFloatVector* cycleTimePtr
      = (LgFrTimeVecFloatVector*) ts1dif.partAttribute(
	  product, itemSubTypeStr, "productCycleTime", &difEssen);
  LgFrTimeVecFloatVector cycleTime = *cycleTimePtr;
  assert( cycleTime.size() == 1 );
  assert( fabs(cycleTime[0][0] - 1.2f) < fltEps );

  LgFrFloatVector* unitCostPtr
      = (LgFrFloatVector*) ts1dif.partAttribute(
	  product, itemSubTypeStr, "productUnitCost", &difEssen);
  LgFrFloatVector unitCost = *unitCostPtr;
  assert( unitCost.size() == 1 );
  assert( fabs(unitCost[0] - 3.21f) < fltEps );


  LgFrFloatVector* procCostPtr
      = (LgFrFloatVector*) ts1dif.partAttribute(
	  product, itemSubTypeStr, "productProcCost", &difEssen);
  LgFrFloatVector procCost = *procCostPtr;
  assert( procCost.size() == 1 );
  assert( fabs(procCost[0] - 3.01f) < fltEps );

  LgFrIntVector* constrainedPtr
      = (LgFrIntVector*) ts1dif.partAttribute(
	  product, itemSubTypeStr, "productConstrained", &difEssen);
  LgFrIntVector constrained = *constrainedPtr;
  assert( constrained.size() == 1 );
  assert( constrained[0] );

  LgFrIntVector* yieldPtr
      = (LgFrIntVector*) ts1dif.partAttribute(
	  product, itemSubTypeStr, "productYield", &difEssen);
  LgFrIntVector yield = *yieldPtr;
  assert( yield.size() == 1 );
  assert( yield[0]==96 );

  LgFrTimeVecFloatVector* obj1ProdCostPtr
      = (LgFrTimeVecFloatVector*) ts1dif.partAttribute(
	  product, itemSubTypeStr, "obj1ProdCost", &difEssen);
  LgFrTimeVecFloatVector obj1ProdCost = *obj1ProdCostPtr;
  assert( obj1ProdCost.size() == 1 );
  assert( 
     obj1ProdCost[0] 
       == LgFrTimeVecFloat( nperiods, 3.4f ) );

  LgFrTimeVecFloatVector* obj1ScrapCostPtr
      = (LgFrTimeVecFloatVector*) ts1dif.partAttribute(
	  product, itemSubTypeStr, "productObj1ScrapCost", &difEssen);
  LgFrTimeVecFloatVector obj1ScrapCost = *obj1ScrapCostPtr;
  assert( obj1ScrapCost.size() == 1 );
  assert( 
     obj1ScrapCost[0]
       == LgFrTimeVecFloat( nperiods, 5.6f ) );

  LgFrTimeVecFloatVector* obj1StockCostPtr
      = (LgFrTimeVecFloatVector*) ts1dif.partAttribute(
	  product, itemSubTypeStr, "productObj1StockCost", &difEssen);
  LgFrTimeVecFloatVector obj1StockCost = *obj1StockCostPtr;
  assert( obj1StockCost.size() == 1 );
  assert( 
     obj1StockCost[0] 
       == LgFrTimeVecFloat( nperiods, 7.8f ) );

  LgFrTimeVecFloatVector* supplyVolPtr
      = (LgFrTimeVecFloatVector*) ts1dif.partAttribute(
	  product, itemSubTypeStr, "productSupplyVol", &difEssen);
  LgFrTimeVecFloatVector supplyVol = *supplyVolPtr;
  assert( supplyVol.size() == 1 );
  assert( supplyVol[0] 
	    == LgFrTimeVecFloat( nperiods, 9.0f ) );


  LgFrTimeVecFloatVector* minLotSizePtr
      = (LgFrTimeVecFloatVector*) ts1dif.partAttribute(
	  product, itemSubTypeStr, "productMinLotSize", &difEssen);
  LgFrTimeVecFloatVector minLotSize = *minLotSizePtr;
  assert( minLotSize.size() == 1 );
  assert( minLotSize[0]
	    == LgFrTimeVecFloat( nperiods, 1.0f ) );

  delete cycleTimePtr;
  delete procCostPtr;
  delete unitCostPtr;
  delete constrainedPtr;
  delete yieldPtr; 
  delete obj1ProdCostPtr;
  delete obj1ScrapCostPtr;
  delete obj1StockCostPtr;
  delete supplyVolPtr;
  delete minLotSizePtr;
  }

  delete productPartsPtr;


  // Test get capacity attributes global functions
  {
  LgFrFloatVector* unitCostPtr
      = (LgFrFloatVector*) ts1dif.partAttribute(
	  capacity, itemSubTypeStr, "capacityUnitCost", &difEssen);
  LgFrFloatVector unitCost = *unitCostPtr;
  assert( unitCost.size() == 1 );
  assert( fabs(unitCost[0] - 2.46f) < fltEps );

  LgFrIntVector* constrainedPtr
      = (LgFrIntVector*) ts1dif.partAttribute(
	  capacity, itemSubTypeStr, "capacityConstrained", &difEssen);
  LgFrIntVector constrained = *constrainedPtr;
  assert( constrained.size() == 1 );
  assert( constrained[0] );

  
  LgFrTimeVecFloatVector* obj1ScrapCostPtr
      = (LgFrTimeVecFloatVector*) ts1dif.partAttribute(
	  capacity, itemSubTypeStr, "capacityObj1ScrapCost", &difEssen);
  LgFrTimeVecFloatVector obj1ScrapCost = *obj1ScrapCostPtr;
  assert( obj1ScrapCost.size() == 1 );
  assert( 
     obj1ScrapCost[0] 
       == LgFrTimeVecFloat( nperiods, 78.f ) );

  LgFrTimeVecFloatVector* supplyVolPtr
      = (LgFrTimeVecFloatVector*) ts1dif.partAttribute(
	  capacity, itemSubTypeStr, "capacitySupplyVol", &difEssen);
  LgFrTimeVecFloatVector supplyVol = *supplyVolPtr;
  assert( supplyVol.size() == 1 );
  assert( supplyVol[0] 
	    == LgFrTimeVecFloat( nperiods, 90.f ) );

  delete unitCostPtr;
  delete constrainedPtr;
  delete obj1ScrapCostPtr;
  delete supplyVolPtr;
  }

  delete capacityPartsPtr;


  // Test for get demands global functions
  LgFrDemandVector dv;

  LgFrDemandVector* committedDemandPtr
    = (LgFrDemandVector*) ts1dif.demandAttribute(
	dv, "committedDemands", &difEssen );
  LgFrDemandVector committed = * committedDemandPtr; 
  assert( committed.size() == 1 );
  assert( committed[0].name() == "Committed" );


  LgFrDemandVector* newOpportunityDemandPtr
    = (LgFrDemandVector*) ts1dif.demandAttribute(
	dv, "newOpportunityDemands", &difEssen );
  LgFrDemandVector newOpportunity = * newOpportunityDemandPtr; 
  assert( newOpportunity.size() == 1 );
  assert( newOpportunity[0].name() == "NewOpportunity" );


  LgFrDemandVector* firmForecastDemandPtr
    = (LgFrDemandVector*) ts1dif.demandAttribute(
	dv, "firmForecastDemands", &difEssen );
  LgFrDemandVector firmForecast = * firmForecastDemandPtr;
  assert( firmForecast.size() == 1 );
  assert( firmForecast[0].name() == "FirmForecast" );

  LgFrDemandVector* riskForecastDemandPtr
    = (LgFrDemandVector*) ts1dif.demandAttribute(
	dv, "riskForecastDemands", &difEssen );
  LgFrDemandVector riskForecast = * riskForecastDemandPtr;
  assert( riskForecast.size() == 1 );
  assert( riskForecast[0].name() == "RiskForecast" );


  // Test get demand Attributes global functions
  {
  // Create a temporary set of demands
  LgFrSetOfDemands tempSod;
  tempSod.push_back( committed[0] );
  tempSod.push_back( newOpportunity[0] );
  tempSod.push_back( firmForecast[0] );
  tempSod.push_back( riskForecast[0] );


  LgFrFloatVector* grossRevPtr
   = (LgFrFloatVector*) ts1dif.demandAttribute(
       tempSod,"demandGrossRev", &difEssen);
  LgFrFloatVector grossRev = *grossRevPtr;
  assert( grossRev.size() == 4 );
  assert( fabs(grossRev[0] - 111.f ) < fltEps );
  assert( fabs(grossRev[1] - 222.f ) < fltEps );
  assert( fabs(grossRev[2] - 333.f ) < fltEps );
  assert( fabs(grossRev[3] - 444.f ) < fltEps );
  delete grossRevPtr;

  LgFrTimeVecFloatVector* demandVolPtr
      = (LgFrTimeVecFloatVector*) ts1dif.demandAttribute(
	  committed, "demandVol", &difEssen);
  LgFrTimeVecFloatVector demandVol = *demandVolPtr;
  assert( demandVol.size() == 1 );
  assert( demandVol[0] 
	    == LgFrTimeVecFloat( nperiods, 111.f ) );
  delete demandVolPtr;

  LgFrTimeVecFloatVector* obj1CumShipRewardPtr
      = (LgFrTimeVecFloatVector*) ts1dif.demandAttribute(
	  newOpportunity, "demandObj1CumShipReward", &difEssen);
  LgFrTimeVecFloatVector obj1CumShipReward = *obj1CumShipRewardPtr;
  assert(  obj1CumShipReward.size() == 1 );
  assert(  
    obj1CumShipReward[0] 
      == LgFrTimeVecFloat( nperiods, 222.f ) );
  delete obj1CumShipRewardPtr;

  LgFrTimeVecFloatVector* obj1ShipRewardPtr
      = (LgFrTimeVecFloatVector*) ts1dif.demandAttribute(
	  newOpportunity, "demandObj1ShipReward", &difEssen);
  LgFrTimeVecFloatVector obj1ShipReward = *obj1ShipRewardPtr;
  assert(  obj1ShipReward.size() == 1 );
  assert(  
    obj1ShipReward[0] 
      == LgFrTimeVecFloat( nperiods, 333.f ) );
  delete obj1ShipRewardPtr;

}

  delete committedDemandPtr;
  delete newOpportunityDemandPtr;
  delete firmForecastDemandPtr;
  delete riskForecastDemandPtr;
  
  // Test define BOM method
//  witAddPart( lps.witRun(), "Raw",     WitRAW );
//  witAddPart( lps.witRun(), "Product", WitPRODUCT );
//  witAddPart( lps.witRun(), "Capacity",WitCAPACITY );
//  ts1dif.defineBom( lps.witRun() );
//  int nBomEntries;
//  witGetPartNBomEntries( lps.witRun(), "Product", &nBomEntries );
//  assert ( nBomEntries == 2 );
}
