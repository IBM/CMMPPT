
// =================================================================
// =================================================================
//
//    THIS CODE IS OBOSOLETE.   It's no longer used by SCE
//
//
// =================================================================
// =================================================================
 



/******************************************************************************
 *
 * API Code to Preform RollOver Heurisitc
 *
 * The rollover heuristic is a priority-based implosion
 * where unmet demand in period t is "rolled over" into period t+1
 * and is assigned the priority of demand in period t+1.
 *
 * The rollover heristic is accomplished through the API by preforming
 * one implosion for each time bucket, where only the demand in that
 
 * time bucket is consider.
 *
 * The main steps are:
 *   - Set each demand's shipLateLimit to 0
 *    - For each period t,
 *    -   For each demand d,
 *    -     set the demand vol [t] = the original demand vol + (unmet
 *                               demand from period [t-1])*(backlogYield factor)
 *    -     zero out demand vol in all other periods
 *    -   For each part
 *    -       set part supply vol = original supply if t=0 otherwise
 *                                  part's excess supplyVol [t-1]
 *    -   If using MinLotSizes, account for previous production
 *    -   Do a heuristic implosion
 *    -   Add current production and shipment schedules to global schedules
 *
 * Minimum Demand Volumes:
 *  If there are minimimum demand vols (specified as cumship soft lower bounds)
 *  a first pass heuristic implosion is done to allocate to the minimum demands
 *  before proceeding to a rollover hueristic. The demand for the rollover
 *  is the amount of demand over and above the minimum demands.
 * 
 * A critical list is produced by combining the critical lists from each
 * of the implosions and filtering out duplicate entries.
 *
 *
 * R. Lougee-Heimer
 * May 10, 1996
 ****************************************************************************/

#include <assert.h> 
#include <scenario/src/machdep.h>
#include <sce/src/rollHeur.h>
#include <sce/src/sceDefin.h>

//#define  ROLL_DEBUG 
#ifndef ELIMINATE_OLD_MAIN


int rolloverHeurImplode( WitRun * const theWitRun,
                         LgFrSceCritList & theCritList,
                         int printInformationalWitMessages,
                         const char * const witMessageFilename)

