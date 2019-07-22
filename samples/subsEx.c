/****************************************************************************** 
 *
 * Create wit data file for Bob's 1 period substitute example
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
   witSetTitle( theWitRun, "Simple 1 Period Example with Substitutes" );
   witSetNPeriods( theWitRun, 1 );

   /*
    * Product 1
    */
   {
      int   dP[] = { 2 };
      float dV[] = { 100. };

      witAddPart( theWitRun, "Product_1", WitPRODUCT );

      witAddDemand( theWitRun, "Product_1", "demand" );
      witSetDemandPriority ( theWitRun, "Product_1", "demand", dP);
      witSetDemandDemandVol( theWitRun, "Product_1", "demand", dV);
   }

   /*
    * Product 2
    */
   {
      int   dP[] = { 3 };
      float dV[] = { 100. };

      witAddPart( theWitRun, "Product_2", WitPRODUCT );

      witAddDemand( theWitRun, "Product_2", "demand" );
      witSetDemandPriority ( theWitRun, "Product_2", "demand", dP);
      witSetDemandDemandVol( theWitRun, "Product_2", "demand", dV);
   }

   /*
    * Product 3
    */
   {
      int   dP[] = { 1 };
      float dV[] = { 100. };

      witAddPart( theWitRun, "Product_3", WitPRODUCT );

      witAddDemand( theWitRun, "Product_3", "demand" );
      witSetDemandPriority ( theWitRun, "Product_3", "demand", dP);
      witSetDemandDemandVol( theWitRun, "Product_3", "demand", dV);
   }

   /*
    * Raw 1
    */
   {
      float rS[] = { 20. };

      witAddPart( theWitRun, "Raw_1", WitRAW );
      witSetPartSupplyVol( theWitRun, "Raw_1", rS );

      witAddBomEntry( theWitRun, "Product_1", "Raw_1" );
   }

   /*
    * Raw 2
    */
   {
      float rS[] = { 85. };

      witAddPart( theWitRun, "Raw_2", WitRAW );
      witSetPartSupplyVol( theWitRun, "Raw_2", rS );

      witAddBomEntry( theWitRun, "Product_2", "Raw_2" );
   }

   /*
    * Raw 3
    */
   {
      float rS[] = { 200. };

      witAddPart( theWitRun, "Raw_3", WitRAW );
      witSetPartSupplyVol( theWitRun, "Raw_3", rS );

      witAddBomEntry( theWitRun, "Product_3", "Raw_3" );
   }

   
   /*
    * Add substitute bom entries
    */
   {
      witAddSubsBomEntry( theWitRun, "Product_1", 0, "Raw_3" );
      witAddSubsBomEntry( theWitRun, "Product_2", 0, "Raw_1" );
      witAddSubsBomEntry( theWitRun, "Product_3", 0, "Raw_2" );
   }

   witSetMesgFileAccessMode( theWitRun, WitFALSE, "w" );
   witWriteData( theWitRun, "subsEx.wit.data" );

   witDeleteRun( theWitRun );

   exit (0);

} /* main */
