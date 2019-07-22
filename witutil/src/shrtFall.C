/* ----------------------------------------------------------------|
|                                                                  |
|                                                                  |
| Usage: shrtFall  <wit_data_file_name>                              |
|                  <----mandatory----->                               |
|                                                                  |
------------------------------------------------------------------ */
 
 
 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <wit.h>
#include <witutil/src/witU5.h>
#include <witutil/src/machdep.h> 

// Note: on usage of WUtlComputeDependentDemandAndPlanOrders()
// If the planOrders array that you pass is NULL, then no planorders
// will be computed.  This sample usage demonstrates using the
// function both by generating planOrders and Not generating them.
// If you #define USE_PLANORDERS, then they will be generated and
// reported.  If you don't define it, then just the demand will
// be reported, and no planorders will be computed.  This should
// make the routine significantly faster (I think).

// #define USE_PLANORDERS




/* ================================================================ */
int writeMrpPlanningSheet(
    WitRun * theWitRun, 
    int      nParts,
    char  ** partList,
    char   * fileName);

/* ================================================================ */
/* ----------------------------------------------------------------

          ------ writeMrpPlanningSheet ------

   This function prints a Planning Sheet report to a file.  You
   must pass it a partList (and a partList length) and it will 
   create a planning sheet for each part on the list showing:
   - for each period (including a past due period)
      - total Independent demand
      - total dependent demand
      - total demand 
      - total firm supply (wit supplyVol)
      - total planned supply (mrpProdVol)
      - total supply
      - projected on-hand (can be negative)

   The return value is 0 if everything was successful.  There 
   is nothing to be free'd.                                      
*/

/* ================================================================ */



int main (int argc, char * argv[]) 

