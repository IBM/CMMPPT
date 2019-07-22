#include <mcl/src/mdsPragma.h>
// RW2STL - inserted:
#include <map>
#include <string>
#include <iostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
#include <wit/src/wit.h>


#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <scenario/src/part.h>
#include <scenario/src/timeVecs.h>
#include <scenario/src/partSchd.h>
#include <scenario/src/pairComp.h>

#include <sce/src/scePegging.h>
#include <sce/src/mPlantH.h>
#include <iostream>
#include <fstream>

// #define DEBUG_INTERPLANT_PEG



//------------------------------------------------------------
// Update the dictionary of consVols.
// Key is (consumedPartName,consumptionPeriod).
// Value is consVol.
// If the entry does not exist, then a new entry in dictionary
// is created with deltaConsVol.
// If an entry exists then it is updated by adding deltaConsVol
//
//-------------------------------------------------------------
void ScePIP::updateConsDict(
                                 PartNamePeriodConsVolDictionary & consDict,
                                 char * consumedPartName,
                                 int consPeriod,
                                 double deltaConsVol)
{

  // Create Dictionary Key from partName and period
  SceConstCharStar_Int key(consumedPartName,consPeriod);
  SceCollectableDouble newConsVol(deltaConsVol);
  
  // insert key and consVol into dicionary
  bool inserted = consDict.insert( PartNamePeriodConsVolPair( key,
                                                            newConsVol ) ).second;

  // Did the insert fail because the key already exists?
  if( !inserted )
  {        
    PartNamePeriodConsVolDictionary::iterator it = consDict.find(key);

    SceCollectableDouble& val = (*it).second;
    double updatedConsVol = val.dbl() + deltaConsVol;
    val.dbl(updatedConsVol);
  }
}


//----------------------------------------------------------------------
// Update ConsDict given consumed part name, consRate, impactPeriod,
// execVolOrSubVol, execPeriod, incrementConsVol flag.
// This is used 3 times:
//   increment bom consumed part for execVol
//   decrement bom consumed part for subVol
//   increment subs consumed part for subVol
//-----------------------------------------------------------------------
void ScePIP::computeConsVolAndUpdateConsDict(
  WitRun * wr,
  char * consumedPartName,
  float * consRate,
  float fallout,
  int * impactPeriod,
  float execOrSubVol,
  int execPeriod,
  bool increment,
  PartNamePeriodConsVolDictionary & consDict )
{
   
  // Test to see if bom is effective in execution period
  if ( impactPeriod[execPeriod] == -1 ) 
    return;
 
  int consPeriod = impactPeriod[execPeriod];
  double incrConsVol = execOrSubVol * consRate[consPeriod]/(1.0-fallout);
  //double incrConsVol = execOrSubVol * consRate[execPeriod]/(1.0-fallout);
  if ( !increment ) {
    incrConsVol = -incrConsVol;
  }
  updateConsDict(consDict,consumedPartName,consPeriod,incrConsVol);
}


//-----------------------------------------------------------------------
// ConsVol for a consumed part connected to a bom is computed from
// the the operations exexVol less any subVols associated with the Bom.
// The process for working with execVol and subVols is the same execpt
// that execVols add to the consVol and subVols subtract from the consVol.
// This function handles both of these operations.
//-----------------------------------------------------------------------
void ScePIP::updateConsDictFromExecOrSubVolAndBom(
                                     WitRun * wr, 
                                     const char * opName,
                                     int bomEntryIndex,
                                     int execPeriod,
                                     float execOrSubVol,
                                     bool isExecVol,
                                     PartNamePeriodConsVolDictionary & consDict)
{
  
  // get name of consumed part
  char * consumedPartName;
  witGetBomEntryConsumedPart ( wr, opName, bomEntryIndex, &consumedPartName );
  
  // get consumption rate
  float * consRate;
  witGetBomEntryConsRate ( wr, opName, bomEntryIndex, &consRate );

  // get consumption rate
  float falloutRate;
  witGetBomEntryFalloutRate ( wr, opName, bomEntryIndex, &falloutRate );
  
  // get impact periods
  int * impactPeriod;
  witGetBomEntryImpactPeriod ( wr, opName, bomEntryIndex, &impactPeriod );
  
  computeConsVolAndUpdateConsDict( wr, consumedPartName, consRate, falloutRate, impactPeriod,
    execOrSubVol, execPeriod, isExecVol, consDict );
  
  witFree(impactPeriod);
  witFree(consRate);
}
    


















