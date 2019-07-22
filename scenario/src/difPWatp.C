#include <math.h>
#include <iostream.h>

#include <rw/rstream.h>
#include <rw/cstring.h>

#include <scenario/src/dif.h>
#include <scenario/src/difImpl.h>
#include <scenario/src/difPWatp.h>
#include <scenario/src/calendar.h>
#include <scenario/src/difEssSc.h>
#include <scenario/src/timeVecs.h>
#include <scenario/src/idnumber.h>

// added for vacpp - vra
//implement (RWGVector, int)

//-------------------------------------------------------------------
//  
//   Create a LgFrDataInterfaceImplementation object
//
//-------------------------------------------------------------------

LgFrDataInterfaceImplementation * createPWatpDifImp()
{
   return new LgFrDataInterfaceImplementation;
} 

// -----------------------------------------------------------------------
//
//  Register all functions
//
// -----------------------------------------------------------------------

void
registerPWatpDifImpFunctions(
    LgFrDataInterfaceImplementation * dImp) 
{
    // Register all raw material attribute functions
    dImp->registerFunction("rawUnitCost", material, part,
			   vectorFloat, difPWatpGetRawUnitCost);
    dImp->registerFunction("rawProcCost", material, part,
			   vectorFloat, difPWatpGetRawProcCost);
    dImp->registerFunction("rawConstrained", material, part,
			   vectorInt, difPWatpGetRawConstrained);
    dImp->registerFunction("rawProcLeadTime", material, part,
			   timeVecFloat, difPWatpGetRawProcLeadTime);
    dImp->registerFunction("rawObj1ScrapCost", material, part,
			   timeVecFloat, difPWatpGetRawObj1ScrapCost);
    dImp->registerFunction("rawObj1StockCost", material, part,
			   timeVecFloat, difPWatpGetRawObj1StockCost);
    dImp->registerFunction("rawSupplyVol", material, part,
			   timeVecFloat, difPWatpGetRawSupplyVol);

    // Register all product attribute functions
    dImp->registerFunction("productUnitCost", material, part,
			   vectorFloat, difPWatpGetProductUnitCost);
    dImp->registerFunction("productProcCost", material, part,
			   vectorFloat, difPWatpGetProductProcCost);
    dImp->registerFunction("productConstrained", material, part,
			   vectorInt, difPWatpGetProductConstrained);
    dImp->registerFunction("productProcLeadTime", material, part,
			   timeVecFloat, difPWatpGetProductProcLeadTime);
    dImp->registerFunction("productYield", material, part,
			   vectorInt, difPWatpGetProductYield);
    dImp->registerFunction("productCycleTime", material, part,
			   timeVecFloat, difPWatpGetProductCycleTime);
    dImp->registerFunction("obj1ProdCost", material, part,
			   timeVecFloat, difPWatpGetObj1ProdCost);
    dImp->registerFunction("productObj1ScrapCost", material, part,
			   timeVecFloat, difPWatpGetProductObj1ScrapCost);
    dImp->registerFunction("productObj1StockCost", material, part,
			   timeVecFloat, difPWatpGetProductObj1StockCost);
    dImp->registerFunction("productSupplyVol", material, part,
			   timeVecFloat, difPWatpGetProductSupplyVol);

    // Register all capacity attribute functions
    dImp->registerFunction("capacityUnitCost", material, part,
			   vectorFloat, difPWatpGetCapacityUnitCost);
    dImp->registerFunction("capacityConstrained", material, part,
			   vectorInt, difPWatpGetCapacityConstrained);
    dImp->registerFunction("capacityObj1ScrapCost", material, part,
			   timeVecFloat, difPWatpGetCapacityObj1ScrapCost);
    dImp->registerFunction("capacitySupplyVol", material, part,
			   timeVecFloat, difPWatpGetCapacitySupplyVol);

    //  need to have demandTable (similar to attrTable)
    // Register all functions to obtain demands
    dImp->registerFunction("committedDemands", demandType, demand,
		   orderedVecDemand, difPWatpGetCommittedDemands);
    dImp->registerFunction("newOpportunityDemands", demandType, demand,
		   orderedVecDemand, difPWatpGetNewOpportunityDemands);
    dImp->registerFunction("firmForecastDemands", demandType, demand,
			   orderedVecDemand, difPWatpGetFirmForecastDemands);
    dImp->registerFunction("riskForecastDemands", demandType, demand,
			   orderedVecDemand, difPWatpGetRiskForecastDemands);

    //  need to have demandtable (similar to attrTable)
    // Register all demand attribute functions

    dImp->registerFunction("demandGrossRev", demandType, demand,
			   vectorFloat, difPWatpGetDemandGrossRev);
    dImp->registerFunction("demandVol", demandType, demand,
			   timeVecFloat, difPWatpGetDemandVol);
    dImp->registerFunction("demandObj1CumShipReward", demandType, demand,
			   timeVecFloat, difPWatpGetDemandObj1CumShipReward);
    dImp->registerFunction("demandObj1ShipReward", demandType, demand,
			   timeVecFloat, difPWatpGetDemandObj1ShipReward);

    //  need to have partTable (similar to attrTable)
    // Register all functions to obtain parts

    dImp->registerFunction("rawParts", material, part,
			   orderedVecPart, difPWatpGetRawParts);
    dImp->registerFunction("productParts", material, part,
			   orderedVecPart, difPWatpGetProductParts);
    dImp->registerFunction("capacityParts", material, part,
			   orderedVecPart, difPWatpGetCapacityParts);

    // Register define bom globle function
    dImp->registerFunction("defineBom", material, part,
			   timeVecFloat, difPWatpDefineBom);

}

//-------------------------------------------------------------------
//  
//   Global functions to obtain the defined parts and capacities.
//
//-------------------------------------------------------------------

// Return vector of raw materials.
// difPWatpGetRawParts global function (registering in the attribute table).
// Get raw parts for the perfect world ATP and insert them into set_of_part
// Return: a LgFrOrderedVectorPart object (vector of raw parts).

void *
difPWatpGetRawParts(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrOrderedVectorItemPtr* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    ) 
{
  LgFrOrderedVectorPart& retVal = *(new LgFrOrderedVectorPart);
  LgFrPart part = difEssenPtr->createAndInsertPart("MEM_Module");
  retVal.insert(part);
  part = difEssenPtr->createAndInsertPart("CPU_Type_F");
  retVal.insert(part);
  part = difEssenPtr->createAndInsertPart("CPU_Type_S");
  retVal.insert(part);
  part = difEssenPtr->createAndInsertPart("RAW_Board_F");
  retVal.insert(part);
  part = difEssenPtr->createAndInsertPart("RAW_Board_S");
  retVal.insert(part);
  part = difEssenPtr->createAndInsertPart("Disk_Drive_L");
  retVal.insert(part);
  part = difEssenPtr->createAndInsertPart("Disk_Drive_S");
  retVal.insert(part);
  part = difEssenPtr->createAndInsertPart("Screen_L");
  retVal.insert(part);
  part = difEssenPtr->createAndInsertPart("Screen_S");
  retVal.insert(part);
  status = 0;
  return &retVal;
}