{
    WitRun * theWitRun;
    char  ** partList;
    int      nParts;
    int i;
    
   /* Mandatory Argument: wit_data_file_name    */
   if ( argc < 2 ) {
      printf("Error.  Usage is %s wit_data_file_name\n", argv[0]);
      exit(1);
      }

    witNewRun(&theWitRun);

    witSetMesgTimesPrint(theWitRun, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
    witSetMesgTimesPrint(theWitRun, WitTRUE, 82, 0);

    witInitialize(theWitRun);
    witSetWit34Compatible( theWitRun, WitTRUE );

    witSetWit34Compatible(theWitRun, WitTRUE);

    fprintf(stderr, "Entering witReadData()\n");
    witReadData(theWitRun, argv[1]);
    fprintf(stderr, "    Leaving witReadData()\n");

/* get the list of  parts */
    witGetParts(theWitRun, &nParts, &partList);


/* Print the Mrp planning sheet */
    writeMrpPlanningSheet(theWitRun, 
			  nParts, 
			  partList, 
			  "./planSheet1.out");


/* free the parts */
    for (i=0; i<nParts; i++)  
	portableFree(partList[i]);
    portableFree(partList);

    witDeleteRun(theWitRun);

}







/* =================================================================== */
int writeMrpPlanningSheet(
    WitRun * theWitRun, 
    int      nParts,
    char  ** partList,
    char   * fileName)
{

    FILE * outfile;
    FILE * theWitRunMesgFile;
    int      nPeriods;
    int      i, j, t, tt, ii;
    witAttr  category;

    int nDemands;
    char ** dList;
    int nBom;

    float ** planOrders;
    float ** dependentDemand;

    float * supplyVol;
    float * totalIndependentDemand;
    float * demandVol;

    /* these are parameters that get passed into the
       computeDependentDemandAndPlanOrders function.
       See the witU5.h file for "documentation". */
    int whereToPutOutstandingReqVolForRaw = WUTL_LAST_PERIOD;
    int whereToPutOutstandingReqVolForProductWithEmptyBom = WUTL_LAST_PERIOD;
    int whereToPutOutstandingReqVolForProductWithBom = WUTL_LAST_PERIOD;
    int includeOrderCancelRecommendationInPlanOrder = FALSE;

    int resetDemandVolsToZeroWhenDone = FALSE;
    

    float cumTot;

    WitRun * tempWitRun;
    
    fprintf(stderr, "Entering writeMrpPlanningSheet()\n");


    /* CLIENT (that's you) creates a tempWitRun to be used in the
       computeDependentDemandAndPlanOrders() function.  */ 
    witNewRun(&tempWitRun);

    /* turn off a bunch off annoying messages */
    witSetMesgTimesPrint(tempWitRun, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
    witSetMesgTimesPrint(tempWitRun, WitTRUE, 82, 0);
    witSetMesgTimesPrint(tempWitRun, WitTRUE, 338, 0);    

    /* CLIENT is responsible for cloning (populating) the tempWitRun.
       This is done for efficiency.  Especially when implementing the
       fssAnalysis() function.        */
    witCopyData(tempWitRun, theWitRun);
    

    witGetNPeriods(theWitRun, &nPeriods);

    /* CLIENT mallocs (and owns responsibility for) the storage vectors
       that will be passed to computeDependentDemandAndPlanOrders().  It's
       a bit inconvenient but is more efficient.  Anyway, you malloc two
       vectors of pointers to float.  And then you malloc the float
       time-vectors.  After the code runs and you've done your reporting,
       you, the CLIENT, must free the storage.  */
#ifdef USE_PLANORDERS
    planOrders = (float **) malloc (nParts * sizeof(float *));
    assert(planOrders != NULL);
#else
    planOrders = NULL;
#endif
    
    dependentDemand = (float **) malloc (nParts * sizeof(float *));
    assert(dependentDemand != NULL);

    /* malloc the solution vectors */
    for (i=0; i<nParts; i++) {
#ifdef USE_PLANORDERS      
      planOrders[i] = (float *) malloc(nPeriods * sizeof(float));
      assert(planOrders[i] != NULL);
#endif
      dependentDemand[i] = (float *) malloc(nPeriods * sizeof(float));
      assert(dependentDemand[i] != NULL);
    }


    /* Run the little ditty that does all the computation. See    */
    /* the comments in witU5.h for explanation on what this does. */
    WUtlComputeDependentDemandAndPlanOrders(
          tempWitRun, 
          nParts,     
          partList,
          dependentDemand,  /* Solution vector -- client must malloc and free */
          planOrders,       /* Solution vector -- client must malloc and free */
          whereToPutOutstandingReqVolForRaw,    /* use either WUTL_CURRENT_PERIOD or WUTL_LAST_PERIOD */
          whereToPutOutstandingReqVolForProductWithEmptyBom,  /* see witU5.h for */
          whereToPutOutstandingReqVolForProductWithBom,       /* explanation     */
          includeOrderCancelRecommendationInPlanOrder,
          resetDemandVolsToZeroWhenDone);


    /* -------------------------------------- */
    /*    Now write the Planning Sheet        */
    /* -------------------------------------- */    
    
    /* malloc a vector to be used for computing Independent Demand */
    totalIndependentDemand = (float *) malloc (nPeriods * sizeof(float));
    assert(totalIndependentDemand != NULL);


    outfile = fopen(fileName, "w");
    if (outfile == NULL) 
	return 1;
    fprintf(outfile, "Planning sheets\n\n");
    for (i=0; i<nParts; i++) {

      witGetPartCategory(theWitRun, partList[i], &category);

      fprintf(outfile, "Part ----> %s\n", partList[i]);

      fprintf(outfile, "MRP Plan Sheet:\n");
      fprintf(outfile, "%12s%12s", "", "Past");
      for (t=0; t<nPeriods; t++)
	fprintf(outfile, "%12d", t);
      fprintf(outfile, "\n");


      /* Compute and write total Independent Demand.   */
      for (t=0; t<nPeriods; t++)
        totalIndependentDemand[t] = 0.0;
      witGetPartDemands(theWitRun, partList[i], &nDemands, &dList);
      if (nDemands > 0) {
	  for (j=0; j<nDemands; j++)  {
	      witGetDemandDemandVol(theWitRun, partList[i], dList[j], &demandVol);
	      for (t=0; t<nPeriods; t++)
		  totalIndependentDemand[t] += demandVol[t];
	      portableFree(demandVol);
              portableFree(dList[j]);
	  }
      }
      portableFree(dList);
      fprintf(outfile, "%12s%12.1f", "Ind Demand", 0.0);      
      for (t=0; t<nPeriods; t++) 
	  fprintf(outfile, "%12.1f", totalIndependentDemand[t]);
      fprintf(outfile, "\n");

      

      /* Compute and write the total Dependent Demand */
      fprintf(outfile, "%12s%12.1f", "Dep Demand", 0.0);
      for (t=0; t<nPeriods; t++) 
          fprintf(outfile, "%12.1f", dependentDemand[i][t]);
      fprintf(outfile, "\n");

      /* Compute and write the total  Demand */
      fprintf(outfile, "%12s%12.1f", "Tot Demand", 0.0);
      for (t=0; t<nPeriods; t++) 
	  fprintf(outfile, "%12.1f", totalIndependentDemand[t] + dependentDemand[i][t]);
      fprintf(outfile, "\n");

#ifdef USE_PLANORDERS
      /* Write out the FIRM Supply (WIT supply line) */
      witGetPartSupplyVol(theWitRun, partList[i], &supplyVol);      
      fprintf(outfile, "%12s%12.1f", "Firm Orders", 0.0);
      for (t=0; t<nPeriods; t++) 
	  fprintf(outfile, "%12.1f", supplyVol[t]);
      fprintf(outfile, "\n");

      /* Write out the planned orders (this is the tricky part) */
      fprintf(outfile, "%12s%12.1f", "Plan orders", 0.0);
      for (t=0; t<nPeriods; t++) 
	  fprintf(outfile, "%12.1f", planOrders[i][t]);
      fprintf(outfile, "\n");

      /* Write out the total Supply */
      fprintf(outfile, "%12s%12.1f", "Tot Supply", 0.0);
      for (t=0; t<nPeriods; t++) 
	  fprintf(outfile, "%12.1f", (supplyVol[t] + planOrders[i][t]));
      fprintf(outfile, "\n");

      /* Write out the Projected On-Hand */
      cumTot = 0.0;
      fprintf(outfile, "%12s%12.1f", "Proj OnHand", cumTot);
      for (t=0; t<nPeriods; t++)   {
	  cumTot += (supplyVol[t] + planOrders[i][t]
                     - totalIndependentDemand[t] - dependentDemand[i][t]);
          if (category == WitCAPACITY)
            fprintf(outfile, "%12.1f", supplyVol[t] - dependentDemand[i][t]);
          else
            fprintf(outfile, "%12.1f", cumTot);
      }
      portableFree(supplyVol);
#endif
      fprintf(outfile, "\n\n\n");
      
    }
    fclose(outfile);




/* Clean up the memory */
    portableFree(totalIndependentDemand);

    for (i=0; i<nParts; i++) {
#ifdef USE_PLANORDERS      
      portableFree(planOrders[i]);
#endif      
      portableFree(dependentDemand[i]);
    }
#ifdef USE_PLANORDERS
    portableFree(planOrders);
#endif
    portableFree(dependentDemand);

    witDeleteRun(tempWitRun);
    
    fprintf(stderr, "    Leaving writeMrpPlanningSheet()\n");
    return 0;
}