// ===========================================
//---------------------------------------------------------------------------
// Populate HashDictionary with DemandConsVol PIP
//---------------------------------------------------------------------------                                    
witReturnCode ScePIP::getDemandConsVolPIP (
                                                   WitRun * const wr,
                                                   const char * const partName,
                                                   const char * const demandName,
                                                   const int firstShipPeriod,
                                                   const int lastShipPeriod,
                                                   bool sceConsVol,
                                                   bool ignoreSubstituteUsage,
                                                   PartNamePeriodConsVolDictionary & consDict
                                                   )
{
  witReturnCode retVal = WitINFORMATIONAL_MESSAGES;
  
  // Get shipVol of input demand
  float * shipVol;
  witGetDemandShipVol(wr,partName,demandName,&shipVol);
  
  // Loop once for each shipPeriod
  int sp;
  for ( sp=firstShipPeriod; sp<=lastShipPeriod; sp++ ) {
    
    // If shipVol is zero, then there is nothing to peg
    if (shipVol[sp]==0) continue;
    
    // Call wit to get pegged supplyVol
    int lenPartList,pn;
    char ** partNameList;
    int * periodList;
    float * pipDemandConsVolList;
    retVal = witGetDemandConsVolPip(wr,partName,demandName,sp,
      &lenPartList,&partNameList,&periodList,&pipDemandConsVolList);
    
    // Size consDict hashtable if first loop iteration
    // can't resize std::map
    // if ( sp==firstShipPeriod )execDict.resize(lenOpList*20*(lastShipPeriod-firstShipPeriod));
    
    // Loop once per part  with an pipDemandConsVol
    for ( pn=0; pn<lenPartList; pn++ ){     
      updateConsDict(consDict,partNameList[pn],periodList[pn],pipDemandConsVolList[pn]);      
    }
    
    // Free storage allocated by witGetDemandExecVolPegging
    //for ( op=0; op<lenOpList; op++ )witFree(opNameList[op]);  //Don't free because char* are referenced by schedule
    witFree(partNameList);
    witFree(periodList);
    witFree(pipDemandConsVolList);
    
    if( ignoreSubstituteUsage ) {
      //----------------------------------------------------
      // Get SubVol. Update existing consVol and compute new consVols
      //----------------------------------------------------
    // Call wit to get PIP execVol
      int lenOpList,op;
      char ** opNameList;
      int * execPeriodList;


      float * PIPSubVolList;
      int * bomEntryIndexList;
      int * subsBomEntryIndexList;
      witGetDemandSubVolPip (wr,partName,demandName,sp,
			     &lenOpList,&opNameList,&bomEntryIndexList,&subsBomEntryIndexList,
			     &execPeriodList,&PIPSubVolList);
      
      // Loop once per operation with an SubVol
      for ( op=0; op<lenOpList; op++ ){
	
	// The consVol of the part connected to the bom is now decremented
	updateConsDictFromExecOrSubVolAndBom( wr, 
					      opNameList[op],
					      bomEntryIndexList[op],
					      execPeriodList[op],
					      PIPSubVolList[op],
					      true,
					      consDict);
      
	// Now increment consVol due to subVol 
	// get name of consumed part
	char * consumedPartName;
	witGetSubsBomEntryConsumedPart ( wr, opNameList[op], 
					 bomEntryIndexList[op], subsBomEntryIndexList[op], 
					 &consumedPartName );    
      
	// get consumption rate
	float * consRate;
	witGetSubsBomEntryConsRate ( wr, opNameList[op], 
				     bomEntryIndexList[op], subsBomEntryIndexList[op], 
				     &consRate );
      
	// get fallout rate
	float falloutRate;
	witGetSubsBomEntryFalloutRate ( wr, opNameList[op], 
					bomEntryIndexList[op], subsBomEntryIndexList[op], 
					&falloutRate );
	
	// get impact periods
	int * impactPeriod;
	witGetSubsBomEntryImpactPeriod ( wr, opNameList[op], 
					 bomEntryIndexList[op], subsBomEntryIndexList[op], 
					 &impactPeriod );
      
	computeConsVolAndUpdateConsDict( wr, consumedPartName, consRate, falloutRate, impactPeriod,
					 PIPSubVolList[op], execPeriodList[op], false, consDict );
      
	witFree(impactPeriod);
	witFree(consRate);
      }
    }      
    
 

    //------------------------------------------------------
    // If computing the sce definition of consVol,
    // then add in the shipVol.
    //------------------------------------------------------
    if ( sceConsVol ) {
      char * pn; 
      witMalloc(strlen(partName)+1,(void**)&pn);
      strcpy(pn,partName);
      updateConsDict(consDict,pn,sp,shipVol[sp]);
    }
    
  }  
  
  witFree(shipVol);
  return retVal;
}




//---------------------------------------------------------------------------
// demand cons volume PIP code: Called by SCENARIO
//---------------------------------------------------------------------------
witReturnCode ScePIP::getDemandConsVolPIP (
   WitRun * const wr,
   const char * const partName,
   const char * const demandName,
   const int firstShipPeriod,
   const int lastShipPeriod,
   LgFrSortingPartScheduleFloat & ps,
   bool sceConsVol,
   bool ignoreSubstituteUsage   )
{
  witReturnCode retVal = WitINFORMATIONAL_MESSAGES;

  int nPeriods;
  witGetNPeriods(wr,&nPeriods);
  
  // Create dictionary which will contain partName and period as key and supplyVol as value
  PartNamePeriodConsVolDictionary consDict;

  retVal = getDemandConsVolPIP (
   wr,partName,demandName,firstShipPeriod,lastShipPeriod, sceConsVol, ignoreSubstituteUsage, consDict );

  for( PartNamePeriodConsVolDictionary::iterator it = consDict.begin();
        it != consDict.end();
        it++ )
  {
    //LgFrModelClient* mc = *it;
    SceConstCharStar_Int key = (*it).first;
    SceCollectableDouble v = (*it).second;
    // don't add zero consVols to returned list
    // TOM_WAS_HERE_
    if ( (v.dbl() > -0.0001) && (v.dbl() < 0.0001) ) 
      continue;

    LgFrPart p(key.getCharStar());
    LgFrTimeVecFloat tv(nPeriods);
    int t=key.getInt();
    // Is part already in schedule
    if (ps.findValue(p,tv)) {
      // part is in schedule, update timeVec
      tv[t] = tv[t] + (float)v.dbl();
    }
    else {
      // part is not in schedule, so insert it
      tv[t] = (float)v.dbl();
    }
    ps.insertKeyAndValue(p,tv);
  }

  
  return retVal;
}
// ==========================================


// tomtom
//---------------------------------------------------------------------------
// Populate HashDictionary with ProdVol PIP Volume
//---------------------------------------------------------------------------                                    
witReturnCode ScePIP::getProdVolPIP (
                                                   WitRun * const wr,
                                                   const char * const partName,
                                                   const char * const demandName,
                                                   const int firstShipPeriod,
                                                   const int lastShipPeriod,
                                                   PartNamePeriodConsVolDictionary & prodDict
                                                   )
{
  witReturnCode retVal = WitINFORMATIONAL_MESSAGES;
  
  // Get shipVol of input demand
  float * shipVol;
  witGetDemandShipVol(wr,partName,demandName,&shipVol);
  
  // Loop once for each shipPeriod
  int sp;
  for ( sp=firstShipPeriod; sp<=lastShipPeriod; sp++ ) {
    
    // If shipVol is zero, then there is nothing to peg
    if (shipVol[sp]==0) continue;
    
    // Call wit to get pegged prodVol
    int lenPartList,pn;
    char ** partNameList;
    int * periodList;
    float * pipProdVolList;
    retVal = witGetDemandProdVolPip(wr,partName,demandName,sp,
      &lenPartList,&partNameList,&periodList,&pipProdVolList);
    
    // Size consDict hashtable if first loop iteration
    // can't resize std::map
    // if ( sp==firstShipPeriod )execDict.resize(lenOpList*20*(lastShipPeriod-firstShipPeriod));
    
    // Loop once per part  with an pipSupplyVol
    for ( pn=0; pn<lenPartList; pn++ ){     
      updateConsDict(prodDict,partNameList[pn],periodList[pn],pipProdVolList[pn]);      
    }
    
    // Free storage allocated by witGetDemandExecVolPegging
    //for ( op=0; op<lenOpList; op++ )witFree(opNameList[op]);  //Don't free because char* are referenced by schedule
    witFree(partNameList);
    witFree(periodList);
    witFree(pipProdVolList);
  }  
  
  witFree(shipVol);
  return retVal;
}




