#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.
#include <math.h>
#include <iostream>


#include <scenario/src/difWitR.h>
#include <scenario/src/witRunVn.h>
#include <scenario/src/calendar.h>
#include <scenario/src/difEssSc.h>
#include <scenario/src/customer.h>


const std::string 
LgFrDataInterfaceImplementationFromWitRun::defaultRiskForecast(
                                     "RiskForecast.*");
const std::string 
LgFrDataInterfaceImplementationFromWitRun::defaultFirmForecast(
#if CUSTOMER_NUMBER == MOOGS_NUMBER
                                     "MPS.*"); 
#else							       
                                     "FirmForecast.*");
#endif
const std::string 
LgFrDataInterfaceImplementationFromWitRun::defaultNewOps(
                                     "NewOps.*");
const std::string 
LgFrDataInterfaceImplementationFromWitRun::defaultCommitted(
                                     "Committed.*");

//-------------------------------------------------------------------
// Set problem title (private member function, not allowed)
//-------------------------------------------------------------------
void
LgFrDataInterfaceImplementationFromWitRun::title(
     std::string t 
    )
{
    t = makeTitle();
    LgFrDataInterfaceImplementation::title(t);
}

//-------------------------------------------------------------------
// Return a problem title
//-------------------------------------------------------------------
std::string
LgFrDataInterfaceImplementationFromWitRun::title(
    const LgFrDataInterfaceEssentials * difEssenPtr)
const
{   
    std::string t = LgFrDataInterfaceImplementation::title(difEssenPtr);
    if ( t == "Unknown" )  // Make a title from dataset
	{
	    t = makeTitle(difEssenPtr);
	    ((LgFrDataInterfaceImplementation*)this)
		->LgFrDataInterfaceImplementation::title(t);
	}
    return t;
}

//-------------------------------------------------------------------
// Make a problem title
//-------------------------------------------------------------------
std::string
LgFrDataInterfaceImplementationFromWitRun::makeTitle(
    const LgFrDataInterfaceEssentials * difEssenPtr)
     const
{
  char * titleWR;
  
  witGetTitle(constWitRun(difEssenPtr), &titleWR);
  std::string titleName;
  sce::strip( titleName );
  portableFree(titleWR);
  return titleName;
}

//-------------------------------------------------------------------
//  
// Global functions to obtain the defined parts and capacities.
//
//-------------------------------------------------------------------

// Return: a LgFrPartVector object (vector of parts in general)
// used by difWitRunGetRawPart, difWitRunGetProductParts, difWitRunGetcapacityParts 
// to get different kinds of parts from WIT.
void *
difWitRunGetParts(
    const LgFrDataInterfaceImplementation *dImp, 
    const LgFrDataInterfaceEssentials *difEssenPtr,
    witAttr ma, 
    LgFrError& status)
{
  witAttr category;
  char * * partList;
  int listLen, i;

  LgFrDataInterfaceImplementationFromWitRun * dImpWR = (
      LgFrDataInterfaceImplementationFromWitRun * ) dImp;

  witGetParts(dImpWR->constWitRun(difEssenPtr), &listLen, &partList);
  LgFrPartVector * retVal = new LgFrPartVector;
  retVal->reserve( listLen );

  for (i = 0 ; i < listLen ; i++ )
    {
      witGetPartCategory (dImpWR->constWitRun(difEssenPtr), partList[i], &category);
      std::string pn(category);
//      if( pn.compareTo( pn(ma) ) == 0 )
      if( sce::matchRegexp( pn, ma) )
        {
          LgFrPart part = difEssenPtr->createAndInsertPart(partList[i]);
          retVal->push_back(part);
        }
      portableFree(partList[i]);
    }
  portableFree(partList);
  return retVal;
}

// difWitRunGetRawPart global function (registering in the attribute table).
// Get raw parts from WIT and insert them into set_of_part
// Return: a LgFrPartVector object (vector of raw parts).
void *
difWitRunGetRawParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  return difWitRunGetParts(dImp, difEssenPtr, WitRAW, status);
}

// difWitRunGetProductPart global function (registering in the attribute table).
// Get product parts from WIT and insert it into set_of_part
// Return: a LgFrPartVector object (vector of producible parts)
void*
difWitRunGetProductParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  return difWitRunGetParts(dImp, difEssenPtr, WitPRODUCT, status);
}

// difWitRunGetCapacityPart global function (registering in the attribute table).
// Get capacity parts from WIT and insert it into set_of_part
// Return: a LgFrPartVector object (vector of capacities).
void*
difWitRunGetCapacityParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  return difWitRunGetParts(dImp, difEssenPtr, WitCAPACITY, status);
}

//--------------------------------------------------------------------
//  
//   Global functions to obtain part attributes
//
//-------------------------------------------------------------------


  
// difWitRunGetPartProcCost global function (registered in the attribute table).
// It returns a LgFrFloatVector object.
void *
difWitRunGetPartProcCost(
    const LgFrDataInterfaceImplementation * difImp,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
  return difWitRunGetPartUnitCost(difImp, itemPtrOrdVecPtr, status);
}

// difWitRunGetPartConstrained global function (registered in the attribute table).
// It returns a LgFrIntVector object
void *
difWitRunGetPartConstrained(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)               
{
  LgFrDataInterfaceImplementationFromWitRun * dImpWR
      = (LgFrDataInterfaceImplementationFromWitRun * ) difImp;

  LgFrItemPtrVector pv = *itemPtrOrdVecPtr;
  LgFrIntVector& retVal = *(new LgFrIntVector);
  retVal.reserve( pv.size() );   

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    retVal.push_back( true );
  }
  return &retVal; 
} 

// difWitRunGetPartProcLeadTime global function (registered in the attribute table).
// It returns a LgFrTimeVecFloatVector object.
void *
difWitRunGetPartProcLeadTime(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{

  float * cycleTime;  
  witAttr partCategory;
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr;
  LgFrTimeVecFloatVector& retVal = 
      *(new LgFrTimeVecFloatVector);
  retVal.reserve( pv.size() );   
  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int calNP(calendar.nPeriods());  // # periods in scenario calendar
  int witNP;                       // # periods in wit data file

  LgFrDataInterfaceImplementationFromWitRun * dImpWR
      = (LgFrDataInterfaceImplementationFromWitRun * ) difImp;

  witGetNPeriods( dImpWR->constWitRun(difEssenPtr), &witNP);
  LgFrTimeVecFloat tv(calNP);

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) 
  {
      witGetPartCategory(dImpWR->constWitRun(difEssenPtr), pv[i]->name().c_str(), &partCategory);
      if ( partCategory == WitRAW ) 
          retVal.push_back( 0.0 );
      else if (partCategory == WitPRODUCT) 
      {
	      witGetPartCycleTime(dImpWR->constWitRun(difEssenPtr), pv[i]->name().c_str(), 
			          &cycleTime);
	      size_t j = 0; // Pulled out of the for below by RW2STL
	      for(j = 0; j < size_tMin(calNP, witNP); j++ )
          {
              tv[j] = cycleTime[j];
	      }
	      retVal.push_back( tv );
	      portableFree(cycleTime);
      }
  }
  return &retVal; 
}      

