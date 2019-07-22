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
#include <scenario/src/difPWcmr.h>
#include <scenario/src/calendar.h>
#include <scenario/src/difEssSc.h>
#include <scenario/src/timeVecs.h>

// -----------------------------------------------------------------------
//
//  Register all functions
//
// -----------------------------------------------------------------------

void
registerPWcmrpDifImpFunctions(
    LgFrDataInterfaceImplementation * dImp) 
{
    // Register all raw material attribute functions
    dImp->registerFunction("rawUnitCost", material, part,
			   vectorFloat, difPWcmrpGetRawUnitCost);
    dImp->registerFunction("rawProcCost", material, part,
			   vectorFloat, difPWcmrpGetRawProcCost);
    dImp->registerFunction("rawConstrained", material, part,
			   vectorInt, difPWcmrpGetRawConstrained);
    dImp->registerFunction("rawProcLeadTime", material, part,
			   timeVecFloat, difPWcmrpGetRawProcLeadTime);
    dImp->registerFunction("rawObj1ScrapCost", material, part,
			   timeVecFloat, difPWcmrpGetRawObj1ScrapCost);
    dImp->registerFunction("rawObj1StockCost", material, part,
			   timeVecFloat, difPWcmrpGetRawObj1StockCost);
    dImp->registerFunction("rawSupplyVol", material, part,
			   timeVecFloat, difPWcmrpGetRawSupplyVol);

    // Register all product attribute functions
    dImp->registerFunction("productUnitCost", material, part,
			   vectorFloat, difPWcmrpGetProductUnitCost);
    dImp->registerFunction("productProcCost", material, part,
			   vectorFloat, difPWcmrpGetProductProcCost);
    dImp->registerFunction("productConstrained", material, part,
			   vectorInt, difPWcmrpGetProductConstrained);
    dImp->registerFunction("productProcLeadTime", material, part,
			   timeVecFloat, difPWcmrpGetProductProcLeadTime);
    dImp->registerFunction("productYield", material, part,
			   vectorInt, difPWcmrpGetProductYield);
    dImp->registerFunction("productCycleTime", material, part,
			   timeVecFloat, difPWcmrpGetProductCycleTime);
    dImp->registerFunction("obj1ProdCost", material, part,
			   timeVecFloat, difPWcmrpGetObj1ProdCost);
    dImp->registerFunction("productObj1ScrapCost", material, part,
			   timeVecFloat, difPWcmrpGetProductObj1ScrapCost);
    dImp->registerFunction("productObj1StockCost", material, part,
			   timeVecFloat, difPWcmrpGetProductObj1StockCost);
    dImp->registerFunction("productSupplyVol", material, part,
			   timeVecFloat, difPWcmrpGetProductSupplyVol);
    dImp->registerFunction("productMinLotSize", material, part,
			   timeVecFloat, difPWcmrpGetProductMinLotSize);

    // Register all capacity attribute functions
    dImp->registerFunction("capacityUnitCost", material, part,
			   vectorFloat, difPWcmrpGetCapacityUnitCost);
    dImp->registerFunction("capacityConstrained", material, part,
			   vectorInt, difPWcmrpGetCapacityConstrained);
    dImp->registerFunction("capacityObj1ScrapCost", material, part,
			   timeVecFloat, difPWcmrpGetCapacityObj1ScrapCost);
    dImp->registerFunction("capacitySupplyVol", material, part,
			   timeVecFloat, difPWcmrpGetCapacitySupplyVol);

    // Register all functions to obtain demands
    dImp->registerFunction("committedDemands", demandType, demand,
		   orderedVecDemand, difPWcmrpGetCommittedDemands);
    dImp->registerFunction("newOpportunityDemands", demandType, demand,
		   orderedVecDemand, difPWcmrpGetNewOpportunityDemands);
    dImp->registerFunction("firmForecastDemands", demandType, demand,
			   orderedVecDemand, difPWcmrpGetFirmForecastDemands);
    dImp->registerFunction("riskForecastDemands", demandType, demand,
			   orderedVecDemand, difPWcmrpGetRiskForecastDemands);

    // Register all demand attribute functions
    dImp->registerFunction("demandGrossRev", demandType, demand,
			   vectorFloat, difPWcmrpGetDemandGrossRev);
    dImp->registerFunction("demandVol", demandType, demand,
			   timeVecFloat, difPWcmrpGetDemandVol);
    dImp->registerFunction("demandObj1CumShipReward", demandType, demand,
			   timeVecFloat, difPWcmrpGetDemandObj1CumShipReward);
    dImp->registerFunction("demandObj1ShipReward", demandType, demand,
			   timeVecFloat, difPWcmrpGetDemandObj1ShipReward);

    // jmt 6/27/95 need to have partTable and demandTable (similar to attrTable)
    // Register all functions to obtain parts
    dImp->registerFunction("rawParts", material, part,
			   orderedVecPart, difPWcmrpGetRawParts);
    dImp->registerFunction("productParts", material, part,
			   orderedVecPart, difPWcmrpGetProductParts);
    dImp->registerFunction("capacityParts", material, part,
			   orderedVecPart, difPWcmrpGetCapacityParts);

    // Register define bom globle function
    dImp->registerFunction("defineBom", material, part,
			   timeVecFloat, difPWcmrpDefineBom);

}

//-------------------------------------------------------------------
//  
//   Create a LgFrDataInterfaceImplementation object
//
//-------------------------------------------------------------------

LgFrDataInterfaceImplementation * createPWcmrpDifImp()
{
   return new LgFrDataInterfaceImplementation("internal data set pw1");
} 

//-------------------------------------------------------------------
//  
//   Global functions to obtain the defined parts and capacities.
//
//-------------------------------------------------------------------

// Return vector of raw materials.
// difPWcmrpGetRawParts global function (registering in the attribute table).
// Get raw parts for the perfect world CMRP and insert them into set_of_part
// Return: a LgFrPartVector object (vector of raw parts).
void *
difPWcmrpGetRawParts(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
 ) 
{
  LgFrPartVector& retVal = *(new LgFrPartVector);
  LgFrPart part = difEssenPtr->createAndInsertPart("MEM_Module");
  retVal.push_back(part);
  part = difEssenPtr->createAndInsertPart("CPU_Type_F");
  retVal.push_back(part);
  part = difEssenPtr->createAndInsertPart("CPU_Type_S");
  retVal.push_back(part);
  part = difEssenPtr->createAndInsertPart("RAW_Board_F");
  retVal.push_back(part);
  part = difEssenPtr->createAndInsertPart("RAW_Board_S");
  retVal.push_back(part);
  part = difEssenPtr->createAndInsertPart("Disk_Drive_L");
  retVal.push_back(part);
  part = difEssenPtr->createAndInsertPart("Disk_Drive_S");
  retVal.push_back(part);
  part = difEssenPtr->createAndInsertPart("Screen_L");
  retVal.push_back(part);
  part = difEssenPtr->createAndInsertPart("Screen_S");
  retVal.push_back(part);
  status = 0;
  return &retVal;
}