//---------------------------------------------------------------------------
// prodVol PIP code: Called by SCENARIO
//---------------------------------------------------------------------------
witReturnCode ScePIP::getProdVolPIP (
   WitRun * const wr,
   const char * const partName,
   const char * const demandName,
   const int firstShipPeriod,
   const int lastShipPeriod,
   LgFrSortingPartScheduleFloat & ps
   )
{
  witReturnCode retVal = WitINFORMATIONAL_MESSAGES;

  int nPeriods;
  witGetNPeriods(wr,&nPeriods);

  // Create dictionary which will contain partName and period as key and supplyVol as value
  PartNamePeriodConsVolDictionary prodDict;

  retVal = getProdVolPIP (
   wr,partName,demandName,firstShipPeriod,lastShipPeriod, prodDict );

  for( PartNamePeriodConsVolDictionary::iterator it = prodDict.begin();
        it != prodDict.end();
        it++ )
  {
    //LgFrModelClient* mc = *it;
    SceConstCharStar_Int key = (*it).first;
    SceCollectableDouble v = (*it).second;
    // don't add zero consVols to returned list
    // TOM_WAS_HERE_
    if ( (v.dbl() > -0.0001) && (v.dbl() < 0.0001) ) 
      continue;

    LgFrPart p(key.getCharStar());
    LgFrTimeVecFloat tv(nPeriods);
    int t=key.getInt();
    // Is part already in schedule
    if (ps.findValue(p,tv)) {
      // part is in schedule, update timeVec
      tv[t] = tv[t] + (float)v.dbl();
    }
    else {
      // part is not in schedule, so insert it
      tv[t] = (float)v.dbl();
    }
    ps.insertKeyAndValue(p,tv);
  }

  
  return retVal;
}

// tomtom




//---------------------------------------------------------------------------
// Populate HashDictionary with SideVol PIP Volume
//---------------------------------------------------------------------------                                    
witReturnCode ScePIP::getSideVolPIP (
                                                   WitRun * const wr,
                                                   const char * const partName,
                                                   const char * const demandName,
                                                   const int firstShipPeriod,
                                                   const int lastShipPeriod,
                                                   PartNamePeriodConsVolDictionary & sideDict
                                                   )
{
  witReturnCode retVal = WitINFORMATIONAL_MESSAGES;
  
  // Get shipVol of input demand
  float * shipVol;
  witGetDemandShipVol(wr,partName,demandName,&shipVol);
  
  // Loop once for each shipPeriod
  int sp;
  for ( sp=firstShipPeriod; sp<=lastShipPeriod; sp++ ) {
    
    // If shipVol is zero, then there is nothing to peg
    if (shipVol[sp]==0) continue;
    
    // Call wit to get pegged sideVol
    int lenPartList,pn;
    char ** partNameList;
    int * periodList;
    float * pipSideVolList;
    retVal = witGetDemandSideVolPip(wr,partName,demandName,sp,
      &lenPartList,&partNameList,&periodList,&pipSideVolList);
    
    // Size consDict hashtable if first loop iteration
    // can't resize std::map
    // if ( sp==firstShipPeriod )execDict.resize(lenOpList*20*(lastShipPeriod-firstShipPeriod));
    
    // Loop once per part  with an pipSupplyVol
    for ( pn=0; pn<lenPartList; pn++ ){     
      updateConsDict(sideDict,partNameList[pn],periodList[pn],pipSideVolList[pn]);      
    }
    
    // Free storage allocated by witGetDemandExecVolPegging
    //for ( op=0; op<lenOpList; op++ )witFree(opNameList[op]);  //Don't free because char* are referenced by schedule
    witFree(partNameList);
    witFree(periodList);
    witFree(pipSideVolList);
  }  
  
  witFree(shipVol);
  return retVal;
}




//---------------------------------------------------------------------------
// sideVol PIP code: Called by SCENARIO
//---------------------------------------------------------------------------
witReturnCode ScePIP::getSideVolPIP (
   WitRun * const wr,
   const char * const partName,
   const char * const demandName,
   const int firstShipPeriod,
   const int lastShipPeriod,
   LgFrSortingPartScheduleFloat & ps
   )
{
  witReturnCode retVal = WitINFORMATIONAL_MESSAGES;

  int nPeriods;
  witGetNPeriods(wr,&nPeriods);

  // Create dictionary which will contain partName and period as key and supplyVol as value
  PartNamePeriodConsVolDictionary sideDict;

  retVal = getSideVolPIP (
   wr,partName,demandName,firstShipPeriod,lastShipPeriod, sideDict );

  for( PartNamePeriodConsVolDictionary::iterator it = sideDict.begin();
        it != sideDict.end();
        it++ )
  {
    //LgFrModelClient* mc = *it;
    SceConstCharStar_Int key = (*it).first;
    SceCollectableDouble v = (*it).second;
    // don't add zero consVols to returned list
    // TOM_WAS_HERE_
    if ( (v.dbl() > -0.0001) && (v.dbl() < 0.0001) ) 
      continue;

    LgFrPart p(key.getCharStar());
    LgFrTimeVecFloat tv(nPeriods);
    int t=key.getInt();
    // Is part already in schedule
    if (ps.findValue(p,tv)) {
      // part is in schedule, update timeVec
      tv[t] = tv[t] + (float)v.dbl();
    }
    else {
      // part is not in schedule, so insert it
      tv[t] = (float)v.dbl();
    }
    ps.insertKeyAndValue(p,tv);
  }

  
  return retVal;
}

// tomtom



//---------------------------------------------------------------------------
// Populate HashDictionary with SupplyPIP Volume
//---------------------------------------------------------------------------                                    
witReturnCode ScePIP::getSupplyVolPIP (
                                                   WitRun * const wr,
                                                   const char * const partName,
                                                   const char * const demandName,
                                                   const int firstShipPeriod,
                                                   const int lastShipPeriod,
                                                   PartNamePeriodConsVolDictionary & supplyDict
                                                   )
{
  witReturnCode retVal = WitINFORMATIONAL_MESSAGES;
  
  // Get shipVol of input demand
  float * shipVol;
  witGetDemandShipVol(wr,partName,demandName,&shipVol);
  
  // Loop once for each shipPeriod
  int sp;
  for ( sp=firstShipPeriod; sp<=lastShipPeriod; sp++ ) {
    
    // If shipVol is zero, then there is nothing to peg
    if (shipVol[sp]==0) continue;
    
    // Call wit to get pegged supplyVol
    int lenPartList,pn;
    char ** partNameList;
    int * periodList;
    float * pipSupplyVolList;
    retVal = witGetDemandSupplyVolPip(wr,partName,demandName,sp,
      &lenPartList,&partNameList,&periodList,&pipSupplyVolList);
    
    // Size consDict hashtable if first loop iteration
    // can't resize std::map
    // if ( sp==firstShipPeriod )execDict.resize(lenOpList*20*(lastShipPeriod-firstShipPeriod));
    
    // Loop once per part  with an pipSupplyVol
    for ( pn=0; pn<lenPartList; pn++ ){     
      updateConsDict(supplyDict,partNameList[pn],periodList[pn],pipSupplyVolList[pn]);      
    }
    
    // Free storage allocated by witGetDemandExecVolPegging
    //for ( op=0; op<lenOpList; op++ )witFree(opNameList[op]);  //Don't free because char* are referenced by schedule
    witFree(partNameList);
    witFree(periodList);
    witFree(pipSupplyVolList);
  }  
  
  witFree(shipVol);
  return retVal;
}