{

  /* Since multiple implosions are being preformed, some information is  */
  /* stored to avoid excessive API calls                                 */

  witBoolean   computeCriticalList;
  witBoolean   minLotSizeUsed ;         /* min lots used somewhere        */
  witBoolean * minLotSizeUsedForPart ;  /* min lots used for this part    */
  witBoolean * partIsProduct ;          /* this part is a product         */
  
  int nPeriods ;                        /* number of periods              */
  int nParts ;                          /* number of parts                */
  int nImplosions ;                     /* either nPeriods or nPeriods+1  */
  int * nDemandsList ;                  /* number of demands for ea part  */
  int i, j, k, t, tt, ctr;

  char ** partList ;                    /* list of part names             */
  char *** allDemandsList ;

  float **   globalProdSchedule ;       /* aggregate production schedule  */
  float **** globalSubsBomProdSchedule ;/* aggregate subs prod schedule   */
  float ***  globalShipSchedule ;       /* aggregate shipment schedule    */
  float **   shipDemandList ;           /* list of demands for each part  */
                                        /* in globalShipSchedule          */

  
  float * prodVol ;                      /* production volume time vector */
  float * shipVol ;                      /* shipment volume time vector   */   
  float * initialProdVol;                
  float * initialShipVol;

  float unmetDemand ;
  float * excessVol ;

  float * cumShipBound;
  float * demandVol ;
  float * workingDemandVol ;

  void * demandAppDataPtr;

  float * backlogYield;
  float * defaultBacklogYield; 

  
  WitRun * theWorkingWitRun;

  /*
   * Create a copy of theWitRun that passed in to work on (ie theWorkingWitRun),
   * turn off some messaging and get necessary basic problem data.
   */
   witNewRun ( & theWorkingWitRun );

   if( witMessageFilename != 0 ) 
     witSetMesgFileName(theWorkingWitRun,
                        WitTRUE,
                        witMessageFilename);
   if ( !printInformationalWitMessages ) {
     witSetMesgTimesPrint(theWorkingWitRun,
                          WitTRUE,
                          WitINFORMATIONAL_MESSAGES,
                          0);
   }
   //regardless - don't print the known warnings
     witSetMesgTimesPrint(theWorkingWitRun,
                          WitTRUE,
                          605,
                          0); //negative prod costs warning
     witSetMesgTimesPrint(theWorkingWitRun,
                          WitTRUE,
                          338,
                          0); //input names too long
   
   //#ifndef ROLL_DEBUG
   //witSetMesgTimesPrint ( theWorkingWitRun, WitTRUE,
   //                       WitINFORMATIONAL_MESSAGES, 0);
   //#endif
   
   witCopyData( theWorkingWitRun, theWitRun ) ;
   witGetNPeriods( theWorkingWitRun, &nPeriods ) ;
   witGetParts( theWorkingWitRun, &nParts, &partList ) ;
   
   //get the demand info so don't have to keep calling API routines
   getDemandLists( theWorkingWitRun, nParts, partList,
                   &nDemandsList, &allDemandsList );
   //get product status of parts so don't have to keep calling API routines
   getProductParts( theWorkingWitRun, nParts,
                    partList, &partIsProduct );

   //get the number of BOM entries for each part. This info is
   //used to update global substitute BOM entry production schedule.
   int * nBom; 
   getNBom ( theWorkingWitRun, nParts, partList, &nBom);

   //get the number of substitute BOM entries for each BOM entry. This info
   //is used to update global sucstitute BOM entry production schedule.
   int ** nSubsBom;  
   getNSubsBom (theWorkingWitRun, nParts, partList, nBom, &nSubsBom);


  /*
   * Malloc and initialize storage
   */
   //the workingDemandVol vector is used through out the iterations of implosions
   workingDemandVol = (float *) malloc (nPeriods * sizeof (float)) ;
   assert ( workingDemandVol != 0) ;
   for ( t=0; t < nPeriods; t++ )
     workingDemandVol[t] = 0.0 ;

   // Malloc memory for the global ship schedule
   // and initialize it to zero        
   globalShipSchedule = (float ***) malloc (nParts * sizeof(float **)) ;
   assert ( globalShipSchedule != 0 ) ;
   for ( i=0; i < nParts ; i++ )
     globalShipSchedule[i] = 0 ;

   for ( i=0; i < nParts; i++ ) {
     if ( nDemandsList[i] > 0) {
       shipDemandList = (float **)
                        malloc (nDemandsList[i] * sizeof(float *)) ;
       assert (shipDemandList != 0) ;
       globalShipSchedule[i] = shipDemandList ;
       for ( j=0; j<nDemandsList[i]; j++) {
         initialShipVol =  (float *) malloc (nPeriods * sizeof (float)) ;
         assert ( initialShipVol != 0) ;
         for ( t=0; t < nPeriods; t++ )
           initialShipVol[t] = 0.0 ;   
         globalShipSchedule[i][j] = initialShipVol ;
       }
     }
   }
   

   // malloc memory for the global production schedule 
   // and initialize it to zero                          
   globalProdSchedule = (float  **) malloc (nParts * sizeof(float *));
   assert ( globalProdSchedule != 0 );
   for ( i=0; i < nParts ; i++ ) {
     initialProdVol = (float *) malloc (nPeriods * sizeof (float)) ;
     for ( t=0; t < nPeriods; t++)
       initialProdVol[t] = 0.0 ;
     globalProdSchedule[i] = initialProdVol  ;
   }
   
   //
   // malloc memory for global substitute BOM entry prod vol
   // 
   // (a different data structure might be more efficient here )
   // this code needs to be industrial strength - so no skimping
   // on the error checking and initialization
   //
   globalSubsBomProdSchedule = (float ****) malloc (nParts * sizeof (float ***));
   assert ( globalSubsBomProdSchedule != 0 );
   
   // look at each part-
   for ( i=0; i < nParts; i++ ) {
     
     // if there are no children in BOM tree for this part,
     // set the pointer to 0 
     if (! (nBom[i] > 0))
       globalSubsBomProdSchedule[i] = 0 ;
     
     // otherwise, malloc memory for each of the BOM entries
     else
       {
         globalSubsBomProdSchedule[i] = (float ***)
           malloc (nBom[i] * sizeof(float **)) ;
         assert (globalSubsBomProdSchedule != 0) ;
         
         // look at each BOM entry-
         for ( j=0; j<nBom[i]; j++) {
           
           // if there are no substittues for this BOM entry
           // set pointer to 0
           if (! (nSubsBom[i][j] > 0))
             globalSubsBomProdSchedule[i][j] = 0 ;
           
           // otherwise, malloc memory for each of the substitutes
           else
             {
               globalSubsBomProdSchedule[i][j] = (float **)
                 malloc (nSubsBom[i][j] * sizeof (float *));
               assert ( globalSubsBomProdSchedule[i][j] != 0 );
               
               // for each of the substitues, malloc a time vec
               // to hold the prod vol for the BOM substitute entry
               // and initialize it to zero
               for (k=0; k<nSubsBom[i][j]; k++){
                 globalSubsBomProdSchedule[i][j][k] = (float *)
                   malloc (nPeriods * sizeof (float));
                 assert ( globalSubsBomProdSchedule[i][j][k] != 0) ;
                 for ( t=0; t < nPeriods; t++ )
                   globalSubsBomProdSchedule[i][j][k][t] = 0.0 ;   
               } 
             } 
         } 
       } 
   } // 4-stars...whew!
   
   /*
    * Determine if Minimum Lot Sizes are used
    */
   //keep track if minLotSize is used anywhere, then also keep track of whether
   // minLotSize is used for each part
   getMinLotSizeUsed ( theWorkingWitRun, nParts,
                       nPeriods, partList, partIsProduct,
                       &minLotSizeUsed, &minLotSizeUsedForPart );
   
  
   // initialize default backlog yield vector 1.0 (ie. entire backlog rolls over)
   // Note: length = nPeriods even though only nPeriods-1 entries are meaningful
   // This is for consistency in the time-vec world. The last entry is garbage.
   defaultBacklogYield = new float[nPeriods];
   for (t=0; t<nPeriods; t++)
     defaultBacklogYield[t] = 1.0;

  
  /*
   * Pass One: one heuristic implosion for minimum demands vols
   */
   witBoolean needPassOne = WitFALSE;
   setupPassOne( theWitRun,
                 theWorkingWitRun,
                 nPeriods,
                 nParts,
                 partList,
                 nDemandsList,
                 allDemandsList,
                 & needPassOne );     
   // REMEMBER to delete the aboveMinDemandVolPtr when done.

   witGetComputeCriticalList( theWorkingWitRun, &computeCriticalList);
     
   if ( needPassOne ) {     
     witHeurImplode( theWorkingWitRun );

     /*
      * update the global ship schedule, global production schedule, and
      * global substitute BOM entry production schedule
      */
     for ( i=0; i<nParts; i++ ) {
       for ( j=0; j<nDemandsList[i]; j++ ) {
         witGetDemandShipVol( theWorkingWitRun,
                              partList[i], allDemandsList[i][j], &shipVol);
         for ( tt=0; tt<nPeriods; tt++ ) 
           globalShipSchedule[i][j][tt] += shipVol[tt];
         portableFree ( shipVol );
       } /* end for j loop */
       if ( !partIsProduct[i]) continue;
       witGetPartProdVol( theWorkingWitRun, partList[i], &prodVol );
       for ( tt=0; tt<nPeriods; tt++ ) 
         globalProdSchedule[i][tt] += prodVol[tt];
       portableFree ( prodVol );
       // update global subs BOM entry prod schedule
       for ( j=0; j<nBom[i]; j++) {
         for ( k=0; k < nSubsBom[i][j]; k++){
           float * subsProdVol;
           witGetSubsBomEntryProdVol( theWorkingWitRun,
                                      partList[i],
                                      j, // bom entry index
                                      k, // sub bom entry index
                                      &subsProdVol);
           for ( tt=0; tt<nPeriods; tt++)
             globalSubsBomProdSchedule[i][j][k][tt] += subsProdVol[tt];
           portableFree ( subsProdVol );
         }
       }
     } /* end for i loop */

    /*
     * update the global critical list
     */
    if (computeCriticalList) {
      theCritList.merge(theWorkingWitRun);
    }    

    // Update supply vols for each part
    for ( i=0; i<nParts; i++ ) {
      witGetPartExcessVol( theWorkingWitRun, partList[i], &excessVol );
      witSetPartSupplyVol (theWorkingWitRun, partList[i], excessVol );
      portableFree(excessVol);
    }
    
    //updates for minlotsizing
    /* carry over production from previous implosions solutions to
       satisfy minimum lot sizes  -- could be more efficient here */
    if (minLotSizeUsed){
      for ( i=0; i<nParts; i++ ) {
        if ( !partIsProduct[i]) continue;
        if (minLotSizeUsedForPart[i]) {
          float * minLotSize;
          witGetPartMinLotSize ( theWorkingWitRun, partList[i], &minLotSize );
          for (tt = 0; tt < nPeriods; tt ++){
            if ( globalProdSchedule[i][tt] > 0 )
              minLotSize[tt] = 0.0;
          }
          witSetPartMinLotSize ( theWorkingWitRun, partList[i], minLotSize );
          portableFree ( minLotSize );
        }
      }
    }
    
  } //endif (Pass One for minimum demand volumes)
  
  /*
   * Pass Two: the nPeriods implosions of rollover
   * the demand vols used here are the amount of demand over and above the
   * min demand vols.
   */

  /*
   * For each Demand, set the demand's shipLateLimit to 0
   * (this effectively "removes" the middle loop in the
   *  earlyDemandScheme (aka original) heuristic)
   */
  for ( i=0; i<nParts; i++ ) {
    for ( j=0; j<nDemandsList[i]; j++ ) {
      witSetDemandShipLateLimit( theWorkingWitRun,
                                 partList[i], allDemandsList[i][j], 0 );
    }
  }
  
        
  /* implosion 0: populate theWorkingWitRun with
     second pass demands stored in theWorkingWitRun's demand app data */
  for ( i=0; i<nParts; i++ ) {
    for ( j=0; j<nDemandsList[i]; j++ ) {
      witGetDemandAppData(theWorkingWitRun, partList[i], allDemandsList[i][j],
                          &demandAppDataPtr);
      assert (demandAppDataPtr != 0) ;
      workingDemandVol[0] =  ( (MyAppData *) demandAppDataPtr)->aboveMinDemandVol()[0];
      witSetDemandDemandVol( theWorkingWitRun,
                             partList[i], allDemandsList[i][j],
                             workingDemandVol );
    }
  }
  
  /* do an heuristic implosion */
  witHeurImplode( theWorkingWitRun );

#ifdef ROLL_DEBUG
    printf ( "\n\nRLH:The results of imploding in time period 0: \n" );
    witWriteShipSched ( theWorkingWitRun, WitSTDOUT,  WitCSV);
#endif


  /*
   * update the global ship schedule, global production schedule, and
   * global substitute BOM entry production schedule
   */
  for ( i=0; i<nParts; i++ ) {
     for ( j=0; j<nDemandsList[i]; j++ ) {
      witGetDemandShipVol( theWorkingWitRun,
                           partList[i], allDemandsList[i][j], &shipVol);
      for ( tt=0; tt<nPeriods; tt++ ) 
        globalShipSchedule[i][j][tt] += shipVol[tt];
      portableFree ( shipVol );
     } /* end for j loop */
    if ( !partIsProduct[i]) continue;
    witGetPartProdVol( theWorkingWitRun, partList[i], &prodVol );
    for ( tt=0; tt<nPeriods; tt++ ) 
      globalProdSchedule[i][tt] += prodVol[tt];
    portableFree ( prodVol );
    //update global sub BOM entry prod schedule
    for ( j=0; j<nBom[i]; j++) {
      for ( k=0; k < nSubsBom[i][j]; k++){
        float * subsProdVol;
        witGetSubsBomEntryProdVol( theWorkingWitRun,
                                   partList[i],
                                   j, // bom entry index
                                   k, // sub bom entry index
                                   &subsProdVol);
        for ( tt=0; tt<nPeriods; tt++)
          globalSubsBomProdSchedule[i][j][k][tt] += subsProdVol[tt];
        portableFree ( subsProdVol );
      }
    }    
  } /* end i-loop */

  /*
   * update the global critical list
   */
  if (computeCriticalList) {
    theCritList.merge(theWorkingWitRun);
  }
  

  /*
   * Loop once for each of the remaining time periods
   */
  for ( t=1; t<nPeriods; t++ ) {
    /*
     * For each part...
     */
    for ( i=0; i<nParts; i++ ) {
      /*
       * Set the part's supply Vol
       */
      witGetPartExcessVol( theWorkingWitRun, partList[i], &excessVol );
      witSetPartSupplyVol (theWorkingWitRun, partList[i], excessVol );
      portableFree(excessVol);

      /*
       * For each demand...
       */
      for ( j=0; j<nDemandsList[i]; j++ ) {

        /*
         * Set theDemand's demand vol for the current period
         * and zero out the demand vol in other periods
         */
        workingDemandVol[t-1] = 0.0;
        /*
         * calculate the unmet demand from the previous implosion and use it
         * to determine the demand for the current implosion iteration
         */
        witGetDemandDemandVol( theWorkingWitRun, partList[i],
                               allDemandsList[i][j], &demandVol );
        witGetDemandShipVol( theWorkingWitRun, partList[i],
                             allDemandsList[i][j], &shipVol);
        unmetDemand = demandVol[t-1] - shipVol[t-1];
        portableFree(demandVol);
        portableFree(shipVol);

      // get "above the min" demand out of the appData of the Working Wit Run
        witGetDemandAppData(theWorkingWitRun, partList[i], allDemandsList[i][j],
                            &demandAppDataPtr);
        assert (demandAppDataPtr != 0) ;
        LgFrTimeVecFloat  passTwoDemandVol =
          ( (MyAppData *) demandAppDataPtr)->aboveMinDemandVol();
        
        // get backlog Yield from the Wit Run and
        // apply backlogYield to backlog ( ie. unmetDemand) from previous implosion  
        void * backlogYieldAppDataPtr;
        witGetDemandAppData(theWitRun, partList[i], allDemandsList[i][j],
                            &backlogYieldAppDataPtr);
        if (backlogYieldAppDataPtr == 0)
          backlogYield = defaultBacklogYield;
        else
          backlogYield = (float *) backlogYieldAppDataPtr;
        assert (backlogYield[t-1] >= 0.0 && backlogYield[t-1] <= 1.0 );        

        workingDemandVol[t] =  passTwoDemandVol[t]
          + (backlogYield[t-1] * unmetDemand);
        witSetDemandDemandVol( theWorkingWitRun, partList[i],
                               allDemandsList[i][j], workingDemandVol);
      } /* end j loop */
    } /* end i loop */


    /*
     * Minimum Lot Sizes
     */
    /* carry over production from previous implosions solutions to
       satisfy minimum lot sizes  -- could be more efficient here */
    if (minLotSizeUsed){
      for ( i=0; i<nParts; i++ ) {
        if ( !partIsProduct[i]) continue;
        if (minLotSizeUsedForPart[i]) {
          float * minLotSize;
          witGetPartMinLotSize ( theWorkingWitRun, partList[i], &minLotSize );
          for (tt = 0; tt < nPeriods; tt ++){
            if ( globalProdSchedule[i][tt] > 0 )
              minLotSize[tt] = 0.0;
          }
          witSetPartMinLotSize ( theWorkingWitRun, partList[i], minLotSize );
          portableFree ( minLotSize );
        }
      }
    }


    /*
     * heuristic implosion
     */
    witHeurImplode( theWorkingWitRun );

#ifdef ROLL_DEBUG
      printf ( "\n\nRLH: The result of imploding in time period %d :", t);
      witWriteShipSched ( theWorkingWitRun, WitSTDOUT,  WitCSV);
#endif

    /*
     * add current production, shipment, and subs BOM entry
     * production schedules to global
     */
    for ( i=0; i<nParts; i++ ) {
      for ( j=0; j<nDemandsList[i]; j++ ) {
        witGetDemandShipVol( theWorkingWitRun, partList[i],
                             allDemandsList[i][j], &shipVol);
        for (tt = 0; tt < t+1; tt ++){
          /* only update to period t, b/c shipLateLimit == 0 */
          globalShipSchedule[i][j][tt] += shipVol[tt];
        }
        portableFree(shipVol);
      } /* end j loop over demands of part[i] */
      if ( !partIsProduct[i]) continue;
      witGetPartProdVol( theWorkingWitRun, partList[i], &prodVol );
      for (tt = 0; tt < nPeriods; tt ++) 
        globalProdSchedule[i][tt] += prodVol[tt];
      portableFree ( prodVol );
      //update global sub BOM entry prod schedule
      for ( j=0; j<nBom[i]; j++) {
        for ( k=0; k < nSubsBom[i][j]; k++){
          float * subsProdVol;
          witGetSubsBomEntryProdVol( theWorkingWitRun,
                                     partList[i],
                                     j, // bom entry index
                                     k, // sub bom entry index
                                     &subsProdVol);
          for ( tt=0; tt<nPeriods; tt++)
            globalSubsBomProdSchedule[i][j][k][tt] += subsProdVol[tt];
          portableFree ( subsProdVol );
        }
      }    
    } //end i-loop
    /* 
     * add current critical list to global
     */
    if (computeCriticalList) {
      theCritList.merge(theWorkingWitRun);
    }
    
  } /* end loop over t */
  portableFree( workingDemandVol );


 /*
  * Stuff the answer --global production, shipment, and substitute
  * BOM entry production schedules-- back
  * into the original witRun
  */
  for ( i=0; i<nParts; i++ ) {
    for ( j=0; j<nDemandsList[i]; j++) {
      witSetDemandShipVol( theWitRun, partList[i], allDemandsList[i][j],
                           globalShipSchedule[i][j] );
    }
    if ( !partIsProduct[i]) continue;
    witSetPartProdVol( theWitRun, partList[i], globalProdSchedule[i] );
    //stuff global sub BOM entry prod schedule into the original witRun
    for ( j=0; j<nBom[i]; j++) {
      for ( k=0; k < nSubsBom[i][j]; k++){
        witSetSubsBomEntryProdVol( theWitRun,
                                   partList[i],
                                   j, // bom entry index
                                   k, // sub bom entry index
                                   globalSubsBomProdSchedule[i][j][k]);
      }
    }        
  } //end i-loop



  /*
   * Post-process the orginal witrun, now that it has the global 
   * production & shipment schedules
   */
  witPostprocess (theWitRun );

  
  /*
   * Clean Up--free up all that malloc-ed storage
   */
    for ( i=0; i<nParts; i++ ) {
      for ( j=0; j<nDemandsList[i]; j++) {
        portableFree(globalShipSchedule[i][j] );
        witGetDemandAppData(theWorkingWitRun,
                            partList[i],
                            allDemandsList[i][j],
                            &demandAppDataPtr);
        assert (demandAppDataPtr != 0) ;
        delete (LgFrTimeVecFloat *) demandAppDataPtr;
      }
      portableFree(globalShipSchedule[i]);
      portableFree(globalProdSchedule[i] );
    }
    portableFree(globalShipSchedule);

    for ( i=0; i<nParts; i++){
      for (j=0; j<nBom[i]; j++){
        for (k=0; k<nSubsBom[i][j]; k++) {
               portableFree ( globalSubsBomProdSchedule[i][j][k] );
        }
        portableFree ( globalSubsBomProdSchedule[i][j] );
      }
      portableFree ( globalSubsBomProdSchedule[i] );
    }
    portableFree ( globalSubsBomProdSchedule );
    
    portableFree(globalProdSchedule);

    for (i=0; i<nParts; i++)
      portableFree (nSubsBom[i]);
    portableFree (nSubsBom);
    portableFree (nBom);
    portableFree(minLotSizeUsedForPart);
    portableFree(partIsProduct);
    delete [] defaultBacklogYield;
    
    for ( i=0; i<nParts; i++ ) {
      for ( j=0; j<nDemandsList[i]; j++ ) portableFree( allDemandsList[i][j] );
      portableFree ( allDemandsList[i] );
    }
    portableFree( allDemandsList );

    for ( i=0; i<nParts; i++ ){
      portableFree( partList[i] );
    }
    portableFree( partList );
    portableFree( nDemandsList );
    witDeleteRun ( theWorkingWitRun );
    return 0;
} /* end function */