// difPWcmrpGetProdcutParts global function (registering in the attribute table).
// Get product parts for the perfect world CMRP and insert it into set_of_part
// Return: a LgFrPartVector object (vector of producible parts)
void*
difPWcmrpGetProductParts(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
 )
{
  LgFrPartVector& retVal = *(new LgFrPartVector);

  // Subassemblies:
  LgFrPart part = difEssenPtr->createAndInsertPart("System_Board_1");
  retVal.push_back(part);
  part = difEssenPtr->createAndInsertPart("System_Board_2");
  retVal.push_back(part);
  part = difEssenPtr->createAndInsertPart("System_Board_3");
  retVal.push_back(part);
  part = difEssenPtr->createAndInsertPart("System_Board_4");
  retVal.push_back(part);

  // Final Products
  part = difEssenPtr->createAndInsertPart("Computer_1");
  retVal.push_back(part);
  part = difEssenPtr->createAndInsertPart("Computer_2");
  retVal.push_back(part);
  part = difEssenPtr->createAndInsertPart("Computer_3");
  retVal.push_back(part);
  part = difEssenPtr->createAndInsertPart("Computer_4");
  retVal.push_back(part);
  part = difEssenPtr->createAndInsertPart("Computer_5");
  retVal.push_back(part);
  part = difEssenPtr->createAndInsertPart("Computer_6");
  retVal.push_back(part);
  part = difEssenPtr->createAndInsertPart("Computer_7");
  retVal.push_back(part);
  part = difEssenPtr->createAndInsertPart("Computer_8");
  retVal.push_back(part);
  status = 0;
  return &retVal;
}

// difPWcmrpGetCapacityParts global function (registering in the attribute table).
// Get capacity parts for the perfect world CMRP and insert it into set_of_part
// Return: a LgFrPartVector object (vector of producible parts)
void*
difPWcmrpGetCapacityParts(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
 ) 
{
  LgFrPartVector& retVal = *(new LgFrPartVector);

  LgFrPart part = difEssenPtr->createAndInsertPart("Board_Assembly");
  retVal.push_back(part);
  part = difEssenPtr->createAndInsertPart("Board_Test");
  retVal.push_back(part);
  part = difEssenPtr->createAndInsertPart("System_Assembly");
  retVal.push_back(part);
  part = difEssenPtr->createAndInsertPart("System_Test");
  retVal.push_back(part);
  status = 0;
  return &retVal;
}

//-------------------------------------------------------------------
//  
//   Global functions to obtain raw material attributes
//
//-------------------------------------------------------------------

// difPWcmrpGetRawUnitCost global function (registered in the attribute table)
// it returns a LgFrFloatVector object.
void *
difPWcmrpGetRawUnitCost(
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
    if ( pv[i]->name() == "MEM_Module" )
       retVal.push_back( 60.0 );
    else if ( pv[i]->name() == "CPU_Type_F" )
       retVal.push_back( 420.0 );
    else if ( pv[i]->name() == "CPU_Type_S" )
       retVal.push_back( 200.0 );
    else if ( pv[i]->name() == "RAW_Board_F" ) 
       retVal.push_back( 10.0 );
    else if ( pv[i]->name() == "RAW_Board_S" ) 
       retVal.push_back( 5.0 );
    else if ( pv[i]->name() == "Screen_L" ) 
       retVal.push_back( 1000.0 );
    else if ( pv[i]->name() == "Screen_S" ) 
       retVal.push_back( 430.0 );
    else if ( pv[i]->name() == "Disk_Drive_L" ) 
       retVal.push_back( 450.0 );
    else if ( pv[i]->name() == "Disk_Drive_S" ) 
       retVal.push_back( 300.0 );
    else
       retVal.push_back( 0.0 );
  }
  status = 0;
  return &retVal;
}
  
// difPWcmrpGetRawProcCost global function (registered in the attribute table).
// It returns a LgFrFloatVector object.
void *
difPWcmrpGetRawProcCost(
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
      retVal.push_back( 1.23f );
  }
  status = 0;
  return &retVal;
}

// difPWcmrpGetRawConstrained global function (registered in the attribute table).
// It returns a LgFrIntVector object
void *
difPWcmrpGetRawConstrained(
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
    if ( pv[i]->name() == "MEM_Module" )
       retVal.push_back( true );
    else if ( pv[i]->name() == "Screen_S" )
       retVal.push_back( true );
    else
       retVal.push_back( false );
  }
  status = 0;
  return &retVal; 
}

// difPWcmrpGetRawProcLeadTime global function (registered in the attribute table).
// It returns a LgFrTimeVecFloatVector object.
void *
difPWcmrpGetRawProcLeadTime(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
 )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr;
  LgFrTimeVecFloatVector& retVal = 
      *(new LgFrTimeVecFloatVector);
  retVal.reserve( pv.size() );   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) { 
    if ( pv[i]->name() == "MEM_Module" )
       retVal.push_back( LgFrTimeVecFloat( nperiods , 8.0 ) );
    else if ( pv[i]->name() == "CPU_Type_F" )
       retVal.push_back( LgFrTimeVecFloat( nperiods , 3.0 ) );
    else if ( pv[i]->name() == "CPU_Type_S" )
       retVal.push_back( LgFrTimeVecFloat( nperiods , 2.0 ) );
    else if ( pv[i]->name() == "Screen_S" )
       retVal.push_back( LgFrTimeVecFloat( nperiods , 8.0 ) );
    else if ( pv[i]->name() == "Disk_Drive_L" )
       retVal.push_back( LgFrTimeVecFloat( nperiods , 1.0 ) );
    else if ( pv[i]->name() == "Disk_Drive_S" )
       retVal.push_back( LgFrTimeVecFloat( nperiods , 1.0 ) );
    else
       retVal.push_back( LgFrTimeVecFloat( nperiods , 0.0 ) );
  }
  status = 0;
  return &retVal;
}      

// difPWcmrpGetRawObj1ScrapCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difPWcmrpGetRawObj1ScrapCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
 )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr;
  LgFrTimeVecFloatVector& retVal = 
      *(new LgFrTimeVecFloatVector);
  retVal.reserve( pv.size() );   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
      retVal.push_back( LgFrTimeVecFloat( nperiods, 0.12f ) );
  }
  status = 0;
  return &retVal;
}

// difPWcmrpGetRawObj1StockCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difPWcmrpGetRawObj1StockCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
 )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr;
  LgFrTimeVecFloatVector& retVal = 
      *(new LgFrTimeVecFloatVector);
  retVal.reserve( pv.size() );   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
      retVal.push_back( LgFrTimeVecFloat( nperiods, 0.10f ) );
  }
  status = 0;
  return &retVal;
}

