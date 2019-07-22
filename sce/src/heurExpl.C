
// RW2STL - inserted:
#include <string>
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <sce/src/heurExpl.h>
#include <iostream>
//#include <fstream.h>
#include <sce/src/appData.h>
#include <sce/src/bopAData.h>

// #define FULL_DEBUG


// states:
//   0: uninitialized (miniWitRun not populated)
//   1: initiailized (miniWitRun populated, no explosion solution)
//   2: Explosion solution 


void 
LgFrSceOptExploder::setAllSupplyToZero()
{
  int nPeriods;
  witGetNPeriods(localWitRun_, &nPeriods);

  int nParts;
  char ** partList;
  witGetParts(localWitRun_, &nParts, &partList);
  float * zeroVec = new float[nPeriods];
  int t = 0; // Pulled out of the for below by RW2STL
  for (t=0; t<nPeriods; t++)
    zeroVec[t] = 0;
  
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
    witSetPartSupplyVol(localWitRun_, partList[i], zeroVec);
    witFree(partList[i]);
  }

  witFree(partList);
  delete [] zeroVec;
}

void 
LgFrSceOptExploder::setAllDemandToZero()
{
  int nPeriods;
  witGetNPeriods(localWitRun_, &nPeriods);

  int nParts;
  char ** partList;
  witGetParts(localWitRun_, &nParts, &partList);
  float * zeroVec = new float[nPeriods];
  int t = 0; // Pulled out of the for below by RW2STL
  for (t=0; t<nPeriods; t++)
    zeroVec[t] = 0;
  
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
    int nDemands;
    char ** dlist;
    witGetPartDemands(localWitRun_, partList[i], &nDemands, &dlist);
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<nDemands; j++) {
      witSetDemandDemandVol(localWitRun_, partList[i], dlist[j], zeroVec);
      witFree(dlist[j]);      
    }
    witFree(dlist);
    witFree(partList[i]);
  }
  witFree(partList);
  delete [] zeroVec;
}

WitRun *
LgFrSceOptExploder::giveMeYourWitRun()
{
  return localWitRun_;
}

  // get the mrpProdVol
// mrpProdVol = 0,      if part is a PCF
//            = prodVol if part is NOT a PCF

int
LgFrSceOptExploder::mrpProdVol(const char * part,
                                float ** mrpProdVolPtr)
{
  //  assert(state_ >= 2);

  int isPartPcf = 0;
  
  witBoolean exists;
  witGetPartExists(localWitRun_, part, &exists);
  if (exists) {
    LgFrScePartAppData * partAppDataPtr;    
    witGetPartAppData(localWitRun_, part, (void **) &partAppDataPtr);
    if (partAppDataPtr != 0) {
      isPartPcf =  partAppDataPtr->isPartPcf();
    }
  }

  if (! isPartPcf) { 
     witGetPartProdVol(localWitRun_, part, mrpProdVolPtr);    
   }

  else {
    int nPeriods;
    witGetNPeriods(localWitRun_, &nPeriods);
    float * zeroVec = (float *) malloc (nPeriods * sizeof(float));
    assert(zeroVec != 0);
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nPeriods; t++)
      zeroVec[t] = 0.0;
    (*mrpProdVolPtr) = zeroVec;
  }
  return 0;
}

// --------------
// get the reqVol
// --------------
// reqVol = 0, if part is NOT PCF
//        = prodVol if part is a PCF
int
LgFrSceOptExploder::reqVol(const char * part,
                            float ** reqVolPtr)
{

  int isPartPcf = 0;
  
  witBoolean exists;
  witGetPartExists(localWitRun_, part, &exists);
  if (exists) {
    LgFrScePartAppData * partAppDataPtr;    
    witGetPartAppData(localWitRun_, part, (void **) &partAppDataPtr);
    if (partAppDataPtr != 0) {
      isPartPcf =  partAppDataPtr->isPartPcf();
    }
  }

  if (isPartPcf) { 
     witGetPartProdVol(localWitRun_, part, reqVolPtr);    
   }
  else {
    int nPeriods;
    witGetNPeriods(localWitRun_, &nPeriods);
    float * zeroVec = (float *) malloc (nPeriods * sizeof(float));
    assert(zeroVec != 0);
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nPeriods; t++)
      zeroVec[t] = 0.0;
    (*reqVolPtr) = zeroVec;
  }
  
  return 0;
}

  // get the past due reqVol
