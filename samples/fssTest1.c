/******************************************************************************
 *
 * Test fss function.
 *
 * 1. Read wit data file (file name specified on command line).
 * 2. Perform implosion
 * 3. Set focusHorizon to last period for all demands
 * 4. get fss
 * 5. increment supplyVol by fss quantities
 * 6. Perform 2nd implosion
 * 7. get fss
 * 8. check to make sure fss obtained in prior step has length zero
 *
 ******************************************************************************/

#include <stdlib.h>
#include <assert.h>
#include <wit.h>


/******************************************************************************
 * Main Program                                                   
 * The name of the wit data file must be specified on command line.
 ******************************************************************************/

void main (int argc, char * argv[]) {

   WitRun *  theWitRun;                  /* Current Wit Run              */
   int nP;

   /*
    * Make sure a wit.data file was specified on command line.
    */
   if ( argc < 2 ) {
      printf( "usage: %s wit_data_file_name\n",argv[0]);
      exit(1);
   }

   /*
    * Establish environment for WIT to run.
    */
   witNewRun(&theWitRun);

   /*
    * Send WIT messages to file wit.out, and write over an existing file.
    */
#if 0
   witSetMesgFileAccessMode( theWitRun, WitTRUE, "w" );
   witSetMesgFileName( theWitRun, WitTRUE, "fssVw.out" );
#endif

   /* 
    * Initialize WIT
    */
   witInitialize( theWitRun );

   /* 
    * Read WIT data file specified on command line.
    */
   witReadData( theWitRun, argv[1] );

   witGetNPeriods( theWitRun, &nP );
   witSetComputeCriticalList( theWitRun, WitTRUE );
   witSetEquitability( theWitRun, 10 );

   witHeurImplode( theWitRun );

   /* set fss vol to demand ship vol */
   {
      int lenPartList, lenDemandList,i,j;
      char ** partList, **demandList;
      witSetUseFocusHorizons( theWitRun, TRUE);
      witGetParts( theWitRun, &lenPartList, &partList );

      /* Set focus horizons */
      for ( i=0; i<lenPartList; i++ ) {
         float *shipVol;
         witGetPartDemands(theWitRun,partList[i], &lenDemandList,&demandList );
         for ( j=0; j<lenDemandList; j++ ) {
            witSetDemandFocusHorizon( theWitRun,
                              partList[i],
                              demandList[j],
                              nP-1);
            free(demandList[j]);
         }
         free(demandList);
         free(partList[j]);
      }
      free(partList);
   }


   /* Increment supplyVol by focus shortage */
   {
   int lenList, p, t;
   char **partList;
   float **fssList;
   witGetFocusShortageVol( theWitRun, &lenList, &partList, &fssList );
   for ( p=0; p<lenList; p++ ) {
      float * supplyVol;
      witGetPartSupplyVol(theWitRun,partList[p],&supplyVol);
      for ( t=0; t<nP; t++ ) {
         supplyVol[t] += fssList[p][t];
      }
      witSetPartSupplyVol(theWitRun,partList[p],supplyVol);
      free(fssList[p]);
      free(partList[p]);
      free(supplyVol);
   }
   free(fssList);
   free(partList);
   }

   /* do another implosion */
   witHeurImplode( theWitRun );

   /* get fss */
   {
   int lenList, p, t;
   char **partList;
   float **fssList;
   witGetFocusShortageVol( theWitRun, &lenList, &partList, &fssList );
   printf(" Length of fssList should be 0, and it is %d\n",lenList);
   printf(" Filename is %s\n",argv[1]);
   assert( lenList == 0 );
   }

   /* Free storage associated with the WIT environment */
   witDeleteRun( theWitRun );

   exit (0);

}  /* main */