// difPWcmrpGetRawSupplyVol global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difPWcmrpGetRawSupplyVol(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
 )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr;
  LgFrTimeVecFloatVector& retVal = 
      *(new LgFrTimeVecFloatVector);
  retVal.reserve( pv.size() );   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) { 
    if ( pv[i]->name() == "MEM_Module" )
       retVal.push_back( LgFrTimeVecFloat( nperiods , "6000. 6000. 6000. 6000. 6000. 6000. 6000. 6000." ) );
    else if ( pv[i]->name() == "CPU_Type_F" )
       retVal.push_back( LgFrTimeVecFloat( nperiods , "0. 700. 700. 700. 700. 0. 0. 0." ) );
    else if ( pv[i]->name() == "CPU_Type_S" )    
       retVal.push_back( LgFrTimeVecFloat( nperiods , "6000. 6000. 6000. 0. 0. 0. 0. 0." ) );
    else if ( pv[i]->name() == "Screen_S")
       retVal.push_back( LgFrTimeVecFloat( nperiods , "1500. 1500. 2000. 2000. 2000. 2000. 2000. 2000." ) );
    else if ( pv[i]->name() == "Disk_Drive_L")
       retVal.push_back( LgFrTimeVecFloat( nperiods , "3000. 0. 0. 0. 0. 0. 0. 0." ) );
    else if ( pv[i]->name() == "Disk_Drive_S")
       retVal.push_back( LgFrTimeVecFloat( nperiods , "6000. 0. 0. 0. 0. 0. 0. 0." ) );
    else      
       retVal.push_back( LgFrTimeVecFloat( nperiods , 0.0 ) );
  }  
  status = 0;
  return &retVal;
}

//--------------------------------------------------------------------
//  
//   Global functions to obtain producible part attributes
//
//-------------------------------------------------------------------

// difPWcmrpGetProductUnitCost global function (registered in the attribute table)
// it returns a LgFrFloatVector object.
void *
difPWcmrpGetProductUnitCost(
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
    if ( pv[i]->name() == "Computer_1" )
       retVal.push_back( 1940. );
    else if ( pv[i]->name() == "Computer_2" )
       retVal.push_back( 2000. );
    else if ( pv[i]->name() == "Computer_3" )    
       retVal.push_back( 1715. );
    else if ( pv[i]->name() == "Computer_4")
       retVal.push_back( 1775. );
    else if ( pv[i]->name() == "Computer_5")
       retVal.push_back( 1565. );
    else if ( pv[i]->name() == "Computer_6")
       retVal.push_back( 1625. );
    else if ( pv[i]->name() == "Computer_7")
       retVal.push_back( 995. );
    else if ( pv[i]->name() == "Computer_8")
       retVal.push_back( 1055. );
    else if ( pv[i]->name() == "System_Board_1")
       retVal.push_back( 490. );
    else if ( pv[i]->name() == "System_Board_2")
       retVal.push_back( 550. );
    else if ( pv[i]->name() == "System_Board_3")
       retVal.push_back( 265. );
    else if ( pv[i]->name() == "System_Board_4")
       retVal.push_back( 325. );
    else
       retVal.push_back( 0.0 );
  }
  status = 0;
  return &retVal;
}

// difPWcmrpGetProductProcCost global function (registered in the attribute table).
// It returns a LgFrFloatVector object.
void *
difPWcmrpGetProductProcCost(
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
      retVal.push_back( 1.43f );
  }
  status = 0;
  return &retVal;
}

// difPWcmrpGetProductConstrained global function (registered in the attribute table).
// It returns a LgFrIntVector object
void *
difPWcmrpGetProductConstrained(
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

// difPWcmrpGetProductProcLeadTime global function (registered in the attribute table).
// It returns a LgFrTimeVecFloatVector object.
void *
difPWcmrpGetProductProcLeadTime(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
 )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr;
  LgFrTimeVecFloatVector& retVal = 
      *(new LgFrTimeVecFloatVector);
  retVal.reserve( pv.size() );   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) { 
    retVal.push_back( LgFrTimeVecFloat( nperiods , -2.0 ) );
  }
  status = 0;
  return &retVal;
}

// difPWcmrpGetProductYield global function (registered in the attribute table).
// Return: a LgFrFloatVector object
void *
difPWcmrpGetProductYield(
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
    retVal.push_back( 100 );
  }
  status = 0;
  return &retVal;
}

// difPWcmrpGetProductCycleTime global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difPWcmrpGetProductCycleTime(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
 )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr;
  LgFrTimeVecFloatVector& retVal = 
      *(new LgFrTimeVecFloatVector);
  retVal.reserve( pv.size() );   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    if ( pv[i]->name() == "System_Board_1" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, 1.2f ) );
    else if ( pv[i]->name() == "System_Board_2" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, 1.9f ) );
    else if (pv[i]->name() == "System_Board_3")
        retVal.push_back( LgFrTimeVecFloat( nperiods, 1.5f ) );
    else if (pv[i]->name() == "System_Board_4")
        retVal.push_back( LgFrTimeVecFloat( nperiods, 1.7f ) );
    else
       retVal.push_back( LgFrTimeVecFloat( nperiods , 0.0f ) );
  }  
  status = 0;
  return &retVal;
}

// difPWcmrpGetObj1ProdCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difPWcmrpGetObj1ProdCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
 )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr;
  LgFrTimeVecFloatVector& retVal = 
      *(new LgFrTimeVecFloatVector);
  retVal.reserve( pv.size() );   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    retVal.push_back( LgFrTimeVecFloat( nperiods, 3.4f ) );
  }
  status = 0;
  return &retVal;
}

// difPWcmrpGetProductObj1ScrapCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difPWcmrpGetProductObj1ScrapCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
 )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr;
  LgFrTimeVecFloatVector& retVal = 
      *(new LgFrTimeVecFloatVector);
  retVal.reserve( pv.size() );   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
      retVal.push_back( LgFrTimeVecFloat( nperiods, 1.0f ) );
  }
  status = 0;
  return &retVal;
}

// difPWcmrpGetProductObj1StockCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difPWcmrpGetProductObj1StockCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
 )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr;
  LgFrTimeVecFloatVector& retVal = 
      *(new LgFrTimeVecFloatVector);
  retVal.reserve( pv.size() );   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
      retVal.push_back( LgFrTimeVecFloat( nperiods, 0.83f ) );
  }
  status = 0;
  return &retVal;
}