/*******************************************************************************
*
* Get Demand Lists -- calling program is responsible for freeing up malloc-ed
*                     memory
*
********************************************************************************/
void getDemandLists(
     WitRun    * theWitRun,
     int         nParts,
     char    **  partList,
     int     **  nDemandsPtr,
     char  ****  demandListPtr)
{
 char ** dList;
 char *** demandList;
 int i;
 int * nDemands;

/* malloc a vector to be a vector of demand lists */
 demandList = (char ***) malloc (nParts * sizeof(char**));
 assert ( demandList != 0 );

/* malloc a vector to hold the number of demands */
 nDemands = (int *) malloc (nParts * sizeof(int));
 assert ( nDemands != 0 );

 for ( i=0; i<nParts; i++){
  demandList[i] = 0;
  nDemands[i] = 0;
 }

 for ( i=0; i<nParts; i++){
   witGetPartDemands ( theWitRun, partList[i], &(nDemands[i]), &dList );
   if (nDemands[i] > 0)
     demandList[i] = dList;
 }

 (*demandListPtr) = demandList;
 (*nDemandsPtr) = nDemands;
}

/*******************************************************************************
*
* Get Minimum Lot Size Used -- calling program responsible for freeing malloc-ed
*                              memory
*
********************************************************************************/
void getMinLotSizeUsed(
     WitRun       * theWitRun,
     int            nParts,
     int            nPeriods,
     char        ** partList,
     witBoolean   * partIsProduct,
     witBoolean   * minLotSizeUsedPtr,
     witBoolean  ** minLotSizeUsedForPartPtr)

