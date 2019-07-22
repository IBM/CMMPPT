
// file: itHelpr.C
// author: robin

#include <stdlib.h>
#include <assert.h>
#include <sce/src/itHelpr.h>
#include <scenario/src/machdep.h>
#include <sce/src/sceDefin.h>
#include <scenario/src/timeVecs.h>


#define ITHELPER_FLT_EPS  0.00001

#ifndef ELIMINATE_OLD_MAIN
// make a working copy of a witRun and set the messaging of the copy
WitRun *
LgFrSceIterativeImpHelper::copyWitRunWithMsging( WitRun * const theOrigWitRun,
                                                 int printInformationalWitMessages,
                                                 const char * witMessageFilename )
{
  // create a new witRun
  WitRun * theWorkingWitRun;
  witNewRun( & theWorkingWitRun );
  
  // use the witMessageFilename that was passed in, if any
  if ( witMessageFilename != 0 ) 
    witSetMesgFileName(theWorkingWitRun,
                       WitTRUE,
                       witMessageFilename );
  
  // don't print informational messages unless they're wanted
  if ( !printInformationalWitMessages ) 
    witSetMesgTimesPrint( theWorkingWitRun,
                          WitTRUE,
                          WitINFORMATIONAL_MESSAGES,
                          0 );
  
  // Don't print the warnings we know about -- no need to scare users
  witSetMesgTimesPrint( theWorkingWitRun,
                        WitTRUE,
                        605,
                        0 ); //negative prod costs warning
  witSetMesgTimesPrint( theWorkingWitRun,
                        WitTRUE,
                        338,
                        0 ); //input names too long
  
  // make a working copy of the original
  witCopyData( theWorkingWitRun, theOrigWitRun );

  return theWorkingWitRun;
}


// FIX_ME:  1) Merge copyMinLotSizes() with getMinlotSizeUsed()
//          2) Need consistent method naming for itHelper class!!
//          3) Are the supplyVols ever being set back to their original values?
//          4) A good sanity check ... Do a witDisplayData() before and after
//             running implode().  The two should 'diff' with no errors!!
//          5) We do not need to use the rollover Message file attribute anymore.
//          6) change setSupplyVolToExcessVol() method name to something about updating with excessVol.
//          7) Be careful about doing float comparisons with zero (#define a FLT_EPS or something)
//          8) test equitability
//

// Create a vector of minLotSize's, and a vector of booleans that describe
// whether a part has a min lot size, and a global flag indicating whether
// ANY part has a minLotSize.  These vectors must be freed by the client! 
// 
void
LgFrSceIterativeImpHelper::getMinLotSizeFlagsAndVols ( WitRun       * theWitRun,
                                                       int            nOperations,
                                                       int            nPeriods,
                                                       char        ** operationList,
                                                       witBoolean   * isMinLotSizeUsedAnywherePtr,
                                                       witBoolean  ** isMinLotSizeUsedForOperationPtr ,
                                                       float ***  minLotSizesPtr)
{
  witBoolean   isMinLotSizeUsedAnywhere = WitFALSE;
  witBoolean * isMinLotSizeUsedForOperation;

  // allocate memory to hold the minimum lot sizes and initialize to 0
  float ** minLotSizes;
  minLotSizes = (float **) malloc (nOperations * sizeof(float *));
  assert ( minLotSizes != 0 );
  
  // malloc a vector of length equal to number of operations,
  // to track whether each product has minimum lot size in
  // some period
  isMinLotSizeUsedForOperation = (witBoolean *) malloc (nOperations * sizeof(witBoolean));
  assert ( isMinLotSizeUsedForOperation != 0 );

  // intialize all operations not to have minimum lot sizes and all minLotSize vectors to 0
  int o = 0; // Pulled out of the for below by RW2STL
  for ( o=0; o<nOperations; o++) {
    isMinLotSizeUsedForOperation[o] = WitFALSE;
    minLotSizes[o] = 0 ;
  }
  
  
  // loop through the operations and account for those having a minLotSize
  for ( o=0; o<nOperations; o++){
    float * minLotSize ;

    witGetOperationMinLotSize ( theWitRun, operationList[o], &minLotSize );
    
    // In addition to keeping track of whether any operation has a minLotSize in
    // any period, keep track of whether minLotSizes are used anywhere (a global flag).
    int t = 0; // Pulled out of the for below by RW2STL
    for ( t=0; t<nPeriods; t++){
      if (minLotSize[t] > ITHELPER_FLT_EPS){
        isMinLotSizeUsedForOperation[o] = WitTRUE;
        isMinLotSizeUsedAnywhere = WitTRUE;
        break;
      }
    }

    if (isMinLotSizeUsedForOperation[o]) 
      minLotSizes[o] = minLotSize;
    else
      portableFree ( minLotSize );
  }

  // set pointers to passed in through the parameter list 
  (*isMinLotSizeUsedAnywherePtr) = isMinLotSizeUsedAnywhere;
  (*isMinLotSizeUsedForOperationPtr) = isMinLotSizeUsedForOperation;
  (* minLotSizesPtr) = minLotSizes ;
}