int
LgFrSceOptExploder::pastDueReqVol(const char * part,
                                   float * pastDueReqVolPtr )
{
  (*pastDueReqVolPtr) = 0;
  return 0;
}

  // get the  reqVol with past due reqVol added to period 0
  // we don't ave reqVol with past due, so we just call
  // the regular one
int
LgFrSceOptExploder::reqVolWithPastDue(const char * part,
                                       float ** reqVolPtr )
{
  this->reqVol(part, reqVolPtr);
  return 0;
}


  // get the mrpConsVol
int
LgFrSceOptExploder::mrpConsVol(const char * part,
                                float ** mrpConsVolPtr)
{
  //  assert(state_ >= 2);
  witBoolean exists;
  witGetPartExists(localWitRun_, part, &exists);
  if (exists) {
    witGetPartConsVol(localWitRun_, part, mrpConsVolPtr);    
  }
  else {
    int nPeriods;
    witGetNPeriods(localWitRun_, &nPeriods);
    float * zeroVec = (float *) malloc (nPeriods * sizeof(float));
    assert(zeroVec != 0);
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nPeriods; t++)
      zeroVec[t] = 0.0;
    (*mrpConsVolPtr) = zeroVec;
  }
  
  return 0;
}

  // get the past due mrpConsVol
int
LgFrSceOptExploder::pastDueMrpConsVol(const char * part,
                                       float * pastDueMrpConsVolPtr )
{
  *pastDueMrpConsVolPtr = 0;
  return 0;

}

  // get the  mrpConsVol with past due mrpConsVol added to period 0
int
LgFrSceOptExploder::mrpConsVolWithPastDue(const char * part,
                                           float ** mrpConsVolPtr )
{
  this->mrpConsVol(part, mrpConsVolPtr);
  return 0;
}





  // get the mrpExcessVol
int
LgFrSceOptExploder::mrpExcessVol(const char * part,
                                  float ** mrpExcessVolPtr)
{
  assert(state_ >= 2);
  witBoolean exists;
  witGetPartExists(localWitRun_, part, &exists);  
  if (exists) {
    witGetPartExcessVol(localWitRun_, part, mrpExcessVolPtr);
  }
  else {
    int nPeriods;
    witGetNPeriods(localWitRun_, &nPeriods);
    float * zeroVec = (float *) malloc (nPeriods * sizeof(float));
    assert(zeroVec != 0);
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nPeriods; t++)
      zeroVec[t] = 0.0;
    (*mrpExcessVolPtr) = zeroVec;
  }
  
  return 0;
}

// determine if this part is something that you deal with
witBoolean
LgFrSceOptExploder::exists(const char * part)
{
  witBoolean exists;
  witGetPartExists(localWitRun_, part, &exists);  
  return exists;
}




  // preferred constructor