// difPWatpGetProdcutParts global function (registering in the attribute table).
// Get product parts for the perfect world ATP and insert it into set_of_part
// Return: a LgFrOrderedVectorPart object (vector of producible parts)
void*
difPWatpGetProductParts(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrOrderedVectorItemPtr* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrOrderedVectorPart& retVal = *(new LgFrOrderedVectorPart);

  // Subassemblies:
  LgFrPart part = difEssenPtr->createAndInsertPart("System_Board_1");
  retVal.insert(part);
  part = difEssenPtr->createAndInsertPart("System_Board_2");
  retVal.insert(part);
  part = difEssenPtr->createAndInsertPart("System_Board_3");
  retVal.insert(part);
  part = difEssenPtr->createAndInsertPart("System_Board_4");
  retVal.insert(part);

  // Final Products
  part = difEssenPtr->createAndInsertPart("Computer_1");
  retVal.insert(part);
  part = difEssenPtr->createAndInsertPart("Computer_2");
  retVal.insert(part);
  part = difEssenPtr->createAndInsertPart("Computer_3");
  retVal.insert(part);
  part = difEssenPtr->createAndInsertPart("Computer_4");
  retVal.insert(part);
  part = difEssenPtr->createAndInsertPart("Computer_5");
  retVal.insert(part);
  part = difEssenPtr->createAndInsertPart("Computer_6");
  retVal.insert(part);
  part = difEssenPtr->createAndInsertPart("Computer_7");
  retVal.insert(part);
  part = difEssenPtr->createAndInsertPart("Computer_8");
  retVal.insert(part);
  status = 0;
  return &retVal;
}

// difPWatpGetCapacityParts global function (registering in the attribute table).
// Get capacity parts for the perfect world ATP and insert it into set_of_part
// Return: a LgFrOrderedVectorPart object (vector of capacity parts)
void*
difPWatpGetCapacityParts(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrOrderedVectorItemPtr* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    ) 
{
  LgFrOrderedVectorPart& retVal = *(new LgFrOrderedVectorPart);

  LgFrPart part = difEssenPtr->createAndInsertPart("Board_Assembly");
  retVal.insert(part);
  part = difEssenPtr->createAndInsertPart("Board_Test");
  retVal.insert(part);
  part = difEssenPtr->createAndInsertPart("System_Assembly");
  retVal.insert(part);
  part = difEssenPtr->createAndInsertPart("System_Test");
  retVal.insert(part);
  status = 0;
  return &retVal;
}

//-------------------------------------------------------------------
//  
//   Global functions to obtain raw material attributes
//
//-------------------------------------------------------------------

// difPWatpGetRawUnitCost global function (registered in the attribute table)
// it returns a LgFrVectorFloat object.
void *
difPWatpGetRawUnitCost(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * /* difEssenPtr not used */
    )
{
  LgFrOrderedVectorItemPtr pv = *itemPtrOrdVecPtr;
  LgFrVectorFloat& retVal = *(new LgFrVectorFloat ( pv.entries() ));

  for ( size_t i=0; i<pv.entries(); i++ ) {
//    assert( setOfParts().contains( pv[i] ) );
    retVal[i] =  1.15f ;
  }
  status = 0;
  return &retVal;
}


// difPWatpGetRawProcCost global function (registered in the attribute table).
// It returns a LgFrVectorFloat object.
void *
difPWatpGetRawProcCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * /* difEssenPtr, not used */
    )
{
  LgFrOrderedVectorItemPtr pv = *itemPtrOrdVecPtr; 
  LgFrVectorFloat& retVal = *(new LgFrVectorFloat ( pv.entries() ));
  for ( size_t i=0; i<pv.entries(); i++ ) {
//    assert( setOfParts().contains( pv[i] ) );
    retVal[i] = 1.23f;
  }
  status = 0;
  return &retVal;
}

// difPWatpGetRawConstrained global function (registered in the attribute table).
// It returns a LgFrVectorInt object
void *
difPWatpGetRawConstrained(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * /* difEssenPtr, not used */
    )
{
  LgFrOrderedVectorItemPtr pv = *itemPtrOrdVecPtr; 
  LgFrVectorInt& retVal = *(new LgFrVectorInt ( pv.entries() ));

  for ( size_t i=0; i<pv.entries(); i++ ) {
//    assert( setOfParts().contains( pv[i] ) );    
    if ( pv[i]->name() == "MEM_Module" )
       retVal[i] =  TRUE;
    else if ( pv[i]->name() == "Screen_S" )
       retVal[i] =  TRUE;
    else
       retVal[i] = FALSE;
  }
  status = 0;
  return &retVal;
} 


// difPWatpGetRawProcLeadTime global function (registered in the attribute table).
// It returns a LgFrVectorTimeVecFloat object.
void *
difPWatpGetRawProcLeadTime(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrOrderedVectorItemPtr pv = *itemPtrOrdVecPtr;
  LgFrVectorTimeVecFloat& retVal = 
      *(new LgFrVectorTimeVecFloat( pv.entries() ));   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  for ( size_t i = 0; i < pv.entries(); i++ ) { 
    if ( pv[i]->name() == "MEM_Module" )
       retVal[i] =  LgFrTimeVecFloat( nperiods , 8.0 );
    else if ( pv[i]->name() == "CPU_Type_F" )
       retVal[i] =  LgFrTimeVecFloat( nperiods , 3.0 );
    else if ( pv[i]->name() == "CPU_Type_S" )
       retVal[i] =  LgFrTimeVecFloat( nperiods , 2.0 );
    else if ( pv[i]->name() == "Screen_S" )
       retVal[i] =  LgFrTimeVecFloat( nperiods , 8.0 );
    else if ( pv[i]->name() == "Disk_Drive_L" )
       retVal[i] =  LgFrTimeVecFloat( nperiods , 1.0 );
    else if ( pv[i]->name() == "Disk_Drive_S" )
       retVal[i] =  LgFrTimeVecFloat( nperiods , 1.0 );
    else
       retVal[i] =  LgFrTimeVecFloat( nperiods , 0.0 );      
  }
  status = 0;
  return &retVal;
}

// difPWatpGetRawObj1ScrapCost global function (registered in the attribute table).
// Return: a LgFrVectorTimeVecFloat object.
void *
difPWatpGetRawObj1ScrapCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrOrderedVectorItemPtr pv = *itemPtrOrdVecPtr;
  LgFrVectorTimeVecFloat& retVal = 
      *(new LgFrVectorTimeVecFloat( pv.entries() ));   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  for ( size_t i = 0; i < pv.entries(); i++ ) {
      retVal[i] =  LgFrTimeVecFloat( nperiods, 0.12f );
  }
  status = 0;
  return &retVal;
}

// difPWatpGetRawObj1StockCost global function (registered in the attribute table).
// Return: a LgFrVectorTimeVecFloat object.
void *
difPWatpGetRawObj1StockCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrOrderedVectorItemPtr pv = *itemPtrOrdVecPtr;
  LgFrVectorTimeVecFloat& retVal = 
      *(new LgFrVectorTimeVecFloat( pv.entries() ));   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  for ( size_t i = 0; i < pv.entries(); i++ ) {
      retVal[i] =  LgFrTimeVecFloat( nperiods, 0.10f );
  }
  status = 0;
  return &retVal;
}