void
LgFrSceIterativeImpHelper::getSupplyVols ( WitRun       * theWitRun,
                                           int            nParts,
                                           char        ** partList,
                                           float ***  allSupplyVolsPtr)
{

  // allocate memory to hold the supplyVol and initialize to 0
  float ** allSupplyVols;
  allSupplyVols = (float **) malloc (nParts * sizeof(float *));
  assert ( allSupplyVols != 0 );
  
  
  
  // loop through the parts and get the part's supply vol
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++){
    witGetPartSupplyVol ( theWitRun, partList[i], &allSupplyVols[i] );
  }
    

  // set pointers to passed in through the parameter list 
  (*allSupplyVolsPtr) = allSupplyVols;
}




// get demand list
// NOTE: calling object is responsible for memory clean-up
void
LgFrSceIterativeImpHelper::getDemandLists( WitRun    * theWitRun,
                                           int         nParts,
                                           char    **  partList,
                                           int     **  nDemandsPtr,
                                           char  ****  demandListPtr )
{
  char ** dList;
  char *** demandList;
  int * nDemands;
  
  // malloc a vector to be a vector of demand lists 
  demandList = (char ***) malloc (nParts * sizeof(char**));
  if (demandList == 0) {
    //*** ? make sure this doesn't get to the assert(..)
  }
  assert ( demandList != 0 );
  
  // malloc a vector to hold the number of demands 
  nDemands = (int *) malloc (nParts * sizeof(int));
  assert ( nDemands != 0 );

  // initialize the newly malloc-ed stuff 
  int i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<nParts; i++ ){
    demandList[i] = 0;
    nDemands[i] = 0;
  }

  // for each part, get the demands from theWitRun and point the newly
  // created pointers at them                                  
  for ( i=0; i<nParts; i++ ){
    witGetPartDemands ( theWitRun, partList[i], &(nDemands[i]), &dList );
    if (nDemands[i] > 0)
      demandList[i] = dList;
  }

  // set ptrs to the created demand list and number-of-demands vector
  (*demandListPtr) = demandList;
  (*nDemandsPtr) = nDemands; 
}

// copy demand vols, so we can clean-up after we muck around
// NOTE: caller is responsible for memory management: free up the allDemandsVolsPtr
void
LgFrSceIterativeImpHelper::getDemandVols ( WitRun * theWitRun,
                                            int nPeriods,
                                            int nParts,
                                            char ** partList,
                                            int *  nDemandsList,
                                            char *** allDemandsList,
                                            float **** allDemandsVolsPtr )
{
  // allocate enough memory for every time vec of demand vols
  float *** allDemandsVols;
  // each part ...
  allDemandsVols = (float ***) malloc (nParts * sizeof(float **));
  assert (allDemandsVols != 0);
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
    // this should be "if = 0", but that test didn't work (?) but it should have
    // if you have NO demands on the part, set pointer to null
    if (!(nDemandsList[i] > 0))
      allDemandsVols[i] = 0 ;
      // you have demands on the part so for each one...
    else {
      assert (nDemandsList[i] > 0); 
      allDemandsVols[i] = (float **) malloc (nDemandsList[i] * sizeof(float*));
      assert (allDemandsVols[i] != 0);
      // get the demand volume from the witRun
      int j = 0; // Pulled out of the for below by RW2STL
      for (j=0; j<nDemandsList[i]; j++)
        witGetDemandDemandVol (theWitRun, partList[i], allDemandsList[i][j],
                               &(allDemandsVols[i][j]) );
    }
  }
  // set pointer to the copy of all the demand vols to pass through parameter list
  (* allDemandsVolsPtr) = allDemandsVols;
}