LgFrSceOptExploder::LgFrSceOptExploder(WitRun * originalWitRun,
                                               LgFrMultiPlantHelper & mpHelper)
  :
  originalWitRun_(originalWitRun),
  localWitRun_(0),
  state_(0)
{  
  witNewRun(&localWitRun_);

  /* turn off a bunch off annoying messages */
  witSetMesgTimesPrint(localWitRun_, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
  witSetMesgTimesPrint(localWitRun_, WitTRUE, 82, 0);
  witSetMesgTimesPrint(localWitRun_, WitTRUE, 338, 0);
  witSetMesgTimesPrint(localWitRun_, WitTRUE, 538, 0);
  witSetMesgTimesPrint(localWitRun_, WitTRUE, 505, 0);        
  // see just "how much" you violate a constraint
  // when you are technically "infeasible"
  witSetMesgTimesPrint(localWitRun_, WitTRUE, 684, 10);        
  
  this->populateLocalWitRun(mpHelper);

  // ok, we're initialized, but not post processed.
  assert(state_ == 1);
  
}


void
LgFrSceOptExploder::smartCopyData(WitRun * const lhsWitRun,
                                      WitRun * const rhsWitRun,
                                      LgFrMultiPlantHelper & mpHelper)
{

  
  int nParts;
  char ** partList;
  witGetParts(rhsWitRun, &nParts, &partList);

  int nOps;
  char ** opList;
  witGetOperations(rhsWitRun, &nOps, &opList);  

  int nPeriods;
  witGetNPeriods(rhsWitRun, &nPeriods);

  // ========= part Attributes  ==========
  float * supplyVol;
  //      --- products only ----
  float * cycleTime;

  // ========= Operation Attributes  ==========
  float * incLotSize;
  float * minLotSize;
  float   * yieldRate;
  

  // ========= BOM attributes ===========
  char * child;
  float * consRate;
  float * offset;
  int fallout;
  witBoolean mandEC;
  int early;
  int late;

  // ========= BOP attributes ==================
  char * producedPart;
  // witBoolean byproduct;
  float prodRate;

  // ========= SUB attributes ==================
  char * sub;

  // ==========  Demand attributes =============
  float * demandVol;

  
  
  // First set up the Parts ...
  // filter out parts and structure that is not necessary for
  // MRP.
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
    char * thePart = partList[i];
    std::string sub(thePart);
    if (mpHelper.isPartSpecialNullSubstitute(rhsWitRun, sub)) {
      continue;
    }

    witAttr category34;
    witAttr category40;	
    witGetPartCategory(rhsWitRun, thePart, &category34);
	// rhs witrun is 3.4 and lhs is 4.0
	// need to check category != PRODUCT	
	if (category34 == WitPRODUCT || category34 == WitRAW )
		category40 = WitMATERIAL ;	
	else if (category34 == WitCAPACITY)
	// FINISH_ME: do the right modeling for capacity
		continue;
    witAddPart(lhsWitRun, thePart, category40);
    // copy the appData if there is one
    LgFrScePartAppData * appDataPtr;
    witGetPartAppData(rhsWitRun, thePart, (void **) &appDataPtr);
    if (appDataPtr != 0) {
      LgFrScePartAppData * newAppDataPtr = new LgFrScePartAppData(*appDataPtr);
      witSetPartAppData(lhsWitRun, thePart, (void *) newAppDataPtr);
    }
    witGetPartSupplyVol(rhsWitRun, thePart, &supplyVol);
    witSetPartSupplyVol(lhsWitRun, thePart, supplyVol);
    witFree(supplyVol);      
    

    // ==== Now add any Demands ====
    if (category40 == WitMATERIAL) {
      int nDemands;
      char ** dList;
      witGetPartDemands(rhsWitRun, thePart, &nDemands, &dList);
      int d = 0;
      for (d=0; d<nDemands; d++) {
        witGetDemandDemandVol(rhsWitRun, thePart, dList[d], &demandVol);
        witAddDemand(lhsWitRun, thePart, dList[d]);
        witSetDemandDemandVol(lhsWitRun, thePart, dList[d], demandVol);
        witFree(demandVol);
        witFree(dList[d]);
      }
      witFree(dList);
    }

  }

  // Now loop over the operations ...
  int o = 0; // Pulled out of the for below by RW2STL
  for (o=0; o<nOps; o++) {
    char * theOperation = opList[o];

    // special considerations for operations:
    //    NONE (at this time).
    //   We will filter bop's based on the appData
    //   can we explode boolean.  AFter we do that
    //   We may determine that there are some default
    //    operations that should be turned back on for explosion.
  
    witGetOperationIncLotSize(rhsWitRun, theOperation, &incLotSize);
    witGetOperationMinLotSize(rhsWitRun, theOperation, &minLotSize);
    witGetOperationYieldRate(rhsWitRun, theOperation, &yieldRate);
    
    witAddOperation(lhsWitRun, theOperation);
    
    witSetOperationIncLotSize(lhsWitRun, theOperation, incLotSize);
    witSetOperationMinLotSize(lhsWitRun, theOperation, minLotSize);
    witSetOperationYieldRate(lhsWitRun, theOperation, yieldRate);
    witFree(incLotSize);
    witFree(minLotSize);
    witFree(yieldRate);

    int nbBops;
    int b_lhs=0;
    witGetOperationNBopEntries(rhsWitRun, theOperation, &nbBops);
    int b = 0; // Pulled out of the for below by RW2STL
    for (b=0; b<nbBops; b++) {

      // Look at BOP app data ...
      LgFrSceBopAppData * appData;
      witGetBopEntryAppData(rhsWitRun, theOperation, b, (void **) &appData);
      // if the appData object exists then  use the isObjectInEffectForOptExplosion
      // flag to determine whether or not to keep it
      if (appData != 0) {
        if (! appData->isObjectInEffectForOptExplosion()) 
          continue;
      }
      // if there is no appData, then assume that we want this bop entry
        
      witGetBopEntryProducedPart(rhsWitRun, theOperation, b, &producedPart);
      // make sure the produced part exists before adding a bop entry.
      // it may not exist for valid reasons.  Simply accept the fact and move on
      if (! exists(producedPart)) {
        witFree(producedPart);
        continue;
      }


      // DO not copy the byproduct attribute.  Let it default to false.
      // It will be re-analyzed later and set accordingly.
      // FINISH_ME: For now we DO copy the byProduct.  Once we get
      // the HeurModelHelper, we can use that to set byProduct.  For
      // now we'll use them the way they are.
      witBoolean byProduct;
      witGetBopEntryByProduct(rhsWitRun, theOperation, b, &byProduct);
      witGetBopEntryEarliestPeriod(rhsWitRun, theOperation, b, &early);
      witGetBopEntryLatestPeriod(rhsWitRun, theOperation, b, &late);
      witGetBopEntryOffset(rhsWitRun, theOperation, b, &offset);
      witGetBopEntryProdRate(rhsWitRun, theOperation, b, &prodRate);


      witAddBopEntry(lhsWitRun, theOperation, producedPart);

      // set the appData (if it exists)
      if (appData != 0) {
        LgFrSceBopAppData * newAppData = new LgFrSceBopAppData(*appData);
        witSetBopEntryAppData(lhsWitRun, theOperation, b_lhs, (void *) newAppData);
      }
      // FINISH_ME: make sure you remove this line when you implement
      // the HeurModelHelper in this object.
      witSetBopEntryByProduct(lhsWitRun, theOperation, b_lhs, byProduct);            
      witSetBopEntryEarliestPeriod(lhsWitRun, theOperation, b_lhs, early);
      witSetBopEntryLatestPeriod(lhsWitRun, theOperation, b_lhs, late);
      witSetBopEntryOffset(lhsWitRun, theOperation, b_lhs, offset);
      witSetBopEntryProdRate(lhsWitRun, theOperation, b_lhs, prodRate);

      b_lhs++;
      witFree(producedPart);    
    }
      
    // -----------------------------
    // Now copy all the  bom records
    // -----------------------------    
    int nBom;
    int j_lhs=0;
    witGetOperationNBomEntries(rhsWitRun, theOperation, &nBom);
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<nBom; j++) {
      // get ...
      witGetBomEntryConsumedPart(rhsWitRun, theOperation, j, &child);

      // check for existence of the child, first
      witBoolean exists;
      witGetPartExists(rhsWitRun, child, &exists);
      if (! exists) {
        witFree(child);
        continue;
      }
      
      witGetBomEntryConsRate(rhsWitRun, theOperation, j, &consRate); 
      witGetBomEntryOffset(rhsWitRun, theOperation, j, &offset); 
      witGetBomEntryEarliestPeriod(rhsWitRun, theOperation, j, &early); 
      witGetBomEntryLatestPeriod(rhsWitRun, theOperation, j, &late);
      witGetBomEntryFallout(rhsWitRun, theOperation, j, &fallout);
      witGetBomEntryMandEC(rhsWitRun, theOperation, j, &mandEC);

      // add ... only if it's not capacity
      witAttr category34;
      witGetPartCategory ( rhsWitRun, child, &category34 );
      if ( category34 != WitCAPACITY )
        witAddBomEntry(lhsWitRun, theOperation, child);
      else if (category34 == WitCAPACITY)
	// FINISH_ME: do the right modeling for capacity
		continue;
      // set ...
      witSetBomEntryConsRate(lhsWitRun, theOperation, j_lhs, consRate); 
      witSetBomEntryEarliestPeriod(lhsWitRun, theOperation, j_lhs, early); 
      witSetBomEntryLatestPeriod(lhsWitRun, theOperation, j_lhs, late);
      witSetBomEntryFallout(lhsWitRun, theOperation, j_lhs, fallout);
      witSetBomEntryMandEC(lhsWitRun, theOperation, j_lhs, mandEC);

      // modify offsets for "full explode" capability
      int t = 0; // Pulled out of the for below by RW2STL
      for (t=0; t<nPeriods; t++)  
        if (offset[t] > (float) t)  
          offset[t] = t;
      witSetBomEntryOffset(lhsWitRun, theOperation, j_lhs, offset);
      witFree(offset);

      
      // ----------------------
      // Now do the Substitutes
      // ----------------------
      int nSubs;
      // note: becuase we filter out some records, we need to keep a seperate
      // subIndex of the sub arc in hte lhs witRun
      int s_lhs=0;
      witGetBomEntryNSubsBomEntries(rhsWitRun, theOperation, j, &nSubs);
      int s = 0; // Pulled out of the for below by RW2STL
      for (s=0; s<nSubs; s++) {

        // Look at sub app data ...
        LgFrSceSubBomAppData * appData;
        witGetSubsBomEntryAppData(rhsWitRun, theOperation, j, s, (void **) &appData);
        // if the appData object exists then  use the isObjectInEffectForOptExplosion
        // flag to determine whether or not to keep it
        if (appData != 0) {
          if (! appData->isObjectInEffectForOptExplosion()) 
            continue;
        }
        //  if there is NO sub appData, then assume isObjectInEffectForOptExplosion=false
        else
            continue;          

        witGetSubsBomEntryConsumedPart(rhsWitRun, theOperation, j, s, &sub);
        // check to see that the sub part exists
        // check for existence of the sub
        witBoolean exists;
        witGetPartExists(rhsWitRun, sub, &exists);
        if (! exists) {
          witFree(sub);
          continue;
        }
        
        
        witGetSubsBomEntryEarliestPeriod(rhsWitRun, theOperation, j, s, &early);
        witGetSubsBomEntryLatestPeriod(rhsWitRun, theOperation, j, s, &late);
        witGetSubsBomEntryFallout(rhsWitRun, theOperation, j, s, &fallout);
        witGetSubsBomEntryConsRate(rhsWitRun, theOperation, j, s, &consRate);

        // add ... but only if it's not a capacity
        witAttr category34;
        witGetPartCategory(rhsWitRun, sub, &category34);
        if (category34 != WitCAPACITY )
          witAddSubsBomEntry(lhsWitRun, theOperation, j_lhs, sub);
	else if (category34 == WitCAPACITY)
	// FINISH_ME: do the right modeling for capacity
		continue;



        // give this subbom  new appdata for later ...
        LgFrSceSubBomAppData * newAppData = new LgFrSceSubBomAppData(*appData);
        witSetSubsBomEntryAppData(lhsWitRun, theOperation, j_lhs, s_lhs, (void *) newAppData);
        // and set the rest of the crap
        witSetSubsBomEntryEarliestPeriod(lhsWitRun, theOperation, j_lhs, s_lhs, early);
        witSetSubsBomEntryLatestPeriod(lhsWitRun, theOperation, j_lhs, s_lhs, late);
        witSetSubsBomEntryFalloutRate(lhsWitRun, theOperation, j_lhs, s_lhs, fallout);
        witSetSubsBomEntryConsRate(lhsWitRun, theOperation, j_lhs, s_lhs, consRate);
	witFree(consRate);

        // increment the subbom index count for lhs
        s_lhs++;
        witFree(sub);
      }
      // increment the bom index count  for lhs
      j_lhs++;
      witFree(child);
    }

  }
      
    
  // free the parts 
  for (i=0; i<nParts; i++)  
    witFree(partList[i]);
  witFree(partList);

  // free the Operations 
  for (o=0; o<nOps; o++)  
    witFree(opList[o]);
  witFree(opList);
  
}




  // copy constructor