// difPWatpGetRawSupplyVol global function (registered in the attribute table).
// Return: a LgFrVectorTimeVecFloat object.
void *
difPWatpGetRawSupplyVol(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrOrderedVectorItemPtr pv = *itemPtrOrdVecPtr;
  LgFrVectorTimeVecFloat& retVal = 
      *(new LgFrVectorTimeVecFloat( pv.entries() ));   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int np = calendar.nPeriods();  // # periods in scenario calendar

  for ( size_t i=0; i<pv.entries(); i++ ) { 
//    assert( setOfParts().contains( pv[i] ) );

    if ( pv[i]->name() == "MEM_Module" )
       retVal[i] =  
         LgFrTimeVecFloat( np , 
         "37200. 3720. 3720. 3720. 3720. 3720. 3720. 3720" );

    else if ( pv[i]->name() == "CPU_Type_F" )
       retVal[i] =  
         LgFrTimeVecFloat( np , 
         "400. 400. 800. 800. 800. 800. 800. 800." );

    else if ( pv[i]->name() == "CPU_Type_S" )    
       retVal[i] =  
         LgFrTimeVecFloat( np , 
         "1500. 1500. 1500. 1000. 1000. 1000. 1000. 1000." );

    else if ( pv[i]->name() == "RAW_Board_F" )    
       retVal[i] =  
         LgFrTimeVecFloat( np , 
         "300. 225. 200. 775. 720. 720. 720. 720." );

    else if ( pv[i]->name() == "RAW_Board_S" )    
       retVal[i] =  
         LgFrTimeVecFloat( np , 
         "1100. 1100. 1000. 1000. 1000. 1000. 1000. 1000." );

    else if ( pv[i]->name() == "Disk_Drive_L" )    
       retVal[i] =  
         LgFrTimeVecFloat( np , 
         "1000. 1000. 900. 900. 900. 850. 850. 850." );

    else if ( pv[i]->name() == "Disk_Drive_S" )    
       retVal[i] =  
         LgFrTimeVecFloat( np , 
         "1585. 1585. 1585. 1585. 1585. 1585. 1585. 1585." );

    else if ( pv[i]->name() == "Screen_L" )
       retVal[i] =  
         LgFrTimeVecFloat( np , 
         "2085. 2085. 2085. 2085. 2085. 2085. 2085. 2085." );

    else if ( pv[i]->name() == "Screen_S")
       retVal[i] =  
         LgFrTimeVecFloat( np , 
         "100. 100. 200. 200. 200. 300. 100. 100." );

    else
       assert( 1==0 && "Unrecognized raw part name" );
       // retVal[i] =  LgFrTimeVecFloat( numberOfPeriods() , 0.0 );

  } 
  status = 0; 
  return &retVal;
}

//--------------------------------------------------------------------
//  
//   Global functions to obtain producible part attributes
//
//-------------------------------------------------------------------


// difPWatpGetProductProcCost global function (registered in the attribute table).
// It returns a LgFrVectorFloat object.
void *
difPWatpGetProductProcCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * /* difEssenPtr, not used */
    )
{
  LgFrOrderedVectorItemPtr pv = *itemPtrOrdVecPtr; 
  LgFrVectorFloat& retVal = *(new LgFrVectorFloat ( pv.entries() ));

  for ( size_t i = 0; i < pv.entries(); i++ ) {
      retVal[i] = 1.43f;
  }
  status = 0;
  return &retVal;
}

void *
difPWatpGetProductUnitCost(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * /* difEssenPtr, not used */
    )
{
  LgFrOrderedVectorItemPtr pv = *itemPtrOrdVecPtr; 
  LgFrVectorFloat& retVal = *(new LgFrVectorFloat ( pv.entries() ));

  for ( size_t i=0; i<pv.entries(); i++ ) {
//    assert( setOfParts().contains( pv[i] ) );
    retVal[i] = 3.82f;
  }
  status = 0;
  return &retVal;
}

// difPWatpGetProductConstrained global function (registered in the attribute table).
// It returns a LgFrVectorInt object
void *
difPWatpGetProductConstrained(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * /* difEssenPtr, not used */
    )
{
  LgFrOrderedVectorItemPtr pv = *itemPtrOrdVecPtr; 
  LgFrVectorInt& retVal = *(new LgFrVectorInt ( pv.entries() ));

  for ( size_t i=0; i<pv.entries(); i++ ) {
//    assert( setOfParts().contains( pv[i] ) );
    retVal[i] =  TRUE;    

  }
  status = 0;
  return &retVal;
}

// difPWatpGetProductYield global function (registered in the attribute table).
// Return: a LgFrVectorFloat object
void *
difPWatpGetProductYield(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * /* difEssenPtr, not used */
    )
{
  LgFrOrderedVectorItemPtr pv = *itemPtrOrdVecPtr;
  LgFrVectorInt& retVal = *(new LgFrVectorInt ( pv.entries() ));

  for ( size_t i=0; i<pv.entries(); i++ ) {
//    assert( setOfParts().contains( pv[i] ) );
    retVal[i] =  100;
  }
  status = 0;
  return &retVal;
}


// obj1ProdCost, obj1ScrapCost, obj1StockCost, supplyVol, procLeadTime,
// cycleTime

// difPWatpGetProductProcLeadTime global function (registered in the attribute table).
// It returns a LgFrVectorTimeVecFloat object.
void *
difPWatpGetProductProcLeadTime(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrOrderedVectorItemPtr pv = *itemPtrOrdVecPtr;
  LgFrVectorTimeVecFloat& retVal = 
      *(new LgFrVectorTimeVecFloat( pv.entries() ));
  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  for ( size_t i=0; i<pv.entries(); i++ ) {
//    assert( setOfParts().contains( pv[i] ) );    
    retVal[i] =  LgFrTimeVecFloat( nperiods , -2.0 );
  } 
  status = 0; 
  return &retVal;
}

// difPWatpGetProductCycleTime global function (registered in the attribute table).
// Return: a LgFrVectorTimeVecFloat object.
void *
difPWatpGetProductCycleTime(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrOrderedVectorItemPtr pv = *itemPtrOrdVecPtr;
  LgFrVectorTimeVecFloat& retVal = 
      *(new LgFrVectorTimeVecFloat( pv.entries() ));   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  for ( size_t i = 0; i < pv.entries(); i++ ) {
    if ( pv[i]->name() == "System_Board_1" )
        retVal[i] = LgFrTimeVecFloat( nperiods, 1.2f );
    else if ( pv[i]->name() == "System_Board_2" )
        retVal[i] = LgFrTimeVecFloat( nperiods, 1.9f );
    else if (pv[i]->name() == "System_Board_3")
        retVal[i] = LgFrTimeVecFloat( nperiods, 1.5f );
    else if (pv[i]->name() == "System_Board_4")
        retVal[i] = LgFrTimeVecFloat( nperiods, 1.7f );
    else
       retVal[i] =  LgFrTimeVecFloat( nperiods , 0.0f );      
  }  
  status = 0;
  return &retVal;
}

// difPWatpGetObj1ProdCost global function (registered in the attribute table).
// Return: a LgFrVectorTimeVecFloat object.
void *
difPWatpGetObj1ProdCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrOrderedVectorItemPtr pv = *itemPtrOrdVecPtr;
  LgFrVectorTimeVecFloat& retVal = 
      *(new LgFrVectorTimeVecFloat( pv.entries() ));   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  for ( size_t i = 0; i < pv.entries(); i++ ) {
    retVal[i] =  LgFrTimeVecFloat( nperiods, 3.4f );
  }
  status = 0;
  return &retVal;
}


// difPWatpGetProductObj1ScrapCost global function (registered in the attribute table).
// Return: a LgFrVectorTimeVecFloat object.
void *
difPWatpGetProductObj1ScrapCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrOrderedVectorItemPtr pv = *itemPtrOrdVecPtr;
  LgFrVectorTimeVecFloat& retVal = 
      *(new LgFrVectorTimeVecFloat( pv.entries() ));   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  for ( size_t i = 0; i < pv.entries(); i++ ) {
      retVal[i] =  LgFrTimeVecFloat( nperiods, 1.0 );
  }
  status = 0;
  return &retVal;
}