// Global function to get some part attributes, such as,
// Obj1ScrapCost, Obj1StockCost, and SupplyVol
// (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difWitRunGetPartAttrTimeVecFloat(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    apiFuncGetPartAttrTimeVecFloat apiFunc,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{

  LgFrItemPtrVector pv = *itemPtrOrdVecPtr;       
  LgFrTimeVecFloatVector& retVal = *(new LgFrTimeVecFloatVector);
  retVal.reserve ( pv.size() );
  float * attrVal;
  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int calNP(calendar.nPeriods());  // # periods in scenario calendar
  int witNP;                       // # periods in wit data file

  LgFrDataInterfaceImplementationFromWitRun * dImpWR 
      = (LgFrDataInterfaceImplementationFromWitRun * ) dImp; 

  witGetNPeriods( dImpWR->constWitRun(difEssenPtr), &witNP);
  LgFrTimeVecFloat tv(calNP);

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    apiFunc(dImpWR->constWitRun(difEssenPtr), pv[i]->name().c_str(), &attrVal);
    size_t j = 0; // Pulled out of the for below by RW2STL
    for(j = 0; j < size_tMin(calNP, witNP); j++ )
        tv[j] = attrVal[j];

    retVal.push_back( tv );
    portableFree(attrVal);
  }
  return &retVal;
}

// difWitRunGetPartObj1ScrapCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difWitRunGetPartObj1ScrapCost(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
  return difWitRunGetPartAttrTimeVecFloat
         (difImp, itemPtrOrdVecPtr, witGetPartObj1ScrapCost, 
	  status, difEssenPtr);
}

// difWitRunGetPartObj1StockCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difWitRunGetPartObj1StockCost(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
  return difWitRunGetPartAttrTimeVecFloat
         (difImp, itemPtrOrdVecPtr, witGetPartObj1StockCost, 
	  status, difEssenPtr);
}

// difWitRunGetPartSupplyVol global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difWitRunGetPartSupplyVol(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
  return difWitRunGetPartAttrTimeVecFloat 
         (difImp, itemPtrOrdVecPtr, witGetPartSupplyVol, 
	  status, difEssenPtr);
}

//--------------------------------------------------------------------
//  
//   Methods to obtain producible part attributes
//
//-------------------------------------------------------------------

// difWitRunGetProductYield global function (registered in the attribute table).
// Return: a LgFrIntVector object
void *
difWitRunGetProductYield(
    const LgFrDataInterfaceImplementation * dImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{

  LgFrDataInterfaceImplementationFromWitRun * dImpWR 
      = (LgFrDataInterfaceImplementationFromWitRun * ) dImp; 

  LgFrItemPtrVector pv = *itemPtrOrdVecPtr;
  LgFrIntVector& retVal = *(new LgFrIntVector);
  retVal.reserve ( pv.size() );
  int * yield;
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    witGetPartYield(dImpWR->constWitRun(difEssenPtr), pv[i]->name().c_str(), &yield);
    retVal.push_back( yield[0] );
    portableFree(yield);
  }
  return &retVal;
}

// difWitRunGetProductYieldFloat global function
// (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object
void *
difWitRunGetProductYieldFloat(
    const LgFrDataInterfaceImplementation * dImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{

  LgFrDataInterfaceImplementationFromWitRun * dImpWR 
      = (LgFrDataInterfaceImplementationFromWitRun * ) dImp;

  LgFrItemPtrVector pv = *itemPtrOrdVecPtr;
  LgFrTimeVecFloatVector& retVal = *(new LgFrTimeVecFloatVector);
  retVal.reserve ( pv.size() );
  int * yield;
  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int calNP(calendar.nPeriods());  // # periods in scenario calendar

  int witNP;                       // # periods in wit data file
  witGetNPeriods( dImpWR->constWitRun(difEssenPtr), &witNP);
  
  LgFrTimeVecFloat tv(calNP);

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    witGetPartYield(dImpWR->constWitRun(difEssenPtr), pv[i]->name().c_str(), &yield);
    size_t j = 0; // Pulled out of the for below by RW2STL
    for(j = 0; j < size_tMin(calNP, witNP); j++ ) tv[j] = (float) yield[j];
    retVal.push_back( tv );
    portableFree(yield);
  }
  return &retVal;
}

// difWitRunGetProductCycleTime global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difWitRunGetProductCycleTime(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
  return difWitRunGetPartAttrTimeVecFloat
         (difImp, itemPtrOrdVecPtr, witGetPartCycleTime, 
	  status, difEssenPtr); 
}

// difWitRunGetProductObj1ProdCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difWitRunGetProductObj1ProdCost(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
  return difWitRunGetPartAttrTimeVecFloat
         (difImp, itemPtrOrdVecPtr, witGetPartObj1ProdCost, 
	  status, difEssenPtr); 
}

void*
difWitRunGetProductMinLotSize(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
  return difWitRunGetPartAttrTimeVecFloat
         (difImp, itemPtrOrdVecPtr, witGetPartMinLotSize, 
	  status, difEssenPtr); 
}

void*
difWitRunGetProductIncLotSize(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
  return difWitRunGetPartAttrTimeVecFloat
         (difImp, itemPtrOrdVecPtr, witGetPartIncLotSize, 
	  status, difEssenPtr); 
}

//--------------------------------------------------------------------
//  
//   Methods to obtain demands
//
//-------------------------------------------------------------------

// return true if demandName matches compareExpression
bool
LgFrDataInterfaceImplementationFromWitRun::isADemand(
  std::string /* demandedPartName */,
  std::string demandName,
  std::string compareExpression) const
{
  if ( sce::matchRegexp( demandName, compareExpression ) )
    return true;
  else
    return false;
}

// return true if demandName is a NewOp
bool
LgFrDataInterfaceImplementationFromWitRun::isANewOpDemand(
  std::string demandedPartName,
  std::string demandName) const
{
  bool retVal = isADemand(demandedPartName,demandName,newOpsRE_);
  if ( retVal) {
    assert( ! isADemand(demandedPartName, demandName, firmForecastRE_));
    assert( ! isADemand(demandedPartName, demandName, riskForecastRE_));
  }
  return retVal;
}

// return true if demandName is a FirmForecast
bool
LgFrDataInterfaceImplementationFromWitRun::isAFirmForecastDemand(
  std::string demandedPartName,
  std::string demandName) const
{
  bool retVal = isADemand(demandedPartName,demandName,firmForecastRE_);
  if ( retVal) {
    assert( ! isADemand(demandedPartName, demandName, newOpsRE_));
    assert( ! isADemand(demandedPartName, demandName, riskForecastRE_));
  }
  return retVal;
}

// return true if demandName is a RiskForecast
bool
LgFrDataInterfaceImplementationFromWitRun::isARiskForecastDemand(
  std::string demandedPartName,
  std::string demandName) const
{
  bool retVal = isADemand(demandedPartName,demandName,riskForecastRE_);
  if ( retVal) {
    assert( ! isADemand(demandedPartName, demandName, newOpsRE_));
    assert( ! isADemand(demandedPartName, demandName, firmForecastRE_));
  }
  return retVal;
}

// return true if demandName is a Committed
bool
LgFrDataInterfaceImplementationFromWitRun::isACommittedDemand(
  std::string demandedPartName,
  std::string demandName) const
{
  bool retVal = false;
  if (
    (! isANewOpDemand(demandedPartName, demandName)) &&
    (! isAFirmForecastDemand(demandedPartName, demandName)) &&
    (! isARiskForecastDemand(demandedPartName, demandName)))
     return true;
  return retVal;
}