// copy ship late limits, so we can clean-up after we set them to 0 in rollover 
// NOTE: caller is responsible for memory management: free up the allDemandsVolsPtr
void
LgFrSceIterativeImpHelper::getShipLateLimits ( WitRun * theWitRun,
                                                int nPeriods,
                                                int nParts,
                                                char ** partList,
                                                int *  nDemandsList,
                                                char *** allDemandsList,
                                                int *** allShipLateLimitsPtr )
{
  // allocate enough memory for every time vec of demand vols
  int ** allShipLateLimits;
  // each part ...
  allShipLateLimits = (int **) malloc (nParts * sizeof(float *));
  assert (allShipLateLimits != 0);
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
    // this should be "if == 0", but that test didn't work (?) but it should have
    // if you have NO demands on the part, set pointer to null
    if (!(nDemandsList[i] > 0))
      allShipLateLimits[i] = 0 ;
      // you have demands on the part so for each one...
    else {
      assert (nDemandsList[i] > 0); 
      allShipLateLimits[i] = (int *) malloc (nDemandsList[i] * sizeof(int));
      assert (allShipLateLimits[i] != 0);
      // get the ship late limit from the witRun
      int j = 0; // Pulled out of the for below by RW2STL
      for (j=0; j<nDemandsList[i]; j++)
        witGetDemandShipLateLimit (theWitRun, partList[i], allDemandsList[i][j],
                               &(allShipLateLimits[i][j]) );
    }
  }
  // set pointer to the copy of all the demand vols to pass through parameter list
  (* allShipLateLimitsPtr) = allShipLateLimits;
}



// getNBom - malloc a vector of length nParts to hold the number of BOM entrees
//           for each part
// NOTE: caller is responsible for memory clean-up 
void
LgFrSceIterativeImpHelper:: getNBom ( WitRun * theWitRun,
                                      int      nOperations,
                                      char **  operationList,
                                      int **   nBomPtr )
{
  //malloc a vector of length nOperations and initialize it to zero
  int * nBom;
  nBom = (int *) malloc (nOperations * sizeof(int));
  assert ( nBom != 0 );
  int o = 0; // Pulled out of the for below by RW2STL
  for (o=0; o<nOperations; o++)
    nBom[o] = 0;
  
  // get the number of BOM entries for each operation from the WitRun and
  // store it in nBom
  for (o=0; o<nOperations; o++) {
    witGetOperationNBomEntries( theWitRun, operationList[o], &nBom[o] );
    assert ( nBom[o] >= 0 );
  }

  // set pointer to pass back through paramter list
  (*nBomPtr) = nBom;
}

// get number of substitute BOM entries - allocate memory and store in it 
// the number of substitute BOM entries for every BOM for every operation
// NOTE: caller is responsible for memory clean-up
void
LgFrSceIterativeImpHelper::getNSubsBom ( WitRun * theWitRun,
                                         int nOperations,
                                         char ** operationList,
                                         int * nBom,
                                         int *** nSubsBomPtr)
{
  // allocate a vector for each of the nOperations.
  int ** nSubsBom;
  nSubsBom = (int **) malloc (nOperations * sizeof (int *));
  assert ( nSubsBom != 0 );

  // for each operation, initialize the number of substitute BOM entries on each
  // BOM entree to be 0
  int o = 0; // Pulled out of the for below by RW2STL
  for (o=0; o<nOperations; o++){
    nSubsBom[o] = 0;
    // there can only be substitute BOM entries if there are BOM entries in the
    // first place...
    if ( nBom[o] > 0 ) {
      nSubsBom[o] = (int * ) malloc ( nBom[o] * sizeof (int));
      assert (nSubsBom[o]  != 0);
      
      // for every BOM entree, get the number of substitutes from theWitRun
      int j = 0; // Pulled out of the for below by RW2STL
      for (j=0; j<nBom[o]; j++){
        witGetBomEntryNSubsBomEntries(theWitRun,
                                      operationList[o],
                                      j,
                                      &nSubsBom[o][j]);
        // it pays to be sure...
        assert ( nSubsBom[o][j] >= 0 );
      }  
    }
  }

  // set a pointer to pass back through the parameter list
  (*nSubsBomPtr) = nSubsBom ;
}

