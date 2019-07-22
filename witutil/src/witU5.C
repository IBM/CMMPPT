#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <witutil/src/witU5.h> 
#include <witutil/src/machdep.h>

#include <iostream>

#define ALTERNATE_ROLL_METHOD
// #define LGFR_WUTL_WRITE_MRP_ITERATION_COUNT_TO_WIT_MSG_FILE
/* =================================================================== */
void WUtlComputeDependentDemandAndPlanOrders(
                                         
    /* a COPY of the original witRun.  MUST be already populated */
    WitRun * tempWitRun, 
    int      nParts,     
    char  ** partList,
    
    /* Solution vector -- client must malloc and free */
    float ** dependentDemand,
    
    /* Solution vector -- client must malloc and free */
    float ** planOrders,

    /* use either WUTL_CURRENT_PERIOD or WUTL_LAST_PERIOD */
    int   whereToPutOutstandingReqVolForRaw,    
    int   whereToPutOutstandingReqVolForProductWithEmptyBom,
    int   whereToPutOutstandingReqVolForProductWithBom,
    int   includeOrderCancelRecommendationInPlanOrder,

    /* use either 1, if you want the demandVols in tempWitRun
       reset to zero (including any dummy demands created here),
       OR, 0, if you don't.  Note, if you will re-use this
       tempWitRun again for this routine, then set it to 1.
       Otherwise set it to 0 (it'll go faster).           */
    int   resetDemandVolsToZeroWhenDone)
    

