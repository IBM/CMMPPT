
#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

// RW2STL -- inserted:
#include <fstream>
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.


#include <math.h>
//#include <iostream.h>
//#include <fstream.h>


#include <scenario/src/difCmrpF.h>
#include <scenario/src/scenario.h>
#include <scenario/src/idnumber.h>
#include <scenario/src/calendar.h>
#include <scenario/src/collflt.h>
#include <scenario/src/difEssSc.h>


// -----------------------------------------------------------------------
//  Create a LgFrDataInterfaceImplementationFromCmrpFiles
// -----------------------------------------------------------------------

LgFrDataInterfaceImplementation * 
createCmrpFilesDifImp(const std::string& filename)
{
   return new LgFrDataInterfaceImplementationFromCmrpFiles(filename);
} 

//----------------------------------------------------------------------------
// To register all global functions for this class
// The following functions are registered by using difWitR globle function: 
// unitCost, procCost, obj1ScrapCost,obj1StockCost, supplyVol, yield, 
// cycleTime, obj1ProdCost, obj1ProdCos, grossRev, demandVol, 
// obj1CumShipReward, obj1ShipReward   
// Register difCmrpF's own global functions for raw parts: constrained and procLeadTime, 
//----------------------------------------------------------------------------
void
registerCmrpFilesDifImpFunctions(LgFrDataInterfaceImplementation *dImpCMRP)
{
// Globle function for raw parts
// Register difWitRun global functions for
//   unitcost, procCost, obj1ScrapCost, obj1StockCost, and supplyVol.
// Register difCmrpF's own global functions for constrained and procLeadTime, 

dImpCMRP->registerFunction("rawUnitCost", material, part,
                            vectorFloat, difWitRunGetPartUnitCost);
dImpCMRP->registerFunction("rawProcCost", material, part,
                            vectorFloat, difWitRunGetPartProcCost);
dImpCMRP->registerFunction("rawConstrained", material, part,
                            vectorInt, difCmrpFGetRawConstrained);
dImpCMRP->registerFunction("rawProcLeadTime", material, part,
                            timeVecFloat, difCmrpFGetRawProcLeadTime);
dImpCMRP->registerFunction("rawObj1ScrapCost", material, part,
                            timeVecFloat, difWitRunGetPartObj1ScrapCost);
dImpCMRP->registerFunction("rawObj1StockCost", material, part,
                            timeVecFloat, difWitRunGetPartObj1StockCost);
dImpCMRP->registerFunction("rawSupplyVol", material, part,
                            timeVecFloat, difWitRunGetPartSupplyVol);


// Global functions for product parts.
// Register difWitRun global functions for
//   unitcost, procCost, constrained, yiels, procLeadTime, cycleTime, 
//   obj1ProdCost, obj1ScrapCost, obj1StockCost, supplyVol, minLotSize,
//   incLotSize. 

dImpCMRP->registerFunction("productUnitCost", material, part,
                            vectorFloat, difWitRunGetPartUnitCost);
dImpCMRP->registerFunction("productProcCost", material, part,
                            vectorFloat, difWitRunGetPartProcCost);
dImpCMRP->registerFunction("productConstrained", material, part,
                            vectorInt, difWitRunGetPartConstrained);
dImpCMRP->registerFunction("productYield", material, part,
                            vectorInt, difWitRunGetProductYield);
dImpCMRP->registerFunction("productYieldFloat", material, part,
                            vectorInt, difWitRunGetProductYieldFloat);
dImpCMRP->registerFunction("productProcLeadTime", material, part,
                            timeVecFloat, difWitRunGetPartProcLeadTime);
dImpCMRP->registerFunction("productCycleTime", material, part,
                           timeVecFloat , difWitRunGetProductCycleTime);
dImpCMRP->registerFunction("productObj1ProdCost", material, part,
                            timeVecFloat, difWitRunGetProductObj1ProdCost);
dImpCMRP->registerFunction("productObj1ScrapCost", material, part,
                            timeVecFloat, difWitRunGetPartObj1ScrapCost);
dImpCMRP->registerFunction("productObj1StockCost", material, part,
                            timeVecFloat, difWitRunGetPartObj1StockCost);
dImpCMRP->registerFunction("productSupplyVol", material, part,
                            timeVecFloat, difWitRunGetPartSupplyVol);
dImpCMRP->registerFunction("productMinLotSize", material, part,
                            timeVecFloat, difWitRunGetProductMinLotSize);
dImpCMRP->registerFunction("productIncLotSize", material, part,
                            timeVecFloat, difWitRunGetProductIncLotSize);


// Global functions for capacity parts.
// Register difWitRun global functions for
//   unitcost, constrained, obj1ScrapCost, and supplyVol 

dImpCMRP->registerFunction("capacityUnitCost", material, part,
                            vectorFloat, difWitRunGetPartUnitCost);
dImpCMRP->registerFunction("capacityConstrained", material, part,
                            vectorInt, difWitRunGetPartConstrained);
dImpCMRP->registerFunction("capacityObj1ScrapCost", material, part,
                            timeVecFloat, difWitRunGetPartObj1ScrapCost);
dImpCMRP->registerFunction("capacitySupplyVol", material, part,
                            timeVecFloat, difWitRunGetPartSupplyVol);


// Global functions for demands
// Register difWitRun global functions for
//  grossRev, demandVol, Obj1CumShipReward, priority, and obj1ShipReward.

dImpCMRP->registerFunction("demandGrossRev", demandType, demand,
                            vectorFloat, difWitRunGetDemandGrossRev);
dImpCMRP->registerFunction("demandVol", demandType, demand,
                            timeVecFloat, difWitRunGetDemandVol);
dImpCMRP->registerFunction("demandObj1CumShipReward", demandType, demand,
                            timeVecFloat, difWitRunGetDemandObj1CumShipReward);
dImpCMRP->registerFunction("demandObj1ShipReward", demandType, demand,
                            timeVecFloat, difWitRunGetDemandObj1ShipReward);
dImpCMRP->registerFunction("demandPriority", demandType, demand,
                            timeVecFloat,difWitRunGetDemandPriority );


// Parts table, register globle functions from difWitR, 7/19/95 DW 
dImpCMRP->registerFunction("rawParts", material, part,
			   orderedVecPart, difWitRunGetRawParts);
dImpCMRP->registerFunction("productParts", material, part,
			   orderedVecPart, difWitRunGetProductParts);
dImpCMRP->registerFunction("capacityParts", material, part,
			   orderedVecPart, difWitRunGetCapacityParts);

// using globle function from difWitR, 7/19/95 DW
dImpCMRP->registerFunction("committedDemands",demandType , demand,
			   orderedVecDemand, difWitRunGetCommittedDemands);
dImpCMRP->registerFunction("newOpportunityDemands", demandType, demand,
			   orderedVecDemand, difWitRunGetNewOpportunityDemands);
dImpCMRP->registerFunction("firmForecastDemands", demandType, demand,
			   orderedVecDemand, difWitRunGetFirmForecastDemands);
dImpCMRP->registerFunction("riskForecastDemands", demandType, demand,
			   orderedVecDemand, difWitRunGetRiskForecastDemands);
// Register define bom globle function
dImpCMRP->registerFunction("defineBom", material, part,
                            timeVecFloat, difWitRunDefineBom);


}


//--------------------------------------------------------------------
//
//   Global functions to obtain raw part attributes
//
//-------------------------------------------------------------------

// difWitRunGetRawConstrained global function (registered in the attribute table).
// It returns a LgFrIntVector object
void *
difCmrpFGetRawConstrained(
    const LgFrDataInterfaceImplementation * difImp,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
  assert( difImp->GetDIIType() == __LGFRDATAINTERFACEIMPLEMENTATIONFROMCMRPFILES);
  witAttr partCategory;
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr;
  LgFrIntVector& retVal = *(new LgFrIntVector);
  retVal.reserve( pv.size() );
  std::string  partName;

  // make a local version of this that's
  // not a pointer-to-const, Scott Meyers item 21 p 77.
  LgFrDataInterfaceImplementationFromCmrpFiles * const dImpCMRP =
     ( LgFrDataInterfaceImplementationFromCmrpFiles * const ) difImp;

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) 
  {
    witGetPartCategory(dImpCMRP->constWitRun(difEssenPtr), pv[i]->name().c_str(), &partCategory);
    if ( partCategory == WitRAW ) 
	{
		partName = pv[i]->name();
		retVal.push_back( (*dImpCMRP->constrainedDict_.find( partName )).second );
    }
    else if ((partCategory == WitPRODUCT) || (partCategory == WitCAPACITY))
	{
		retVal.push_back( true );
    }
  }
  return &retVal;
}