// demand in general
// Return: a LgFrDemandVector object (vector of demands in general)
// used by commited, newOps, firmForecast, riskForecast to get different
// kinds of demands from WIT.
void*
difWitRunGetDemands(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrDataInterfaceEssentials * difEssenPtr,
    bool
     ( LgFrDataInterfaceImplementationFromWitRun::*isA)
        (std::string, std::string)const,
    LgFrError & status
   )                                          
{

    char * * demandList;
    char * * partList;
    int lenDemandList, listLen;

    LgFrDataInterfaceImplementationFromWitRun * dImpWR 
	= (LgFrDataInterfaceImplementationFromWitRun * ) dImp;

    LgFrDemandVector& retVal = *(new LgFrDemandVector);
    witGetParts(dImpWR->constWitRun(difEssenPtr), &listLen, &partList);

    size_t j = 0; // Pulled out of the for below by RW2STL
    for (j = 0 ; j < listLen ; j++ )
      {
        witGetPartDemands(dImpWR->constWitRun(difEssenPtr), partList[j],
	                  &lenDemandList, &demandList);
        size_t i = 0; // Pulled out of the for below by RW2STL
        for (i = 0 ; i < lenDemandList ; i++ )
          {
	    std::string dn(demandList[i]);
            if ((dImpWR->*isA)(std::string(partList[j]),dn))
	      {     
                LgFrDemand demand = 
		  difEssenPtr->createAndInsertDemand(dn, partList[j]);
                retVal.push_back(demand);
	      } 
	      portableFree(demandList[i]);
	  }
	portableFree(demandList);
	portableFree(partList[j]); 
      }
    portableFree(partList);
    return &retVal;
} 

// difWitRunGetCommittedDemands global function (registering in the attribute table).
// Get committed demands from WIT and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of committed demands).
void *
difWitRunGetCommittedDemands(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  return difWitRunGetDemands(
      dImp,
      difEssenPtr, 
      & LgFrDataInterfaceImplementationFromWitRun::isACommittedDemand,
      status);
}

// difWitRunGetNewOpportunityDemands global function (registering in the attribute table).
// Get new opportunity demands from WIT and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of new opportunity demands).
void *
difWitRunGetNewOpportunityDemands(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  return difWitRunGetDemands(
      dImp,
      difEssenPtr, 
      & LgFrDataInterfaceImplementationFromWitRun::isANewOpDemand,
      status);
}

// difWitRunGetFirmForecastDemands global function (registering in the attribute table).
// Get firm forcast demands from WIT and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of firm forcast demands).
void *
difWitRunGetFirmForecastDemands(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  return difWitRunGetDemands(
      dImp,
      difEssenPtr, 
      & LgFrDataInterfaceImplementationFromWitRun::isAFirmForecastDemand,
      status);
}

// difWitRunGetRiskForecastDemands global function (registering in the attribute table).
// Get risk forcast demands from WIT and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of risk forcast demands).
void *
difWitRunGetRiskForecastDemands(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  return difWitRunGetDemands(
      dImp,
      difEssenPtr, 
      & LgFrDataInterfaceImplementationFromWitRun::isARiskForecastDemand,
      status);
}

// -------------------- ---------------------------------------------------
// Golbal functions to obtain demands attributes
// ------------------------------------------------------------------------



// Global function to get some demand attributes, such as,
// demandVol, getObj1CumShipReward, getObj1ShipReward
// (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difWitRunGetDemandAttrTimeVecFloat(
    const LgFrDataInterfaceImplementation * dImp,
      const LgFrItemPtrVector* itemPtrOrdVecPtr,
      apiFuncGetDemandAttrTimeVecFloat  apiFunc,
             LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{

  LgFrDataInterfaceImplementationFromWitRun * dImpWR 
      = (LgFrDataInterfaceImplementationFromWitRun * ) dImp; 

  LgFrItemPtrVector dv = *itemPtrOrdVecPtr;       
  LgFrTimeVecFloatVector& retVal = 
      *(new LgFrTimeVecFloatVector);
  retVal.reserve ( dv.size() );      
  float * attrVal;
  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int calNp(calendar.nPeriods());  // # periods in scenario calendar
  int witNp;                       // # periods in wit data file

  LgFrTimeVecFloat tv(calNp);
  witGetNPeriods(dImpWR->constWitRun(difEssenPtr), &witNp);
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < dv.size(); i++) 
  {
    apiFunc(dImpWR->constWitRun(difEssenPtr), 
	    ((LgFrDemand*)dv[i])->partPtr()->name().c_str(),
	    dv[i]->name().c_str(), &attrVal);
    size_t j = 0; // Pulled out of the for below by RW2STL
    for(j = 0; j < size_tMin(calNp, witNp); j++)
       tv[j] = attrVal[j];
    retVal.push_back( tv ); 
    portableFree(attrVal);
  }
  return &retVal;
}

// Global function to get some demand attributes, such as,
// demandPriority
// (registered in the attribute table).

// Should return a LgFrVectorTimeVecInt object, since LgFrTimeVecInt
// has not yet been implemented, now,
// simply returns a LgFrTimeVecFloatVector object.
void *
difWitRunGetDemandAttrTimeVecInt(
    const LgFrDataInterfaceImplementation * dImp,
      const LgFrItemPtrVector* itemPtrOrdVecPtr,
      apiFuncGetDemandAttrTimeVecInt apiFunc,
             LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{

  LgFrDataInterfaceImplementationFromWitRun * dImpWR 
      = (LgFrDataInterfaceImplementationFromWitRun * ) dImp; 

  LgFrItemPtrVector dv = *itemPtrOrdVecPtr;       
  LgFrTimeVecFloatVector& retVal = 
      *(new LgFrTimeVecFloatVector);
  retVal.reserve ( dv.size() );      
  int * attrVal;
  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int calNp(calendar.nPeriods());  // # periods in scenario calendar
  int witNp;                       // # periods in wit data file

  LgFrTimeVecFloat tv(calNp);
  witGetNPeriods(dImpWR->constWitRun(difEssenPtr), &witNp);
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < dv.size(); i++) 
  {
    apiFunc(dImpWR->constWitRun(difEssenPtr), 
	        ((LgFrDemand*)dv[i])->partPtr()->name().c_str(),
	        dv[i]->name().c_str(),
            &attrVal);
    size_t j = 0; // Pulled out of the for below by RW2STL
    for(j = 0; j < size_tMin(calNp, witNp); j++)
       tv[j] = (float)attrVal[j];
    retVal.push_back( tv ); 
    portableFree(attrVal);
  }
  return &retVal;
}

// difWitRunGetDemandVol global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difWitRunGetDemandVol(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
  return difWitRunGetDemandAttrTimeVecFloat
         (difImp, itemPtrOrdVecPtr, witGetDemandDemandVol,
	  status, difEssenPtr); 
}

// difWitRunGetDemanObj1CumShipReward global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difWitRunGetDemandObj1CumShipReward(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
  return difWitRunGetDemandAttrTimeVecFloat
         (difImp, itemPtrOrdVecPtr, witGetDemandObj1CumShipReward,
	  status, difEssenPtr); 
}

// difWitRunGetDemanObj1ShipReward global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difWitRunGetDemandObj1ShipReward(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
  return difWitRunGetDemandAttrTimeVecFloat
         (difImp, itemPtrOrdVecPtr, witGetDemandObj1ShipReward,
	  status, difEssenPtr); 
}

// difWitRunGetDemanPriority global function (registered in the attribute table).
// Return: a LgFrVectorTimeVecInt object.
//
// Note that priority should be a TimeVecInt, and not  TimeVecFloat.
// should return LgFrVectorTimeVecInt jmt 7/21/95, need to implement TimeVectInt class.
// Now, simply returns a LgFrTimeVecFloatVector object.

