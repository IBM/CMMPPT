/****************************************************************************** 
 *
 * Sample WIT API Program
 *
 * This program defines a multi-level BOM with substitutes and bounds.
 * It then uses witOptImplode to compute a production schedule. 
 * The production schedule is obtained and printed.
 *
 * All data values are defined internally to this application.  A more 
 * realistic example would obtain the data from an external data base. 
 *
 ******************************************************************************/
 
/******************************************************************************/
/* Please Note:                                                               */
/*    This program will no longer work, because it calls                      */
/*    variable-argument-list WIT API functions and these have been removed    */
/*    from WIT.                                                               */
/******************************************************************************/

#include <stdlib.h>
#include <wit.h>

#define OPT_IMPLODE 


void checkWitRc( char * funcName, int rc );

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
void main (int argc, char * argv[])
{
   witReturnCode rc;
   const int nPeriods = 8;
   WitRun * theWitRun;

   /*
    * Establish environment for the WIT problem.
    */
   rc = witNewRun( &theWitRun );
   checkWitRc( "witNewRun", rc );

   /*
    * Set flag so WIT will return to this program after a severe error.
    */
   rc = witSetMesgStopRunning( theWitRun, 
           WitFALSE, WitSEVERE_MESSAGES, WitFALSE );
   checkWitRc( "witSetMesgStopRunning", rc );

   /*
    * Initialize WIT
    */
   rc = witInitialize ( theWitRun );
   checkWitRc( "witInitialize", rc );

   /*
    * Turn on 3.4 compatibility mode.
    */
   witSetWit34Compatible( theWitRun, WitTRUE );

   /*
    * Set the title, number of periods, and objective function choice.
    */
   rc = witSetTitle( theWitRun, "WIT API Sample" );
   checkWitRc( "witSetTitle", rc );

   rc = witSetNPeriods( theWitRun, nPeriods );
   checkWitRc( "witSetNPeriods", rc );

   rc = witSetObjChoice( theWitRun, 1 );
   checkWitRc( "witSetObjChoice", rc );

   /*
    * Set weight of objective function soft bound violations.
    */
   rc = witSetWbounds( theWitRun, 200000.0 );
   checkWitRc( "witSetWbounds", rc );

   /*
    * Define part COMP1, using variable argument function witAddPartVa.
    */
   {
      float supply[]           = {101., 50., 50., 50., 50., 50., 50., 50.};
      float stock[]            = {  1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.};
      float softlb[]           = { 10.,  9.,  8.,  7.,  6.,  5.,  4.,  3.};

      rc = witAddPartVa( theWitRun, "COMP1", WitRAW,
              WitNsupplyVol,         supply,
              WitNobj1StockCost,      stock,
              WitNstockBounds,    NULL, softlb, NULL,
              NULL );
      checkWitRc( "witAddPartVa", rc );
   }

   /*
    * Define part COMP2, using fixed argument functions
    */
   {
      float supply[]           = {52.5, 10., 10., 10., 10., 10., 10., 10.};
      float obj1Stock[]        = {  1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.};
      float softlb[]           = {  3.,  4.,  5.,  6.,  7.,  8.,  9., 10.};

      rc = witAddPart( theWitRun, "COMP2", WitRAW );
      checkWitRc( "witAddPart", rc );

      rc = witSetPartSupplyVol( theWitRun, "COMP2", supply );
      checkWitRc( "witSetPartSupplyVol", rc );

      rc = witSetPartObj1StockCost( theWitRun, "COMP2", obj1Stock );
      checkWitRc( "witSetPartObj1StockCost", rc );

      rc = witSetPartStockBounds( theWitRun, "COMP2", NULL, softlb, NULL );
      checkWitRc( "witSetPartStockBounds", rc );
   }

   /*
    * Define part COMP3
    */
   {
      float supply[]           = {202.,200.,200.,200.,300.,300.,300.,300.};
      float stock[]            = {  1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.};

      rc = witAddPartVa( theWitRun, "COMP3", WitRAW,
              WitNsupplyVol,         supply,
              WitNobj1StockCost,      stock,
              NULL );
      checkWitRc( "witAddPartVa", rc );
   }

   /*
    * Define part COMP4
    */
   {
      float supply[]           = {150.,150.,150.,150.,150.,150.,150.,150.};
      float stock[]            = {  1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.};

      rc = witAddPartVa( theWitRun, "COMP4", WitRAW,
              WitNsupplyVol,        supply,
              WitNobj1StockCost,     stock,
              NULL );
      checkWitRc( "witAddPartVa", rc );
   }

   /*
    * Define part COMP5
    */
   {
      float supply[]           = {400.,  0.,  0.,300.,  0.,  0.,300.,  0.};
      float stock[]            = {  1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.};

      rc = witAddPartVa( theWitRun, "COMP5", WitRAW,
              WitNsupplyVol,      supply,
              WitNobj1StockCost,   stock,
              NULL );
      checkWitRc( "witAddPartVa", rc );
   }

   /*
    * Define part SUBASMBL1, with a BOM using COMP1 and COMP2
    */
   {
      float stock[]            = {200.,  1.,  1.,  1.,  1.,  1.,  1.,  1.};
      float cycle[]            = {  1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.};
      int   yield[]            = { 95., 95., 95., 95., 95., 95., 95., 95.};
      float prod[]             = { 10., 10., 10., 10., 10., 10., 10., 10.};
      float scrap[]            = { 10., 10., 10., 10., 10., 10., 10., 10.};

      rc = witAddPartVa( theWitRun, "SUBASMBL1", WitPRODUCT,
              WitNobj1StockCost,   stock,
              WitNcycleTime,       cycle,
              WitNyield,           yield,
              WitNobj1ProdCost,    prod,
              WitNobj1ScrapCost,   scrap,
              NULL );
      checkWitRc( "witAddPartVa", rc );

      /* Add first BOM entry with variable argument function */
      rc = witAddBomEntryVa(  theWitRun, "SUBASMBL1", "COMP1",
              WitNusageTime, 0.6,
              WitNlatestPeriod,        3,
              NULL );
      checkWitRc( "witAddBomEntryVa", rc );

      /* Add second BOM entry with fixed argument functions */
      rc = witAddBomEntry (  theWitRun, "SUBASMBL1", "COMP3" );
      checkWitRc( "witAddBomEntry", rc );

      rc = witSetBomEntryUsageTime(  theWitRun, "SUBASMBL1", 1, 0.8 );
      checkWitRc( "witSetBomEntryUsageTime", rc );

      rc = witSetBomEntryUsageRate(  theWitRun, "SUBASMBL1", 1, 2.0 );
      checkWitRc( "witSetBomEntryUsageRate", rc );

      rc = witSetBomEntryFallout(  theWitRun, "SUBASMBL1", 1, 5 );
      checkWitRc( "witSetBomEntryFallout", rc );

      rc = witSetBomEntryLatestPeriod(  theWitRun, "SUBASMBL1", 1, 7 );
      checkWitRc( "witSetBomEntryLatestPeriod", rc );
   } 


   /*
    * Define part SUBASMBL2, with a BOM using COMP2 twice with different
    * usage rates and effectivity dates, and COMP5.
    * A demand of 20 units is placed on the part for periods 2 through 7.
    */
   {
      float supply[]          = {110.,  0., 10.,  0., 10.,  0.,  0., 10.};
      float stock[]           = {  1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.};
      float cycle[]           = { 1.2, 1.2, 1.2,-10.,-10., 3.8, 3.8, 3.8};
      int   yield[]           = { 95., 95., 95., 95., 96., 96., 96., 96.};
      float prod[]            = { 10., 10., 10., 10., 10., 10., 10., 10.};
      float scrap[]           = { 10., 10., 10., 10., 10., 10., 10., 10.};

      float demand[]          = {  0.,  0., 20., 20., 20., 20., 20., 20.};
      float ship[]            = {  0.,  0.,100.,100.,100.,100.,100.,100.};
      float cumShip[]         = {  0.,  0., 2.0, 2.0, 2.0, 2.0, 2.0, 2.0};

      rc = witAddPartVa( theWitRun, "SUBASMBL2", WitPRODUCT,
              WitNsupplyVol,       supply,
              WitNobj1StockCost,   stock,
              WitNcycleTime,       cycle,
              WitNyield,           yield,
              WitNobj1ProdCost,    prod,
              WitNobj1ScrapCost,   scrap,
              NULL );
      checkWitRc( "witAddPartVa", rc );

      /* Add 1st BOM entry using COMP2 */
      rc = witAddBomEntryVa(  theWitRun, "SUBASMBL2", "COMP2",
              WitNusageTime,       1.1,
              WitNusageRate,       2.0,
              WitNlatestPeriod,    2,
              WitNmandEC,          WitTRUE,
              NULL );
      checkWitRc( "witAddBomEntry", rc );

      /* Add 2nd BOM entry also using COMP2 */
      rc = witAddBomEntryVa( theWitRun, "SUBASMBL2", "COMP2",
              WitNusageTime,       0.5,
              WitNusageRate,       1.0,
              WitNearliestPeriod,  2,
              WitNlatestPeriod,    5,
              WitNmandEC,          WitTRUE,
              NULL );
      checkWitRc( "witAddBomEntry", rc );

      /* Add 3rd BOM entry using COMP5 */
      rc = witAddBomEntryVa(  theWitRun, "SUBASMBL2", "COMP5",
              WitNusageTime,       0.4,
              WitNusageRate,       1.0,
              NULL );
      checkWitRc( "witAddBomEntryVa", rc );

      rc = witAddDemandVa(  theWitRun, "SUBASMBL2", "S2DEM1",
              WitNdemandVol,       demand,
              WitNobj1ShipReward,  ship,
              WitNobj1CumShipReward, cumShip,
              NULL );
      checkWitRc( "witAddDemand", rc );
   }

   /*
    * Define part SUBASMBL3, with a BOM using COMP1
    */
   {
      float supply[]          = {160.,  0., 10.,  0., 10.,  0.,  0., 10.};
      float stock[]           = {  1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.};
      float cycle[]           = { 1.2, 1.2, 1.2,-10.,-10., 3.8, 3.8, 3.8};
      int   yield[]           = { 95., 95., 95., 95., 96., 96., 96., 96.};
      float prod[]            = { 10., 10., 10., 10., 10., 10., 10., 10.};
      float scrap[]           = { 10., 10., 10., 10., 10., 10., 10., 10.};

      rc = witAddPartVa(  theWitRun, "SUBASMBL3", WitPRODUCT,
              WitNsupplyVol,       supply,
              WitNobj1StockCost,   stock,
              WitNcycleTime,       cycle,
              WitNyield,           yield,
              WitNobj1ProdCost,    prod,
              WitNobj1ScrapCost,   scrap,
              NULL );
      checkWitRc( "witAddPartVa", rc );

      rc = witAddBomEntry ( theWitRun, "SUBASMBL3", "COMP1" ); 
      checkWitRc( "witAddBomEntry", rc );
   }
   
   /*
    * Define part PROD1, with BOM using SUBASMBL1 and SUBASMBL2.
    * Place two demand streams on PROD1.
    */
   {
      float stock[]           = { 51.,  1.,  1.,  1.,  1.,  1.,  1.,  1.};
      float demand1[]         = { 10., 10., 40., 40., 50., 50., 50., 50.};
      float cycle[]           = { 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0};
      int   yield[]           = { 93., 93., 93., 93., 93., 93., 93., 93.};
      float prod[]            = { 10., 10., 10., 10., 10., 10., 10., 10.};
      float scrap[]           = { 10., 10., 10., 10., 10., 10., 10., 10.};

      float dem1[]            = { 10., 10., 40., 40., 50., 50., 50., 50.};
      float dem1ship[]        = {  0.,  0.,100.,100.,100.,100.,100.,100.};
      float dem1CumShip[]     = { 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0};

      float dem2[]            = {  0., 10., 20., 20., 20., 30., 30., 30.};
      float dem2ship[]        = {100.,100.,100.,100.,100.,100.,100.,100.};
      float dem2CumShip[]     = { 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0};

      rc = witAddPartVa(  theWitRun, "PROD1", WitPRODUCT,
              WitNobj1StockCost,   stock,
              WitNcycleTime,       cycle,
              WitNyield,           yield,
              WitNobj1ProdCost,    prod,
              WitNobj1ScrapCost,   scrap,
              NULL );
      checkWitRc( "witAddPartVa", rc );

      rc = witAddBomEntryVa(  theWitRun, "PROD1", "SUBASMBL1",
              WitNusageTime,        1.7,
              NULL );
      checkWitRc( "witAddBomEntryVa", rc );

      rc = witAddBomEntryVa(  theWitRun, "PROD1", "SUBASMBL3",
               WitNusageTime,       1.8,
               NULL );
      checkWitRc( "witAddBomEntryVa", rc );

      rc = witAddDemandVa(  theWitRun, "PROD1", "P1DEM1",
              WitNdemandVol,        dem1,
              WitNobj1ShipReward,   dem1ship,
              WitNobj1CumShipReward,dem1CumShip,
              NULL );
      checkWitRc( "witAddDemand", rc );

      rc = witAddDemandVa(  theWitRun, "PROD1", "P1DEM2",
              WitNdemandVol,       dem1,
              WitNobj1ShipReward,  dem1ship,
              WitNobj1CumShipReward, dem1CumShip,
              NULL );
      checkWitRc( "witAddDemandVa", rc );
   }


   /*
    * Define part PROD2, with BOM using SUBASMBL2, SUBASMBL3,
    * and COMP4.  Allow COMP1 to be substituted for SUBASMBL2.
    * Allow COMP1, SUBASMBL2, COMP2, or COMP4 (with a different fallout)
    * to be substituted for COMP4.
    * Place a demand stream on PROD2.
    */
   {
      float supply[]          = { 40.,  9.,  8.,  7.,  6.,  5.,  4.,  3.};
      float stock[]           = {  1.,  1.,  1.,  1.,  1.,  1.,  1.,  1.};
      float cycle[]           = {  .8,  .8,  .8,  .8,  .8,  .8,  .8,  .8};
      int   yield[]           = { 95., 95., 95., 95., 95., 95., 95., 95.};
      float prod[]            = { 10., 10., 10., 10., 10., 10., 10., 10.};
      float scrap[]           = { 10., 10., 10., 10., 10., 10., 10., 10.};

      float demand[]          = { 20., 20., 20., 20., 20., 20., 20., 20.};
      float ship[]            = {100.,100.,100.,100.,100.,100.,100.,100.};
      float cumShip[]         = { 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0};

      rc = witAddPartVa(  theWitRun, "PROD2", WitPRODUCT,
              WitNsupplyVol,       supply,
              WitNobj1StockCost,   stock,
              WitNcycleTime,       cycle,
              WitNyield,           yield,
              WitNobj1ProdCost,    prod,
              WitNobj1ScrapCost,   scrap,
              NULL );
      checkWitRc( "witAddPartVa", rc );

      rc = witAddBomEntryVa(  theWitRun, "PROD2", "SUBASMBL2",
              WitNusageTime,        0.6,
              NULL );
      checkWitRc( "witAddBomEntryVa", rc );

      rc = witAddBomEntry(  theWitRun, "PROD2", "SUBASMBL3" );
      checkWitRc( "witAddBomEntry", rc );

      rc = witAddBomEntryVa(  theWitRun, "PROD2", "COMP4",
              WitNusageTime,      0.4,
              WitNusageRate,      3.0,
              NULL );
      checkWitRc( "witAddBomEntryVa", rc );

      /* Add COMP1 as a substitute for SUBASMBL2 */
      rc = witAddSubsBomEntryVa(  theWitRun, "PROD2", 0, "COMP1",
              WitNusageRate,      2.0,
              WitNfallout,        1,
              NULL );
      checkWitRc( "witAddSubsBomEntryVa", rc );

      /* Add COMP1 as a substitute for COMP4 */
      rc = witAddSubsBomEntryVa(  theWitRun, "PROD2", 2, "COMP1",
              WitNusageRate,      3.0,
              WitNfallout,        1,
              NULL );
      checkWitRc( "witAddSubsBomEntryVa", rc );

      /* Add SUBASMBL2 as a substitute for COMP4 */
      rc = witAddSubsBomEntryVa(  theWitRun, "PROD2", 2, "SUBASMBL2",
              WitNusageRate,      5.0,
              WitNfallout,        3,
              NULL );
      checkWitRc( "witAddSubsBomEntryVa", rc );

      /* Add COMP2 as a substitute for COMP4 */
      rc = witAddSubsBomEntryVa(  theWitRun, "PROD2", 2, "COMP2",
              WitNusageRate,      4.0,
              WitNfallout,        2,
              NULL );
      checkWitRc( "witAddSubsBomEntryVa", rc );

      /* Add COMP4 as a substitute for COMP4 */
      rc = witAddSubsBomEntry(  theWitRun, "PROD2", 2, "COMP4" );
      checkWitRc( "witAddSubsBomEntry", rc );

      rc = witSetSubsBomEntryUsageRate(  theWitRun, "PROD2", 2, 3, 6.0 );
      checkWitRc( "witSetSubsBomEntryUsageRate", rc );

      rc = witSetSubsBomEntryFallout(  theWitRun, "PROD2", 2, 3, 4 );
      checkWitRc( "witSetSubsBomEntryFallout", rc );

      rc = witAddDemandVa(  theWitRun, "PROD2", "P2DEM1",
              WitNdemandVol,      demand,
              WitNobj1ShipReward, ship,
              WitNobj1CumShipReward,cumShip,
              NULL );
      checkWitRc( "witAddDemandVa", rc );
  }



   /*
    * Display model data just entered.
    */
   rc = witDisplayData (  theWitRun, NULL );
   checkWitRc( "witDisplayData", rc );

   /*
    * Perform implosion
    */
#ifdef OPT_IMPLODE
   rc = witOptImplode( theWitRun );
   checkWitRc( "witOptImplode", rc );
#else
   rc = witHeurImplode( theWitRun );
   checkWitRc( "witOptImplode", rc );
#endif

   /*
    * Turn off the display of WIT informational messages.
    */
   witSetMesgTimesPrint( theWitRun, WitFALSE, WitINFORMATIONAL_MESSAGES, 0 );

   /*
    * Get the per period production quantity of each product
    * and subassembly.
    */
   {
      char *names[]= { 
                     "PROD1",
                     "PROD2",
                     "SUBASMBL1",
                     "SUBASMBL2",
                     "SUBASMBL3",
                     };
      float * prodq;
      int i;
                       
      /*
       * Loop once for each product and subassembly
       */
      for ( i=0; i<(sizeof(names)/sizeof(names[0])); i++ ) {

         int t, nBomEntries, j;
         
         /*
          * Get production quantity of i'th part
          */
         rc = witGetPartProdVol( theWitRun, names[i], &prodq );
         checkWitRc( "witGetPartProdVol", rc );
         printf( "** Part: %s\n", names[i] );

         /*
          * Display production quantity of i'th part
          */
         for ( t=0; t<nPeriods; t++ )
            printf( "**    period %d production is %f\n", t, prodq[t] );

         free( prodq );

         /*
          * Get number of BOM entries in i'th part
          */
         rc = witGetPartNBomEntries( theWitRun, names[i], &nBomEntries );
         checkWitRc( "witGetPartNBomEntries", rc );

         /* 
          * Loop once for each BOM entry
          */
         for ( j=0; j<nBomEntries; j++ ) {

            int nSubsBomEntries, k;
          
            /*
             * Get number of substitutes for j'th BOM entry of i'th part
             */
            rc = witGetBomEntryNSubsBomEntries( theWitRun, 
                    names[i], j, &nSubsBomEntries );
            checkWitRc( "witGetBomEntryNSubsBomEntries", rc );

            /*
             * Loop once for each substitute
             */
            for ( k=0; k<nSubsBomEntries; k++ ) {

               float * prodq;
               int t;
  
               /*
                * Get production quantity due to k'th substitute
                */
               rc = witGetSubsBomEntryProdVol( theWitRun, 
                       names[i], j, k, &prodq );
               checkWitRc( "witGetSubsBomEntryProdVol", rc );
               printf( "**    Production from substitute %d of BOM entry %d\n",
                       k, j );

               /*
                * Display production quantity of i'th part due to use
                * of k'th substitute of j'th BOM entry
                */
               for ( t=0; t<nPeriods; t++ ) 
                  printf( "**       %f in period %d\n", prodq[t], t);

               free( prodq );

           } /* for ( k=0; k<nSubsBomEntries; k++ ) */

         } /* for ( j=0; j<nBomEntries; j++ ) */

      } /* for ( i=0; i<(sizeof(names)/sizeof(names[0])); i++ ) */

   } 

  rc = witDeleteRun( theWitRun );
  checkWitRc( "witDeleteRun", rc );

  exit (0);

} /* main */


/*
 * Check WIT Function Return Codes.
 */
void checkWitRc(char * funcName, int rc)
{
   if ( rc >= WitSEVERE_RC ) {
      printf("Severe problem when calling %s.\n",funcName);
      exit(1);
   }
   else if ( rc >= WitWARNING_RC ) {
      printf("Warning message condition occurred when calling %s.\n",funcName);
   }
}