// difWitRunGetRawProcLeadTime global function (registered in the attribute table).
// It returns a LgFrTimeVecFloatVector object.
void *
difCmrpFGetRawProcLeadTime(
    const LgFrDataInterfaceImplementation * difImp,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr)
{
  assert( difImp->GetDIIType() == __LGFRDATAINTERFACEIMPLEMENTATIONFROMCMRPFILES);
  float * cycleTime;
  witAttr partCategory;
  LgFrItemPtrVector pv = *itemPtrOrdVecPtr;
  LgFrTimeVecFloatVector& retVal = *(new LgFrTimeVecFloatVector);
  retVal.reserve( pv.size() );
  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int calNP(calendar.nPeriods());  // # periods in scenario calendar
  int witNP;                       // # periods in wit data file

  std::string  partName;
  float procLT;


  LgFrDataInterfaceImplementationFromCmrpFiles * dImpCMRP
      = (LgFrDataInterfaceImplementationFromCmrpFiles * ) difImp;

  witGetNPeriods( dImpCMRP->constWitRun(difEssenPtr), &witNP);
  LgFrTimeVecFloat tv(calNP);

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++ ) {
    witGetPartCategory(dImpCMRP->constWitRun(difEssenPtr), pv[i]->name().c_str(), &partCategory);
    if ( partCategory == WitRAW ) {
      partName = pv[i]->name();
      procLT = (*dImpCMRP->procLeadTimeDict_.find( partName ) ).second;
    retVal.push_back( LgFrTimeVecFloat(calNP, procLT ) );
    }
    else if (partCategory == WitPRODUCT) {
      witGetPartCycleTime(dImpCMRP->constWitRun(difEssenPtr), pv[i]->name().c_str(),
                              &cycleTime);
      size_t j = 0; // Pulled out of the for below by RW2STL
      for(j = 0; j < size_tMin(calNP, witNP); j++ ){
        tv[j] = cycleTime[j];
          }
      retVal.push_back( tv );
      portableFree(cycleTime);
      }
  }
  return &retVal;
}


//--------------------------------------------------------------------
//  
//   Define the Wit Run
//
//-------------------------------------------------------------------


void
  LgFrDataInterfaceImplementationFromCmrpFiles::defineWitRun(
      const LgFrDataInterfaceEssentials * difEssenPtr)