void *
difWitRunGetDemandPriority(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
  return difWitRunGetDemandAttrTimeVecInt
         (difImp, itemPtrOrdVecPtr, witGetDemandPriority,
	  status, difEssenPtr); 
}

//--------------------------------------------------------------------
//  
//   Global function to define BOM
//
//-------------------------------------------------------------------

void*
difWitRunDefineBom(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector*, // itemPtrOrdVecPtr,
    LgFrError &, // status, 
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
   int listLen,i,j;
   char * * partList;
   int nBomEntries;
   witAttr category;
   char * consumedPart;
   float usageRate,usageTime;
   int  earliestPeriod;
   int fallout, latestPeriod;
   witBoolean  mandEC;
   int nSubBom;
   float * obj1SubCost;
   float  obj2SubPenalty;

   const LgFrCalendar& calendar = difEssenPtr->calendar();
   int calNP(calendar.nPeriods());  // # periods in scenario calendar

   LgFrDataInterfaceImplementationFromWitRun * dImpWR 
     = (LgFrDataInterfaceImplementationFromWitRun * ) difImp; 

   WitRun* eater = 
     ((LgFrDataInterfaceEssentialsFromScenario*)difEssenPtr)
     ->mutableWitRun();

   witGetParts (dImpWR->constWitRun(difEssenPtr), &listLen, &partList);
   for (i= 0; i < listLen ; i++)
     {
       witGetPartCategory(dImpWR->constWitRun(difEssenPtr), partList[i], &category);
       if(category == WitPRODUCT ) 
         {
           witGetPartNBomEntries(dImpWR->constWitRun(difEssenPtr), partList[i], &nBomEntries);
           for (j = 0 ; j < nBomEntries ; j++)
             {
               witGetBomEntryConsumedPart(dImpWR->constWitRun(difEssenPtr),partList[i],j,
                                          &consumedPart);
               witAddBomEntry(eater, partList[i],consumedPart);
               witGetBomEntryUsageRate(dImpWR->constWitRun(difEssenPtr),partList[i],j,
                                          &usageRate);
               witGetBomEntryEarliestPeriod(dImpWR->constWitRun(difEssenPtr),partList[i],j,
                                          &earliestPeriod);
               witGetBomEntryFallout(dImpWR->constWitRun(difEssenPtr),partList[i],j,
                                          &fallout);
               witGetBomEntryLatestPeriod(dImpWR->constWitRun(difEssenPtr),partList[i],j,
                                          &latestPeriod);
               witGetBomEntryMandEC(dImpWR->constWitRun(difEssenPtr),partList[i],j,
                                          &mandEC);
               witGetBomEntryUsageTime(dImpWR->constWitRun(difEssenPtr),partList[i],j,
                                          &usageTime);
               witSetBomEntryUsageRate(eater, partList[i], 
                                j, usageRate);
               witSetBomEntryEarliestPeriod(eater, partList[i], 
                                j, size_tMin(calNP-1,earliestPeriod) );
               witSetBomEntryFallout(eater, partList[i], 
                                j, fallout);
               witSetBomEntryLatestPeriod(eater, partList[i], 
                                j, size_tMin(calNP-1,latestPeriod) );
               witSetBomEntryMandEC(eater, partList[i], 
                                j, mandEC);
               witSetBomEntryUsageTime(eater, partList[i], 
                                j, usageTime);
               witGetBomEntryNSubsBomEntries(dImpWR->constWitRun(difEssenPtr), partList[i], j,
                                             &nSubBom);
               size_t k = 0; // Pulled out of the for below by RW2STL
               for (k = 0; k < nSubBom ; k++)
                 {
                   
                   witAddSubsBomEntry(eater, partList[i], j, consumedPart);
                   witGetSubsBomEntryUsageRate(dImpWR->constWitRun(difEssenPtr), partList[i],
                                               j, k, &usageRate);
                   witGetSubsBomEntryEarliestPeriod(dImpWR->constWitRun(difEssenPtr), partList[i],
                                               j, k, &earliestPeriod);
                   witGetSubsBomEntryFallout(dImpWR->constWitRun(difEssenPtr), partList[i],
                                               j, k, &fallout);
                   witGetSubsBomEntryLatestPeriod(dImpWR->constWitRun(difEssenPtr), partList[i],
                                               j, k, &latestPeriod);
                   witGetSubsBomEntryObj1SubCost(dImpWR->constWitRun(difEssenPtr), partList[i], j,
                                               k, &obj1SubCost);
                   witSetSubsBomEntryUsageRate(eater, partList[i], j,
                                               k, usageRate);
                   witSetSubsBomEntryEarliestPeriod(eater, partList[i], j,
                                             k, 
                                             size_tMin(calNP-1,earliestPeriod));
                   witSetSubsBomEntryFallout(eater, partList[i], j,
                                               k, fallout);
                   witSetSubsBomEntryLatestPeriod(eater, partList[i], j,
                                             k, 
                                             size_tMin(calNP-1,latestPeriod));
                   witSetSubsBomEntryObj1SubCost(eater, partList[i], j,
                                               k, obj1SubCost);
                 }
                                        
                 portableFree(consumedPart);                       

             }
         }
       portableFree(partList[i]);
     }
   portableFree(partList);
   return 0;
}

// -----------------------------------------------------------------------
//  register functions
// -----------------------------------------------------------------------