// allocate and initialize structures for the global shipment schedule
// NOTE: caller is responsible for memory clean-up
void
LgFrSceIterativeImpHelper::initGlobalShipSchedule ( float **** globalShipSchedulePtr,
                                                    int nPeriods,
                                                    int nParts,
                                                    int * nDemandsList )
{
  // Malloc memory for the global ship schedule and initialize it to 0
  float *** globalShipSchedule ;
  globalShipSchedule = (float ***) malloc (nParts * sizeof(float **)) ;
  assert ( globalShipSchedule != 0 ) ;
  int i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i < nParts ; i++ )
    globalShipSchedule[i] = 0 ;

  // loop through the parts...
   for ( i=0; i < nParts; i++ ) {

     // only need to allocate space if there are demands for this part
     if ( nDemandsList[i] > 0) {
       float ** shipDemandList ;
       shipDemandList = (float **)
         malloc (nDemandsList[i] * sizeof(float *)) ;
       assert (shipDemandList != 0) ;
       globalShipSchedule[i] = shipDemandList ;
       
       // for each demand on this part, allocate a shipment time vec
       // and initialize it to 0.0
       int j = 0; // Pulled out of the for below by RW2STL
       for ( j=0; j<nDemandsList[i]; j++) {
         float * initialShipVol;
         initialShipVol =  (float *) malloc (nPeriods * sizeof (float)) ;
         assert ( initialShipVol != 0) ;
         int t = 0; // Pulled out of the for below by RW2STL
         for ( t=0; t < nPeriods; t++ )
           initialShipVol[t] = 0.0 ;   
         globalShipSchedule[i][j] = initialShipVol ;
       }
     }
   }
   
   // set pointer to pass through parameter list
   (*globalShipSchedulePtr) = globalShipSchedule;
}

// Allocate and initialize a global execution vol schedule.
// The solution from each implosion iteration will be added to this schedule
// to arrive at the "global" or final execution vol schedule.
// NOTE: caller is responsible for memory clean-up
void
LgFrSceIterativeImpHelper::initGlobalExecSchedule ( float *** globalExecSchedulePtr,
                                                   int nPeriods,
                                                   int nOperations )
{
  // malloc memory for the global production schedule 
  float ** globalExecSchedule;
  globalExecSchedule = (float  **) malloc (nOperations * sizeof(float *));
  assert ( globalExecSchedule != 0 );
  int o = 0; // Pulled out of the for below by RW2STL
  for ( o=0; o < nOperations ; o++ ) {
    globalExecSchedule[o] = 0  ;
  }
  
  // for each operations, allocate a time vec to hold its exec vol schedule
  // and initialize it to 0.0
  for ( o=0; o < nOperations ; o++ ) {
    float * initialExecVol;
    initialExecVol = (float *) malloc (nPeriods * sizeof (float)) ;
    int t = 0; // Pulled out of the for below by RW2STL
    for ( t=0; t < nPeriods; t++)
      initialExecVol[t] = 0.0 ;
    globalExecSchedule[o] = initialExecVol  ;
   }

  // set pointer to pass back through parameter list
  (*globalExecSchedulePtr) = globalExecSchedule;
}  