LgFrSceOptExploder::LgFrSceOptExploder( const LgFrSceOptExploder& source )
{
  assert(0 == "Do not use this method");
}
  
  // assignment operator
LgFrSceOptExploder&
LgFrSceOptExploder::operator=(const LgFrSceOptExploder& rhs) {
  assert(0 == "Do not use this method");
  return *this;
}

  
  // destructor
LgFrSceOptExploder::~LgFrSceOptExploder()
{
  // delete any appDAta's
  // delete bop app data
  char ** operationList;
  int nOps;
  int b, o; 
  
  witGetOperations ( localWitRun_, &nOps, &operationList );
  for (o=0; o<nOps; o++) {
    int  nbBops;   
    witGetOperationNBopEntries ( localWitRun_, operationList[o], & nbBops );
    for (b=0; b<nbBops; b++) {
      LgFrSceBopAppData * bopAppDataPtr;
      witGetBopEntryAppData ( localWitRun_, operationList[o], b, (void **) & bopAppDataPtr );
      delete bopAppDataPtr ;
    } // for b nbBops
  } // for o operations

  // delete subBom App Data
  for (o=0; o<nOps; o++) {
    int nBoms;
    int nSubBoms;
    witGetOperationNBomEntries( localWitRun_, operationList[o], &nBoms );
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<nBoms; j++) {
      witGetBomEntryNSubsBomEntries( localWitRun_, operationList[o], j, &nSubBoms );
      int s = 0; // Pulled out of the for below by RW2STL
      for (s=0; s<nSubBoms; s++) { 
        LgFrSceSubBomAppData * subBomAppDataPtr;
        witGetSubsBomEntryAppData( localWitRun_, operationList[o], j, s, (void **) &subBomAppDataPtr );
        delete subBomAppDataPtr;
      } // for s nSubBoms
    } // for j nBoms
  } // for o nOperations
  
  for (o=0; o<nOps; o++)
    witFree (operationList[o]);
  witFree (operationList);

  // delete part app data
  char ** partList;
  int nParts;
  
  witGetParts ( localWitRun_, &nParts, &partList );
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
    LgFrScePartAppData * partAppDataPtr;
    witGetPartAppData ( localWitRun_, partList[i], (void **) &partAppDataPtr );
    delete partAppDataPtr;
  } 

  for (i=0; i<nParts; i++)
    witFree (partList[i]);
  witFree (partList);
  
  witDeleteRun(localWitRun_);
}