{
 int i, t;
 witBoolean   minLotSizeUsed = WitFALSE;
 witBoolean * minLotSizeUsedForPart;

/* malloc a vector of length equal to number of parts,
 * to track whether each product has minimum lot size in
 * some period
 */
 minLotSizeUsedForPart = (witBoolean *) malloc (nParts * sizeof(witBoolean));
 assert ( minLotSizeUsedForPart != 0 );

 for ( i=0; i<nParts; i++){
  minLotSizeUsedForPart[i] = WitFALSE;
 }

 for ( i=0; i<nParts; i++){
   float * minLotSize ;
   float * incLotSize;

   if( !partIsProduct[i] ) continue;
   witGetPartMinLotSize ( theWitRun, partList[i], &minLotSize );
   witGetPartIncLotSize (theWitRun, partList[i], &incLotSize );

   //In addition to keeping track of whether any part has a minLotSize in
   //any period, keep track of whether minLotSizes are used anywhere.
   for ( t=0; t<nPeriods; t++){
     if (minLotSize[t] > 0){
       minLotSizeUsedForPart[i] = WitTRUE;
       minLotSizeUsed = WitTRUE;
       break;
     }
   }
   portableFree ( minLotSize );
   portableFree ( incLotSize );
 }

 (*minLotSizeUsedPtr) = minLotSizeUsed;
 (*minLotSizeUsedForPartPtr) = minLotSizeUsedForPart;

}
/*******************************************************************************
*
* part Is Product -- calling program must free the memory that is malloced here
*
********************************************************************************/
void getProductParts(
     WitRun    * theWitRun,
     int         nParts,
     char    **  partList,
     int     **  productPartsPtr) 
{
 int i;
 int * productParts; 
 witAttr pc;

/* malloc a vector to track whether each part is a product or not*/
 //this information is kept externally to the wit run for computational efficiency.
 
 productParts = (int *) malloc (nParts * sizeof(int));
 assert ( productParts != 0 );

 for ( i=0; i<nParts; i++)
  productParts[i] = 0;

 for ( i=0; i<nParts; i++){
   witGetPartCategory( theWitRun, partList[i], &pc );
   if ( pc == WitPRODUCT ) productParts[i] = 1;
 }

 (*productPartsPtr) = productParts;
}