// allocate and initialize global substitute BOM production schedule
// NOTE: callers must clean up the memory
void
LgFrSceIterativeImpHelper::initGlobalSubsBomProdSchedule(
                               float ***** globalSubsBomProdSchedulePtr,
                               int nPeriods,
                               int nOperations,
                               int * nBom,
                               int ** nSubsBom )
{
  // malloc memory for global substitute BOM entry prod vol
  // 
  // (a different data structure might be more efficient here )
  // this code needs to be industrial strength - so no skimping
  // on the error checking and initialization
  float  **** globalSubsBomProdSchedule;
  globalSubsBomProdSchedule = (float ****) malloc (nOperations * sizeof (float ***));
  assert ( globalSubsBomProdSchedule != 0 );
  
  // look at each operation-
  int o = 0; // Pulled out of the for below by RW2STL
  for ( o=0; o<nOperations; o++ ) {
    
    // if there are no children in BOM tree for this operation,
    // set the pointer to 0 
    if (! (nBom[o] > 0))
      globalSubsBomProdSchedule[o] = 0 ;
    
    // otherwise, malloc memory for each of the BOM entries
    else {
      globalSubsBomProdSchedule[o] = (float ***)
        malloc (nBom[o] * sizeof(float **)) ;
      assert (globalSubsBomProdSchedule != 0) ;
      
      // look at each BOM entry-
      int j = 0; // Pulled out of the for below by RW2STL
      for ( j=0; j<nBom[o]; j++) {
        
        // if there are no substittues for this BOM entry
        // set pointer to 0
        if (! (nSubsBom[o][j] > 0))
          globalSubsBomProdSchedule[o][j] = 0 ;
        
        // otherwise, malloc memory for each of the substitutes
        else {
          globalSubsBomProdSchedule[o][j] = (float **)
            malloc (nSubsBom[o][j] * sizeof (float *));
          assert ( globalSubsBomProdSchedule[o][j] != 0 );
          
          // for each of the substitues, malloc a time vec
          // to hold the prod vol for the BOM substitute entry
          // and initialize it to zero
          int k = 0; // Pulled out of the for below by RW2STL
          for (k=0; k<nSubsBom[o][j]; k++){
            globalSubsBomProdSchedule[o][j][k] = (float *)
              malloc (nPeriods * sizeof (float));
            assert ( globalSubsBomProdSchedule[o][j][k] != 0) ;
            int t = 0; // Pulled out of the for below by RW2STL
            for ( t=0; t < nPeriods; t++ )
              globalSubsBomProdSchedule[o][j][k][t] = 0.0 ;   
          } 
        } 
      } 
    } 
  } // 4-stars...whew!
  
  // set pointer to pass back through parameter list
  (*globalSubsBomProdSchedulePtr) = globalSubsBomProdSchedule;
}

// copy the exec upper bounds so after we muck up theWitRun, we can 
// set the exec upper bounds back to their original values.
// NOTE: caller is responsible for memory clean-up
void
LgFrSceIterativeImpHelper::getExecUpperBounds ( WitRun * theWitRun,
                                                int nOperations,
                                                char ** operationList,
                                                float *** globalExecUpperBoundsPtr )
{
  // malloc memory for the global production schedule 
  float ** globalExecUpperBoundsSchedule;
  globalExecUpperBoundsSchedule = (float  **) malloc (nOperations * sizeof(float *));
  assert ( globalExecUpperBoundsSchedule != 0 );
  
  // loop through the operations and get the production hard upper bound from WIT
  int o = 0; // Pulled out of the for below by RW2STL
  for ( o=0; o < nOperations ; o++ ) {
    globalExecUpperBoundsSchedule[o] = 0;

    float * hardLower;
    float * softLower;
    witGetOperationExecBounds( theWitRun, operationList[o], 
			       &hardLower, &softLower, &(globalExecUpperBoundsSchedule[o]) );
    portableFree( hardLower );
    portableFree( softLower );
  }

  // set pointer to pass back through parameter list
  (*globalExecUpperBoundsPtr) = globalExecUpperBoundsSchedule;
}


// update global (ship, production, substitute BOM production) schedules
// to be called AFTER an implosion has been preformed
void
LgFrSceIterativeImpHelper::updateGlobalSchedules( WitRun * theWitRun,
                                                  int nPeriods,
                                                  int nParts,
                                                  int nOperations,
                                                  char ** partList,
                                                  char ** operationList,
                                                  int * nDemandsList,
                                                  char *** allDemandsList,
                                                  int * nBom,
                                                  int ** nSubsBom,
                                                  float *** globalShipSchedule,
                                                  float ** globalExecSchedule,                        
                                                  float **** globalSubsBomProdSchedule )