void
LgFrSceOptExploder::print(char * filename)
{
  assert(state_ >= 2);

  std::ofstream outFile(filename);//, ios::out);
  int nPeriods;
  witGetNPeriods(originalWitRun_, &nPeriods);

  int nParts;
  char ** partList;
  witGetParts(originalWitRun_, &nParts, &partList);

  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
    char * thePart = partList[i];
    witAttr category;
    witGetPartCategory(originalWitRun_, thePart, &category);

    // pastDue demand
    float pastDueMrpConsVol;
    this->pastDueMrpConsVol(thePart, &pastDueMrpConsVol);
    outFile << "\"" << thePart << "\", pastDueMrpConsVol\", " << pastDueMrpConsVol;
    outFile << "\n";
    
    outFile << "\"" << thePart << "\", mrpConsVol\"";
    float * mrpConsVol;    
    this->mrpConsVol(thePart, &mrpConsVol);    
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nPeriods; t++) 
      outFile << "," << mrpConsVol[t];
    outFile << "\n";
    witFree(mrpConsVol);

    if (category == WitPRODUCT) {
      float * mrpProdVol;
      this->mrpProdVol(thePart, &mrpProdVol);
      outFile << "\"" << thePart << "\", mrpProdVol\" ";
      for (t=0; t<nPeriods; t++) 
        outFile << "," << mrpProdVol[t];
      outFile << "\n";
      witFree(mrpProdVol);
    }

    float * mrpExcessVol;
    this->mrpExcessVol(thePart, &mrpExcessVol);
    outFile << "\"" << thePart << "\", mrpExcessVol\" ";
    for (t=0; t<nPeriods; t++) 
      outFile << "," << mrpExcessVol[t];
    outFile << "\n";
    witFree(mrpExcessVol);

    float pastDueReqVol;
    this->pastDueReqVol(thePart, &pastDueReqVol);
    outFile << "\"" << thePart << "\", pastDueReqVol\", " << pastDueReqVol;
    outFile << "\n";


    
    float * reqVol;
    this->reqVol(thePart, &reqVol);        
    outFile << "\"" << thePart << "\", reqVol\" ";
    for (t=0; t<nPeriods; t++) 
      outFile << "," << reqVol[t];
    outFile << "\n";
    witFree(reqVol);

  }
  outFile.close();

  for (i=0; i<nParts; i++)
    witFree(partList[i]);
  witFree(partList);
  
}