void
registerWitRunDifImpFunctions(
    LgFrDataInterfaceImplementation * dImpWR) 
{
// Globle function for raw parts
// Register difWitRun global functions for
//   unitcost, procCost, obj1ScrapCost, obj1StockCost, and supplyVol.

dImpWR->registerFunction("rawUnitCost", material, part,
                            vectorFloat, difWitRunGetPartUnitCost);
dImpWR->registerFunction("rawProcCost", material, part,
                            vectorFloat, difWitRunGetPartProcCost);
dImpWR->registerFunction("rawConstrained", material, part,
                            vectorInt, difWitRunGetPartConstrained);
dImpWR->registerFunction("rawProcLeadTime", material, part,
                            timeVecFloat, difWitRunGetPartProcLeadTime);
dImpWR->registerFunction("rawObj1ScrapCost", material, part,
                            timeVecFloat, difWitRunGetPartObj1ScrapCost);
dImpWR->registerFunction("rawObj1StockCost", material, part,
                            timeVecFloat, difWitRunGetPartObj1StockCost);
dImpWR->registerFunction("rawSupplyVol", material, part,
                            timeVecFloat, difWitRunGetPartSupplyVol);


// Global functions for product parts.
// Register difWitRun global functions for
//   unitcost, procCost, constrained, yiels, procLeadTime, cycleTime, 
//   obj1ProdCost, obj1ScrapCost, obj1StockCost, supplyVol, and minLotSize. 

dImpWR->registerFunction("productUnitCost", material, part,
                            vectorFloat, difWitRunGetPartUnitCost);
dImpWR->registerFunction("productProcCost", material, part,
                            vectorFloat, difWitRunGetPartProcCost);
dImpWR->registerFunction("productConstrained", material, part,
                            vectorInt, difWitRunGetPartConstrained);
dImpWR->registerFunction("productYield", material, part,
                            vectorInt, difWitRunGetProductYield);
dImpWR->registerFunction("productYieldFloat", material, part,
                            timeVecFloat, difWitRunGetProductYieldFloat);
dImpWR->registerFunction("productProcLeadTime", material, part,
                            timeVecFloat, difWitRunGetPartProcLeadTime);
dImpWR->registerFunction("productCycleTime", material, part,
                           timeVecFloat , difWitRunGetProductCycleTime);
dImpWR->registerFunction("productObj1ProdCost", material, part,
                            timeVecFloat, difWitRunGetProductObj1ProdCost);
dImpWR->registerFunction("productObj1ScrapCost", material, part,
                            timeVecFloat, difWitRunGetPartObj1ScrapCost);
dImpWR->registerFunction("productObj1StockCost", material, part,
                            timeVecFloat, difWitRunGetPartObj1StockCost);
dImpWR->registerFunction("productSupplyVol", material, part,
                            timeVecFloat, difWitRunGetPartSupplyVol);
dImpWR->registerFunction("productMinLotSize", material, part,
                            timeVecFloat, difWitRunGetProductMinLotSize);
dImpWR->registerFunction("productIncLotSize", material, part,
                            timeVecFloat, difWitRunGetProductIncLotSize);


// Global functions for capacity parts.
// Register difWitRun global functions for
//   unitcost, constrained, obj1ScrapCost, and supplyVol 

dImpWR->registerFunction("capacityUnitCost", material, part,
                            vectorFloat, difWitRunGetPartUnitCost);
dImpWR->registerFunction("capacityConstrained", material, part,
                            vectorInt, difWitRunGetPartConstrained);
dImpWR->registerFunction("capacityObj1ScrapCost", material, part,
                            timeVecFloat, difWitRunGetPartObj1ScrapCost);
dImpWR->registerFunction("capacitySupplyVol", material, part,
                            timeVecFloat, difWitRunGetPartSupplyVol);


// Global functions for demands
// Register difWitRun global functions for
//  grossRev, demandVol, Obj1CumShipReward, priority, and obj1ShipReward.

dImpWR->registerFunction("demandGrossRev", demandType, demand,
                            vectorFloat, difWitRunGetDemandGrossRev);
dImpWR->registerFunction("demandVol", demandType, demand,
                            timeVecFloat, difWitRunGetDemandVol);
dImpWR->registerFunction("demandObj1CumShipReward", demandType, demand,
                            timeVecFloat, difWitRunGetDemandObj1CumShipReward);
dImpWR->registerFunction("demandObj1ShipReward", demandType, demand,
                            timeVecFloat, difWitRunGetDemandObj1ShipReward);
dImpWR->registerFunction("demandPriority", demandType, demand,
                            timeVecFloat,difWitRunGetDemandPriority );


// Parts table, register globle functions from difWitR, 7/19/95 DW 
dImpWR->registerFunction("rawParts", material, part,
			   orderedVecPart, difWitRunGetRawParts);
dImpWR->registerFunction("productParts", material, part,
			   orderedVecPart, difWitRunGetProductParts);
dImpWR->registerFunction("capacityParts", material, part,
			   orderedVecPart, difWitRunGetCapacityParts);

// using globle function from difWitR, 7/19/95 DW
dImpWR->registerFunction("committedDemands",demandType , demand,
			   orderedVecDemand, difWitRunGetCommittedDemands);
dImpWR->registerFunction("newOpportunityDemands", demandType, demand,
			   orderedVecDemand, difWitRunGetNewOpportunityDemands);
dImpWR->registerFunction("firmForecastDemands", demandType, demand,
			   orderedVecDemand, difWitRunGetFirmForecastDemands);
dImpWR->registerFunction("riskForecastDemands", demandType, demand,
			   orderedVecDemand, difWitRunGetRiskForecastDemands);
// Register define bom globle function
dImpWR->registerFunction("defineBom", material, part,
                            timeVecFloat, difWitRunDefineBom);

}

LgFrDataInterfaceImplementation * createWitRunDifImp(WitRun * wr)
{
   return new LgFrDataInterfaceImplementationFromWitRun(
                       wr,
                       "RiskForecast.*",
                       "FirmForecast.*",
                       "NewOps.*");
} 

// Free up any resources
void
LgFrDataInterfaceImplementationFromWitRun::deleteExtraResources()
{
  // Nothing to do here in base class
}



// -----------------------------------------------------------------------
//  standard methods
// -----------------------------------------------------------------------


// Return witRun
WitRun *  
LgFrDataInterfaceImplementationFromWitRun::constWitRun(
const LgFrDataInterfaceEssentials* difEssenPtr)
const
{
  LgFrDataInterfaceImplementationFromWitRun * const localThis =
        ( LgFrDataInterfaceImplementationFromWitRun * const ) this;
  if ( !validWitRun() ) localThis->witRunVeneerPtr_ = new LgFrWitRunVeneer;
  return witRunVeneerPtr_->constWitRun();
}

WitRun *  
LgFrDataInterfaceImplementationFromWitRun::mutableWitRun(
    const LgFrDataInterfaceEssentials*)
{
  if ( !validWitRun() ) witRunVeneerPtr_ = new LgFrWitRunVeneer;
  return witRunVeneerPtr_->mutableWitRun();
}

bool
LgFrDataInterfaceImplementationFromWitRun::validWitRun()
const
{
  return witRunVeneerPtr_!=0;
}

void
LgFrDataInterfaceImplementationFromWitRun::deleteWitRun()
{
  delete witRunVeneerPtr_;
  witRunVeneerPtr_ = 0;
}

// Make clone copy of object
LgFrDataInterfaceImplementation *
LgFrDataInterfaceImplementationFromWitRun::clone()
const
{
  return new LgFrDataInterfaceImplementationFromWitRun(*this);
}

// Destructor
LgFrDataInterfaceImplementationFromWitRun::~LgFrDataInterfaceImplementationFromWitRun()
{
  delete witRunVeneerPtr_;
}

// Assignment operator.
LgFrDataInterfaceImplementationFromWitRun&
LgFrDataInterfaceImplementationFromWitRun::operator=(
  const LgFrDataInterfaceImplementationFromWitRun& rhs)
{
  if (this != &rhs) {           // Check for assignment to self
    LgFrDataInterfaceImplementation::operator=  (rhs);     
     delete witRunVeneerPtr_;

     if ( rhs.witRunVeneerPtr_ != 0 ) 
       witRunVeneerPtr_ = new LgFrWitRunVeneer( *(rhs.witRunVeneerPtr_) );
     else
       witRunVeneerPtr_ = 0;

     riskForecastRE_ = rhs.riskForecastRE_;
     firmForecastRE_ = rhs.firmForecastRE_;
     newOpsRE_ = rhs.newOpsRE_;
  }
  return *this;
}

// Copy constructor
LgFrDataInterfaceImplementationFromWitRun::LgFrDataInterfaceImplementationFromWitRun(
  const LgFrDataInterfaceImplementationFromWitRun& source)
:
   LgFrDataInterfaceImplementation(source),
   witRunVeneerPtr_(0),
   riskForecastRE_(source.riskForecastRE_),
   firmForecastRE_(source.firmForecastRE_),
   newOpsRE_(source.newOpsRE_)
{
   if ( source.witRunVeneerPtr_ != 0 ) 
     witRunVeneerPtr_ = new LgFrWitRunVeneer( *(source.witRunVeneerPtr_) );
   else
     witRunVeneerPtr_ = 0;
}