{
  // loop through the parts...
  int i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<nParts; i++ ) {
    // for every demand on the part...
    int j = 0; // Pulled out of the for below by RW2STL
    for ( j=0; j<nDemandsList[i]; j++ ) {
      // get the ship vol and add it to the global ship schedule
      float * shipVol;
      witGetDemandShipVol( theWitRun,
                           partList[i], allDemandsList[i][j], &shipVol);
      int t = 0; // Pulled out of the for below by RW2STL
      for ( t=0; t<nPeriods; t++ ) 
        globalShipSchedule[i][j][t] += shipVol[t];
      portableFree ( shipVol );
    } /* end for j loop */

    
  } /* end for i loop */

  // loop through the operations...
  int o = 0; // Pulled out of the for below by RW2STL
  for ( o=0; o<nOperations; o++) {
    float * execVol;
    witGetOperationExecVol( theWitRun, operationList[o], &execVol );
    int t = 0; // Pulled out of the for below by RW2STL
    for ( t=0; t<nPeriods; t++ ) 
      globalExecSchedule[o][t] += execVol[t];
    portableFree ( execVol );

    // consider every BOM entry...
    int j = 0; // Pulled out of the for below by RW2STL
    for ( j=0; j<nBom[o]; j++) {
      // for every substitute on the BOM entry..
      int k = 0; // Pulled out of the for below by RW2STL
      for ( k=0; k < nSubsBom[o][j]; k++){
        // get the substitute BOM entry production vol and
        // add it to the global substitute BOM production schedule
        float * subsProdVol;
        witGetSubsBomEntryProdVol( theWitRun,
                                   operationList[o],
                                   j, // bom entry index
                                   k, // sub bom entry index
                                   &subsProdVol);
        int t = 0; // Pulled out of the for below by RW2STL
        for ( t=0; t<nPeriods; t++)
          // hold on! here goes the 5 star-er....
          globalSubsBomProdSchedule[o][j][k][t] += subsProdVol[t];
        portableFree ( subsProdVol );
      }
    }

  }  

}


// Update supply vols AFTER an implosion
// NOTE: supply vols in theWitRun are changed
void
LgFrSceIterativeImpHelper::setSupplyVolToExcessVol ( WitRun * theWitRun,
                                              int nParts,
                                              char ** partList )
{
  int nPeriods;
  witGetNPeriods(theWitRun, &nPeriods);
  int i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<nParts; i++){
    float * residualVol;
    // witGetPartExcessVol( theWitRun, partList[i], &excessVol );
    witGetPartResidualVol( theWitRun, partList[i], &residualVol );    
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=0; t<nPeriods; t++) {
      // 2.30 fix: we want to get rid of ALL instances of setting a supplyVol
      // to a negative number.  This causes a severe error in wit and who knows 
      // what the heck it does to the SCE rollover logic.  It's a FINISH_ME: to 
      // put in a clever message that tells us when and where this happens.  Right
      // now we just blow off the negative residual vols.  (which probably causes 
      // some infeasibilities somewhere at the end.
      if (residualVol[t] < 0.0001) {
	//      if (residualVol[t] < 0.0001 && residualVol[t] > -0.01) {
	residualVol[t] = 0.0;
      }
    }
    witSetPartSupplyVol (theWitRun, partList[i], residualVol );
    portableFree(residualVol);
  }

}


// In going from one iteration of implosion to the next, the
// production upper bounds in iteration t+1 need to be adjusted
// to reflect the amount of production in iteration t. 
// 
// NOTE: production upper bounds in theWitRun are changed
void
LgFrSceIterativeImpHelper::updateExecUpperBounds ( WitRun * theWitRun,
                                                   int nPeriods,
                                                   int nOperations,
                                                   char ** operationList)
{
  int o = 0; // Pulled out of the for below by RW2STL
  for ( o=0; o<nOperations; o++){
    // Get the exec upper bound of the operation.
    float * hardLower;
    float * softLower;
    float * execUB;
    witGetOperationExecBounds( theWitRun, operationList[o], &hardLower, &softLower, &execUB );
    portableFree( hardLower );
    portableFree( softLower );

    // If the operation had a constraining production upperbound, set the production upper bound for
    // next implosion iteration equal to the previous upper bound less any amount
    // of production.
    LgFrTimeVecFloat execUBTV ( (size_t) nPeriods, execUB);
    if ( execUBTV.max() > -0.00001 ) {
      float * execVol;
      witGetOperationExecVol ( theWitRun, operationList[o], &execVol );
      LgFrTimeVecFloat execVolTV ( (size_t) nPeriods, execVol);
      assert ( execUBTV >= execVolTV );  // ...just in case...
      execUBTV = execUBTV - execVolTV;
      portableFree( execVol );
      witSetOperationExecBounds ( theWitRun, operationList[o], 0, 0, execUBTV.data() );
    }
    portableFree( execUB );
  }
}