// difPWcmrpGetProductSupplyVol global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difPWcmrpGetProductSupplyVol(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
 )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr;
  LgFrTimeVecFloatVector& retVal = 
      *(new LgFrTimeVecFloatVector);
  retVal.reserve( pv.size() );   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) { 
    if ( pv[i]->name() == "Computer_1" )
       retVal.push_back( LgFrTimeVecFloat( nperiods , 0.0f ) );
    else if ( pv[i]->name() == "Computer_2" )
       retVal.push_back( LgFrTimeVecFloat( nperiods , 0.0f ) );
    else if ( pv[i]->name() == "Computer_3" )    
       retVal.push_back( LgFrTimeVecFloat( nperiods , "100. 100. 0. 0. 0. 0. 0. 0." ) );
    else if ( pv[i]->name() == "Computer_4")
       retVal.push_back( LgFrTimeVecFloat( nperiods ,  "100. 150. 0. 0. 0. 0. 0. 0." ) );
    else if ( pv[i]->name() == "Computer_5")
       retVal.push_back( LgFrTimeVecFloat( nperiods ,  "100. 150. 0. 0. 0. 0. 0. 0." ) );
    else if ( pv[i]->name() == "Computer_6")
       retVal.push_back( LgFrTimeVecFloat( nperiods , "150. 200. 0. 0. 0. 0. 0. 0." ) );
    else if ( pv[i]->name() == "Computer_7")
       retVal.push_back( LgFrTimeVecFloat( nperiods , "100. 200. 0. 0. 0. 0. 0. 0." ) );
    else if ( pv[i]->name() == "Computer_8")
       retVal.push_back( LgFrTimeVecFloat( nperiods , "200. 300. 0. 0. 0. 0. 0. 0." ) );
    else if ( pv[i]->name() == "System_Board_1")
       retVal.push_back( LgFrTimeVecFloat( nperiods , "650. 625. 0. 0. 0. 0. 0. 0." ) );
    else if ( pv[i]->name() == "System_Board_2")
       retVal.push_back( LgFrTimeVecFloat( nperiods , "800. 730. 0. 0. 0. 0. 0. 0." ) );
    else if ( pv[i]->name() == "System_Board_3")
       retVal.push_back( LgFrTimeVecFloat( nperiods , "2600. 3000. 0. 0. 0. 0. 0. 0." ) );
    else if ( pv[i]->name() == "System_Board_4")
       retVal.push_back( LgFrTimeVecFloat( nperiods , "2800. 3000. 0. 0. 0. 0. 0. 0." ) );
    else
       retVal.push_back( LgFrTimeVecFloat( nperiods , 0.0f ) );
  }
  status = 0;
  return &retVal;
}

// difPWcmrpGetProductMinLotSize global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void*
difPWcmrpGetProductMinLotSize(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
 )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr;
  LgFrTimeVecFloatVector& retVal = 
      *(new LgFrTimeVecFloatVector);
  retVal.reserve( pv.size() );   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<pv.size(); i++ ) { 
       retVal.push_back( LgFrTimeVecFloat( nperiods , 1.0f ) );
  }
  status = 0;
  return &retVal;
}

//--------------------------------------------------------------------
//  
//   Global functions to obtain capacity attributes
//
//-------------------------------------------------------------------

// difPWcmrpGetCapacityUnitCost global function (registered in the attribute table)
// it returns a LgFrFloatVector object.
void *
difPWcmrpGetCapacityUnitCost(
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
      retVal.push_back( 1.46f );
  }
  status = 0;
  return &retVal;
}

// difPWcmrpGetCapacityConstrained global function (registered in the attribute table).
// It returns a LgFrIntVector object
void *
difPWcmrpGetCapacityConstrained(
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

// difPWcmrpGetCapacityObj1ScrapCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difPWcmrpGetCapacityObj1ScrapCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
 )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr;
  LgFrTimeVecFloatVector& retVal = 
      *(new LgFrTimeVecFloatVector);
  retVal.reserve( pv.size() );   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
      retVal.push_back( LgFrTimeVecFloat( nperiods, 0.73f ) );
  }
  status = 0;
  return &retVal;
}

// difPWcmrpGetCapacitySupplyVol global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difPWcmrpGetCapacitySupplyVol(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
 )
{
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr;
  LgFrTimeVecFloatVector& retVal = 
      *(new LgFrTimeVecFloatVector);
  retVal.reserve( pv.size() );   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) { 
    if ( pv[i]->name() == "Board_Assembly")
      retVal.push_back( LgFrTimeVecFloat(nperiods, "900. 900. 900. 900. 900. 900. 900. 900." ) );
    else if ( pv[i]->name() == "Board_Test") 
      retVal.push_back( LgFrTimeVecFloat(nperiods , "1300. 1300. 1300. 1300. 1300. 1300. 1300. 1300." ) );
    else if ( pv[i]->name() == "System_Assembly")    
      retVal.push_back( LgFrTimeVecFloat(nperiods , "1200. 1200. 1200. 1200. 1200. 1200. 1200. 1200." ) );
    else if ( pv[i]->name() == "System_Test")    
      retVal.push_back( LgFrTimeVecFloat(nperiods , "1200. 1200. 1200. 1200. 1200. 1200. 1200. 1200." ) );
    else
      retVal.push_back( LgFrTimeVecFloat(nperiods , 0.0f ) );
  }
  status = 0;
  return &retVal;
}

//--------------------------------------------------------------------
//  
//   Global functions to obtain demands
//
//-------------------------------------------------------------------

// difPWcmrpGetCommittedDemands global function (registering in the attribute table).
// Get committed demands for the perfect world CMRP and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of committed demands).
void *
difPWcmrpGetCommittedDemands(
  const LgFrDataInterfaceImplementation * /* dImp,not used */,
  const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  LgFrDemandVector& retVal = 
      *(new LgFrDemandVector);

  // All demand names
  int nDemands = 13;
  std::string dn[] = 
  {
      "Albert_Einstein",
      "Robert_Oppenheimer",
      "Issac_Newton",
      "Mr_Pythagoras",
      "Turings_Machine_Shop",
      "Joseph_Orlicky",
      "Michael_Faraday",
      "Michael_Faraday",
      "Leonhard_Euler",
      "Thomas_Edisons_Repair",
      "Thomas_Edisons_Repair",
      "Thomas_Edisons_Repair",
      "Thomas_Edisons_Repair"
  };

  // All part names
  std::string pn[] =
  {
      "Computer_1",
      "Computer_2",
      "Computer_3",
      "Computer_4",
      "Computer_5",
      "Computer_6",
      "Computer_7",
      "System_Board_3",
      "Computer_8",
      "System_Board_1",
      "System_Board_2",
      "System_Board_3",
      "System_Board_4"
  };
  
  int i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < nDemands; i++ )
      {
	  LgFrDemand demand = 
	      difEssenPtr->createAndInsertDemand(dn[i], pn[i]);
	  retVal.push_back(demand);
      }
  status = 0;
  return &retVal;    
}

// difPWcmrpGetNewOpportunityDemands global function (registering in the attribute table).
// Get new opportunity demands for perfect world CMRP and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of new opportunity demands).
void *
difPWcmrpGetNewOpportunityDemands(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  LgFrDemandVector& retVal = 
      *(new LgFrDemandVector);

  // All demand names
  int nDemands = 3;
  std::string dn[] = 
  {
      "NewOpportunity",
      "NewOpportunity",
      "NewOpportunity"
  };

  // All part names
  std::string pn[] =
  {
      "Computer_1",
      "Computer_2",
      "Computer_3"
  };
  
  int i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < nDemands; i++ )
      {
	  LgFrDemand demand = 
	      difEssenPtr->createAndInsertDemand(dn[i], pn[i]);
	  retVal.push_back(demand);
      }
  status = 0;
  return &retVal;    
}