/*******************************************************************************
*
* setupPassOne - initialize demands for
*                Pass 1: a regular heuristic implosion
*                        towards the minimum demand volumes)
*                and calculate demands for
*                Pass 2: a rollover heuristic implosion
*                        towards the demand vol over and above the minimum
*                        demand volume).
*
* the calling program is responsible for deleting the application data pointer
* that is new-ed here
********************************************************************************/
void setupPassOne(
                  WitRun * theWitRun,
                  WitRun * theWorkingWitRun,
                  int      nPeriods,
                  int      nParts,
                  char **  partList,
                  int *    nDemandsList,
                  char *** allDemandsList,
                  witBoolean * needFirstPassPtr )
{
  int i, j;
  float * cumShipBound;
  float * demandVol;

  //get the cum ship soft lower bounds, which are the cummulated
  //minimum demand volumes
  for ( i=0; i<nParts; i++ ) {
    for ( j=0; j<nDemandsList[i]; j++ ) {
      float * hardLower, * hardUpper;
      witGetDemandCumShipBounds( theWitRun, partList[i], allDemandsList[i][j],
                                 &hardLower,
                                 &cumShipBound,
                                 &hardUpper);
      portableFree ( hardLower);
      portableFree ( hardUpper );
      
      // un-cum the lower bounds and use them as the demand vols for the first pass
      LgFrTimeVecFloat minDemandVol =
        LgFrTimeVecFloat(nPeriods,cumShipBound).undoCum();
      // make sure the de-cummed demand vol is non-negative
      assert ( minDemandVol >= 0.0 );
      if (minDemandVol.max() > 0.0) *needFirstPassPtr = WitTRUE;
      witSetDemandDemandVol( theWorkingWitRun, partList[i],
                             allDemandsList[i][j],
                             minDemandVol.data() );

      //get the total demand
      witGetDemandDemandVol ( theWitRun, partList[i],
                              allDemandsList[i][j], &demandVol );
      LgFrTimeVecFloat demandVolTV = LgFrTimeVecFloat ( nPeriods, demandVol );

      // the amt of demand in excess of the minimum (i.e. "above the min demand")
      // is demand vol used in the second pass. Calculate it and store as appData
      // in the workingWitRun
      MyAppData * madPtr = new MyAppData;
      assert ( madPtr != 0);
      madPtr->aboveMinDemandVol ( demandVolTV - minDemandVol );
      assert ( madPtr->aboveMinDemandVol().min() >= 0.0 );
      witSetDemandAppData(theWorkingWitRun,
                          partList[i], 
                          allDemandsList[i][j], 
                          madPtr );
      
      portableFree ( cumShipBound );
      portableFree ( demandVol );
    } // end j loop     
  } //end i loop
}