// Update minimum lot size AFTER an implosion -- the structures and logic
// could be more efficient here.
//
// Carry over the production from previous implosions (plural)
// to count towards satisfying minimum lot sizing in the next
// implosions
//
// NOTE: only call after the global production schedule's been called.
//       changes minimum lot size in theWitRun
void
LgFrSceIterativeImpHelper::updateMinLotSize ( WitRun * theWitRun,
                                              int nPeriods,
                                              int nOperations,
                                              char ** operationList,
                                              float ** globalExecSchedule,
                                              witBoolean isMinLotSizeUsedAnywhere,
                                              witBoolean * isMinLotSizeUsedForOperation )
{
  // check to see if there are ANY minLotSizes in the witRun
  if (isMinLotSizeUsedAnywhere){
    // if there are, loop through the operations..
    int o = 0; // Pulled out of the for below by RW2STL
    for ( o=0; o<nOperations; o++ ) {
      // that have minLotSize in some time bucket.
      if (isMinLotSizeUsedForOperation[o]) {
        float * minLotSize;
        // get the minLotSize vec from theWitRun..
        witGetOperationMinLotSize ( theWitRun, operationList[o], &minLotSize );
        // and check each period..
        int t = 0; // Pulled out of the for below by RW2STL
        for ( t = 0; t < nPeriods; t ++){
          // if production is positive, that means that the
          // minLotSize was satisfied, and should not be enforced in
          // the next implosions iteration. So, set minLotSize to 0.0 
          if ( globalExecSchedule[o][t] > 0 )
            minLotSize[t] = 0.0;
        }
        witSetOperationMinLotSize ( theWitRun, operationList[o], minLotSize );
        portableFree ( minLotSize );
      }
    }
  }
}


// update:
//    a) excessVol -> supplyVol
//    b) adjust execVolUpperBounds 
//    c) adjust minLotSizes
void
LgFrSceIterativeImpHelper::prepOperationConstraintsForNextImplosionIteration ( WitRun * theWitRun,
                                                                          int nPeriods,
                                                                          int nParts,
                                                                          char ** partList,
                                                                          int nOperations,
                                                                          char ** operationList,
                                                                          float ** globalExecSchedule,
                                                                          float ** globalExecUpperBounds,
                                                                          witBoolean isMinLotSizeUsedAnywhere,
                                                                          witBoolean * isMinLotSizeUsedForOperation )
{
  
  setSupplyVolToExcessVol( theWitRun, nParts, partList );
  
  updateMinLotSize ( theWitRun, nPeriods, nOperations, operationList,
                     globalExecSchedule, 
                     isMinLotSizeUsedAnywhere, isMinLotSizeUsedForOperation ) ;

  updateExecUpperBounds ( theWitRun, nPeriods, nOperations, operationList );
}


// stuff global shipment, production, and substitute BOM production schedules
// into a theWitRun
void
LgFrSceIterativeImpHelper::stuffGlobalSolutionSchedules ( WitRun * theWitRun,
							  int nPeriods,
							  int nParts,
							  int nOperations, 
							  int * nDemandsList,
							  char *** allDemandsList,
							  char ** partList,
							  char ** operationList,
							  int * nBom,
							  int ** nSubsBom,
							  float *** globalShipSchedule,
							  float ** globalExecSchedule,
							  float **** globalSubsBomProdSchedule )
{ 
  int i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<nParts; i++ ) {
    int j = 0; // Pulled out of the for below by RW2STL
    for ( j=0; j<nDemandsList[i]; j++)
      // set the demand ship vol with the global ship schedule
      witSetDemandShipVol( theWitRun, partList[i], allDemandsList[i][j],
                           globalShipSchedule[i][j] );
  } //end i-loop

  int o = 0; // Pulled out of the for below by RW2STL
  for ( o=0; o<nOperations; o++ ) {
    // set the operation exec vol with the global execution schedule
    witSetOperationExecVol( theWitRun, operationList[o], globalExecSchedule[o] );
     // set the subs BOM entry prod vol with the  global sub BOM entry prod schedule
    int j = 0; // Pulled out of the for below by RW2STL
    for ( j=0; j<nBom[o]; j++) {
      int k = 0; // Pulled out of the for below by RW2STL
      for ( k=0; k < nSubsBom[o][j]; k++){
        witSetSubsBomEntrySubVol( theWitRun,
				  operationList[o],
				  j, // bom entry index
				  k, // sub bom entry index
				  globalSubsBomProdSchedule[o][j][k]);
      }
    }        
  }
}