// difPWcmrpGetFirmForecastDemands global function (registering in the attribute table).
// Get firm forcast demands for perfect world CMRP and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of firm forcast demands).
void *
difPWcmrpGetFirmForecastDemands(
    const LgFrDataInterfaceImplementation * /* dImp,not used */,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  LgFrDemandVector& retVal = 
      *(new LgFrDemandVector);

  // All demand names
  int nDemands = 12;
  std::string dn[] = 
  {
      "FirmForecast",
      "FirmForecast",
      "FirmForecast",
      "FirmForecast",
      "FirmForecast",
      "FirmForecast",
      "FirmForecast",
      "FirmForecast",
      "FirmForecast",
      "FirmForecast",
      "FirmForecast",
      "FirmForecast"
  };

  // All part names
  std::string pn[] =
  {
      "Computer_1",
      "Computer_2",
      "Computer_3",
      "Computer_4",
      "Computer_5",
      "Computer_6",
      "Computer_7",
      "Computer_8",
      "System_Board_1",
      "System_Board_2",
      "System_Board_3",
      "System_Board_4"
  };
  
  int i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < nDemands; i++ )
      {
	  LgFrDemand demand = 
	      difEssenPtr->createAndInsertDemand(dn[i], pn[i]);
	  retVal.push_back(demand);
      }
  status = 0;
  return &retVal;    
}

// difPWcmrpGetRiskForecastDemands global function (registering in the attribute table).
// Get risk forcast demands for perfect world CMRP and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of risk forcast demands).
void *
difPWcmrpGetRiskForecastDemands(
    const LgFrDataInterfaceImplementation * /* dImp,not used */,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  LgFrDemandVector& retVal = 
      *(new LgFrDemandVector);

  // All demand names
  int nDemands = 3;
  std::string dn[] = 
  {
      "RiskForecast",
      "RiskForecast",
      "RiskForecast" 
  };

  // All part names
  std::string pn[] =
  {
      "Computer_1",
      "Computer_2",
      "Computer_3"
  };
  
  int i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < nDemands; i++ )
      {
	  LgFrDemand demand = 
	      difEssenPtr->createAndInsertDemand(dn[i], pn[i]);
	  retVal.push_back(demand);
      }
  status = 0;
  return &retVal;    
}

//--------------------------------------------------------------------
//  
//   Global functions to obtain demand attributes
//
//-------------------------------------------------------------------

// difPWcmrpGetDemandGrossRev global function (registering in the attribute table).
// Compute total demand gross revenue
// Return: a LgFrFloatVector object (vector of demand gross revenues).
void *
difPWcmrpGetDemandGrossRev(
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
    if      ( dv[i]->name() == "Albert_Einstein" )  retVal.push_back( 111.f );
    else if ( dv[i]->name() == "Robert_Oppenheimer" )  retVal.push_back( 111.f );
    else if ( dv[i]->name() == "Issac_Newton" )  retVal.push_back( 111.f );
    else if ( dv[i]->name() == "Mr_Pythagoras" )  retVal.push_back( 111.f );
    else if ( dv[i]->name() == "Joseph_Orlicky" )  retVal.push_back( 111.f );
    else if ( dv[i]->name() == "Michael_Faraday" )  retVal.push_back( 111.f );
    else if ( dv[i]->name() == "Leonhard_Euler" )  retVal.push_back( 111.f );
    else if ( dv[i]->name() == "Turings_Machine_Shop" )  retVal.push_back( 111.f );
    else if ( dv[i]->name() == "Thomas_Edisons_Repair" )  retVal.push_back( 111.f );
    else if ( dv[i]->name() == "NewOpportunity" )  retVal.push_back( 222.f );
    else if ( dv[i]->name() == "FirmForecast" )  retVal.push_back( 333.f );
    else if ( dv[i]->name() == "RiskForecast" )  retVal.push_back( 444.f );
    else assert( 0 && "illegal demand name for Gross Revenue" );
  }
  status = 0;
  return &retVal;
}

// difPWcmrpGetDemandVol global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difPWcmrpGetDemandVol(
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

  int i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < dv.size(); i++) {
    if ( dv[i]->name() == "Albert_Einstein" ){
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_1" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, "300. 300. 300. 300. 300. 200. 200. 100." ) );
    }  
    else if ( dv[i]->name() == "Robert_Oppenheimer" ){
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_2" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, "450. 350. 350. 350. 350. 150. 150. 150." ) );
    }
    else if ( dv[i]->name() == "Issac_Newton" ){
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_3" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, "500. 500. 400. 400. 300. 300. 300. 300." ) );
    }
    else if ( dv[i]->name() == "Mr_Pythagoras" ){
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_4" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, "600. 600. 600. 300. 300. 300. 200. 200." ) );
    }
    else if ( dv[i]->name() == "Joseph_Orlicky" ){
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_6" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, "750. 750. 750. 750. 600. 600. 600. 600." ) );
    }
    else if ( dv[i]->name() == "Michael_Faraday" ){
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_7" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, "700. 700. 700. 500. 500. 500. 500. 500." ) );
      else if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "System_Board_3" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, "30. 30. 30. 20. 20. 20. 20. 20." ) );
    }
    else if ( dv[i]->name() == "Leonhard_Euler" ){
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_8" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, "900. 900. 800. 800. 800. 600. 600. 600." ) );
    }
    else if (dv[i]->name() == "Turings_Machine_Shop" ){
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_5" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, "800. 800. 800. 800. 500. 500. 500. 500." ) );
    }
    else if (dv[i]->name() == "Thomas_Edisons_Repair" ){
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "System_Board_1" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, "10. 10. 10. 10. 10. 10. 10. 10." ) );
      else if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "System_Board_2" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, "20. 20. 20. 10. 10. 10. 10. 10." ) );
      else if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "System_Board_3" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, "40. 40. 40. 30. 30. 20. 20. 20." ) );
      else if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "System_Board_4" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, "90. 90. 60. 60. 60. 60. 60.  60." ) );
    }    
    else if ( dv[i]->name() == "FirmForecast" ){
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_1" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, "100. 100. 100. 100. 100. 300. 300. 400." ) );
      else if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_2" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, "100. 100. 300. 350. 350. 450. 500. 500." ) );
      else if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_3" )
        retVal.push_back( LgFrTimeVecFloat( nperiods,  "100. 100. 300. 300. 300. 300. 350. 350." ) );
      else if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_4" )
        retVal.push_back( LgFrTimeVecFloat( nperiods,  "100. 100. 400. 400. 400. 400. 750. 750." ) );
      else if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_5" )
        retVal.push_back( LgFrTimeVecFloat( nperiods,   "100. 100. 250. 250. 500. 500. 550. 550." ) );
      else if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_6" )
        retVal.push_back( LgFrTimeVecFloat( nperiods,   "100. 100. 300. 300. 300. 300. 500. 500." ) );
      else if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_7" )
        retVal.push_back( LgFrTimeVecFloat( nperiods,   "100. 100. 100. 500. 500. 500. 500. 500." ) );
      else if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_8" )
        retVal.push_back( LgFrTimeVecFloat( nperiods,   "100. 200. 200. 400. 500. 500. 600. 600." ) );
      else if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "System_Board_1" )
        retVal.push_back( LgFrTimeVecFloat( nperiods,   "10. 10. 20. 20. 20. 20. 20. 20." ) );
      else if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "System_Board_2" )
        retVal.push_back( LgFrTimeVecFloat( nperiods,   "10. 10. 10. 30. 30. 30. 30. 30." ) );
      else if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "System_Board_3" )
        retVal.push_back( LgFrTimeVecFloat( nperiods,   "30. 30. 30. 60. 60. 60. 90. 90." ) );
      else if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "System_Board_4" )
        retVal.push_back( LgFrTimeVecFloat( nperiods,   "30. 30. 30. 60. 60. 60. 90. 90." ) );
    }
    else if ( dv[i]->name() == "RiskForecast" ){
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_1" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, 1.0f ) );
      else if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_2" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, 1.0f ) );
      else if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_3" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, 1.0f ) );
    }      
    else if ( dv[i]->name() == "NewOpportunity" )  
        retVal.push_back( LgFrTimeVecFloat( nperiods, 0.0f ) );
    else assert( 0 && "illegal demand name for Demand Vol" );
  }      
  status = 0;
  return &retVal;
}