// difPWatpGetProductObj1StockCost global function (registered in the attribute table).
// Return: a LgFrVectorTimeVecFloat object.
void *
difPWatpGetProductObj1StockCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrOrderedVectorItemPtr pv = *itemPtrOrdVecPtr;
  LgFrVectorTimeVecFloat& retVal = 
      *(new LgFrVectorTimeVecFloat( pv.entries() ));   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  for ( size_t i = 0; i < pv.entries(); i++ ) {
      retVal[i] =  LgFrTimeVecFloat( nperiods, 0.83f );
  }
  status = 0;
  return &retVal;
}

// difPWatpGetProductSupplyVol global function (registered in the attribute table).
// Return: a LgFrVectorTimeVecFloat object.
void *
difPWatpGetProductSupplyVol(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrOrderedVectorItemPtr pv = *itemPtrOrdVecPtr;
  LgFrVectorTimeVecFloat& retVal = 
      *(new LgFrVectorTimeVecFloat( pv.entries() ));   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int np = calendar.nPeriods();  // # periods in scenario calendar

  for ( size_t i=0; i<pv.entries(); i++ ) {
//    assert( setOfParts().contains( pv[i] ) );
    if ( pv[i]->name() == "System_Board_1" )
       retVal[i] =  
         LgFrTimeVecFloat( np , 
         "100. 0. 0. 0. 0. 0. 0. 0." );

    else if ( pv[i]->name() == "System_Board_2" )
       retVal[i] =  
         LgFrTimeVecFloat( np , 
         "100. 0. 0. 0. 0. 0. 0. 0." );

    else if ( pv[i]->name() == "System_Board_3" )    
       retVal[i] =  
         LgFrTimeVecFloat( np , 
         "100. 0. 0. 0. 0. 0. 0. 0." );
    
    else if ( pv[i]->name() == "System_Board_4" )    
       retVal[i] =  
         LgFrTimeVecFloat( np ,
         "100. 0. 0. 0. 0. 0. 0. 0." );
    
    else retVal[i] =  LgFrTimeVecFloat( np , 0.0 );    
  }
  status = 0;
  return &retVal;
}


//--------------------------------------------------------------------
//  
//   Global functions to obtain capacity attributes
//
//-------------------------------------------------------------------

// difPWatpGetCapacityUnitCost global function (registered in the attribute table)
// it returns a LgFrVectorFloat object.
void *
difPWatpGetCapacityUnitCost(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * /* difEssenPtr, not used */
    )
{
  LgFrOrderedVectorItemPtr pv = *itemPtrOrdVecPtr; 
  LgFrVectorFloat& retVal = *(new LgFrVectorFloat ( pv.entries() ));

  for ( size_t i = 0; i < pv.entries(); i++ ) {
      retVal[i] = 1.46f;
  }
  status = 0;
  return &retVal;
}

// difPWatpGetCapacityConstrained global function (registered in the attribute table).
// It returns a LgFrVectorInt object
void *
difPWatpGetCapacityConstrained(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * /* difEssenPtr, not used */
    )
{
  LgFrOrderedVectorItemPtr pv = *itemPtrOrdVecPtr; 
  LgFrVectorInt& retVal = *(new LgFrVectorInt ( pv.entries() ));

  for ( size_t i = 0; i < pv.entries(); i++ ) {
      retVal[i] =  TRUE;
  }
  status = 0;
  return &retVal; 
}

// difPWatpGetCapacityObj1ScrapCost global function (registered in the attribute table).
// Return: a LgFrVectorTimeVecFloat object.
void *
difPWatpGetCapacityObj1ScrapCost(
    const LgFrDataInterfaceImplementation * /* difImp, not used */, 
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrOrderedVectorItemPtr pv = *itemPtrOrdVecPtr;
  LgFrVectorTimeVecFloat& retVal = 
      *(new LgFrVectorTimeVecFloat( pv.entries() ));   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  for ( size_t i = 0; i < pv.entries(); i++ ) {
      retVal[i] =  LgFrTimeVecFloat( nperiods, 0.73f );
  }
  status = 0;
  return &retVal;
}


// difPWatpGetCapacitySupplyVol global function (registered in the attribute table).
// Return: a LgFrVectorTimeVecFloat object.
void *
difPWatpGetCapacitySupplyVol(
    const LgFrDataInterfaceImplementation * /* difImp, not used */,
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrOrderedVectorItemPtr pv = *itemPtrOrdVecPtr;
  LgFrVectorTimeVecFloat& retVal = 
      *(new LgFrVectorTimeVecFloat( pv.entries() ));   

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  for ( size_t i=0; i<pv.entries(); i++ ) {
//    assert( setOfParts().contains( pv[i] ) );
    if ( pv[i]->name() == "Board_Assembly")
      retVal[i] = LgFrTimeVecFloat( nperiods, 1600. );
    else if ( pv[i]->name() == "Board_Test") 
      retVal[i] =  LgFrTimeVecFloat( nperiods , 2000. );
    else if ( pv[i]->name() == "System_Assembly")    
      retVal[i] =  LgFrTimeVecFloat( nperiods , 2300. );
    else if ( pv[i]->name() == "System_Test")    
      retVal[i] =  LgFrTimeVecFloat( nperiods , 2600. );
    else
      retVal[i] =  LgFrTimeVecFloat( nperiods , 0.0 );
  }
  status = 0;  
  return &retVal;
}

//--------------------------------------------------------------------
//  
//   Global functions to obtain demands
//
//-------------------------------------------------------------------

// difPWatpGetCommittedDemands global function (registering in the attribute table).
// Get committed demands for the perfect world ATP and insert them into set_of_demand
// Return: a LgFrOrderedVectorDemand object (vector of committed demands).
void *
difPWatpGetCommittedDemands(
  const LgFrDataInterfaceImplementation * /* dImp,not used */,
  const LgFrOrderedVectorItemPtr* /* itemPtrOrdVecPtr, not used */,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  LgFrOrderedVectorDemand& retVal = 
      *(new LgFrOrderedVectorDemand);

  // All demand names
  int nDemands = 9;
  RWCString dn[] = 
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
  };

  // All part names
  RWCString pn[] =
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
  };
  
  for ( int i = 0; i < nDemands; i++ )
      {
	  LgFrDemand demand = 
	      difEssenPtr->createAndInsertDemand(dn[i], pn[i]);
	  retVal.insert(demand);
      }
  status = 0;
  return &retVal;    
}

// difPWatpGetNewOpportunityDemands global function (registering in the attribute table).
// Get new opportunity demands for perfect world ATP and insert them into set_of_demand
// Return: a LgFrOrderedVectorDemand object (vector of new opportunity demands).
void *
difPWatpGetNewOpportunityDemands(
    const LgFrDataInterfaceImplementation * /* dImp, not used */,
    const LgFrOrderedVectorItemPtr* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  LgFrOrderedVectorDemand& retVal = 
      *(new LgFrOrderedVectorDemand);

  // All demand names
  int nDemands = 3;
  RWCString dn[] = 
  {
      "NewOpportunity",
      "NewOpportunity",
      "NewOpportunity"
  };

  // All part names
  RWCString pn[] =
  {
      "Computer_1",
      "Computer_2",
      "Computer_3"
  };
  
  for ( int i = 0; i < nDemands; i++ )
      {
	  LgFrDemand demand = 
	      difEssenPtr->createAndInsertDemand(dn[i], pn[i]);
	  retVal.insert(demand);
      }
  status = 0;
  return &retVal;    
} 


