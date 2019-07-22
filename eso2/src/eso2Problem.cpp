#if defined(_MSC_VER)
#pragma warning(disable:4018)
#pragma warning(disable:4786)
#pragma warning(disable:4503)
#pragma warning(disable:4996)
#endif

#include <numeric>
#include <set>

#include <cassert>
#include <iostream>
#include <cfloat>
#include <cstdio>
#include <climits>

#include <wit.h>

#include "CoinFloatEqual.h"
#include "probAppData.h"
#include "partDemandAppData.h"
#include "mtmAppData.h"
#include "mtmDemanBomAppData.h"
#include "featureXrefAppData.h"
#include "featureAppData.h"
#include "loosePartAppData.h"
#include "eso2Problem.h"
#include "eso2.h"
#include "unitTestResultsData.h"
#include "pipSequenceTriple.h"
//#include "eso2WitPrune.h"
#include "stringStringIntFloatTuple.h"


#define CONSUME_ACQUIRED
#define REDUCE_PREPROCESSING_TIME


//--------------------------------------------
//--------------------------------------------
//class ESO2pipSequenceItem
//--------------------------------------------
//--------------------------------------------
// Return true if subsBomEntry exists
bool ESO2problem::witGetSubsBomEntryExists(
                                           const std::string & operationName,
                                           int bomEntry,
                                           int subEntry) const
{
  int opExists;
  witGetOperationExists(mutableWitRun(),operationName.c_str(),&opExists);
  if ( !opExists ) return false;

  int nBomEntries;
  witGetOperationNBomEntries(mutableWitRun(),operationName.c_str(),&nBomEntries);

  int nSubBomEntries;
  witGetBomEntryNSubsBomEntries(mutableWitRun(),operationName.c_str(),bomEntry,&nSubBomEntries);
  if ( subEntry >= nSubBomEntries ) return false;
  return true;
}
// Return true if demand exist
bool ESO2problem::witGetDemandExists(const std::string & pn, const std::string & dn ) const
{
  bool retVal = false;
  
  witBoolean partExists;
  witGetPartExists( mutableWitRun(), pn.c_str(), &partExists );
  if ( !partExists ) return retVal;   
  
  int nPartDemands;
  char ** demandList;
  witGetPartDemands( mutableWitRun(), pn.c_str(), &nPartDemands, &demandList);
  for ( int d=0; d<nPartDemands; ++d ) {
    if ( demandList[d] == dn ) {
      retVal = true;
    }    
    witFree(demandList[d]);
  }
  witFree(demandList);
  return retVal;
}

void ESO2problem::compareWitSolutionsTimeVec(
                                const std::string & id,
                                const std::vector<float> & execTv1,
                                const std::vector<float> & execTv2, 
                                const std::vector<float> & costTv1,
                                const std::vector<float> & costTv2 ) 
{
  int t;
  assert( execTv1.size()==execTv2.size() );
  for ( t=0; t<execTv1.size(); ++t ) {
    CoinRelFltEq eq;
    if ( !eq(execTv1[t],execTv2[t]) ) {
      std::cout <<id.c_str() <<" period: " <<t <<" vol: " <<execTv1[t] <<" " <<execTv2[t] <<" cost: " <<costTv1[t] <<" " <<costTv2[t] <<std::endl;
    }
  }
}

void ESO2problem::compareWitSolutions( const ESO2problem & p1,  const ESO2problem & p2 ) 
{
  // Iterate over demands in p1
  {  
    witAttr objItrState;
    witGetObjItrState(p1.mutableWitRun(), &objItrState);
    while( true )   {
      witAdvanceObjItr(p1.mutableWitRun());
      witGetObjItrState(p1.mutableWitRun(), &objItrState);
      if (objItrState==WitINACTIVE) break;
      if( objItrState == WitAT_DEMAND ) {
        char * demandedPartName;
        char * demandName;
        witGetObjItrDemand(p1.mutableWitRun(),&demandedPartName,&demandName);
        std::vector<float> shipVol1 = p1.witGetDemandAttribute(witGetDemandShipVol,demandedPartName,demandName);
        std::vector<float> shipRew1 = p1.witGetDemandAttribute(witGetDemandObj1ShipReward,demandedPartName,demandName);
        
        std::vector<float> shipVol2 = p2.floatToStlVec(0.0);
        std::vector<float> shipRew2 = p2.floatToStlVec(-FLT_MAX);
        bool exists = p2.witGetDemandExists(demandedPartName,demandName);
        if ( exists ) {
          shipVol2 = p2.witGetDemandAttribute(witGetDemandShipVol,demandedPartName,demandName);
          shipRew2 = p2.witGetDemandAttribute(witGetDemandObj1ShipReward,demandedPartName,demandName);
        }

        std::string id = std::string(demandName) + "/" + demandedPartName;
        compareWitSolutionsTimeVec(id,shipVol1,shipVol2,shipRew1,shipRew2);        
        
        witFree(demandedPartName);
        witFree(demandName);
      } 
      
    } // end while( true )
  }
  
  // Iterate over demands in p2
  {  
    witAttr objItrState;
    witGetObjItrState(p2.mutableWitRun(), &objItrState);
    while( true )   {
      witAdvanceObjItr(p2.mutableWitRun());
      witGetObjItrState(p2.mutableWitRun(), &objItrState);
      if (objItrState==WitINACTIVE) break;
      if( objItrState == WitAT_DEMAND ) {
        char * demandedPartName;
        char * demandName;
        witGetObjItrDemand(p2.mutableWitRun(),&demandedPartName,&demandName);
        std::vector<float> shipVol2 = p2.witGetDemandAttribute(witGetDemandShipVol,demandedPartName,demandName);
        std::vector<float> shipRew2 = p2.witGetDemandAttribute(witGetDemandObj1ShipReward,demandedPartName,demandName);
        
        std::vector<float> shipVol1 = p1.floatToStlVec(0.0);
        std::vector<float> shipRew1 = p1.floatToStlVec(-FLT_MAX);
        bool exists = p1.witGetDemandExists(demandedPartName,demandName);
        if ( exists ) continue;

        std::string id = std::string(demandName) + "/" + demandedPartName;
        compareWitSolutionsTimeVec(id,shipVol1,shipVol2,shipRew1,shipRew2);        
        
        witFree(demandedPartName);
        witFree(demandName);
      } 
      
    } // end while( true )
  }

  
  // Iterate over operations in p1
  {  
    witAttr objItrState;
    witGetObjItrState(p1.mutableWitRun(), &objItrState);
    while( true )   {
      witAdvanceObjItr(p1.mutableWitRun());
      witGetObjItrState(p1.mutableWitRun(), &objItrState);
      if (objItrState==WitINACTIVE) break;
      if( objItrState == WitAT_OPERATION ) {
        char * operationName;
        witGetObjItrOperation(p1.mutableWitRun(),&operationName);
        std::vector<float> execVol1 = p1.witGetNameAttribute(witGetOperationExecVol,operationName);
        std::vector<float> execCost1 = p1.witGetNameAttribute(witGetOperationObj1ExecCost,operationName);
        
        std::vector<float> execVol2 = p2.floatToStlVec(0.0);
        std::vector<float> execCost2 = p2.floatToStlVec(-FLT_MAX);
        int exists;
        witGetOperationExists(p2.mutableWitRun(),operationName, &exists);
        if ( exists ) {
          execVol2 = p2.witGetNameAttribute(witGetOperationExecVol,operationName);
          execCost2 = p2.witGetNameAttribute(witGetOperationObj1ExecCost,operationName);
        }

        std::string id = std::string("Operation:: ") + operationName;
        compareWitSolutionsTimeVec(id,execVol1,execVol2,execCost1,execCost2);        
        
        witFree(operationName);
      } 
      
    } // end while( true )
  }
  
  // Iterate over operations in p2
  {  
    witAttr objItrState;
    witGetObjItrState(p2.mutableWitRun(), &objItrState);
    while( true )   {
      witAdvanceObjItr(p2.mutableWitRun());
      witGetObjItrState(p2.mutableWitRun(), &objItrState);
      if (objItrState==WitINACTIVE) break;
      if( objItrState == WitAT_OPERATION ) {
        char * operationName;
        witGetObjItrOperation(p2.mutableWitRun(),&operationName);
        std::vector<float> execVol2 = p2.witGetNameAttribute(witGetOperationExecVol,operationName);
        std::vector<float> execCost2 = p2.witGetNameAttribute(witGetOperationObj1ExecCost,operationName);
        
        std::vector<float> execVol1 = p1.floatToStlVec(0.0);
        std::vector<float> execCost1 = p1.floatToStlVec(-FLT_MAX);
        int exists;
        witGetOperationExists(p1.mutableWitRun(),operationName, &exists);
        if ( exists ) continue; 

        std::string id = std::string("Operation:: ") + operationName;
        compareWitSolutionsTimeVec(id,execVol1,execVol2,execCost1,execCost2);        
        
        witFree(operationName);
      } 
      
    } // end while( true )
  }

   
  // Iterate over substitutes in p1
  {  
    witAttr objItrState;
    witGetObjItrState(p1.mutableWitRun(), &objItrState);
    while( true )   {
      witAdvanceObjItr(p1.mutableWitRun());
      witGetObjItrState(p1.mutableWitRun(), &objItrState);
      if (objItrState==WitINACTIVE) break;
      if( objItrState == WitAT_SUB_ENTRY ) {
        char * operationName;
        int bomEntry;
        int subEntry;
        witGetObjItrSubsBomEntry(p1.mutableWitRun(),&operationName, &bomEntry, &subEntry);
        std::vector<float> subVol1 = p1.witGetSubArcAttribute(witGetSubsBomEntrySubVol,operationName,bomEntry,subEntry);
        std::vector<float> subCost1 = p1.witGetSubArcAttribute(witGetSubsBomEntryObj1SubCost,operationName,bomEntry,subEntry);
        
        std::vector<float> subVol2 = p2.floatToStlVec(0.0);
        std::vector<float> subCost2 = p2.floatToStlVec(-FLT_MAX);
        bool exists = p2.witGetSubsBomEntryExists(operationName,bomEntry,subEntry);
        if ( exists ) {
          subVol2 = p2.witGetSubArcAttribute(witGetSubsBomEntrySubVol,operationName,bomEntry,subEntry);
          subCost2 = p2.witGetSubArcAttribute(witGetSubsBomEntryObj1SubCost,operationName,bomEntry,subEntry);
        }


        char bomEntryAsChar[100];
        char subEntryAsChar[100];
        sprintf(bomEntryAsChar,"%d",bomEntry);
        sprintf(subEntryAsChar,"%d",subEntry);        
        std::string id = std::string("Subs:: ") + operationName + " bomEntry: " + bomEntryAsChar + " subEntry: " + subEntryAsChar;
        compareWitSolutionsTimeVec(id,subVol1,subVol2,subCost1,subCost2);        
        
        witFree(operationName);
      } 
      
    } // end while( true )
  }
    
  // Iterate over substitutes in p2
  {  
    witAttr objItrState;
    witGetObjItrState(p2.mutableWitRun(), &objItrState);
    while( true )   {
      witAdvanceObjItr(p2.mutableWitRun());
      witGetObjItrState(p2.mutableWitRun(), &objItrState);
      if (objItrState==WitINACTIVE) break;
      if( objItrState == WitAT_SUB_ENTRY ) {
        char * operationName;
        int bomEntry;
        int subEntry;
        witGetObjItrSubsBomEntry(p2.mutableWitRun(),&operationName, &bomEntry, &subEntry);
        std::vector<float> subVol2 = p2.witGetSubArcAttribute(witGetSubsBomEntrySubVol,operationName,bomEntry,subEntry);
        std::vector<float> subCost2 = p2.witGetSubArcAttribute(witGetSubsBomEntryObj1SubCost,operationName,bomEntry,subEntry);
        
        std::vector<float> subVol1 = p1.floatToStlVec(0.0);
        std::vector<float> subCost1 = p1.floatToStlVec(-FLT_MAX);
        bool exists = p1.witGetSubsBomEntryExists(operationName,bomEntry,subEntry);
        if ( exists ) continue; 

        char bomEntryAsChar[100];
        char subEntryAsChar[100];
        sprintf(bomEntryAsChar,"%d",bomEntry);
        sprintf(subEntryAsChar,"%d",subEntry);        
        std::string id = std::string("Subs:: ") + operationName + "bomEntry: " + bomEntryAsChar + "subEntry: " + subEntryAsChar;
        compareWitSolutionsTimeVec(id,subVol1,subVol2,subCost1,subCost2);        
        
        witFree(operationName);
      } 
      
    } // end while( true )
  }
}
//--------------------------------------------
//--------------------------------------------

void ESO2problem::addSubsForPeriod1SupplyPerishPeriod(
       const std::string & acquireOperation, 
       const std::string & acquiredPart,
       float moveTime)
{  
  int perishLimit = getPeriod1SupplyPerishPeriod();
  assert( perishLimit>=0 );
  int np = getNPeriods();

  witAddSubsBomEntry(witRun(),acquireOperation.c_str(),0,acquiredPart.c_str());
  if ( perishLimit==0 ) {
    // Substitute arc is not needed so turn it off by setting earliest period to be nPeriods
    witSetSubsBomEntryEarliestPeriod(witRun(),acquireOperation.c_str(),0,0,np);
  }
  else {
    // determine earliest period that substitute could be used
    int earliest = (int)(moveTime /*+ 1*/);
    
    // if moveTime longer than time horizon just turn off arc
    if ( earliest>=np ) {
      earliest=np;
    }
    else {
      // Need to set offsets
      float * offset = floatToConstFloatStar(0.0f);
      int t;
      int latest = -1;
      for ( t=earliest; t<=(int)(moveTime+perishLimit); ++t ) {
        if ( t >= np ) break;
        latest = t;
        offset[t] =(float) t;
      }
      assert( latest != -1 );
      witSetSubsBomEntryOffset(witRun(),acquireOperation.c_str(),0,0,offset);
      witSetSubsBomEntryLatestPeriod(witRun(),acquireOperation.c_str(),0,0,latest);
      delete [] offset;
    }
    witSetSubsBomEntryEarliestPeriod(witRun(),acquireOperation.c_str(),0,0,earliest);
  }


#if 0
  if ( perishLimit>np ) perishLimit = np;

  float * offset = floatToConstFloatStar(0.0f);
  int l;
  for ( l=0; l<perishLimit; ++l ) {
    offset[l] = l;
  }

  witAddSubsBomEntry(witRun(),acquireOperation.c_str(),0,acquiredPart.c_str());
  witSetSubsBomEntryOffset(witRun(),acquireOperation.c_str(),0,0,offset);
  witSetSubsBomEntryLatestPeriod(witRun(),acquireOperation.c_str(),0,0,perishLimit);

  // The model is not wrong if this is not done, but it is slightly more efficient.
  if ( np>0 )
    witSetSubsBomEntryEarliestPeriod(witRun(),acquireOperation.c_str(),0,0,1);

  delete [] offset;
#endif
}

std::vector<int> ESO2problem::multiLevelImpactPeriod( 
       const std::vector<StdVecInt> & impPersVecs )
{
  std::vector<int> retVal;
  
  int p;
  int l; 

  size_t nLevels = impPersVecs.size();
  assert( nLevels > 0 );

  size_t np = impPersVecs[0].size();
  
  // initialize retVal
  for (p=0; p<np; ++p) retVal.push_back(p);

  // loop once for each level
  for (l=0; l<nLevels; ++l ) {

    // loop once for each time period
    for (p=0; p<np; ++p) {

      int x=retVal[p];
      if ( retVal[p] == -1 ) continue;

      int y=impPersVecs[l][ retVal[p] ];
      retVal[p] = impPersVecs[l][ retVal[p] ];

    } // end for p
  } // end for l


#if 0
  for (l=0; l<nLevels; ++l ) {
    for (p=0; p<np; ++p) {
      std::cout <<impPersVecs[l][p] <<" ";
    }
    std::cout <<std::endl;
  }
  for (p=0; p<np; ++p) {
      std::cout <<retVal[p] <<" ";
  }
  std::cout <<std::endl <<std::endl;
#endif

  return retVal;
}

// Set prodRates from mtm attach rates and featureXref ApplProdRates
void ESO2problem::setDemanBomAndFeatureXrefProdRates()
{
  if ( getDemanBomProdRatesAreSet() ) return;
  int np=getNPeriods();

  setDemanBomAndFeatureXrefProdRatesAreSet(true);

  // Make copy of temporary eso2Problem to be used with witGet functions.
  // This is to avoid causing wit to have to do preprocess in a loop and
  // vastly improves performance.
  const ESO2problem * getProb = new ESO2problem(*this);
  //const ESO2problem * getProb = this;

  {
  
  std::cout <<"Setting attachRates using period of mtm supply" <<std::endl;
    std::vector<std::string> mtms;
    std::vector<std::string> srcLocs;
    std::vector<std::string> destLocs;
    getMtmDemanLocs(mtms, srcLocs, destLocs );
    int m;
    for (m=0; m<mtms.size(); ++m ) {

      if ( (m % 10)==0 ) 
        std::cout <<"  Setting attachRate for mtm-srcLoc-destLoc triple number:" 
        <<m 
        <<" out of "
        <<(unsigned int)mtms.size()
        <<std::endl;
      
      
      std::string baseDemanOpName = demanMtmName(mtms[m],srcLocs[m],srcLocs[m] );
      int nBops;
      witGetOperationNBopEntries(mutableWitRun(),baseDemanOpName.c_str(),&nBops);
      // loop once for each bop
      int b;
      for (b=0; b<nBops; ++b ) {
        ESO2mtmDemanBomAppData * bopAppData; 
        witGetBopEntryAppData(getProb->mutableWitRun(),baseDemanOpName.c_str(),b,(void**)&bopAppData);
        std::vector<float> attachRate = bopAppData->getAttachRate();

        // must map attachRates to prodRates via impactPeriods.
        // AttachRate[t] is for supply from period t
        // WIT needs prodRate[t] were t is the period of deman operation

        // Get names of wit objects
        std::string demanOpName = demanMtmName(mtms[m],srcLocs[m],destLocs[m] );
        std::string moveAndAcquireOpName = acquireMtmName(mtms[m],srcLocs[m],destLocs[m] );
        
        // Get all impact periods that are in affect from deman operation
        // to source of supply
        std::vector<StdVecInt> impPersVecs(3);
        impPersVecs[0]=( getProb->witGetArcAttribute(witGetBomEntryImpactPeriod,demanOpName,0) );
        impPersVecs[1]=( getProb->witGetArcAttribute(witGetBopEntryImpactPeriod,moveAndAcquireOpName,0) );
        impPersVecs[2]=( getProb->witGetArcAttribute(witGetBomEntryImpactPeriod,moveAndAcquireOpName,0) );

        std::vector<int> impPers = multiLevelImpactPeriod(impPersVecs);
		//for ( int t=0; t<impPers.size(); ++t ) 
		//	std::cout <<"impPers[" <<t <<"]=" <<impPers[t] <<std::endl;




        // map attach rate to prodRate
        std::vector<float> prodRate(np);
        int p;
        for ( p=0; p<np; ++p ) {
          prodRate[p] = 0.0f;
          if ( impPers[p] != -1 ) {
            
            if ( useMtmPerishableSupply(mtms[m],srcLocs[m],destLocs[m],p ) ){
              prodRate[p] = attachRate[0];
            }
            else {
              prodRate[p] = attachRate[impPers[p]];
            }
          }
          //std::cout <<p <<" " <<impPers[p] <<" " <<attachRate[p] <<" " <<prodRate[p] <<std::endl;
        }

        witSetArcAttribute(witSetBopEntryProductRate,demanOpName,b,prodRate); 

      } // end of b loop
      
    } // end of m loop
    
    std::cout <<"Finished setting attachRates using period of mtm supply" <<std::endl;
  }

#ifdef REDUCE_PREPROCESSING_TIME
  //----------------------------------------------
  {
  
    std::cout <<"Setting featureXref prodRates using period of feature supply" <<std::endl;
    std::vector<std::string> parts;
    std::vector<std::string> features;
    std::vector<std::string> locs;
    getFeatureXrefs( parts, features, locs );
 
    int x;
    for (x=0; x<parts.size(); ++x ) {

      if ( (x % 10)==0 ) 
        std::cout <<"  Setting prodRate for part-feature-loc triple number: " 
        <<x 
        <<" out of "
        <<(unsigned int)parts.size()
        <<std::endl;
  
     // must map featureXref applProdRate to  wit prodRate via impactPeriods.
     // The applProdRate's period is the period of supply.
     // Wit's prodRate is the period of the crossReference operation.
     // Wit's prodRate must be computed from applProdRate and impact periods.
  
     // Get names of wit objects
     std::string xrefOpName = featureXrefName(parts[x],features[x],locs[x] );
     std::string acquireFeatureOpName = acquireFeatureName(features[x],locs[x] );
  
     // Get all impact periods that are in affect from xref operation
     // to source of supply
     std::vector<StdVecInt> impPersVecs(3);
     
     impPersVecs[0]=( getProb->witGetArcAttribute(witGetBomEntryImpactPeriod,xrefOpName,0) );
     impPersVecs[1]=( getProb->witGetArcAttribute(witGetBopEntryImpactPeriod,acquireFeatureOpName,0) );
     impPersVecs[2]=( getProb->witGetArcAttribute(witGetBomEntryImpactPeriod,acquireFeatureOpName,0) );
  
     std::vector<int> impPers = multiLevelImpactPeriod(impPersVecs);
  
     // Get current application prodRates
     std::vector<float> applProdRate = getProb->getFeatureXrefProdRate(parts[x],features[x],locs[x]);
  
     
        
     // map applProdRate to witProdRate.
     // There are two cases to consider, since period zero supply may not perish
        std::vector<float> prodRate(np);
        int p;
        for ( p=0; p<np; ++p ) {
          prodRate[p] = 0.0f;
          if ( impPers[p] != -1 ) {
            
            if ( useFeaturePerishableSupply(features[x],locs[x],p )  ){
              prodRate[p] = applProdRate[0];
            }
            else {
              prodRate[p] = applProdRate[impPers[p]];
            }
          }
        }
        
        //for ( p=0; p<np; ++p )
        //  std::cout <<p <<" " <<impPers[p] <<" " <<applProdRate[p] <<" " <<prodRate[p] <<std::endl;


     witSetArcAttribute(witSetBopEntryProductRate,xrefOpName,0,prodRate); 
    } // end of x loop
    
    std::cout <<"Finished featureXref prodRates using period of feature supply" <<std::endl;
  }
#endif
  //--------------------------------------------------
  delete getProb;
    
    
}

//----------------------
// Return true if perishable supply will be consumed
//----------------------
bool ESO2problem::useLoosePartPerishableSupply(
                         const std::string & part,
                         const std::string & partLoc,
                         int period ) 
{
  // get name of acquire op and acquired part
  std::string acquireOpName = acquireLoosePartName(part,partLoc);  
  std::string acquiredPartName = igfLoosePartName(part,partLoc);
  return usePerishableSupply(acquireOpName,acquiredPartName,period);
}
bool ESO2problem::useFeaturePerishableSupply(
                         const std::string & feature,
                         const std::string & featureLoc,
                         int period ) 
{
  // get name of acquire op and acquired part
  std::string acquireOpName = acquireFeatureName(feature,featureLoc);  
  std::string acquiredPartName = igfFeatureName(feature,featureLoc);
  return usePerishableSupply(acquireOpName,acquiredPartName,period);
}
bool ESO2problem::useMtmPerishableSupply(
                         const std::string & mtm,
                         const std::string & srcLoc,
                         const std::string & destLoc,
                         int period
                         )  
{
  // get name of acquire op and acquired part
  std::string moveAndAcquireOpName = acquireMtmName(mtm,srcLoc,destLoc);  
  std::string acquiredPartName = igfMtmName(mtm,srcLoc);
  return usePerishableSupply(moveAndAcquireOpName,acquiredPartName,period);
}

bool ESO2problem::usePerishableSupply(
                         const std::string & witAcquireOpName,
                         const std::string & witAcquiredPartName,
                         int period
                         )  
{
  // Get periods that substitute arc is active
  int subEarliestPer = witGetSubArcAttribute(witGetSubsBomEntryEarliestPeriod,witAcquireOpName,0,0);
  int subLatestPer = witGetSubArcAttribute(witGetSubsBomEntryLatestPeriod,witAcquireOpName,0,0);

  // get supply
  std::vector<float> supply = witGetNameAttribute(witGetPartSupplyVol,witAcquiredPartName);
  
  // Test Kevin's claims about perishable supply
  // Only need to test once per wit operation
  if ( perishableSupplyWarningMsgTested_.find(witAcquireOpName)==perishableSupplyWarningMsgTested_.end() ) {
    perishableSupplyWarningMsgTested_.insert(witAcquireOpName);
    bool ok=true;
    int badPer;
    int p;
    if ( supply[0]!= 0.0f ) {
      for ( p=1; p<supply.size(); ++p ) {
        if ( supply[p] != 0.0f ) {
          ok=false;
          badPer=p;
          break;
        }
      }
    }
    if ( !ok ) {
      // only issue warning if hasn't already been done for this operation
      std::cout 
        <<"-------------------------------------------------------"            <<std::endl
        <<"Warning: supply in first period and another period is non-zero."<<std::endl
        <<"   name: " <<witAcquireOpName.c_str()                                    <<std::endl
        <<"   supply in period 1 is " <<supply[0]    <<std::endl
        <<"   supply in period "<<badPer+1 <<" is " <<supply[badPer]    <<std::endl
        <<"-------------------------------------------------------"            <<std::endl;
      
    }
  }
  
  // determine if deman in period is consuming supply from period 0
  // via substitute bom
  bool useSubs = true;
  if ( subEarliestPer == getNPeriods() ) useSubs=false;
  if ( period<subEarliestPer ) useSubs=false;
  if ( period>subLatestPer ) useSubs=false;
  if ( supply[0]==0.0f ) useSubs=false;
  return useSubs;
}
//-------------------------------------------

void ESO2problem::addNonScrapableCapacity(const std::string & name )
{
  witAddPart(witRun(), name.c_str(), WitCAPACITY);
#ifdef CONSUME_ACQUIRED
  witSetNameAttribute(witSetPartObj1ScrapCost,name.c_str(),bigM());
#endif
}
//-------------------------------------------
// 
//----------------------
// Mtm Methods
//----------------------
void ESO2problem::addMtm(const std::string & mtmName, const std::string & mtmLoc,
                         float demanCostS, float demanTimeS)
{
  std::string igfMtmWitName = igfMtmName(mtmName, mtmLoc );
  std::string acquireMtmWitName = acquireMtmName(mtmName, mtmLoc, mtmLoc );
  std::string serverMtmWitName = serverMtmName(mtmName, mtmLoc, mtmLoc );
  std::string demanMtmWitName = demanMtmName(mtmName, mtmLoc, mtmLoc );

  int np = getNPeriods();

  // Add igf owned mtm, acquire operation, and connect them
  witAddPart(witRun(), igfMtmWitName.c_str(), WitCAPACITY);
  witAddOperation(witRun(), acquireMtmWitName.c_str() );
  witAddBomEntry(witRun(), acquireMtmWitName.c_str(), igfMtmWitName.c_str() );

  // Add substitute boms to allow period 0 supply to be consumed in latter periods.
  addSubsForPeriod1SupplyPerishPeriod(acquireMtmWitName,igfMtmWitName,0.0f);

  // Add server owned mtm, and connect to acquire operation
  addNonScrapableCapacity(serverMtmWitName.c_str());
  witAddBopEntry(witRun(), acquireMtmWitName.c_str(), serverMtmWitName.c_str() );

  // Add demanufacturing operation and connect to server owned mtm
  witAddOperation(witRun(), demanMtmWitName.c_str() );
  witAddBomEntry(witRun(), demanMtmWitName.c_str(), serverMtmWitName.c_str() );

  // Set default demanCost
  float * demanCost = floatToConstFloatStar( (float) demanCostS );
  witSetOperationObj1ExecCost(witRun(), demanMtmWitName.c_str(), demanCost );
  delete [] demanCost;

  // Set default demanTime
  float * demanTime = floatToConstFloatStar( (float) demanTimeS );
  witSetBomEntryOffset(witRun(), demanMtmWitName.c_str(), 0, demanTime );
  delete [] demanTime;

  // Set default app data to hold wfmv
  ESO2mtmAppData * appData = new ESO2mtmAppData(np);
  std::vector<float> moveCost = floatToStlVec(0.0f);
  appData->setMoveCost(moveCost);
  witSetOperationAppData (mutableWitRun(), acquireMtmWitName.c_str(), appData);
  
  std::string baseName = baseMtmName(mtmName, mtmLoc );
  mtmNames_.insert(baseName);
  //addMtmAcquireLoc(baseName,mtmLoc);
}

bool ESO2problem::mtmExists(const std::string & mtmName, const std::string & mtmLoc )const
{
  std::string igfMtmWitName = igfMtmName(mtmName, mtmLoc );
  witBoolean partExists;
  witGetPartExists(mutableWitRun(),igfMtmWitName.c_str(),&partExists);
  bool retVal = false;
  if( partExists) retVal = true;
  return retVal;
}


void ESO2problem::setMtmSupply(
    const std::string & mtmName, 
    const std::string & mtmLoc, 
    int period, 
    float supply )
{ 
  std::string name = igfMtmName(mtmName, mtmLoc );
  witSetNameAttribute(&witGetPartSupplyVol,witSetPartSupplyVol,name,period,supply);
}
void ESO2problem::setMtmWfmv(
    const std::string & mtmName, 
    const std::string & mtmLoc, 
    int period, 
    float wholeSaleFairMarketValue )
{
  std::string name = acquireMtmName(mtmName, mtmLoc, mtmLoc );
  ESO2mtmAppData * appData;
  witGetOperationAppData(mutableWitRun(), name.c_str(), (void**)&appData);
  appData->setWfmv(period,wholeSaleFairMarketValue);
}
std::vector<float> ESO2problem::getMtmWfmv(
    const std::string & mtmName, 
    const std::string & mtmLoc ) const
{
  std::string name = acquireMtmName(mtmName, mtmLoc, mtmLoc );
  ESO2mtmAppData * appData;
  witGetOperationAppData(mutableWitRun(), name.c_str(), (void**)&appData);
  return appData->getWfmv();
}
void ESO2problem::setMtmDemanCost(
                     const std::string & mtmName, 
                     const std::string & supplyLoc, 
                     const std::string & demanLoc, 
                     int period, 
                     float cost )
{ 
  std::string name = demanMtmName(mtmName, supplyLoc, demanLoc );
  witSetNameAttribute(witGetOperationObj1ExecCost,witSetOperationObj1ExecCost,name,period,cost);
}
void ESO2problem::setMtmDemanCost(
    const std::string & mtmName, 
    const std::string & mtmLoc, 
    int period, 
    float demanCost )
{ 
  std::string name = demanMtmName(mtmName, mtmLoc, mtmLoc );
  witSetNameAttribute(witGetOperationObj1ExecCost,witSetOperationObj1ExecCost,name,period,demanCost);
}
void ESO2problem::setMtmDemanCost(
    const std::string & mtmName, 
    const std::string & supplyLoc, 
    const std::string & demanLoc,
    const std::vector<float> & demanCost )
{ 
  std::string name = demanMtmName(mtmName, supplyLoc, demanLoc );
  witSetNameAttribute(witSetOperationObj1ExecCost,name,demanCost);
}
std::vector<float> ESO2problem::getMtmDemanCost(
    const std::string & mtmName, 
    const std::string & mtmLoc ) const
{
  std::string name = demanMtmName(mtmName, mtmLoc, mtmLoc );
  return witGetNameAttribute(witGetOperationObj1ExecCost,name);
}
void ESO2problem::setMtmDemanTime(
    const std::string & mtmName, 
    const std::string & mtmLoc, 
    int period, 
    float time )
{ 
  std::string name = demanMtmName(mtmName, mtmLoc,mtmLoc );
  witSetArcAttribute(witGetBomEntryOffset,witSetBomEntryOffset,
    name,0,period,time);
}
void ESO2problem::setMtmDemanTime(
                     const std::string & mtmName, 
                     const std::string & supplyLoc, 
                     const std::string & demanLoc, 
                     int period, 
                     float time )
{ 
  std::string name = demanMtmName(mtmName, supplyLoc, demanLoc );
  witSetArcAttribute(witGetBomEntryOffset,witSetBomEntryOffset,
    name,0,period,time);
}
void ESO2problem::setMtmDemanTime(
    const std::string & mtmName, 
    const std::string & supplyLoc,  
    const std::string & demanLoc, 
    const std::vector<float> & time )
{ 
  std::string name = demanMtmName(mtmName, supplyLoc,demanLoc );
  witSetArcAttribute(witSetBomEntryOffset,name,0,time);
}
std::vector<float> ESO2problem::getMtmDemanTime(
    const std::string & mtmName, 
    const std::string & mtmLoc ) const
{
  std::string name = demanMtmName(mtmName, mtmLoc, mtmLoc );
  return witGetArcAttribute(witGetBomEntryOffset,name,0);
}
std::vector<float> ESO2problem::getMtmDemanTime(
    const std::string & mtmName, 
    const std::string & supplyLoc,
	const std::string & demanLoc) const
{
  std::string name = demanMtmName(mtmName, supplyLoc, demanLoc );
  return witGetArcAttribute(witGetBomEntryOffset,name,0);
}


void ESO2problem::getMtms(
    std::vector<std::string> & mtms,
    std::vector<std::string> & mtmLoc ) const
{
  mtms.clear();
  mtmLoc.clear();
#if 0
  int nMtms = mtmNames_.size();
  int m;
  for ( m=0; m<nMtms; m++ ) {
    mtms.push_back( mtmFromMtmName(mtmNames_[m]) );
    mtmLoc.push_back( mtmDestLocFromMtmName(mtmNames_[m]) );
  }
#else
  std::set<std::string>::const_iterator it;
  for ( it=mtmNames_.begin(); it!=mtmNames_.end(); ++it ) {
    mtms.push_back( mtmFromMtmName(*it) );
    mtmLoc.push_back( mtmDestLocFromMtmName(*it) );
  }
#endif
}