const
{
  assert( !validWitRun() );
  // make a local version of this that's
  // not a pointer-to-const, Scott Meyers item 21 p 77.
  LgFrDataInterfaceImplementationFromCmrpFiles * const localThis =
                  ( LgFrDataInterfaceImplementationFromCmrpFiles * const ) this;
  WitRun * wr = localThis->LgFrDataInterfaceImplementationFromWitRun::mutableWitRun();
  witSetMesgTimesPrint(wr, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
  witInitialize( wr );

  witSetMesgTimesPrint( wr, WitTRUE, 538, 0 );
  witSetWit34Compatible( wr, WitTRUE );

  const LgFrCalendar& calendar = difEssenPtr->calendar();
  int calNp(calendar.nPeriods());  // # periods in scenario calendar
  witSetNPeriods( wr, calNp );
  witSetTitle( wr, baseFileName_.c_str() );
  witSetObjChoice( wr, 1 );


      // Now read CMRP data files and populate the wit run
      std::string line;
      bool result;
      
      // Read Raw Parts File
      {
        // Open file
		std::string fname( baseFileName_ + ".raw" );
		std::ifstream rawF( fname.c_str() );
        if ( !rawF ) 
        {
          std::cerr <<"LgFrERROR: File open failed: " << fname << std::endl;
          exit( EXIT_FAILURE );
        }
        readFirstRecord( rawF, ".raw" );

        // Read remaining records until eof encountered
        long lineCount = 1; 
        for (;;)
        {
          lineCount++;
          std::getline( rawF, line );
          if (rawF.eof() ) break;
          if ( !rawF )
          {
            std::cerr <<"LgFrERROR: Read failed: " << baseFileName_ << ".raw. Line " +
                sce::stringFrom( lineCount ) << std::endl;
            exit( EXIT_FAILURE );
          }

          // Set Part Name
          SCETokenizer next(line);
          std::string partName = next();
          witAddPart( wr, partName.c_str(), WitRAW );

          // Unit Cost
          double unitCostDbl;
          result = sce::fromString( next(), unitCostDbl ); // RWLocale::global().stringToNum( next().c_str(), &unitCostDbl);
          if ( !result ) 
          {
            std::cerr <<"LgFrERROR: Read failed: " << baseFileName_ << ".raw. Line " +
                    sce::stringFrom( lineCount ) +
                   " Token 2 (unitCost)" << std::endl;
            exit( EXIT_FAILURE );
          }
          float unitCost = (float)unitCostDbl;
          witSetPartUnitCost( wr, partName.c_str(), unitCost );

          // Constrainted flag
          std::string constrainedString = next();
		  sce::toLower( constrainedString );
          if ( constrainedString != "constrained" &&
               constrainedString != "unconstrained" ) 
          {
            std::cerr <<"LgFrERROR: Read failed: " << baseFileName_ << ".raw. Line " +
                    sce::stringFrom( lineCount ) +
                   " Token 3 (constrained)" << std::endl;
            exit( EXIT_FAILURE );
          }
          bool constrained = (constrainedString == "constrained");
          localThis->constrainedDict_.insert( ConstrainedDictPair( partName, constrained ) );

          // Procurement Lead time
          double procLTDbl;
          result = sce::fromString( next(), procLTDbl ); // RWLocale::global().stringToNum( next().c_str(), &procLTDbl);
          if ( !result ) 
          {
            std::cerr <<"LgFrERROR: Read failed: " << baseFileName_ << ".raw. Line " +
                    sce::stringFrom( lineCount ) +
                   " Token 4 (procurement lead time)" << std::endl;
            exit( EXIT_FAILURE );
          }
          float procLT = (float)procLTDbl;
          localThis->procLeadTimeDict_.insert( ProcLeadTimePair( partName, procLT) );

          // Supply Volume
          std::string supplyVol = next();
          //std::string supplyVol;
          //RWCSubString token = next();
          //supplyVol = line(token.start(), line.length() - token.start() );
          witSetPartSupplyVol( wr, partName.c_str(), 
               LgFrTimeVecFloat( calNp, supplyVol ).data() );

        } // end of for loop reading records
      }  // Finished reading .raw file


      // Read Product Parts File
      {
        // Open file
		  std::ifstream prdF( std::string( baseFileName_ + ".prd" ).c_str() );
        if ( !prdF ) 
        {
          std::cerr <<"LgFrERROR: File open failed: " + baseFileName_ + ".prd" << std::endl;
          exit( EXIT_FAILURE );
        }
        readFirstRecord( prdF, ".prd" );

        // Read remaining records until eof encountered
        long lineCount = 1;
        for (;;)
        {
          lineCount++;
          std::getline( prdF, line );
          if (prdF.eof() ) break;
          if ( !prdF )
          {
            std::cerr <<"LgFrERROR: Read failed: " << baseFileName_ << ".prd. Line " +
                    sce::stringFrom( lineCount ) << std::endl;
            exit( EXIT_FAILURE );
          }

          // Set Part Name
          SCETokenizer next(line);
          std::string partName = next();
          witAddPart( wr, partName.c_str(), WitPRODUCT );

          // Unit Cost
          double unitCostDbl;
          result = sce::fromString( next(), unitCostDbl ); // RWLocale::global().stringToNum( next().c_str(), &unitCostDbl );
          if ( !result )
          {
            std::cerr <<"LgFrERROR: Read failed: " << baseFileName_ << ".prd. Line " +
                    sce::stringFrom( lineCount ) +
                   " Token 2 (unitCost)" << std::endl;
            exit( EXIT_FAILURE );
          }
          float unitCost = (float)unitCostDbl;
          witSetPartUnitCost( wr, partName.c_str(), unitCost );

          // Cycle Time
          double cycleTimeDbl;
          result = sce::fromString( next(), cycleTimeDbl ); // RWLocale::global().stringToNum( next().c_str(), &cycleTimeDbl );
          if ( !result )
          {
            std::cerr <<"LgFrERROR: Read failed: " << baseFileName_ << ".prd. Line " +
                    sce::stringFrom( lineCount ) +
                   " Token 3 (cycleTime)" << std::endl;
            exit( EXIT_FAILURE );
          }
          float cycleTime = (float)cycleTimeDbl;
          witSetPartCycleTime( wr, partName.c_str(), 
                   LgFrTimeVecFloat( calNp, cycleTime ).data() );

          // Supply Volume
          std::string supplyVol = next();
          //std::string supplyVol;
          //RWCSubString token = next();
          //supplyVol = line(token.start(), line.length() - token.start() );
          witSetPartSupplyVol( wr, partName.c_str(),
               LgFrTimeVecFloat( calNp, supplyVol ).data() );
	  int supPerNo = 0; // Pulled out of the for below by RW2STL
	  for (supPerNo = 1; // token is already at first supply volume
	       supPerNo < calNp; ++supPerNo)
	      next();

        } // end of for loop reading records

      }  // Finished reading .prd file



      // Read Capacity File
      {
        // Open file
		  std::ifstream capF( std::string( baseFileName_ + ".cap" ).c_str() );
        if ( !capF )
        {
          std::cerr <<"LgFrERROR: File open failed: " + baseFileName_ + ".cap" << std::endl;
          exit( EXIT_FAILURE );
        }
        readFirstRecord( capF, ".cap" );

        // Read remaining records until eof encountered
        long lineCount = 1;
        int dupCapPart = 0;
        for (;;)
        {
          lineCount++;
          std::getline( capF, line );
          if (capF.eof() ) break;
          if ( !capF )
          {
            std::cerr <<"LgFrERROR: Read failed: " << baseFileName_ << ".cap. Line " +
                    sce::stringFrom( lineCount ) << std::endl;
            exit( EXIT_FAILURE );
          }

          // Set Capacity Name
          SCETokenizer next(line);
          std::string partName = next();

          witBoolean exists;
          witGetPartExists( wr, partName.c_str(), &exists );
          if ( !exists )
             witAddPart( wr, partName.c_str(), WitCAPACITY );
          else {
             dupCapPart++;
             std::cout <<"LgFrWARNING: duplicate capacity part: " <<partName << std::endl;
             continue;
          }

          // Supply Volume
          std::string supplyVol = next( "\n" );
          witSetPartSupplyVol( wr, partName.c_str(),
               LgFrTimeVecFloat( calNp, supplyVol ).data() );

        } // end of for loop reading records


        std::cout <<"LgFrINFO: duplicate capacity part count " <<dupCapPart << std::endl;

      }  // Finished reading .cap file




      // Read Demand File
      {
        // Open file
		  std::ifstream demF( std::string( baseFileName_ + ".dem" ).c_str() );
        if ( !demF )
        {
          std::cerr <<"LgFrERROR: File open failed: " + baseFileName_ + ".dem" << std::endl;
          exit( EXIT_FAILURE );
        }
        readFirstRecord( demF, ".dem" );

        // Read remaining records until eof encountered
        long lineCount = 1;
        int undefDemandedPart = 0;
        int demandOnCap = 0;
        for (;;)
        {
          lineCount++;
          std::getline( demF, line );
          if (demF.eof() ) break;
          if ( !demF )
          {
            std::cerr <<"LgFrERROR: Read failed: " << baseFileName_ << ".dem. Line " +
                    sce::stringFrom( lineCount ) << std::endl;
            exit( EXIT_FAILURE );
          }

          // Set Demand Name
          SCETokenizer next(line);
          std::string demandName = next();
          std::string partName = next();

          witBoolean exists;
          witGetPartExists( wr, partName.c_str(), &exists );
          if ( exists ) {
             witAttr cat;
             witGetPartCategory( wr, partName.c_str(), &cat );
             if ( cat != WitCAPACITY )
                witAddDemand( wr, partName.c_str(), demandName.c_str() );
             else {
                demandOnCap++;
                std::cout <<"LgFrWARNING: demand on capacity part: " <<partName << std::endl;
                continue;
             }
          }
          else {
             undefDemandedPart++;
                std::cout <<"LgFrWARNING: undefined demanded part " <<partName << std::endl;
             continue;
          }

          // Priority flag
          //RWCSubString priorityString = next();
          long priorityLong;
          bool result = sce::fromString( next(), priorityLong );
//          bool result = RWLocale::global().stringToNum(
//                  priorityString, &priorityLong);
          if ( !result )
          {
            std::cerr <<"LgFrERROR: Read failed: " << baseFileName_ << ".bom. Line " +
                    sce::stringFrom( lineCount ) +
                   " Token 3 (priority)" << std::endl;
            exit( EXIT_FAILURE );
          }
          int * priority = new int[ calNp ];
          int p = 0; // Pulled out of the for below by RW2STL
          for ( p=0; p<calNp; p++ ) 
             priority[p]=priorityLong;
          witSetDemandPriority( wr, partName.c_str(), demandName.c_str(), priority );
          delete [] priority; 

          // Demand Volume
          std::string demandVol = next();
          //RWCSubString token = next();
          //demandVol = line(token.start(), line.length() - token.start() );
          witSetDemandDemandVol( wr, partName.c_str(), demandName.c_str(),
               LgFrTimeVecFloat( calNp, demandVol ).data() );

        } // end of for loop reading records

        std::cout <<"LgFrINFO: undefined demanded part count "
             <<undefDemandedPart << std::endl;
        std::cout <<"LgFrINFO: demand on capacity part count "
             <<demandOnCap << std::endl;

      }  // Finished reading .dem file




      // Read BOM File
      {
        // Open file
		  std::ifstream bomF( std::string( baseFileName_ + ".bom" ).c_str() );
        if ( !bomF )
        {
          std::cerr <<"LgFrERROR: File open failed: " + baseFileName_ + ".bom" << std::endl;
          exit( EXIT_FAILURE );
        }
        readFirstRecord( bomF, ".bom" );

        // Read remaining records until eof encountered
        long lineCount = 1;
        int undefProdPart = 0;
        int undefConsPart = 0;
        int prodCapPart = 0;
        int prodRawPart = 0;
        for (;;)
        {
          lineCount++;
          std::getline( bomF, line );
          if (bomF.eof() ) break;
          if ( !bomF )
          {
            std::cerr <<"LgFrERROR: Read failed: " << baseFileName_ << ".bom. Line " +
                    sce::stringFrom( lineCount ) << std::endl;
            exit( EXIT_FAILURE );
          }

          // Get produced and consumed parts
          SCETokenizer next(line);
          std::string producedPartName = next();
          std::string consumedPartName = next();

          // Get usage rate
          double usageRateDbl;
          bool result = sce::fromString( next(), usageRateDbl );
//          result = RWLocale::global().stringToNum(
//                   std::string(next()), &usageRateDbl);
          if ( !result )
          {
            std::cerr <<"LgFrERROR: Read failed: " << baseFileName_ << ".bom. Line " +
                    sce::stringFrom( lineCount ) +
                   " Token 3 (usageRate)" << std::endl;
            exit( EXIT_FAILURE );
          }
          float usageRate = (float)usageRateDbl;

          witBoolean prodPartExists, consPartExists;
          witGetPartExists( wr, producedPartName.c_str(), &prodPartExists );
          witGetPartExists( wr, consumedPartName.c_str(), &consPartExists );
          if ( !prodPartExists ) {
             undefProdPart++;
             std::cout <<"LgFrWARNING: undefined produced part: "
                  <<producedPartName << std::endl;
          }
          if ( !consPartExists ) {
             undefConsPart++;
             std::cout <<"LgFrWARNING undefined consumed part: "
                  <<consumedPartName << std::endl;
          }

          if ( prodPartExists && consPartExists ) {
             witAttr cat;
             witGetPartCategory( wr, producedPartName.c_str(), &cat );
             if ( cat == WitPRODUCT )
                {
                int bomEntIdx;

                witGetOperationNBomEntries (
                     wr,
                     producedPartName.c_str (),
                   & bomEntIdx);

                witAddBomEntry (
                   wr, 
                   producedPartName.c_str (),
                   consumedPartName.c_str ());

                witSetBomEntryUsageRate (
                   wr,
                   producedPartName.c_str (),
                   bomEntIdx,
                   usageRate);
                }
             else if ( cat == WitCAPACITY ) {
                prodCapPart++;
                std::cout <<"LgFrWARNING: produced capacity: "
                     <<producedPartName << std::endl;
             }
             else {
                assert( cat == WitRAW );
                prodRawPart++;
                std::cout <<"LgFrWARNING: produced raw part: "
                     <<producedPartName << std::endl;
             }
          }

        } // end of for loop reading records
        std::cout <<"LgFrINFO: undefined produced parts " <<undefProdPart << std::endl;
        std::cout <<"LgFrINFO: undefined consumed parts " <<undefConsPart << std::endl;
        std::cout <<"LgFrINFO: produced capacities " <<prodCapPart << std::endl;
        std::cout <<"LgFrINFO: produced raw parts " <<prodRawPart << std::endl;

      }  // Finished reading .bom file


      // Read optional revenue File
      {
        // Open file
		std::ifstream revF( std::string( baseFileName_ + ".rev" ).c_str() );
        if ( !revF 	)
        {
          std::cerr <<"LgFrINFO: File " + baseFileName_ + ".rev " 
               <<"was not found. Using default revenue values" << std::endl;
        }
        else 
        {
          readFirstRecord( revF, ".rev" );

          // Read remaining records until eof encountered
          long lineCount = 1;
          for (;;)
          {
            lineCount++;
            std::getline( revF, line );
            if (revF.eof() ) break;
            if ( !revF )
            {
              std::cerr <<"LgFrERROR: Read failed: " << baseFileName_ << ".rev. Line " +
                      sce::stringFrom( lineCount ) << std::endl;
              exit( EXIT_FAILURE );
            }

            // Set Demand Name
            SCETokenizer next(line);
            std::string demandName = next();
            std::string partName = next();

#ifdef DEBUG
            witBoolean exists;
            witGetPartExists( wr, partName.c_str(), &exists );
	    if ( ! exists ) {
	       std::cerr << "LgFrERROR:Error while reading revenue file, "
                    << "part does not exit: " << partName
	            <<  std::endl;
	    }
            assert ( exists );
#endif

            // revenue value 
            double revDbl;
			bool result = sce::fromString( next(), revDbl );
//            result = RWLocale::global().stringToNum(
//                     std::string(next()), &revDbl);
            if ( !result )
            {
            std::cerr <<"LgFrERROR: Read failed: " << baseFileName_ << ".rev. Line " +
                    sce::stringFrom( lineCount ) +
                   " Token 3 (revenue)" << std::endl;
              exit( EXIT_FAILURE );
            }
            LgFrTimeVecFloat obj1ShipReward(calNp,(float)revDbl);
            witSetDemandObj1ShipReward( wr, partName.c_str(), demandName.c_str(), 
                                        obj1ShipReward.data() );

          } // end of for loop reading records
        } // if (!refF) ... else { }
      }  // Finished reading .rev File





      // Read optional minLotSize File
      readOperationAttribute( witSetOperationMinLotSize,
                              ".mls",
                              "minLotSize",
                              difEssenPtr );
      
      // Read optional incLotSize File
      readOperationAttribute( witSetOperationIncLotSize,
                              ".ils",
                              "incLotSize",
                              difEssenPtr );
      
      // Read optional Yield File
      readOperationAttribute( witSetOperationYield,
                              ".yld",
                              "yield",
                              difEssenPtr );


  assert (validWitRun() );
}

// ---------------------------------------
// Read Float* Operation attributes      
// ---------------------------------------
void
 LgFrDataInterfaceImplementationFromCmrpFiles::readOperationAttribute(
   apiFuncsetOperationFloatVecAttribute witSetOperation,
   std::string fileExtension,
   std::string attributeName,
   const LgFrDataInterfaceEssentials * difEssenPtr )
const
{
 // Get the witRun 
 LgFrDataInterfaceImplementationFromCmrpFiles * const localThis =
                  ( LgFrDataInterfaceImplementationFromCmrpFiles * const ) this;
 WitRun * wr = localThis->
   LgFrDataInterfaceImplementationFromWitRun::mutableWitRun();

 // Get number of periods in calendar
 const LgFrCalendar& calendar = difEssenPtr->calendar();
 int calNp(calendar.nPeriods());
 
 // Open file
 std::ifstream file( std::string( baseFileName_ + fileExtension ).c_str() );
 if ( !file )
   {
	 std::cerr <<"LgFrINFO: File " << baseFileName_ << fileExtension
          <<"was not found. Using default " + attributeName + " values"
          << std::endl;
   }
   else
   {
     readFirstRecord( file, fileExtension );

     // Read remaining records until eof encountered
     long lineCount = 1;
     std::string line;
     for (;;)
       {
         lineCount++;
         std::getline( file, line );
         if (file.eof() ) break;
         if ( !file )
           {
              std::cerr <<"LgFrERROR: Read failed: " << baseFileName_ <<
				  fileExtension << ". Line " << lineCount << std::endl;
              exit( EXIT_FAILURE );
           }

           // Get operation name
           SCETokenizer next(line);
           std::string operationName = next();

#ifdef DEBUG
            witBoolean exists;
            witGetOperationExists( wr, operationName.c_str(), &exists );
	    if ( ! exists ) {
	       std::cerr << "LgFrERROR: Error while reading" + attributeName + " file, "
                    << "operation does not exit: " 
		    << operationName <<  std::endl;
	    }
            assert ( exists );
#endif

            // Get attribute value
            double attrValDbl;
            
			bool result = sce::fromString( next(), attrValDbl );
//            bool result = RWLocale::global().stringToNum(
//                                         std::string(next()), &attrValDbl);
            if ( !result )
            {
            std::cerr <<"LgFrERROR: Read failed: " << baseFileName_ <<
                   fileExtension << " Line " << lineCount <<
                   " Token 2 (" << attributeName << ")" << std::endl;
              exit( EXIT_FAILURE );
            }
            LgFrTimeVecFloat attrValTv(calNp,(float)attrValDbl);
            witSetOperation( wr, operationName.c_str(), 
                             attrValTv.data() );

      } // end of for loop reading records
   } // if (!file) ... else { }
} 


// ---------------------------------------
// Read int* Operation attributes      
// ---------------------------------------
void
 LgFrDataInterfaceImplementationFromCmrpFiles::readOperationAttribute(
   apiFuncsetOperationIntVecAttribute witSetOperation,
   std::string fileExtension,
   std::string attributeName,
   const LgFrDataInterfaceEssentials * difEssenPtr )
const
{
 // Get the witRun 
 LgFrDataInterfaceImplementationFromCmrpFiles * const localThis =
                  ( LgFrDataInterfaceImplementationFromCmrpFiles * const ) this;
 WitRun * wr = localThis->
   LgFrDataInterfaceImplementationFromWitRun::mutableWitRun();

 // Get number of periods in calendar
 const LgFrCalendar& calendar = difEssenPtr->calendar();
 int calNp(calendar.nPeriods());
 
 // Open file
 std::ifstream file( std::string( baseFileName_ + fileExtension ).c_str() );
 if ( !file )
   {
     std::cerr <<"LgFrINFO: File " + baseFileName_ + fileExtension
          <<"was not found. Using default " + attributeName + " values"
          << std::endl;
   }
   else
   {
     readFirstRecord( file, fileExtension );

     // Read remaining records until eof encountered
     long lineCount = 1;
     std::string line;
     for (;;)
       {
         lineCount++;
         std::getline( file, line );
         if (file.eof() ) break;
         if ( !file )
           {
              std::cerr <<"LgFrERROR: Read failed: " << baseFileName_ <<
                     fileExtension << ". Line " << lineCount << std::endl;
              exit( EXIT_FAILURE );
           }

           // Get operation name
           SCETokenizer next(line);
           std::string operationName = next();

#ifdef DEBUG
            witBoolean exists;
            witGetOperationExists( wr, operationName.c_str(), &exists );
	    if ( ! exists ) {
	       std::cerr << "LgFrERROR: Error while reading" + attributeName + " file, "
                    << "operation does not exit: " 
		    << operationName <<  std::endl;
	    }
            assert ( exists );
#endif

            // Get attribute value
            double attrValDbl;
            
			bool result = sce::fromString( next(), attrValDbl );
//            bool result = RWLocale::global().stringToNum(
//                                         std::string(next()), &attrValDbl);
            if ( !result )
            {
            std::cerr <<"LgFrERROR: Read failed: " << baseFileName_ <<
                   fileExtension << " Line " << lineCount <<
                   " Token 2 (" << attributeName << ")" << std::endl;
              exit( EXIT_FAILURE );
            }
            LgFrTimeVecFloat attrValTv(calNp,(float)attrValDbl);

            witSetOperation( wr, operationName.c_str(), 
                                 attrValTv.intData() );

      } // end of for loop reading records
      
   } // if (!file) ... else { }
}


WitRun * 
  LgFrDataInterfaceImplementationFromCmrpFiles::mutableWitRun(
      const LgFrDataInterfaceEssentials * difEssenPtr)
{
  if( !validWitRun() ) defineWitRun(difEssenPtr);
  return LgFrDataInterfaceImplementationFromWitRun::mutableWitRun();
}

WitRun * 
  LgFrDataInterfaceImplementationFromCmrpFiles::constWitRun(
      const LgFrDataInterfaceEssentials * difEssenPtr)
const
{
  if( !validWitRun() ) defineWitRun(difEssenPtr);
  return LgFrDataInterfaceImplementationFromWitRun::constWitRun();
}

void
LgFrDataInterfaceImplementationFromCmrpFiles::readFirstRecord(
  std::ifstream & inputFile,
  const std::string& fileSuffix )
const
{
  std::string line;

  // Read first line and do sanity check
  std::getline( inputFile, line );

  //inputFile >> line;
  assert( !inputFile.eof() && std::string( "LgFrERROR: Read failed:" + baseFileName_ + fileSuffix + ". Line 1" ).c_str() );
  SCETokenizer next(line);
  //RWCSubString token = next();
  long nPeriods;
  bool result = sce::fromString( next(), nPeriods );
  //bool result = RWLocale::global().stringToNum(token, &nPeriods);
  if ( !result )
  {
    std::cerr <<"LgFrERROR: Read failed: " + baseFileName_ + fileSuffix +
           ". Line 1, token 1"
         << std::endl;
    exit( EXIT_FAILURE );
  }
//  assert( nPeriods == numberOfPeriods() );
}

LgFrCalendar
LgFrDataInterfaceImplementationFromCmrpFiles::calendar (
    const std::string& fileNameWithoutSuffix
)
{
  LgFrCalendar retVal;
  LgFrTimeUnit periodSize = LgFrWeek;
  long nPeriods = 0;
  LgFrDatetime startDate;
  {
    std::string fileName = fileNameWithoutSuffix + ".raw";

    // Open file
    std::ifstream rawF( fileName.c_str() );
    if ( !rawF )
    {
      std::cerr <<"LgFrERROR: File open failed: " + fileName << std::endl;
      exit( EXIT_FAILURE );
    }

    // Read first line
    std::string line;
    std::getline( rawF, line );
    if ( !rawF )
    {
      std::cerr <<"LgFrERROR: Read failed:" << fileName << ". Line 1" << std::endl;
      exit( EXIT_FAILURE );
    }

    // Get number of periods
    SCETokenizer next(line);
    //RWCSubString token = next();
    //bool result = RWLocale::global().stringToNum(token, &nPeriods);
    bool result = sce::fromString( next(), nPeriods );
    if ( !result )
    {
      std::cerr <<"LgFrERROR: Read failed: " + fileName + ". Line 1, token 1"
	   << std::endl;
      exit( EXIT_FAILURE );
    }

    // Get starting day
    //RWCSubString token2 = next();
    long day = 0;
    //result = RWLocale::global().stringToNum(token2, &day);
    result = sce::fromString( next(), day );
    if ( !result )
    {
      std::cerr <<"LgFrERROR: Read failed: " << fileName << ". Line 1, token 2"
	   << std::endl;
      exit( EXIT_FAILURE );
    }

    // Get starting month
    //RWCSubString token3 = next();
    long month = 0;
    //result = RWLocale::global().stringToNum(token3, &month);
    result = sce::fromString( next(), month );
    if ( !result )
    {
      std::cerr <<"LgFrERROR: Read failed: " << fileName << ". Line 1, token 3"
	   << std::endl;
      exit( EXIT_FAILURE );
    }

    // Get starting year
    //RWCSubString token4 = next();
    long year = 0;
    //result = RWLocale::global().stringToNum(token4, &year);
    result = sce::fromString( next(), year );
    if ( !result )
    {
      std::cerr <<"LgFrERROR: Read failed: " << fileName << ". Line 1, token 4"
	   << std::endl;
      exit( EXIT_FAILURE );
    }

    // Get period size 
    std::string periodSizeString = next();
	sce::toLower( periodSizeString );
    if      ( periodSizeString == "hour"  ) periodSize = LgFrHour;
    else if ( periodSizeString == "day"   ) periodSize = LgFrDay;
    else if ( periodSizeString == "week"  ) periodSize = LgFrWeek;
    else if ( periodSizeString == "month" ) periodSize = LgFrMonth;
    else
    {
      std::cerr <<"LgFrERROR: Read failed: " + fileName + ". Line 1, token 5"
	   << std::endl;
      exit( EXIT_FAILURE );
    }

    // Create first period start date
    startDate.setDate( month, day, year );
    if ( !startDate.isValid() )
    {
      std::cerr <<"LgFrERROR: Read failed: " + fileName +
             ". Line 1, tokens 2 through 4"
	   << std::endl
	   <<"Invalid starting date specified." << std::endl;
      exit( EXIT_FAILURE );
    }

    retVal = LgFrCalendar(nPeriods, startDate, periodSize );
  }

  {
    // Check to see if a *.cal file exists
    std::string calFileName = fileNameWithoutSuffix + ".cal";

    // Open file
    std::ifstream calF( calFileName.c_str() );
    if ( calF )
      {
        LgFrDatetimeVector periodStartVec(nPeriods+1);
        std::string line;
        // Loop once reading each line of file
        int i = 0; // Pulled out of the for below by RW2STL
        for ( i = 0; i<=nPeriods; i++ ) {
            std::getline( calF, line );
           if ( !calF )
             {
             std::cerr <<"LgFrERROR: Read failed:" << calFileName << ". Line " <<i+1 << std::endl;
             exit( EXIT_FAILURE );
             }

           // Get period number
           SCETokenizer next(line);
           //RWCSubString token = next();
           long period;
           //bool result = RWLocale::global().stringToNum(token, &period);
           bool result = sce::fromString( next(), period);
           if ( !result )
             {
             std::cerr <<"LgFrERROR: Read failed:" << calFileName <<
                    ". Line " << i+1 <<", token 1." << std::endl;
             exit( EXIT_FAILURE );
             }
           if ( period != i )
             {
             std::cerr <<"LgFrERROR: Data problem in file:" << calFileName <<
                    ". Line " <<i+1 <<", token 1." << std::endl
                  <<"           Period read was " <<period << std::endl
                  <<"           Expected a value of " <<i << std::endl;
             exit( EXIT_FAILURE );
             }

          // Get starting day
          //RWCSubString token2 = next();
          long day = 0;
          result = sce::fromString( next(), day); // RWLocale::global().stringToNum(token2, &day);
          if ( !result )
            {
            std::cerr <<"LgFrERROR: Read failed: " << calFileName <<
                   ". Line " <<i+1 <<", token 2." << std::endl;
            exit( EXIT_FAILURE );
            }

          // Get starting month
          //RWCSubString token3 = next();
          long month = 0;
          result = result = sce::fromString( next(), month); // RWLocale::global().stringToNum(token3, &month);
          if ( !result )
            {
            std::cerr <<"LgFrERROR: Read failed: " << calFileName <<
                   ". Line " <<i+1 <<", token 3." << std::endl;
            exit( EXIT_FAILURE );
            }

          // Get starting year
          //RWCSubString token4 = next();
          long year = 0;
          result = result = sce::fromString( next(), year); // RWLocale::global().stringToNum(token4, &year);
          if ( !result )
            {
            std::cerr <<"LgFrERROR: Read failed: " << calFileName <<
                   ". Line " <<i+1 <<", token 4." << std::endl;
            exit( EXIT_FAILURE );
            }
          
         // Create period start time
         LgFrDatetime periodStartTime;
         periodStartTime.setDate( month, day, year );
         if ( !periodStartTime.isValid() )
           {
           std::cerr <<"LgFrERROR: Read failed: " << calFileName << ". Line " <<i+1
                <<", tokens 2 through 4"
	        << std::endl
	        <<"Invalid date specified." << std::endl;
                exit( EXIT_FAILURE );
           }

         periodStartVec[i] = periodStartTime;

       }  // end of for loop
       retVal =  LgFrCalendar(nPeriods, periodStartVec, periodSize );
        
      } // end of reading *.cal file
  }
  return retVal;
}


void
LgFrDataInterfaceImplementationFromCmrpFiles::deleteExtraResources()
{
  deleteWitRun();
  freeDictKeysAndValues();
}



// Free private dictionary keys and values.
void
LgFrDataInterfaceImplementationFromCmrpFiles::freeDictKeysAndValues()
{
    // Nothing to do with stl map<>
    /*
  size_t d = 0; // Pulled out of the for below by RW2STL
  for ( d=0; dictPtrVec_[d]!=0; d++ )
  {
    RWHashDictionaryIterator iter(*(dictPtrVec_[d]));
    for ( ;; )
    {
      if ( iter() == 0 ) break;
      delete iter.key();
      delete iter.value();
    }
    dictPtrVec_[d]->clear();
  }*/
}


// Deep copy private dictionary keys and values.
void
LgFrDataInterfaceImplementationFromCmrpFiles::deepCopyDictKeysAndValues(
   const LgFrDataInterfaceImplementationFromCmrpFiles& source)
{
  size_t d = 0; // Pulled out of the for below by RW2STL

  constrainedDict_.clear();
  for( ConstrainedDictionary::const_iterator itc = source.constrainedDict_.begin();
        itc != source.constrainedDict_.end();
        itc++ )
  {
    constrainedDict_.insert( *itc );
  }

  procLeadTimeDict_.clear();
  for( ProcLeadTimeDictionary::const_iterator itp = source.procLeadTimeDict_.begin();
        itp != source.procLeadTimeDict_.end();
        itp++ )
  {
    procLeadTimeDict_.insert( *itp );
  }

/* for ( d=0; dictPtrVec_[d]!=0; d++ )
  {
    dictPtrVec_[d]->clear();
    RWHashDictionaryIterator iter(*(source.dictPtrVec_[d]));
    for ( ;; )
    {
      if ( iter() == 0 ) break;
      assert( iter.key()->is A() == __RWCOLLECTABLESTRING );
      assert( iter.value()->is A() == __RWCOLLECTABLEINT ||
              iter.value()->is A() == __LGFRCOLLECTABLEFLOAT );
      key = new std::string( *((std::string*)(iter.key())) );
      if ( iter.value()->is A() == __RWCOLLECTABLEINT )
         value = new RWCollectableInt( *((RWCollectableInt*)(iter.value())) );
      else
         value = new LgFrCollectableFloat(
                        *((LgFrCollectableFloat*)(iter.value())) );
      dictPtrVec_[d]->insertKeyAndValue( key, value );
    }
  } */
}


// Make clone copy of object
LgFrDataInterfaceImplementation *
LgFrDataInterfaceImplementationFromCmrpFiles::clone()
const
{
  LgFrDataInterfaceImplementationFromCmrpFiles* newInst =
      new LgFrDataInterfaceImplementationFromCmrpFiles(*this);
  return (newInst);
}

// Destructor
LgFrDataInterfaceImplementationFromCmrpFiles::~LgFrDataInterfaceImplementationFromCmrpFiles()
{
  // free key and values in dictionaries
  freeDictKeysAndValues();
}

// Assignment operator.
LgFrDataInterfaceImplementationFromCmrpFiles&
LgFrDataInterfaceImplementationFromCmrpFiles::operator=(
  const LgFrDataInterfaceImplementationFromCmrpFiles& rhs)
{
  if (this != &rhs) {           // Check for assignment to self
     (this->LgFrDataInterfaceImplementationFromWitRun::operator= ) (rhs);
     
     
     baseFileName_ = rhs.baseFileName_;
     freeDictKeysAndValues();
     deepCopyDictKeysAndValues(rhs);

  }
  return *this;
}

// Copy constructor
LgFrDataInterfaceImplementationFromCmrpFiles::LgFrDataInterfaceImplementationFromCmrpFiles(
  const LgFrDataInterfaceImplementationFromCmrpFiles& source)
:
   LgFrDataInterfaceImplementationFromWitRun(source),
   baseFileName_( source.baseFileName_ ),
   constrainedDict_(),
   procLeadTimeDict_()
{
   //dictPtrVec_[0] = &constrainedDict_;
   //dictPtrVec_[1] = &procLeadTimeDict_;
   //dictPtrVec_[2] = 0;
   deepCopyDictKeysAndValues(source);
}



// Preferred Constructor
LgFrDataInterfaceImplementationFromCmrpFiles::LgFrDataInterfaceImplementationFromCmrpFiles(
   std::string  baseFileName)
:
   LgFrDataInterfaceImplementationFromWitRun(),
   baseFileName_( baseFileName ),
   constrainedDict_(),
   procLeadTimeDict_()
{
   //dictPtrVec_[0] = &constrainedDict_;
   //dictPtrVec_[1] = &procLeadTimeDict_;
   //dictPtrVec_[2] = 0;
}

// Default Constructor
// This is a private method
LgFrDataInterfaceImplementationFromCmrpFiles::LgFrDataInterfaceImplementationFromCmrpFiles()
:
   LgFrDataInterfaceImplementationFromWitRun(),
   baseFileName_(""),
   constrainedDict_(),
   procLeadTimeDict_()
{
   //dictPtrVec_[0] = &constrainedDict_;
   //dictPtrVec_[1] = &procLeadTimeDict_;
   //dictPtrVec_[2] = 0;
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
void
LgFrDataInterfaceImplementationFromCmrpFiles::contextTest
  (
   LgFrDataInterface    	* tdif1Ptr,	// testScenario1's
   LgFrLastProblemSolved        & lps,          // testScenario1's
   LgFrDataInterface    	* ts1dupDifPtr,	// testScenario1dup's
   LgFrDataInterface    	* tdif2Ptr,	// testScenario2
   const LgFrScenario           * scenPtr)
{
  const float fltEps = .0001f;

  LgFrDataInterface& tdif1 = *tdif1Ptr;
  LgFrDataInterface& ts1dupDif = *ts1dupDifPtr;
  LgFrDataInterface& tdif2 = *tdif2Ptr;
  LgFrDataInterfaceImplementation* dimp
      = tdif1.dataInterfaceImplementation();
  LgFrDataInterfaceImplementation* dimp2
      = tdif2.dataInterfaceImplementation();
  LgFrDataInterfaceImplementation* dimptsi
      = ts1dupDif.dataInterfaceImplementation();

  
  // Register all functions
  registerCmrpFilesDifImpFunctions(dimp);
  registerCmrpFilesDifImpFunctions(dimp2);

  // Test title method
  assert( tdif1.title() == CMRP_DATA_PATH "difTest" );
  assert( ts1dupDif.title() == CMRP_DATA_PATH "difTest" );
  assert( tdif2.title() == CMRP_DATA_PATH "difTest" );
  assert( dimp->title() == CMRP_DATA_PATH "difTest" );
  assert( dimptsi->title() == CMRP_DATA_PATH "difTest" );
  assert( dimp2->title() == CMRP_DATA_PATH "difTest" );

  // Test calendar method
  {
      
      std::string title = tdif1.title();
      LgFrCalendar c = LgFrDataInterfaceImplementationFromCmrpFiles::
	                     calendar( title );
      assert (c.nPeriods() == 3);
      assert (c.periodStart(0) == LgFrDatetime( "4/10/95"));
      assert (c.periodStart(1) == LgFrDatetime( "4/18/95"));
      assert (c.periodStart(2) == LgFrDatetime( "4/24/95"));
      assert (c.timeUnitType() == LgFrDay);
      assert (c.intervalUnitMultiplier() == 1);
      
  }

  LgFrDataInterfaceImplementationFromCmrpFiles & tdifCmrp1 =
     *( ( LgFrDataInterfaceImplementationFromCmrpFiles * ) dimp);
  LgFrDataInterfaceImplementationFromCmrpFiles & tdifCmrp2 =
     *( ( LgFrDataInterfaceImplementationFromCmrpFiles * ) dimp2);


#ifdef COPY_CONSTRUCTOR_WORKING
  LgFrDataInterfaceImplementationFromCmrpFiles  tdifCmrp1Copy(tdifCmrp1);
#endif
  LgFrDataInterfaceImplementationFromCmrpFiles  tdifCmrp1Assign;
  tdifCmrp1Assign = tdifCmrp1;

  // Test number Of Periods using LgFrDataInterfaceEssentialsFromScenario method
  LgFrDataInterfaceEssentialsFromScenario difEssen(scenPtr);
  const LgFrCalendar& calendar = difEssen.calendar();
  int calNp(calendar.nPeriods());
  assert( calNp == 3 );

  // Test title method
  assert(  dimp->title(&difEssen) == tdifCmrp1.baseFileName_ );


  // Test Get Part Methods
  LgFrPartVector partpv;
  std::string itemSubTypeStr = "material";
  LgFrPartVector* rawPartsPtr
    = (LgFrPartVector*) tdif2.partAttribute(
        partpv, itemSubTypeStr, "rawParts" , &difEssen);
  LgFrPartVector raw = *rawPartsPtr;
  size_t t = 0; // Pulled out of the for below by RW2STL
  for(t = 0 ; t < raw.size() ; t++)
    witAddPart( lps.witRun(), raw[t].name().c_str(), WitRAW);
  assert( raw.size() == 2 );
  assert( raw[0].name() == "raw1" );
  assert( raw[1].name() == "raw2" );

  LgFrPartVector* productPartsPtr
    = (LgFrPartVector*) tdif2.partAttribute(
        partpv, itemSubTypeStr, "productParts", &difEssen );
  LgFrPartVector product = * productPartsPtr;
  for( t = 0 ; t < product.size() ; t++)
    witAddPart( lps.witRun(), product[t].name().c_str(), WitPRODUCT);
  assert( product.size() == 2 );
  assert( product[0].name() == "prod1" );
  assert( product[1].name() == "sub1" );

  LgFrPartVector* capacityPartsPtr
    = (LgFrPartVector*) tdif2.partAttribute(
        partpv, itemSubTypeStr, "capacityParts", &difEssen );
  LgFrPartVector capacity = * capacityPartsPtr;
  for( t = 0 ; t < capacity.size() ; t++)
    witAddPart( lps.witRun(), capacity[t].name().c_str(), WitCAPACITY);
  assert( capacity.size() == 1 );
  assert( capacity[0].name() == "cap1" );


  // Test get raw partAttribute methods
  {
  LgFrFloatVector* unitCostVectPtr
    = (LgFrFloatVector*) tdif2.partAttribute(
	raw,itemSubTypeStr,"rawUnitCost", &difEssen);
  assert( unitCostVectPtr->size() == 2 );
  assert ( fabs((*unitCostVectPtr)[0] - 1.0) < fltEps );
  assert ( fabs((*unitCostVectPtr)[1] - 2.0) < fltEps );

  delete unitCostVectPtr;

  LgFrIntVector* constrainedVectPtr
   = (LgFrIntVector*) tdif2.partAttribute(
       raw,itemSubTypeStr,"rawConstrained", &difEssen);
  assert( constrainedVectPtr->size() == 2 );
  assert(  (*constrainedVectPtr)[0] );
  assert( !(*constrainedVectPtr)[1] );

  delete constrainedVectPtr;
#ifdef COPY_CONSTRUCTOR_WORKING
  constrained = tdifCmrp1Copy.partAttribute(raw,rawInstance,LgFrConstrained());
  assert( constrained.length() == 2 );
  assert(  constrained[0] );
  assert( !constrained[1] );
  constrained = tdifCmrp1Assign.partAttribute(
                    raw,rawInstance,LgFrConstrained());
  assert( constrained.length() == 2 );
  assert(  constrained[0] );
  assert( !constrained[1] );
#endif

  LgFrTimeVecFloatVector* procLeadTimeVectPtr
   = (LgFrTimeVecFloatVector*) tdif2.partAttribute(
       raw,itemSubTypeStr,"rawProcLeadTime", &difEssen);
  assert( procLeadTimeVectPtr->size() == 2 );
  assert( (*procLeadTimeVectPtr)[0] == LgFrTimeVecFloat(3,.5) );
  assert( (*procLeadTimeVectPtr)[1] == LgFrTimeVecFloat(3,1.) );

  delete procLeadTimeVectPtr;

#ifdef COPY_CONSTRUCTOR_WORKING
  procLT = tdifCmrp1Copy.partAttribute(raw,rawInstance,LgFrProcLeadTime());
  assert( procLT.size() == 2 );
  assert( procLT[0] == LgFrTimeVecFloat(3,.5) );
  assert( procLT[1] == LgFrTimeVecFloat(3,1.) );
  procLT = tdifCmrp1Assign.partAttribute(raw,rawInstance,LgFrProcLeadTime());
  assert( procLT.size() == 2 );
  assert( procLT[0] == LgFrTimeVecFloat(3,.5) );
  assert( procLT[1] == LgFrTimeVecFloat(3,1.) );
#endif
  
  LgFrTimeVecFloatVector* supplyVolVectPtr
   = (LgFrTimeVecFloatVector*) tdif2.partAttribute(
       raw,itemSubTypeStr,"rawSupplyVol", &difEssen);
  assert( supplyVolVectPtr->size() == 2 );
  assert( (*supplyVolVectPtr)[0] == LgFrTimeVecFloat(3," 2 4.0 5") );
  assert( (*supplyVolVectPtr)[1] == LgFrTimeVecFloat(3," 2 2 2") );

  delete supplyVolVectPtr;
  delete rawPartsPtr;
  }

  // Test get product partAttribute methods
  {
  LgFrTimeVecFloatVector* cycleTimeVectPtr
   = (LgFrTimeVecFloatVector*) tdif2.partAttribute(
       product,itemSubTypeStr,"productCycleTime", &difEssen);
  assert( cycleTimeVectPtr->size() == 2 );
  assert( (*cycleTimeVectPtr)[0] == LgFrTimeVecFloat(3,.5) );
  assert( (*cycleTimeVectPtr)[1] == LgFrTimeVecFloat(3,.25) );
 
  delete cycleTimeVectPtr;

  LgFrFloatVector* unitCostVectPtr2
    = (LgFrFloatVector*) tdif2.partAttribute(
        product,itemSubTypeStr,"productUnitCost", &difEssen);
  assert( unitCostVectPtr2->size() == 2 );
  assert ( fabs((*unitCostVectPtr2)[0] - 1.3) < fltEps );
  assert ( fabs((*unitCostVectPtr2)[1] - 2.3) < fltEps );

  delete unitCostVectPtr2;
  
  LgFrIntVector* constrainedVectPtr2
   = (LgFrIntVector*) tdif2.partAttribute(
       product,itemSubTypeStr,"productConstrained", &difEssen);
  assert( constrainedVectPtr2->size() == 2 );
  for (unsigned int i=0; i<constrainedVectPtr2->size(); i++) 
      assert( (*constrainedVectPtr2)[i] );

  delete constrainedVectPtr2;

  LgFrTimeVecFloatVector* supplyVolVectPtr2
   = (LgFrTimeVecFloatVector*) tdif2.partAttribute(
       product,itemSubTypeStr,"productSupplyVol", &difEssen);
  assert( supplyVolVectPtr2->size() == 2 );
  assert( (*supplyVolVectPtr2)[0] == LgFrTimeVecFloat(3,"  2.0  0.0  0.0 ") );
  assert( (*supplyVolVectPtr2)[1] == LgFrTimeVecFloat(3,"  2.0  0.0  0.0 ") );

  delete supplyVolVectPtr2;

  }

  // Test minLotSize
  {
  LgFrTimeVecFloatVector* minLotSizePtr
   = (LgFrTimeVecFloatVector*) tdif2.partAttribute(
       product,itemSubTypeStr,"productMinLotSize", &difEssen);
  LgFrTimeVecFloatVector minLotSize = *minLotSizePtr;
  assert( minLotSize.size() == 2 );
  assert( minLotSize[0] == LgFrTimeVecFloat(3,1.5) );
  assert( minLotSize[1] == LgFrTimeVecFloat(3,.5) );

  delete minLotSizePtr;

  }

  // Test incLotSize
  {
  LgFrTimeVecFloatVector* incLotSizePtr
   = (LgFrTimeVecFloatVector*) tdif2.partAttribute(
       product,itemSubTypeStr,"productIncLotSize", &difEssen);
  LgFrTimeVecFloatVector incLotSize = *incLotSizePtr;
  assert( incLotSize.size() == 2 );
  assert( incLotSize[0] == LgFrTimeVecFloat(3, 0.5) );
  assert( incLotSize[1] == LgFrTimeVecFloat(3, 1.0) );
  delete incLotSizePtr;
  }

  
  // Test yield
  {
  LgFrTimeVecFloatVector* yieldPtr
   = (LgFrTimeVecFloatVector*) tdif2.partAttribute(
       product,itemSubTypeStr,"productYieldFloat", &difEssen);
  LgFrTimeVecFloatVector yield = *yieldPtr;
  assert( yield.size() == 2 );
  assert( yield[0] == LgFrTimeVecFloat(3,89.) );
  assert( yield[1] == LgFrTimeVecFloat(3,95.) );
  delete yieldPtr;
  }
 
  
  delete productPartsPtr;
  // Test get capacity partAttribute methods
  {
  LgFrIntVector* constrainedVectPtr
   = (LgFrIntVector*) tdif2.partAttribute(
       capacity,itemSubTypeStr,"capacityConstrained", &difEssen);
  assert( constrainedVectPtr->size() == 1 );
  assert( (*constrainedVectPtr)[0] );

  delete constrainedVectPtr;

  LgFrTimeVecFloatVector* supplyVolVectPtr
   = (LgFrTimeVecFloatVector*) tdif2.partAttribute(
       capacity,itemSubTypeStr,"capacitySupplyVol", &difEssen);
  assert( supplyVolVectPtr->size() == 1 );
  assert( (*supplyVolVectPtr)[0] == LgFrTimeVecFloat(3," 8 8  16") );
  
  delete supplyVolVectPtr;

  }

  delete capacityPartsPtr;

// Test Get Demands Methods
  LgFrDemandVector demandpv;

  LgFrDemandVector* committedDemandPtr
    = (LgFrDemandVector*) tdif2.demandAttribute(
        demandpv, "committedDemands", &difEssen );
  LgFrDemandVector committed = * committedDemandPtr;
  assert( committed.size() == 2 ); 

  LgFrDemandVector* newOpportunityDemandPtr
    = (LgFrDemandVector*) tdif2.demandAttribute(
        demandpv, "newOpportunityDemands", &difEssen );
  LgFrDemandVector newOpportunity = * newOpportunityDemandPtr;
  assert( newOpportunity.size() == 0 );

  LgFrDemandVector* firmForecastDemandPtr
    = (LgFrDemandVector*) tdif2.demandAttribute(
        demandpv, "firmForecastDemands", &difEssen );
  LgFrDemandVector firmForecast = * firmForecastDemandPtr;
  assert( firmForecast.size() == 0 );


  LgFrDemandVector* riskForecastDemandPtr
    = (LgFrDemandVector*) tdif2.demandAttribute(
        demandpv, "riskForecastDemands", &difEssen );
  LgFrDemandVector riskForecast = * riskForecastDemandPtr;
  assert( riskForecast.size() == 0 );


  // Test get demand Attribute methods
  LgFrTimeVecFloatVector * demandVolVectPtr = (LgFrTimeVecFloatVector *)
      tdif2.demandAttribute(committed, "demandVol", &difEssen);
  assert(demandVolVectPtr->size() == 2 );
  assert( (*demandVolVectPtr)[0] == LgFrTimeVecFloat(3," 2 1 2") );
  assert( (*demandVolVectPtr)[1] == LgFrTimeVecFloat(3," 1 1 1") );

  delete demandVolVectPtr;

  LgFrTimeVecFloatVector * demandVolVectPtr2 = (LgFrTimeVecFloatVector *)
      tdif2.demandAttribute(newOpportunity, "demandVol", &difEssen);
  assert(demandVolVectPtr2->size() == 0 );

  delete demandVolVectPtr2;
  delete newOpportunityDemandPtr;

  LgFrTimeVecFloatVector * demandVolVectPtr3 = (LgFrTimeVecFloatVector *)
      tdif2.demandAttribute(firmForecast, "demandVol", &difEssen);
  assert(demandVolVectPtr3->size() == 0 );

  delete demandVolVectPtr3; 
  delete firmForecastDemandPtr;

  LgFrTimeVecFloatVector * demandVolVectPtr4 = (LgFrTimeVecFloatVector *)
      tdif2.demandAttribute(riskForecast, "demandVol", &difEssen);
  assert(demandVolVectPtr4->size() == 0 );

  delete demandVolVectPtr4;
  delete riskForecastDemandPtr;

  LgFrTimeVecFloatVector* priorityPtr
    = (LgFrTimeVecFloatVector*) tdif2.demandAttribute(committed,
                                            "demandPriority", &difEssen);
  assert(priorityPtr->size() == 2 );
  assert( (*priorityPtr)[0] == LgFrTimeVecFloat(3," 5 5 5") );
  assert( (*priorityPtr)[1] == LgFrTimeVecFloat(3," 4 4 4") );

  delete priorityPtr;

  LgFrTimeVecFloatVector* obj1ShipRewardPtr
    = (LgFrTimeVecFloatVector*) 
        tdif2.demandAttribute(committed,
				  "demandObj1ShipReward", &difEssen);
  LgFrTimeVecFloatVector obj1ShipReward = *obj1ShipRewardPtr;
  assert(obj1ShipReward.size() == 2 );
  assert( obj1ShipReward[0] == LgFrTimeVecFloat(3,35.0) );
  assert( obj1ShipReward[1] == LgFrTimeVecFloat(3,40.0) );



  delete obj1ShipRewardPtr;
  delete committedDemandPtr;


  // Test BOM
  tdif1.partAttribute(
        partpv, itemSubTypeStr, "defineBom", &difEssen );
  int nBomEntries;
  float retFloat;
  char * consumedPartName; 
  witGetPartNBomEntries( lps.witRun(), "prod1", &nBomEntries);
  assert( nBomEntries == 3);
  witGetBomEntryConsumedPart(lps.witRun(), "prod1", 0,
                             &consumedPartName);
  assert ( std::string (consumedPartName) == std::string ("sub1"));
  portableFree (consumedPartName);
  witGetBomEntryUsageRate( lps.witRun(), "prod1", 0,
                           &retFloat);
  assert ( fabs(retFloat - 1.0) < fltEps );
  witGetBomEntryConsumedPart(lps.witRun(), "prod1", 2,
                             &consumedPartName);
  assert ( std::string (consumedPartName) == std::string ("cap1"));
  portableFree (consumedPartName);
  witGetBomEntryUsageRate(lps.witRun(), "prod1", 2,
                               &retFloat);
  assert ( fabs(retFloat - .25) < fltEps );

  witGetPartNBomEntries( lps.witRun(), "sub1", &nBomEntries);
  assert( nBomEntries == 1);
  witGetBomEntryConsumedPart(lps.witRun(), "sub1", 0,
                             &consumedPartName);
  assert ( std::string (consumedPartName) == std::string ("raw1"));
  portableFree (consumedPartName);
  witGetBomEntryUsageRate( lps.witRun(), "sub1", 0,
                           &retFloat);
  assert ( fabs(retFloat - 3.0) < fltEps );
}