// difPWatpGetFirmForecastDemands global function (registering in the attribute table).
// Get firm forcast demands for perfect world ATP and insert them into set_of_demand
// Return: a LgFrOrderedVectorDemand object (vector of firm forcast demands).
void *
difPWatpGetFirmForecastDemands(
    const LgFrDataInterfaceImplementation * /* dImp,not used */,
    const LgFrOrderedVectorItemPtr* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  LgFrOrderedVectorDemand& retVal = 
      *(new LgFrOrderedVectorDemand);

  // All demand names
  int nDemands = 8;
  RWCString dn[] = 
  {
      "FirmForecast",
      "FirmForecast",
      "FirmForecast",
      "FirmForecast",
      "FirmForecast",
      "FirmForecast",
      "FirmForecast",
      "FirmForecast",
  };

  // All part names
  RWCString pn[] =
  {
      "Computer_1",
      "Computer_2",
      "Computer_3",
      "Computer_4",
      "Computer_5",
      "Computer_6",
      "Computer_7",
      "Computer_8",
  };
  
  for ( int i = 0; i < nDemands; i++ )
      {
	  LgFrDemand demand = 
	      difEssenPtr->createAndInsertDemand(dn[i], pn[i]);
	  retVal.insert(demand);
      }
  status = 0;
  return &retVal;    
}


// difPWatpGetRiskForecastDemands global function (registering in the attribute table).
// Get risk forcast demands for perfect world ATP and insert them into set_of_demand
// Return: a LgFrOrderedVectorDemand object (vector of risk forcast demands).
void *
difPWatpGetRiskForecastDemands(
    const LgFrDataInterfaceImplementation * /* dImp,not used */,
    const LgFrOrderedVectorItemPtr* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr) 
{
  LgFrOrderedVectorDemand& retVal = 
      *(new LgFrOrderedVectorDemand);

  // All demand names
  int nDemands = 3;
  RWCString dn[] = 
  {
      "RiskForecast",
      "RiskForecast",
      "RiskForecast" 
  };

  // All part names
  RWCString pn[] =
  {
      "Computer_1",
      "Computer_2",
      "Computer_3"
  };
  
  for ( int i = 0; i < nDemands; i++ )
      {
	  LgFrDemand demand = 
	      difEssenPtr->createAndInsertDemand(dn[i], pn[i]);
	  retVal.insert(demand);
      }
  status = 0;
  return &retVal;    
}


//--------------------------------------------------------------------
//  
//   Global functions to obtain demand attributes
//
//-------------------------------------------------------------------

// difPWatpGetDemandGrossRev global function (registering in the attribute table).
// Compute total demand gross revenue
// Return: a LgFrVectorFloat object (vector of demand gross revenues).
void *
difPWatpGetDemandGrossRev(
    const LgFrDataInterfaceImplementation * /* difImp,  not used */,
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * /* difEssenPtr, not used */
    )
{
  LgFrOrderedVectorItemPtr dv = *itemPtrOrdVecPtr; 
  LgFrVectorFloat& retVal = *(new LgFrVectorFloat( dv.entries() ));    
  
  for ( size_t i = 0; i < dv.entries(); i++ ) {
    if      ( dv[i]->name() == "Albert_Einstein" )  retVal[i] = 111.;
    else if ( dv[i]->name() == "Robert_Oppenheimer" )  retVal[i] = 111.;
    else if ( dv[i]->name() == "Issac_Newton" )  retVal[i] = 111.;
    else if ( dv[i]->name() == "Mr_Pythagoras" )  retVal[i] = 111.;
    else if ( dv[i]->name() == "Joseph_Orlicky" )  retVal[i] = 111.;
    else if ( dv[i]->name() == "Michael_Faraday" )  retVal[i] = 111.;
    else if ( dv[i]->name() == "Leonhard_Euler" )  retVal[i] = 111.;
    else if ( dv[i]->name() == "Turings_Machine_Shop" )  retVal[i] = 111.;
    else if ( dv[i]->name() == "Thomas_Edisons_Repair" )  retVal[i] = 111.;    
    else if ( dv[i]->name() == "NewOpportunity"  )  retVal[i] = 222.;
    else if ( dv[i]->name() == "FirmForecast" )  retVal[i] = 333.;
    else if ( dv[i]->name() == "RiskForecast" )  retVal[i] = 444.;
    else assert( 0 && "illegal ATP type for Gross Revenue" );
  }
  status = 0;
  return &retVal;
}

// difPWatpGetDemandVol global function (registered in the attribute table).
// Return: a LgFrVectorTimeVecFloat object.
void *
difPWatpGetDemandVol(
    const LgFrDataInterfaceImplementation * /* difImp,  not used */,
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrOrderedVectorItemPtr dv = *itemPtrOrdVecPtr; 
  LgFrVectorTimeVecFloat& retVal = 
      *(new LgFrVectorTimeVecFloat( dv.entries() ));    

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  // computers 1, 2, 5, 6 bread and butter machines
  //           3, 4, the older models leaving the market
  //           7, 8, the hot new machines just being rolled out
  for ( int i=0; i<dv.entries(); i++) {
//    assert( setOfDemands().contains( dv[i] ) );
    if ( dv[i]->name() == "Albert_Einstein" ){
      if ((((LgFrDemand*) dv[i])->partPtr()->name()) == "Computer_1" )
        retVal[i] = LgFrTimeVecFloat( nperiods, "200. 150. 100. 75. 50. 50. 0. 0." );
    }  
    else if ( dv[i]->name() == "Robert_Oppenheimer" ){
      if ((((LgFrDemand*) dv[i])->partPtr()->name()) == "Computer_2" )
        retVal[i] = LgFrTimeVecFloat( nperiods, "200. 175. 100. 100. 75. 75. 0. 0." );
    }
    else if ( dv[i]->name() == "Issac_Newton" ){
      if ((((LgFrDemand*) dv[i])->partPtr()->name()) == "Computer_3" )
        retVal[i] = LgFrTimeVecFloat( nperiods, "200. 200. 100. 0. 0. 0. 0. 0." );
    }
    else if ( dv[i]->name() == "Mr_Pythagoras" ){
      if ((((LgFrDemand*) dv[i])->partPtr()->name()) == "Computer_4" )
        retVal[i] = LgFrTimeVecFloat( nperiods, "250. 250. 150. 100. 0. 0. 0. 0." );
    }
    else if ( dv[i]->name() == "Joseph_Orlicky" ){
      if ((((LgFrDemand*) dv[i])->partPtr()->name()) == "Computer_6" )
        retVal[i] = LgFrTimeVecFloat( nperiods, "200. 150. 125. 100. 50. 50. 0. 0." );
    }
    else if ( dv[i]->name() == "Michael_Faraday" ){
      if ((((LgFrDemand*) dv[i])->partPtr()->name()) == "Computer_7" )
        retVal[i] = LgFrTimeVecFloat( nperiods, "0. 0. 50. 150. 200. 150. 50. 50." );
      else if ((((LgFrDemand*) dv[i])->partPtr()->name()) == "System_Board_3" )
        retVal[i] = LgFrTimeVecFloat( nperiods, "0. 0. 10. 30. 40. 50. 50. 50." );
    }
    else if ( dv[i]->name() == "Leonhard_Euler" ){
      if ((((LgFrDemand*) dv[i])->partPtr()->name()) == "Computer_8" )
        retVal[i] = LgFrTimeVecFloat( nperiods, "0. 0. 50. 150. 200. 100. 50. 50." );
    }    
    else if (dv[i]->name() == "Turings_Machine_Shop" ){
      if ((((LgFrDemand*) dv[i])->partPtr()->name()) == "Computer_5" )
        retVal[i] = LgFrTimeVecFloat( nperiods, "200. 150. 125. 100. 100. 50. 0. 0." );
    }
    else if ( dv[i]->name() == "FirmForecast" ){
      if ((((LgFrDemand*) dv[i])->partPtr()->name()) == "Computer_1" )
        retVal[i] = LgFrTimeVecFloat( nperiods, "50. 100. 160. 180. 210. 210. 250. 250." );
      else if (((LgFrDemand*) dv[i])->partPtr()->name() == "Computer_2" )
        retVal[i] = LgFrTimeVecFloat( nperiods, "50. 125. 180. 180. 225. 225. 300. 300."  );
      else if (((LgFrDemand*) dv[i])->partPtr()->name() == "Computer_3" )
        retVal[i] = LgFrTimeVecFloat( nperiods, "0. 0. 50. 50. 0. 0. 0. 0. " );
      else if (((LgFrDemand*) dv[i])->partPtr()->name() == "Computer_4" )
        retVal[i] = LgFrTimeVecFloat( nperiods, "0. 0. 75. 25. 0. 0. 0. 0. " );
      else if (((LgFrDemand*) dv[i])->partPtr()->name() == "Computer_5" )
        retVal[i] = LgFrTimeVecFloat( nperiods, "75. 125. 140. 180. 180. 215. 275. 275. " );
      else if (((LgFrDemand*) dv[i])->partPtr()->name() == "Computer_6" )
        retVal[i] = LgFrTimeVecFloat( nperiods, "50. 50. 160. 200. 275. 275. 300. 300. ");
      else if (((LgFrDemand*) dv[i])->partPtr()->name() == "Computer_7" )
        retVal[i] = LgFrTimeVecFloat( nperiods, "0. 0. 175. 175. 70. 70. 200. 300." );
      else if (((LgFrDemand*) dv[i])->partPtr()->name() == "Computer_8" )
        retVal[i] = LgFrTimeVecFloat( nperiods, "0. 0. 160. 160. 60. 60. 210. 280." );           
    }
    else if ( dv[i]->name() == "RiskForecast" ){
      if (((LgFrDemand*) dv[i])->partPtr()->name() == "Computer_1" )
        retVal[i] = LgFrTimeVecFloat( nperiods, 0.0 );
      else if (((LgFrDemand*) dv[i])->partPtr()->name() == "Computer_2" )
        retVal[i] = LgFrTimeVecFloat( nperiods, 0.0 );
      else if (((LgFrDemand*) dv[i])->partPtr()->name() == "Computer_3" )
        retVal[i] = LgFrTimeVecFloat( nperiods, 0.0 );
    }      
    else if ( dv[i]->name() == "NewOpportunity"  )  
        retVal[i] = LgFrTimeVecFloat( nperiods, 0.0 );
    else assert( 0 && "illegal ATP type for Demand Vol" );
  }      
  return &retVal;
}