// Zero out all the demands in theWitRun
// This method mucks up the demands, so if you don't want to lose the demand information
// make a copy somewhere safe.
void
LgFrSceIterativeImpHelper::zeroOutAllDemands( WitRun * theWitRun,
                                              int nPeriods,
                                              int nParts,
                                              char ** partList,
                                              int *  nDemandsList,
                                              char *** allDemandsList)
{
  // make a zero time vector
  float * zeroTimeVec = (float *) malloc (nPeriods * sizeof(float));
  assert (zeroTimeVec != 0);
  int t = 0; // Pulled out of the for below by RW2STL
  for (t=0; t<nPeriods; t++)
   zeroTimeVec[t]=0.0;

  // for every demand, set the demand vol equal to zero
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
      int j = 0; // Pulled out of the for below by RW2STL
      for (j=0; j<nDemandsList[i]; j++)
        witSetDemandDemandVol( theWitRun, partList[i], allDemandsList[i][j], zeroTimeVec);
  }
  portableFree (zeroTimeVec);
}

// set the minLotSizes
void
LgFrSceIterativeImpHelper::setMinLotSize( WitRun * theWitRun,
                                          int nPeriods,
                                          int nOperations,
                                          char    **  operationList,
                                          witBoolean * isMinLotSizeUsedForOperation,
                                          float **  minLotSizes )
{
  // loop through the operations
  int o = 0; // Pulled out of the for below by RW2STL
  for (o=0; o<nOperations; o++) {
    // if a operation uses minimum lot sizes, set the minLotSize in theWitRun
    // operations that are not products have isMinLotSizeUsedForOperation set to WitFALSE.
    if (isMinLotSizeUsedForOperation[o]) 
      witSetOperationMinLotSize (theWitRun, operationList[o], minLotSizes[o] );
  }
}

// set the supplyVols
void
LgFrSceIterativeImpHelper::setSupplyVols( WitRun * theWitRun,
                                          int nParts,
                                          char    **  partList,
                                          float **  allSupplyVols )
{
  // loop through the parts and set
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
    witSetPartSupplyVol (theWitRun, partList[i], allSupplyVols[i] );
  }
}

// set the the production hard upper bounds
void
LgFrSceIterativeImpHelper::setExecUpperBounds( WitRun * theWitRun,
                                               int nOperations,
                                               char    **  operationList,
                                               float **  allExecUpperBounds )
{
  // loop through the operations and set
  int o = 0; // Pulled out of the for below by RW2STL
  for (o=0; o<nOperations; o++) 
    witSetOperationExecBounds (theWitRun, operationList[o], 
			       0, 0, allExecUpperBounds[o] );
}


// Set demand vols
// Use this member function to clean-up the mucked demand vols
void
LgFrSceIterativeImpHelper::setDemandVols ( WitRun * theWitRun,
                                           int nParts,
                                           char ** partList,
                                           int *  nDemandsList,
                                           char *** allDemandsList,
                                           float *** allDemandsVols )
{
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<nDemandsList[i]; j++)
      witSetDemandDemandVol (theWitRun, partList[i], allDemandsList[i][j],
                             allDemandsVols[i][j]) ;
  }
}  

// Set ship late limits
// Use this member function to clean-up the mucked ship late limits
void
LgFrSceIterativeImpHelper::setShipLateLimits ( WitRun * theWitRun,
                                               int nParts,
                                               char ** partList,
                                               int *  nDemandsList,
                                               char *** allDemandsList,
                                               int ** allShipLateLimits )
{
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<nDemandsList[i]; j++)
      witSetDemandShipLateLimit (theWitRun, partList[i], allDemandsList[i][j],
                                 allShipLateLimits[i][j]) ;
  }
}  



// default constructor
LgFrSceIterativeImpHelper::LgFrSceIterativeImpHelper()
{
    // all the work is done in initializer
}

// copy constructor 
LgFrSceIterativeImpHelper::LgFrSceIterativeImpHelper(const LgFrSceIterativeImpHelper& source)
{
  // nothing to do
}

// assignment operator
LgFrSceIterativeImpHelper&
LgFrSceIterativeImpHelper::operator=(const LgFrSceIterativeImpHelper& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    // do nothing
  }
  return *this;
}

// destructor
LgFrSceIterativeImpHelper::~LgFrSceIterativeImpHelper()
{
  // nothing to do, 
}


#ifdef NDEBUG
#undef NDEBUG
#endif



// self-test : can I self test ??
void
LgFrSceIterativeImpHelper::test()
{
  // do nothing
 }

#endif