void
LgFrSceOptExploder::populateLocalWitRun(LgFrMultiPlantHelper & mpHelper)
{

  assert(state_ == 0);
  witInitialize(localWitRun_);

  // strictly 4.0 mode
  witSetWit34Compatible(localWitRun_, WitFALSE);
  

  // -----------------------------------------
  // STEP 1: Set up Some Global Wit Attributes
  // -----------------------------------------  
  // Set the nPeriods  
  int nPeriods;
  witGetNPeriods(originalWitRun_, &nPeriods);
  witSetNPeriods(localWitRun_, nPeriods);

  witSetExecEmptyBom(localWitRun_, WitTRUE);

  // FINISH_ME: When we implement LP optimization here
  // then objValue needs to be set accordingly.
  int objValue = 0;
  witSetObjChoice(localWitRun_, objValue);

  // FINISH_ME: Set equitability to 100 for now.  Eventually, it
  // should be yet another parameter
  witSetEquitability(localWitRun_, 100);

  // ----------------------------
  // STEP 2: Copy over the witRun
  // ----------------------------  
  // This  is a not-too-terribly-complicated copy of the witRun.
  // Here are some key points:
  //    1) some parts are fitlered out (special maxwithout nullsubs)
  //    2) Because some parts are missing, we need to be real careful
  //       about copying bom's, bop's. and subbom's.  We must also
  //       be careful about indexing, since bop j does not necessarily
  //       correspond to bop j in the copy because you may not have copied
  //       bop j-i, say.
  //    3) Any bop preferences or subbom preferences are ignored.
  //    4) Subs and bops are filtered as follows:
  //          if a BOP has no app data, then DO copy it over
  //          if a SUBBOM has NO app data, then DO NOT copy it over
  //          otherwise we respect isObjectInEffectForOptExplosion.
  //    5) All BOP byProduct flags are set to false.  they will be
  //       reset in step 3 when preferences are considered.
  smartCopyData(localWitRun_, originalWitRun_, mpHelper);

  // ---------------------------------
  // STEP 3: Restructure for Implosion
  // ---------------------------------
  // In this step we restructure the model so that an implosion
  // can accurately represent an explosion
  // Here are some assumptions:
  //   1) set effectivities back to full for pcf default bops
  // FINISH_ME: ahem ... implement me please!
  restructureForImplosionToSolveExplosion();

  
  // ----------------------------------
  // STEP 4: Do Robin's split BOP thing 
  // ----------------------------------
  // The Final step is to do the exact same processing of BOP entries
  // that is done in initialProblem.  This is where we look at
  // bop and subbom preferences.
  // FINISH_ME: For now we will strictly use heuristic implosion.  For
  //      LP, this step is unnecessary.  Of course, other steps are.
  //      WAhhh! I wanna go home.
  // FINISH_ME: implemnt this.  we do not need this for the MD
  // alpha run.
  // doRobinsBopThing()
  
  state_ = 1;
}