// difPWatpGetDemanObj1CumShipReward global function (registered in the attribute table).
// Return: a LgFrVectorTimeVecFloat object.
void *
difPWatpGetDemandObj1CumShipReward(
    const LgFrDataInterfaceImplementation * /* difImp,  not used */,
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrOrderedVectorItemPtr dv = *itemPtrOrdVecPtr; 
  LgFrVectorTimeVecFloat& retVal = 
      *(new LgFrVectorTimeVecFloat( dv.entries() ));    

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  for ( size_t i = 0; i < dv.entries(); i++) {
    if ( dv[i]->name() == "Albert_Einstein" ){
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_1" )
        retVal[i] = LgFrTimeVecFloat( nperiods, 100. );
    }
    else if ( dv[i]->name() == "Robert_Oppenheimer" ){    
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_2" )
        retVal[i] = LgFrTimeVecFloat( nperiods, 125. );
    }
    else if ( dv[i]->name() == "Issac_Newton" ){        
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_3" )
        retVal[i] = LgFrTimeVecFloat( nperiods, 100. );
    }
    else if ( dv[i]->name() == "Mr_Pythagoras" ){     
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_4" )
        retVal[i] = LgFrTimeVecFloat( nperiods, 125. );
    }
    else if ( dv[i]->name() == "Joseph_Orlicky" ){    
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_6" )
        retVal[i] = LgFrTimeVecFloat( nperiods, 90. );
    }
    else if ( dv[i]->name() == "Michael_Faraday" ){
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_7" )
        retVal[i] = LgFrTimeVecFloat( nperiods, 100. );
      else if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "System_Board_3" )
        retVal[i] = LgFrTimeVecFloat( nperiods, 90. ); 
    }
    else if ( dv[i]->name() == "Leonhard_Euler" ){    
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_8" )
        retVal[i] = LgFrTimeVecFloat( nperiods, 110. );
    }
    else if ( dv[i]->name() == "Turings_Machine_Shop" ){
      if ( (((LgFrDemand*)dv[i])->partPtr()->name()) == "Computer_5" )
        retVal[i] = LgFrTimeVecFloat( nperiods, 80. );
    }
    else if ( dv[i]->name() == "FirmForecast" )  retVal[i] = LgFrTimeVecFloat( nperiods, 0. );
    else if ( dv[i]->name() == "RiskForecast" )  retVal[i] = LgFrTimeVecFloat( nperiods, 0. );
    else assert( 0 && "illegal ATP type for Cummulative Shipment Reward" );
  }      
  status = 0;
  return &retVal;
}