//---------------------------------------------------------------------------
// supply volume PIP code: Called by SCENARIO
//---------------------------------------------------------------------------
witReturnCode ScePIP::getSupplyVolPIP (
   WitRun * const wr,
   const char * const partName,
   const char * const demandName,
   const int firstShipPeriod,
   const int lastShipPeriod,
   LgFrSortingPartScheduleFloat & ps
   )
{
  witReturnCode retVal = WitINFORMATIONAL_MESSAGES;

  int nPeriods;
  witGetNPeriods(wr,&nPeriods);

  // Create dictionary which will contain partName and period as key and supplyVol as value
  PartNamePeriodConsVolDictionary supplyDict;

  retVal = getSupplyVolPIP (
   wr,partName,demandName,firstShipPeriod,lastShipPeriod, supplyDict );

  for( PartNamePeriodConsVolDictionary::iterator it = supplyDict.begin();
        it != supplyDict.end();
        it++ )
  {
    //LgFrModelClient* mc = *it;
    SceConstCharStar_Int key = (*it).first;
    SceCollectableDouble v = (*it).second;
    // don't add zero consVols to returned list
    // TOM_WAS_HERE_
    if ( (v.dbl() > -0.0001) && (v.dbl() < 0.0001) ) 
      continue;

    LgFrPart p(key.getCharStar());
    LgFrTimeVecFloat tv(nPeriods);
    int t=key.getInt();
    // Is part already in schedule
    if (ps.findValue(p,tv)) {
      // part is in schedule, update timeVec
      tv[t] = tv[t] + (float)v.dbl();
    }
    else {
      // part is not in schedule, so insert it
      tv[t] = (float)v.dbl();
    }
    ps.insertKeyAndValue(p,tv);
  }

  
  return retVal;
}






//---------------------------------------------------------------------------
// Populate HashDictionary with Exec Volume
//---------------------------------------------------------------------------                                    
witReturnCode ScePIP::getDemandExecVolPIP (
                                                   WitRun * const wr,
                                                   const char * const partName,
                                                   const char * const demandName,
                                                   const int firstShipPeriod,
                                                   const int lastShipPeriod,
                                                   PartNamePeriodConsVolDictionary & execDict
                                                   )
{
  witReturnCode retVal = WitINFORMATIONAL_MESSAGES;
  
  // Get shipVol of input demand
  float * shipVol;
  witGetDemandShipVol(wr,partName,demandName,&shipVol);
  
  // Loop once for each shipPeriod
  int sp;
  for ( sp=firstShipPeriod; sp<=lastShipPeriod; sp++ ) {
    
    // If shipVol is zero, then there is nothing to PIP
    if (shipVol[sp]==0) continue;
    
    // Call wit to get PIP execVol
    int lenOpList,op;
    char ** opNameList;
    int * execPeriodList;
    float * PIPExecVolList;
    retVal = witGetDemandExecVolPip(wr,partName,demandName,sp,
      &lenOpList,&opNameList,&execPeriodList,&PIPExecVolList);
    
    // Size consDict hashtable if first loop iteration
    // can't resize std::map
    // if ( sp==firstShipPeriod )execDict.resize(lenOpList*20*(lastShipPeriod-firstShipPeriod));
    
    // Loop once per operation with an execVol
    for ( op=0; op<lenOpList; op++ ){     
      updateConsDict(execDict,opNameList[op],execPeriodList[op],PIPExecVolList[op]);      
    }
    
    // Free storage allocated by witGetDemandExecVolPIP

    //    for ( op=0; op<lenOpList; op++ )  {
    //      witFree(opNameList[op]);  //Don't free because char* are referenced by schedule
    //    }
    witFree(opNameList);
    witFree(execPeriodList);
    witFree(PIPExecVolList);
  }  
  
  witFree(shipVol);
  return retVal;
}


witReturnCode ScePIP::getDemandExecVolPIP (
                                                   WitRun * const wr,
                                                   const char * const partName,
                                                   const char * const demandName,
                                                   const int firstShipPeriod,
                                                   const int lastShipPeriod,
                                                   LgFrSortingPartScheduleFloat & os
                                                   )
{
  witReturnCode retVal = WitINFORMATIONAL_MESSAGES;

  int nPeriods;
  witGetNPeriods(wr,&nPeriods);

  // Create dictionary which will contain operationName and period as key
  // and execVol as value
  PartNamePeriodConsVolDictionary execDict;

  retVal = getDemandExecVolPIP (
   wr,partName,demandName,firstShipPeriod,lastShipPeriod,
   execDict );

  for( PartNamePeriodConsVolDictionary::iterator it = execDict.begin();
        it != execDict.end();
        it++ )
  {
    //LgFrModelClient* mc = *it;
    SceConstCharStar_Int key = (*it).first;
    SceCollectableDouble v = (*it).second;
    // don't add zero consVols to returned list
    // TOM_WAS_HERE_
    if ( (v.dbl() > -0.0001) && (v.dbl() < 0.0001) ) 
      continue;

    LgFrPart o(key.getCharStar());
    LgFrTimeVecFloat tv(nPeriods);
    int t=key.getInt();
    // Is part already in schedule
    if (os.findValue(o,tv)) {
      // part is in schedule, update timeVec
      tv[t] = tv[t] + (float)v.dbl();
    }
    else {
      // part is not in schedule, so insert it
      tv[t] = (float)v.dbl();
    }
    os.insertKeyAndValue(o,tv);
  }


  
  return retVal;
}