// difPWcmrpGetDemanObj1CumShipReward global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difPWcmrpGetDemandObj1CumShipReward(
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
  for ( i = 0; i < dv.size(); i++) {
    if ( dv[i]->name() == "Albert_Einstein" ){
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_1" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, 100.f ) );
    }
    else if ( dv[i]->name() == "Robert_Oppenheimer" ){    
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_2" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, 125.f ) );
    }
    else if ( dv[i]->name() == "Issac_Newton" ){        
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_3" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, 100.f ) );
    }
    else if ( dv[i]->name() == "Mr_Pythagoras" ){     
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_4" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, 125.f ) );
    }
    else if ( dv[i]->name() == "Joseph_Orlicky" ){    
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_6" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, 90.f ) );
    }
    else if ( dv[i]->name() == "Michael_Faraday" ){
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_7" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, 100.f ) );
      else if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "System_Board_3" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, 90.f ) );
    }
    else if ( dv[i]->name() == "Leonhard_Euler" ){    
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_8" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, 110.f ) );
    }
    else if ( dv[i]->name() == "Turings_Machine_Shop" ){
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_5" )
        retVal.push_back( LgFrTimeVecFloat( nperiods, 80.f ) );
    }
    else if ( dv[i]->name() == "Thomas_Edisons_Repair" )  retVal.push_back( LgFrTimeVecFloat( nperiods, 30.f ) );
    else if ( dv[i]->name() == "FirmForecast" )  retVal.push_back( LgFrTimeVecFloat( nperiods, 0.f ) );
    else if ( dv[i]->name() == "RiskForecast" )  retVal.push_back( LgFrTimeVecFloat( nperiods, 0.f ) );
    else assert( 0 && "illegal demand name for Cummulative Shipment Reward" );
  }      
  status = 0;
  return &retVal;
}