// difPWatpGetDemanObj1ShipReward global function (registered in the attribute table).
// Return: a LgFrVectorTimeVecFloat object.
void *
difPWatpGetDemandObj1ShipReward(
    const LgFrDataInterfaceImplementation * /* difImp,  not used */,
    const LgFrOrderedVectorItemPtr* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr
    )
{
  LgFrOrderedVectorItemPtr dv = *itemPtrOrdVecPtr; 
  LgFrVectorTimeVecFloat& retVal = 
      *(new LgFrVectorTimeVecFloat( dv.entries() ));    

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int nperiods = calendar.nPeriods();  // # periods in scenario calendar

  for ( size_t i = 0; i < dv.entries(); i++) {
    retVal[i] =  LgFrTimeVecFloat( nperiods, 1. );
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
difPWatpDefineBom(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrOrderedVectorItemPtr*, // itemPtrOrdVecPtr,
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
   return NULL;
}   


#ifdef NDEBUG
#undef NDEBUG
#endif
// Tests this class.  Only returns if all tests passed
// Tests this class.  Only returns if all tests passed
void
difPWatpContextTest(
   LgFrDataInterface    	& tdif1,	// testScenario1's
   LgFrDataInterface    	& ts1dupDif,	// testScenario1dup's
   const LgFrScenario           * scenPtr)	// testScenario1's
{
  const float fltEps = .0001f;
  LgFrDataInterfaceImplementation* dImp 
      = tdif1.dataInterfaceImplementation();
  LgFrDataInterfaceImplementation* dImptsi 
      = ts1dupDif.dataInterfaceImplementation();

  // Test isA method
  assert (dImp->isA() == __LGFRDATAINTERFACEIMPLEMENTATION);
  assert (dImptsi->isA() 
	  == __LGFRDATAINTERFACEIMPLEMENTATION);
  // Test title method
  assert( dImp->title() == "internal data set pw2" );
  assert( dImptsi->title()=="internal data set pw2" );
  assert( tdif1.title() == "internal data set pw2");
  assert( ts1dupDif.title() == "internal data set pw2");

  // Register all functions
  registerPWatpDifImpFunctions(dImp);


  // Test Get Part global functions
  LgFrDataInterfaceEssentialsFromScenario difEssen(scenPtr);
  LgFrOrderedVectorPart pv;
  RWCString itemSubTypeStr = "material";  
  LgFrOrderedVectorPart* rawPartsPtr
    = (LgFrOrderedVectorPart*) tdif1.partAttribute(
	pv, itemSubTypeStr, "rawParts", &difEssen);
  LgFrOrderedVectorPart raw = *rawPartsPtr;
  assert( raw.entries() == 9 );
  assert( raw[1].name() == "CPU_Type_F" );

  LgFrOrderedVectorPart* productPartsPtr
    = (LgFrOrderedVectorPart*) tdif1.partAttribute(
	pv, itemSubTypeStr, "productParts", &difEssen);
  LgFrOrderedVectorPart product = *productPartsPtr;
  assert( product.entries() == 12 );
  assert( product[11].name() == "Computer_8" );

  LgFrOrderedVectorPart* capacityPartsPtr
    = (LgFrOrderedVectorPart*) tdif1.partAttribute(
	pv, itemSubTypeStr, "capacityParts", &difEssen);
  LgFrOrderedVectorPart capacity = *capacityPartsPtr;
  assert( capacity.entries() == 4 );
  assert( capacity[1].name() == "Board_Test" );


  // Test get raw part attribute global functions
  {
  LgFrVectorFloat* unitCostPtr
    = (LgFrVectorFloat*) tdif1.partAttribute(
	raw, itemSubTypeStr, "rawUnitCost", &difEssen);
  LgFrVectorFloat unitCost = *unitCostPtr;
  assert( unitCost.length() == 9 );

  LgFrVectorInt* constrainedPtr
      = (LgFrVectorInt*) tdif1.partAttribute(
	  raw, itemSubTypeStr, "rawConstrained", &difEssen);
  LgFrVectorInt constrained = *constrainedPtr;
  assert( constrained.length() == 9 );
  assert( constrained[0] );
  for (int i=1; i<7; i++)
     assert( !constrained[i] );
  assert( constrained[8] );

  LgFrVectorTimeVecFloat* obj1ScrapCostPtr
      = (LgFrVectorTimeVecFloat*) tdif1.partAttribute(
	  raw, itemSubTypeStr, "rawObj1ScrapCost", &difEssen);
  LgFrVectorTimeVecFloat obj1ScrapCost = *obj1ScrapCostPtr;
  assert( obj1ScrapCost.length() == 9 );

  LgFrVectorTimeVecFloat* obj1StockCostPtr
      = (LgFrVectorTimeVecFloat*) tdif1.partAttribute(
	  raw, itemSubTypeStr, "rawObj1StockCost", &difEssen);
  LgFrVectorTimeVecFloat obj1StockCost = *obj1StockCostPtr;
  assert( obj1StockCost.length() == 9 );
  
  LgFrVectorTimeVecFloat* supplyVolPtr
      = (LgFrVectorTimeVecFloat*) tdif1.partAttribute(
	  raw, itemSubTypeStr, "rawSupplyVol", &difEssen);
  LgFrVectorTimeVecFloat supplyVol = *supplyVolPtr;
  assert( supplyVol.length() == 9 );

  delete unitCostPtr;     // 12:20 pm, 9/25/95 
  delete constrainedPtr;  // 12:20 pm, 9/25/95
  delete obj1ScrapCostPtr;     // 12:20 pm, 9/25/95
  delete obj1StockCostPtr;     // 12:20 pm, 9/25/95
  delete supplyVolPtr;     // 12:20 pm, 9/25/95
}

  // Test get product part attribute global functions
  {
  LgFrVectorTimeVecFloat* cycleTimePtr
      = (LgFrVectorTimeVecFloat*) tdif1.partAttribute(
	  product, itemSubTypeStr, "productCycleTime", &difEssen);
  LgFrVectorTimeVecFloat cycleTime = *cycleTimePtr;
  assert( cycleTime.length() == 12 );
//  for (int i=0; i<cycleTime.length(); i++)
//  assert( fabs(cycleTime[i][0] - 1.0) < fltEps );
 
  LgFrVectorFloat* unitCostPtr
      = (LgFrVectorFloat*) tdif1.partAttribute(
	  product, itemSubTypeStr, "productUnitCost", &difEssen);
  LgFrVectorFloat unitCost = *unitCostPtr;
  assert( unitCost.length() == 12 );
//  for (i=0; i<cycleTime.length(); i++)
//  assert( fabs(cycleTime[i][0] - 1.0) < fltEps );
  
  LgFrVectorInt* constrainedPtr
      = (LgFrVectorInt*) tdif1.partAttribute(
	  product, itemSubTypeStr, "productConstrained", &difEssen);
  LgFrVectorInt constrained = *constrainedPtr;
  assert( constrained.length() == 12 );
  for (int i=0; i<constrained.length(); i++)
  assert( constrained[i] );

  LgFrVectorInt* yieldPtr
      = (LgFrVectorInt*) tdif1.partAttribute(
	  product, itemSubTypeStr, "productYield", &difEssen);
  LgFrVectorInt yield = *yieldPtr;
  assert( yield.length() == 12 );
//  for (i=0; i<yield.length(); i++)
//  assert( yield[i]==100 );
  
  LgFrVectorTimeVecFloat* obj1ProdCostPtr
      = (LgFrVectorTimeVecFloat*) tdif1.partAttribute(
	  product, itemSubTypeStr, "obj1ProdCost", &difEssen);
  LgFrVectorTimeVecFloat obj1ProdCost = *obj1ProdCostPtr;
  assert( obj1ProdCost.length() == 12 );

  LgFrVectorTimeVecFloat* obj1ScrapCostPtr
      = (LgFrVectorTimeVecFloat*) tdif1.partAttribute(
	  product, itemSubTypeStr, "productObj1ScrapCost", &difEssen);
  LgFrVectorTimeVecFloat obj1ScrapCost = *obj1ScrapCostPtr;
  assert( obj1ScrapCost.length() == 12 );
  
  LgFrVectorTimeVecFloat* obj1StockCostPtr
      = (LgFrVectorTimeVecFloat*) tdif1.partAttribute(
	  product, itemSubTypeStr, "productObj1StockCost", &difEssen);
  LgFrVectorTimeVecFloat obj1StockCost = *obj1StockCostPtr;
  assert( obj1StockCost.length() == 12 );
  
  LgFrVectorTimeVecFloat* supplyVolPtr
      = (LgFrVectorTimeVecFloat*) tdif1.partAttribute(
	  product, itemSubTypeStr, "productSupplyVol", &difEssen);
  LgFrVectorTimeVecFloat supplyVol = *supplyVolPtr;
  assert( supplyVol.length() == 12 );

  delete unitCostPtr;     // 12:20 pm, 9/25/95 
  delete cycleTimePtr;  // 12:20 pm, 9/25/95
  delete constrainedPtr;     // 12:20 pm, 9/25/95
  delete yieldPtr;     // 12:20 pm, 9/25/95
  delete obj1ProdCostPtr;     // 12:20 pm, 9/25/95
  delete obj1ScrapCostPtr;     // 12:20 pm, 9/25/95 
  delete obj1StockCostPtr;  // 12:20 pm, 9/25/95
  delete supplyVolPtr;     // 12:20 pm, 9/25/95

  }

  // Test get capacity attribute global functions
  {
 LgFrVectorFloat* unitCostPtr
      = (LgFrVectorFloat*) tdif1.partAttribute(
	  capacity, itemSubTypeStr, "capacityUnitCost", &difEssen);
  LgFrVectorFloat unitCost = *unitCostPtr;
  assert( unitCost.length() == 4 );
//  assert( fabs(unitCost[0] - 1.46) < fltEps );

  LgFrVectorInt* constrainedPtr
      = (LgFrVectorInt*) tdif1.partAttribute(
	  capacity, itemSubTypeStr, "capacityConstrained", &difEssen);
  LgFrVectorInt constrained = *constrainedPtr;
  assert( constrained.length() == 4 );
  assert( constrained[0] );


  LgFrVectorTimeVecFloat* obj1ScrapCostPtr
      = (LgFrVectorTimeVecFloat*) tdif1.partAttribute(
	  capacity, itemSubTypeStr, "capacityObj1ScrapCost", &difEssen);
  LgFrVectorTimeVecFloat obj1ScrapCost = *obj1ScrapCostPtr;
  assert( obj1ScrapCost.length() == 4 );

  LgFrVectorTimeVecFloat* supplyVolPtr
      = (LgFrVectorTimeVecFloat*) tdif1.partAttribute(
	  capacity, itemSubTypeStr, "capacitySupplyVol", &difEssen);
  LgFrVectorTimeVecFloat supplyVol = *supplyVolPtr;
  assert( supplyVol.length() == 4 );

  delete unitCostPtr;     // 12:20 pm, 9/25/95 
  delete constrainedPtr;  // 12:20 pm, 9/25/95
  delete obj1ScrapCostPtr;     // 12:20 pm, 9/25/95
  delete supplyVolPtr;     // 12:20 pm, 9/25/95

  }

// Test Get Demands global functions
  LgFrOrderedVectorDemand dv;

  LgFrOrderedVectorDemand* committedDemandPtr
    = (LgFrOrderedVectorDemand*) tdif1.demandAttribute(
	dv, "committedDemands", &difEssen );
  LgFrOrderedVectorDemand committed = * committedDemandPtr;
  assert( committed.entries() == 9 ); 
//  assert( committed[0].name() == "Committed" );
//  assert( committed[1].name() == "Committed" );
//  assert( committed[2].name() == "Committed" );
  
  LgFrOrderedVectorDemand* newOpportunityDemandPtr
    = (LgFrOrderedVectorDemand*) tdif1.demandAttribute(
	dv, "newOpportunityDemands", &difEssen );
  LgFrOrderedVectorDemand newOpportunity = * newOpportunityDemandPtr; 
  assert( newOpportunity.entries() == 3 );
  assert( newOpportunity[0].name() == "NewOpportunity" );

  LgFrOrderedVectorDemand* firmForecastDemandPtr
    = (LgFrOrderedVectorDemand*) tdif1.demandAttribute(
	dv, "firmForecastDemands", &difEssen );
  LgFrOrderedVectorDemand firmForecast = * firmForecastDemandPtr;
  assert( firmForecast.entries() == 8 );
  assert( firmForecast[1].name() == "FirmForecast" );

  LgFrOrderedVectorDemand* riskForecastDemandPtr
    = (LgFrOrderedVectorDemand*) tdif1.demandAttribute(
	dv, "riskForecastDemands", &difEssen );
  LgFrOrderedVectorDemand riskForecast = * riskForecastDemandPtr;
  assert( riskForecast.entries() == 3 );
  assert( riskForecast[0].name() == "RiskForecast" );
  assert( riskForecast[1].name() == "RiskForecast" );
  assert( riskForecast[2].name() == "RiskForecast" );

  // Test get demand Attribute methods
  LgFrVectorFloat* committedGrossRevPtr
   = (LgFrVectorFloat*) tdif1.demandAttribute(
       committed,"demandGrossRev", &difEssen);
  LgFrVectorFloat committedGrossRev = *committedGrossRevPtr;
  assert( committedGrossRev.length() == 9 );
//  assert( fabs(grossRev[0] - 111. ) < fltEps );
//  assert( fabs(grossRev[1] - 111. ) < fltEps );
//  assert( fabs(grossRev[2] - 111. ) < fltEps );

  LgFrVectorFloat* newOpportunityGrossRevPtr
      = (LgFrVectorFloat*) tdif1.demandAttribute(
	  newOpportunity,"demandGrossRev", &difEssen);
  LgFrVectorFloat newOpportunityGrossRev = *newOpportunityGrossRevPtr;
  assert( newOpportunityGrossRev.length() == 3 );
//  assert( fabs(grossRev[0] - 222. ) < fltEps );
//  assert( fabs(grossRev[1] - 222. ) < fltEps );
//  assert( fabs(grossRev[2] - 222. ) < fltEps );

  LgFrVectorFloat* firmForecastGrossRevPtr
   = (LgFrVectorFloat*) tdif1.demandAttribute(
       firmForecast,"demandGrossRev", &difEssen);
  LgFrVectorFloat firmForecastGrossRev = *firmForecastGrossRevPtr;
  assert( firmForecastGrossRev.length() == 8 );
//  assert( fabs(grossRev[0] - 333. ) < fltEps );

  LgFrVectorFloat* riskForecastGrossRevPtr
   = (LgFrVectorFloat*) tdif1.demandAttribute(
       riskForecast,"demandGrossRev", &difEssen);
  LgFrVectorFloat riskForecastGrossRev = *riskForecastGrossRevPtr;
  assert( riskForecastGrossRev.length() == 3 );
//  assert( fabs(grossRev[0] - 444. ) < fltEps );

  // Test define BOM method
//  witAddPart( lps.witRun(), "Raw1",     WitRAW );
//  witAddPart( lps.witRun(), "Raw2",     WitRAW );
//  witAddPart( lps.witRun(), "Raw3",     WitRAW );
//  witAddPart( lps.witRun(), "Raw4",     WitRAW );
//  witAddPart( lps.witRun(), "Raw5",     WitRAW );
//  witAddPart( lps.witRun(), "SubAss1", WitPRODUCT );
//  witAddPart( lps.witRun(), "SubAss2", WitPRODUCT );
//  witAddPart( lps.witRun(), "SubAss3", WitPRODUCT );
//  witAddPart( lps.witRun(), "FinalProduct1", WitPRODUCT );
//  witAddPart( lps.witRun(), "FinalProduct2", WitPRODUCT );
//  witAddPart( lps.witRun(), "FinalProduct3", WitPRODUCT );
//  witAddPart( lps.witRun(), "PlantCapacity",WitCAPACITY );

//  tdif1.defineBom( lps.witRun() );
//  int nBomEntries;
//  witGetPartNBomEntries( lps.witRun(), "SubAss1", &nBomEntries );
//  assert ( nBomEntries == 2 );
//  witGetPartNBomEntries( lps.witRun(), "SubAss2", &nBomEntries );
//  assert ( nBomEntries == 3 );
//  witGetPartNBomEntries( lps.witRun(), "SubAss3", &nBomEntries );
//  assert ( nBomEntries == 3 );
//  witGetPartNBomEntries( lps.witRun(), "FinalProduct1", &nBomEntries );
//  assert ( nBomEntries == 3 );
//  witGetPartNBomEntries( lps.witRun(), "FinalProduct2", &nBomEntries );
//  assert ( nBomEntries == 4 );
//  witGetPartNBomEntries( lps.witRun(), "FinalProduct3", &nBomEntries );
//  assert ( nBomEntries == 3 );
  delete committedGrossRevPtr;      // 12:20 pm, 9/25/95
  delete newOpportunityGrossRevPtr; // 12:20 pm, 9/25/95
  delete firmForecastGrossRevPtr;  // 12:20 pm, 9/25/95
  delete riskForecastGrossRevPtr;  // 12:20 pm, 9/25/95
  delete productPartsPtr;  // 12:20 pm, 9/25/95
  delete rawPartsPtr;      // 12:20 pm, 9/25/95
  delete capacityPartsPtr; // 12:20 pm, 9/25/95
  delete committedDemandPtr;  // 12:20 pm, 9/25/95
  delete newOpportunityDemandPtr;  // 12:20 pm, 9/25/95
  delete firmForecastDemandPtr; // 12:20 pm, 9/25/95
  delete riskForecastDemandPtr;  // 12:20 pm, 9/25/95

}