// =============================
//---------------------------------------------------------------------------
// Populate HashDictionary with Exec Volume
//---------------------------------------------------------------------------                                    
witReturnCode ScePIP::getDemandInterplantVolPIP (
                                                   WitRun * const wr,
						   LgFrMultiPlantHelper * mpHelper,
                                                   const char * const partName,
                                                   const char * const demandName,
                                                   const int firstShipPeriod,
                                                   const int lastShipPeriod,
                                                   PartNamePeriodConsVolDictionary & execDict,
						   bool supplierPerspectiveInterplantPeg
                                                   )
{
  witReturnCode retVal = WitINFORMATIONAL_MESSAGES;
  
  // Get shipVol of input demand
  float * shipVol;
  witGetDemandShipVol(wr,partName,demandName,&shipVol);

  float supplierExecVol = 0.0;
  
  // Loop once for each shipPeriod
  int sp;
  for ( sp=firstShipPeriod; sp<=lastShipPeriod; sp++ ) {
    
    // If shipVol is zero, then there is nothing to PIP
    if (shipVol[sp]==0) continue;
    
    // Call wit to get PIP execVol
    int lenOpList,op;
    char ** opNameList;
    int * execPeriodList;
    float * PIPInterplantVolList;
    retVal = witGetDemandExecVolPip(wr,partName,demandName,sp,
      &lenOpList,&opNameList,&execPeriodList,&PIPInterplantVolList);
    
    // Size consDict hashtable if first loop iteration
    // can't resize std::map
    // if ( sp==firstShipPeriod )execDict.resize(lenOpList*20*(lastShipPeriod-firstShipPeriod));
    
    // Loop once per operation with an execVol
    // Only keep those execVols for operations which are SCE Interplant Operations
    for ( op=0; op<lenOpList; op++ ){     
      std::string destination;
      std::string destinationPdf;
      std::string source;
      std::string sourcePdf;


#ifdef DEBUG_INTERPLANT_PEG
      std::cout << "fitering operations for special Interplant: " << opNameList[op];
#endif
      
      if (mpHelper->isOperationSpecialInterplant(wr, opNameList[op], destination, destinationPdf, source, sourcePdf)) {
#ifdef DEBUG_INTERPLANT_PEG
	std::cout << "YES" << std::endl;
#endif
	// modify the data so that it gives customer or supplier perspective
	if (supplierPerspectiveInterplantPeg) {
	  // get the pegging with respect to supplier part
	  int nPeriods;
	  witGetNPeriods(wr,&nPeriods);
	  
	  supplierExecVol = 0.0;
	  int nBom;
	  witGetOperationNBomEntries(wr, opNameList[op], &nBom);
	  assert(nBom == 1);
	  
	  // get impact periods
	  int * impactPeriod;
	  witGetBomEntryImpactPeriod ( wr, opNameList[op], 0, &impactPeriod );
	  
	  // get consumption rate
	  float * consRate;
	  witGetBomEntryConsRate ( wr, opNameList[op], 0, &consRate );
	  
	  // get consumption rate
	  float falloutRate;
	  witGetBomEntryFalloutRate ( wr, opNameList[op], 0, &falloutRate );
	  
	  int ip = impactPeriod[execPeriodList[op]];
	  assert (ip > -1);
	  supplierExecVol += PIPInterplantVolList[op] 
	    * consRate[execPeriodList[op]] 
	    * (1.0 - falloutRate);
	    
	  witFree(consRate);
	  witFree(impactPeriod);
	  PIPInterplantVolList[op] = supplierExecVol;	  
	  execPeriodList[op] = ip;
	}   // end if supplierPerspective ==1

	// update the peg schedule regardless of supplierPerspective
	updateConsDict(execDict,opNameList[op],execPeriodList[op],PIPInterplantVolList[op]);      

      }   // end if (isOperationSpecialInterplant)

      // otherwise, the operation is not Special Interplant and we  ignore
      else {   
#ifdef DEBUG_INTERPLANT_PEG
	std::cout << "NO" << std::endl;
#endif
	witFree(opNameList[op]);
      }

    }   // end of the for (op=0; op<lenOpList; op+)
    

  
    // Free storage allocated by witGetDemandExecVolPIP
    //for ( op=0; op<lenOpList; op++ )witFree(opNameList[op]);  //Don't free because char* are referenced by schedule
    witFree(opNameList);
    witFree(execPeriodList);
    witFree(PIPInterplantVolList);
  }  
  
  witFree(shipVol);
  return retVal;
}


witReturnCode ScePIP::getDemandInterplantVolPIP ( WitRun * const wr,
						  LgFrMultiPlantHelper * mpHelper,
						  const char * const partName,
						  const char * const demandName,
						  const int firstShipPeriod,
						  const int lastShipPeriod,
						  LgFrSortingPartScheduleFloat & os,
						  bool supplierPerspectiveInterplantPeg
						  )
{
  witReturnCode retVal = WitINFORMATIONAL_MESSAGES;

  int nPeriods;
  witGetNPeriods(wr,&nPeriods);

  // Create dictionary which will contain operationName and period as key
  // and execVol as value
  PartNamePeriodConsVolDictionary execDict;

  retVal = getDemandInterplantVolPIP (wr, mpHelper,
				      partName,demandName,
				      firstShipPeriod,lastShipPeriod,
				      execDict,
				      supplierPerspectiveInterplantPeg);

  for( PartNamePeriodConsVolDictionary::iterator it = execDict.begin();
        it != execDict.end();
        it++ )
  {
    //LgFrModelClient* mc = *it;
    SceConstCharStar_Int key = (*it).first;
    SceCollectableDouble v = (*it).second;
    // don't add zero consVols to returned list
    // TOM_WAS_HERE_
    if ( (v.dbl() > -0.0001) && (v.dbl() < 0.0001) ) 
      continue;

    LgFrPart o(key.getCharStar());
    LgFrTimeVecFloat tv(nPeriods);
    int t=key.getInt();
    // Is part already in schedule
    if (os.findValue(o,tv)) {
      // part is in schedule, update timeVec
      tv[t] = tv[t] + (float)v.dbl();
    }
    else {
      // part is not in schedule, so insert it
      tv[t] = (float)v.dbl();
    }
    os.insertKeyAndValue(o,tv);
  }


  
  return retVal;
}
// =================







// constructor
ScePIP::ScePIP()
{  
  // notthing to do here
}

// copy constructor
ScePIP::ScePIP( const ScePIP& source )
{
  assert(0 == "Do not use this method");
}

// assignment operator
ScePIP&
ScePIP::operator=(const ScePIP& rhs) {
  assert(0 == "Do not use this method");
  return *this;
}


// destructor
ScePIP::~ScePIP()
{
  
  // nothing to do here
}





//---------------------------------------------------------------------------
// Testing
//---------------------------------------------------------------------------
#ifdef NDEBUG
#undef NDEBUG
#endif