// difPWcmrpGetDemanObj1ShipReward global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
void *
difPWcmrpGetDemandObj1ShipReward(
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
  for ( i = 0; i < dv.size(); i++) {
    retVal.push_back( LgFrTimeVecFloat( nperiods, 1.f ) );
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
difPWcmrpDefineBom(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector*, // itemPtrOrdVecPtr,
    LgFrError &, // status, 
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
   WitRun* eater = 
     ((LgFrDataInterfaceEssentialsFromScenario*)difEssenPtr)
     ->mutableWitRun();
   
   witAddBomEntry(eater,"System_Board_1","MEM_Module");
   witAddBomEntry(eater,"System_Board_1","CPU_Type_F");
   witAddBomEntry(eater,"System_Board_1","RAW_Board_F");
   witAddBomEntry(eater,"System_Board_1","Board_Assembly");
   witSetBomEntryUsageRate(eater,"System_Board_1", 3, 0.2f);    
   witAddBomEntry(eater,"System_Board_1","Board_Test");
   witSetBomEntryUsageRate(eater,"System_Board_1", 4, 0.1f);
   

   
   witAddBomEntry(eater,"System_Board_2","MEM_Module");
   witSetBomEntryUsageRate(eater,"System_Board_2", 0, 2); 
   witAddBomEntry(eater,"System_Board_2","CPU_Type_F");
   witAddBomEntry(eater,"System_Board_2","RAW_Board_F");
   witAddBomEntry(eater,"System_Board_2","Board_Assembly");
   witSetBomEntryUsageRate(eater,"System_Board_2", 3, 0.2f);    
   witAddBomEntry(eater,"System_Board_2","Board_Test");
   witSetBomEntryUsageRate(eater,"System_Board_2", 4, 0.2f);   

 
   
   witAddBomEntry(eater,"System_Board_3","MEM_Module");
   witAddBomEntry(eater,"System_Board_3","CPU_Type_S");
   witAddBomEntry(eater,"System_Board_3","RAW_Board_S");
   witAddBomEntry(eater,"System_Board_3","Board_Assembly");
   witSetBomEntryUsageRate(eater,"System_Board_3", 3, 0.1f);    
   witAddBomEntry(eater,"System_Board_3","Board_Test");
   witSetBomEntryUsageRate(eater,"System_Board_3", 4, 0.1f);      


   
   witAddBomEntry(eater,"System_Board_4","MEM_Module");
   witSetBomEntryUsageRate(eater,"System_Board_4", 0, 2);    
   witAddBomEntry(eater,"System_Board_4","CPU_Type_S");
   witAddBomEntry(eater,"System_Board_4","RAW_Board_S");
   witAddBomEntry(eater,"System_Board_4","Board_Assembly");
   witSetBomEntryUsageRate(eater,"System_Board_4", 3, 0.1f);    
   witAddBomEntry(eater,"System_Board_4","Board_Test");
   witSetBomEntryUsageRate(eater,"System_Board_4", 4, 0.2f);     

     
   
   witAddBomEntry(eater,"Computer_1","System_Board_1");
   witAddBomEntry(eater,"Computer_1","Disk_Drive_L");
   witAddBomEntry(eater,"Computer_1","Screen_L");
   witAddBomEntry(eater,"Computer_1","System_Assembly");
   witSetBomEntryUsageRate(eater,"Computer_1", 3, 0.2f);    
   witAddBomEntry(eater,"Computer_1","System_Test");
   witSetBomEntryUsageRate(eater,"Computer_1", 4, 0.3f);
   
   


   witAddBomEntry(eater,"Computer_2","System_Board_2");
   witAddBomEntry(eater,"Computer_2","Disk_Drive_L");
   witAddBomEntry(eater,"Computer_2","Screen_L");
   witAddBomEntry(eater,"Computer_2","System_Assembly");
   witSetBomEntryUsageRate(eater,"Computer_2", 3, 0.2f);    
   witAddBomEntry(eater,"Computer_2","System_Test");
   witSetBomEntryUsageRate(eater,"Computer_2", 4, 0.4f);   
   

   
   witAddBomEntry(eater,"Computer_3","System_Board_3");
   witAddBomEntry(eater,"Computer_3","Disk_Drive_L");
   witAddBomEntry(eater,"Computer_3","Screen_L");
   witAddBomEntry(eater,"Computer_3","System_Assembly");
   witSetBomEntryUsageRate(eater,"Computer_3", 3, 0.2f);    
   witAddBomEntry(eater,"Computer_3","System_Test");
   witSetBomEntryUsageRate(eater,"Computer_3", 4, 0.2f);    

   
   
   witAddBomEntry(eater,"Computer_4","System_Board_4");
   witAddBomEntry(eater,"Computer_4","Disk_Drive_L");
   witAddBomEntry(eater,"Computer_4","Screen_L");
   witAddBomEntry(eater,"Computer_4","System_Assembly");
   witSetBomEntryUsageRate(eater,"Computer_4", 3, 0.2f);    
   witAddBomEntry(eater,"Computer_4","System_Test");
   witSetBomEntryUsageRate(eater,"Computer_4", 4, 0.1f);       
   


   witAddBomEntry(eater,"Computer_5","System_Board_3");
   witAddBomEntry(eater,"Computer_5","Disk_Drive_S");
   witAddBomEntry(eater,"Computer_5","Screen_L");
   witAddBomEntry(eater,"Computer_5","System_Assembly");
   witSetBomEntryUsageRate(eater,"Computer_5", 3, 0.15f);    
   witAddBomEntry(eater,"Computer_5","System_Test");
   witSetBomEntryUsageRate(eater,"Computer_5", 4, 0.15f);       
      
   

   
   witAddBomEntry(eater,"Computer_6","System_Board_4");
   witAddBomEntry(eater,"Computer_6","Disk_Drive_S");
   witAddBomEntry(eater,"Computer_6","Screen_L");
   witAddBomEntry(eater,"Computer_6","System_Assembly");
   witSetBomEntryUsageRate(eater,"Computer_6", 3, 0.15f);    
   witAddBomEntry(eater,"Computer_6","System_Test");
   witSetBomEntryUsageRate(eater,"Computer_6", 4, 0.1f);     
   

   
   witAddBomEntry(eater,"Computer_7","System_Board_3");
   witAddBomEntry(eater,"Computer_7","Disk_Drive_S");
   witAddBomEntry(eater,"Computer_7","Screen_S");
   witAddBomEntry(eater,"Computer_7","System_Assembly");   
   witSetBomEntryUsageRate(eater,"Computer_7", 3, 0.1f);    
   witAddBomEntry(eater,"Computer_7","System_Test");
   witSetBomEntryUsageRate(eater,"Computer_7", 4, 0.1f);   
   

   
   witAddBomEntry(eater,"Computer_8","System_Board_4");
   witAddBomEntry(eater,"Computer_8","Disk_Drive_S");
   witAddBomEntry(eater,"Computer_8","Screen_S");
   witAddBomEntry(eater,"Computer_8","System_Assembly");   
   witSetBomEntryUsageRate(eater,"Computer_8", 3, 0.15f);    
   witAddBomEntry(eater,"Computer_8","System_Test");
   witSetBomEntryUsageRate(eater,"Computer_8", 4, 0.1f);      
   return 0;
}   

#ifdef NDEBUG
#undef NDEBUG
#endif
// tdif1 is a LgFrDataInterfaceImplementationPerfectWorldCMRP dif.
// ts1dupDif is the dif contained in the scenario created by invoking
// the scenario copy contructor on the scenario containing tdif1.
void
difPWcmrpContextTest(
   LgFrDataInterface    	& tdif1,	// testScenario1's
   LgFrDataInterface    	& ts1dupDif,	// testScenario1dup's
   const LgFrScenario           * scenPtr)	// testScenario1's
{
  const float fltEps = .0001f;

  LgFrDataInterfaceImplementation* dImp 
      = tdif1.dataInterfaceImplementation();
  LgFrDataInterfaceImplementation* dImptsi 
      = ts1dupDif.dataInterfaceImplementation();

  // Test title method
  assert( dImp->title() == "internal data set pw1" );
  assert( dImptsi->title()=="internal data set pw1" );
  assert( tdif1.title() == "internal data set pw1");
  assert( ts1dupDif.title() == "internal data set pw1");

  // Register all functions
  registerPWcmrpDifImpFunctions(dImp);

  // Test Get Part global functions
  LgFrDataInterfaceEssentialsFromScenario difEssen(scenPtr);
  LgFrPartVector pv;
  std::string itemSubTypeStr = "material";  
  LgFrPartVector* rawPartsPtr
    = (LgFrPartVector*) tdif1.partAttribute(
	pv, itemSubTypeStr, "rawParts", &difEssen);
  LgFrPartVector raw = *rawPartsPtr;

  assert( raw.size() == 9 );
  assert( raw[1].name() == "CPU_Type_F" );

  LgFrPartVector* productPartsPtr
    = (LgFrPartVector*) tdif1.partAttribute(
	pv, itemSubTypeStr, "productParts", &difEssen);
  LgFrPartVector product = *productPartsPtr;

  assert( product.size() == 12 );
  assert( product[11].name() == "Computer_8" );

  LgFrPartVector* capacityPartsPtr
    = (LgFrPartVector*) tdif1.partAttribute(
	pv, itemSubTypeStr, "capacityParts", &difEssen);
  LgFrPartVector capacity = *capacityPartsPtr;

  assert( capacity.size() == 4 );
  assert( capacity[1].name() == "Board_Test" );

  // Test get raw material attributes functions
  {
  LgFrFloatVector* unitCostPtr
    = (LgFrFloatVector*) tdif1.partAttribute(
	raw, itemSubTypeStr, "rawUnitCost", &difEssen);
  LgFrFloatVector unitCost = *unitCostPtr;
  assert( unitCost.size() == 9 );

  LgFrIntVector* constrainedPtr
      = (LgFrIntVector*) tdif1.partAttribute(
	  raw, itemSubTypeStr, "rawConstrained", &difEssen);
  LgFrIntVector constrained = *constrainedPtr;
  assert( constrained.size() == 9 );
  assert( constrained[0] );
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=1; i<7; i++)
     assert( !constrained[i] );
  assert( constrained[8] );

  LgFrTimeVecFloatVector* obj1ScrapCostPtr
      = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
	  raw, itemSubTypeStr, "rawObj1ScrapCost", &difEssen);
  LgFrTimeVecFloatVector obj1ScrapCost = *obj1ScrapCostPtr;
  assert( obj1ScrapCost.size() == 9 );

  LgFrTimeVecFloatVector* obj1StockCostPtr
      = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
	  raw, itemSubTypeStr, "rawObj1StockCost", &difEssen);
  LgFrTimeVecFloatVector obj1StockCost = *obj1StockCostPtr;
  assert( obj1StockCost.size() == 9 );
  
  LgFrTimeVecFloatVector* supplyVolPtr
      = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
	  raw, itemSubTypeStr, "rawSupplyVol", &difEssen);
  LgFrTimeVecFloatVector supplyVol = *supplyVolPtr;
  assert( supplyVol.size() == 9 );

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
      = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
	  product, itemSubTypeStr, "productCycleTime", &difEssen);
  LgFrTimeVecFloatVector cycleTime = *cycleTimePtr;
  assert( cycleTime.size() == 12 );

  LgFrFloatVector* unitCostPtr
      = (LgFrFloatVector*) tdif1.partAttribute(
	  product, itemSubTypeStr, "productUnitCost", &difEssen);
  LgFrFloatVector unitCost = *unitCostPtr;
  assert( unitCost.size() == 12 );
  
  LgFrIntVector* constrainedPtr
      = (LgFrIntVector*) tdif1.partAttribute(
	  product, itemSubTypeStr, "productConstrained", &difEssen);
  LgFrIntVector constrained = *constrainedPtr;
  assert( constrained.size() == 12 );
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<constrained.size(); i++)
  assert( constrained[i] );

  LgFrIntVector* yieldPtr
      = (LgFrIntVector*) tdif1.partAttribute(
	  product, itemSubTypeStr, "productYield", &difEssen);
  LgFrIntVector yield = *yieldPtr;
  assert( yield.size() == 12 );

  LgFrTimeVecFloatVector* obj1ProdCostPtr
      = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
	  product, itemSubTypeStr, "obj1ProdCost", &difEssen);
  LgFrTimeVecFloatVector obj1ProdCost = *obj1ProdCostPtr;
  assert( obj1ProdCost.size() == 12 );

  LgFrTimeVecFloatVector* obj1ScrapCostPtr
      = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
	  product, itemSubTypeStr, "productObj1ScrapCost", &difEssen);
  LgFrTimeVecFloatVector obj1ScrapCost = *obj1ScrapCostPtr;
  assert( obj1ScrapCost.size() == 12 );
  
  LgFrTimeVecFloatVector* obj1StockCostPtr
      = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
	  product, itemSubTypeStr, "productObj1StockCost", &difEssen);
  LgFrTimeVecFloatVector obj1StockCost = *obj1StockCostPtr;
  assert( obj1StockCost.size() == 12 );
  
  LgFrTimeVecFloatVector* supplyVolPtr
      = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
	  product, itemSubTypeStr, "productSupplyVol", &difEssen);
  LgFrTimeVecFloatVector supplyVol = *supplyVolPtr;
  assert( supplyVol.size() == 12 );

  delete cycleTimePtr;
  delete unitCostPtr;
  delete constrainedPtr;
  delete yieldPtr;
  delete obj1ProdCostPtr;
  delete obj1ScrapCostPtr;
  delete obj1StockCostPtr;
  delete supplyVolPtr;

  }

  delete productPartsPtr;

  // Test get capacity attributes global functions
  {
  LgFrFloatVector* unitCostPtr
      = (LgFrFloatVector*) tdif1.partAttribute(
	  capacity, itemSubTypeStr, "capacityUnitCost", &difEssen);
  LgFrFloatVector unitCost = *unitCostPtr;
  assert( unitCost.size() == 4 );

  LgFrIntVector* constrainedPtr
      = (LgFrIntVector*) tdif1.partAttribute(
	  capacity, itemSubTypeStr, "capacityConstrained", &difEssen);
  LgFrIntVector constrained = *constrainedPtr;
  assert( constrained.size() == 4 );
  assert( constrained[0] );

  LgFrTimeVecFloatVector* obj1ScrapCostPtr
      = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
	  capacity, itemSubTypeStr, "capacityObj1ScrapCost", &difEssen);
  LgFrTimeVecFloatVector obj1ScrapCost = *obj1ScrapCostPtr;
  assert( obj1ScrapCost.size() == 4 );

  LgFrTimeVecFloatVector* supplyVolPtr
      = (LgFrTimeVecFloatVector*) tdif1.partAttribute(
	  capacity, itemSubTypeStr, "capacitySupplyVol", &difEssen);
  LgFrTimeVecFloatVector supplyVol = *supplyVolPtr;
  assert( supplyVol.size() == 4 );

  delete unitCostPtr;
  delete constrainedPtr;
  delete obj1ScrapCostPtr;
  delete supplyVolPtr;

  }

  delete capacityPartsPtr;