// Preferred Constructor
LgFrDataInterfaceImplementationFromWitRun::LgFrDataInterfaceImplementationFromWitRun(
   WitRun * witRun,                                                      
   const std::string & rf, 
   const std::string & ff, 
   const std::string & no)
:
   LgFrDataInterfaceImplementation(),
   witRunVeneerPtr_(0),
   riskForecastRE_(rf),
   firmForecastRE_(ff),
   newOpsRE_(no)
{
   if ( witRun != 0 ) witRunVeneerPtr_ = new LgFrWitRunVeneer( witRun );
   std::string t = makeTitle(); 
   LgFrDataInterfaceImplementation::title(t);
}

// Default Constructor
// This is a private method
LgFrDataInterfaceImplementationFromWitRun::LgFrDataInterfaceImplementationFromWitRun()
:
   LgFrDataInterfaceImplementation(),
   witRunVeneerPtr_(0),
   riskForecastRE_(defaultRiskForecast),
   firmForecastRE_(defaultFirmForecast),
   newOpsRE_(defaultNewOps)
{
   // Nothing to do here
}     

#ifdef NDEBUG
#undef NDEBUG
#endif
// Tests this class.  Only returns if all tests passed
// 6/29/95 DW
void
LgFrDataInterfaceImplementationFromWitRun::contextTest
  (
   LgFrDataInterface    	* tdif1Ptr,	// testScenario1's
   LgFrLastProblemSolved        & lps,          // testScenario1's
   LgFrDataInterface    	* ts1dupDifPtr, // testScenario1dup's
   LgFrDataInterface    	* tdif2Ptr,	// testScenario2's
   const LgFrScenario           * scenPtr)	
{
  const float fltEps = .0001f;
  LgFrDataInterface& tdif1 = *tdif1Ptr;
  LgFrDataInterface& ts1dupDif = *ts1dupDifPtr;
  LgFrDataInterface& tdif2 = *tdif2Ptr;
  LgFrDataInterfaceImplementation* dimp 
      = tdif1.dataInterfaceImplementation();
  LgFrDataInterfaceImplementation* dimptsi 
      = ts1dupDif.dataInterfaceImplementation();
  LgFrDataInterfaceImplementation* dimp2 
      = tdif2.dataInterfaceImplementation();

  // Register all functions
  registerWitRunDifImpFunctions(dimp);
  registerWitRunDifImpFunctions(dimp2);

  // Test title method
  assert( tdif1.title() == "Diner");
  assert( ts1dupDif.title() == "Diner");
  assert( tdif2.title() == "Diner");
  assert( dimp->title() == "Diner");
  assert( dimptsi->title() == "Diner");
  assert( dimp2->title() == "Diner");
                    
  // Test get parts global functions
  LgFrDataInterfaceEssentialsFromScenario difEssen(scenPtr);
  LgFrPartVector partpv;
  std::string itemSubTypeStr = "material";  
  LgFrPartVector* rawPartsPtr
    = (LgFrPartVector*) tdif1.partAttribute(
	partpv, itemSubTypeStr, "rawParts" , &difEssen);
  LgFrPartVector raw = *rawPartsPtr;
  size_t t = 0; // Pulled out of the for below by RW2STL
  for(t = 0 ; t < 7 ; t++)
    witAddPart( lps.witRun(), raw[t].name().c_str(), WitRAW);
  assert( raw.size() == 7 );
  assert( raw[0].name() == "Egg" );
  assert( raw[1].name() == "Butter" );

  LgFrPartVector* productPartsPtr
    = (LgFrPartVector*) tdif1.partAttribute(
	partpv, itemSubTypeStr, "productParts", &difEssen );
  LgFrPartVector product = * productPartsPtr;
  for( t = 0 ; t < 10 ; t++)
    witAddPart( lps.witRun(), product[t].name().c_str(), WitPRODUCT);
  assert( product.size() == 10 );
  assert( product[9].name() == "HamEgg_Sand" );

  LgFrPartVector* capacityPartsPtr
    = (LgFrPartVector*) tdif1.partAttribute(
	partpv, itemSubTypeStr, "capacityParts", &difEssen );
  LgFrPartVector capacity = * capacityPartsPtr;
  for( t = 0 ; t < 2 ; t++)
    witAddPart( lps.witRun(), capacity[t].name().c_str(), WitCAPACITY);
  assert( capacity.size() == 2 );
  assert( capacity[0].name() == "Skillet" );

  // Test get raw attributes global functions
  {
  LgFrFloatVector* unitCostVectPtr
    = (LgFrFloatVector*) tdif1.partAttribute(
	raw,itemSubTypeStr,"rawUnitCost", &difEssen ); 
  assert( unitCostVectPtr->size() == 7 );
  assert( (*unitCostVectPtr)[0] == 1.5 );

  LgFrIntVector* constrainedVectPtr
   = (LgFrIntVector*) tdif1.partAttribute(
       raw,itemSubTypeStr,"rawConstrained", &difEssen );
  assert( constrainedVectPtr->size() == 7 );
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<constrainedVectPtr->size(); i++) 
      assert( (*constrainedVectPtr)[i] );

  LgFrFloatVector* procCostVectPtr
    = (LgFrFloatVector*) tdif1.partAttribute(
	raw,itemSubTypeStr,"rawProcCost", &difEssen ); 
  assert( procCostVectPtr->size() == 7 );
  assert( (*procCostVectPtr)[0] == 1.5 );

  LgFrTimeVecFloatVector* procLeadTimeVectPtr
   = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
       raw,itemSubTypeStr,"rawProcLeadTime", &difEssen);
  assert( procLeadTimeVectPtr->size() == 7 );
  int j = 0; // Pulled out of the for below by RW2STL
  for (j=0; j<procLeadTimeVectPtr->size(); j++){
      assert( (*procLeadTimeVectPtr)[j] == 0.0 );  }

  LgFrTimeVecFloatVector* obj1ScrapCostVectPtr
    = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
	raw,itemSubTypeStr,"rawObj1ScrapCost", &difEssen );
  assert( obj1ScrapCostVectPtr->size() == 7 );
  assert( (*obj1ScrapCostVectPtr)[0] == 
	  LgFrTimeVecFloat(8,".01 .01 0 0 0 0 0 0") );

  LgFrTimeVecFloatVector* obj1StockCostVectPtr
    = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
	raw,itemSubTypeStr,"rawObj1StockCost", &difEssen );
  assert( obj1StockCostVectPtr->size() == 7 );
  assert( (*obj1StockCostVectPtr)[0] == 
	  LgFrTimeVecFloat(8,".02 .02 0 0 0 0 0 0") ); 
 
  LgFrTimeVecFloatVector* supplyVolVectPtr
   = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
       raw,itemSubTypeStr,"rawSupplyVol", &difEssen);
  assert( supplyVolVectPtr->size() == 7 );
  assert( (*supplyVolVectPtr)[0] == 
	  LgFrTimeVecFloat(8," 5.0 2.0 0 0 0 0 0 0") );
  }

  // Test get product attribute global functions
  {
  LgFrIntVector* yieldVectPtr
   = (LgFrIntVector*) tdif1.partAttribute(
       product,itemSubTypeStr,"productYield", &difEssen);
  assert( yieldVectPtr->size() == 10 );
  assert ((*yieldVectPtr)[0] == 99 );

  LgFrTimeVecFloatVector* cycleTimeVectPtr
   = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
       product,itemSubTypeStr,"productCycleTime", &difEssen);
  assert( cycleTimeVectPtr->size() == 10 );
  assert( (*cycleTimeVectPtr)[0] == 
	  LgFrTimeVecFloat(8," 0.3 0.3 0 0 0 0 0 0") );

  LgFrTimeVecFloatVector* obj1ProdCostVectPtr
   = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
     product,itemSubTypeStr,"productObj1ProdCost", &difEssen);
  assert( obj1ProdCostVectPtr->size() == 10 );
  assert( (*obj1ProdCostVectPtr)[0] == 
	  LgFrTimeVecFloat(8," 0.5 0.5 0 0 0 0 0 0") );

  LgFrFloatVector* unitCostVectPtr2
    = (LgFrFloatVector*) tdif1.partAttribute(
	product,itemSubTypeStr,"productUnitCost", &difEssen); 
  assert( unitCostVectPtr2->size() == 10 );
  assert ( fabs((*unitCostVectPtr2)[0] - 1.2) < fltEps );

  LgFrIntVector* constrainedVectPtr2
   = (LgFrIntVector*) tdif1.partAttribute(
       product,itemSubTypeStr,"productConstrained", &difEssen);
  assert( constrainedVectPtr2->size() == 10 );
  int k = 0; // Pulled out of the for below by RW2STL
  for (k=0; k<constrainedVectPtr2->size(); k++) 
      assert( (*constrainedVectPtr2)[k] ); 

  LgFrFloatVector* procCostVectPtr2
    = (LgFrFloatVector*) tdif1.partAttribute(
	product,itemSubTypeStr,"productProcCost", &difEssen); 
  assert( procCostVectPtr2->size() == 10 );
  assert ( fabs((*procCostVectPtr2)[0] - 1.2) < fltEps );
  
  LgFrTimeVecFloatVector* obj1ProdCostVectPtr2
   = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
     product,itemSubTypeStr,"productObj1ProdCost", &difEssen);
  assert( obj1ProdCostVectPtr2->size() == 10 );
  assert( (*obj1ProdCostVectPtr2)[0] == 
	  LgFrTimeVecFloat(8," 0.5 0.5 0 0 0 0 0 0") );

  // procLeadTime functions for product has not been implemented yet.
  LgFrTimeVecFloatVector* procLeadTimeVectPtr
   = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
       product,itemSubTypeStr,"productProcLeadTime", &difEssen);
  assert( procLeadTimeVectPtr->size() == 10 );
  assert( (*procLeadTimeVectPtr)[0] == 
	  LgFrTimeVecFloat(8," 0.3 0.3 0 0 0 0 0 0") );

  LgFrTimeVecFloatVector* obj1ScrapCostVectPtr2
    = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
	product,itemSubTypeStr,"productObj1ScrapCost", &difEssen );
  assert( obj1ScrapCostVectPtr2->size() == 10 );
  assert( (*obj1ScrapCostVectPtr2)[0] == 
	  LgFrTimeVecFloat(8," 0.23 0.23 0 0 0 0 0 0") );

  LgFrTimeVecFloatVector* obj1StockCostVectPtr2
    = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
	product,itemSubTypeStr,"productObj1StockCost", &difEssen );
  assert( obj1StockCostVectPtr2->size() == 10 );
  assert( (*obj1StockCostVectPtr2)[0] == 
	  LgFrTimeVecFloat(8," 0.24 0.24 0 0 0 0 0 0") );

  LgFrTimeVecFloatVector* supplyVolVectPtr2
   = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
       product,itemSubTypeStr,"productSupplyVol", &difEssen);
  assert( supplyVolVectPtr2->size() == 10 );
  assert( (*supplyVolVectPtr2)[0] == 
	  LgFrTimeVecFloat(8," 0.01 0.01 0 0 0 0 0 0") );
  }

  // Test get capacity attributes global functions
  {
  LgFrFloatVector* unitCostVectPtr
    = (LgFrFloatVector*) tdif1.partAttribute(
	capacity,itemSubTypeStr,"capacityUnitCost", &difEssen); 
  assert( unitCostVectPtr->size() == 2 );
  assert ( fabs((*unitCostVectPtr)[0] - 1.11 ) < fltEps );

  LgFrIntVector* constrainedVectPtr
   = (LgFrIntVector*) tdif1.partAttribute(
       capacity,itemSubTypeStr,"capacityConstrained", &difEssen);
  assert( constrainedVectPtr->size() == 2 );
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<constrainedVectPtr->size(); i++) 
      assert( (*constrainedVectPtr)[i] );

  LgFrTimeVecFloatVector* obj1ScrapCostVectPtr
    = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
	capacity,itemSubTypeStr,"capacityObj1ScrapCost", &difEssen );
  assert( obj1ScrapCostVectPtr->size() == 2 );
  assert( (*obj1ScrapCostVectPtr)[0] == 
	  LgFrTimeVecFloat(8," 0.33 0.22 0 0 0 0 0 0") );

  LgFrTimeVecFloatVector* supplyVolVectPtr
   = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
       capacity,itemSubTypeStr,"capacitySupplyVol", &difEssen);
  assert( supplyVolVectPtr->size() == 2 );
  assert( (*supplyVolVectPtr)[0] == 
	  LgFrTimeVecFloat(8," 15 20 0 0 0 0 0 0") );
  }