void
ScePIP::test()
{
#if 0
  std::string filenames[46];  
  const int nFiles=46;   
  filenames[1]="/u/wit/wit-data/examples/wutltest/wit.data";
  filenames[0]="/u/wit/wit-data/anet/anet/wit.data";
  //  filenames[x]="/u/wit/wit-data/anet/anet/wit34.data",     //WIT READ DATA FAILS
  filenames[2]="/u/wit/wit-data/brenda/vern929/wit.data";
  filenames[3]="/u/wit/wit-data/brenda/denise/wit.data";
  filenames[4]="/u/wit/wit-data/brenda/cmos26/wit.data";
  filenames[5]="/u/wit/wit-data/brenda/benson/wit.data";
  filenames[6]="/u/wit/wit-data/brenda/rch818/wit.data";
  filenames[7]="/u/wit/wit-data/brenda/rochrun1/wit.data";
  filenames[8]="/u/wit/wit-data/brenda/karen/wit.data";
  filenames[9]="/u/wit/wit-data/brenda/rch0110/wit.data";
  filenames[10]="/u/wit/wit-data/brenda/test103/wit.data";
  filenames[11]="/u/wit/wit-data/brenda/samp2/wit.data";
  filenames[12]="/u/wit/wit-data/brenda/samp2b/wit.data";
  filenames[13]="/u/wit/wit-data/brenda/bcase10b/wit.data";
  filenames[14]="/u/wit/wit-data/brenda/case10b/wit.data";
  filenames[15]="/u/wit/wit-data/csi/case10/wit.data";
  filenames[16]="/u/wit/wit-data/csi/case11/wit.data";
  filenames[17]="/u/wit/wit-data/csi/case09/wit.data";
  filenames[18]="/u/wit/wit-data/examples/diner/wit.data";
  std::string & diner = filenames[18];
  filenames[19]="/u/wit/wit-data/examples/neglead/wit.data";
  filenames[20]="/u/wit/wit-data/examples/diner12/wit.data";
  filenames[21]="/u/wit/wit-data/examples/chal1/wit.data";
  filenames[22]="/u/wit/wit-data/examples/difWit/wit.data";
  filenames[23]="/u/wit/wit-data/examples/bldAhead/wit.data";
  filenames[24]="/u/wit/wit-data/examples/subs/wit.data";
  filenames[25]="/u/wit/wit-data/examples/twoprod/wit.data";
  filenames[26]="/u/wit/wit-data/examples/chal2/wit.data";
  filenames[27]="/u/wit/wit-data/examples/dillen/wit.data";
  filenames[28]="/u/wit/wit-data/examples/ex1/wit.data";
  // filenames[x]="/u/wit/wit-data/examples/ex1/supply.data",    //WIT READ DATA FAILS
  filenames[29]="/u/wit/wit-data/examples/wutltest/wit2.data";
  filenames[30]="/u/wit/wit-data/examples/diner12a/wit.data";
  filenames[31]="/u/wit/wit-data/holder/lh/wit.data";
  filenames[32]="/u/wit/wit-data/holder/lht/wit.data";
  filenames[33]="/u/wit/wit-data/standard/obj1/wit.data";
  filenames[34]="/u/wit/wit-data/standard/obj1.bounds/wit.data";
  filenames[35]="/u/wit/wit-data/standard/obj2/wit.data";
  filenames[36]="/u/wit/wit-data/standard/obj2.nobounds/wit.data";
  filenames[37]="/u/wit/wit-data/wally/simple2/wit.data";
  filenames[38]="/u/wit/wit-data/wally/simple/wit.data";
  filenames[39]="/u/wit/wit-data/wally/obj1s2d1/wit.data";
  filenames[40]="/u/wit/wit-data/wally/obj1twodemands/wit.data";
  filenames[41]="/u/wit/wit-data/wally/deniseLps/wit.data";
  filenames[42]="/u/wit/wit-data/operations/case2/wit.data";
  filenames[43]="/u/wit/wit-data/operations/case3/wit.data";
  filenames[44]="/u/wit/wit-data/operations/case4/wit.data";
  filenames[45]="/u/wit/wit-data/xwit/wit.data";

  int f;
#if defined(_MSC_VER)
  for ( f=0; f<nFiles; ++f ) {
    filenames[f].replace(0,6,"../../../..");
  }
#endif
  
  // loop once for each wit.data file
  WitRun * wr;
  int nPeriods;
  for (f=0; f<2 /*nFiles*/; ++f )  {
    
    fprintf(stderr, "  %d: using file: %s \n",f,filenames[f].c_str());

    // Do two loops, one for sceConsVol and the other wit consVol
    int consVolType;
    for ( consVolType=0; consVolType<2; consVolType++ ) {
       bool sceConsVol=false;
       if (consVolType==1) sceConsVol = true;
      
      witNewRun(&wr);
      witSetMesgTimesPrint(wr, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
      witSetMesgTimesPrint(wr, WitTRUE, 338, 0);
      witSetMesgTimesPrint(wr, WitTRUE, 538, 0);
      witInitialize(wr);
      witSetPerfPIP(wr,true);
      witReadData(wr, filenames[f].c_str());
      
      witGetNPeriods(wr,&nPeriods);
      
      witHeurImplode(wr);
    
      // Create a dictionary containing wit computed consVols.
      // The key is the part name and consumption period, the value is the consVols
      char  ** partList;
      int      nParts,p;
      witGetParts(wr,&nParts,&partList);

      LgFrSortingPartScheduleFloat consVolScheds[5];
      int fwa=0; // fromWitAttributes computed here from witConsVol 
                 // and optionally shipVol.
      int sa1=1; // from getDemandConsVolPIP with schedule arg & 1 period
      int saa=2; // from getDemandConsVolPIP with schedule arg & all periods
      int sl1=3; // from getDemandConsVolPIP with lists arg & 1 period
      int sla=4; // from getDemandConsVolPIP with lists arg & all periods

      double maxConsVol=0;
      {
        for ( p=0; p<nParts; p++ ){
          // Get ConsVol of part
          float * consVol;
          witGetPartConsVol(wr,partList[p],&consVol);
          
          LgFrPart pn(partList[p]);
          LgFrTimeVecFloat tv(nPeriods,consVol);
          if ( tv!=0.0 ) {         
            if (tv.max()>maxConsVol) maxConsVol=tv.max();
            LgFrItemTimeVecPairFloat itv(pn,tv);
            consVolScheds[fwa].insert(itv);
          }
          
          witFree(consVol);
          
          // If this is an sceConsVol then must add shipVol to dictionary
          if ( sceConsVol ) {
            char ** demList;
            int nDems,d;
            witGetPartDemands(wr,partList[p],&nDems,&demList);
            for ( d=0; d<nDems; d++ ) {
              
              float * shipVol;
              witGetDemandShipVol(wr,partList[p],demList[d],&shipVol);
              
              
              {
                LgFrPart pn(partList[p]);
                LgFrTimeVecFloat shipVolTv(nPeriods,shipVol);
                if ( shipVolTv!=0.0 ) {
                  
                  LgFrTimeVecFloat cv;
                  // Is part already in schedule
                  if (consVolScheds[fwa].findValue(pn,cv)) {
                    // part is in schedule, update timeVec
                    cv = cv + shipVolTv;
                  }
                  else {
                    cv = shipVolTv;
                  }
                  consVolScheds[fwa].insertKeyAndValue(pn,cv);
                }
              }
              witFree(shipVol);
              witFree(demList[d]);
            }
            witFree(demList);
          }
        }
      }
      double consVolEps = fabs(maxConsVol) * 0.000001;
      
      // Loop over all demands
      for ( p=0; p<nParts; p++ ){
          
        if( p == 225 )
            p = 225;
        char ** demList;
        int nDems,d;
        witGetPartDemands(wr,partList[p],&nDems,&demList);
        for ( d=0; d<nDems; d++ ) {
          
          float * shipVol;
          witGetDemandShipVol(wr,partList[p],demList[d],&shipVol);
          
          // loop once for each period
          int t;
          for ( t=0; t<nPeriods; t++ ) {
            
            if (shipVol[t] == 0.0 ) continue;
            
            char ** consPartList;
            float * consVolList;
            int * consPerList;
            int nConsParts;
            
            ScePIP::getDemandConsVolPIP(wr,partList[p],demList[d],t,t,
              &nConsParts,&consPartList,&consPerList,&consVolList,
              sceConsVol);

            {
              //LgFrSortingPartScheduleFloat temp;
              ScePIP::getDemandConsVolPIP(wr,partList[p],demList[d],t,t,
                consVolScheds[sa1], //temp,
                sceConsVol);
              //consVolScheds[sa1] = consVolScheds[sa1] + temp;
            }
            
            int cp;
            for( cp=0; cp<nConsParts; cp++ ) { 
              LgFrPart p(consPartList[cp]);
              LgFrTimeVecFloat tv(nPeriods);
              int t=consPerList[cp];
              consVolScheds[sl1].findValue(p,tv);
              tv[t] = tv[t] + consVolList[cp];
              consVolScheds[sl1].insertKeyAndValue(p,tv);
              free(consPartList[cp]);
            }
            free(consPartList);
            free(consPerList);
            free(consVolList);
          }
          
          witFree(shipVol);
          
          //-----------------
          // Do consVol pegging over all periods
          { 
            char ** consPartList;
            float * consVolList;
            int * consPerList;
            int nConsParts;
            
            ScePIP::getDemandConsVolPIP(wr,partList[p],demList[d],0,nPeriods-1,
              &nConsParts,&consPartList,&consPerList,&consVolList,
              sceConsVol);
            
            {
              //LgFrSortingPartScheduleFloat temp;
              ScePIP::getDemandConsVolPIP(wr,partList[p],demList[d],0,nPeriods-1,
                consVolScheds[saa], //temp,
                sceConsVol);
              //consVolScheds[saa] = consVolScheds[saa] + temp;
            }
            
            int cp;
            for( cp=0; cp<nConsParts; cp++ ) {   
              LgFrPart p(consPartList[cp]);
              LgFrTimeVecFloat tv(nPeriods);
              int t=consPerList[cp];
              consVolScheds[sla].findValue(p,tv);
              tv[t] = tv[t] + consVolList[cp];
              consVolScheds[sla].insertKeyAndValue(p,tv);
              
              free(consPartList[cp]);
            }
            free(consPartList);
            free(consPerList);
            free(consVolList);
          }
          //-----------------
          witFree(demList[d]);
        }
        witFree(demList);
      }      
    
      /* Test that all consVol schedules are equal */
      int s;
      for ( s=1; s<5; s++ ) {
        const LgFrSortingPartScheduleFloat temp = consVolScheds[0]-consVolScheds[s];
        //std::cout 
        //  <<"============================" << std::endl
        //  <<"S=" <<s << std::endl
        //  <<temp.format() << std::endl;
        size_t e;
        for ( e=0; e<temp.size() ; e++ ) {
          const LgFrItemTimeVecPairFloat & itv = temp[e];
          float tvSum = itv.timeVecFloat().sum();
          assert( fabs(tvSum) <= consVolEps );
        }
      }
      
      for ( p=0; p<nParts; p++ ){
        witFree(partList[p]);
      }
      witFree(partList);
    } // for each consVolType
  } // for each file

  // test getDemandConsVolPIP without considering substitutes
  {
    
    LgFrSortingPartScheduleFloat ps;
    // Get parts
    char  ** partList;
    int      nParts,p;
    witGetParts(wr,&nParts,&partList);
    
    // loop over all demands and store consVol in appData
    for ( p=0; p<nParts; p++ ){
      char ** demList;
      int nDems,d;
      witGetPartDemands(wr,partList[p],&nDems,&demList);
      for ( d=0; d<nDems; d++ ) {
        
        ScePIP::getDemandConsVolPIP(wr,partList[p],demList[d],0,nPeriods-1,
          ps,true,false);
        
        witFree(demList[d]);
      }
      witFree(demList);
      witFree(partList[p]);
    }
    witFree(partList);
  }


  // Test execVol pegging
  {    
    // Pegged Exec Volumes from method in this class
    LgFrSortingPartScheduleFloat pev;

    // Pegged Exec Volumes from wit execVol attribute
    LgFrSortingPartScheduleFloat wev;

    // Get parts
    char  ** partList;
    int      nParts,p;
    witGetParts(wr,&nParts,&partList);
    
    // loop over all demands and store consVol in appData
    for ( p=0; p<nParts; p++ ){
      char ** demList;
      int nDems,d;
      witGetPartDemands(wr,partList[p],&nDems,&demList);
      for ( d=0; d<nDems; d++ ) {
        
        // Get demands pegged consVol and directly store in appData
        ScePIP::getDemandExecVolPIP(wr,partList[p],demList[d],0,nPeriods-1,pev);
        
        witFree(demList[d]);
      }
      witFree(demList);
      witFree(partList[p]);
    }
    witFree(partList);
    
    // Get operations
    char  ** opList;
    int      nOps,o;
    witGetOperations(wr,&nOps,&opList);

    // loop once for each operation
    double maxExecVol=0;
    for ( o=0; o<nOps; ++o ) {
      float *ev;
      witGetOperationExecVol(wr,opList[o],&ev);
      
      LgFrPart on(opList[o]);
      LgFrTimeVecFloat tv(nPeriods,ev);
      if ( tv!=0.0 ) {         
        if (tv.max()>maxExecVol) maxExecVol=tv.max();
        LgFrItemTimeVecPairFloat itv(on,tv);
        wev.insert(itv);
      }
      witFree(ev);
      witFree(opList[o]);
    }
    witFree(opList);

    // Test to ensure that the 2 execVol schedules are equal
    
    const LgFrSortingPartScheduleFloat temp = pev-wev;
    //std::cout 
    //  <<"============================" << std::endl
    //  <<temp.format() << std::endl;
    size_t e;    
    double execVolEps = fabs(maxExecVol) * 0.000001;
    for ( e=0; e<temp.size() ; e++ ) {
      const LgFrItemTimeVecPairFloat & itv = temp[e];
      float tvSum = itv.timeVecFloat().sum();
      assert( fabs(tvSum) <= execVolEps );
    }
  }

  witDeleteRun(wr);


  //A possible SCE like usage
  {
    class DemandAppData {
    public:
      LgFrSortingPartScheduleFloat ps1;
      LgFrSortingPartScheduleFloat ps2;
      LgFrSortingPartScheduleFloat execVolSched1;
      LgFrSortingPartScheduleFloat execVolSched2;
    };
    
    WitRun * wr;
    
    witNewRun(&wr);
    witSetMesgTimesPrint(wr, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
    witSetMesgTimesPrint(wr, WitTRUE, 338, 0);
    witSetMesgTimesPrint(wr, WitTRUE, 538, 0);
    witInitialize(wr);
    witSetPerfPIP(wr,true);
    witReadData(wr, diner.c_str());
    
    // Get periods
    int nPeriods;
    witGetNPeriods(wr,&nPeriods);
    
    // Get parts
    char  ** partList;
    int      nParts,p;
    witGetParts(wr,&nParts,&partList);
    
    // Do a heuristic implosion
    witSetExecEmptyBom(wr,WitTRUE);
    witHeurImplode(wr);
    
    // loop over all demands and store consVol in appData
    for ( p=0; p<nParts; p++ ){
      char ** demList;
      int nDems,d;
      witGetPartDemands(wr,partList[p],&nDems,&demList);
      for ( d=0; d<nDems; d++ ) {

        // Initialize demand app data
        DemandAppData * dad = new DemandAppData;
        witSetDemandAppData(wr,partList[p],demList[d],dad);
        
        // Get demands pegged consVol and directly store in appData
        ScePIP::getDemandConsVolPIP(wr,partList[p],demList[d],0,nPeriods-1,
          dad->ps1, true);

        // Get demands pegged execVol and directly store in appData
        ScePIP::getDemandExecVolPIP(wr,partList[p],demList[d],0,nPeriods-1,
          dad->execVolSched1);
        
        witFree(demList[d]);
      }
      witFree(demList);
    }

    // Do a different heuristic implosion
    witSetExecEmptyBom(wr,WitFALSE);
    witHeurImplode(wr);
    
    // loop over all demands and store consVol in appData
    for ( p=0; p<nParts; p++ ){
      char ** demList;
      int nDems,d;
      witGetPartDemands(wr,partList[p],&nDems,&demList);
      for ( d=0; d<nDems; d++ ) {

        // Initialize demand app data
        DemandAppData * dad;
        witGetDemandAppData(wr,partList[p],demList[d],(void**)&dad);
        
        // Get demands pegged consVol and directly store in appData
        ScePIP::getDemandConsVolPIP(wr,partList[p],demList[d],0,nPeriods-1,
          dad->ps2, true);

        // Get demands pegged execVol and directly store in appData
        ScePIP::getDemandExecVolPIP(wr,partList[p],demList[d],0,nPeriods-1,
          dad->execVolSched2);
        
        witFree(demList[d]);
      }
      witFree(demList);
    }
    
    // loop over all demands and access consVols and execVol
    for ( p=0; p<nParts; p++ ){
      char ** demList;
      int nDems,d;
      witGetPartDemands(wr,partList[p],&nDems,&demList);
      for ( d=0; d<nDems; d++ ) {

        std::cout 
          <<"--------------------" << std::endl
          <<"Demand: " <<demList[d]  <<" Part: " <<partList[p] << std::endl;

        // Initialize demand app data
        DemandAppData * dad;
        witGetDemandAppData(wr,partList[p],demList[d],(void**)&dad);

        // Get const references to the 2 schedules stored in app data
        const LgFrSortingPartScheduleFloat & ps1Const = dad->ps1;
        const LgFrSortingPartScheduleFloat & ps2Const = dad->ps2;
        const LgFrSortingPartScheduleFloat & ev1Const = dad->execVolSched1;
        const LgFrSortingPartScheduleFloat & ev2Const = dad->execVolSched2;

        // Compute combined consVol
        const LgFrSortingPartScheduleFloat total = ps1Const + ps2Const;

        // Loop over each consumed part
        int ncp = total.size();
        int cp = 0; // Pulled out of the for below by RW2STL
        for ( cp=0; cp<ncp; cp++ ) {
          // get part
          const LgFrPart part = total.part(cp);
          std::cout <<"  Consumed part: " <<part.name() << std::endl;

          // Get this part consVol for 1st and 2nd implosions and total
          const LgFrTimeVecFloat & tv1 = ps1Const[part].timeVecFloat();
          const LgFrTimeVecFloat & tv2 = ps2Const[part].timeVecFloat();
          LgFrTimeVecFloat tvT = total[cp].timeVecFloat();

          // Print consVols of each implosion and total
          int t;
          std::cout <<"    implosion 1: ";
          for ( t=0; t<nPeriods; ++t ) std::cout <<" " <<tv1[t];
          std::cout << std::endl;
          
          std::cout <<"    implosion 2: ";
          for ( t=0; t<nPeriods; ++t ) std::cout <<" " <<tv2[t];
          std::cout << std::endl;

          std::cout <<"    total:       ";
          for ( t=0; t<nPeriods; ++t ) std::cout <<" " <<tvT[t];
          std::cout << std::endl;
        }

        // execVol Schedule
        std::cout 
          <<"ExecVol Sched for demand: "
          <<demList[d] << std::endl <<ev1Const.format() << std::endl;

        delete dad;
        witFree(demList[d]);
      }
      witFree(demList);
    }    
    
    for ( p=0; p<nParts; p++ ){
      witFree(partList[p]);
    }
    witFree(partList);
    witDeleteRun(wr);
    
  }
#endif  
} 