/*****************************************************************************
*
* getNBom - malloc a vector of length nParts to hold the number of BOM entrees
*           for each part
*         - calling routine's responsible for freeing memory
*
********************************************************************************/
void  getNBom ( WitRun * theWitRun,
                int      nParts,
                char **  partList,
                int **   nBomPtr )
{
  int i;
  int * nBom;

  //malloc a vector of length nParts and initialize it to zero
  nBom = (int *) malloc (nParts * sizeof(int));
  assert ( nBom != 0 );
  for (i=0; i<nParts; i++)
    nBom[i] = 0;
  
  // get the number of BOM entries for each part from the WitRun and
  // store it in nBom
  for (i=0; i<nParts; i++) {
    witGetPartNBomEntries( theWitRun, partList[i], &nBom[i] );
    assert ( nBom[i] >= 0 );
  }
    
  (*nBomPtr) = nBom;
}

/*****************************************************************************
*
* getNSubsBom - malloc memory and store in it the number of substitute BOM entries for
*               every BOM for every part
*             - calling routine's responsible for freeing memory
*
********************************************************************************/
void getNSubsBom (WitRun * theWitRun,
                  int nParts,
                  char ** partList,
                  int * nBom,
                  int *** nSubsBomPtr)
{
  int i, j ;
  int ** nSubsBom;
  
  nSubsBom = (int **) malloc (nParts * sizeof (int *));
  assert ( nSubsBom != 0 );

  for (i=0; i<nParts; i++){
    nSubsBom[i] = 0;
    if ( nBom[i] > 0 ) {
      nSubsBom[i] = (int * ) malloc ( nBom[i] * sizeof (int));
      assert (nSubsBom[i]  != 0);
      for (j=0; j<nBom[i]; j++){
        witGetBomEntryNSubsBomEntries(theWitRun,
                                      partList[i],
                                      j,
                                      &nSubsBom[i][j]);
        assert ( nSubsBom[i][j] >= 0 );
      }  
    }
  }
  (*nSubsBomPtr) = nSubsBom ;
}
/*****************************************************************************/
#endif