{

    int      nPeriods;
    int      i, j, t, tt, ii;
    witAttr  category;

    int nDemands;
    char ** dList;
    int nBom;

    float * cycleTime;
    float * reqVol;
    int isReqVolPositive;
    float * mrpProdVol;
    float * mrpConsVol;
    float * mrpExcessVol;    

    int couldWeMoveTheReqVol;
    float * movedReqVol;
    int needToMrpAgain;

    float * addedDemandVol;
    
    float * zeroVec;
    float * aFloatVec;
    float * totalIndDemandVol;
    float * demandVol;

    float cumTot;

#ifdef LGFR_WUTL_WRITE_MRP_ITERATION_COUNT_TO_WIT_MSG_FILE
    FILE * theWitMesgFile;
    witGetMesgFile(tempWitRun, &theWitMesgFile);
#endif
      
#if 0
    fprintf(stderr, "Entering computeDependentDemandAndPlanOrders()\n");
#endif
    
    witGetNPeriods(tempWitRun, &nPeriods);

    /* malloc a vector to be a vector of Zero's */
    zeroVec = (float *) malloc (nPeriods * sizeof(float));
    assert(zeroVec != NULL);
    for (t=0; t<nPeriods; t++)
	zeroVec[t] = 0.0;
    
    /* malloc a vector to be used as the movedReqVol */
    movedReqVol = (float *) malloc (nPeriods * sizeof(float));
    assert(movedReqVol != NULL);


    /* initialize dependentDemand result array  */
    for (i=0; i<nParts; i++) {
      for (t=0; t<nPeriods; t++) {
        dependentDemand[i][t] = 0.0;
      }
    }


    /* initialize planOrders result array  */
    /* NOTE: if planOrders is NULL, then skip */
    if (! (planOrders == NULL)) {
      for (i=0; i<nParts; i++) {
        for (t=0; t<nPeriods; t++) {
          planOrders[i][t] = 0.0;
        }
      }
    }


#ifdef ALTERNATE_ROLL_METHOD

    //    cerr << "WUtlComputeDependentDemandAndPlanOrders: computing firstExplodablePeriod " << endl;
    
    witBoolean ** canBuild = (witBoolean **) malloc(nParts * sizeof(witBoolean *));
    assert(canBuild != NULL);
    witBoolean postProcessed;
    witGetPostprocessed(tempWitRun, &postProcessed); 
    if (! postProcessed)
      witPostprocess(tempWitRun);
    witBoolean * operExecBool;
    for (i=0; i<nParts; i++) {
      // intialize canBuild to NULL
      canBuild[i] = NULL;
      witGetPartCategory(tempWitRun, partList[i], &category);
      if (category == WitPRODUCT) {
        witGetOperationExecutable(tempWitRun, partList[i], &operExecBool);
        canBuild[i] = operExecBool;
      }
    }
    //    cerr << "WUtlComputeDependentDemandAndPlanOrders: done firstExplodablePeriod " << endl;    
#endif

    
    needToMrpAgain = 1;
    for (size_t loopCount=0;
         needToMrpAgain;
         loopCount++    ) {


#ifdef LGFR_WUTL_WRITE_MRP_ITERATION_COUNT_TO_WIT_MSG_FILE
      fprintf(theWitMesgFile,
              "WUtlComputeDependentDemandAndPlanOrders:  entering witMrp(%d)\n",
              loopCount);
#endif

      /* do the MRP */      
      witMrp(tempWitRun);
      
#ifdef LGFR_WUTL_WRITE_MRP_ITERATION_COUNT_TO_WIT_MSG_FILE
      fprintf(theWitMesgFile,
              "WUtlComputeDependentDemandAndPlanOrders:  leaving witMrp(%d)\n",
              loopCount);
#endif      
      
      needToMrpAgain = 0;
      for (i=0; i<nParts; i++) {
        /* step 1: Zero out all demands */
        witGetPartDemands(tempWitRun, partList[i], &nDemands, &dList);
        for (j=0; j<nDemands; j++) {
          witSetDemandDemandVol(tempWitRun, partList[i], dList[j], zeroVec);
          portableFree(dList[j]);
        }
        portableFree(dList);

        /* step 2: add mrpConsVol to dependentDemand */
        witGetPartMrpConsVol(tempWitRun, partList[i], &mrpConsVol);
        for (t=0; t<nPeriods; t++) {
          dependentDemand[i][loopCount==0?t:0] += mrpConsVol[t];
        }
        portableFree(mrpConsVol);
        
        /* step 3: add mrpProdVol to planOrders (this is for products only) */
        /* NOTE: Skip this step when planOrders == NULL */
        witGetPartCategory(tempWitRun, partList[i], &category);
        if (planOrders) {
          if (category == WitPRODUCT) {
            witGetPartMrpProdVol(tempWitRun, partList[i], &mrpProdVol);          
            for (t=0; t<nPeriods; t++) {
              planOrders[i][t] += mrpProdVol[t];
            }
            portableFree(mrpProdVol);
          }
        }

        /* step 4: Examine reqVols */
        witGetPartReqVol(tempWitRun, partList[i], &reqVol);
        isReqVolPositive = 0;        
        for (t=0; t<nPeriods; t++) {
          if (reqVol[t] > 0.0) {
            isReqVolPositive = 1;
            break;
          }
        }

        /* If there are no net requirements, then skip to the next part */
        if (! isReqVolPositive) {
          portableFree(reqVol);
          continue;
        }

        /* CAPACITY: */
        /* Add any netRequirements to the planOrders NOW.  otherwise
           we won't see them again.  This is a bit bad because we could
           get more supply later through unexploded negative usage rates.
           We'll have to fix it in the analysis.  */

        if (category == WitCAPACITY) {
          if (planOrders) {            
            for (t=0; t<nPeriods; t++) {
              planOrders[i][t] += reqVol[t];
            }
          }
	  portableFree(reqVol);
          continue;
        }

        /*
         * If the part is RAW, then we just add the reqVol right back onto the
         * demand
         * We add it onto the first demand record in the list.
         * If there is none, 
         * then create a new demand record
         */
        if (category == WitRAW) {
          addedDemandVol = reqVol;
        }

        /* PRODUCTS:  These are a bit complicated */
        if (category == WitPRODUCT) {
          witGetPartNBomEntries(tempWitRun, partList[i], &nBom);          
          if (nBom == 0) {

            /*
             * sub case 1: If the thing has NO children then
             *   treat it just like 
             *   a raw part.  Put a demand on it in the same period.
             */
            addedDemandVol = reqVol;            
          }

          else {    /* Part has children */
            /* this is tricky.  we need to move the demand out */
            /* first see if you can move it out in time to a period */
            /* when we can explode it further. */

            /* re-set the movedReqVol vector to 0 */
            for (t=0; t<nPeriods; t++) {
              movedReqVol[t] = 0.0;
            }

#ifndef ALTERNATE_ROLL_METHOD            
            witGetPartCycleTime(tempWitRun, partList[i], &cycleTime);
#endif            
            for (t=0; t<nPeriods; t++) {
              if (reqVol[t] > 0.0) {
                couldWeMoveTheReqVol = 0;

                // see if you can move the reqVol to the next period
                //                if (t+1 < nPeriods) {
                //                  tt = t+1;
                //                  couldWeMoveTheReqVol = 1;
                //                }


                for (tt=t+1; tt<nPeriods; tt++) {
#ifdef ALTERNATE_ROLL_METHOD
                  if (canBuild[i][tt]) {
#else                  
                  if ((floor(cycleTime[tt]) <= tt) && (cycleTime[tt] >= 0.0)) {
#endif                                                    
                    couldWeMoveTheReqVol = 1;
                    break;
                  }
                }

                /* if you can move the reqVol out to a feasible period then we
                   need to mrp again.  */                
                if (couldWeMoveTheReqVol) {
                    movedReqVol[tt] += reqVol[t];                  
                    needToMrpAgain = 1;
                }
                /* if you couldn't move the reqVol out to a feasible period,
                   then drop it back on this part as a demand.  */
                else {
                    movedReqVol[t] += reqVol[t];                  
                }

              }
            }
            /* AFter scanning the entire reqVol vector, we need to add a
               demand back onto this part.  But, mkae sure you add the moved
               reqVol rather than the plain reqVol.  */
            addedDemandVol = movedReqVol;

            /* Now we must replace the supplyVol with the mrpExcess */
            witGetPartMrpExcessVol(tempWitRun, partList[i], &mrpExcessVol);
            witSetPartSupplyVol(tempWitRun, partList[i], mrpExcessVol);
            portableFree(mrpExcessVol);
#ifndef ALTERNATE_ROLL_METHOD            
            portableFree(cycleTime);
#endif
          }
        }



        /* Need to set one of the existing demand stream volumes to */
        /* addedDemandVol.  IF this part has no demand streams, then */
        /* we create a dummy one.                                    */
        witGetPartDemands(tempWitRun, partList[i], &nDemands, &dList);
        if (nDemands == 0) {
          witAddDemand (
             tempWitRun,
             partList[i],
             "xxSwellingxxItchingxxBrainxx");

          witSetDemandDemandVol (
             tempWitRun, 
             partList[i],
             "xxSwellingxxItchingxxBrainxx",
             addedDemandVol);
        }
        else {
          witSetDemandDemandVol(tempWitRun, partList[i], dList[0],
                                addedDemandVol);
        }
        for (j=0; j<nDemands; j++) {
          portableFree(dList[j]);
        }
        portableFree(dList);
        portableFree(reqVol);
      }
    } 

    /* Now do one final cleanup loop to do final bookeeping for planOrders. */
    /* NOTE: if planOrders == NULL, then we skip right over this entire loop */
    if (planOrders) {
      for (i=0; i<nParts; i++) {
        witGetPartCategory(tempWitRun, partList[i], &category);

        /*  Pick up any outstanding mrpExcessVol or reqVol.  Add these onto  */
        /*  the planOrders.  */

        if (category == WitCAPACITY) {
          /* for now do nothing.  However, we need to figure out what
             to do about extra supply that appears due to negative
             usage rates. */
        ;
        }
        if (category == WitRAW) {
          witGetPartReqVol(tempWitRun, partList[i], &reqVol);
          witGetPartMrpExcessVol(tempWitRun, partList[i], &mrpExcessVol);
          for (t=0; t<nPeriods; t++) {
            if (whereToPutOutstandingReqVolForRaw == WUTL_LAST_PERIOD ) 
              planOrders[i][nPeriods-1] += reqVol[t];
            else 
              planOrders[i][t] += reqVol[t];
            
            if (includeOrderCancelRecommendationInPlanOrder)
              planOrders[i][t] -= mrpExcessVol[t];          
          }
          portableFree(reqVol);
          portableFree(mrpExcessVol);
        }
        

        if (category == WitPRODUCT) {
          witGetPartReqVol(tempWitRun, partList[i], &reqVol);
          witGetPartMrpExcessVol(tempWitRun, partList[i], &mrpExcessVol);
          witGetPartNBomEntries(tempWitRun, partList[i], &nBom);
          for (t=0; t<nPeriods; t++) {
            /* should we subtract off the mrpExcessVol? */
            if (includeOrderCancelRecommendationInPlanOrder)
              planOrders[i][t] -= mrpExcessVol[t];

            /* Product with Empty Bom  */
            if (nBom == 0) {
              if (whereToPutOutstandingReqVolForProductWithEmptyBom ==
                  WUTL_LAST_PERIOD) 
                planOrders[i][nPeriods-1] += reqVol[t];
              else 
                planOrders[i][t] += reqVol[t];
            }

            /* Product with Children */
            else {
              if (whereToPutOutstandingReqVolForProductWithBom ==
                  WUTL_LAST_PERIOD) 
                planOrders[i][nPeriods-1] += reqVol[t];
              else 
                planOrders[i][t] += reqVol[t];
            }
          }
          portableFree(reqVol);
          portableFree(mrpExcessVol);
        }
      }
    }
      
      

        /* OK, we need to cleanup the demands in the tempWitRun:  So we   */
        /* set them ALL to zero, and let the calling application set them */
        /* to what they were.  Remember, we told you that we're going to  */
        /* muck up the demands.                                           */

    if (resetDemandVolsToZeroWhenDone) {
      for (i=0; i<nParts; i++) {
        witGetPartDemands(tempWitRun, partList[i], &nDemands, &dList);
        for (j=0; j<nDemands; j++) {
          witSetDemandDemandVol(tempWitRun, partList[i], dList[j], zeroVec);
          portableFree(dList[j]);
        }
        portableFree(dList);
      }
    }


    /* Clean up the memory */
    portableFree(zeroVec);
    portableFree(movedReqVol);
    
#ifdef ALTERNATE_ROLL_METHOD
    for (i=0; i<nParts; i++) {
      if (canBuild[i])
        portableFree(canBuild[i]);
    }
    portableFree(canBuild);
#endif
    
#if 0    
    fprintf(stderr, "    Leaving computeDependentDemandAndPlanOrders()\n");
#endif
}

