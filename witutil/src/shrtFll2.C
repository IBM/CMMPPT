// ----------------------------------------------------------------
//
// Usage: shrtFll2  <wit_data_file_name>                           
//                  <----mandatory----->                           
//
// ----------------------------------------------------------------
 
 
 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <wit.h>
#include <witutil/src/fullExpl.h>
#include <witutil/src/machdep.h>

#include <iostream>
#include <fstream>

/* ================================================================ */
int writeMrpPlanningSheet(
    WitRun * theWitRun, 
    int      nParts,
    char  ** partList,
    float *  pastDueDemand,
    float ** regMrpConsVol,
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

// ================================================================



int main (int argc, char * argv[]) 

{
    WitRun * theWitRun;
    char  ** partList;
    int      nParts;
    int i;


    // Mandatory Argument: wit_data_file_name   
    if ( argc < 3 ) {
      std::cerr << "Error.  Usage is " << argv[0] << "wit_data_file_name  K\n";
      exit(1);
    }

    int K = atoi(argv[2]);

#if 0
    // Mandatory Argument: wit_data_file_name   
    if ( argc < 2 ) {
      std::cerr << "Error.  Usage is " << argv[0] << "wit_data_file_name\n";
      exit(1);
    }
#endif
    
    // create a witrun and populate it from the specified witData file
    witNewRun(&theWitRun);

    witSetMesgTimesPrint(theWitRun, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
    witSetMesgTimesPrint(theWitRun, WitTRUE, 82, 0);
    witSetMesgTimesPrint(theWitRun, WitTRUE, 338, 0);
    witSetMesgTimesPrint(theWitRun, WitTRUE, 538, 0);        

    witInitialize(theWitRun);
    witSetWit34Compatible(theWitRun, WitTRUE);

    std::cout << "processing witReadData() ... " << std::endl;
    witReadData(theWitRun, argv[1]);


    // get the list of  parts 
    witGetParts(theWitRun, &nParts, &partList);

    // Now instantiate a fullexploder object
    std::cout << "constructing WUtlFullMrpExploder object ... " << std::endl;    
    WUtlFullMrpExploder testExploder(theWitRun);



    std::cout << "running explode(" << K << ")  ... " << std::endl;        
    testExploder.explode(K);


#if 0
    std::cout << "running explode()  ... " << std::endl;        
    testExploder.explode();
#endif    

    float * pastDueDemand;
    float ** totalDemand;
    float ** regMrpConsVol;

#if 0    
    std::cout << "retrieving explode results ... " << std::endl;            
    testExploder.getPastDue(&pastDueDemand);
    testExploder.getMrpConsVolWithPastDue(&totalDemand);
    testExploder.getMrpConsVol(&regMrpConsVol);
#endif
    
    testExploder.print("testExplode.out");
    
    std::cout << "writing plan sheets ... " << std::endl;
#if 0    
    writeMrpPlanningSheet(theWitRun, 
			  nParts, 
			  partList,
                          pastDueDemand,
                          regMrpConsVol,
			  "./planSheet2.out");


    
    // free the solution vectors
    for (i=0; i<nParts; i++) {
      portableFree(totalDemand[i]);
      portableFree(regMrpConsVol[i]);
    }
    
    delete [] pastDueDemand;
    portableFree(totalDemand);
    portableFree(regMrpConsVol);
#endif        

    // free the parts 
    for (i=0; i<nParts; i++)  
	portableFree(partList[i]);
    portableFree(partList);

    // free the witRun
    witDeleteRun(theWitRun);
  
}







/* =================================================================== */
int writeMrpPlanningSheet(
    WitRun * theWitRun, 
    int      nParts,
    char  ** partList,
    float *  pastDueDemand,
    float ** regMrpConsVol,
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

    float * totalIndependentDemand;
    float * demandVol;

    

    float cumTot;


    witGetNPeriods(theWitRun, &nPeriods);



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
      fprintf(outfile, "%12s%12.1f", "Dep Demand", pastDueDemand[i]);
      for (t=0; t<nPeriods; t++) 
          fprintf(outfile, "%12.1f", regMrpConsVol[i][t]);
      fprintf(outfile, "\n");

      /* Compute and write the total  Demand */
      fprintf(outfile, "%12s%12.1f", "Tot Demand", pastDueDemand[i]);
      for (t=0; t<nPeriods; t++) 
	  fprintf(outfile, "%12.1f", totalIndependentDemand[t] + regMrpConsVol[i][t]);
      fprintf(outfile, "\n");

      fprintf(outfile, "\n\n\n");
      
    }
    fclose(outfile);




/* Clean up the memory */
    portableFree(totalIndependentDemand);

    return 0;
}



