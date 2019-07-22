/****************************************************************************** 
 *
 * Create wit data file for Bob's 2 period build ahead example
 *
 ******************************************************************************/
 
#include <stdlib.h>
#include <wit.h>

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
void main (int argc, char * argv[])
{

  WitRun * theWitRun;

   /*
    * Initialize WIT
    */
   witNewRun( &theWitRun );
   witInitialize ( theWitRun );

   /*
    * Set the title, number of periods, and objective function choice,
    * number of periods per year.
    */
   witSetTitle( theWitRun, "Simple 2 Period Example with Build Ahead" );
   witSetNPeriods( theWitRun, 2 );

   /*
    * Part 1
    */
   {
      int   dP[] = { 2,   2 };
      float dV[] = { 0.,  10. };

      witAddPart( theWitRun, "Part_1", WitPRODUCT );

      witAddDemand( theWitRun, "Part_1", "demand" );
      witSetDemandPriority ( theWitRun, "Part_1", "demand", dP);
      witSetDemandDemandVol( theWitRun, "Part_1", "demand", dV);
      witSetDemandBuildAheadLimit( theWitRun, "Part_1", "demand", 1);
   }

   /*
    * Part 2
    */
   {
      int   dP[] = { 1,   1 };
      float dV[] = { 10., 0. };

      witAddPart( theWitRun, "Part_2", WitPRODUCT );

      witAddDemand( theWitRun, "Part_2", "demand" );
      witSetDemandPriority ( theWitRun, "Part_2", "demand", dP);
      witSetDemandDemandVol( theWitRun, "Part_2", "demand", dV);
      witSetDemandBuildAheadLimit( theWitRun, "Part_2", "demand", 1);
   }

   /*
    * Part 3
    */
   {
      witAddPart( theWitRun, "Part_3", WitPRODUCT );
      witAddBomEntry( theWitRun, "Part_1", "Part_3" );
   }

   /*
    * Capacity 1
    */
   {
      float cS[] = { 5., 5. };

      witAddPart( theWitRun, "Capacity_1", WitCAPACITY );
      witSetPartSupplyVol( theWitRun, "Capacity_1", cS );

      witAddBomEntry( theWitRun, "Part_3", "Capacity_1" );
   }


   /*
    * Capacity 2
    */
   {
      float cS[] = { 10., 10. };

      witAddPart( theWitRun, "Capacity_2", WitCAPACITY );
      witSetPartSupplyVol( theWitRun, "Capacity_2", cS );

      witAddBomEntry( theWitRun, "Part_1", "Capacity_2" );
      witAddBomEntry( theWitRun, "Part_2", "Capacity_2" );
   }

   witSetMesgFileAccessMode( theWitRun, WitFALSE, "w" );
   witWriteData( theWitRun, "bldAhdEx.wit.data" );

   witDeleteRun( theWitRun );

   exit (0);

} /* main */