// Test Get Demands global functions
  LgFrDemandVector dv;

  LgFrDemandVector* committedDemandPtr
    = (LgFrDemandVector*) tdif1.demandAttribute(
	dv, "committedDemands", &difEssen );
  LgFrDemandVector committed = * committedDemandPtr; 
  assert( committed.size() == 13 ); 

  LgFrDemandVector* newOpportunityDemandPtr
    = (LgFrDemandVector*) tdif1.demandAttribute(
	dv, "newOpportunityDemands", &difEssen );
  LgFrDemandVector newOpportunity = * newOpportunityDemandPtr; 
  assert( newOpportunity.size() == 3 );
  assert( newOpportunity[0].name() == "NewOpportunity" );

  LgFrDemandVector* firmForecastDemandPtr
    = (LgFrDemandVector*) tdif1.demandAttribute(
	dv, "firmForecastDemands", &difEssen );
  LgFrDemandVector firmForecast = * firmForecastDemandPtr;
  assert( firmForecast.size() == 12 );
  assert( firmForecast[1].name() == "FirmForecast" );

  LgFrDemandVector* riskForecastDemandPtr
    = (LgFrDemandVector*) tdif1.demandAttribute(
	dv, "riskForecastDemands", &difEssen );
  LgFrDemandVector riskForecast = * riskForecastDemandPtr;
  assert( riskForecast.size() == 3 );
  assert( riskForecast[0].name() == "RiskForecast" );
  assert( riskForecast[1].name() == "RiskForecast" );
  assert( riskForecast[2].name() == "RiskForecast" );

  // Test get demand attributes global functions
  LgFrFloatVector* committedGrossRevPtr
   = (LgFrFloatVector*) tdif1.demandAttribute(
       committed,"demandGrossRev", &difEssen);
  LgFrFloatVector committedGrossRev = *committedGrossRevPtr;
  assert( committedGrossRev.size() == 13 );

  LgFrFloatVector* newOpportunityGrossRevPtr
      = (LgFrFloatVector*) tdif1.demandAttribute(
	  newOpportunity,"demandGrossRev", &difEssen);
  LgFrFloatVector newOpportunityGrossRev = *newOpportunityGrossRevPtr;
  assert( newOpportunityGrossRev.size() == 3 );

  LgFrFloatVector* firmForecastGrossRevPtr
   = (LgFrFloatVector*) tdif1.demandAttribute(
       firmForecast,"demandGrossRev", &difEssen);
  LgFrFloatVector firmForecastGrossRev = *firmForecastGrossRevPtr;
  assert( firmForecastGrossRev.size() == 12 );

  LgFrFloatVector* riskForecastGrossRevPtr
   = (LgFrFloatVector*) tdif1.demandAttribute(
       riskForecast,"demandGrossRev", &difEssen);
  LgFrFloatVector riskForecastGrossRev = *riskForecastGrossRevPtr;
  assert( riskForecastGrossRev.size() == 3 );

  delete riskForecastGrossRevPtr;
  delete firmForecastGrossRevPtr;
  delete newOpportunityGrossRevPtr;
  delete committedGrossRevPtr;
  delete committedDemandPtr;
  delete newOpportunityDemandPtr;
  delete firmForecastDemandPtr;
  delete riskForecastDemandPtr;

}