void ESO2problem::getMtmDemanLocs(
    std::vector<std::string> & mtms,
    std::vector<std::string> & srcLocs,
    std::vector<std::string> & destLocs ) const
{
  mtms.clear();
  srcLocs.clear();
  destLocs.clear();
#if 0
  int nMtms = mtmNames_.size();
  int m;
  for ( m=0; m<nMtms; m++ ) {
    
    const std::string & mtm = mtmFromMtmName(mtmNames_[m]);
    const std::string & srcLoc = mtmDestLocFromMtmName(mtmNames_[m]);
#else
  std::set<std::string>::const_iterator it;
  for ( it=mtmNames_.begin(); it!=mtmNames_.end(); ++it ) {
    const std::string & mtm = mtmFromMtmName(*it);
    const std::string & srcLoc = mtmDestLocFromMtmName(*it);
#endif
    mtms.push_back( mtm );
    srcLocs.push_back( srcLoc );
    destLocs.push_back( srcLoc );
    
    // are there any interplant moves to deman mtm at another location
    std::string mtmBName = baseMtmName(mtm,srcLoc);
    if ( mtmAcquireExists(mtmBName) ) {
      
      // mtm has interplant moves so get all destLocs
      
      // Get all locations that this mtm can be moved to
      const std::set<std::string> & moveToLocs = getMtmAcquireLocs(mtmBName);
      
      // Loop once for each location this mtm can be moved to for deman
      std::set<std::string>::const_iterator locIt;
      for(locIt=moveToLocs.begin(); locIt!=moveToLocs.end(); ++locIt ) {
        const std::string & destLoc = *locIt;
        
        mtms.push_back( mtm );
        srcLocs.push_back( srcLoc );
        destLocs.push_back( destLoc );
        
      } // end of locIt loop
    } // end of if (mtmAcquireExists(baseMtmName)) 
  } // end of m loop
}

std::vector<float> ESO2problem::getMtmAcquireQty(
                                                 const std::string & mtmName, 
                                                 const std::string & mtmLoc )const
{
  return getMtmAcquireQty(mtmName,mtmLoc,mtmLoc);
}

std::vector<float> ESO2problem::getMtmAcquireQtyWithOffset(
                                                 const std::string & mtmName, 
                                                 const std::string & mtmLoc )const
{
  return getMtmAcquireQtyWithOffset(mtmName,mtmLoc,mtmLoc);
}

std::vector<float> ESO2problem::getMtmAcquireQty(
                                                 const std::string & mtmName, 
                                                 const std::string & srcLoc , 
                                                 const std::string & destLoc )const
{
  // Wit ExecVol includes the time to demanufacture.
  // To get acquireQty, execVol needs to be shifted by bom offset
  std::string opName = acquireMtmName(mtmName,srcLoc,destLoc);
  return witGetNameAttribute(witGetOperationExecVol,opName);
}

std::vector<float> ESO2problem::getMtmAcquireQtyWithOffset(
                                                 const std::string & mtmName, 
                                                 const std::string & srcLoc , 
                                                 const std::string & destLoc )const
{
  // Wit ExecVol includes the time to demanufacture.
  // To get acquireQty, execVol needs to be shifted by bom offset
  std::string opName = acquireMtmName(mtmName,srcLoc,destLoc);
  return getOffsetExecVol(opName);
}

std::vector<float> ESO2problem::getMtmSupply(
                                                 const std::string & mtmName, 
                                                 const std::string & mtmLoc )const
{
  std::vector<float> retVal;
  std::string name = igfMtmName(mtmName,mtmLoc);
  retVal = witGetNameAttribute(witGetPartSupplyVol,name);
  return retVal;
}


//----------------------
// mtm interplant methods
//----------------------
bool ESO2problem::mtmInterplantExists     (const std::string & mtm, const std::string & srcLoc, const std::string & destLoc )const
{
  std::string mtmBName = baseMtmName(mtm,srcLoc);
  return mtmAcquireLocExists(mtmBName,destLoc);
}
bool ESO2problem::mtmInterplantExists     (const std::string & mtm, const std::string & srcLoc )const
{
  std::string mtmBName = baseMtmName(mtm,srcLoc);
  return mtmAcquireExists(mtmBName);
}
void ESO2problem::addMtmInterplant(
                                   const std::string & mtm, 
                                   const std::string & srcLoc, 
                                   const std::string & destLoc,
                                   float defaultMoveCost, 
                                   float defaultMoveTime  )
{
  assert(mtmExists(mtm,srcLoc));
  
  std::string mtmBName = baseMtmName(mtm,srcLoc);
  std::string igfMtmWitName = igfMtmName(mtm, srcLoc );
  std::string acquireMtmWitName = acquireMtmName(mtm, srcLoc, destLoc );
  std::string serverMtmWitName = serverMtmName(mtm, srcLoc, destLoc );
  std::string demanMtmWitName = demanMtmName(mtm, srcLoc, destLoc );

  // Add acquire operation, and connect to IGF owned part
  //witAddPart(witRun(), igfMtmWitName.c_str(), WitCAPACITY);
  witAddOperation(witRun(), acquireMtmWitName.c_str() );
  witAddBomEntry(witRun(), acquireMtmWitName.c_str(), igfMtmWitName.c_str() );

  // Add substitute boms to allow period 0 supply to be consumed in latter periods.
  addSubsForPeriod1SupplyPerishPeriod(acquireMtmWitName,igfMtmWitName,defaultMoveTime);

  // Set default moveTime
  std::vector<float> defaultMoveTimeVec = floatToStlVec(defaultMoveTime);
  witSetArcAttribute(witSetBomEntryOffset,acquireMtmWitName,0,defaultMoveTimeVec);

  // Add server owned mtm, and connect to acquire operation
  addNonScrapableCapacity(serverMtmWitName.c_str());
  witAddBopEntry(witRun(), acquireMtmWitName.c_str(), serverMtmWitName.c_str() );

  // Add demanufacturing operation and connect to server owned mtm
  witAddOperation(witRun(), demanMtmWitName.c_str() );
  witAddBomEntry(witRun(), demanMtmWitName.c_str(), serverMtmWitName.c_str() );

  // Set default app data
  int np = getNPeriods();
  ESO2mtmAppData * appData = new ESO2mtmAppData(np);
  std::vector<float> defaultMoveCostVec = floatToStlVec(defaultMoveCost);
  appData->setMoveCost(defaultMoveCostVec);
  witSetOperationAppData (mutableWitRun(), acquireMtmWitName.c_str(), appData);

  std::vector<float> demanCost=floatToStlVec(ESO2problem::bigM());
  witSetNameAttribute(witSetOperationObj1ExecCost,demanMtmWitName,demanCost);

  std::vector<float> demanTime=floatToStlVec((float)(np+1));
  witSetArcAttribute(witSetBomEntryOffset,demanMtmWitName,0,demanTime);

  addMtmAcquireLoc(mtmBName,destLoc);

}
void ESO2problem::setMtmInterplantMoveCost(const std::string & mtm, const std::string & srcLoc, const std::string & destLoc, int period, float moveCost )
{
  std::string name = acquireMtmName(mtm, srcLoc, destLoc );
  ESO2mtmAppData * appData;
  witGetOperationAppData(mutableWitRun(), name.c_str(), (void**)&appData);
  appData->setMoveCost(period,moveCost);
}
void ESO2problem::setMtmInterplantMoveTime(const std::string & mtm, const std::string & srcLoc, const std::string & destLoc, int period, float moveTime )
{  
  std::string opName = acquireMtmName(mtm,srcLoc,destLoc); 
  witSetArcAttribute(
    witGetBomEntryOffset,
    witSetBomEntryOffset,
    opName, 0,
    period, moveTime );

  // check to see if offset on substitute arc with move time needs to be updated.
  int perishLimit = getPeriod1SupplyPerishPeriod();
  int np = getNPeriods();
  if ( period>=moveTime+1 && period<=moveTime+perishLimit && moveTime+1<np ) {
    float offsetOfT = moveTime+1;
    witSetSubArcAttribute(
      witGetSubsBomEntryOffset,
      witSetSubsBomEntryOffset,
      opName,0,0,
      period,offsetOfT);
  }
  else {
    // substitute can not be used to consume from first period,
    // so set substitute offset to be same as bom's offset which
    // has the effect of making the substitute redundant
    witSetSubArcAttribute(
      witGetSubsBomEntryOffset,
      witSetSubsBomEntryOffset,
      opName,0,0,
      period,0.0f);
  }

}
std::vector<float> ESO2problem::getMtmInterplantMoveCost(const std::string & mtm,const std::string & srcLoc, const std::string & destLoc )const
{
  std::string name = acquireMtmName(mtm, srcLoc, destLoc );
  ESO2mtmAppData * appData;
  witGetOperationAppData(mutableWitRun(), name.c_str(), (void**)&appData);
  return appData->getMoveCost();
}  
std::vector<float> ESO2problem::getMtmInterplantMoveTime(const std::string & mtm,const std::string & srcLoc, const std::string & destLoc )const
{
  std::vector<float> retVal;
  std::string opName = acquireMtmName(mtm,srcLoc,destLoc);
  retVal = witGetArcAttribute(witGetBomEntryOffset,opName,0);
  return retVal;
} 
std::vector<float> ESO2problem::getMtmInterplantMoveVol(const std::string & mtm,const std::string & srcLoc, const std::string & destLoc )const
{
  std::vector<float> retVal;
  std::string opName = acquireMtmName(mtm,srcLoc,destLoc);
  retVal = witGetNameAttribute(witGetOperationExecVol,opName);
  return retVal;
}

// This is an internal method called when a new part is created.
// The location of the part is added to a map from part to location.
// This allows one to quickly find all locations given a partname.
void ESO2problem::addPartOrFeatureLoc(const std::string & partOrFeatureName, const std::string & loc )
{
  // is the first location for partOrFeature?
  ESOmapStringToStringSetIterator it;
  it = partOrFeatureLocs_.find(partOrFeatureName);
  if ( it == partOrFeatureLocs_.end() ) {
    // this is first location for partOrFeature
    ESOstringSet locs;
    locs.insert(loc);
    partOrFeatureLocs_[partOrFeatureName] = locs;
  }
  else {
    // partOrFeature already has loc, so get vector of locs
    ESOstringSet & locs = it->second;
    // add new loc.  
    locs.insert(loc);
  }
}
void ESO2problem::removePartOrFeatureLoc(const std::string & partOrFeatureName, const std::string & loc )
{
  ESOmapStringToStringSetIterator it;
  it = partOrFeatureLocs_.find(partOrFeatureName);

  // part does not exist in map, then just return.
  if( it != partOrFeatureLocs_.end() ) return;

  // get vector of locs for this part
  ESOstringSet & locs = it->second;
  // remove loc. 
  locs.erase(loc);

  // if there are no more locs for this part, then remove part from map
  if ( locs.size()==0 ) {
    partOrFeatureLocs_.erase(partOrFeatureName);
  }
}

// Get all locations were a part exists
std::vector<std::string> ESO2problem::getPartLocs(const std::string & partName ) const
{
  std::vector<std::string> retVal;

  ESOmapStringToStringSetConstIterator it;
  it = partOrFeatureLocs_.find(partName);

  // part does not exist in map, then just return.
  if( it == partOrFeatureLocs_.end() ) return retVal;

  // get vector of locs for this part
  const ESOstringSet & locs = it->second;

  // iterate over set makeing a vector
  ESOstringSetConstIterator locIt;
  for ( locIt = locs.begin(); locIt!=locs.end(); ++locIt ) {
    retVal.push_back(*locIt);
  }
  return retVal;
}
  
void ESO2problem::addMtmAcquireLoc(const std::string & mtmName, const std::string & mtmAcquireLoc )
{
#if 0
  // is the first location for mtm?
  ESOmapStringToStringSetIterator it;
  it = mtmAcquireLocs_.find(mtmName);
  if ( it == mtmAcquireLocs_.end() ) {
    // this is first location for mtm
    ESOstringSet locs;
    locs.insert(mtmAcquireLoc);
    mtmAcquireLocs_[mtmName] = locs;
    //mtmAcquireLocs_[mtmName].insert(mtmAcquireLoc);
  }
  else {
    // mtm already has acquire loc, so get vector of acquire locs
    ESOstringSet & locs = it->second;
    // make sure loc does not already exist
    assert( !mtmAcquireLocExists(mtmName,mtmAcquireLoc) );
    // add new loc.  
    locs.insert(mtmAcquireLoc);
  }
#else
  mtmAcquireLocs_[mtmName].insert(mtmAcquireLoc);
#endif
}

bool ESO2problem::mtmAcquireLocExists(const std::string & mtmName, const std::string & mtmAcquireLoc ) const
{
  bool retVal = false;
  ESOmapStringToStringSetConstIterator it;
  it = mtmAcquireLocs_.find(mtmName);
  if ( it != mtmAcquireLocs_.end() ) {
    const ESOstringSet & locs = it->second;
    if ( locs.find(mtmAcquireLoc) != locs.end() )
      retVal = true;
  }
  return retVal;
}

bool ESO2problem::mtmAcquireExists(const std::string & mtmName ) const
{
  bool retVal = false;
  ESOmapStringToStringSetConstIterator it;
  it = mtmAcquireLocs_.find(mtmName);
  if ( it != mtmAcquireLocs_.end() ) {
    retVal = true;
  }
  return retVal;
}

const std::set<std::string> & ESO2problem::getMtmAcquireLocs(const std::string & mtmName)const
{
  ESOmapStringToStringSetConstIterator it;
  it = mtmAcquireLocs_.find(mtmName);
  assert( it != mtmAcquireLocs_.end() );
  const ESOstringSet & retVal = it->second;
  return retVal;
}





//----------------------
// Feature Methods
//----------------------
void ESO2problem::addFeature(const std::string & featureName, const std::string & featureLoc )
{
  std::string igfFeatureWitName = igfFeatureName(featureName, featureLoc );
  std::string acquireFeatureWitName = acquireFeatureName(featureName, featureLoc );
  std::string serverFeatureWitName = partName(featureName, featureLoc );

  // Add igf owned feature, acquire operation, and connect them
  witAddPart(witRun(), igfFeatureWitName.c_str(), WitCAPACITY);
  witAddOperation(witRun(), acquireFeatureWitName.c_str() );
  witAddBomEntry(witRun(), acquireFeatureWitName.c_str(), igfFeatureWitName.c_str() );  

  // Add substitute boms to allow period 0 supply to be consumed in latter periods.
  addSubsForPeriod1SupplyPerishPeriod(acquireFeatureWitName,igfFeatureWitName,0.0f);

  // Add server owned feature, and connect to acquire operation
  addPartWithOutApplyingInterplants(featureName,featureLoc);
#if 0
  std::string serverOwnedFeatureName = partName(featureName, featureLoc );
  std::string moveableServerOwnedFeatureName = moveablePartName(featureName, featureLoc );
  witAddPart(witRun(), serverOwnedFeatureName.c_str(), WitCAPACITY);
  witAddPart(witRun(), moveableServerOwnedFeatureName.c_str(), WitCAPACITY);
  witAddOperation(witRun(),serverOwnedFeatureName.c_str());
  witAddBomEntry(witRun(),serverOwnedFeatureName.c_str(),moveableServerOwnedFeatureName.c_str());
  witAddBopEntry(witRun(),serverOwnedFeatureName.c_str(),serverOwnedFeatureName.c_str());
#endif
  
  std::string moveableServerOwnedFeatureName = moveablePartName(featureName, featureLoc );
  witAddBopEntry(witRun(), acquireFeatureWitName.c_str(), moveableServerOwnedFeatureName.c_str() );


#if 0
  // Set default acquistion cost
  float * acquireCost = floatToConstFloatStar(bigM());
  witSetOperationObj1ExecCost(witRun(),acquireFeatureWitName.c_str(),acquireCost);
  free(acquireCost);
#endif
  
  // Set default app data to hold wfmv (ie acquistion cost)
  int np = getNPeriods();
  ESO2featureAppData * appData = new ESO2featureAppData(np);
  witSetOperationAppData (mutableWitRun(), acquireFeatureWitName.c_str(), appData);
  
  std::string baseName = baseFeatureName(featureName, featureLoc );
  featureNames_.insert(baseName);
}
bool ESO2problem::featureExists(const std::string & featureName, const std::string & featureLoc )const
{
  std::string igfFeatureWitName = igfFeatureName(featureName, featureLoc );
  witBoolean partExists;
  witGetPartExists(mutableWitRun(),igfFeatureWitName.c_str(),&partExists);
  bool retVal = false;
  if( partExists) retVal = true;
  return retVal;
}
void ESO2problem::setFeatureSupply(const std::string & featureName, const std::string & featureLoc, int period, float supply )
{
  std::string name = igfFeatureName(featureName, featureLoc );
  witSetNameAttribute(witGetPartSupplyVol,witSetPartSupplyVol,name,period,supply);
}
void ESO2problem::setFeatureWfmv(const std::string & featureName, const std::string & featureLoc, int period, float wholeSaleFairMarketValue )
{ 
  std::string name = acquireFeatureName(featureName, featureLoc );
  
  ESO2featureAppData * appData;
  witGetOperationAppData(mutableWitRun(), name.c_str(), (void**)&appData);
  appData->setWfmv(period,wholeSaleFairMarketValue);
}

void ESO2problem::getFeatures(
    std::vector<std::string> & features,
    std::vector<std::string> & featureLocs ) const
{
  features.clear();
  featureLocs.clear();
#if 0
  int nFeatures = featureNames_.size();
  int f;
  for ( f=0; f<nFeatures; f++ ) {
    features.push_back( featureFromFeatureName(featureNames_[f]) );
    featureLocs.push_back( locFromFeatureName(featureNames_[f]) );
  }
#else
  std::set<std::string>::const_iterator it;
  for ( it=featureNames_.begin(); it!=featureNames_.end(); ++it ) {
    features.push_back( featureFromFeatureName(*it) );
    featureLocs.push_back( locFromFeatureName(*it) );
  }
#endif
}

std::vector<float> ESO2problem::getFeatureAcquireQty(
                                                 const std::string & featureName, 
                                                 const std::string & featureLoc )const
{
  std::vector<float> retVal;
  std::string opName = acquireFeatureName(featureName,featureLoc);
  retVal = witGetNameAttribute(witGetOperationExecVol,opName);
  return retVal;
}
std::vector<float> ESO2problem::getFeatureSupply(
                                                 const std::string & featureName, 
                                                 const std::string & featureLoc )const
{
  std::vector<float> retVal;
  std::string name = igfFeatureName(featureName,featureLoc);
  retVal = witGetNameAttribute(witGetPartSupplyVol,name);
  return retVal;
}

std::vector<float> ESO2problem::getFeatureWfmv(
                                                 const std::string & featureName, 
                                                 const std::string & featureLoc )const
{
  std::string name = acquireFeatureName(featureName,featureLoc);
  ESO2featureAppData * appData;
  witGetOperationAppData(mutableWitRun(), name.c_str(), (void**)&appData);
  return appData->getWfmv();
}








//----------------------
// LoosePart Methods
//----------------------
void ESO2problem::addLoosePart(const std::string & loosePartName, const std::string & loosePartLoc )
{
  std::string igfLoosePartWitName = igfLoosePartName(loosePartName, loosePartLoc );
  std::string acquireLoosePartWitName = acquireLoosePartName(loosePartName, loosePartLoc );
  std::string serverLoosePartWitName = moveablePartName(loosePartName, loosePartLoc );

  // Add igf owned loosePart, acquire operation, and connect them
  witAddPart(witRun(), igfLoosePartWitName.c_str(), WitCAPACITY);
  witAddOperation(witRun(), acquireLoosePartWitName.c_str() );
  witAddBomEntry(witRun(), acquireLoosePartWitName.c_str(), igfLoosePartWitName.c_str() );

  // Add substitute boms to allow period 0 supply to be consumed in latter periods.
  addSubsForPeriod1SupplyPerishPeriod(acquireLoosePartWitName,igfLoosePartWitName,0.0f);
  
  // if server owned part does not exist, then add it to model
  if ( !partExists(loosePartName,loosePartLoc) ) {
    addPart(loosePartName,loosePartLoc);
  }
  // connect server owned loose part to acquire operation
  witAddBopEntry(witRun(), acquireLoosePartWitName.c_str(), serverLoosePartWitName.c_str() );

#if 0
  // Set default acquistion cost
  float * acquireCost = floatToConstFloatStar(bigM());
  witSetOperationObj1ExecCost(witRun(),acquireLoosePartWitName.c_str(),acquireCost);
  free(acquireCost);
#endif
  
  // Set default app data to hold wfmv (ie acquistion cost)
  int np = getNPeriods();
  ESO2loosePartAppData * appData = new ESO2loosePartAppData(np);
  witSetOperationAppData (mutableWitRun(), acquireLoosePartWitName.c_str(), appData);
  
  std::string baseName = baseLoosePartName(loosePartName, loosePartLoc );
  loosePartNames_.insert(baseName);
}
bool ESO2problem::loosePartExists(const std::string & loosePartName, const std::string & loosePartLoc )const
{
  std::string igfLoosePartWitName = igfLoosePartName(loosePartName, loosePartLoc );
  witBoolean partExists;
  witGetPartExists(mutableWitRun(),igfLoosePartWitName.c_str(),&partExists);
  bool retVal = false;
  if( partExists) retVal = true;
  return retVal;
}
void ESO2problem::setLoosePartSupply(const std::string & loosePartName, const std::string & loosePartLoc, int period, float supply )
{
  std::string name = igfLoosePartName(loosePartName, loosePartLoc );
  witSetNameAttribute(witGetPartSupplyVol,witSetPartSupplyVol,name,period,supply);
}
void ESO2problem::setLoosePartWfmv(const std::string & loosePartName, const std::string & loosePartLoc, int period, float wholeSaleFairMarketValue )
{ 
  std::string name = acquireLoosePartName(loosePartName, loosePartLoc );
  
  ESO2loosePartAppData * appData;
  witGetOperationAppData(mutableWitRun(), name.c_str(), (void**)&appData);
  appData->setWfmv(period,wholeSaleFairMarketValue);  
}

void ESO2problem::getLooseParts(
    std::vector<std::string> & looseParts,
    std::vector<std::string> & loosePartLocs ) const
{
  looseParts.clear();
  loosePartLocs.clear();
#if 0
  int nLooseParts = loosePartNames_.size();
  int f;
  for ( f=0; f<nLooseParts; f++ ) {
    looseParts.push_back( loosePartFromLoosePartName(loosePartNames_[f]) );
    loosePartLocs.push_back( locFromLoosePartName(loosePartNames_[f]) );
  }
#else
  std::set<std::string>::const_iterator it;
  for ( it=loosePartNames_.begin(); it!=loosePartNames_.end(); ++it ) {
    looseParts.push_back( loosePartFromLoosePartName(*it) );
    loosePartLocs.push_back( locFromLoosePartName(*it) );
  }
#endif
}

std::vector<float> ESO2problem::getLoosePartAcquireQty(
                                                 const std::string & loosePartName, 
                                                 const std::string & loosePartLoc )const
{
  std::vector<float> retVal;
  std::string opName = acquireLoosePartName(loosePartName,loosePartLoc);
  retVal = witGetNameAttribute(witGetOperationExecVol,opName);
  return retVal;
}
std::vector<float> ESO2problem::getLoosePartSupply(
                                                 const std::string & loosePartName, 
                                                 const std::string & loosePartLoc )const
{
  std::vector<float> retVal;
  std::string name = igfLoosePartName(loosePartName,loosePartLoc);
  retVal = witGetNameAttribute(witGetPartSupplyVol,name);
  return retVal;
}
std::vector<float> ESO2problem::getLoosePartWfmv(
                                                 const std::string & loosePartName, 
                                                 const std::string & loosePartLoc )const
{ 
  std::string name = acquireLoosePartName(loosePartName,loosePartLoc);
  ESO2featureAppData * appData;
  witGetOperationAppData(mutableWitRun(), name.c_str(), (void**)&appData);
  return appData->getWfmv();
}
//----------------------
// Part Methods
//----------------------
void ESO2problem::addPartWithOutApplyingInterplants(const std::string & part, const std::string & partLoc )
{
  std::string witName = partName(part, partLoc );
  std::string moveAbleWitName = moveablePartName(part, partLoc );
  assert( !partExists(part,partLoc) );
  addNonScrapableCapacity( witName.c_str());
  addNonScrapableCapacity(moveAbleWitName.c_str());
  witAddOperation(witRun(),witName.c_str());
  witAddBomEntry(witRun(),witName.c_str(),moveAbleWitName.c_str());
  witAddBopEntry(witRun(),witName.c_str(),witName.c_str());

  // Add partLoc to map of parts to locations.
  addPartOrFeatureLoc(part,partLoc);
}
void ESO2problem::addPart(const std::string & part, const std::string & partLoc )
{
  std::string witName = partName(part, partLoc );
  std::string moveAbleWitName = moveablePartName(part, partLoc );
  addPartWithOutApplyingInterplants(part,partLoc);

  // Now add all interplant moves for this part.
  // First get vector of part interplant moves from partLoc
  ESO2partInterplantVec & interPlantVec = defaultPartInterplants_[partLoc];

  // Loop once for each destination location
  int l=0;
  for ( l=0; l<interPlantVec.size(); l++ ) {
    const std::string & destLoc = interPlantVec[l].getDestLoc();
    // does part at dest loc exist
    //bool partExist = partExists(part,destLoc);
    
    // If link between srcLoc and destLoc for part does not exist, then add it to model
    if ( !partInterplantExists(part,partLoc,destLoc ) ) {
      
      // Get default move cost and time between srcLoc and destLoc
      float defaultMoveCost = interPlantVec[l].getDefaultMoveCost();
      float defaultMoveTime = interPlantVec[l].getDefaultMoveTime();
      addPartInterplant(part,partLoc,destLoc,defaultMoveCost,defaultMoveTime);
    } // end adding interplant.

    // If  destination part did not exist, now it does after adding interplant.
    // Add all interplant moves for this new part
    //if ( !partExist ) {
    //  addAllPartInterplants( eso2Prob, srcToDestLocs, partInterplantFile, part, destLoc);
    //}
  }  // end for l 
}
bool ESO2problem::partExists(const std::string & name, const std::string & loc )const
{
  std::string witName = partName(name, loc );
  witBoolean partExists;
  witGetPartExists(mutableWitRun(),witName.c_str(),&partExists);
  bool retVal = false;
  if( partExists) retVal = true;
  return retVal;
}

//----------------------
// Methods for Deman (demanufactuing boms).
//----------------------
bool ESO2problem::demanBomExists(const std::string & mtm, const std::string & part, const std::string & loc )const
{
  bool retVal = false;
  // the demanBom exists if there is a wit bop entry.
  // get index of bop entry. if doesn't exist then -1 is returned.
  int bei = getBopEntryIndex(mtm,part,loc);
  if ( bei!=-1 ) retVal = true;
  return retVal;
}
void ESO2problem::addDemanBom(const std::string & mtm, const std::string & part, const std::string & loc )
{
  assert( !demanBomExists(mtm,part,loc) );
  int np = getNPeriods();

  // if produced part does not exist, then add it to model
  if ( !partExists(part,loc) ) {
    addPart(part,loc);
  }
  
#ifdef CONSUME_ACQUIRED
  // make sure scrap cost of produced parts is zero
  witSetNameAttribute(witSetPartObj1ScrapCost,partName(part, loc ),0.0f);
  witSetNameAttribute(witSetPartObj1ScrapCost,moveablePartName(part, loc ),0.0f);
#endif

  // link mtm demanufacturing operation at src location of consumed part with produced part
  std::string producedPart = moveablePartName(part, loc );
  std::string demanOp = demanMtmName(mtm, loc, loc );
  witAddBopEntry(witRun(),demanOp.c_str(),producedPart.c_str() );

  // set prodRate to default value
  int nBops;
  witGetOperationNBopEntries(mutableWitRun(),demanOp.c_str(),&nBops);
  std::vector<float> prodRate = floatToStlVec(0.f);
  witSetArcAttribute(witSetBopEntryProductRate,demanOp,nBops-1,prodRate);

  // add appData to hold attachRate
  ESO2mtmDemanBomAppData * appData = new ESO2mtmDemanBomAppData(np);
  std::vector<float> attachRate = floatToStlVec(0.0f);
  appData->setAttachRate(attachRate);
  witSetBopEntryAppData (mutableWitRun(), demanOp.c_str(),nBops-1, appData);

  // Are there any interplant moves for this mtm
  std::string mtmBName = baseMtmName(mtm,loc);
  if ( mtmAcquireExists(mtmBName) ) {
    
    // MTM has move operations so add bopEntry for each move to location
    
    // Get all locations that this mtm can be moved to
    const std::set<std::string> & moveToLocs = getMtmAcquireLocs(mtmBName);
    
    // Loop once for each location this mtm can be moved to
    std::set<std::string>::const_iterator locIt;
    for(locIt=moveToLocs.begin(); locIt!=moveToLocs.end(); ++locIt ) {
      const std::string & destLoc = *locIt;
      
      // if produced part at destLoc does not exist, then add it to model
      if ( !partExists(part,destLoc) ) {
        addPart(part,destLoc);
      }
        
#ifdef CONSUME_ACQUIRED
      // make sure scrap cost of produced parts is zero
      witSetNameAttribute(witSetPartObj1ScrapCost,partName(part, destLoc ),0.0f);
      witSetNameAttribute(witSetPartObj1ScrapCost,moveablePartName(part, destLoc ),0.0f);
#endif
      
      // Add bop entry
      demanOp = demanMtmName(mtm,loc,destLoc);
      std::string producedPart = moveablePartName(part, destLoc );
      witAddBopEntry(witRun(),demanOp.c_str(),producedPart.c_str() );

#if 0
      // Test that nBops has not changed
      {  
        int nDestBops;
        witGetOperationNBopEntries(mutableWitRun(),demanOp.c_str(),&nDestBops);
        assert( nDestBops == nBops );
      }
#endif

      // set prodRate to default
      witSetArcAttribute(witSetBopEntryProductRate,demanOp,nBops-1,prodRate);
      
    } // end of locIt loop
    
  } // end of if (mtmAcquireExists(baseMtmName)) 
}

std::vector<std::string> ESO2problem::getDemanBoms(const std::string & mtm, const std::string & mtmSrcLoc, const std::string & demanLoc)const
{
  std::vector<std::string> retVal;

  std::string demanOp = demanMtmName(mtm, mtmSrcLoc, demanLoc );

#if 0
  int nBopEntries;
  witGetOperationNBopEntries(mutableWitRun(),demanOp.c_str(),&nBopEntries);

  int b;
  for ( b=0; b<nBopEntries; b++ ) {
    char * prodPart;
    witGetBopEntryProducedPart(mutableWitRun(),demanOp.c_str(),b,&prodPart);
    retVal.push_back(partFromPartName(prodPart));    
  }
#else
  std::vector<std::string> witNames = witGetOperProducts(demanOp.c_str());
  int n;
  for (n=0; n<witNames.size(); ++n) {
    retVal.push_back(witNames[n].c_str());
  }
#endif

  return retVal;
}
std::vector<std::string> ESO2problem::witGetOperProducts(const std::string & op)const
{
  std::vector<std::string> retVal;

  int nBopEntries;
  witGetOperationNBopEntries(mutableWitRun(),op.c_str(),&nBopEntries);

  int b;
  for ( b=0; b<nBopEntries; b++ ) {
    char * prodPart;
    witGetBopEntryProducedPart(mutableWitRun(),op.c_str(),b,&prodPart);
    retVal.push_back(prodPart);    
    witFree(prodPart);
  }
  return retVal;
}

void ESO2problem::setDemanBomAttachRate(
          const std::string & mtm, 
          const std::string & part, 
          const std::string & loc, 
          int period, 
          float attachRate )
{
  setDemanBomAndFeatureXrefProdRatesAreSet(false);

  std::string producedPart = moveablePartName(part, loc );
  std::string demanOp = demanMtmName(mtm, loc, loc );

  // Set ProdRate for part at its src location
  int index = getBopEntryIndex(demanOp,producedPart);

  ESO2mtmDemanBomAppData * appData;
  witGetBopEntryAppData(mutableWitRun(), demanOp.c_str(),index, (void**)&appData);
  appData->setAttachRate(period,attachRate);
#if 0
  witSetArcAttribute(witGetBopEntryProductRate,
    witSetBopEntryProductRate,
    demanOp, index,
    period,attachRate);

  // Are there any interplant moves for this mtm
  std::string mtmBName = baseMtmName(mtm,loc);
  if ( mtmAcquireExists(mtmBName) ) {
    
    // MTM has move operations so set prodRate for each move to location
    
    // Get all locations that this mtm can be moved to
    const std::set<std::string> & moveToLocs = getMtmAcquireLocs(mtmBName);
    
    // Loop once for each location this mtm can be moved to
    std::set<std::string>::const_iterator locIt;
    for(locIt=moveToLocs.begin(); locIt!=moveToLocs.end(); ++locIt ) {
      const std::string & destLoc = *locIt;
      
      // set ProdRate
      demanOp = demanMtmName(mtm,loc,destLoc);
      witSetArcAttribute(witGetBopEntryProductRate,
          witSetBopEntryProductRate,
          demanOp, index,
          period,attachRate);
      
    } // end of locIt loop
    
  } // end of if (mtmAcquireExists(baseMtmName)) 
#endif
}


std::vector<float> ESO2problem::getDemanBomAttachRate(
          const std::string & mtm, 
          const std::string & part, 
          const std::string & loc ) const
{
  std::string producedPart = moveablePartName(part, loc );
  std::string demanOp = demanMtmName(mtm, loc, loc );

  // Set ProdRate for part at its src location
  int index = getBopEntryIndex(demanOp,producedPart);

  ESO2mtmDemanBomAppData * appData;
  witGetBopEntryAppData(mutableWitRun(), demanOp.c_str(),index, (void**)&appData);
  return appData->getAttachRate();
}


// Get Bop Entry Index
int ESO2problem::getBopEntryIndex( 
                                const std::string & mtm, 
                                const std::string & part, 
                                const std::string & loc ) const
{
  // Get wit names
  std::string producedPart = moveablePartName(part, loc );
  std::string producingOp = demanMtmName(mtm, loc, loc );
  int retVal = getBopEntryIndex(producingOp,producedPart);

  return retVal;
}

// Get Bop Entry Index
int ESO2problem::getBopEntryIndex( 
                                const std::string & opName, 
                                const std::string & partName
                                ) const
{
  int retVal = -1;
  
  int nP = getNPeriods();

  // Get current number of bops connected to operation
  int nBops;
  witGetOperationNBopEntries(mutableWitRun(),opName.c_str(),&nBops);

  int bei;
  for ( bei=0; bei<nBops; bei++ ) {
    char * prodPartFromWit;
    witGetBopEntryProducedPart(mutableWitRun(),opName.c_str(),bei,&prodPartFromWit);
    if(partName==prodPartFromWit) retVal=bei;
    witFree(prodPartFromWit);
    if(retVal!=-1) break;
  }

  return retVal;
}


//----------------------
// feature cross reference methods
//----------------------
bool ESO2problem::featureXrefExists(
                                  const std::string & part, 
                                  const std::string & feature, 
                                  const std::string & loc )const
{
  std::string xrefName = featureXrefName(part,feature,loc);
  witBoolean exists;
  witGetOperationExists(mutableWitRun(),xrefName.c_str(),&exists);
  bool retVal = false;
  if( exists ) retVal = true;
  return retVal;
}
void ESO2problem::addFeatureXref(
                               const std::string & part, 
                               const std::string & feature, 
                               const std::string & loc )
{
  int np=getNPeriods();

  assert( !featureXrefExists(part,feature,loc) );
  //assert(  partExists(part,loc) );
  assert(  featureExists(feature,loc) );
  std::string xrefName = featureXrefName(part,feature,loc);  
  std::string refPartName = moveablePartName(part,loc); 
  std::string featureName = partName(feature,loc);

  // Create xref operation and connect to feature
  witAddOperation(witRun(),xrefName.c_str());
  witAddBomEntry(witRun(),xrefName.c_str(),featureName.c_str());

  // if referenced part doesn't exist, then add it to model.
  if ( !partExists(part,loc) ) {
    addPart(part,loc);
  }

  // Connect referenced part to xref operation  
  witAddBopEntry(witRun(),xrefName.c_str(),refPartName.c_str());

  // set default cost
  float * cost = floatToConstFloatStar(bigM());
  witSetOperationObj1ExecCost(witRun(),xrefName.c_str(),cost);
  delete [] cost;

  // set default prodRate
#if 0
  {
  float * prodRate = floatToConstFloatStar(0.f);
  witSetBopEntryProductRate(witRun(),xrefName.c_str(),0,prodRate);
  delete [] prodRate;
  }
#endif
  {
  // add appData to hold applProdRate
  ESO2featureXrefAppData * appData = new ESO2featureXrefAppData(np);
  std::vector<float> prodRate = floatToStlVec(0.0f);
  appData->setProdRate(prodRate);
  witSetOperationAppData (witRun(),xrefName.c_str(), appData);
  }


  // Set default usageTime
  float * usageTime = floatToConstFloatStar(bigM());
  witSetBomEntryOffset(witRun(),xrefName.c_str(),0,usageTime);
  delete [] usageTime;

  xrefFeatures_.insert(xrefName);
}
 
void ESO2problem::setFeatureXrefProdRate(
          const std::string & part, 
          const std::string & feature, 
          const std::string & loc, 
          int period, 
          float applProdRate )
{
  int np = getNPeriods();
  std::string xrefName = featureXrefName(part,feature,loc);

  // Set prod rate in appData.
  // It will latter be set in WIT data structures.
  // To do it now requires witPreprocessing, which can be time consuming.
  
  ESO2featureXrefAppData * appData;
  witGetOperationAppData(mutableWitRun(), xrefName.c_str(), (void**)&appData);
  appData->setProdRate(period,applProdRate);

#ifndef REDUCE_PREPROCESSING_TIME
  
  // must map featureXref applProdRate to  wit prodRate via impactPeriods.
  // The applProdRate's period is the period of supply.
  // Wit's prodRate is the period of the crossReference operation.
  // Wit's prodRate must be computed from applProdRate and impact periods.
  
  // Get names of wit objects
  std::string xrefOpName = featureXrefName(part,feature,loc );
  std::string acquireFeatureOpName = acquireFeatureName(feature,loc );
  
  // Get all impact periods that are in affect from xref operation
  // to source of supply
  std::vector<StdVecInt> impPersVecs;
  impPersVecs.push_back( witGetArcAttribute(witGetBomEntryImpactPeriod,xrefOpName,0) );
  impPersVecs.push_back( witGetArcAttribute(witGetBopEntryImpactPeriod,acquireFeatureOpName,0) );
  impPersVecs.push_back( witGetArcAttribute(witGetBomEntryImpactPeriod,acquireFeatureOpName,0) );
  
  std::vector<int> impPers = multiLevelImpactPeriod(impPersVecs);
  
  // Get current wit prodRates
  std::vector<float> witProdRate = witGetArcAttribute(witGetBopEntryProductRate,xrefOpName,0);
  
  // map applProdRate to witProdRate.
  // There are two cases to consider, since period zero supply may not perish
  
  if ( period==0 ) {
    // This is the perishable supply case
    int p;
    for ( p=0; p<np; ++p ) {
      if ( impPers[p] != -1 ) {
        if ( p==0 || useFeaturePerishableSupply(feature,loc,p ) ){
          witProdRate[p] = applProdRate;
        }
      }
    }
  }
  else {
    // This is the non-perishable supply case  
    int p;
    for ( p=0; p<np; ++p ) {
      if ( impPers[p] != period ) continue;
      witProdRate[p] = applProdRate;
    }
  }
  
  witSetArcAttribute(witSetBopEntryProductRate,xrefOpName,0,witProdRate); 
#endif
  
}  

void ESO2problem::setFeatureXrefCost(
          const std::string & part, 
          const std::string & feature, 
          const std::string & loc, 
          int period, 
          float cost )
{
  std::string xrefOpName = featureXrefName(part,feature,loc);
  witSetNameAttribute(
    witGetOperationObj1ExecCost,
    witSetOperationObj1ExecCost,
    xrefOpName,
    period, cost );
}
void ESO2problem::setFeatureXrefCost(
          const std::string & part, 
          const std::string & feature, 
          const std::string & loc,  
          float cost )
{
  std::string xrefOpName = featureXrefName(part,feature,loc);
  witSetNameAttribute(
    witSetOperationObj1ExecCost,
    xrefOpName,
    cost );
}
void ESO2problem::setFeatureXrefUsageTime(
          const std::string & part, 
          const std::string & feature, 
          const std::string & loc, 
          int period, 
          float usageTime )
{
  std::string xrefOpName = featureXrefName(part,feature,loc);
  witSetArcAttribute(
    witGetBomEntryOffset,
    witSetBomEntryOffset,
    xrefOpName, 0,
    period, usageTime );
} 

void ESO2problem::setFeatureXrefUsageTime(
          const std::string & part, 
          const std::string & feature, 
          const std::string & loc, 
          float usageTime )
{
  std::string xrefOpName = featureXrefName(part,feature,loc);
  witSetArcAttribute(
    witSetBomEntryOffset,
    xrefOpName, 0,
    usageTime );
} 

void ESO2problem::getFeatureXrefs(
    std::vector<std::string> & parts,
    std::vector<std::string> & features,
    std::vector<std::string> & locs ) const
{
  parts.clear();
  features.clear();
  locs.clear();
#if 0
  int nItems = alternateParts_.size();
  int n;
  for ( n=0; n<nItems; n++ ) {
    upLevelParts.push_back( upLevelPartFromAlternatePartName(alternateParts_[n]) );
    downLevelParts.push_back( downLevelPartFromAlternatePartName(alternateParts_[n]) );
    locs.push_back( locFromAlternatePartName(alternateParts_[n]) );
  }
#else
  std::set<std::string>::const_iterator it;
  for ( it=xrefFeatures_.begin(); it!=xrefFeatures_.end(); ++it ) {
    parts.push_back( partFromFeatureXrefName(*it) );
    features.push_back( featureFromFeatureXrefName(*it) );
    locs.push_back( locFromFeatureXrefName(*it) );
  }
#endif
}
std::vector<float> ESO2problem::getFeatureXrefVol(
                                                 const std::string & part,
                                                 const std::string & feature, 
                                                 const std::string & loc )const
{
  std::vector<float> retVal;
  std::string opName = featureXrefName(part,feature,loc);
  retVal = witGetNameAttribute(witGetOperationExecVol,opName);
  return retVal;
}

std::vector<float> ESO2problem::getFeatureXrefCost(
                                                 const std::string & part,
                                                 const std::string & feature, 
                                                 const std::string & loc )const
{
  std::vector<float> retVal;
  std::string opName = featureXrefName(part,feature,loc);
  retVal = witGetNameAttribute(witGetOperationObj1ExecCost,opName);
  return retVal;
}


std::vector<float> ESO2problem::getFeatureXrefProdRate(
                                                 const std::string & part,
                                                 const std::string & feature, 
                                                 const std::string & loc )const
{
  std::vector<float> retVal;
  std::string opName = featureXrefName(part,feature,loc);

  ESO2featureXrefAppData * appData;
  witGetOperationAppData(mutableWitRun(), opName.c_str(), (void**)&appData);
  assert( appData != NULL );
  retVal = appData->getProdRate();
  
  return retVal;
}


//----------------------
// alternate part methods
//----------------------
bool ESO2problem::altExists(
                                  const std::string & partOrFeature, 
                                  const std::string & altPart, 
                                  const std::string & loc )const
{
  std::string ecOpName = altName(partOrFeature,altPart,loc);
  witBoolean exists;
  witGetOperationExists(mutableWitRun(),ecOpName.c_str(),&exists);
  bool retVal = false;
  if( exists ) retVal = true;
  return retVal;
}

void ESO2problem::addAlt(
                               const std::string & partOrFeature, 
                               const std::string & altPart, 
                               const std::string & loc,
                               float altCost,
                               float altTime )
{
  assert( !altExists(partOrFeature,altPart,loc) );
  assert(  partExists(partOrFeature,loc) );
  std::string ecOpName = altName(partOrFeature,altPart,loc);  
  std::string upLevelPartName = moveablePartName(altPart,loc); 
  std::string downLevelPartName = partName(partOrFeature,loc);

  // Create EC operation and connect to downlevel part
  witAddOperation(witRun(),ecOpName.c_str());
  witAddBomEntry(witRun(),ecOpName.c_str(),downLevelPartName.c_str());

  float * offset = floatToConstFloatStar(altTime);
  witSetBomEntryOffset(witRun(),ecOpName.c_str(),0,offset);
  delete [] offset;

  // if up level part doesn't exist, then add it to model.
  if ( !partExists(altPart,loc) ) {
    addPart(altPart,loc);
  }

  // Connect uplevel part to ec operation  
  witAddBopEntry(witRun(),ecOpName.c_str(),upLevelPartName.c_str());

  // set default values
  float * cost = floatToConstFloatStar(altCost);
  witSetOperationObj1ExecCost(witRun(),ecOpName.c_str(),cost);
  delete [] cost;

  float * prodRate = floatToConstFloatStar(1.f);
  witSetBopEntryProductRate(witRun(),ecOpName.c_str(),0,prodRate);
  delete [] prodRate;

  alts_.insert(ecOpName);
}

#if 0  
void ESO2problem::setAlternatePartProdRate(
          const std::string & upLevelPart, 
          const std::string & downLevelPart, 
          const std::string & loc, 
          int period, 
          float prodRate )
{
  std::string ecOpName = alternatePartName(upLevelPart,downLevelPart,loc); 
  //std::string producedPart = partName(upLevelPart,loc);
  witSetArcAttribute(
    witGetBopEntryProductRate,
    witSetBopEntryProductRate,
    ecOpName, 0,
    period, prodRate );
}  

void ESO2problem::setAlternatePartCost(
          const std::string & upLevelPart, 
          const std::string & downLevelPart, 
          const std::string & loc, 
          int period, 
          float cost )
{
  std::string ecOpName = alternatePartName(upLevelPart,downLevelPart,loc);
  witSetNameAttribute(
    witGetOperationObj1ExecCost,
    witSetOperationObj1ExecCost,
    ecOpName,
    period, cost );
}
void ESO2problem::setAlternatePartUsageTime(
          const std::string & upLevelPart, 
          const std::string & downLevelPart, 
          const std::string & loc, 
          int period, 
          float usageTime )
{
  std::string ecOpName = alternatePartName(upLevelPart,downLevelPart,loc); 
  //std::string producedPart = partName(upLevelPart,loc);
  witSetArcAttribute(
    witGetBomEntryOffset,
    witSetBomEntryOffset,
    ecOpName, 0,
    period, usageTime );
} 

#endif

void ESO2problem::getAlts(
    std::vector<std::string> & partOrFeatures,
    std::vector<std::string> & altParts,
    std::vector<std::string> & locs ) const
{
  partOrFeatures.clear();
  altParts.clear();
  locs.clear();
  std::set<std::string>::const_iterator it;
  for ( it=alts_.begin(); it!=alts_.end(); ++it ) {
    altParts.push_back( altPartFromAltName(*it) );
    partOrFeatures.push_back( partFromAltName(*it) );
    locs.push_back( locFromAltName(*it) );
  }
}
std::set<std::string> ESO2problem::getAltPartOrFeature(const std::string & altPart,const std::string & loc) const
{
  std::set<std::string> retVal;
  std::set<std::string>::const_iterator it;
  for ( it=alts_.begin(); it!=alts_.end(); ++it ) {
    if ( altPart==altPartFromAltName(*it) && loc==locFromAltName(*it) )
      retVal.insert(partFromAltName(*it));
  }
  return retVal;
}


std::vector<float> ESO2problem::getAltVolWithOffset(
                                                 const std::string & partOrFeature,
                                                 const std::string & altPart, 
                                                 const std::string & loc )const
{
  // Wit ExecVol includes the time to demanufacture.
  // To get acquireQty, execVol needs to be shifted by bom offset
  std::string opName = altName(partOrFeature,altPart,loc);
#if 0
  std::vector<float> altVol = witGetNameAttribute(witGetOperationExecVol,opName);
  std::vector<float> offset = getAltConvTime(partOrFeature,altPart,loc );
  return shiftAttributeByOffset(altVol,offset);
#endif
  return getOffsetExecVol(opName);
}

std::vector<float> ESO2problem::getAltVol(
                                                 const std::string & partOrFeature,
                                                 const std::string & altPart, 
                                                 const std::string & loc )const
{
  std::string opName = altName(partOrFeature,altPart,loc);
  return witGetNameAttribute(witGetOperationExecVol,opName);
}

std::vector<float> ESO2problem::getAltConvCost(
                                                 const std::string & partOrFeature,
                                                 const std::string & altPart, 
                                                 const std::string & loc )const
{
  std::vector<float> retVal;
  std::string opName = altName(partOrFeature,altPart,loc);
  retVal = witGetNameAttribute(witGetOperationObj1ExecCost,opName);
  return retVal;
}
std::vector<float> ESO2problem::getAltConvTime(
                                                 const std::string & partOrFeature,
                                                 const std::string & altPart, 
                                                 const std::string & loc )const
{
  std::vector<float> retVal;
  std::string opName = altName(partOrFeature,altPart,loc); 
  
  int n;
  witGetOperationNBomEntries(mutableWitRun(),opName.c_str(),&n);
  assert(n==1 || n==0);

  if (n==0) {
    retVal = floatToStlVec(0.0f);
  } else {
    retVal = witGetArcAttribute(witGetBomEntryOffset,opName,0);
  }

  return retVal;
}

 
//----------------------
// partInterplant methods
//----------------------
bool ESO2problem::partInterplantExists(
                                  const std::string & part, 
                                  const std::string & srcLoc, 
                                  const std::string & destLoc )const
{
  std::string opName = partInterplantName(part,srcLoc,destLoc);
  witBoolean exists;
  witGetOperationExists(mutableWitRun(),opName.c_str(),&exists);
  bool retVal = false;
  if( exists ) retVal = true;
  return retVal;
}

void ESO2problem::addPartsInterplant(
                                     const std::string & srcLoc, 
                                     const std::string & destLoc, 
                                     float defaultMoveCost, 
                                     float defaultMoveTime )
{
  ESO2partInterplant pi(srcLoc,destLoc,defaultMoveCost,defaultMoveTime);
  defaultPartInterplants_[srcLoc].push_back(pi);
}

void ESO2problem::addPartInterplant(
                               const std::string & part, 
                               const std::string & srcLoc, 
                               const std::string & destLoc, 
                               float defaultMoveCost , 
                               float defaultMoveTime )
{
  assert( !partInterplantExists(part,srcLoc,destLoc) );
  std::string opName = partInterplantName(part,srcLoc,destLoc);  
  std::string srcPartName = moveablePartName(part,srcLoc); 
  std::string destPartName = partName(part,destLoc);

  // Create move operation and connect to src part
  witAddOperation(witRun(),opName.c_str());
  witAddBomEntry(witRun(),opName.c_str(),srcPartName.c_str());

  // if destination part doesn't exist, then add it to model.
  if ( !partExists(part,destLoc) ) {
    addPart(part,destLoc);
  }

  // Connect destination part to move operation  
  witAddBopEntry(witRun(),opName.c_str(),destPartName.c_str());

  // set default move cost
  float * moveCost = floatToConstFloatStar(defaultMoveCost);
  witSetOperationObj1ExecCost(witRun(),opName.c_str(),moveCost);
  delete [] moveCost;

  // set default move time
  float * moveTime = floatToConstFloatStar(defaultMoveTime);
  witSetBomEntryOffset(witRun(),opName.c_str(),0,moveTime );
  delete [] moveTime;

  partInterplants_.insert(opName);
}
void ESO2problem::setPartInterplantMoveCost(
          const std::string & part, 
          const std::string & srcLoc, 
          const std::string & destLoc, 
          int period, 
          float moveCost )
{
  std::string opName = partInterplantName(part,srcLoc,destLoc);
  witSetNameAttribute(
    witGetOperationObj1ExecCost,
    witSetOperationObj1ExecCost,
    opName,
    period, moveCost );
}
void ESO2problem::setPartInterplantMoveTime(
          const std::string & part, 
          const std::string & srcLoc, 
          const std::string & destLoc, 
          int period, 
          float moveTime )
{
  std::string opName = partInterplantName(part,srcLoc,destLoc); 
  witSetArcAttribute(
    witGetBomEntryOffset,
    witSetBomEntryOffset,
    opName, 0,
    period, moveTime );
} 

void ESO2problem::getPartInterplants( 
                    std::vector<std::string> & parts, 
                    std::vector<std::string> & srcLocs, 
                    std::vector<std::string> &destLocs ) const
{
  parts.clear();
  srcLocs.clear();
  destLocs.clear();
#if 0
  int nItems = partInterplants_.size();
  int n;
  for ( n=0; n<nItems; n++ ) {
    parts.push_back( partFromPartInterplantName(partInterplants_[n]) );
    srcLocs.push_back( srcLocFromPartInterplantName(partInterplants_[n]) );
    destLocs.push_back( destLocFromPartInterplantName(partInterplants_[n]) );
  }
#else
  std::set<std::string>::const_iterator it;
  for ( it=partInterplants_.begin(); it!=partInterplants_.end(); ++it ) {
    parts.push_back( partFromPartInterplantName(*it) );
    srcLocs.push_back( srcLocFromPartInterplantName(*it) );
    destLocs.push_back( destLocFromPartInterplantName(*it) );
  }
#endif
}

std::set<std::string> ESO2problem::getPartInterplantSrcLocs( const std::string & part, const std::string & destLoc ) const
{
  std::set<std::string> retVal;
  std::set<std::string>::const_iterator it;
  for ( it=partInterplants_.begin(); it!=partInterplants_.end(); ++it ) {
    if ( (partFromPartInterplantName(*it) == part ) && ( destLocFromPartInterplantName(*it) == destLoc ) )
      retVal.insert( srcLocFromPartInterplantName(*it) );
  }
  return retVal;
}

std::vector<float> ESO2problem::getPartInterplantMoveVolWithOffset(
                                        const std::string & part,
                                        const std::string & srcLoc, 
                                        const std::string & destLoc )const
{
#if 0
  std::vector<float> retVal;
  std::string opName = partInterplantName(part,srcLoc,destLoc);
  retVal = witGetNameAttribute(witGetOperationExecVol,opName);
  return retVal;
#endif

  // Wit ExecVol includes the time to move.
  // To get volume at start of move, then execVol needs to be shifted by bom offset
  std::string opName = partInterplantName(part,srcLoc,destLoc);
#if 0
  std::vector<float> execVol = witGetNameAttribute(witGetOperationExecVol,opName);
  std::vector<float> offset = getPartInterplantMoveTime(part, srcLoc,destLoc );
  return shiftAttributeByOffset(execVol,offset);
#endif
   return getOffsetExecVol(opName);
}

std::vector<float> ESO2problem::getPartInterplantMoveVol(
                                        const std::string & part,
                                        const std::string & srcLoc, 
                                        const std::string & destLoc )const
{
#if 1
  std::vector<float> retVal;
  std::string opName = partInterplantName(part,srcLoc,destLoc);
  retVal = witGetNameAttribute(witGetOperationExecVol,opName);
  return retVal;
#else

  // Wit ExecVol includes the time to move.
  // To get volume at start of move, then execVol needs to be shifted by bom offset
  std::string opName = partInterplantName(part,srcLoc,destLoc);
#if 0
  std::vector<float> execVol = witGetNameAttribute(witGetOperationExecVol,opName);
  std::vector<float> offset = getPartInterplantMoveTime(part, srcLoc,destLoc );
  return shiftAttributeByOffset(execVol,offset);
#endif
   return getOffsetExecVol(opName);
#endif
}

std::vector<float> ESO2problem::getOffsetExecVol(const std::string & witOpName) const
{
  std::vector<float> retVal;
  std::vector<float> execVol = witGetNameAttribute(witGetOperationExecVol,witOpName);

  int nBoms;
  witGetOperationNBomEntries(mutableWitRun(),witOpName.c_str(),&nBoms);

  if ( nBoms==0 ) {
    retVal = execVol;
  } else {
    assert(nBoms==1);
    std::vector<int> impPer = witGetArcAttribute(witGetBomEntryImpactPeriod,witOpName,0 );

    retVal=floatToStlVec(0.0f);
    for (int p=0;p<retVal.size(); ++p ){
      int ip = impPer[p];
      if ( ip == -1 ) continue;
      float ev = execVol[p];
      retVal[ ip ] = ev;
    }
  }
  return retVal;
}
std::vector<float> ESO2problem::getPartInterplantMoveCost(
                                        const std::string & part,
                                        const std::string & srcLoc, 
                                        const std::string & destLoc )const
{
  std::vector<float> retVal;
  std::string opName = partInterplantName(part,srcLoc,destLoc);
  retVal = witGetNameAttribute(witGetOperationObj1ExecCost,opName);
  return retVal;
}

std::vector<float> ESO2problem::getPartInterplantMoveTime(
                                        const std::string & part,
                                        const std::string & srcLoc, 
                                        const std::string & destLoc )const
{
  std::vector<float> retVal;
  std::string opName = partInterplantName(part,srcLoc,destLoc);
  
  int n;
  witGetOperationNBomEntries(mutableWitRun(),opName.c_str(),&n);
  assert(n==1 || n==0);

  if (n==0) {
    retVal = floatToStlVec(0.0f);
  } else {
    retVal = witGetArcAttribute(witGetBomEntryOffset,opName,0);
  }

  return retVal;
}


//----------------------
// partDemand methods
//----------------------
bool ESO2problem::partDemandExists(
                                  const std::string & part, 
                                  const std::string & custLoc, 
                                  const std::string & plantLoc )const
{
#if 0
  bool retVal=false;
  std::string dName = partDemandName(part,custLoc,plantLoc);
  std::string pName = partName(part,plantLoc);
  int nDems;
  char ** dNames;
  witGetPartDemands(mutableWitRun(),dName.c_str(),&nDems,&dNames);
  for ( int d=0; d<nDems; d++ ) {
    if ( dName==dNames[d] ) retVal = true;
    witFree(dNames[d]);
  }
  witFree(dNames);
  return retVal;
#endif
  // Since every demand has its own part, just have to see if part exists
  std::string dName = partDemandName(part,custLoc,plantLoc);
  witBoolean partExists;
  witGetPartExists(mutableWitRun(),dName.c_str(),&partExists);
  bool retVal = false;
  if( partExists) retVal = true;
  return retVal;
}

// Pegging information
std::set<std::string> ESO2problem::getPartDemandPeggedAttributeNames(
  const std::string & part, const std::string & custLoc, const std::string & plantLoc, int shipPeriod )const
{
  std::string dName = partDemandName(part,custLoc,plantLoc);
  ESO2partDemandAppData * appDataPtr;
  witGetDemandAppData(mutableWitRun(),dName.c_str(),dName.c_str(),(void**)&appDataPtr);
  return appDataPtr->getAttributeNames();
}
 
void ESO2problem::getPartDemandPegging(
                                       const std::string & peggedAttributeName,
                                       const std::string & part, const std::string & custLoc, const std::string & plantLoc, int shipPeriod,

                                       std::vector<std::string> & items, 
                                       std::vector<std::string> & locs, 
                                       std::vector<int> & periods, 
                                       std::vector<int> & depths, 
                                       std::vector<float> & quantities ) const
{
  std::string dName = partDemandName(part,custLoc,plantLoc);
  ESO2partDemandAppData * appDataPtr;
  witGetDemandAppData(mutableWitRun(),dName.c_str(),dName.c_str(),(void**)&appDataPtr);
  
  appDataPtr->getPeggedItems(peggedAttributeName,shipPeriod,items,locs,periods,depths,quantities);
}

#if 0
void ESO2problem::getPartDemandPegging(
    const std::string & part, 
    const std::string & custLoc, 
    const std::string & plantLoc, 
    int shipPeriod,

    std::vector<std::string> & mtms, 
    std::vector<std::string> & mtmSrcLocs, 
    std::vector<std::string> & mtmAcquireLocs, 
    std::vector<int> & mtmAcquirePeriods, 
    std::vector<float> & mtmAcquireQuantity,
    
    std::vector<std::string> & demanMtms,
    std::vector<std::string> & demanLocs,
    std::vector<int> & demanPeriods,
    std::vector<float> & demanQuantities,

    std::vector<std::string> & features, 
    std::vector<std::string> & featurePlantLocs, 
    std::vector<int> & featureAcquirePeriods, 
    std::vector<float> & featureAcquireQuantity,

    std::vector<std::string> & looseParts, 
    std::vector<std::string> & loosePartPlantLocs, 
    std::vector<int> & loosePartAcquirePeriods, 
    std::vector<float> & loosePartAcquireQuantity )
    const
{
  
  std::string dName = partDemandName(part,custLoc,plantLoc);
  ESO2partDemandAppData * appDataPtr;
  witGetDemandAppData(mutableWitRun(),dName.c_str(),dName.c_str(),(void**)&appDataPtr);

  // Get MTM pegging info
  {
    mtms.clear();
    mtmSrcLocs.clear();
    mtmAcquireLocs.clear();
    mtmAcquirePeriods.clear();
    mtmAcquireQuantity.clear();
    
    std::vector<std::string> witMtms;
    std::vector<int> periods;
    std::vector<float> quantities;
    appDataPtr->getPeggedItems("MtmAcquire",shipPeriod,witMtms,periods,quantities);
    
    int m;
    for( m=0; m<witMtms.size(); ++m ) {
      std::string t = mtmFromMtmName(witMtms[m].c_str());
      mtms.push_back(t);
      
      t=mtmSrcLocFromMtmName(witMtms[m].c_str());
      mtmSrcLocs.push_back(t);
      
      t=mtmDestLocFromMtmName(witMtms[m].c_str());
      mtmAcquireLocs.push_back(t);
      
      mtmAcquirePeriods.push_back(periods[m]);
      
      mtmAcquireQuantity.push_back(quantities[m]);
    }
  } // end get MTM pegging info

  
  // Get mtm deman info
  {
    demanMtms.clear();
    demanLocs.clear();
    demanPeriods.clear();
    demanQuantities.clear();
    
    std::vector<std::string> witDemanOps;
    std::vector<int> periods;
    std::vector<float> quantities;
    appDataPtr->getPeggedItems("MtmDisassemble",shipPeriod,witDemanOps,periods,quantities);
    
    int m;
    for( m=0; m<witDemanOps.size(); ++m ) {
      std::string t = mtmFromMtmName(witDemanOps[m].c_str());
      demanMtms.push_back(t);
      
      t=mtmDestLocFromMtmName(witDemanOps[m].c_str());
      demanLocs.push_back(t);
      
      demanPeriods.push_back(periods[m]);
      
      demanQuantities.push_back(quantities[m]);
    }
  } // end get mtm deman pegging info

    // Get feature pegging info
  {
    features.clear();
    featurePlantLocs.clear();
    featureAcquirePeriods.clear();
    featureAcquireQuantity.clear();
    
    std::vector<std::string> witFeatures;
    std::vector<int> periods;
    std::vector<float> quantities;
    appDataPtr->getPeggedItems("FeatureAcquire",shipPeriod,witFeatures,periods,quantities);
    
    int m;
    for( m=0; m<witFeatures.size(); ++m ) {
      std::string t = featureFromFeatureName(witFeatures[m].c_str());
      features.push_back(t);
      
      t=locFromFeatureName(witFeatures[m].c_str());
      featurePlantLocs.push_back(t);
      
      featureAcquirePeriods.push_back(periods[m]);
      
      featureAcquireQuantity.push_back(quantities[m]);
    }
  } // end get feature pegging info

  // Get loosePart pegging info
  {
    looseParts.clear();
    loosePartPlantLocs.clear();
    loosePartAcquirePeriods.clear();
    loosePartAcquireQuantity.clear();
    
    std::vector<std::string> witLooseParts;
    std::vector<int> periods;
    std::vector<float> quantities;
    appDataPtr->getPeggedItems("LoosePartAcquire",shipPeriod,witLooseParts,periods,quantities);
    
    int m;
    for( m=0; m<witLooseParts.size(); ++m ) {
      std::string t = loosePartFromLoosePartName(witLooseParts[m].c_str());
      looseParts.push_back(t);
      
      t=locFromLoosePartName(witLooseParts[m].c_str());
      loosePartPlantLocs.push_back(t);
      
      loosePartAcquirePeriods.push_back(periods[m]);
      
      loosePartAcquireQuantity.push_back(quantities[m]);
    }
  } // end get loosePart pegging info


}
#endif

void ESO2problem::reportPart(const std::string & witPartName, 
                             int period,
                             int recursionDepth,
                             float reqVol,
                             float reqVal ) const
{
  int np=getNPeriods();
  recursionDepth++;

  std::string indent="";
  int i;
  for (i=0; i<recursionDepth; i++ ) indent=indent+"   ";

  std::vector<float> consVol = witGetNameAttribute(witGetPartConsVol,witPartName);
  std::vector<float> excessVol = witGetNameAttribute(witGetPartExcessVol,witPartName);
  //std::vector<float> stockVol = witGetNameAttribute(witGetPartStockVol,witPartName);
  std::vector<float> supplyVol = witGetNameAttribute(witGetPartSupplyVol,witPartName);
  
  std::cout 
    <<indent.c_str() <<"----------------------------------" <<std::endl
    <<indent.c_str() <<"part name: "   <<witPartName.c_str() <<std::endl
    <<indent.c_str() <<"period: "   <<period <<std::endl
    <<indent.c_str() <<"requiredVol: "   <<reqVol <<std::endl
    <<indent.c_str() <<"value of satisfy requiredVol: "   <<reqVal <<std::endl
    <<indent.c_str() <<"consVol: "   <<consVol[period] <<std::endl
    <<indent.c_str() <<"excessVol: "     <<excessVol[period] <<std::endl
    //<<indent.c_str() <<"stockVol: "     <<excessVol[period] <<std::endl
    <<indent.c_str() <<"supplyVol: "     <<supplyVol[period] <<std::endl;

  int nProdBops;
  witGetPartNProducingBopEntries(mutableWitRun(),witPartName.c_str(),&nProdBops);

  int prodBop;
  for ( prodBop=0; prodBop<nProdBops; prodBop++ ) {
    int bopIndex;
    char * opName;
    witGetPartProducingBopEntry(mutableWitRun(),witPartName.c_str(),prodBop,&opName,&bopIndex);
    std::cout 
      <<indent.c_str() <<"producing op: "   <<opName <<std::endl;

    // Make sure bop offsets are zero
    std::vector<float> bopOffset = witGetArcAttribute(witGetBopEntryOffset,opName,bopIndex);
    assert(bopOffset.size()==np);
    for ( i=0; i<np; i++ ) 
      assert(bopOffset[i]==0);

    // get bop prod rate    
    std::vector<float> bopProdRate = witGetArcAttribute(witGetBopEntryProductRate,opName,bopIndex);

    // get operation attributes
    std::vector<float> obj1ExecCost = witGetNameAttribute(witGetOperationObj1ExecCost,opName);
    std::vector<float> execVol = witGetNameAttribute(witGetOperationExecVol,opName);

    std::vector<float> bomOffset = witGetArcAttribute(witGetBomEntryOffset,opName,0);
    std::vector<int> impPer = witGetArcAttribute(witGetBomEntryImpactPeriod,opName,0);
    std::vector<float> bomConsRate = witGetArcAttribute(witGetBomEntryConsRate,opName,0);
    std::vector<float> hlb,slb,hub;
    witGetNameBoundAttribute(witGetOperationExecBounds,opName,hlb,slb,hub);
    for ( i=0; i<np; i++ ) 
      assert(bomConsRate[i]==1.0);
    int newPer = impPer[period];

    std::cout 
      <<indent.c_str() <<"period: "   <<period <<std::endl
      <<indent.c_str() <<"bop prodRate: "   <<bopProdRate[period] <<std::endl      
      <<indent.c_str() <<"opName: "   <<opName <<std::endl
      <<indent.c_str() <<"operation obj1ExecCost: "   <<obj1ExecCost[period] <<std::endl
      <<indent.c_str() <<"operation execVol: "   <<execVol[period] <<std::endl
      <<indent.c_str() <<"operation exec hub: "   <<hub[period] <<std::endl
      <<indent.c_str() <<"bom offset: "   <<bomOffset[period] <<std::endl
      <<indent.c_str() <<"impact period: "   <<impPer[period] <<std::endl;

    // get consumed part
    char * consumedPart;
    witGetBomEntryConsumedPart(mutableWitRun(),opName,0,&consumedPart);

    // are there any subs
    int nSubBoms;
    witGetBomEntryNSubsBomEntries(mutableWitRun(),opName,0,&nSubBoms);
    assert( nSubBoms==0 || nSubBoms==1 );
    if ( nSubBoms==1 ) {
      std::vector<float> subOffset = witGetSubArcAttribute(witGetSubsBomEntryOffset,opName,0,0);
      std::vector<int> subImpPer = witGetSubArcAttribute(witGetSubsBomEntryImpactPeriod,opName,0,0);
      int earliestPeriod = witGetSubArcAttribute(witGetSubsBomEntryEarliestPeriod,opName,0,0);
      int latestPeriod = witGetSubArcAttribute(witGetSubsBomEntryLatestPeriod,opName,0,0);

      std::cout 
        <<indent.c_str() <<"subBom offset: "   <<subOffset[period] <<std::endl
        <<indent.c_str() <<"subImpact period: "   <<subImpPer[period] <<std::endl
        <<indent.c_str() <<"subEarliest period: "   <<earliestPeriod <<std::endl
        <<indent.c_str() <<"subLatest period: "   <<latestPeriod <<std::endl;
      
      if ( subImpPer[period]!=-1 ) {
        float newReqVol = reqVol/bopProdRate[period];
        float newReqVal = reqVal-obj1ExecCost[period]*newReqVol;
        reportPart(consumedPart,subImpPer[period],recursionDepth,newReqVol,newReqVal );
      }

    }

    if ( newPer == -1 ) {
      // can not consume part
      std::cout 
        <<indent.c_str() <<"part name: "   <<consumedPart <<std::endl;
    }
    else {
      // report on consumed part{
      float newReqVol = reqVol/bopProdRate[period];
      float newReqVal = reqVal-obj1ExecCost[period]*newReqVol;
      reportPart(consumedPart,newPer,recursionDepth, newReqVol, newReqVal );
    }

    witFree(consumedPart);
    witFree(opName);
  }  // for prodBop
}
void ESO2problem::reportPartDemand(const std::string & part, 
                      const std::string & custLoc, 
                      const std::string & plantLoc, 
                      int period ) const 
{ 
  std::string dName = partDemandName(part,custLoc,plantLoc);
  std::string regulatorName = partDemandRegulatorName(part,custLoc,plantLoc);
  std::string pName = partName(part,plantLoc);

  std::vector<float> shipVol = getPartDemandShipVol(part,custLoc,plantLoc);
  std::vector<float> demandVol = getPartDemandDemandVol(part,custLoc,plantLoc);
  std::vector<float> wac = getPartDemandWac(part,custLoc,plantLoc);

  std::cout 
    <<"----------------------------------" <<std::endl
    <<"demand name: " <<dName.c_str() <<std::endl
    <<"demanded part name: "   <<pName.c_str() <<std::endl
    <<"period: "   <<period <<std::endl
    <<"demandVol: "   <<demandVol[period] <<std::endl
    <<"shipVol: "     <<shipVol[period] <<std::endl
    <<"wac: "     <<wac[period] <<std::endl
    <<"wac*demandVol: "     <<wac[period]*demandVol[period] <<std::endl;
  if ( demandVol[period] == shipVol[period] ) {
    std::cout 
      <<"demand satisfied." <<std::endl ;
  }
  //else {
    reportPart(pName,period,0,demandVol[period],wac[period]*demandVol[period]);
  //}

  std::cout
      <<"----------------------------------" <<std::endl;

}

void ESO2problem::addPartDemand(
                               const std::string & part, 
                               const std::string & custLoc, 
                               const std::string & plantLoc)
{
  std::string dName = partDemandName(part,custLoc,plantLoc);
  std::string regulatorName = partDemandRegulatorName(part,custLoc,plantLoc);
  std::string pName = partName(part,plantLoc);

  // Add part and attach demand to it.
  addNonScrapableCapacity(dName.c_str());
  witAddDemand(witRun(),dName.c_str(),dName.c_str());

  // Add operation and connect to demanded part
  witAddOperation(witRun(),dName.c_str());
  witAddBopEntry(witRun(),dName.c_str(),dName.c_str());

  //-------------------------------------------
  // Add part to model inventory holding at demand location

  std::string inventoryPartName = partDemandInventoryName(part,custLoc,plantLoc);
  if ( getAllowInventoryHoldingAtDemandLocation() ) {
    witAddPart(witRun(), inventoryPartName.c_str(), WitMATERIAL );
  } else {
    witAddPart(witRun(), inventoryPartName.c_str(), WitCAPACITY );
  }
  witAddBomEntry(witRun(),dName.c_str(),inventoryPartName.c_str());

  witAddOperation(witRun(),inventoryPartName.c_str());
  witAddBopEntry(witRun(),inventoryPartName.c_str(),inventoryPartName.c_str());
  //-------------------------------------------

  // Attach existing part in model to operation
  witAddBomEntry(witRun(),/*dName.c_str()*/ inventoryPartName.c_str() ,pName.c_str());

  // Add regulator capacity and attach to operation.
  // This capacity is used to control demand perishability
  witAddPart(witRun(), regulatorName.c_str(), WitCAPACITY);
  witAddBomEntry(witRun(),dName.c_str(),regulatorName.c_str());

  // Set default app data to hold pegging information
  ESO2partDemandAppData * appData = new ESO2partDemandAppData(getNPeriods());
  witSetDemandAppData (mutableWitRun(), dName.c_str(),dName.c_str(), appData);

  partDemands_.insert(dName);
}

void ESO2problem::setPartDemandDemandVol(
                               const std::string & part, 
                               const std::string & custLoc, 
                               const std::string & plantLoc,
                               int period,
                               float demandVol)
{
  std::string dName = partDemandName(part,custLoc,plantLoc);
  std::string rName = partDemandRegulatorName(part,custLoc,plantLoc);
  witSetDemandAttribute(witGetDemandDemandVol,witSetDemandDemandVol,
    dName, dName, period, demandVol);

  // Set supplyVol of regulator capacity to enforce perishability of demand
  witSetNameAttribute(witGetPartSupplyVol,witSetPartSupplyVol,
    rName, period, demandVol);
}

void ESO2problem::setPartDemandWac(
                               const std::string & part, 
                               const std::string & custLoc, 
                               const std::string & plantLoc,
                               int period,
                               float wac)
{
  std::string dName = partDemandName(part,custLoc,plantLoc);
  std::string pName = partName(part,plantLoc);
#if 0
  witSetDemandAttribute(witGetDemandObj1ShipReward,witSetDemandObj1ShipReward,
    dName, dName, period, wac);
#endif
  ESO2partDemandAppData * appData;
  witGetDemandAppData(mutableWitRun(), dName.c_str(), dName.c_str(), (void**)&appData);
  appData->setWac(period,wac);
}

void ESO2problem::setPartDemandPriority(
                               const std::string & part, 
                               const std::string & custLoc, 
                               const std::string & plantLoc,
                               int priority)
{
  std::string dName = partDemandName(part,custLoc,plantLoc);
  std::string pName = partName(part,plantLoc);
  ESO2partDemandAppData * appData;
  witGetDemandAppData(mutableWitRun(), dName.c_str(), dName.c_str(), (void**)&appData);
  appData->setPriority(priority);
}

int ESO2problem::getPartDemandPriority(
                               const std::string & part, 
                               const std::string & custLoc, 
                               const std::string & plantLoc)const
{
  std::string dName = partDemandName(part,custLoc,plantLoc);
  std::string pName = partName(part,plantLoc);
  ESO2partDemandAppData * appData;
  witGetDemandAppData(mutableWitRun(), dName.c_str(), dName.c_str(), (void**)&appData);
  return appData->getPriority();
}

   
// Get list of all partDemands added
void ESO2problem::getPartDemands( 
                                 std::vector<std::string> & parts, 
                                 std::vector<std::string> & custLocs, 
                                 std::vector<std::string> &plantLocs ) const
{
  parts.clear();
  custLocs.clear();
  plantLocs.clear();
#if 0
  int nItems = partDemands_.size();
  int n;
  for ( n=0; n<nItems; n++ ) {
    parts.push_back( partFromPartDemandName(partDemands_[n]) );
    custLocs.push_back( custLocFromPartDemandName(partDemands_[n]) );
    plantLocs.push_back( plantLocFromPartDemandName(partDemands_[n]) );
  }
#else
  std::set<std::string>::const_iterator it;
  for ( it=partDemands_.begin(); it!=partDemands_.end(); ++it ) {
    parts.push_back( partFromPartDemandName(*it) );
    custLocs.push_back( custLocFromPartDemandName(*it) );
    plantLocs.push_back( plantLocFromPartDemandName(*it) );
  }
#endif
}
std::vector<float> ESO2problem::getPartDemandShipVol(
                                                     const std::string & part,
                                                     const std::string & custLoc, 
                                                     const std::string & plantLoc )const
{
  std::vector<float> retVal;
  std::string dName = partDemandName(part,custLoc,plantLoc);
  std::string pName = partName(part,plantLoc);
  retVal = witGetDemandAttribute(witGetDemandShipVol,dName,dName);
  return retVal;
}
std::vector<float> ESO2problem::getPartDemandDemandVol(
                                                     const std::string & part,
                                                     const std::string & custLoc, 
                                                     const std::string & plantLoc )const
{
  std::vector<float> retVal;
  std::string dName = partDemandName(part,custLoc,plantLoc);
  std::string pName = partName(part,plantLoc);
  retVal = witGetDemandAttribute(witGetDemandDemandVol,dName,dName);
  return retVal;
}
std::vector<float> ESO2problem::getPartDemandWac(
                                                     const std::string & part,
                                                     const std::string & custLoc, 
                                                     const std::string & plantLoc )const
{
  std::vector<float> retVal;
  std::string dName = partDemandName(part,custLoc,plantLoc);
  std::string pName = partName(part,plantLoc);
#if 0
  retVal = witGetDemandAttribute(witGetDemandObj1ShipReward,dName,dName);
#endif  
  ESO2partDemandAppData * appData;
  witGetDemandAppData(mutableWitRun(), dName.c_str(), dName.c_str(), (void**)&appData);
  retVal = appData->getWac();

  return retVal;
}
std::vector<float> ESO2problem::getPartDemandScaledWac(
                                                     const std::string & part,
                                                     const std::string & custLoc, 
                                                     const std::string & plantLoc )const
{
  std::vector<float> retVal = getPartDemandWac(part,custLoc,plantLoc);
  int demandPriority = getPartDemandPriority(part,custLoc,plantLoc);
  const std::vector<float> & wacScaleFactors = getWacScaleFactors();
  for (size_t t=0; t<retVal.size(); ++t ) 
    retVal[t]=retVal[t]*wacScaleFactors[demandPriority];
  return retVal;
}




// Label depth of Wit Objects to determine order that they are performed.
// This is important when they are done in the same period
void ESO2problem::labelObjectDepth(
    std::map<std::string,int> & operationDepths,
    std::map<std::string,int> & partDepths)
{
  operationDepths.clear();
  partDepths.clear();

  // Make sure wit is in preprocessed state
  witPreprocess(witRun());

  // Loop once for each WIT object and record its depth
  {
    witAttr objItrState;
    witGetObjItrState(witRun(), &objItrState);
    assert(objItrState==WitINACTIVE);
    for( int depth=0; depth=depth+10; )   {
      witAdvanceObjItr(witRun());
      witGetObjItrState(witRun(), &objItrState);
      if (objItrState==WitINACTIVE) break;
      if( objItrState == WitAT_OPERATION ) {
        char * operationName;
        witGetObjItrOperation(witRun(),&operationName);
        operationDepths[operationName] = depth;  
        witFree(operationName);
      }
      if( objItrState == WitAT_PART ) {
        char * partName;
        witGetObjItrPart(witRun(),&partName);
        partDepths[partName] = depth; 
        witFree(partName);
      }
    }
  }



}

void ESO2problem::updateActivePartList(const std::string & partName,std::set<std::string> &activeParts) const
{
	// if partName is already in list, just return
	if( activeParts.find(partName)!=activeParts.end() ) return;

	activeParts.insert(partName);

	// get number bop entries that produce this part
	int nProdBops;
    witGetPartNProducingBopEntries(mutableWitRun(),partName.c_str(),&nProdBops);

	// loop once for each bop 
    for ( int prodBop=0; prodBop<nProdBops; prodBop++ ) {

      // get operation name
      int bopIndex;
      char * opName;
      witGetPartProducingBopEntry(mutableWitRun(),partName.c_str(),prodBop,&opName,&bopIndex);

	  // get number of boms connected to operation
	  int nBoms;
	  witGetOperationNBomEntries(mutableWitRun(),opName,&nBoms);

	  // Loop once for each bom entries
	  for ( int bom=0; bom<nBoms; bom++ ) {
		  char * consumedPartName;
		  witGetBomEntryConsumedPart(mutableWitRun(),opName,bom,&consumedPartName);

		  // recursively call
		  updateActivePartList(consumedPartName,activeParts);
		  witFree(consumedPartName);
	  }

	  witFree(opName);
	}	
}
//-------------------------------------------------
// Try to remove cycles
//-------------------------------------------------
void ESO2problem::removeCycles()
{
  if ( !getRemoveCycles() ) return;
/*
WIT0531S The following explodeable cycle was found in the complete BOM
         structure:

WIT0532S Part part: 0000003N6612 at 980 is produced by operation part:
         0000003N6612 at 980.
         Operation part: 0000003N6612 at 980 consumes part moveablepart:
         0000003N6612 at 980.
WIT0532S Part moveablepart: 0000003N6612 at 980 is produced by operation ec:
         0000080P6949 to 0000003N6612 at 980.
         Operation ec: 0000080P6949 to 0000003N6612 at 980 consumes part part:
         0000080P6949 at 980.
WIT0532S Part part: 0000080P6949 at 980 is produced by operation part:
         0000080P6949 at 980.
         Operation part: 0000080P6949 at 980 consumes part moveablepart:
         0000080P6949 at 980.

WIT0532S Part moveablepart: 0000080P6949 at 980 is produced by operation ec:
         0000010N9534 to 0000080P6949 at 980.
         Operation ec: 0000010N9534 to 0000080P6949 at 980 consumes part part:
         0000010N9534 at 980.
WIT0532S Part part: 0000010N9534 at 980 is produced by operation part:
         0000010N9534 at 980.
         Operation part: 0000010N9534 at 980 consumes part moveablepart:
         0000010N9534 at 980.
WIT0532S Part moveablepart: 0000010N9534 at 980 is produced by operation ec:
         0000003N6612 to 0000010N9534 at 980.
         Operation ec: 0000003N6612 to 0000010N9534 at 980 consumes part part:
         0000003N6612 at 980.

WIT0533S Explodeable cycles in the complete BOM structure are not allowed.


WIT0090S WIT has encountered an error condition and is now terminating
         execution of the program.
         The return code is 3.


Produced Part: "part: 0000003N6612 at 980"
Operation:     "part: 0000003N6612 at 980"
Consumed Part: "moveablepart: 0000003N6612 at 980"

Produced Part: "moveablepart: 0000003N6612 at 980"
Operation:     "ec: 0000080P6949 to 0000003N6612 at 980"
Consumes Part: "part: 0000080P6949 at 980"

Produced Part: "part: 0000080P6949 at 980"
Operation:     "part: 0000080P6949 at 980"
Consumes Part: "moveablepart: 0000080P6949 at 980"

Produced Part: "moveablepart: 0000080P6949 at 980"
Operation:     "ec: 0000010N9534 to 0000080P6949 at 980"
Consumes Part: "part: 0000010N9534 at 980"

Produced Part: "0000010N9534 at 980"
Operation:     "part: 0000010N9534 at 980"
Consumes Part: "moveablepart: 0000010N9534 at 980"

Produced Part: "moveablepart: 0000010N9534 at 980"
Operation:     "ec: 0000003N6612 to 0000010N9534 at 980"
Consumes Part: "0000003N6612 at 980"


'part: 0000003N6612 at 980'         'part: 0000003N6612 at 980'
'moveablepart: 0000003N6612 at 980' 'ec: 0000080P6949 to 0000003N6612 at 980'
'part: 0000080P6949 at 980'         'part: 0000080P6949 at 980'
'moveablepart: 0000080P6949 at 980' 'ec: 0000010N9534 to 0000080P6949 at 980'
'part: 0000010N9534 at 980'         'part: 0000010N9534 at 980'
'moveablepart: 0000010N9534 at 980' 'ec: 0000003N6612 to 0000010N9534 at 980'
*/
  // Loop once for each cycle
  for (;;) {
    char **partNameList;
    char **operationNameList;
    int lenLists;
    witGetExpCycle( mutableWitRun(),&lenLists,&partNameList,&operationNameList);

    // if no more cycles then exit
    if ( lenLists==0 ) break;

    std::vector<std::string> altParts;
    std::vector<std::string> ecOps;
    std::cout <<std::endl <<"Cycle found: " <<std::endl;
    // Make sure cycle is a recognizabile pattern
    {
      bool expectedPattern=true;
      std::vector<std::string> partPrefix(2), opPrefix(2);
      partPrefix[0]=std::string("part: ");
      partPrefix[1]="moveablepart: ";
      opPrefix[0]="part: ";
      opPrefix[1]="ec: ";
      int startType=1;
      if ( std::string(partNameList[0]).find(partPrefix[0])==0 ) startType=0;
      for ( int i=0; i<lenLists; ++i ) {  
        //std::cout <<"  Produced Part: '" <<partNameList[i] <<"' by operation: '" <<operationNameList[i] <<"'" <<std::endl;
        
        if( std::string(partNameList[i]).find(partPrefix[(i%2)+startType])!=0 )
          expectedPattern=false;
        if( std::string(operationNameList[i]).find(opPrefix[(i%2)+startType])!=0 )
          expectedPattern=false;
        // Had better be from same location 
        if ( locFromPartName(partNameList[0]) != locFromPartName(partNameList[i]) )
          expectedPattern=false;
        if ( locFromPartName(partNameList[0]) != locFromAltName(operationNameList[i]))
          expectedPattern=false;
        
        if( std::string(partNameList[i]).find(partPrefix[0])==0 ) {
          altParts.push_back(partNameList[i]);        
          std::cout <<"  Produced Part: '" <<partNameList[i] <<std::endl;
        }
        if( std::string(operationNameList[i]).find(opPrefix[1])==0 )
          ecOps.push_back(operationNameList[i]);
          
      }
      if ( !expectedPattern )
        std::cout <<"Cycle of unrecognizable type found" <<std::endl;
      
      if(altParts.size()!=ecOps.size())
        std::cout <<"Cycle of unrecognizable type found" <<std::endl;
      if(altParts.size()*2!=lenLists)
        std::cout <<"Cycle of unrecognizable type found" <<std::endl;

      assert(altParts.size()==ecOps.size());
      assert(altParts.size()*2==lenLists);
      assert( expectedPattern );
    }

    // Create cycle breaking part
    std::string cbpn=cycleBreakingPartName(altParts);
    witAddPart(witRun(),cbpn.c_str(),WitMATERIAL);

    // Loop once for each ec operation.
    // Remove existing Bop and EC operation.
    // Create new EC Opearation and connect to cycle breaking part
    for ( int i=0; i<ecOps.size(); ++i ) {

      // Make sure only 1 bop
      int nBops;
      witGetOperationNBopEntries(mutableWitRun(),ecOps[i].c_str(),&nBops);
      assert(nBops=1);

      // create new ec operation if cycle is >2 parts
      std::string cbon;
      bool newEcOp;
      int cycBopIndex;
      if ( altParts.size()>2 ) {
        cbon=cycleBreakingOperationName(ecOps[i],altParts);
        witAddOperation(witRun(),cbon.c_str());
        witCopyOperationData(witRun(),cbon.c_str(), mutableWitRun(),ecOps[i].c_str());
        newEcOp=true;
        cycBopIndex=0;
        alts_.insert(cbon);
        alts_.erase(ecOps[i]);
      } else {
        cbon=ecOps[i];
        newEcOp=false;
        cycBopIndex=1;
      }

      //Make sure EC operation has some cost, so that it will not be an alternate optimal to using the orig part.
      std::vector<float> execCost=witGetNameAttribute(witGetOperationExecCost,cbon);
      for ( int t=0; t<execCost.size(); ++t ) {
        if (execCost[t]==0.0f) 
          execCost[t]=0.02;
      }
      witSetNameAttribute(witSetOperationExecCost,cbon,execCost);


      // Get prod rate of existing bop
      //float * prodRate;
      //witGetBopEntryProductRate(mutableWitRun(),ecOps[i].c_str(),0,&prodRate);

      // Add new bop and set attributes
      witAddBopEntry(witRun(),cbon.c_str(),cbpn.c_str());
      witCopyBopEntryData(witRun(),cbon.c_str(),cycBopIndex, mutableWitRun(),ecOps[i].c_str(),0);
 
      //witSetBopEntryProductRate(witRun(),ecOps[i].c_str(),1,prodRate);
      //witFree(prodRate);

      // Remove Operation & Bop which is part of the troublesome loop
      if ( newEcOp )
        witSetOperationSelForDel(witRun(),ecOps[i].c_str(),WitTRUE);
      witSetBopEntrySelForDel(witRun(),ecOps[i].c_str(),0,WitTRUE);
      witSetBopEntryExpAllowed(witRun(),ecOps[i].c_str(),0,WitFALSE);
      
    }

    // Loop once for each alt part in cycle
    for ( int i=0; i<altParts.size(); ++i ) {

      // Loop once for each bom consuming altParts[i]
      int numConsIndies;
      witGetPartNConsumingBomEntries(mutableWitRun(),altParts[i].c_str(),&numConsIndies);
      //std::cout <<"  " <<altParts[i] <<" used by:" <<std::endl;
      for ( int consIndex=0; consIndex<numConsIndies; ++consIndex ) {
        char * consumingOperationName;
        int bomEntryIndex;
        witGetPartConsumingBomEntry(mutableWitRun(),altParts[i].c_str(),consIndex,&consumingOperationName,&bomEntryIndex);
        std::string consOpName(consumingOperationName);
        witFree(consumingOperationName);

        if ( isAltName(consOpName) ) continue;
        //std::cout <<"    " <<consOpName <<" " <<bomEntryIndex <<std::endl;

        // Add BOM to cycleBreakingPartName
        int dupBomEntry;
        witGetOperationNBomEntries(mutableWitRun(),consOpName.c_str(),&dupBomEntry);
        witAddBomEntry(witRun(),consOpName.c_str(),cbpn.c_str());
 
        // Copy attributes to new bom Entry
        witCopyBomEntryData(witRun(),consOpName.c_str(),dupBomEntry, mutableWitRun(),consOpName.c_str(),bomEntryIndex);
      
      } // for ( int consIndex=0; consIndex<numConsIndies; ++consIndex )
  
    } //for ( int i=0; i<altParts.size(); ++i )


    // Free WIT allocated memory
    for ( int i=0; i<lenLists; ++i ) {
      witFree(partNameList[i]);
      witFree(operationNameList[i]);
    }
    witFree(partNameList);
    witFree(operationNameList);
  } //for (;;) {

  witPurgeData(witRun());

}

//-------------------------------------------------
// Prune Wit Model by removing object that will
// not participate in the solution
//-------------------------------------------------
void ESO2problem::pruneWitModel()
{
  
  // Cycles need to be removed before setting prod rates.
  removeCycles();

  // ensure prodRates are set before prunning
  setDemanBomAndFeatureXrefProdRates();

  std::set<std::string> activeParts;

  // Loop once for each demand and create set of parts
  // that can be used to satisfy the demand.
  {
    witAttr objItrState;
    witGetObjItrState(witRun(), &objItrState);
    assert(objItrState==WitINACTIVE);
    while( true )   {
      witAdvanceObjItr(witRun());
      witGetObjItrState(witRun(), &objItrState);
      if (objItrState==WitINACTIVE) break;
      if( objItrState == WitAT_DEMAND ) {
        char * demandedPartName;
        char * demandName;
        witGetObjItrDemand(witRun(),&demandedPartName,&demandName);
        
#if 0
        // Get list of all parts used by demand
        int lenList;
        char ** belowList;
        witGetPartBelowList(witRun(),demandedPartName,&lenList,&belowList);
        int b;
        for( b=0; b<lenList; b++ ) {
          // Add part to set of activeParts
          std::string activePart(belowList[b]);
          activeParts.insert(activePart);
          witFree(belowList[b]);
        }
        witFree(belowList);
#else
		// Update set of parts that can be used to meet this demand
		updateActivePartList(demandedPartName,activeParts);
#endif
        witFree(demandedPartName);
        witFree(demandName);
      }
    }
  }

  // Remove inactive partInterplants
  {
    std::vector<std::string> parts;
    std::vector<std::string> srcLocs;
    std::vector<std::string> destLocs;
    getPartInterplants( parts, srcLocs, destLocs);
    int p;
    for (p=0; p<parts.size(); ++p ) {
      std::string pName = partName(parts[p],destLocs[p]);
      std::string mpName = moveablePartName(parts[p],destLocs[p]);
      std::string opName = partInterplantName(parts[p],srcLocs[p],destLocs[p]);
      //std::string srcPName = partName(parts[p],destLocs[p]);
      // If part is not active, then delete
      if( activeParts.find(mpName)==activeParts.end() ) {
        //std::cout <<"Prunning: " <<opName.c_str() <<std::endl;

        witSetPartSelForDel(witRun(),pName.c_str(),WitTRUE);
        witSetPartSelForDel(witRun(),mpName.c_str(),WitTRUE);
        witSetOperationSelForDel(witRun(),opName.c_str(),WitTRUE);
        witSetOperationSelForDel(witRun(),pName.c_str(),WitTRUE);
        partInterplants_.erase(opName);

        removePartOrFeatureLoc(parts[p],destLocs[p]);
      }
    }
  }
  
  // Remove inactive featureXrefs
  {
    std::vector<std::string> parts;
    std::vector<std::string> features;
    std::vector<std::string> locs;
    getFeatureXrefs( parts, features, locs);
    int p;
    for (p=0; p<parts.size(); ++p ) {
      std::string pName = partName(parts[p],locs[p]);
      std::string mpName = moveablePartName(parts[p],locs[p]);
      std::string opName = featureXrefName(parts[p],features[p],locs[p]);
      // If part is not active, then delete
      if( activeParts.find(mpName)==activeParts.end() ) {
        //std::cout <<"Prunning: " <<pName.c_str() <<std::endl;

        witSetPartSelForDel(witRun(),pName.c_str(),WitTRUE);
        witSetPartSelForDel(witRun(),mpName.c_str(),WitTRUE);
        witSetOperationSelForDel(witRun(),opName.c_str(),WitTRUE);
        witSetOperationSelForDel(witRun(),pName.c_str(),WitTRUE);
        xrefFeatures_.erase(opName);

        removePartOrFeatureLoc(parts[p],locs[p]);
      }
    }
  }
  
  // Remove inactive looseParts
  {
    std::vector<std::string> parts;
    std::vector<std::string> locs;
    getLooseParts( parts, locs);
    int p;
    for (p=0; p<parts.size(); ++p ) {
      std::string pName = partName(parts[p],locs[p]);
      std::string mpName = moveablePartName(parts[p],locs[p]);
      std::string opName = acquireLoosePartName(parts[p],locs[p]);
      std::string igfPName = igfLoosePartName(parts[p],locs[p]);
      std::string baseName = baseLoosePartName(parts[p],locs[p]);
      // If part is not active, then delete
      if( activeParts.find(mpName)==activeParts.end() ) {
        //std::cout <<"Prunning: " <<pName.c_str() <<std::endl;

        witSetPartSelForDel(witRun(),pName.c_str(),WitTRUE);
        witSetPartSelForDel(witRun(),mpName.c_str(),WitTRUE);
        witSetOperationSelForDel(witRun(),opName.c_str(),WitTRUE);
        witSetOperationSelForDel(witRun(),pName.c_str(),WitTRUE);
        assert( activeParts.find(igfPName)==activeParts.end() );
        witSetPartSelForDel(witRun(),igfPName.c_str(),WitTRUE);
        loosePartNames_.erase(baseName);

        removePartOrFeatureLoc(parts[p],locs[p]);
      }
    }
  }
   
  // Remove inactive features
  {
    std::vector<std::string> features;
    std::vector<std::string> locs;
    getFeatures( features, locs);
    int p;
    for (p=0; p<features.size(); ++p ) {
      std::string pName = partName(features[p],locs[p]);
      std::string mpName = moveablePartName(features[p],locs[p]);
      std::string opName = acquireFeatureName(features[p],locs[p]);
      std::string igfFName = igfFeatureName(features[p],locs[p]);
      std::string baseName = baseFeatureName(features[p],locs[p]);
      // If part is not active, then delete
      if( activeParts.find(mpName)==activeParts.end() ) {
        //std::cout <<"Prunning: " <<pName.c_str() <<std::endl;

        witSetPartSelForDel(witRun(),pName.c_str(),WitTRUE);
        witSetPartSelForDel(witRun(),mpName.c_str(),WitTRUE);
        witSetOperationSelForDel(witRun(),opName.c_str(),WitTRUE);
        witSetOperationSelForDel(witRun(),pName.c_str(),WitTRUE);
        assert( activeParts.find(igfFName)==activeParts.end() );
        witSetPartSelForDel(witRun(),igfFName.c_str(),WitTRUE);
        featureNames_.erase(baseName);

        removePartOrFeatureLoc(features[p],locs[p]);
      }
    }
  }

#if 1
     
  // Remove inactive alts
  {
    std::vector<std::string> partOrFeatures;
    std::vector<std::string> altParts;
    std::vector<std::string> locs;
    getAlts( partOrFeatures, altParts, locs);
    int p;
    for (p=0; p<partOrFeatures.size(); ++p ) {
      
      std::string ecOpName = altName(partOrFeatures[p],altParts[p],locs[p]);   
      std::string upLevelPartName = partName(altParts[p],locs[p]); 
      std::string moveableUpLevelPartName = moveablePartName(altParts[p],locs[p]); 
      std::string downLevelPartName = partName(partOrFeatures[p],locs[p]);

      // If part is not active, then delete
      if( activeParts.find(upLevelPartName)==activeParts.end() ) {

        // Due the the cycle removing code upLevelPartName may be a pseudo part that represents
        // more than one part and may not be in the wit model.
        witBoolean partExists;
        witGetPartExists(mutableWitRun(),upLevelPartName.c_str(),&partExists);
        if ( partExists ) {
          //std::cout <<"Prunning: " <<upLevelPartName.c_str() <<std::endl;

          witSetPartSelForDel(witRun(),upLevelPartName.c_str(),WitTRUE);
 
          witSetOperationSelForDel(witRun(),upLevelPartName.c_str(),WitTRUE);

          removePartOrFeatureLoc(altParts[p],locs[p]);
        }
      }
    }
  }
#endif
   
  // Remove inactive mtm interplants
  {
    std::vector<std::string> mtms;
    std::vector<std::string> srcLocs;
    std::vector<std::string> destLocs;
    getMtmDemanLocs(mtms, srcLocs, destLocs );
    int p;
    for (p=0; p<mtms.size(); ++p ) {
      if (srcLocs[p] == destLocs[p] ) continue;
      std::string igfPName = igfMtmName(mtms[p],srcLocs[p] );
      std::string acquireOpName = acquireMtmName(mtms[p],srcLocs[p],destLocs[p] );
      std::string serverPName = serverMtmName(mtms[p],srcLocs[p],destLocs[p] );
      std::string demanOpName = demanMtmName(mtms[p],srcLocs[p],destLocs[p] );
      std::string baseName = baseMtmName(mtms[p],srcLocs[p] );
      // If part is not active, then delete
      if( activeParts.find(serverPName)==activeParts.end() ) { 
        //std::cout <<"Prunning: " <<demanOpName.c_str() <<std::endl;

        witSetPartSelForDel(witRun(),serverPName.c_str(),WitTRUE);
        witSetOperationSelForDel(witRun(),acquireOpName.c_str(),WitTRUE);
        witSetOperationSelForDel(witRun(),demanOpName.c_str(),WitTRUE);
        // Delete all parts birthed in demanufacturing
        int nBops;
        witGetOperationNBopEntries(witRun(),demanOpName.c_str(),&nBops);
        int b;
        for (b=0; b<nBops; ++b) {
          char * moveableProducedPart;
          witGetBopEntryProducedPart(witRun(),demanOpName.c_str(),b,&moveableProducedPart);

          //fprintf(pruneF,"witSetPartSelForDel\n%s\n",moveableProducedPart);
          witSetPartSelForDel(witRun(),moveableProducedPart,WitTRUE);

          char *op;
          int bei;
          witGetPartConsumingBomEntry(witRun(),moveableProducedPart,0,&op,&bei);
          assert(bei==0);

          //fprintf(pruneF,"witSetOperationSelForDel\n%s\n",op);
          witSetOperationSelForDel(witRun(),op,WitTRUE);   
          
          char * producedPart;
          witGetBopEntryProducedPart(witRun(),op,0,&producedPart);   
          //std::cout <<"Prunning: " <<producedPart <<std::endl;  
          
          witSetPartSelForDel(witRun(),producedPart,WitTRUE);

          assert(strcmp(op,producedPart)==0);
          witFree(op);
          witFree(producedPart);
          witFree(moveableProducedPart);
        }
        //if (srcLocs[p] != destLocs[p] ) {
          assert(mtmAcquireExists(baseName));
          assert(mtmAcquireLocs_[baseName].find(destLocs[p]) != mtmAcquireLocs_[baseName].end() );
          mtmAcquireLocs_[baseName].erase(destLocs[p]);
          if ( mtmAcquireLocs_[baseName].size() ==  0 ) {
            mtmAcquireLocs_.erase(baseName);
          }
        //}
        //else {
        //  // Is igf mtm inactive?
        //  if ( activeParts.find(igfPName)==activeParts.end() ) {
        //    witSetPartSelForDel(witRun(),igfPName.c_str(),WitTRUE);
        //    mtmNames_.erase(baseName);
        //  }
        //}
      }
    }
  }
  
  //std::cout <<"Calling witPurgeData" <<std::endl;
  witPurgeData(witRun());  
  //std::cout <<"Returned from calling witPurgeData" <<std::endl;
  

#if 0
  // Loop for each part and delete any part that is inactive
  {
    witAttr objItrState;
    witGetObjItrState(witRun(), &objItrState);
    assert(objItrState==WitINACTIVE);
    while( true )   {
      witAdvanceObjItr(witRun());
      witGetObjItrState(witRun(), &objItrState);
      if (objItrState==WitINACTIVE) break;

      if( objItrState == WitAT_PART ) {
        char * partName;
        witGetObjItrPart(witRun(),&partName);
        
        // If part is not active, then delete
        if( activeParts.find(partName)==activeParts.end() ) {
          std::cout <<"Inactive part: " <<partName <<std::endl;
          //witSetPartSelForDel(witRun(),partName,WitTRUE);
        }
        witFree(partName);
      }
      
      if( objItrState == WitAT_OPERATION ) {
        char * opName;
        witGetObjItrOperation(witRun(),&opName);
        
        int nBops;
        witGetOperationNBopEntries(witRun(),opName,&nBops);
        
        int nBoms;
        witGetOperationNBomEntries(witRun(),opName,&nBoms);

        if( nBops==0 || nBoms==0 ) {
          std::cout <<"Inactive Op: " <<opName <<std::endl;
          //witSetOperationSelForDel(witRun(),opName,WitTRUE);
        }
        
        witFree(opName);
      }
    }
    //witPurgeData(witRun());
  }

  
  std::cout "Completed pruning unused objects from model" <<std::endl;

#endif
}

// Set wit bounds and ensure that hlb<=slb<=hub
// This condition is not being met.
void ESO2problem::setExecBounds(
                                const std::string opName,
                                std::vector<float> hlb,
                                std::vector<float> slb,
                                std::vector<float> hub )
{
  CoinRelFltEq eq(1.e-7);
  int np = getNPeriods();
  int p;
  for (p=0; p<np; ++p) {
    if( eq( hlb[p],slb[p] ) ) slb[p] = hlb[p];
    if( eq( slb[p],hub[p] ) ) hub[p] = slb[p];
  }
  witSetNameBoundAttribute(witSetOperationExecBounds,opName, hlb,slb,hub);
}
//-------------------------------------------------
// Optimization methods
//-------------------------------------------------
void ESO2problem::solve()
{

  //writeWitData("eso2.wit");

  std::vector<float> zeroTv = floatToStlVec(0.0f);
  std::vector<float> nullTv;
  std::vector<float> infinityTv = floatToStlVec(FLT_MAX);


  // set scrapCost on parts produced from demanufacturing to be zero
  // This is so that mtm can be demanufactured and the resulting parts scrapped 
  // without penalty
#if 0
  std::vector<std::string> mtms,srcLocs,demanLocs;
  getMtmDemanLocs(mtms,srcLocs,demanLocs);
  int msdTriple;
  for (msdTriple=0; msdTriple<mtms.size(); msdTriple++) {
    std::vector<std::string> prodParts = getDemanBoms(mtms[msdTriple],srcLocs[msdTriple],demanLocs[msdTriple]);
    int pp;
    for ( pp=0; pp<prodParts.size(); pp++ ) {
      std::string pName = partName(prodParts[pp], demanLocs[msdTriple] );
      std::string moveablePName = moveablePartName(prodParts[pp], demanLocs[msdTriple] );
      witSetNameAttribute(witSetPartObj1ScrapCost,pName.c_str(),0.0f);
      witSetNameAttribute(witSetPartObj1ScrapCost,moveablePName.c_str(),0.0f);
    }
  }
#endif

  // set demanProdRates from attachRates
  setDemanBomAndFeatureXrefProdRates();
  //pruneWitModel();

  // Turn off all demands and set obj1ShipReward to Wac*wacScaleFactor
  {
    std::vector<std::string> parts, custLocs, plantLocs;
    getPartDemands( parts, custLocs, plantLocs );
    
    // loop once for each demand and set associated operations upper Exec Bounds to zero
    int d;
    for (d=0; d<parts.size(); ++d ) {
      std::string dName = partDemandName(parts[d],custLocs[d],plantLocs[d]);
      std::string inventoryName = partDemandInventoryName(parts[d],custLocs[d],plantLocs[d]);
      witSetNameBoundAttribute(witSetOperationExecBounds,dName, zeroTv,zeroTv,zeroTv);
      std::vector<float> scaledWac = getPartDemandScaledWac(parts[d],custLocs[d],plantLocs[d]);
      witSetDemandAttribute(witSetDemandObj1ShipReward, dName, dName, scaledWac);
      if ( getAllowInventoryHoldingAtDemandLocation() ) {
        float * inventCost = new float [getNPeriods()];
        for ( int t=0; t<getNPeriods(); ++t )
          inventCost[t] = scaledWac[t] * 0.000001;
        witSetPartStockCost(witRun(),inventoryName.c_str(),inventCost);
        delete [] inventCost;
      }
    } // end of (d=0; d<parts.size(); ++d )
  } // end of Turn off all demands

  //xx--------------------------------------------
  //set operations execBounds Hard Upper Bound to zero.
  //set execVol to zero
  //xx--------------------------------------------
  {  
    
    // update wfmv and execBounds for mtms
    { 
      std::vector<std::string> mtms;
      std::vector<std::string> plantLocs;
      getMtms(mtms,plantLocs);
      
      // loop once for each mtm
      int m;
      for ( m=0; m<mtms.size(); m++ ) {
        std::vector<float> wfmvVec = getMtmWfmv(mtms[m],plantLocs[m]);          
        std::string mtmBName = baseMtmName(mtms[m],plantLocs[m] );         
        std::string acquireOpName = acquireMtmName(mtms[m],plantLocs[m],plantLocs[m] );
        
        witSetNameAttribute(witSetOperationExecVol,acquireOpName, zeroTv); 
        witSetNameBoundAttribute(witSetOperationExecBounds,acquireOpName, zeroTv,zeroTv,zeroTv); 
        
        // are there any locations that this mtm is moved to?
        if ( mtmAcquireExists(mtmBName) ) {
          
          // Get all locations that this mtm can be moved to
          const std::set<std::string> & moveToLocs = getMtmAcquireLocs(mtmBName);
          
          // Loop once for each location this mtm can be moved to
          std::set<std::string>::const_iterator locIt;
          for(locIt=moveToLocs.begin(); locIt!=moveToLocs.end(); ++locIt ) {
            const std::string & destLoc = *locIt;
            std::vector<float> moveCost = getMtmInterplantMoveCost(mtms[m],plantLocs[m],destLoc);
            
            std::string acquireMtmWitName = acquireMtmName(mtms[m],plantLocs[m],destLoc );
            std::vector<int> impPer = witGetArcAttribute(witGetBomEntryImpactPeriod,acquireMtmWitName,0);
                       
            std::string acquireAndMoveOpName = acquireMtmName(mtms[m],plantLocs[m],destLoc );
            witSetNameAttribute(witSetOperationExecVol,acquireAndMoveOpName, zeroTv); 
            witSetNameBoundAttribute(witSetOperationExecBounds,acquireAndMoveOpName, zeroTv,zeroTv,zeroTv);
            
          } // end of locIt loop
          
        } // end of if (mtmAcquireExists(baseMtmName))
        
      } // end of loop m
      
    } // end of update for mtms
    
    // update execBounds for features
    {
      std::vector<std::string> features;
      std::vector<std::string> featureLocs;
      getFeatures(features,featureLocs);
      
      // loop once for each feature
      int f;
      for ( f=0; f<features.size(); f++ ) {
        std::vector<float> wfmvVec = getFeatureWfmv(features[f],featureLocs[f]);          
        std::string featureName = baseFeatureName(features[f],featureLocs[f] );         
        std::string acquireOpName = acquireFeatureName(features[f],featureLocs[f] );
        
        witSetNameBoundAttribute(witSetOperationExecBounds,acquireOpName, zeroTv,zeroTv,zeroTv);       
        
      } // end of loop f
      
    } // end of update for features
    
    
    // update execBounds for looseParts
    {
      std::vector<std::string> looseParts;
      std::vector<std::string> loosePartLocs;
      getLooseParts(looseParts,loosePartLocs);
      
      // loop once for each loosePart
      int l;
      for ( l=0; l<looseParts.size(); l++ ) {
        std::vector<float> wfmvVec = getLoosePartWfmv(looseParts[l],loosePartLocs[l]);          
        std::string loosePartName = baseLoosePartName(looseParts[l],loosePartLocs[l] );         
        std::string acquireOpName = acquireLoosePartName(looseParts[l],loosePartLocs[l] );
        
        witSetNameAttribute(witSetOperationExecVol,acquireOpName, zeroTv); 
        witSetNameBoundAttribute(witSetOperationExecBounds,acquireOpName, zeroTv,zeroTv,zeroTv);           
        
      } // end of loop l
      
    } // end of update for looseParts
  } // finished setting obj1ExecCost and execBounds
 
  
  //xx--------------------------------------------
  // finished setting operations execBounds Hard Upper Bound to zero.
  //xx--------------------------------------------
  
  // Loop once for each demand priority
  int dp;
  for ( dp=0; dp<2; ++dp ) {

  // Set obj1ExecCost of mtm, feature, and loosePart acquire operations 
  // to wfmv * wfmvScaleFactor.
  {    
    
    // update wfmv for mtms
    { 
      std::vector<std::string> mtms;
      std::vector<std::string> plantLocs;
      getMtms(mtms,plantLocs);
      
      // loop once for each mtm
      int m;
      for ( m=0; m<mtms.size(); m++ ) {
        std::vector<float> wfmvVec = getMtmWfmv(mtms[m],plantLocs[m]);          
        std::string mtmBName = baseMtmName(mtms[m],plantLocs[m] );         
        std::string acquireOpName = acquireMtmName(mtms[m],plantLocs[m],plantLocs[m] );
        
        float scaleFactor = getWfmvScaleFactor(dp);
        // If applying scale factor by location and location is not specified then use zero
        if ( getUseApplyScaleFactorLoc() && !getApplyLocWfmvScaleFactor(plantLocs[m],dp) ) scaleFactor = 0.0;
        
        // Set wfmv for aquiring mtm at its src location (without moving it)
        std::vector<float> scaledWfmvVec = wfmvVec;
        int p;
        for ( p=0; p<wfmvVec.size(); ++p ) {
          if ( useMtmPerishableSupply(mtms[m],plantLocs[m],plantLocs[m],p) ) {
            scaledWfmvVec[p] = scaleFactor*wfmvVec[0];
          }
          else {
            scaledWfmvVec[p] = scaleFactor*wfmvVec[p];
          }
        }
        witSetNameAttribute(witSetOperationObj1ExecCost,acquireOpName, scaledWfmvVec); 
        
        // are there any locations that this mtm is moved to?
        if ( mtmAcquireExists(mtmBName) ) {
          
          // Get all locations that this mtm can be moved to
          const std::set<std::string> & moveToLocs = getMtmAcquireLocs(mtmBName);
          
          // Loop once for each location this mtm can be moved to
          std::set<std::string>::const_iterator locIt;
          for(locIt=moveToLocs.begin(); locIt!=moveToLocs.end(); ++locIt ) {
            const std::string & destLoc = *locIt;
            std::vector<float> moveCost = getMtmInterplantMoveCost(mtms[m],plantLocs[m],destLoc);
            
            std::string acquireMtmWitName = acquireMtmName(mtms[m],plantLocs[m],destLoc );
            std::vector<int> impPer = witGetArcAttribute(witGetBomEntryImpactPeriod,acquireMtmWitName,0);
            
            // compute execCost. this is scaleFactor*wfmv + moveCost
            std::vector<float> scaledWfmvWithMoveCostVec = wfmvVec;
            for( p=0; p<moveCost.size(); ++p ) {
              if ( impPer[p]==-1 )
                scaledWfmvWithMoveCostVec[p] = ESO2problem::bigM();
              else
                if ( useMtmPerishableSupply(mtms[m],plantLocs[m],destLoc,p) ) {
                  scaledWfmvWithMoveCostVec[p] = scaleFactor*wfmvVec[0] + moveCost[p];
                }
                else {
                  scaledWfmvWithMoveCostVec[p] = scaleFactor*wfmvVec[impPer[p]] + moveCost[p];
                }
            }
            
            std::string acquireAndMoveOpName = acquireMtmName(mtms[m],plantLocs[m],destLoc );
            witSetNameAttribute(witSetOperationObj1ExecCost,acquireAndMoveOpName, scaledWfmvWithMoveCostVec);
            
          } // end of locIt loop
          
        } // end of if (mtmAcquireExists(baseMtmName))
        
      } // end of loop m
      
    } // end of update wfmv for mtms
    
    // update wfmv for features
    {
      std::vector<std::string> features;
      std::vector<std::string> featureLocs;
      getFeatures(features,featureLocs);
      
      // loop once for each feature
      int f;
      for ( f=0; f<features.size(); f++ ) {
        std::vector<float> wfmvVec = getFeatureWfmv(features[f],featureLocs[f]);          
        std::string featureName = baseFeatureName(features[f],featureLocs[f] );         
        std::string acquireOpName = acquireFeatureName(features[f],featureLocs[f] );

        float scaleFactor = getWfmvScaleFactor(dp);
        // If applying scale factor by location and location is not specified then use zero
        if ( getUseApplyScaleFactorLoc() && !getApplyLocWfmvScaleFactor(featureLocs[f],dp) ) scaleFactor = 0.0;
               
        // Set wfmv for aquiring feature
        int p;
        for ( p=0; p<wfmvVec.size(); ++p ) {
          if ( useFeaturePerishableSupply(features[f],featureLocs[f],p) ) {
            wfmvVec[p] = scaleFactor*wfmvVec[0];
          }
          else {
            wfmvVec[p] = scaleFactor*wfmvVec[p];
          }
        }
        witSetNameAttribute(witSetOperationObj1ExecCost,acquireOpName, wfmvVec);       
        
      } // end of loop f
      
    } // end of update wfmv for features
    
    
    // update wfmv for looseParts
    {
      std::vector<std::string> looseParts;
      std::vector<std::string> loosePartLocs;
      getLooseParts(looseParts,loosePartLocs);
      
      // loop once for each loosePart
      int l;
      for ( l=0; l<looseParts.size(); l++ ) {
        std::vector<float> wfmvVec = getLoosePartWfmv(looseParts[l],loosePartLocs[l]);          
        std::string loosePartName = baseLoosePartName(looseParts[l],loosePartLocs[l] );         
        std::string acquireOpName = acquireLoosePartName(looseParts[l],loosePartLocs[l] );
        
        float scaleFactor = getWfmvScaleFactor(dp);
        // If applying scale factor by location and location is not specified then use zero
        if ( getUseApplyScaleFactorLoc() && !getApplyLocWfmvScaleFactor(loosePartLocs[l],dp) ) scaleFactor = 0.0;
        
        
        // Set wfmv for aquiring loosePart
        int p;
        for ( p=0; p<wfmvVec.size(); ++p ) {
          if ( useLoosePartPerishableSupply(looseParts[l],loosePartLocs[l],p) ) {
            wfmvVec[p] = scaleFactor*wfmvVec[0];
          }
          else {
            wfmvVec[p] = scaleFactor*wfmvVec[p];
          }
        }
        witSetNameAttribute(witSetOperationObj1ExecCost,acquireOpName, wfmvVec);       
        
      } // end of loop l
      
    } // end of update wfmv for looseParts
  } // finished setting obj1ExecCost and execBounds



  
    
    // Turn on demands with demand priority equal to dp.
    // Set lower bound on all demands with demand priority <dp.
    {
      std::vector<std::string> parts, custLocs, plantLocs;
      getPartDemands( parts, custLocs, plantLocs );
      
      // loop once for each demand and set associated operations upper Exec Bounds to zero
      int d;
      for (d=0; d<parts.size(); ++d ) {
        std::string dName = partDemandName(parts[d],custLocs[d],plantLocs[d]);
        int demandPriority = getPartDemandPriority(parts[d],custLocs[d],plantLocs[d]);
        std::vector<float> execSlb = witGetNameAttribute(witGetOperationExecVol,dName);
        std::vector<float> execHub;
        if (demandPriority<=dp) execHub = infinityTv;
        else  execHub = execSlb;
        //witSetNameBoundAttribute(witSetOperationExecBounds,dName, zeroTv,execSlb,execHub);
        setExecBounds(dName,zeroTv,execSlb,execHub);
      } // end of (d=0; d<parts.size(); ++d )
    } // end of Turn on demand with priority eqaul to dp
    
    // Loop once for each supply priority
    int p;
    for ( p=1; p<4; ++p ) {
      
      // update execBounds for mtms
      { 
        std::vector<std::string> mtms;
        std::vector<std::string> plantLocs;
        getMtms(mtms,plantLocs);
        
        // loop once for each mtm
        int m;
        for ( m=0; m<mtms.size(); m++ ) {           
          std::string mtmBName = baseMtmName(mtms[m],plantLocs[m] );    
          std::string acquireOpName = acquireMtmName(mtms[m],plantLocs[m],plantLocs[m] );
          // Get execVol and set to softLower bound
          std::vector<float> execSlb = witGetNameAttribute(witGetOperationExecVol,acquireOpName);
          std::vector<float> execHub;
          if (getMtmPriority(dp)<=p) execHub = infinityTv;
          else  execHub = execSlb;
          //witSetNameBoundAttribute(witSetOperationExecBounds,acquireOpName, zeroTv,execSlb,execHub);  
          setExecBounds(acquireOpName, zeroTv,execSlb,execHub);  
          
          // are there any locations that this mtm is moved to?
          if ( mtmAcquireExists(mtmBName) ) {
            // Get all locations that this mtm can be moved to
            const std::set<std::string> & moveToLocs = getMtmAcquireLocs(mtmBName);
            // Loop once for each location this mtm can be moved to
            std::set<std::string>::const_iterator locIt;
            for(locIt=moveToLocs.begin(); locIt!=moveToLocs.end(); ++locIt ) {
              const std::string & destLoc = *locIt;
              std::string acquireAndMoveOpName = acquireMtmName(mtms[m],plantLocs[m],destLoc );       
              // Get execVol and set to softLower bound
              std::vector<float> execSlb = witGetNameAttribute(witGetOperationExecVol,acquireAndMoveOpName);
              std::vector<float> execHub;
              if (getMtmPriority(dp)<=p) execHub = infinityTv;
              else  execHub = execSlb;
              //witSetNameBoundAttribute(witSetOperationExecBounds,acquireAndMoveOpName, zeroTv,execSlb,execHub);
              setExecBounds(acquireAndMoveOpName, zeroTv,execSlb,execHub);            
            } // end of locIt loop
          } // end of if (mtmAcquireExists(baseMtmName))
        } // end of loop m      
      } // end of update execBounds for mtms
      
      // update execBounds for features
      {
        std::vector<std::string> features;
        std::vector<std::string> featureLocs;
        getFeatures(features,featureLocs);
        
        // loop once for each feature
        int f;
        for ( f=0; f<features.size(); f++ ) {      
          std::string acquireOpName = acquireFeatureName(features[f],featureLocs[f] );        
          // Get execVol and set to softLower bound
          std::vector<float> execSlb = witGetNameAttribute(witGetOperationExecVol,acquireOpName);
          std::vector<float> execHub;
          if (getFeaturePriority(dp)<=p) execHub = infinityTv;
          else  execHub = execSlb;
          //witSetNameBoundAttribute(witSetOperationExecBounds,acquireOpName, zeroTv,execSlb,execHub); 
          setExecBounds(acquireOpName, zeroTv,execSlb,execHub);            
        } // end of loop f      
      } // end of update execBounds for features
      
      
      // update execBounds for looseParts
      {
        std::vector<std::string> looseParts;
        std::vector<std::string> loosePartLocs;
        getLooseParts(looseParts,loosePartLocs);
        
        // loop once for each loosePart
        int l;
        for ( l=0; l<looseParts.size(); l++ ) {            
          std::string acquireOpName = acquireLoosePartName(looseParts[l],loosePartLocs[l] );  
          // Get execVol and set to softLower bound
          std::vector<float> execSlb = witGetNameAttribute(witGetOperationExecVol,acquireOpName);
          std::vector<float> execHub;
          if (getLoosePartPriority(dp)<=p) execHub = infinityTv;
          else  execHub = execSlb;
          //witSetNameBoundAttribute(witSetOperationExecBounds,acquireOpName, zeroTv,execSlb,execHub); 
          setExecBounds(acquireOpName, zeroTv,execSlb,execHub);                       
        } // end of loop l
      } // end of update execBounds for looseParts
      
#if 0
      // Write wit data file before invoking witOptImplode
      {
        char asChar[100];
        sprintf(asChar,"%d",implosionCount());
        std::string witDataFileName = std::string("implode")+std::string(asChar)+".wit";
        writeWitData(witDataFileName);
      }
#endif
      
      std::cout <<"Calling witOptImplode" <<std::endl;
      witOptImplode (witRun());
      implosionCount_++;
      std::cout <<"Returned from witOptImplode call" <<std::endl;
      //reportPartDemand("p2","pok","pok",0);
    } // end of p loop for each supply priority
  } // end of dp loop for each demand priority

  witBoolean postProcessed; 
  witGetPostprocessed(mutableWitRun(),&postProcessed);
  assert(postProcessed);

}

void ESO2problem::solveWithPegging() {
  std::cout <<"entered: ESO2problem::solveWithPegging()" <<std::endl;
  solve();
  doPegging();
}

//-------------------------------------------------
// Do pegging
//-------------------------------------------------
void ESO2problem::doPegging() {


#if 0
  // Is it only deman ops that have multiple bops?
  // Tests show that only demanOps have multiple bops
  {
    // Get all wit operations
    int nOps;
    char ** ops;
    witGetOperations(mutableWitRun(),&nOps,&ops);
    int o;
    for ( o=0; o<nOps; ++o ) {
      int nBops;
      witGetOperationNBopEntries(mutableWitRun(),ops[o],&nBops);
      if ( nBops> 1 ) 
        assert( isMtmDemanName(ops[o]) );
      witFree(ops[o]);
    }
    witFree(ops);
  }
#endif

  // Label depth of Wit Objects to determine order that they are performed.
  // This is important when they are done in the same period
  std::map<std::string,int> operationDepths;
  std::map<std::string,int> partDepths;
  labelObjectDepth(operationDepths,partDepths);


  // clear pegging info in partDemand appData which may be laying around from a prior call to solve
  {
    std::vector<std::string> parts;
    std::vector<std::string> custLocs;
    std::vector<std::string> plantLocs;
    getPartDemands( parts, custLocs, plantLocs);
    int p;
    for (p=0; p<parts.size(); ++p ) { 
      ESO2partDemandAppData * srcAppData;  
      std::string dName = partDemandName(parts[p],custLocs[p],plantLocs[p]);
      witGetDemandAppData(mutableWitRun(),dName.c_str(),dName.c_str(),(void**)&srcAppData);
      srcAppData->clearPegging();
    }
  }


  witBoolean postProcessed;
  witGetPostprocessed(mutableWitRun(),&postProcessed);
  assert(postProcessed);


  // Set the PIP sequence
  std::set<ESO2pipSequenceTriple,ESO2sortPipSequenceTriple> demandSet;
  {
    witClearPipSeq(witRun());

    // Loop once for each demanded part
    std::vector<std::string> demdParts, demdCustLocs, demdPlantLocs;
    getPartDemands( demdParts, demdCustLocs, demdPlantLocs );
    int d;
    for ( d=0; d<demdParts.size(); ++d ) {

      std::string dName = partDemandName(demdParts[d], demdCustLocs[d], demdPlantLocs[d] );
      std::vector<float> shipVol = witGetDemandAttribute(witGetDemandShipVol,dName,dName);
      std::vector<float> shipReward = witGetDemandAttribute(witGetDemandObj1ShipReward,dName,dName);

      // Loop once for each period
      int t;
      for ( t=0; t<shipVol.size(); ++t ) {
        if ( shipVol[t]>0.0 ) {
          // Determine value of this shipment
          float value = shipVol[t] * shipReward[t];

          // determine pip sequence by inserting demand in sorted list
          ESO2pipSequenceTriple seq(dName.c_str(),t,shipVol[t],value);
          demandSet.insert(seq);
        }
      } // end loop for each perid t
    } // end loop for each demanded part d


    // Loop through each pipSequenceTriple and add them to the pip sequence
    std::set<ESO2pipSequenceTriple,ESO2sortPipSequenceTriple>::const_iterator it;
    int nInserted = 0;
    for (it=demandSet.begin(); it!=demandSet.end(); ++it ) {
      std::string dn = it->getDemandName();
      int sp = it->getShipPeriod();
      float isv = it->getIncShipVol();
      float v = it->getValue();
      witAppendToPipSeq(witRun(),dn.c_str(),dn.c_str(),sp,isv);
      nInserted++;
    }
    assert( nInserted==demandSet.size() );
  } // end setting the PIP sequence

  // Have WIT compute pegging
  witBuildPip(witRun());

  // Extract pegging and store in member data 
  {     
    // Loop through each pipSequenceTriple 
    std::set<ESO2pipSequenceTriple,ESO2sortPipSequenceTriple>::const_iterator it;
    for (it=demandSet.begin(); it!=demandSet.end(); ++it ) {
      std::string dn = it->getDemandName();
      int sp = it->getShipPeriod();
      float isv = it->getIncShipVol();
      ESO2partDemandAppData * appDataPtr;
      witGetDemandAppData(mutableWitRun(),dn.c_str(),dn.c_str(),(void**)&appDataPtr);
      std::string plantLoc = plantLocFromPartDemandName(dn);

      // Since birthed parts can have multiple bops producing them, we need to 
      // ensure that these produced parts are only pegged once
      std::set< std::string > peggedDemanPart;

      // Get operations pegged to demand
      {
        int nOps;
        char ** ops;
        int * execPeriods;
        float * execVols;
        witGetDemandExecVolPip(mutableWitRun(),
          dn.c_str(),dn.c_str(),sp,
          &nOps,&ops,&execPeriods,&execVols);  

        // get produced parts pegged to demand
        std::map<std::string,std::vector<std::pair<int,float> > > peggedProdParts;
        {
          int nProdParts;
          char ** prodParts;
          int * prodPeriods;
          float * prodVols;
          /*witGetDemandExecVolPip*/witGetDemandProdVolPip(mutableWitRun(),
            dn.c_str(),dn.c_str(),sp,
            &nProdParts,&prodParts,&prodPeriods,&prodVols);
          for (int pp=0; pp<nProdParts; ++pp) {
            std::string prodPart(prodParts[pp]);
            std::pair<int,float> periodVolPair(prodPeriods[pp],prodVols[pp]);
            peggedProdParts[prodPart].push_back(periodVolPair);
            witFree(prodParts[pp]);
          }
          witFree(prodParts);
          witFree(prodPeriods);
          witFree(prodVols);
        }

        // loop once for each pegged operation
        int o;
        for ( o=0; o<nOps; ++o ) {
          if ( isAcquireMtmName(ops[o]) ) {  

            std::string mtm = mtmFromMtmName(ops[o]);
            std::string srcLoc = mtmSrcLocFromMtmName(ops[o]);
            std::string destLoc = mtmDestLocFromMtmName(ops[o]);
            // Need to shift period by acquire time.
            std::vector<int> impPer = witGetArcAttribute(witGetBomEntryImpactPeriod,ops[o],0);
            int shiftedPeriod = impPer[execPeriods[o]];
            int depth = operationDepths[ops[o]];
            appDataPtr->addPeggedItem("MtmAcquire",sp,mtm,srcLoc,shiftedPeriod,depth+0,execVols[o]); 
            appDataPtr->addPeggedItem("MtmMoveFrom",sp,mtm,srcLoc,shiftedPeriod,depth+1,execVols[o]); 
            appDataPtr->addPeggedItem("MtmMovedTo",sp,mtm,destLoc,execPeriods[o],depth+2,execVols[o]);         
          }
          else if ( isMtmDemanName(ops[o] ) ) {
            std::string mtm = mtmFromMtmName(ops[o]);
            std::string loc = mtmDestLocFromMtmName(ops[o]);
            // Need to shift period by deman time.
            std::vector<int> impPer = witGetArcAttribute(witGetBomEntryImpactPeriod,ops[o],0);
            int shiftedPeriod = impPer[execPeriods[o]];
            int depth = operationDepths[ops[o]];
            appDataPtr->addPeggedItem("MtmDisassemble",sp,mtm,loc,shiftedPeriod,depth,execVols[o]);

            // loop once for each Bop to find the pegged birthed part
            int nBops;
            witGetOperationNBopEntries(mutableWitRun(),ops[o],&nBops);
            for ( int b=0; b<nBops; ++b ) {
              // Get bops produced part
              char * witProdPart;
              witGetBopEntryProducedPart(mutableWitRun(),ops[o],b,&witProdPart);
              std::string prodPart = partFromPartName(witProdPart);
              std::string witProdPartAsStlString(witProdPart);
              witFree(witProdPart);

              // is current demand pegged to this produced part? If not, continue.
              if ( peggedProdParts.find(witProdPartAsStlString)==peggedProdParts.end() ) 
                continue;

              // has this produced part already been pegged to the current demand
              if ( peggedDemanPart.find(witProdPartAsStlString)!=peggedDemanPart.end() ) 
                continue;
              peggedDemanPart.insert(witProdPartAsStlString);

              // record produced part pegging in demand app data
              const std::vector<std::pair<int,float> > & vectorPeriodProdvolPairs =  peggedProdParts.find(witProdPartAsStlString)->second;
              for ( size_t ppp=0; ppp<vectorPeriodProdvolPairs.size(); ++ppp ) {
                int prodPeriod = vectorPeriodProdvolPairs[ppp].first;
                float prodVol = vectorPeriodProdvolPairs[ppp].second;
                appDataPtr->addPeggedItem("PartHarvestedFromMtm",sp,prodPart,loc,prodPeriod,depth+1,prodVol);
              }
            } // end of loop b for each bop
          }
          else if ( isAcquireFeatureName(ops[o]) ) {
            std::string feature = featureFromFeatureName(ops[o]);
            std::string loc = locFromFeatureName(ops[o]);              
            int depth = operationDepths[ops[o]];
            appDataPtr->addPeggedItem("FeatureAcquire",sp,feature,loc,execPeriods[o],depth,execVols[o]);
          }
          else if ( isFeatureXrefName(ops[o]) ) {
            std::string feature = featureFromFeatureXrefName(ops[o]);
            std::string loc = locFromFeatureXrefName(ops[o]);
            std::string part = partFromFeatureXrefName(ops[o]);
            // Need to shift period by xref time.
            std::vector<int> impPer = witGetArcAttribute(witGetBomEntryImpactPeriod,ops[o],0);
            int shiftedPeriod = impPer[execPeriods[o]];
            int depth = operationDepths[ops[o]];
            appDataPtr->addPeggedItem("FeatureXrefFrom",sp,feature,loc,shiftedPeriod,depth,execVols[o]);
            appDataPtr->addPeggedItem("PartXrefedTo",sp,part,loc,execPeriods[o],depth+1,execVols[o]);
          }
          else if ( isAcquireLoosePartName(ops[o]) ) {
            std::string lp = loosePartFromLoosePartName(ops[o]);
            std::string loc = locFromLoosePartName(ops[o]);
            int depth = operationDepths[ops[o]];
            appDataPtr->addPeggedItem("LoosePartAcquire",sp,lp,loc,execPeriods[o],depth,execVols[o]);
          }
          else if ( isAltName(ops[o]) ) {
            std::string fromPart = partFromAltName(ops[o]);
            std::string toPart = altPartFromAltName(ops[o]);
            std::string loc = locFromAltName(ops[o]);
            // Need to shift period by alt time.
            std::vector<int> impPer = witGetArcAttribute(witGetBomEntryImpactPeriod,ops[o],0);
            int shiftedPeriod = impPer[execPeriods[o]];
            int depth = operationDepths[ops[o]];
            appDataPtr->addPeggedItem("PartAltFrom",sp,fromPart,loc,shiftedPeriod,depth,execVols[o]);
            appDataPtr->addPeggedItem("PartAltTo",sp,toPart,loc,execPeriods[o],depth+1,execVols[o]);
          }
          else if ( isPartInterplantName(ops[o]) ) {
            std::string part = partFromPartInterplantName(ops[o]);
            std::string srcLoc = srcLocFromPartInterplantName(ops[o]);
            std::string destLoc = destLocFromPartInterplantName(ops[o]);
            // Need to shift period by acquire time.
            std::vector<int> impPer = witGetArcAttribute(witGetBomEntryImpactPeriod,ops[o],0);
            int shiftedPeriod = impPer[execPeriods[o]];
            bool skip=false;
            if ( 
              sp==shiftedPeriod &&
              sp==execPeriods[o] &&
              plantLoc == srcLoc &&
              plantLoc == destLoc ) 
              skip=true;
            if ( !skip ) {
              int depth = operationDepths[ops[o]];
              appDataPtr->addPeggedItem("PartMoveFrom",sp,part,srcLoc,shiftedPeriod,depth,execVols[o]); 
              appDataPtr->addPeggedItem("PartMovedTo",sp,part,destLoc,execPeriods[o],depth+1,execVols[o]);   
            }
          }
          witFree(ops[o]);
        }
        witFree(ops);
        witFree(execPeriods);
        witFree(execVols);
      } // end Get pegged operations
    } // end Loop through each pipSequenceTriple 
  }  // end Extract pegging and store in member data 

} // end Do pegging


//-------------------------------------------------------------------------
// mtm Name Methods
//-------------------------------------------------------------------------
std::string ESO2problem::igfMtmName(const std::string & mtm,const std::string & mtmSrcLoc )
{
  return "igf: "+baseMtmName(mtm,mtmSrcLoc);
}
std::string ESO2problem::acquireMtmName(const std::string & mtm,const std::string & mtmSrcLoc,const std::string & destLoc )
{
  return "acquire: "+baseMtmName(mtm,destLoc)+" from: "+mtmSrcLoc;
}
std::string ESO2problem::serverMtmName(const std::string & mtm,const std::string & mtmSrcLoc,const std::string & destLoc )
{
  return "server: "+baseMtmName(mtm,destLoc)+" from: "+mtmSrcLoc;
}
std::string ESO2problem::demanMtmName(const std::string & mtm,const std::string & mtmSrcLoc,const std::string & destLoc )
{
  return "deman: "+baseMtmName(mtm,destLoc)+" from: "+mtmSrcLoc;
}
std::string ESO2problem::baseMtmName(const std::string & mtm,const std::string & mtmDestLoc )
{
  return "mtm "+mtm+" at "+mtmDestLoc;
}
std::string ESO2problem::mtmFromMtmName(const std::string & mtmName)
{  
  return textBetween(mtmName,"mtm "," at ");
}
std::string ESO2problem::mtmDestLocFromMtmName(const std::string & mtmName)
{ 
  std::string retVal;
  if ( contains(mtmName," from: ") ) {
    retVal = textBetween(mtmName," at "," from: ");
  }
  else {
    retVal = textAfter(mtmName," at ");
  }
  return retVal;
}
std::string ESO2problem::mtmSrcLocFromMtmName(const std::string & mtmName)
{  
  return textAfter(mtmName," from: ");
}
bool ESO2problem::isMtmDemanName( const std::string & witName )
{
  bool correctPrefix = beginsWith(witName,"deman: ");
  bool hasFromText = contains(witName," from: ");
  bool retVal = false;
  if ( correctPrefix && hasFromText ) retVal = true;
  return retVal;
}
bool ESO2problem::isAcquireMtmName( const std::string & witName )
{
  bool correctPrefix = beginsWith(witName,"acquire: ");
  bool hasFromText = contains(witName," from: ");
  bool retVal = false;
  if ( correctPrefix && hasFromText ) retVal = true;
  return retVal;
}

//-------------------------------------------------------------------------
// feature Name Methods
//-------------------------------------------------------------------------
std::string ESO2problem::igfFeatureName(const std::string & feature,const std::string & loc )
{
  return "igf: "+baseFeatureName(feature,loc);
}
std::string ESO2problem::acquireFeatureName(const std::string & feature,const std::string & loc )
{
  return "acquire: "+baseFeatureName(feature,loc);
}
std::string ESO2problem::baseFeatureName(const std::string & feature,const std::string & loc )
{
  return "feature "+feature+" at "+loc;
}
std::string ESO2problem::featureFromFeatureName(const std::string & featureName)
{  
  return textBetween(featureName,"feature "," at ");
}
std::string ESO2problem::locFromFeatureName(const std::string & featureName)
{  
  return textAfter(featureName," at ");
}
bool ESO2problem::isAcquireFeatureName( const std::string & witName )
{
  bool correctPrefix = beginsWith(witName,"acquire: ");
  bool hasFeatureText = contains(witName,"feature ");
  bool hasAtText = contains(witName," at ");
  bool retVal = false;
  if ( correctPrefix && hasFeatureText && hasAtText ) retVal = true;
  return retVal;
}

//-------------------------------------------------------------------------
// loose part Name Methods
//-------------------------------------------------------------------------
std::string ESO2problem::igfLoosePartName(const std::string & loosePart,const std::string & loc )
{
  return "igf: "+baseLoosePartName(loosePart,loc);
}
std::string ESO2problem::acquireLoosePartName(const std::string & loosePart,const std::string & loc )
{
  return "acquire: "+baseLoosePartName(loosePart,loc);
}
std::string ESO2problem::baseLoosePartName(const std::string & loosePart,const std::string & loc )
{
  return "loosePart "+loosePart+" at "+loc;
}
std::string ESO2problem::loosePartFromLoosePartName(const std::string & loosePartName)
{  
  return textBetween(loosePartName,"loosePart "," at ");
}
std::string ESO2problem::locFromLoosePartName(const std::string & loosePartName)
{  
  return textAfter(loosePartName," at ");
}
bool ESO2problem::isAcquireLoosePartName( const std::string & witName )
{
  bool correctPrefix = beginsWith(witName,"acquire: ");
  bool hasLoosePartText = contains(witName,"loosePart ");
  bool hasAtText = contains(witName," at ");
  bool retVal = false;
  if ( correctPrefix && hasLoosePartText && hasAtText ) retVal = true;
  return retVal;
}


bool ESO2problem::isAcquireName( const std::string & witName )
{
  bool retVal = 
    isAcquireMtmName(witName) || 
    isAcquireFeatureName(witName) || 
    isAcquireLoosePartName(witName);
  return retVal;
}

//-------------------------------------------------------------------------
// Feature Cross Reference Name Methods
//-------------------------------------------------------------------------
std::string ESO2problem::featureXrefName(const std::string & part, const std::string & feature, const std::string & loc)
{
  return "xref: "+feature+" to "+part+" at "+loc;
}
std::string ESO2problem::partFromFeatureXrefName(const std::string & featXrefName)
{ 
  return textBetween(featXrefName," to "," at ");
}
std::string ESO2problem::featureFromFeatureXrefName(const std::string & featXrefName)
{   
  return textBetween(featXrefName,"xref: "," to ");
}
std::string ESO2problem::locFromFeatureXrefName(const std::string & featXrefName)
{  
  return textAfter(featXrefName," at ");
}
bool ESO2problem::isFeatureXrefName( const std::string & witName )
{
  bool correctPrefix = beginsWith(witName,"xref: ");
  bool hasToText = contains(witName," to ");
  bool hasAtText = contains(witName," at ");
  bool retVal = false;
  if ( correctPrefix && hasToText && hasAtText ) retVal = true;
  return retVal;
}

//-------------------------------------------------------------------------
// etnCsp Name Methods
//-------------------------------------------------------------------------
std::string ESO2problem::altName(const std::string & partOrFeature, const std::string & altPart, const std::string & loc)
{
  return "ec: "+partOrFeature+" to "+altPart+" at "+loc;
}
std::string ESO2problem::partFromAltName(const std::string & altName)
{  
  return textBetween(altName,"ec: "," to ");
}
std::string ESO2problem::altPartFromAltName(const std::string & altName)
{  
  return textBetween(altName," to "," at ");
}
std::string ESO2problem::locFromAltName(const std::string & altName)
{  
  return textAfter(altName," at ");
}
bool ESO2problem::isAltName( const std::string & witName )
{
  bool correctPrefix = beginsWith(witName,"ec: ");
  bool hasToText = contains(witName," to ");
  bool hasAtText = contains(witName," at ");
  bool retVal = false;
  if ( correctPrefix && hasToText && hasAtText ) retVal = true;
  return retVal;
}
//-------------------------------------------------------------------------
// partInterplant Name Methods
//-------------------------------------------------------------------------
std::string ESO2problem::partInterplantName(const std::string & part, const std::string & srcLoc, const std::string & destLoc)
{
  return "move: "+part+" from "+srcLoc+" to "+destLoc;
}
std::string ESO2problem::partFromPartInterplantName(const std::string & partInterplantName)
{  
  return textBetween(partInterplantName,"move: "," from ");
}
std::string ESO2problem::srcLocFromPartInterplantName(const std::string & partInterplantName)
{  
  return textBetween(partInterplantName," from "," to ");
}
std::string ESO2problem::destLocFromPartInterplantName(const std::string & partInterplantName)
{  
  return textAfter(partInterplantName," to ");
}
bool ESO2problem::isPartInterplantName( const std::string & witName )
{
  bool correctPrefix = beginsWith(witName,"move: ");
  bool hasFromText = contains(witName," from ");
  bool hasToText = contains(witName," to ");
  bool retVal = false;
  if ( correctPrefix && hasFromText && hasToText ) retVal = true;
  return retVal;
}
//-------------------------------------------------------------------------
// partDemand Name Methods
//-------------------------------------------------------------------------
std::string ESO2problem::partDemandName(const std::string & part, const std::string & custLoc, const std::string & plantLoc)
{
  return "part: "+part+" custLoc: "+custLoc+" plantLoc: "+plantLoc;;
}
std::string ESO2problem::partDemandRegulatorName(const std::string & part, const std::string & custLoc, const std::string & plantLoc)
{
  return "regulator for: "+partDemandName(part,custLoc,plantLoc);
}
std::string ESO2problem::partDemandInventoryName(const std::string & part, const std::string & custLoc, const std::string & plantLoc)
{
  return "Inventoryable Part for: "+partDemandName(part,custLoc,plantLoc);
}
std::string ESO2problem::partFromPartDemandName(const std::string & partDemandName)
{  
  return textBetween(partDemandName,"part: "," custLoc: ");
}
std::string ESO2problem::custLocFromPartDemandName(const std::string & partDemandName)
{  
  return textBetween(partDemandName," custLoc: "," plantLoc: ");
}
std::string ESO2problem::plantLocFromPartDemandName(const std::string & partDemandName)
{  
  return textAfter(partDemandName," plantLoc: ");
}


//-------------------------------------------------------------------------
// part Name Methods
//-------------------------------------------------------------------------
std::string ESO2problem::partName(const std::string & part,const std::string & loc )
{
  return "part: "+part+" at "+loc;
}

std::string ESO2problem::moveablePartName(const std::string & part,const std::string & loc )
{
  return "moveable"+partName(part,loc);
}

std::string ESO2problem::partFromPartName( const std::string & partName )
{
  return textBetween(partName, "part: ", " at " );
}
std::string ESO2problem::locFromPartName( const std::string & partName )
{
  return textAfter(partName," at " );
}



//-------------------------------------------------------------------------
// Cycle Breaking Name Methods
//-------------------------------------------------------------------------
std::string ESO2problem::cycleBreakingPartName(const std::vector<std::string> & partNames)
{
  std::string retVal="Cycle Breaking Part: ";
  retVal += "|"+partFromPartName(partNames[0])+"|";
  for ( int i=1; i<partNames.size(); ++i ) {
    retVal += " or ";
    retVal += "|"+partFromPartName(partNames[i])+"|";
  }
  retVal += " at " + locFromPartName(partNames[0]);
  return retVal;
}

std::string ESO2problem::cycleBreakingOperationName(const std::string & ecOpNameInCycle, const std::vector<std::string> & partNamesInCycle)
{
  std::string fromPart=partFromAltName(ecOpNameInCycle);
  std::string loc = locFromAltName(ecOpNameInCycle);

  // Find fromPart in list of parts in cycle
  int fp;
  for ( fp=0; fp<partNamesInCycle.size(); fp++ ) {
    if(fromPart==partFromPartName(partNamesInCycle[fp])) 
      break;
  }
  // assert that fromPart was found in list
  assert(fp<partNamesInCycle.size());

  std::string uplevelPartNames="";
  for ( int i=1; i<partNamesInCycle.size(); i++ ) {
    int npi=(fp+i)%partNamesInCycle.size();
    uplevelPartNames+=partFromPartName(partNamesInCycle[npi]);
    if ( i==(partNamesInCycle.size()-1)  ) break;
    uplevelPartNames+= " / ";
  }

  std::string retVal=altName(fromPart, uplevelPartNames, loc);

  return retVal;

}

//-------------------------------------------------------------------------
// text utilities Methods
//-------------------------------------------------------------------------
std::string ESO2problem::textBetween(
                        const std::string & src, 
                        const std::string & begMark,
                        const std::string & endMark)
{
  size_t begPos = src.find(begMark);
  assert( begPos != std::string::npos );
  begPos = begPos+begMark.size();

  size_t endPos = src.find(endMark);
  assert( endPos != std::string::npos );

  size_t len = endPos-begPos;
  std::string retVal = src.substr(begPos,len);
  return retVal;
}
std::string ESO2problem::textAfter(
                        const std::string & src, 
                        const std::string & begMark)
{
  size_t begPos = src.find(begMark);
  assert( begPos != std::string::npos );
  begPos = begPos+begMark.size();

  size_t endPos = src.size();

  size_t len = endPos-begPos;
  std::string retVal = src.substr(begPos,len);
  return retVal;
}
bool ESO2problem::beginsWith(
                        const std::string & src, 
                        const std::string & begString)
{
  size_t begPos = src.find(begString);
  if ( begPos==0 ) return true;
  else return false;
}
bool ESO2problem::contains(
                        const std::string & haystack, 
                        const std::string & needle)
{
  size_t pos = haystack.find(needle);
  if ( pos!=std::string::npos ) return true;
  else return false;
}



//-------------------------------------------------------------------------
// Utilities for invoking Wit Methods
//-------------------------------------------------------------------------
// Set element of Part/Operation float* attribute, given period and float
void ESO2problem::witSetNameAttribute(
    witGetNameFloatStarStar witGetFunc,
    witSetNameFloatStar     witSetFunc,
    const std::string & name,
    int period, 
    float flt )    
{ 
  assert( period>= 0 );
  assert( period<getNPeriods() );
  float * floatStar;
  witGetFunc(mutableWitRun(),name.c_str(),&floatStar);
  floatStar[period] = flt;
  witSetFunc(witRun(),name.c_str(),floatStar);
  witFree(floatStar);
}

// Set Part/Operation float* attribute, given std::vector<float>
void ESO2problem::witSetNameAttribute(
    witSetNameFloatStar     witSetFunc,
    const std::string & name,
    const std::vector<float> & value )
{
  float * fltStar = stlVecToFloatStar(value);
  witSetFunc(witRun(),name.c_str(),fltStar);
  delete [] fltStar;
}

// Set Part/Operation float* attribute, given float
void ESO2problem::witSetNameAttribute(
    witSetNameFloatStar     witSetFunc,
    const std::string & name,
    float value )
{
  float * fltStar = floatToConstFloatStar(value);
  witSetFunc(witRun(),name.c_str(),fltStar);
  delete [] fltStar;
}

// Set Part/Operation bound attribute, given three std::vector<float> vectors
void ESO2problem::witSetNameBoundAttribute(
    witSetName3FloatStar     witSetFunc,
    const std::string & name,
    const std::vector<float> & hlb,
    const std::vector<float> & slb,
    const std::vector<float> & hub )
{
  float * hlbFltStar = stlVecToFloatStar(hlb);
  float * slbFltStar = stlVecToFloatStar(slb);
  float * hubFltStar = stlVecToFloatStar(hub);
  witSetFunc(witRun(),name.c_str(),hlbFltStar, slbFltStar, hubFltStar);
  delete [] hlbFltStar;
  delete [] slbFltStar;
  delete [] hubFltStar;
}
// Get Part/Operation bound attributes, given three std::vector<float> vectors
void ESO2problem::witGetNameBoundAttribute(
    witGetName3FloatStar     witGetFunc,
    const std::string & name,
    std::vector<float> & hlb,
    std::vector<float> & slb,
    std::vector<float> & hub )const
{
  float * hlbFltStar;
  float * slbFltStar;
  float * hubFltStar;
  witGetFunc(mutableWitRun(),name.c_str(),&hlbFltStar, &slbFltStar, &hubFltStar);
  hlb=witFloatStarToStlVec(hlbFltStar);
  slb=witFloatStarToStlVec(slbFltStar);
  hub=witFloatStarToStlVec(hubFltStar);
  witFree(hlbFltStar);
  witFree(slbFltStar);
  witFree(hubFltStar);
}



// Set element of Bop/Bop Entry float* attribute, given period and float
void ESO2problem::witSetArcAttribute(
    witGetNameIndexFloatStarStar witGetFunc,
    witSetNameIndexFloatStar     witSetFunc,
    const std::string & opName,
    int index,
    int period, 
    float flt )    
{ 
  assert( period>= 0 );
  assert( period<getNPeriods() );
  //int bei = getBopEntryIndex(opName,partName);
  //assert( bei!=-1 );
  float * floatStar;
  witGetFunc(mutableWitRun(),opName.c_str(),index,&floatStar);
  floatStar[period] = flt;
  witSetFunc(witRun(),opName.c_str(),index,floatStar);
  witFree(floatStar);
}

// Set element of Bop/Bop Entry float* attribute, given std::vector<float>
void ESO2problem::witSetArcAttribute(
    witSetNameIndexFloatStar     witSetFunc,
    const std::string & opName,
    int index,
    const std::vector<float> & value )    
{ 
  float * fltStar = stlVecToFloatStar(value);
  witSetFunc(witRun(),opName.c_str(),index,fltStar);
  delete []  fltStar;
}

// Set element of Bop/Bop Entry float* attribute, given float
void ESO2problem::witSetArcAttribute(
    witSetNameIndexFloatStar     witSetFunc,
    const std::string & opName,
    int index,
    float value )    
{ 
  float * fltStar = floatToConstFloatStar(value);
  witSetFunc(witRun(),opName.c_str(),index,fltStar);
  delete []  fltStar;
}



// Set element on demand float* attribute, given period and fltValue
void ESO2problem::witSetDemandAttribute(
    witGetDblNameFloatStarStar      witGetFunc,
    witSetDblNameFloatStar          witSetFunc,
    const std::string & partName,
    const std::string & demandName,
    int period, 
    float flt )
{
  assert( period>= 0 );
  assert( period<getNPeriods() );
  float * floatStar;
  witGetFunc(mutableWitRun(),partName.c_str(),demandName.c_str(),&floatStar);
  floatStar[period] = flt;
  witSetFunc(witRun(),partName.c_str(),demandName.c_str(),floatStar);
  witFree(floatStar);
}

// Set element on demand float* attribute, given stl::vector<float>
void ESO2problem::witSetDemandAttribute(
    witSetDblNameFloatStar          witSetFunc,
    const std::string & partName,
    const std::string & demandName,
    const std::vector<float> & fltVec )
{
  float * floatStar = stlVecToFloatStar(fltVec);
  witSetFunc(witRun(),partName.c_str(),demandName.c_str(),floatStar);
  delete [] floatStar;
}

// Get vector of wit part/Operation float* attribute
std::vector<float> ESO2problem::witGetNameAttribute(
    witGetNameFloatStarStar witGetFunc,
    const std::string & name ) const
{
  float * attr;
  witGetFunc(mutableWitRun(),name.c_str(),&attr);
  std::vector<float> retVal = witFloatStarToStlVec(attr);
  witFree(attr);
  return retVal;
}

// Get vector of bom/bop float* attribute
std::vector<float> ESO2problem::witGetArcAttribute(
                                     witGetNameIndexFloatStarStar     witGetFunc,
                                     const std::string & opName,
                                     int index )const
{
  float * attr;
  witGetFunc(mutableWitRun(),opName.c_str(),index,&attr);
  std::vector<float> retVal = witFloatStarToStlVec(attr);
  witFree(attr);
  return retVal;
}

// Get vector of bom/bop int* attribute
std::vector<int> ESO2problem::witGetArcAttribute(
                                     witGetNameIndexIntStarStar     witGetFunc,
                                     const std::string & opName,
                                     int index )const
{
  int * attr;
  witGetFunc(mutableWitRun(),opName.c_str(),index,&attr);
  std::vector<int> retVal = witIntStarToStlVec(attr);
  witFree(attr);
  return retVal;
}



// Get vector of sub float* attribute
std::vector<float> ESO2problem::witGetSubArcAttribute(
                                     witGetNameIndexIndexFloatStarStar     witGetFunc,
                                     const std::string & opName,
                                     int index, int subIndex )const
{
  float * attr;
  witGetFunc(mutableWitRun(),opName.c_str(),index,subIndex,&attr);
  std::vector<float> retVal = witFloatStarToStlVec(attr);
  witFree(attr);
  return retVal;
}

// Get vector of sub int* attribute
std::vector<int> ESO2problem::witGetSubArcAttribute(
                                     witGetNameIndexIndexIntStarStar     witGetFunc,
                                     const std::string & opName,
                                     int index, int subIndex )const
{
  int * attr;
  witGetFunc(mutableWitRun(),opName.c_str(),index,subIndex,&attr);
  std::vector<int> retVal = witIntStarToStlVec(attr);
  witFree(attr);
  return retVal;
}

// Get vector of sub int attribute
int ESO2problem::witGetSubArcAttribute(
                                     witGetNameIndexIndexIntStar     witGetFunc,
                                     const std::string & opName,
                                     int index, int subIndex )const
{
  int retVal;
  witGetFunc(mutableWitRun(),opName.c_str(),index,subIndex,&retVal);
  return retVal;
}

// Set sub float* attribute, given period and fltValue
void ESO2problem::witSetSubArcAttribute(
    witGetNameIndexIndexFloatStarStar witGetFunc,
    witSetNameIndexIndexFloatStar     witSetFunc,
    const std::string & opName,
    int index, int subIndex,
    int period, 
    float flt )    
{ 
  assert( period>= 0 );
  assert( period<getNPeriods() );
  float * floatStar;
  witGetFunc(mutableWitRun(),opName.c_str(),index,subIndex,&floatStar);
  floatStar[period] = flt;
  witSetFunc(witRun(),opName.c_str(),index,subIndex,floatStar);
  witFree(floatStar);
}





// Get vector of wit demand float* attribute
std::vector<float> ESO2problem::witGetDemandAttribute(
    witGetDblNameFloatStarStar witGetFunc,
    const std::string & partName,
    const std::string & demandName )const
{
  float * attr;
  witGetFunc(mutableWitRun(),partName.c_str(),demandName.c_str(),&attr);
  std::vector<float> retVal = witFloatStarToStlVec(attr);
  witFree(attr);
  return retVal;
}




#if 0
//---------------------------------------------
// Get resource shortage list
//---------------------------------------------

void ESO2problem::getResourceShortages(     
    std::vector<std::string> & resourceDescriptions,
    std::vector<int> & periods,
    std::vector<float> & shortageVols ) const
{
  resourceDescriptions.clear();
  periods.clear();
  shortageVols.clear();

  std::vector<std::string> projects;
  getProjects(projects);
  int np = getNPeriods();
  
  // Set Focus
  for ( int p=0; p<projects.size(); p++ ) {
    float *dv;
    witGetDemandDemandVol(mutableWitRun(),projects[p].c_str(),projects[p].c_str(),&dv);
    witSetDemandFssShipVol(mutableWitRun(),projects[p].c_str(),projects[p].c_str(),dv);
    witFree(dv);
  }

  int lenList;
  char **partList;
  float ** shortageVolList;  
  //OsiRelFltEq eq;
  witGetFocusShortageVol ( mutableWitRun(), &lenList, &partList, &shortageVolList );
  for ( int i=0; i<lenList; i++ ) {
    for ( int t=0; t<np; t++ ) {
      if ( shortageVolList[i][t]==0.0 ) continue;
      resourceDescriptions.push_back(partList[i]);
      periods.push_back(t);
      shortageVols.push_back(shortageVolList[i][t]);
    }
    witFree(partList[i]);
    witFree(shortageVolList[i]);
  }
  witFree(partList);
  witFree(shortageVolList);
}
#endif

//--------------------------------------------------------

void ESO2problem::setTitle( const std::string & title)
{
  witSetTitle(witRun(),title.c_str());
}

std::string ESO2problem::getTitle() const
{
  char * t;
  witGetTitle(mutableWitRun(),&t);
  std::string retVal(t);
  witFree(t);
  return retVal;
}

void ESO2problem::setNPeriods( int nP )
{
  witSetNPeriods(witRun(),nP);
  nPeriods_ = nP;
}


double ESO2problem::getObjValue() const
{
  double retVal;
  float objVal;
  float boundsVal;
  witGetObjValues (mutableWitRun(),&objVal,&boundsVal);
  retVal = objVal;
  return retVal;
}

double ESO2problem::sumOfFeatureXrefCost() const
{
  double retVal=0.0;
  int np=getNPeriods();
    std::vector<std::string> parts;
    std::vector<std::string> features;
    std::vector<std::string> locs;
    getFeatureXrefs(parts,features,locs);
  int i;
  for ( i=0; i<parts.size(); ++i ) {
    std::string witOpName = featureXrefName(parts[i],features[i],locs[i]);
    std::vector<float> execVol = witGetNameAttribute(witGetOperationExecVol,witOpName);
    std::vector<float> execCost = witGetNameAttribute(witGetOperationObj1ExecCost,witOpName);
    
    int t;
    for ( t=0; t<np; ++t ) {
      retVal += execVol[t]*execCost[t];
    }
  }
  return retVal;
}
double ESO2problem::sumOfPartMoveCost() const
{
  double retVal=0.0;
  int np=getNPeriods();  
  std::vector<std::string> parts;
  std::vector<std::string> srcLocs;
  std::vector<std::string> destLocs;
  getPartInterplants(parts,srcLocs,destLocs);
  int i;
  for ( i=0; i<parts.size(); ++i ) {
    std::string witMoveOpName = partInterplantName(parts[i],srcLocs[i],destLocs[i]);
    std::vector<float> execVol = witGetNameAttribute(witGetOperationExecVol,witMoveOpName);
    std::vector<float> execCost = witGetNameAttribute(witGetOperationObj1ExecCost,witMoveOpName);
    
    int t;
    for ( t=0; t<np; ++t ) {
      retVal += execVol[t]*execCost[t];
    }
  }
  return retVal;
}
double ESO2problem::sumOfWac(std::map<std::string,double> & wacComponents) const
{
  CoinRelFltEq eq;

  double retVal=0.0;
  wacComponents.clear();

  int np=getNPeriods();
  std::vector<std::string> parts;
  std::vector<std::string> custLocs;
  std::vector<std::string> plantLocs;
  getPartDemands(parts,custLocs,plantLocs);
  int i;
  for ( i=0; i<parts.size(); ++i ) {
    std::string witDemandName = partDemandName(parts[i],custLocs[i],plantLocs[i]);
    std::vector<float> witShipVols = witGetDemandAttribute(witGetDemandShipVol,witDemandName,witDemandName);
    std::vector<float> witShipReward = witGetDemandAttribute(witGetDemandObj1ShipReward,witDemandName,witDemandName);
 
    std::vector<float> shipVols = getPartDemandShipVol(parts[i],custLocs[i],plantLocs[i]);
    std::vector<float> scaledWacs = getPartDemandScaledWac(parts[i],custLocs[i],plantLocs[i]);
    int t;

    for ( t=0; t<np; ++t ) {
      assert( eq(witShipVols[t],shipVols[t]) );
      //assert( eq(witShipReward[t],scaledWacs[t]) );
      double incrementalWac = witShipVols[t]*witShipReward[t];
      if ( incrementalWac==0.0 ) continue;
      retVal += incrementalWac;

      std::vector<std::string> mtmItems,featureItems,loosePartItems,locs;
      std::vector<int> periods;
      std::vector<int> depths;
      std::vector<float> quantities;    
      getPartDemandPegging("MtmAcquire",      parts[i],custLocs[i],plantLocs[i],t, mtmItems,       locs, periods, depths, quantities );
      getPartDemandPegging("FeatureAcquire",  parts[i],custLocs[i],plantLocs[i],t, featureItems,   locs, periods, depths, quantities );
      getPartDemandPegging("LoosePartAcquire",parts[i],custLocs[i],plantLocs[i],t, loosePartItems, locs, periods, depths, quantities );

      std::string compName;
      if ( mtmItems.size()>0 )       compName+="-MtmAcquire-";
      if ( featureItems.size()>0 )   compName+="-FeatureAcquire-";
      if ( loosePartItems.size()>0 ) compName+="-LoosePartAcquire-";
      if ( compName=="" ) compName="-other-";
      // assert( compName!="-other-");
   
      if ( wacComponents.find(compName)==wacComponents.end() ) wacComponents[compName]=0.0;
      wacComponents[compName]+=incrementalWac;
    }
  }
  return retVal;
}
double ESO2problem::sumOfFeatureWfmvWithScaleFactor() const
{
  double retVal=0.0;
  int np=getNPeriods();
  std::vector<std::string> features;
  std::vector<std::string> plantLocs;
  getFeatures(features,plantLocs);
  int i;
  for ( i=0; i<features.size(); ++i ) {
    
    std::string witAcquireOpName = acquireFeatureName(features[i],plantLocs[i]);
    //std::string witPartName = igfFeatureName(features[i],plantLocs[i]);
    std::vector<float> execVol = witGetNameAttribute(witGetOperationExecVol,witAcquireOpName);
    std::vector<float> execCost = witGetNameAttribute(witGetOperationObj1ExecCost,witAcquireOpName);

    int t;
    for ( t=0; t<np; ++t ) {
      retVal += execVol[t]*execCost[t];
    }
  }
  return retVal;
}
double ESO2problem::sumOfLoosePartWfmvWithScaleFactor() const
{
  double retVal=0.0;
  int np=getNPeriods();
  std::vector<std::string> looseParts;
  std::vector<std::string> plantLocs;
  getLooseParts(looseParts,plantLocs);
  int i;
  for ( i=0; i<looseParts.size(); ++i ) {
    
    std::string witAcquireOpName = acquireLoosePartName(looseParts[i],plantLocs[i]);
    //std::string witPartName = igfLoosePartName(looseParts[i],plantLocs[i]);
    std::vector<float> execVol = witGetNameAttribute(witGetOperationExecVol,witAcquireOpName);
    std::vector<float> execCost = witGetNameAttribute(witGetOperationObj1ExecCost,witAcquireOpName);

    //std::vector<float> acquireQty = getLoosePartAcquireQty(looseParts[i],plantLocs[i]);
    //std::vector<float> wfmv = getLoosePartWfmv(looseParts[i],plantLocs[i]);
    //float scaleFactor = getWfmvScaleFactor();
    int t;
    for ( t=0; t<np; ++t ) {
      
      //assert( execVol[t]==acquireQty[t] );
      //assert( execCost[t]==wfmv[t]*scaleFactor );

      retVal += execVol[t]*execCost[t];
    }
  }
  
  return retVal;
}

double ESO2problem::sumOfAltCost() const
{
  double retVal=0.0;
  int np=getNPeriods();
  
    std::vector<std::string> partOrFeatures;
    std::vector<std::string> altParts;
    std::vector<std::string> locs;
    getAlts(partOrFeatures,altParts,locs);
    
    // loop once for each altPart
    int i;
    for ( i=0; i<partOrFeatures.size(); i++ ) {
    
    std::string witAltOpName = altName(partOrFeatures[i],altParts[i],locs[i]);
    std::vector<float> execVol = witGetNameAttribute(witGetOperationExecVol,witAltOpName);
    std::vector<float> execCost = witGetNameAttribute(witGetOperationObj1ExecCost,witAltOpName);

    int t;
    for ( t=0; t<np; ++t ) {
      retVal += execVol[t]*execCost[t];
    }
  }
  
  return retVal;
}

double ESO2problem::sumOfMtmWfmvWithScaleFactorAndMoveCost() const
{
  double retVal=0.0;
  int np=getNPeriods();
    std::vector<std::string> mtms;
    std::vector<std::string> srcLocs;
    std::vector<std::string> destLocs;
    getMtmDemanLocs(mtms,srcLocs,destLocs);
  int i;
  for ( i=0; i<mtms.size(); ++i ) {
    
    std::string witAcquireOpName = acquireMtmName(mtms[i],srcLocs[i],destLocs[i]);
    //std::string witPartName = igfFeatureName(features[i],plantLocs[i]);
    std::vector<float> execVol = witGetNameAttribute(witGetOperationExecVol,witAcquireOpName);
    std::vector<float> execCost = witGetNameAttribute(witGetOperationObj1ExecCost,witAcquireOpName);

    int t;
    for ( t=0; t<np; ++t ) {
      retVal += execVol[t]*execCost[t];
    }
  }
  return retVal;
}

double ESO2problem::sumOfMtmWfmvWithScaleFactor() const
{
  double retVal=sumOfMtmWfmvWithScaleFactorAndMoveCost()-sumOfMtmMoveCost();
  return retVal;
}

double ESO2problem::sumOfMtmMoveCost() const
{
  double retVal=0.0;
  int np=getNPeriods();
    std::vector<std::string> mtms;
    std::vector<std::string> srcLocs;
    std::vector<std::string> destLocs;
    getMtmDemanLocs(mtms,srcLocs,destLocs);
  int i;
  for ( i=0; i<mtms.size(); ++i ) {
    
    std::string witAcquireOpName = acquireMtmName(mtms[i],srcLocs[i],destLocs[i]);
    std::vector<float> execVol = witGetNameAttribute(witGetOperationExecVol,witAcquireOpName);
    std::vector<float> moveCost = getMtmInterplantMoveCost(mtms[i],srcLocs[i],destLocs[i]);

    int t;
    for ( t=0; t<np; ++t ) {
      retVal += execVol[t]*moveCost[t];
    }
  }
  return retVal;
}
double ESO2problem::sumOfDemanCost() const
{
  double retVal=0.0;
  int np=getNPeriods();
  std::vector<std::string> mtms;
  std::vector<std::string> srcLocs;
  std::vector<std::string> destLocs;
  getMtmDemanLocs(mtms,srcLocs,destLocs);
  int i;
  for ( i=0; i<mtms.size(); ++i ) {
    
    std::string witAcquireOpName = demanMtmName(mtms[i],srcLocs[i],destLocs[i]);
    //std::string witPartName = igfFeatureName(features[i],plantLocs[i]);
    std::vector<float> execVol = witGetNameAttribute(witGetOperationExecVol,witAcquireOpName);
    std::vector<float> execCost = witGetNameAttribute(witGetOperationObj1ExecCost,witAcquireOpName);

    int t;
    for ( t=0; t<np; ++t ) {
      retVal += execVol[t]*execCost[t];
    }
  }
  return retVal;
}
void ESO2problem::mtmMetrics(
                              double & totalWfmvWithScale,
                              double & sumAcquireCost,
                              double & sumMoveCost,
                              double & sumDemanCost,
                              int & numUnique,
                              int & numUniqueAcquired,
                              double & supplyQuantity,
                              double & acquiredQuantity)
                              const
{
  sumAcquireCost = sumOfMtmWfmvWithScaleFactor();
  sumMoveCost = sumOfMtmMoveCost();
  sumDemanCost = sumOfDemanCost();
  
  int np=getNPeriods();
  float scaleFactor = getWfmvScaleFactor();
  {
    std::vector<std::string> mtms;
    std::vector<std::string> locs;
    getMtms(mtms,locs);
    
    totalWfmvWithScale = 0.0;
    numUnique = (int)mtms.size();
    supplyQuantity =0;
    
    int i;
    for ( i=0; i<mtms.size(); ++i ) {
      
      std::vector<float> wfmv = getMtmWfmv(mtms[i],locs[i]);
      std::vector<float> supplyQty = getMtmSupply(mtms[i],locs[i]);
      
      int t;
      for ( t=0; t<np; ++t ) {
        totalWfmvWithScale += supplyQty[t]*wfmv[t]*scaleFactor;
        supplyQuantity += supplyQty[t];
      }
    }
  }
  {
    std::vector<std::string> mtms;
    std::vector<std::string> srcLocs;
    std::vector<std::string> destLocs;
    getMtmDemanLocs(mtms,srcLocs,destLocs);
    
    numUniqueAcquired =0;
    acquiredQuantity = 0;
    
    int i;
    for ( i=0; i<mtms.size(); ++i ) {
      
      //std::vector<float> wfmv = getMtmWfmv(mtms[i],srcLocs[i]);
      //std::vector<float> supplyQty = getMtmSupply(mtms[i],srcLocs[i]);
      std::vector<float> acquireQty = getMtmAcquireQty(mtms[i],srcLocs[i],destLocs[i]);
      
      int t;
      bool nonZero = false;
      for ( t=0; t<np; ++t ) {
        //totalWfmvWithScale += supplyQty[t]*wfmv[t]*scaleFactor;
        //supplyQuantity += supplyQty[t];
        acquiredQuantity += acquireQty[t];
        if( acquireQty[t] > 0.0 ) 
          nonZero=true;
      }
      if (nonZero) numUniqueAcquired++;
    }
  }
}

void ESO2problem::featureMetrics(
                              double & totalWfmvWithScale,
                              double & sumAcquireCost,
                              double & sumXrefCost,
                              int & numUnique,
                              int & numUniqueAcquired,
                              double & supplyQuantity,
                              double & acquiredQuantity)
                              const
{
  sumAcquireCost = sumOfFeatureWfmvWithScaleFactor();
  sumXrefCost = sumOfFeatureXrefCost();
  
  int np=getNPeriods();
  float scaleFactor = getWfmvScaleFactor();

  std::vector<std::string> features;
  std::vector<std::string> plantLocs;
  getFeatures(features,plantLocs);
  
  totalWfmvWithScale = 0.0;
  numUnique = (int)features.size();
  numUniqueAcquired =0;
  supplyQuantity =0;
  acquiredQuantity = 0;

  int i;
  for ( i=0; i<features.size(); ++i ) {
    
    std::vector<float> wfmv = getFeatureWfmv(features[i],plantLocs[i]);
    std::vector<float> supplyQty = getFeatureSupply(features[i],plantLocs[i]);
    std::vector<float> acquireQty = getFeatureAcquireQty(features[i],plantLocs[i]);

    int t;
    bool nonZero = false;
    for ( t=0; t<np; ++t ) {
      totalWfmvWithScale += supplyQty[t]*wfmv[t]*scaleFactor;
      supplyQuantity += supplyQty[t];
      acquiredQuantity += acquireQty[t];
      if( acquireQty[t] > 0.0 ) nonZero=true;
    }
    if (nonZero) numUniqueAcquired++;
  }
}

void ESO2problem::loosePartMetrics(
                              double & totalWfmvWithScale,
                              double & sumAcquireCost,
                              double & sumAltCost,
                              int & numUnique,
                              int & numUniqueAcquired,
                              double & supplyQuantity,
                              double & acquiredQuantity)
                              const
{
  sumAcquireCost = sumOfLoosePartWfmvWithScaleFactor();
  sumAltCost = sumOfAltCost();
  
  int np=getNPeriods();
  float scaleFactor = getWfmvScaleFactor();
  
  std::vector<std::string> looseParts;
  std::vector<std::string> plantLocs;
  getLooseParts(looseParts,plantLocs);
  
  totalWfmvWithScale = 0.0;
  numUnique = (int)looseParts.size();
  numUniqueAcquired =0;
  supplyQuantity =0;
  acquiredQuantity = 0;

  int i;
  for ( i=0; i<looseParts.size(); ++i ) {
    
    std::vector<float> wfmv = getLoosePartWfmv(looseParts[i],plantLocs[i]);
    std::vector<float> supplyQty = getLoosePartSupply(looseParts[i],plantLocs[i]);
    std::vector<float> acquireQty = getLoosePartAcquireQty(looseParts[i],plantLocs[i]);

    int t;
    bool nonZero = false;
    for ( t=0; t<np; ++t ) {
      totalWfmvWithScale += supplyQty[t]*wfmv[t]*scaleFactor;
      supplyQuantity += supplyQty[t];
      acquiredQuantity += acquireQty[t];
      if( acquireQty[t] > 0.0 ) nonZero=true;
    }
    if (nonZero) numUniqueAcquired++;
  }
}


void ESO2problem::partDemandMetrics(
                              double & sumDemandWac,
                              double & sumShippedWac,
                              std::map<std::string,double> & sumShippedWacComponents,
                              int & numUnique,
                              int & numUniqueShipped,
                              double & sumDemandVol,
                              double & sumShipVol)
                              const
{
  sumShippedWac = sumOfWac(sumShippedWacComponents);
  
  int np=getNPeriods();
  
  std::vector<std::string> parts;
  std::vector<std::string> custLocs;
  std::vector<std::string> plantLocs;
  getPartDemands(parts,custLocs,plantLocs);
  
  sumDemandWac = 0.0;
  numUnique = (int)parts.size();
  numUniqueShipped =0;
  sumDemandVol=0;
  sumShipVol = 0;

  int i;
  for ( i=0; i<numUnique; ++i ) {
    
    std::vector<float> scaledWac = getPartDemandScaledWac(parts[i],custLocs[i],plantLocs[i]);
    std::vector<float> shipVol = getPartDemandShipVol(parts[i],custLocs[i],plantLocs[i]);
    std::vector<float> demandVol = getPartDemandDemandVol(parts[i],custLocs[i],plantLocs[i]);

    int t;
    bool nonZero = false;
    for ( t=0; t<np; ++t ) {
      sumDemandWac += demandVol[t]*scaledWac[t];
      sumDemandVol += demandVol[t];
      sumShipVol += shipVol[t];
      if( shipVol[t] > 0.0 ) nonZero=true;
    }
    if (nonZero) numUniqueShipped++;
  }
}

// Set OSL Logfile name
void ESO2problem::setSolverLogFileName(const std::string & name)
{
  witSetSolverLogFileName(witRun(),name.c_str());
}

// Set/Get Wholesale Fair Market Value Scale Factor
void ESO2problem::setWfmvScaleFactor(float wfmvScaleFactor, float wfmv2ScaleFactor)
{
  ESO2probAppData * appData;
  witGetAppData(witRun(),(void**)&appData);
  appData->setWfmvScaleFactor(wfmvScaleFactor);
  appData->setWfmv2ScaleFactor(wfmv2ScaleFactor);
}
float ESO2problem::getWfmvScaleFactor() const
{
  ESO2probAppData * appData;
  witGetAppData(mutableWitRun(),(void**)&appData);
  return appData->getWfmvScaleFactor();
}

float ESO2problem::getWfmv2ScaleFactor() const
{
  ESO2probAppData * appData;
  witGetAppData(mutableWitRun(),(void**)&appData);
  return appData->getWfmv2ScaleFactor();
}
float ESO2problem::getWfmvScaleFactor(int demandFileNumber) const
{
  assert(demandFileNumber==0 || demandFileNumber==1 );
  float retVal=-FLT_MAX;
  if (demandFileNumber==0) retVal = getWfmvScaleFactor();
  if (demandFileNumber==1) retVal = getWfmv2ScaleFactor();
  return retVal;
}
void ESO2problem::setApplyLocWfmvScaleFactor(const std::string & plantLoc,int demandFileNumber)
{
  std::pair<std::string,int> pair(plantLoc,demandFileNumber);
  wfmvLocScaleFactorSfDemandFilePair_.insert(pair);
}
bool ESO2problem::getApplyLocWfmvScaleFactor(const std::string & plantLoc,int demandFileNumber) const
{
  bool retVal;
  std::pair<std::string,int> pair(plantLoc,demandFileNumber);
  retVal = wfmvLocScaleFactorSfDemandFilePair_.find(pair)!=wfmvLocScaleFactorSfDemandFilePair_.end();
  return retVal;
}

void ESO2problem::setUseApplyScaleFactorLoc(bool val)
{
  useApplyScaleFactorLoc_ = val;
}
bool ESO2problem::getUseApplyScaleFactorLoc() const
{
  // Return true if using wfmvLocScaleFactorSfDemandFilePair_ (ie applyScaleFactorLocFile)
  bool retVal = useApplyScaleFactorLoc_;
  return retVal;
}



// Set/Get priorities of mtm, features, and looseParts.
// Priorities are:
// 1 - high. Select this category of supply first.
// 2 - medium. 
// 3 - low. Select this category of supply last.
// 4 - never. Never consume this category of supply to meet demand.
void ESO2problem::setMtmPriority( int p )
{
  assert(p>=1 && p<=4);
  mtmPriority_=p;
}
int ESO2problem::getMtmPriority() const
{
  return mtmPriority_;
}
void ESO2problem::setFeaturePriority( int p )
{
  assert(p>=1 && p<=4);
  featurePriority_=p;
}
int ESO2problem::getFeaturePriority() const
{
  return featurePriority_;
}
void ESO2problem::setLoosePartPriority( int p )
{
  assert(p>=1 && p<=4);
  loosePartPriority_=p;
}
int ESO2problem::getLoosePartPriority() const
{
  return loosePartPriority_;
}

void ESO2problem::setMtm2Priority( int p )
{
  assert(p>=1 && p<=4);
  mtm2Priority_=p;
}
int ESO2problem::getMtm2Priority() const
{
  return mtm2Priority_;
}
void ESO2problem::setFeature2Priority( int p )
{
  assert(p>=1 && p<=4);
  feature2Priority_=p;
}
int ESO2problem::getFeature2Priority() const
{
  return feature2Priority_;
}
void ESO2problem::setLoosePart2Priority( int p )
{
  assert(p>=1 && p<=4);
  loosePart2Priority_=p;
}
int ESO2problem::getLoosePart2Priority() const
{
  return loosePart2Priority_;
}

int ESO2problem::getMtmPriority(int demandPriority) const
{
  assert(demandPriority>=0 && demandPriority<=1);
  int retVal;
  if (demandPriority==0) 
    retVal=getMtmPriority();
  else
    retVal=getMtm2Priority();
  return retVal;
}
int ESO2problem::getFeaturePriority(int demandPriority) const
{
  assert(demandPriority>=0 && demandPriority<=1);
  int retVal;
  if (demandPriority==0) 
    retVal=getFeaturePriority();
  else
    retVal=getFeature2Priority();
  return retVal;
}
int ESO2problem::getLoosePartPriority(int demandPriority) const
{
  assert(demandPriority>=0 && demandPriority<=1);
  int retVal;
  if (demandPriority==0) 
    retVal=getLoosePartPriority();
  else
    retVal=getLoosePart2Priority();
  return retVal;
}

void ESO2problem::setPeriod1SupplyPerishPeriod( int periods )
{
  assert(periods>=0);
  period1SupplyPerishPeriod_=periods;
}

void ESO2problem::setAllowInventoryHoldingAtDemandLocation( bool allow )
{
  allowInventoryHoldingAtDemandLocation_=allow;
}

void ESO2problem::setRemoveCycles( bool remove )
{
  removeCycles_=remove;
}
void ESO2problem::setWacScaleFactors( float wsf1, float wsf2 )
{
  wacScaleFactors_.clear();
  wacScaleFactors_.push_back(wsf1);
  wacScaleFactors_.push_back(wsf2);
}
const std::vector<float> & ESO2problem::getWacScaleFactors()const
{
  return wacScaleFactors_;
}

void ESO2problem::writeWitData( std::string filename ) const
{
  witSetMesgFileAccessMode(mutableWitRun(),WitFALSE,"w");
  witWriteData(mutableWitRun(), filename.c_str() );
}











WitRun * ESO2problem::mutableWitRun() const { return wr_; }
WitRun * ESO2problem::witRun() { return wr_; }

int ESO2problem::implosionCount() const { return implosionCount_; }

// default constructor
ESO2problem::ESO2problem()
:
wr_(NULL),
implosionCount_(0),
nPeriods_(26),
mtmNames_(),
featureNames_(),
loosePartNames_(),
//alternateParts_(),
partInterplants_(),
partDemands_(),
demanBomAndFeatureXrefProdRatesAreSet_(false),
mtmAcquireLocs_(),
partOrFeatureLocs_(),
defaultPartInterplants_(),
perishableSupplyWarningMsgTested_(),
mtmPriority_(1),
featurePriority_(1),
loosePartPriority_(1),
mtm2Priority_(1),
feature2Priority_(1),
loosePart2Priority_(1),
wacScaleFactors_(),
wfmvLocScaleFactorSfDemandFilePair_(),
allowInventoryHoldingAtDemandLocation_(false),
removeCycles_(false),
useApplyScaleFactorLoc_(true),
period1SupplyPerishPeriod_(0)
{
  witNewRun( &wr_ );
  witInitialize( witRun() );
  //witSetOslMesgFileName(witRun(),WitSTDOUT);
  
  // Turn off WIT informational messages
  witSetMesgTimesPrint( witRun(), WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
  // Turn off warning msg about long names.
  witSetMesgTimesPrint( witRun(), WitTRUE, 338, 0);

  // Turn on some interesting informational messages
  witSetMesgTimesPrint( witRun(), WitTRUE, 729, UCHAR_MAX);
  witSetMesgTimesPrint( witRun(), WitTRUE, 730, UCHAR_MAX);

  //WIT0729I Deleting selected objects.
  //         Number of objects selected by the user:  158900
  //         Number of objects selected by WIT:       160629
  //         Number of objects selected total:        319529

  //WIT0730I Object purge complete.
  //         Number of objects before purge:  332170
  //         Number of objects deleted:       319529
  //         Number of objects after purge:    12641

  /* Turn off warning msg:
  WIT0749W One or more BOP entries have been rejected for possible explosion,
         because they had positive effective production rates (= yieldRate *
         productRate) that were below the value of expCutoff. The largest
         effective production rate that was rejected and the corresponding BOP
         entry and execution period are given below:

            expCutoff:             0.01
            Largest Rejected Rate: 0.008
            Operation:             "disassemble_26353AU"
            BOP Entry Index:       4
            Exec. Period:          0
  */
  //witSetMesgTimesPrint( witRun(), WitTRUE, 749, 0);
  
  
  //witSetTitle( witRun(), "DaimlerChrysler" );
  //witSetNPeriods( witRun(), 1 );
  witSetObjChoice( witRun(), 1 );
  witSetUseFocusHorizons( witRun(), WitFALSE );

  witSetExecEmptyBom( witRun(), WitFALSE );
  witSetMultiRoute( witRun(), WitTRUE );
  witSetIndependentOffsets( witRun(), WitTRUE );

  witSetAccAfterOptImp(witRun(),WitTRUE );
  witSetAccAfterSoftLB(witRun(),WitTRUE );

  //witSetPostSolveRecovery(witRun(),WitTRUE);

  //witSetGroupPipMode(witRun(),WitTRUE);

  ESO2probAppData * problemAppData = new ESO2probAppData;
  witSetAppData(witRun(),problemAppData);

  // Set default wacScaleFactor for both demand files to 1
  wacScaleFactors_.push_back(1.0f);
  wacScaleFactors_.push_back(1.0f);
 
}

// destructor
ESO2problem::~ESO2problem()
{
  gutsOfDestructor();
}

// copy constructor
ESO2problem::ESO2problem( const ESO2problem& source ):
wr_(NULL),
implosionCount_(source.nPeriods_),
nPeriods_(source.nPeriods_),
mtmNames_(),
featureNames_(),
loosePartNames_(),
xrefFeatures_(),
alts_(),
partInterplants_(),
partDemands_(),
demanBomAndFeatureXrefProdRatesAreSet_(false),
mtmAcquireLocs_(),
partOrFeatureLocs_(),
defaultPartInterplants_(),
perishableSupplyWarningMsgTested_(),
mtmPriority_(1),
featurePriority_(1),
loosePartPriority_(1),
mtm2Priority_(1),
feature2Priority_(1),
loosePart2Priority_(1),
wacScaleFactors_(),
wfmvLocScaleFactorSfDemandFilePair_(),
allowInventoryHoldingAtDemandLocation_(false),
removeCycles_(false),
useApplyScaleFactorLoc_(true),
period1SupplyPerishPeriod_(0)
{
  gutsOfCopy(source);
}

// assignment operator
ESO2problem&
ESO2problem::operator=(const ESO2problem& rhs)
{  
  if (this != &rhs) { 
    if ( wr_ != NULL ) gutsOfDestructor();
    wr_ = NULL;
    gutsOfCopy(rhs);
  }
  return *this;
}

// copy object
void ESO2problem::gutsOfCopy( const ESO2problem& source )
{
  // Copy Wit Run
  assert(wr_==NULL);
  witNewRun( &wr_ );

  // Turn off WIT informational messages
  witSetMesgTimesPrint( wr_, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
  // Turn off warning msg about long names.
  witSetMesgTimesPrint( wr_, WitTRUE, 338, 0);

  witCopyData(wr_,source.wr_);
  implosionCount_ = source.implosionCount_;
  nPeriods_ = source.nPeriods_;
  mtmNames_ = source.mtmNames_;
  featureNames_  = source.featureNames_;
  loosePartNames_  = source.loosePartNames_;
  xrefFeatures_  = source.xrefFeatures_;
  alts_  = source.alts_;
  partInterplants_  = source.partInterplants_;
  partDemands_  = source.partDemands_;
  demanBomAndFeatureXrefProdRatesAreSet_  = source.demanBomAndFeatureXrefProdRatesAreSet_;
  mtmAcquireLocs_  = source.mtmAcquireLocs_;
  partOrFeatureLocs_  = source.partOrFeatureLocs_;
  defaultPartInterplants_ = source.defaultPartInterplants_;
  perishableSupplyWarningMsgTested_ = source.perishableSupplyWarningMsgTested_;
  mtmPriority_  = source.mtmPriority_;
  featurePriority_  = source.featurePriority_;
  loosePartPriority_  = source.loosePartPriority_;
  mtm2Priority_  = source.mtm2Priority_;
  feature2Priority_  = source.feature2Priority_;
  loosePart2Priority_  = source.loosePart2Priority_;
  wacScaleFactors_  = source.wacScaleFactors_;
  wfmvLocScaleFactorSfDemandFilePair_ = source.wfmvLocScaleFactorSfDemandFilePair_;
  allowInventoryHoldingAtDemandLocation_ = source.allowInventoryHoldingAtDemandLocation_;
  removeCycles_ = source.removeCycles_;
  useApplyScaleFactorLoc_ = source.useApplyScaleFactorLoc_;
  period1SupplyPerishPeriod_  = source.period1SupplyPerishPeriod_;

  // Copy global app data
  {      
    ESO2probAppData * srcAppData;  
    ESO2probAppData * sinkAppData;
    witGetAppData(source.mutableWitRun(), (void**)&srcAppData);
    sinkAppData = new ESO2probAppData(*srcAppData);    
    witSetAppData(witRun(), sinkAppData);
  }
    
  // copy partDemand appData
  {
    std::vector<std::string> parts;
    std::vector<std::string> custLocs;
    std::vector<std::string> plantLocs;
    getPartDemands( parts, custLocs, plantLocs);
    int p;
    for (p=0; p<parts.size(); ++p ) { 
      ESO2partDemandAppData * srcAppData;  
      ESO2partDemandAppData * sinkAppData;
      std::string dName = partDemandName(parts[p],custLocs[p],plantLocs[p]);
      witGetDemandAppData(source.mutableWitRun(),dName.c_str(),dName.c_str(),(void**)&srcAppData);
      sinkAppData = new ESO2partDemandAppData(*srcAppData);    
      witSetDemandAppData(witRun(),dName.c_str(),dName.c_str(),sinkAppData);
    }
  }
  // copy looseParts appData
  {
    std::vector<std::string> parts;
    std::vector<std::string> locs;
    getLooseParts( parts, locs);
    int p;
    for (p=0; p<parts.size(); ++p ) { 
      ESO2loosePartAppData * srcAppData;  
      ESO2loosePartAppData * sinkAppData;
      std::string opName = acquireLoosePartName(parts[p],locs[p]);
      witGetOperationAppData(source.mutableWitRun(),opName.c_str(),(void**)&srcAppData);
      sinkAppData = new ESO2loosePartAppData(*srcAppData);    
      witSetOperationAppData(witRun(),opName.c_str(),sinkAppData);
    }
  }
   
  // copy features appData
  {
    std::vector<std::string> features;
    std::vector<std::string> locs;
    getFeatures( features, locs);
    int p;
    for (p=0; p<features.size(); ++p ) {
      ESO2featureAppData * srcAppData;  
      ESO2featureAppData * sinkAppData;
      std::string opName = acquireFeatureName(features[p],locs[p]);
      witGetOperationAppData(source.mutableWitRun(),opName.c_str(),(void**)&srcAppData);
      sinkAppData = new ESO2featureAppData(*srcAppData);    
      witSetOperationAppData(witRun(),opName.c_str(),sinkAppData);
    }
  }
  
  // copy featureXrefs appData
  {
    std::vector<std::string> features;
    std::vector<std::string> parts;
    std::vector<std::string> locs;
    getFeatureXrefs( features, parts, locs);
    int p;
    for (p=0; p<features.size(); ++p ) {
      ESO2featureXrefAppData * srcAppData;  
      ESO2featureXrefAppData * sinkAppData;
      std::string opName = featureXrefName(features[p],parts[p],locs[p]);
      witGetOperationAppData(source.mutableWitRun(),opName.c_str(),(void**)&srcAppData);
      sinkAppData = new ESO2featureXrefAppData(*srcAppData);    
      witSetOperationAppData(witRun(),opName.c_str(),sinkAppData);
    }
  }
   
  // Copy Mtm App Data and demanufacturing bops
  {
    std::vector<std::string> mtms;
    std::vector<std::string> srcLocs;
    std::vector<std::string> destLocs;
    getMtmDemanLocs(mtms, srcLocs, destLocs );
    int p;
    for (p=0; p<mtms.size(); ++p ) {
      ESO2mtmAppData * srcAppData;  
      ESO2mtmAppData * sinkAppData;
      std::string opName = acquireMtmName(mtms[p],srcLocs[p],destLocs[p] );
      witGetOperationAppData(source.mutableWitRun(),opName.c_str(),(void**)&srcAppData);
      sinkAppData = new ESO2mtmAppData(*srcAppData);    
      witSetOperationAppData(witRun(),opName.c_str(),sinkAppData);

      // copy appData for each deman bop
      if ( srcLocs[p]==destLocs[p] ) {
        std::string demanOpName = demanMtmName(mtms[p],srcLocs[p],destLocs[p] );
        int nBops;
        witGetOperationNBopEntries(source.mutableWitRun(),demanOpName.c_str(),&nBops);
        // loop once for each bop
        int b;
        for (b=0; b<nBops; ++b ) {
          ESO2mtmDemanBomAppData * srcAppData;  
          ESO2mtmDemanBomAppData * sinkAppData;
          witGetBopEntryAppData(source.mutableWitRun(),demanOpName.c_str(),b,(void**)&srcAppData);
          sinkAppData = new ESO2mtmDemanBomAppData(*srcAppData);    
          witSetBopEntryAppData(witRun(),demanOpName.c_str(),b,sinkAppData);
        }
      }

    } //end of p loop
  }

}

// destructor
void ESO2problem::gutsOfDestructor()
{

  // Delete global app data
  {  
    ESO2probAppData * probAppData;  
    witGetAppData(mutableWitRun(),  (void**)&probAppData);
    delete probAppData;
  }
  
  // delete partDemand appData
  {
    std::vector<std::string> parts;
    std::vector<std::string> custLocs;
    std::vector<std::string> plantLocs;
    getPartDemands( parts, custLocs, plantLocs);
    int p;
    for (p=0; p<parts.size(); ++p ) { 
      ESO2partDemandAppData * srcAppData;  
      std::string dName = partDemandName(parts[p],custLocs[p],plantLocs[p]);
      witGetDemandAppData(mutableWitRun(),dName.c_str(),dName.c_str(),(void**)&srcAppData);
      delete srcAppData;
    }
  }
    
    
  // delete looseParts appData
  {
    std::vector<std::string> parts;
    std::vector<std::string> locs;
    getLooseParts( parts, locs);
    int p;
    for (p=0; p<parts.size(); ++p ) { 
      ESO2loosePartAppData * srcAppData;  
      std::string opName = acquireLoosePartName(parts[p],locs[p]);
      witGetOperationAppData(mutableWitRun(),opName.c_str(),(void**)&srcAppData);
      delete srcAppData;
    }
  }
   
  // delete features appData
  {
    std::vector<std::string> features;
    std::vector<std::string> locs;
    getFeatures( features, locs);
    int p;
    for (p=0; p<features.size(); ++p ) {
      ESO2featureAppData * srcAppData;  
      std::string opName = acquireFeatureName(features[p],locs[p]);
      witGetOperationAppData(mutableWitRun(),opName.c_str(),(void**)&srcAppData);
      delete srcAppData;
    }
  }
  
  // copy featureXrefs appData
  {
    std::vector<std::string> features;
    std::vector<std::string> parts;
    std::vector<std::string> locs;
    getFeatureXrefs( features, parts, locs);
    int p;
    for (p=0; p<features.size(); ++p ) {
      ESO2featureXrefAppData * srcAppData;  
      std::string opName = featureXrefName(features[p],parts[p],locs[p]);
      witGetOperationAppData(mutableWitRun(),opName.c_str(),(void**)&srcAppData);
      delete srcAppData;
    }
  }
   
   
  // delete Mtm App Data and deman bop app data
  {
    std::vector<std::string> mtms;
    std::vector<std::string> srcLocs;
    std::vector<std::string> destLocs;
    getMtmDemanLocs(mtms, srcLocs, destLocs );
    int p;
    for (p=0; p<mtms.size(); ++p ) {
      ESO2mtmAppData * srcAppData;  
      std::string opName = acquireMtmName(mtms[p],srcLocs[p],destLocs[p] );
      witGetOperationAppData(mutableWitRun(),opName.c_str(),(void**)&srcAppData);
      delete srcAppData;
      
      if ( srcLocs[p]==destLocs[p] ) {
        // delete appData for each deman bop
        std::string demanOpName = demanMtmName(mtms[p],srcLocs[p],destLocs[p] );
        int nBops;
        witGetOperationNBopEntries(mutableWitRun(),demanOpName.c_str(),&nBops);
        // loop once for each bop
        int b;
        for (b=0; b<nBops; ++b ) {
          ESO2mtmDemanBomAppData * srcAppData; 
          witGetBopEntryAppData(mutableWitRun(),demanOpName.c_str(),b,(void**)&srcAppData);
          delete srcAppData;
        }
      }
      
    } // end of p loop
    
  }

  // delete the witRun
  witDeleteRun(witRun());
}



#ifdef NDEBUG
#undef NDEBUG
#endif
//------------------------------------------------------------------------------
// Test function
//------------------------------------------------------------------------------
void
ESO2problem::test()
{
  //OsiRelFltEq eq(1e-05);
  CoinRelFltEq eq(1.e-05);

  {
    ESO2problem prob;
    
    prob.setNPeriods(12);
    assert( prob.getNPeriods() == 12 );
    
    prob.setTitle("wittitle");
    assert( prob.getTitle() == "wittitle");

    assert( prob.implosionCount()==0 );
    

  }


         
  // Test mtm interplant time and deman time
  {
    ESO2problem prob;
    prob.setNPeriods(25);
    prob.setPeriod1SupplyPerishPeriod(0);

    prob.addMtm("m1","loc1",0.,25);
    prob.setMtmSupply("m1","loc1",0,3.0f);
    
    prob.setMtmDemanCost("m1","loc1",4,2.0f);
    prob.setMtmDemanTime("m1","loc1",4,3.0f);

    prob.setMtmWfmv("m1","loc1",0,0.1f);

    prob.addMtmInterplant("m1","loc1","loc2",0.5f,1.0f);
#if 0
    std::vector<float> vTime = prob.floatToStlVec(0.0f);
    std::vector<float> vCost = vTime;
    vCost[3]=1.0f;
    prob.setMtmDemanCost("m1","loc1","loc2",vCost);
    vTime[3]=2.f;
    prob.setMtmDemanTime("m1","loc1","loc2",vTime);
#else
    prob.setMtmDemanCost("m1","loc1","loc2",3,1.0f);
    prob.setMtmDemanTime("m1","loc1","loc2",3,2.0f);
#endif
 
    prob.addDemanBom("m1","p1","loc1");

    prob.setDemanBomAttachRate("m1","p1","loc1",0,1.0f);

    prob.addPartDemand("p1","US","loc2");
    prob.setPartDemandDemandVol("p1","US","loc2",3,1.0f);
    prob.setPartDemandWac("p1","US","loc2",3,10.0f);
#if 0
    {
      std::string demanMtmWitName = demanMtmName("m1","loc1","loc2" );
      std::vector<float> demanTime = prob.witGetArcAttribute(witGetBomEntryOffset,demanMtmWitName,0);
      assert( demanTime[0]==100.0f );
      assert( demanTime[1]==3.0f );
      assert( demanTime[2]==100.0f );
      assert( demanTime[3]==100.0f );
    }
#endif

    prob.solveWithPegging();
    assert( prob.implosionCount()==6 );
    prob.reportPartDemand("p1","US","loc2",3);
    std::vector<float> sv = prob.getPartDemandShipVol("p1","US","loc2");
    assert(sv[0]==0.0f);
    assert(sv[1]==0.0f);
    assert(sv[2]==0.0f);
    assert(sv[3]==1.0f);
    assert(sv[4]==0.0f);
    
    std::vector<std::string> acquireMtms,mtmSrcLocs,mtmAcquireLocs;
    std::vector<int> mtmAcquirePeriods,depths;
    std::vector<float> mtmAcquireQuantity;       

    std::vector<std::string> demanMtms,demanLocs;
    std::vector<int> demanPeriods;
    std::vector<float> demanQuantity;    

    std::vector<std::string> features,featurePlantLocs;
    std::vector<int> featureAcquirePeriods;
    std::vector<float> featureAcquireQuantity;    
    std::vector<std::string> looseParts,loosePartPlantLocs;
    std::vector<int> loosePartAcquirePeriods;
    std::vector<float> loosePartAcquireQuantity;
    prob.getPartDemandPegging(
      "MtmAcquire",
      "p1","US","loc2",3, 
      acquireMtms,mtmAcquireLocs,mtmAcquirePeriods,depths,mtmAcquireQuantity );
    size_t m1= acquireMtms.size();

    
    prob.getPartDemandPegging(
      "MtmAcquire",
      "p1","US","loc2",0,  
      acquireMtms,mtmAcquireLocs,mtmAcquirePeriods,depths,mtmAcquireQuantity );
    size_t m2= acquireMtms.size();
    assert(m1!=m2);
  }

  
  // Test loose part perishability
  {
    ESO2problem prob;
    prob.setNPeriods(4);
    prob.setPeriod1SupplyPerishPeriod(2);
    prob.addLoosePart("lp1","loc1");
    prob.setLoosePartSupply("lp1","loc1",0,3.0f);

    prob.setLoosePartWfmv("lp1","loc1",0,0.1f);
    //prob.setLoosePartWfmv("lp1","loc1",1,1000.0f);
    //prob.setLoosePartWfmv("lp1","loc1",2,1000.0f);
    //prob.setLoosePartWfmv("lp1","loc1",3,1000.0f);

    prob.addPartDemand("lp1","US","loc1");
    prob.setPartDemandDemandVol("lp1","US","loc1",0,1.0f);
    prob.setPartDemandDemandVol("lp1","US","loc1",1,1.0f);
    prob.setPartDemandDemandVol("lp1","US","loc1",2,1.0f);
    prob.setPartDemandDemandVol("lp1","US","loc1",3,1.0f);
    prob.setPartDemandWac("lp1","US","loc1",0,10.0f);
    prob.setPartDemandWac("lp1","US","loc1",1,10.0f);
    prob.setPartDemandWac("lp1","US","loc1",2,10.0f);
    prob.setPartDemandWac("lp1","US","loc1",3,10.0f);

    prob.solveWithPegging();
    std::vector<float> sv = prob.getPartDemandShipVol("lp1","US","loc1");
    assert(sv[0]==1.0f);
    assert(sv[1]==1.0f);
    assert(sv[2]==1.0f);
    assert(sv[3]==0.0f);
  }

    // Test loose part perishability
  {
    ESO2problem prob;
    createModel(prob, "../data/loosePartPerishability", true );

    prob.solveWithPegging();
    std::vector<float> sv = prob.getPartDemandShipVol("lp1","US","loc1");
    assert(sv[0]==1.0f);
    assert(sv[1]==1.0f);
    assert(sv[2]==1.0f);
    assert(sv[3]==0.0f);
  }

    
  // Test feature perishability
  {
    ESO2problem prob;
    prob.setNPeriods(4);
    prob.setPeriod1SupplyPerishPeriod(2);
    prob.addFeature("f1","loc1");
    prob.setFeatureSupply("f1","loc1",0,3.0f);

    prob.setFeatureWfmv("f1","loc1",0,0.1f);
    //prob.setFeatureWfmv("f1","loc1",1,1000.1f);
    //prob.setFeatureWfmv("f1","loc1",2,1000.1f);
    //prob.setFeatureWfmv("f1","loc1",3,1000.1f);

    prob.addFeatureXref("p1","f1","loc1");
    prob.setFeatureXrefCost("p1","f1","loc1",0,0);
    prob.setFeatureXrefCost("p1","f1","loc1",1,0);
    prob.setFeatureXrefCost("p1","f1","loc1",2,0);
    prob.setFeatureXrefCost("p1","f1","loc1",3,0);
    prob.setFeatureXrefUsageTime("p1","f1","loc1",0,0.0f);
    prob.setFeatureXrefUsageTime("p1","f1","loc1",1,0.0f);
    prob.setFeatureXrefUsageTime("p1","f1","loc1",2,0.0f);
    prob.setFeatureXrefUsageTime("p1","f1","loc1",3,0.0f);
    prob.setFeatureXrefProdRate("p1","f1","loc1",0,1.0f);
    prob.setFeatureXrefProdRate("p1","f1","loc1",1,1.0f);
    prob.setFeatureXrefProdRate("p1","f1","loc1",2,1.0f);
    prob.setFeatureXrefProdRate("p1","f1","loc1",3,1.0f);

    prob.addPartDemand("p1","US","loc1");
    prob.setPartDemandDemandVol("p1","US","loc1",0,1.0f);
    prob.setPartDemandDemandVol("p1","US","loc1",1,1.0f);
    prob.setPartDemandDemandVol("p1","US","loc1",2,1.0f);
    prob.setPartDemandDemandVol("p1","US","loc1",3,1.0f);
    prob.setPartDemandWac("p1","US","loc1",0,10.0f);
    prob.setPartDemandWac("p1","US","loc1",1,10.0f);
    prob.setPartDemandWac("p1","US","loc1",2,10.0f);
    prob.setPartDemandWac("p1","US","loc1",3,10.0f);

    prob.solveWithPegging();
    //prob.reportPartDemand("p1","US","loc1",0);
    std::vector<float> sv = prob.getPartDemandShipVol("p1","US","loc1");
    assert(sv[0]==1.0f);
    assert(sv[1]==1.0f);
    assert(sv[2]==1.0f);
    assert(sv[3]==0.0f);
  }

      
  // Test mtm perishability
  {
    ESO2problem prob;
    prob.setNPeriods(4);
    prob.setPeriod1SupplyPerishPeriod(2);
    prob.addMtm("m1","loc1",0,5);
    prob.setMtmSupply("m1","loc1",0,3.0f);

    prob.setMtmDemanCost("m1","loc1",0,0.0f);
    prob.setMtmDemanCost("m1","loc1",1,0.0f);
    prob.setMtmDemanCost("m1","loc1",2,0.0f);
    prob.setMtmDemanCost("m1","loc1",3,0.0f);

    prob.setMtmDemanTime("m1","loc1",0,0.0f);
    prob.setMtmDemanTime("m1","loc1",1,0.0f);
    prob.setMtmDemanTime("m1","loc1",2,0.0f);
    prob.setMtmDemanTime("m1","loc1",3,0.0f);

    prob.setMtmWfmv("m1","loc1",0,0.1f);
    prob.setMtmWfmv("m1","loc1",1,0.1f);
    prob.setMtmWfmv("m1","loc1",2,0.1f);
    prob.setMtmWfmv("m1","loc1",3,0.1f);

    prob.addDemanBom("m1","p1","loc1");

    prob.setDemanBomAttachRate("m1","p1","loc1",0,1.0f);
    prob.setDemanBomAttachRate("m1","p1","loc1",1,1.0f);
    prob.setDemanBomAttachRate("m1","p1","loc1",2,1.0f);
    prob.setDemanBomAttachRate("m1","p1","loc1",3,1.0f);

    prob.addPartDemand("p1","US","loc1");
    prob.setPartDemandDemandVol("p1","US","loc1",0,1.0f);
    prob.setPartDemandDemandVol("p1","US","loc1",1,1.0f);
    prob.setPartDemandDemandVol("p1","US","loc1",2,1.0f);
    prob.setPartDemandDemandVol("p1","US","loc1",3,1.0f);
    prob.setPartDemandWac("p1","US","loc1",0,10.0f);
    prob.setPartDemandWac("p1","US","loc1",1,10.0f);
    prob.setPartDemandWac("p1","US","loc1",2,10.0f);
    prob.setPartDemandWac("p1","US","loc1",3,10.0f);

    prob.solveWithPegging();
    //prob.reportPartDemand("p1","US","loc1",0);
    std::vector<float> sv = prob.getPartDemandShipVol("p1","US","loc1");
    assert(sv[0]==1.0f);
    assert(sv[1]==1.0f);
    assert(sv[2]==1.0f);
    assert(sv[3]==0.0f);
  }

  
      
  // Test mtm perishability with mtm interplant
  {
    ESO2problem prob;
    prob.setNPeriods(4);
    prob.setPeriod1SupplyPerishPeriod(2);

    prob.addMtm("m1","loc1",0,5.);
    prob.setMtmSupply("m1","loc1",0,3.0f);

    prob.setMtmDemanCost("m1","loc1",0,0.0f);
    prob.setMtmDemanCost("m1","loc1",1,0.0f);
    prob.setMtmDemanCost("m1","loc1",2,0.0f);
    prob.setMtmDemanCost("m1","loc1",3,0.0f);

    prob.setMtmDemanTime("m1","loc1",0,0.0f);
    prob.setMtmDemanTime("m1","loc1",1,0.0f);
    prob.setMtmDemanTime("m1","loc1",2,0.0f);
    prob.setMtmDemanTime("m1","loc1",3,0.0f);

    prob.setMtmWfmv("m1","loc1",0,0.1f);
    //prob.setMtmWfmv("m1","loc1",1,0.1f);
    //prob.setMtmWfmv("m1","loc1",2,0.1f);
    //prob.setMtmWfmv("m1","loc1",3,0.1f);

    prob.addMtmInterplant("m1","loc1","loc2",0.0f,0.0f);

    prob.setMtmDemanCost("m1","loc1","loc2",0,0.0f);
    prob.setMtmDemanCost("m1","loc1","loc2",1,0.0f);
    prob.setMtmDemanCost("m1","loc1","loc2",2,0.0f);
    prob.setMtmDemanCost("m1","loc1","loc2",3,0.0f);

    prob.setMtmDemanTime("m1","loc1","loc2",0,0.0f);
    prob.setMtmDemanTime("m1","loc1","loc2",1,0.0f);
    prob.setMtmDemanTime("m1","loc1","loc2",2,0.0f);
    prob.setMtmDemanTime("m1","loc1","loc2",3,0.0f);

    prob.addDemanBom("m1","p1","loc1");

    prob.setDemanBomAttachRate("m1","p1","loc1",0,1.0f);
    prob.setDemanBomAttachRate("m1","p1","loc1",1,0.0f);
    prob.setDemanBomAttachRate("m1","p1","loc1",2,0.0f);
    prob.setDemanBomAttachRate("m1","p1","loc1",3,0.0f);

    prob.addPartDemand("p1","US","loc2");
    prob.setPartDemandDemandVol("p1","US","loc2",0,1.0f);
    prob.setPartDemandDemandVol("p1","US","loc2",1,1.0f);
    prob.setPartDemandDemandVol("p1","US","loc2",2,1.0f);
    prob.setPartDemandDemandVol("p1","US","loc2",3,1.0f);
    prob.setPartDemandWac("p1","US","loc2",0,10.0f);
    prob.setPartDemandWac("p1","US","loc2",1,10.0f);
    prob.setPartDemandWac("p1","US","loc2",2,10.0f);
    prob.setPartDemandWac("p1","US","loc2",3,10.0f);

    prob.solveWithPegging();
    prob.reportPartDemand("p1","US","loc2",0);
    std::vector<float> sv = prob.getPartDemandShipVol("p1","US","loc2");
    assert(sv[0]==1.0f);
    assert(sv[1]==1.0f);
    assert(sv[2]==1.0f);
    assert(sv[3]==0.0f);
  }

  // Test moving loose part once
  {
    ESO2problem prob;
    prob.setNPeriods(1);
    prob.addPartsInterplant("loc1","loc2",0.0f,0.0f);   
    prob.addLoosePart("lp1","loc1");
    prob.setLoosePartSupply("lp1","loc1",0,1.0f);
    prob.setLoosePartWfmv("lp1","loc1",0,0.1f);
    prob.addPartDemand("lp1","US","loc2");
    prob.setPartDemandDemandVol("lp1","US","loc2",0,1.0f);
    prob.setPartDemandWac("lp1","US","loc2",0,10.0f);

    prob.solveWithPegging();
    std::vector<float> sv = prob.getPartDemandShipVol("lp1","US","loc2");
    assert(sv[0]==1.0f);
  }

    
  
  // Test moving loose part twice
  {
    ESO2problem prob;
    prob.setNPeriods(1);
    prob.addPartsInterplant("loc1","loc2",0.0f,0.0f);   
    prob.addPartsInterplant("loc2","loc3",0.0f,0.0f);   
    prob.addLoosePart("lp1","loc1");
    prob.setLoosePartSupply("lp1","loc1",0,1.0f);
    prob.setLoosePartWfmv("lp1","loc1",0,0.1f);
    prob.addPartDemand("lp1","US","loc3");
    prob.setPartDemandDemandVol("lp1","US","loc3",0,1.0f);
    prob.setPartDemandWac("lp1","US","loc3",0,10.0f);

    prob.solveWithPegging();
    std::vector<float> sv = prob.getPartDemandShipVol("lp1","US","loc3");
    assert(sv[0]==0.0f); // If move twice supported then assert would be for 1.0f
  }
  
  // Test low priority demand
  {
    ESO2problem p1;
    p1.setWfmvScaleFactor(0.0f,0.0f);
    createModel(p1, "../data/03Apr07KevinOwithLPDemand", true );
    
    p1.solveWithPegging();


    // Open partShip file and write header.
    {
      std::string heading;
      heading="\"part\",\"custLoc\",\"plantLoc\",\"period\",\"demandVol\",\"shipVol\""; 
      printf("%s\n",heading.c_str());
      
      std::vector<std::string> parts;
      std::vector<std::string> custLocs;
      std::vector<std::string> plantLocs;
      p1.getPartDemands(parts,custLocs,plantLocs);
      
      // loop once for each shipped part
      int p;
      for ( p=0; p<parts.size(); p++ ) {
        
        std::vector<float> demandVol = p1.getPartDemandDemandVol(parts[p],custLocs[p],plantLocs[p]);
        std::vector<float> shipVol = p1.getPartDemandShipVol(parts[p],custLocs[p],plantLocs[p]);
        std::vector<float> wac = p1.getPartDemandWac(parts[p],custLocs[p],plantLocs[p]);
        
        for ( int t=0; t<shipVol.size(); t++ ) {
          if ( shipVol[t]==0.0 ) continue;
          printf( "\"%s\",\"%s\",\"%s\",%d,%f,%f\n",
            parts[p].c_str(),custLocs[p].c_str(),plantLocs[p].c_str(),t,demandVol[t],shipVol[t]);
          //partAcquireAvoidanceValue += shipVol[t]*wac[t];
        } // finished t loop
      } // finished p loop
    } // finished writing partShip file

    std::vector<float> vol = p1.getPartDemandShipVol("0000000P1413","980","980");
    assert(eq(vol[0],0.0));
    assert(eq(vol[1],137.));
    assert(eq(vol[2],137.));
    assert(eq(vol[3],21.));
    assert(eq(vol[4],16.));
    assert(eq(vol[5],16.));
    assert(eq(vol[6],16.));
    assert(eq(vol[7],23.));
    assert(eq(vol[8],20.));
    assert(eq(vol[9],21.));
    assert(eq(vol[10],22.));
    assert(eq(vol[11],28.));
    assert(eq(vol[12],55.));
    assert(eq(vol[13],55.));
    assert(eq(vol[14],56.));
    assert(eq(vol[15],57.));
    assert(eq(vol[16],3.));
    assert(eq(vol[17],3.));
    vol = p1.getPartDemandShipVol("0000000P1414","980","980");
    assert(eq(vol[0],0.0));
    assert(eq(vol[1],0.0));
    assert(eq(vol[2],0.0));
    assert(eq(vol[3],0.0));
    assert(eq(vol[4],0.0));
    assert(eq(vol[5],0.0));
    assert(eq(vol[6],0.0));
    assert(eq(vol[7],0.0));
    assert(eq(vol[8],0.0));
    assert(eq(vol[9],0.0));
    assert(eq(vol[10],0.0));
    assert(eq(vol[11],0.0));
    assert(eq(vol[12],0.0));
    assert(eq(vol[13],55.));
    assert(eq(vol[14],0.0));
    assert(eq(vol[15],47.));
    assert(eq(vol[16],0.0)||eq(vol[16],3.));

  } // end Test low priority demand

  // Test example3 - mtm only, high/low priority demand, perishable inventory
  {
    ESO2problem p1;
    createModel(p1, "../data/example3", true );
    p1.solveWithPegging();

    std::vector<float> sv = p1.getPartDemandShipVol("p2","highPriority1","loc3");
    assert(eq(sv[3],1.0f));
    assert(eq(std::accumulate(sv.begin(),sv.end(),0.0f),1.0f));

    sv = p1.getPartDemandShipVol("p2","highPriority2","loc3");
    assert(eq(sv[4],1.0f));
    assert(eq(std::accumulate(sv.begin(),sv.end(),0.0f),1.0f));
    
    sv = p1.getPartDemandShipVol("p2","highPriority3","loc3");
    assert(eq(sv[5],1.0f));
    assert(eq(std::accumulate(sv.begin(),sv.end(),0.0f),1.0f));

    sv = p1.getPartDemandShipVol("p2","lowPriority1","loc3");
    assert(eq(std::accumulate(sv.begin(),sv.end(),0.0f),0.0f));

    sv = p1.getPartDemandShipVol("p2","lowPriority2","loc3");
    assert(eq(sv[5],1.0f));
    assert(eq(std::accumulate(sv.begin(),sv.end(),0.0f),1.0f));

    std::vector<float> aq = p1.getMtmAcquireQtyWithOffset("mtm1","invLoc1","loc2");
    assert(eq(aq[0],1.0f));
    assert(eq(aq[1],1.0f));
    assert(eq(std::accumulate(aq.begin(),aq.end(),0.0f),2.0f));

    aq = p1.getMtmAcquireQtyWithOffset("mtm1","loc1","loc2");
    assert(eq(aq[2],2.0f));
    assert(eq(std::accumulate(aq.begin(),aq.end(),0.0f),2.0f));

    std::vector<float> pi = p1.getPartInterplantMoveVolWithOffset("p2","loc2","loc3");
    assert(eq(pi[2],1.0f));
    assert(eq(pi[3],1.0f));
    assert(eq(pi[4],2.0f));
    assert(eq(std::accumulate(pi.begin(),pi.end(),0.0f),4.0f));

    double obj=p1.getObjValue();
    //assert(eq(obj,484.));
    assert(eq(obj,488.));

    // test example 3 pegging
    {
      std::vector<std::string>  items,locs; 
      std::vector<int> periods; 
      std::vector<int> depths; 
      std::vector<float> quanity; 

      p1.getPartDemandPegging("MtmAcquire",  "p2","highPriority1","loc3",4-1, items,locs,periods,depths,quanity);
      assert(items.size()==1);
      assert(depths[0]==90);
      assert( ESO2stringStringIntFloatTuple(items[0],locs[0],periods[0],quanity[0]) == ESO2stringStringIntFloatTuple("mtm1","invLoc1",1-1,1.f));

      p1.getPartDemandPegging("MtmMoveFrom",  "p2","highPriority1","loc3",4-1, items,locs,periods,depths,quanity);
      assert(items.size()==1);
      assert(depths[0]==91);
      assert( ESO2stringStringIntFloatTuple(items[0],locs[0],periods[0],quanity[0]) == ESO2stringStringIntFloatTuple("mtm1","invLoc1",1-1,1.f));

      p1.getPartDemandPegging("MtmMovedTo",  "p2","highPriority1","loc3",4-1, items,locs,periods,depths,quanity);
      assert(items.size()==1);
      assert(depths[0]==92);
      assert( ESO2stringStringIntFloatTuple(items[0],locs[0],periods[0],quanity[0]) == ESO2stringStringIntFloatTuple("mtm1","loc2",2-1,1.f));
      
      p1.getPartDemandPegging("MtmDisassemble",  "p2","highPriority1","loc3",4-1, items,locs,periods,depths,quanity);
      assert(items.size()==1);
      assert(depths[0]==140);
      assert( ESO2stringStringIntFloatTuple(items[0],locs[0],periods[0],quanity[0]) == ESO2stringStringIntFloatTuple("mtm1","loc2",2-1,1.f));
      
      p1.getPartDemandPegging("PartHarvestedFromMtm",  "p2","highPriority1","loc3",4-1, items,locs,periods,depths,quanity);
      assert(items.size()==1);
      assert(depths[0]==141);
      assert( ESO2stringStringIntFloatTuple(items[0],locs[0],periods[0],quanity[0]) == ESO2stringStringIntFloatTuple("p2","loc2",3-1,1.f));
      
      p1.getPartDemandPegging("PartMoveFrom",  "p2","highPriority1","loc3",4-1, items,locs,periods,depths,quanity);
      assert(items.size()==1);
      assert(depths[0]==620);
      assert( ESO2stringStringIntFloatTuple(items[0],locs[0],periods[0],quanity[0]) == ESO2stringStringIntFloatTuple("p2","loc2",3-1,1.f));
      
      p1.getPartDemandPegging("PartMovedTo",  "p2","highPriority1","loc3",4-1, items,locs,periods,depths,quanity);
      assert(items.size()==1);
      assert(depths[0]==621);
      assert( ESO2stringStringIntFloatTuple(items[0],locs[0],periods[0],quanity[0]) == ESO2stringStringIntFloatTuple("p2","loc3",4-1,1.f));

    } // end of test example 3 pegging

  } // end of Test example3

  // test applying Wfmv ScaleFactor by Loc using example 3
  {
    ESO2problem px;
    createModel(px, "../data/example3", true );
    px.solveWithPegging();
    double objx=px.getObjValue();
    
    ESO2problem py;
    createModel(py, "../data/example3", true );
    py.setWfmvScaleFactor(0.5f,0.5f);
    py.solveWithPegging();
    double objy=py.getObjValue();
    
    ESO2problem pa;
    createModel(pa, "../data/example3", true );
    pa.setWfmvScaleFactor(0.75f,0.75f);
    pa.setApplyLocWfmvScaleFactor("loc1",1);
    pa.setApplyLocWfmvScaleFactor("invLoc1",1);
    pa.solveWithPegging();
    double obja=pa.getObjValue();
    
    ESO2problem pb;
    createModel(pb, "../data/example3", true );
    pb.setWfmvScaleFactor(0.5f,0.5f);
    pb.setApplyLocWfmvScaleFactor("loc1",1);
    pb.setApplyLocWfmvScaleFactor("invLoc1",1);
    pb.solveWithPegging();
    double objb=pb.getObjValue();

    assert(eq(objx,objy));
    assert(objx>obja);
    assert(obja<objb);
    //double dif1=obja-objx;
    //double dif2=objb-obja;
    //assert(eq(dif1,dif2));
    
  }



  // Test example4 - feature only, high/low priority demand, perishable inventory
  {
    ESO2problem p1;
    createModel(p1, "../data/example4", true );
    p1.solveWithPegging();

    std::vector<float> sv = p1.getPartDemandShipVol("p2","highPriority1","loc3");
    assert(eq(sv[2],1.0f));
    assert(eq(std::accumulate(sv.begin(),sv.end(),0.0f),1.0f));

    sv = p1.getPartDemandShipVol("p2","highPriority2","loc3");
    assert(eq(sv[3],1.0f));
    assert(eq(std::accumulate(sv.begin(),sv.end(),0.0f),1.0f));
    
    sv = p1.getPartDemandShipVol("p2","lowPriority2","loc3");
    assert(eq(sv[4],1.0f));
    assert(eq(std::accumulate(sv.begin(),sv.end(),0.0f),1.0f));

    sv = p1.getPartDemandShipVol("p2","lowPriority1","loc3");
    assert(eq(std::accumulate(sv.begin(),sv.end(),0.0f),0.0f));


    std::vector<float> aq = p1.getFeatureAcquireQty("f1","invLoc4");
    assert(eq(aq[0],1.0f));
    assert(eq(aq[1],1.0f));
    assert(eq(std::accumulate(aq.begin(),aq.end(),0.0f),2.0f));

    aq = p1.getFeatureAcquireQty("f1","loc4");
    assert(eq(aq[2],1.0f));
    assert(eq(std::accumulate(aq.begin(),aq.end(),0.0f),1.0f));

    std::vector<float> fx = p1.getFeatureXrefVol("p2","f1","invLoc4");
    assert(eq(fx[1],1.0f));
    assert(eq(fx[2],1.0f));
    assert(eq(std::accumulate(fx.begin(),fx.end(),0.0f),2.0f));
    
    fx = p1.getFeatureXrefVol("p2","f1","loc4");
    assert(eq(fx[3],1.0f));
    assert(eq(std::accumulate(fx.begin(),fx.end(),0.0f),1.0f));

    std::vector<float> pi = p1.getPartInterplantMoveVolWithOffset("p2","invLoc4","loc3");
    assert(eq(pi[1],1.0f));
    assert(eq(pi[2],1.0f));
    assert(eq(std::accumulate(pi.begin(),pi.end(),0.0f),2.0f));

    pi = p1.getPartInterplantMoveVolWithOffset("p2","loc4","loc3");
    assert(eq(pi[3],1.0f));
    assert(eq(std::accumulate(pi.begin(),pi.end(),0.0f),1.0f));

    double obj=p1.getObjValue();
    //assert(eq(obj,1190.));
    assert(eq(obj,1193.));
    
    // test example 4 pegging
    {
      std::vector<std::string>  items,locs; 
      std::vector<int> periods;  
      std::vector<int> depths; 
      std::vector<float> quanity; 

      p1.getPartDemandPegging("MtmAcquire",  "p2","highPriority1","loc3",3-1, items,locs,periods,depths,quanity);
      assert(items.size()==0);

      p1.getPartDemandPegging("MtmMoveFrom",  "p2","highPriority1","loc3",3-1, items,locs,periods,depths,quanity);
      assert(items.size()==0);

      p1.getPartDemandPegging("MtmMovedTo",  "p2","highPriority1","loc3",3-1, items,locs,periods,depths,quanity);
      assert(items.size()==0);
      
      p1.getPartDemandPegging("MtmDisassemble",  "p2","highPriority1","loc3",3-1, items,locs,periods,depths,quanity);
      assert(items.size()==0);
      
      p1.getPartDemandPegging("PartHarvestedFromMtm",  "p2","highPriority1","loc3",3-1, items,locs,periods,depths,quanity);
      assert(items.size()==0);

      p1.getPartDemandPegging("FeatureAcquire",  "p2","highPriority1","loc3",3-1, items,locs,periods,depths,quanity);
      assert(items.size()==1);
      assert(depths[0]==20);
      assert( ESO2stringStringIntFloatTuple(items[0],locs[0],periods[0],quanity[0]) == ESO2stringStringIntFloatTuple("f1","invLoc4",1-1,1.f));

      p1.getPartDemandPegging("FeatureXrefFrom",  "p2","highPriority1","loc3",3-1, items,locs,periods,depths,quanity);
      assert(items.size()==1);
      assert(depths[0]==210);
      assert( ESO2stringStringIntFloatTuple(items[0],locs[0],periods[0],quanity[0]) == ESO2stringStringIntFloatTuple("f1","invLoc4",1-1,1.f));

      p1.getPartDemandPegging("PartXrefedTo",  "p2","highPriority1","loc3",3-1, items,locs,periods,depths,quanity);
      assert(items.size()==1);
      assert(depths[0]==211);
      assert( ESO2stringStringIntFloatTuple(items[0],locs[0],periods[0],quanity[0]) == ESO2stringStringIntFloatTuple("p2","invLoc4",2-1,1.f));
      
      p1.getPartDemandPegging("PartMoveFrom",  "p2","highPriority1","loc3",3-1, items,locs,periods,depths,quanity);
      assert(items.size()==1);
      assert(depths[0]==280);
      assert( ESO2stringStringIntFloatTuple(items[0],locs[0],periods[0],quanity[0]) == ESO2stringStringIntFloatTuple("p2","invLoc4",2-1,1.f));
      
      p1.getPartDemandPegging("PartMovedTo",  "p2","highPriority1","loc3",3-1, items,locs,periods,depths,quanity);
      assert(items.size()==1);
      assert(depths[0]==281);
      assert( ESO2stringStringIntFloatTuple(items[0],locs[0],periods[0],quanity[0]) == ESO2stringStringIntFloatTuple("p2","loc3",3-1,1.f));

    } // end of test example 4 pegging


  } // end of Test example4


  // Test example5 - loose parts only, high/low priority demand, perishable inventory
  {
    ESO2problem p1;
    createModel(p1, "../data/example5", true );
    p1.solveWithPegging();

    std::vector<float> sv = p1.getPartDemandShipVol("p2","highPriority1","loc3");
    assert(eq(sv[2],1.0f));
    assert(eq(std::accumulate(sv.begin(),sv.end(),0.0f),1.0f));

    sv = p1.getPartDemandShipVol("p2","highPriority2","loc3");
    assert(eq(sv[3],1.0f));
    assert(eq(std::accumulate(sv.begin(),sv.end(),0.0f),1.0f));
    
    sv = p1.getPartDemandShipVol("p2","lowPriority2","loc3");
    assert(eq(sv[4],1.0f));
    assert(eq(std::accumulate(sv.begin(),sv.end(),0.0f),1.0f));

    sv = p1.getPartDemandShipVol("p2","lowPriority1","loc3");
    assert(eq(std::accumulate(sv.begin(),sv.end(),0.0f),0.0f));


    std::vector<float> aq = p1.getLoosePartAcquireQty("lp1","invLoc5");
    assert(eq(aq[0],1.0f));
    assert(eq(aq[1],1.0f));
    assert(eq(std::accumulate(aq.begin(),aq.end(),0.0f),2.0f));

    aq = p1.getLoosePartAcquireQty("lp1","loc5");
    assert(eq(aq[2],1.0f));
    assert(eq(std::accumulate(aq.begin(),aq.end(),0.0f),1.0f));

    std::vector<float> ap = p1.getAltVolWithOffset("lp1","p2","invLoc5");
    assert(eq(ap[0],1.0f));
    assert(eq(ap[1],1.0f));
    assert(eq(std::accumulate(ap.begin(),ap.end(),0.0f),2.0f));
    
    ap = p1.getAltVolWithOffset("lp1","p2","loc5");
    assert(eq(ap[2],1.0f));
    assert(eq(std::accumulate(ap.begin(),ap.end(),0.0f),1.0f));

    std::vector<float> pi = p1.getPartInterplantMoveVolWithOffset("p2","invLoc5","loc3");
    assert(eq(pi[1],1.0f));
    assert(eq(pi[2],1.0f));
    assert(eq(std::accumulate(pi.begin(),pi.end(),0.0f),2.0f));

    pi = p1.getPartInterplantMoveVolWithOffset("p2","loc5","loc3");
    assert(eq(pi[3],1.0f));
    assert(eq(std::accumulate(pi.begin(),pi.end(),0.0f),1.0f));

    double obj=p1.getObjValue();
    //assert(eq(obj,1190.));
    assert(eq(obj,1193.));

    // test example 5 pegging
    {
      std::vector<std::string>  items,locs; 
      std::vector<int> periods;  
      std::vector<int> depths; 
      std::vector<float> quanity; 
      
      p1.getPartDemandPegging("FeatureAcquire",  "p2","lowPriority2","loc3",5-1, items,locs,periods,depths,quanity);
      assert(items.size()==0);

      p1.getPartDemandPegging("LoosePartAcquire",  "p2","lowPriority2","loc3",5-1, items,locs,periods,depths,quanity);
      assert(items.size()==1);
      assert(depths[0]==200);
      assert( ESO2stringStringIntFloatTuple(items[0],locs[0],periods[0],quanity[0]) == ESO2stringStringIntFloatTuple("lp1","loc5",3-1,1.f));

      p1.getPartDemandPegging("PartAltFrom",  "p2","lowPriority2","loc3",5-1, items,locs,periods,depths,quanity);
      assert(items.size()==1);
      assert(depths[0]==480);
      assert( ESO2stringStringIntFloatTuple(items[0],locs[0],periods[0],quanity[0]) == ESO2stringStringIntFloatTuple("lp1","loc5",3-1,1.f));

      p1.getPartDemandPegging("PartAltTo",  "p2","lowPriority2","loc3",5-1, items,locs,periods,depths,quanity);
      assert(items.size()==1);
      assert(depths[0]==481);
      assert( ESO2stringStringIntFloatTuple(items[0],locs[0],periods[0],quanity[0]) == ESO2stringStringIntFloatTuple("p2","loc5",4-1,1.f));
      
      p1.getPartDemandPegging("PartMoveFrom",  "p2","lowPriority2","loc3",5-1, items,locs,periods,depths,quanity);
      assert(items.size()==1);
      assert(depths[0]==550);
      assert( ESO2stringStringIntFloatTuple(items[0],locs[0],periods[0],quanity[0]) == ESO2stringStringIntFloatTuple("p2","loc5",4-1,1.f));
      
      p1.getPartDemandPegging("PartMovedTo",  "p2","lowPriority2","loc3",5-1, items,locs,periods,depths,quanity);
      assert(items.size()==1);
      assert(depths[0]==551);
      assert( ESO2stringStringIntFloatTuple(items[0],locs[0],periods[0],quanity[0]) == ESO2stringStringIntFloatTuple("p2","loc3",5-1,1.f));

    } // end of test example 5 pegging

  } // end of Test example5
  
//#define WRITE_RESULTS
#ifdef WRITE_RESULTS
  FILE * file = fopen("unitTestResults.csv","w");
  fprintf(file,"tc,perishLimit,p,sf,allowInventoryHoldingAtDemandLocation,removeCycles,objValue\n");
#else
  std::string fileName = "unitTestResults.csv";
  ESO2unitTestResultsData file(fileName.c_str());
#endif

  std::vector<std::string> testCases;
  testCases.push_back("../data/example2" );
  testCases.push_back("../data/03Apr07KevinO" );
  testCases.push_back("../data/03Apr03KevinOB" );
  testCases.push_back("../data/03Apr03KevinOA" );
  testCases.push_back("../data/03Mar31KevinO" );
  testCases.push_back("../data/testcaseA00" );
  testCases.push_back("../data/example1" );
  //testCases.push_back("../data/08JFP09base575" );
  
  std::vector<float> scaleFactors;
  scaleFactors.push_back(1.0f);
  scaleFactors.push_back(0.5f);
  scaleFactors.push_back(0.f);
  
  std::vector<int> mtmPriority;
  std::vector<int> featurePriority;
  std::vector<int> loosePartPriority;
  mtmPriority.push_back(3);
  featurePriority.push_back(1);
  loosePartPriority.push_back(2);
  mtmPriority.push_back(2);
  featurePriority.push_back(1);
  loosePartPriority.push_back(3);
  mtmPriority.push_back(2);
  featurePriority.push_back(2);
  loosePartPriority.push_back(2);
  mtmPriority.push_back(1);
  featurePriority.push_back(2);
  loosePartPriority.push_back(2);
  
  // Test pruning and assignment with varying
  // test problems, scale factors, and priorities.
  {
    int tc;
    for ( tc=0; tc<testCases.size(); ++tc ) {  

    for ( int allowInvAtDemLoc=0; allowInvAtDemLoc<2; ++allowInvAtDemLoc ) {
    
    int remCyc=allowInvAtDemLoc;
    //for ( int remCyc=0; remCyc<2; ++remCyc ) {

#if 0
      // Test getPartLocs function
      {
        ESO2problem prob;
        createModel(prob, testCases[tc], true );
        std::vector<std::string> parts = p.getParts();
        for ( part
      }
#endif
      
      //if ( tc==1 ) continue;
      //if ( tc==2 ) continue;
      //if ( tc==3 ) continue;
      //if ( tc==4 ) continue;
      //if ( tc==5 ) continue;

      int perishLimit;
      for ( perishLimit=0; perishLimit<6; perishLimit=perishLimit+3 ) {
        int p;
        for ( p=0; p<mtmPriority.size(); ++p ) {
          
          ESO2problem p1;
          ESO2problem p2;
          ESO2problem p3;
          
          p1.setPeriod1SupplyPerishPeriod(perishLimit);
          p1.setAllowInventoryHoldingAtDemandLocation(allowInvAtDemLoc);
          p1.setRemoveCycles(remCyc);
          
          bool useGlobalAttrFileSettings = false;
          createModel(p1, testCases[tc].c_str(), useGlobalAttrFileSettings );
          
          p1.setMtmPriority(mtmPriority[p]);
          p1.setFeaturePriority(featurePriority[p]);
          p1.setLoosePartPriority(loosePartPriority[p]);
          //p1.setFeaturePriority(4);
          //p1.setLoosePartPriority(4);
          
          p2=p1;
          p3=p2;
          p3.pruneWitModel();
          
          int sf;
          for ( sf=0; sf<scaleFactors.size(); ++sf ) {
            
            p1.setWfmvScaleFactor(scaleFactors[sf],scaleFactors[sf]);
            p2.setWfmvScaleFactor(scaleFactors[sf],scaleFactors[sf]);
            p3.setWfmvScaleFactor(scaleFactors[sf],scaleFactors[sf]);
            
            std::cout 
              <<"tc="<<tc 
              <<" perishLimit=" <<perishLimit
              <<" p=" <<p
              <<" sf=" <<sf
              <<std::endl;
               
            p1.solveWithPegging();
            p2.solveWithPegging();        
            p3.solveWithPegging();
            
            double p1Obj = p1.getObjValue();
            double p2Obj = p2.getObjValue();
            double p3Obj = p3.getObjValue();
            assert( p1Obj==p2Obj || tc==6 );
#if 1
            if ( p1Obj != p3Obj ) {
              p1.compareWitSolutions(p1,p3);
              //p3.reportPartDemand("p2","pok","pok",0);
              //p1.writeWitData("p1.wit");
              //p3.writeWitData("p3.wit");

            }
#endif
            assert( p1Obj==p3Obj || tc==6 );

            // determine which parts are being scrapped
            {  
              witAttr objItrState;
              witGetObjItrState(p3.witRun(), &objItrState);
              while( true )   {
                witAdvanceObjItr(p3.witRun());
                witGetObjItrState(p3.witRun(), &objItrState);
                if (objItrState==WitINACTIVE) break;
                if( objItrState == WitAT_PART ) {
                  char * name;
                  witGetObjItrPart(p3.witRun(),&name);

                  // if it is an IGF part, then OK to scrap
                  std::string stdName(name);
                  witFree(name);
                  if ( beginsWith(stdName,"igf: ") ) 
                    continue;

                  // if it is a wit modeling trick, then OK to scrap
                  if ( beginsWith(stdName,"regulator for: part: ") ) 
                    continue;
                  
                  std::vector<float> scrapVol = p3.witGetNameAttribute(witGetPartScrapVol,stdName);
                  
                  int p;
                  for( p=0; p<scrapVol.size(); p++ ) {
                    if (scrapVol[p]!=0.0) {
                      printf("Nonzero scrapVol[%d] for %s\n",p,stdName.c_str());
                      continue;
                    }
                  }
                }
              }

            }

            // Test KPI Functions
            {
              CoinRelFltEq eq(1.e-6);
              CoinRelFltEq eq5(1.e-5);
              double obj=p3.getObjValue();
              std::map<std::string,double> wacComponents;
              double wac=p3.sumOfWac(wacComponents);
              double featureWfmv=p3.sumOfFeatureWfmvWithScaleFactor();
              double loosePartWfmv=p3.sumOfLoosePartWfmvWithScaleFactor();
              double mtmWfmv=p3.sumOfMtmWfmvWithScaleFactor();
              double demanCost=p3.sumOfDemanCost();
              double mtmMoveCost=p3.sumOfMtmMoveCost();
              double partMoveCost=p3.sumOfPartMoveCost();
              double featureXrefCost=p3.sumOfFeatureXrefCost();
              double altCost=p3.sumOfAltCost();

              {
                std::map<std::string,double>::const_iterator it;
                double sumOfWacComponents=0;
                for( it=wacComponents.begin(); it!=wacComponents.end(); ++it ) {
                  sumOfWacComponents+=it->second;
                }
                assert( eq(sumOfWacComponents, wac ) );
              }
       
              double calcObj=
                wac -
                (mtmWfmv + featureWfmv + altCost + loosePartWfmv +
                demanCost +
                mtmMoveCost + partMoveCost +
                featureXrefCost);
              assert(eq5(calcObj,obj));
              
              double mtmTotalWfmvWithScale;
              double mtmSumAcquireCost;
              double mtmSumMoveCost;
              double mtmSumDemanCost;
              int mtmNumUnique;
              int mtmNumUniqueAcquired;
              double mtmSupplyQuantity;
              double mtmAcquiredQuantity;
              
              p3.mtmMetrics(
                mtmTotalWfmvWithScale,
                mtmSumAcquireCost,
                mtmSumMoveCost,
                mtmSumDemanCost,
                mtmNumUnique,
                mtmNumUniqueAcquired,
                mtmSupplyQuantity,
                mtmAcquiredQuantity);
              
              double featureTotalWfmvWithScale;
              double featureSumAcquireCost;
              double featureSumXrefCost;
              int featureNumUnique;
              int featureNumUniqueAcquired;
              double featureSupplyQuantity;
              double featureAcquiredQuantity;
              
              p3.featureMetrics(
                featureTotalWfmvWithScale,
                featureSumAcquireCost,
                featureSumXrefCost,
                featureNumUnique,
                featureNumUniqueAcquired,
                featureSupplyQuantity,
                featureAcquiredQuantity);

              double loosePartTotalWfmvWithScale;
              double loosePartSumAcquireCost;
              double loosePartAltSumCost;
              int loosePartNumUnique;
              int loosePartNumUniqueAcquired;
              double loosePartSupplyQuantity;
              double loosePartAcquiredQuantity;
              
              p3.loosePartMetrics(
                loosePartTotalWfmvWithScale,
                loosePartSumAcquireCost,
                loosePartAltSumCost,
                loosePartNumUnique,
                loosePartNumUniqueAcquired,
                loosePartSupplyQuantity,
                loosePartAcquiredQuantity);
              
              double partDemandSumDemandWac;
              double partDemandSumShippedWac;
              int partDemandNumUnique;
              int partDemandNumUniqueShipped;
              double partDemandSumDemandVol;
              double partDemandSumShipVol;
              std::map<std::string,double> sumShippedWacComponents;
              
              p3.partDemandMetrics(
                partDemandSumDemandWac,
                partDemandSumShippedWac,
                sumShippedWacComponents,
                partDemandNumUnique,
                partDemandNumUniqueShipped,
                partDemandSumDemandVol,
                partDemandSumShipVol);
            }

            
#ifdef WRITE_RESULTS
            {
              fprintf(file,"%d,%d,%d,%d,%d,%d,%f\n",tc,perishLimit,p,sf,allowInvAtDemLoc,remCyc,p1Obj);
            }
#else
            {
              float historicObjValue = file.objValueAsFloat(file.key(tc,perishLimit,p,sf,allowInvAtDemLoc,remCyc));
              assert( eq(p1Obj,historicObjValue) );
              //assert( eq(p1Obj,historicObjValue) || tc==5 );
            }
#endif
          } // end for sf loop  
        } // end for p loop
      } // end for perishLimit loop
    //} // end for remCyc loop
    } // end for allowInvAtDemLoc loop
    } // end for tc loop
  }
  
#ifdef WRITE_RESULTS
  fclose(file);
#endif
  
}