void
LgFrSceOptExploder::restructureForImplosionToSolveExplosion()
{
  int nParts;
  char ** partList;
  witGetParts(localWitRun_, &nParts, &partList);

  int nOps;
  char ** opList;
  witGetOperations(localWitRun_, &nOps, &opList);  

  int nPeriods;
  witGetNPeriods(localWitRun_, &nPeriods);

  // FINISH_ME: Must also set yield to some non-zero value
  // whenever it was shut off because of cycleTime
  
  // Look for parts that are PCF
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
    char * thePart = partList[i];

    LgFrScePartAppData * partAppData;
    witGetPartAppData(originalWitRun_, thePart, (void **) &partAppData);
    if (partAppData == 0)
      continue;

    if (! partAppData->isPartPcf())
      continue;

    // Yeah!! we found a bonafide PCF part.

    // find its default operation.
    std::string defaultOperation(thePart);
    witBoolean exists;
    witGetOperationExists(localWitRun_, defaultOperation.c_str(), &exists);
    assert(exists);

    float * yieldRate;
    witGetOperationYieldRate(localWitRun_, defaultOperation.c_str(), &yieldRate);    
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<partAppData->unConstrainedStartPeriod(); t++) {
      yieldRate[t] = 1.0;
    }
    witSetOperationYieldRate(localWitRun_, defaultOperation.c_str(), yieldRate);
    witFree(yieldRate);
    

    // find the bop entry that leads to the default operation
    // set effectivities on and byProduct off
    int nbBops;
    char * producedPart;
    witGetOperationNBopEntries(localWitRun_, defaultOperation.c_str(), &nbBops);
    int b = 0; // Pulled out of the for below by RW2STL
    for (b=0; b<nbBops; b++) {
      witGetBopEntryProducedPart(localWitRun_, defaultOperation.c_str(), b, &producedPart);
      if (producedPart == defaultOperation) {
        witSetBopEntryByProduct(localWitRun_, defaultOperation.c_str(), b, WitFALSE);            
        witSetBopEntryEarliestPeriod(localWitRun_, defaultOperation.c_str(), b, 0);
        witSetBopEntryLatestPeriod(localWitRun_, defaultOperation.c_str(), b, nPeriods-1);
      }
      witFree(producedPart);
    }
    
  }
  
  // free the parts 
  for (i=0; i<nParts; i++)  
    witFree(partList[i]);
  witFree(partList);

  // free the Operations 
  int o = 0; // Pulled out of the for below by RW2STL
  for (o=0; o<nOps; o++)  
    witFree(opList[o]);
  witFree(opList);
  
}