// Test Get Demands global functions
  LgFrDemandVector demandpv;

  LgFrDemandVector* committedDemandPtr
    = (LgFrDemandVector*) tdif1.demandAttribute(
	demandpv, "committedDemands", &difEssen );
  LgFrDemandVector committed = * committedDemandPtr; 
  assert( committed.size() == 7 ); 
  
  LgFrDemandVector* newOpportunityDemandPtr
    = (LgFrDemandVector*) tdif1.demandAttribute(
	demandpv, "newOpportunityDemands", &difEssen );
  LgFrDemandVector newOpportunity = * newOpportunityDemandPtr; 
  assert( newOpportunity.size() == 1 );

  LgFrDemandVector* firmForecastDemandPtr
    = (LgFrDemandVector*) tdif1.demandAttribute(
	demandpv, "firmForecastDemands", &difEssen );
  LgFrDemandVector firmForecast = * firmForecastDemandPtr;
  assert( firmForecast.size() == 1 );

  LgFrDemandVector* riskForecastDemandPtr
    = (LgFrDemandVector*) tdif1.demandAttribute(
	demandpv, "riskForecastDemands", &difEssen );
  LgFrDemandVector riskForecast = * riskForecastDemandPtr;
  assert( riskForecast.size() == 1 );

  // Test get demand attributes global functions
  LgFrFloatVector* grossRevVectPtr
   = (LgFrFloatVector*) tdif1.demandAttribute(
       committed,"demandGrossRev", &difEssen);
  assert( grossRevVectPtr->size() == 7 );
  assert ( fabs((*grossRevVectPtr)[0] - 3.1 ) < fltEps );

  grossRevVectPtr = (LgFrFloatVector*)
      tdif1.demandAttribute(newOpportunity,"demandGrossRev", &difEssen );
  assert( grossRevVectPtr->size() == 1 );
  assert ( fabs((*grossRevVectPtr)[0] - 3.55 ) < fltEps );

  grossRevVectPtr = (LgFrFloatVector*)
      tdif1.demandAttribute(firmForecast,"demandGrossRev", &difEssen );
  assert( grossRevVectPtr->size() == 1 );
  assert ( fabs((*grossRevVectPtr)[0] - 1.23 ) < fltEps );
    
  grossRevVectPtr = (LgFrFloatVector*)
      tdif1.demandAttribute(riskForecast,"demandGrossRev", &difEssen );
  assert( grossRevVectPtr->size() == 1 );
  assert ( fabs((*grossRevVectPtr)[0] - 2.45 ) < fltEps );

  LgFrTimeVecFloatVector * demandVolVectPtr = (LgFrTimeVecFloatVector *)
      tdif1.demandAttribute(committed, "demandVol", &difEssen);
  assert(demandVolVectPtr->size() == 7 );
  assert( (*demandVolVectPtr)[0] == 
	  LgFrTimeVecFloat(8," 0.14 0.14 0 0 0 0 0 0") );

  LgFrTimeVecFloatVector * demandVolVectPtr2 = (LgFrTimeVecFloatVector *)
      tdif1.demandAttribute(newOpportunity, "demandVol", &difEssen);
  assert(demandVolVectPtr2->size() == 1 );
  assert( (*demandVolVectPtr2)[0] == 
	  LgFrTimeVecFloat(8," 3 4 0 0 0 0 0 0") );

  LgFrTimeVecFloatVector * demandVolVectPtr3 = (LgFrTimeVecFloatVector *)
      tdif1.demandAttribute(firmForecast, "demandVol", &difEssen);
  assert(demandVolVectPtr3->size() == 1 );
  assert( (*demandVolVectPtr3)[0] == 
	  LgFrTimeVecFloat(8," 4 2 0 0 0 0 0 0") );

  LgFrTimeVecFloatVector * demandVolVectPtr4 = (LgFrTimeVecFloatVector *)
      tdif1.demandAttribute(riskForecast, "demandVol", &difEssen);
  assert(demandVolVectPtr4->size() == 1 );
  assert( (*demandVolVectPtr4)[0] == 
	  LgFrTimeVecFloat(8," 0.27 0.27 0 0 0 0 0 0") );

  LgFrTimeVecFloatVector* obj1CumShipRewardVectPtr
    = (LgFrTimeVecFloatVector*) tdif1.demandAttribute(
	committed, "demandObj1CumShipReward", &difEssen);
  assert(obj1CumShipRewardVectPtr->size() == 7 );
  assert( (*obj1CumShipRewardVectPtr)[0] ==
          LgFrTimeVecFloat(8," 0.21 0.21 0 0 0 0 0 0") );

  LgFrTimeVecFloatVector* obj1CumShipRewardVectPtr2
    = (LgFrTimeVecFloatVector*) tdif1.demandAttribute(
newOpportunity, "demandObj1CumShipReward", &difEssen);
  assert(obj1CumShipRewardVectPtr2->size() == 1 );
  assert( (*obj1CumShipRewardVectPtr2)[0] ==
          LgFrTimeVecFloat(8," 0.09 0.09 0 0 0 0 0 0") );

  LgFrTimeVecFloatVector* obj1CumShipRewardVectPtr3
    = (LgFrTimeVecFloatVector*) tdif1.demandAttribute(
	firmForecast,"demandObj1CumShipReward", &difEssen);
  assert(obj1CumShipRewardVectPtr3->size() == 1 );
  assert( (*obj1CumShipRewardVectPtr3)[0] ==
          LgFrTimeVecFloat(8," 0.17 0.17 0 0 0 0 0 0") );

  LgFrTimeVecFloatVector* obj1CumShipRewardVectPtr4
    = (LgFrTimeVecFloatVector*) tdif1.demandAttribute(
	riskForecast, "demandObj1CumShipReward", &difEssen);
  assert(obj1CumShipRewardVectPtr4->size() == 1 );
  assert( (*obj1CumShipRewardVectPtr4)[0] ==
          LgFrTimeVecFloat(8," 0.07 0.07 0 0 0 0 0 0") );

  LgFrTimeVecFloatVector* obj1ShipRewardVectPtr
   = (LgFrTimeVecFloatVector*) tdif1.demandAttribute(
       committed, "demandObj1ShipReward", &difEssen);
  assert(obj1ShipRewardVectPtr->size() == 7 );
  assert( (*obj1ShipRewardVectPtr)[0] ==
          LgFrTimeVecFloat(8," 3.81 3.81 0 0 0 0 0 0") );

  LgFrTimeVecFloatVector* obj1ShipRewardVectPtr2
   = (LgFrTimeVecFloatVector*) tdif1.demandAttribute(
       newOpportunity, "demandObj1ShipReward", &difEssen);
  assert(obj1ShipRewardVectPtr2->size() == 1 );
  assert( (*obj1ShipRewardVectPtr2)[0] ==
          LgFrTimeVecFloat(8," 3.5 3.5 0 0 0 0 0 0") );

  LgFrTimeVecFloatVector* obj1ShipRewardVectPtr3
   = (LgFrTimeVecFloatVector*) tdif1.demandAttribute(
       firmForecast, "demandObj1ShipReward", &difEssen);
  assert(obj1ShipRewardVectPtr3->size() == 1 );
  assert( (*obj1ShipRewardVectPtr3)[0] ==
          LgFrTimeVecFloat(8," 1.12 1.12 0 0 0 0 0 0") );

  LgFrTimeVecFloatVector* obj1ShipRewardVectPtr4
   = (LgFrTimeVecFloatVector*) tdif1.demandAttribute(
       riskForecast, "demandObj1ShipReward", &difEssen);
  assert(obj1ShipRewardVectPtr4->size() == 1 );
  assert( (*obj1ShipRewardVectPtr4)[0] ==
          LgFrTimeVecFloat(8," 2.19 2.19 0 0 0 0 0 0") );

  // Test BOM
  LgFrDataInterfaceImplementationFromWitRun * dImpWR = (
      LgFrDataInterfaceImplementationFromWitRun * ) dimp;
  tdif1.partAttribute(
      partpv, itemSubTypeStr, "defineBom" , &difEssen);
  int nBomEntries, retInt;
  witBoolean retBool;
  float retFloat;
  char * consumedPartName; 
  witGetPartNBomEntries( lps.witRun(), "Plain_Omlet", &nBomEntries);
  assert( nBomEntries == 3);
  witGetBomEntryConsumedPart(lps.witRun(), "Plain_Omlet", 0,
                             &consumedPartName);
  assert ( std::string (consumedPartName) == std::string ("Skillet"));
  portableFree (consumedPartName);
  witGetBomEntryUsageRate( lps.witRun(), "Plain_Omlet", 0,
                           &retFloat);
  assert ( fabs(retFloat - 3.0) < fltEps );
  witGetBomEntryConsumedPart(lps.witRun(), "Plain_Omlet", 2,
                             &consumedPartName);
  assert ( std::string (consumedPartName) == std::string ("Butter"));
  portableFree (consumedPartName);
  witGetBomEntryEarliestPeriod(lps.witRun(), "Plain_Omlet", 2,
                               &retInt);
  assert ( retInt == 1 );
  witGetBomEntryLatestPeriod(lps.witRun(), "Plain_Omlet", 2,
                               &retInt);
  assert ( retInt == 1 );
  witGetBomEntryMandEC(lps.witRun(), "Plain_Omlet", 2,
                               &retBool);
  assert ( retBool );
  witGetBomEntryUsageRate(lps.witRun(), "Plain_Omlet", 2,
                               &retFloat);
  assert ( fabs(retFloat - 1.0) < fltEps );
  witGetBomEntryUsageTime(lps.witRun(), "Plain_Omlet", 2,
                               &retFloat);
  assert ( fabs(retFloat - 0.1) < fltEps );
}