// set supplyVols and demandVols to the current
// setting in originalWitRun_.  this is needed for
// FSS users.
void
LgFrSceOptExploder::synchronize()
{

  // must be in initialized state
  assert(state_ > 0);

  int nParts;
  char ** partList;
  witGetParts(localWitRun_, &nParts, &partList);
  
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++)  {
    char * thePart = partList[i];
    float * supplyVol;
    witGetPartSupplyVol(originalWitRun_, thePart, &supplyVol);
    witSetPartSupplyVol(localWitRun_, thePart, supplyVol);
    witFree(supplyVol);
    int nDemands;
    char ** demandList;
    witGetPartDemands(originalWitRun_, thePart, &nDemands, &demandList);
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<nDemands; j++)   {
      float * demandVol;
      witGetDemandDemandVol(originalWitRun_, thePart, demandList[j], &demandVol);
      witSetDemandDemandVol(localWitRun_, thePart, demandList[j], demandVol);
      witFree(demandVol);
      witFree(demandList[j]);
    }
    witFree(demandList);
  }

  for (i=0; i<nParts; i++)
    witFree(partList[i]);
  witFree(partList);
  
}



int
LgFrSceOptExploder::explode()
{

  // must be in initialized state
  assert(state_ > 0);

  // run the mrp on the localWitRun 
  witHeurImplode(localWitRun_);

#ifdef FULL_DEBUG
  witSetMesgFileAccessMode(localWitRun_, WitTRUE, "w");
  witSetMesgTimesPrint(localWitRun_, WitTRUE, WitINFORMATIONAL_MESSAGES, UCHAR_MAX);
  witDisplayData(localWitRun_, "localWitRun.optExpl.echo");
  witWriteData(localWitRun_, "localWitRun.optExpl.witData");
  witSetMesgTimesPrint(localWitRun_, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
#endif  
    
  state_ = 2;
  
  return 0;
}



#ifdef NDEBUG
#undef NDEBUG
#endif

void
LgFrSceOptExploder::test()
{



} 

