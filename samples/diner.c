/**************************************************************************** 
 *
 * Sample WIT API Program.
 * Runs implosion on the Diner problem.
 *
 ****************************************************************************/
 
#include <stdlib.h>
#include <wit.h>

/****************************************************************************/
/* Main Program                                                             */
/****************************************************************************/
void main (int argc, char * argv[])
{

  WitRun * theWitRun;

   /* Initialize WIT */
   witNewRun( &theWitRun );
   witInitialize ( theWitRun );

   /*
    * Set the title, number of periods, and objective function choice,
    * number of periods per year.
    */
   witSetTitle( theWitRun, "Diner" );
   witSetNPeriods( theWitRun, 2 );
   witSetExecEmptyBom( theWitRun, WitFALSE );
   witSetObjChoice( theWitRun, 1 );
   witSetPeriodsPerYear( theWitRun, 26.0 );

   /* Skillet */
   {
      float supply[]           = { 15., 20. };

      witAddPart( theWitRun, "Skillet", WitCAPACITY );
      witSetPartSupplyVol( theWitRun, "Skillet", supply );
   }

   /* Toaster */
   {
      float supply[]           = { 40., 30. };

      witAddPart( theWitRun, "Toaster", WitCAPACITY );
      witSetPartSupplyVol( theWitRun, "Toaster", supply );
   }

   /* Egg    */
   {
      float supply[]           = {  5.,  2. };
      float obj1Scrap[]        = { .01, .01 };

      witAddPart( theWitRun, "Egg", WitMATERIAL );
      witSetPartSupplyVol( theWitRun, "Egg", supply );
      witSetPartObj1ScrapCost( theWitRun, "Egg", obj1Scrap );
   }

   /* Butter    */
   {
      float obj1Scrap[]        = { .01, .01 };

      witAddPart( theWitRun, "Butter", WitMATERIAL );
      witSetPartObj1ScrapCost( theWitRun, "Butter", obj1Scrap );
   }

   /* Cheese    */
   {
      float supply[]           = { 30., 20. };
      float obj1Scrap[]        = { .01, .01 };

      witAddPart( theWitRun, "Cheese", WitMATERIAL);
      witSetPartSupplyVol( theWitRun, "Cheese", supply );
      witSetPartObj1ScrapCost( theWitRun, "Cheese", obj1Scrap );
   }

   /* Ham    */
   {
      float supply[]           = { 25., 15. };
      float obj1Scrap[]        = { .01, .01 };

      witAddPart( theWitRun, "Ham", WitMATERIAL);
      witSetPartSupplyVol( theWitRun, "Ham", supply );
      witSetPartObj1ScrapCost( theWitRun, "Ham", obj1Scrap );
   }

   /* Mushrooms    */
   {
      float obj1Scrap[]        = { .01, .01 };

      witAddPart( theWitRun, "Mushrooms", WitMATERIAL );
      witSetPartObj1ScrapCost( theWitRun, "Mushrooms", obj1Scrap );
   }

   /* Peppers    */
   {
      float obj1Scrap[]        = { .01, .01 };

      witAddPart( theWitRun, "Peppers", WitMATERIAL );
      witSetPartObj1ScrapCost( theWitRun, "Peppers", obj1Scrap );
   }

   /* Bread    */
   {
      float supply[]           = { 20., 30. };
      float obj1Scrap[]        = { .01, .01 };

      witAddPart( theWitRun, "Bread", WitMATERIAL );
      witSetPartSupplyVol( theWitRun, "Bread", supply );
      witSetPartObj1ScrapCost( theWitRun, "Bread", obj1Scrap );
   }

   /* Add Part Plain_Omlet    */

   witAddPart( theWitRun, "Plain_Omlet", WitMATERIAL );
   
   /* Plain Sandwich */

   {
     float supply[]           = {  2.,  0. };

     witAddPart( theWitRun, "Plain_Sandwich", WitMATERIAL );
     witSetPartSupplyVol( theWitRun, "Plain_Sandwich", supply );
   }

   /* Cheese Omlet */
   
   {
     float supply[]           = {  20., 20. };

     witAddPart( theWitRun, "Cheese_Omlet", WitMATERIAL );
     witSetPartSupplyVol( theWitRun, "Cheese_Omlet", supply );
   }

   /* Add Part Ham Omlet */

   witAddPart( theWitRun, "Ham_Omlet", WitMATERIAL );

   /* Add Part Ham and Cheese Omlet */

   witAddPart( theWitRun, "Ham_and_Cheese_Omlet", WitMATERIAL );

   /* Add Part Vegetable Omlet */

   witAddPart( theWitRun, "Vegtable_Omlet", WitMATERIAL );
   
   /* Grilled Cheese Sandwich */
   
   {
     float supply[]           = {  1., 0. };

     witAddPart( theWitRun, "Grilled_Cheese_Sandwich", WitMATERIAL );
     witSetPartSupplyVol( theWitRun, "Grilled_Cheese_Sandwich", supply );
   }

   /* Add Part Ham_Sandwich */

   witAddPart( theWitRun, "Ham_Sandwich", WitMATERIAL );

   /* Add Part Ham_and_Cheese_Sandwich */

   witAddPart( theWitRun, "Ham_and_Cheese_Sandwich", WitMATERIAL );

   /* Add Part Ham_and_Egg_Sandwich */

   witAddPart( theWitRun, "Ham_and_Egg_Sandwich", WitMATERIAL );



   /* Add Operation Plain_Omlet */

   witAddOperation( theWitRun, "Plain_Omlet" );

   /* Add Operation Plain_Sandwich */

   witAddOperation( theWitRun, "Plain_Sandwich" );
   
   /* Add Operation Cheese_Omlet */

   witAddOperation( theWitRun, "Cheese_Omlet" );
   
   /* Add Operation Ham_Omlet */

   witAddOperation( theWitRun, "Ham_Omlet" );

   /* Add Operation Ham_and_Cheese_Omlet */

   witAddOperation( theWitRun, "Ham_and_Cheese_Omlet" );
   
   /* Add Operation Vegetable_Omlet */

   witAddOperation( theWitRun, "Vegtable_Omlet" );
   
   /* Add Operation Grilled_Cheese_Sandwich */

   witAddOperation( theWitRun, "Grilled_Cheese_Sandwich" );
   
   /* Add Operation Ham_Sandwich */

   witAddOperation( theWitRun, "Ham_Sandwich" );
   
   /* Add Operation Ham_and_Cheese_Sandwich */

   witAddOperation( theWitRun, "Ham_and_Cheese_Sandwich" );
   
   /* Add Operation Ham_and_Egg_Sandwich */

   witAddOperation( theWitRun, "Ham_and_Egg_Sandwich" );


   /* Add bomEntry Plain_Omlet */

   witAddBomEntry( theWitRun, "Plain_Omlet", "Skillet" );
   witSetBomEntryUsageRate( theWitRun, "Plain_Omlet", 0, 3.0 );

   witAddBomEntry( theWitRun, "Plain_Omlet", "Egg" );
   witSetBomEntryUsageRate( theWitRun, "Plain_Omlet", 1, 3.0 );

   witAddBomEntry( theWitRun, "Plain_Omlet", "Butter" );

   /* Add bomEntry Plain_Sandwich */

   witAddBomEntry( theWitRun, "Plain_Sandwich", "Bread" );
   witSetBomEntryUsageRate( theWitRun, "Plain_Sandwich", 0, 2.0 );

   /* Add bomEntry Cheese_Omlet */

   witAddBomEntry( theWitRun, "Cheese_Omlet", "Skillet" );
   witSetBomEntryUsageRate( theWitRun, "Cheese_Omlet", 0, 3.0 );

   witAddBomEntry( theWitRun, "Cheese_Omlet", "Plain_Omlet" );

   witAddBomEntry( theWitRun, "Cheese_Omlet", "Cheese" );
   witSetBomEntryUsageRate( theWitRun, "Cheese_Omlet", 2, 3.0 );

   /* Add bomEntry Ham_Omlet */

   witAddBomEntry( theWitRun, "Ham_Omlet", "Skillet" );
   witSetBomEntryUsageRate( theWitRun, "Ham_Omlet", 0, 2.0 );

   witAddBomEntry( theWitRun, "Ham_Omlet", "Plain_Omlet" );

   witAddBomEntry( theWitRun, "Ham_Omlet", "Ham" );
   witSetBomEntryUsageRate( theWitRun, "Ham_Omlet", 2, 3.0 );

   /* Add bomEntry Ham_and_Cheese_Omlet */

   witAddBomEntry( theWitRun, "Ham_and_Cheese_Omlet", "Skillet" );
   witSetBomEntryUsageRate( theWitRun, "Ham_and_Cheese_Omlet", 0, 4.0 );

   witAddBomEntry( theWitRun, "Ham_and_Cheese_Omlet", "Plain_Omlet" );

   witAddBomEntry( theWitRun, "Ham_and_Cheese_Omlet", "Ham" );
   witSetBomEntryUsageRate( theWitRun, "Ham_and_Cheese_Omlet", 2, 2.0 );

   witAddBomEntry( theWitRun, "Ham_and_Cheese_Omlet", "Cheese" );
   witSetBomEntryUsageRate( theWitRun, "Ham_and_Cheese_Omlet", 3, 2.0 );

   /* Add bomEntry Vegtable_Omlet */

   witAddBomEntry( theWitRun, "Vegtable_Omlet", "Skillet" );

   witAddBomEntry( theWitRun, "Vegtable_Omlet", "Plain_Omlet" );

   witAddBomEntry( theWitRun, "Vegtable_Omlet", "Peppers" );
   witSetBomEntryUsageRate( theWitRun, "Ham_and_Cheese_Omlet", 2, 2.0 );

   witAddBomEntry( theWitRun, "Vegtable_Omlet", "Mushrooms" );
   witSetBomEntryUsageRate( theWitRun, "Vegtable_Omlet", 3, 2.0 );

   /* Add bomEntry Grilled_Cheese_Sandwich */

   witAddBomEntry( theWitRun, "Grilled_Cheese_Sandwich", "Skillet" );
   witSetBomEntryUsageRate( theWitRun, "Grilled_Cheese_Sandwich", 0, 3.0 );

   witAddBomEntry( theWitRun, "Grilled_Cheese_Sandwich", "Plain_Sandwich" );

   witAddBomEntry( theWitRun, "Grilled_Cheese_Sandwich", "Cheese" );
   witSetBomEntryUsageRate( theWitRun, "Grilled_Cheese_Sandwich", 2, 3.0 );

   /* Add bomEntry Ham_Sandwich */

   witAddBomEntry( theWitRun, "Ham_Sandwich", "Toaster" );
   witSetBomEntryUsageRate( theWitRun, "Ham_Sandwich", 0, 2.0 );

   witAddBomEntry( theWitRun, "Ham_Sandwich", "Plain_Sandwich" );

   witAddBomEntry( theWitRun, "Ham_Sandwich", "Ham" );
   witSetBomEntryUsageRate( theWitRun, "Ham_Sandwich", 2, 3.0 );

   /* Add bomEntry Ham_and_Cheese_Sandwich */

   witAddBomEntry( theWitRun, "Ham_and_Cheese_Sandwich", "Toaster" );
   witSetBomEntryUsageRate( theWitRun, "Ham_and_Cheese_Sandwich", 0, 2.0 );

   witAddBomEntry( theWitRun, "Ham_and_Cheese_Sandwich", "Plain_Sandwich" );

   witAddBomEntry( theWitRun, "Ham_and_Cheese_Sandwich", "Ham" );
   witSetBomEntryUsageRate( theWitRun, "Ham_and_Cheese_Sandwich", 2, 2.0 );

   witAddBomEntry( theWitRun, "Ham_and_Cheese_Sandwich", "Cheese" );
   witSetBomEntryUsageRate( theWitRun, "Ham_and_Cheese_Sandwich", 3, 2.0 );
   

   /* Add bomEntry Ham_and_Egg_Sandwich */

   witAddBomEntry( theWitRun, "Ham_and_Egg_Sandwich", "Toaster" );
   witSetBomEntryUsageRate( theWitRun, "Ham_and_Egg_Sandwich", 0, 2.0 );

   witAddBomEntry( theWitRun, "Ham_and_Egg_Sandwich", "Skillet" );
   witSetBomEntryUsageRate( theWitRun, "Ham_and_Egg_Sandwich", 1, 5.0 );

   witAddBomEntry( theWitRun, "Ham_and_Egg_Sandwich", "Plain_Sandwich" );

   witAddBomEntry( theWitRun, "Ham_and_Egg_Sandwich", "Ham" );

   witAddBomEntry( theWitRun, "Ham_and_Egg_Sandwich", "Egg" );


   /* Add bopEntries */

   witAddBopEntry( theWitRun, "Plain_Omlet", "Plain_Omlet" );
   witAddBopEntry( theWitRun, "Plain_Sandwich", "Plain_Sandwich" );
   witAddBopEntry( theWitRun, "Cheese_Omlet", "Cheese_Omlet" );
   witAddBopEntry( theWitRun, "Ham_Omlet", "Ham_Omlet" );
   witAddBopEntry( theWitRun, "Ham_and_Cheese_Omlet", "Ham_and_Cheese_Omlet");
   witAddBopEntry( theWitRun, "Vegtable_Omlet", "Vegtable_Omlet" );
   witAddBopEntry( theWitRun, "Grilled_Cheese_Sandwich",
                   "Grilled_Cheese_Sandwich" );
   witAddBopEntry( theWitRun, "Ham_Sandwich", "Ham_Sandwich" );
   witAddBopEntry( theWitRun, "Ham_and_Cheese_Sandwich",
                   "Ham_and_Cheese_Sandwich" );
   witAddBopEntry( theWitRun, "Ham_and_Egg_Sandwich",
                   "Ham_and_Egg_Sandwich" );

   
   /* Add Demand Plain_Omlet */

   {
     float iship[] = { 2.0, 2.0 };

     witAddDemand( theWitRun, "Plain_Omlet", "demand" );
     witSetDemandGrossRev( theWitRun, "Plain_Omlet", "demand", 2.0 );
     witSetDemandObj1ShipReward( theWitRun, "Plain_Omlet", "demand", iship );
   }

   /* Add Demand Plain_Sandwich */

   {
     float iship[] = { 1.0, 1.0 };
     float quantity[] = { 4.0, 2.0 };
     
     witAddDemand( theWitRun, "Plain_Sandwich", "demand" );
     witSetDemandDemandVol( theWitRun, "Plain_Sandwich", "demand", quantity );
     witSetDemandGrossRev( theWitRun, "Plain_Sandwich", "demand", 1.0 );
     witSetDemandObj1ShipReward( theWitRun, "Plain_Sandwich", "demand",
                                 iship );
   }
   
   /* Add Demand Cheese_Omlet */

   {
     float iship[] = { 3.0, 3.0 };
     
     witAddDemand( theWitRun, "Cheese_Omlet", "demand" );
     witSetDemandGrossRev( theWitRun, "Cheese_Omlet", "demand", 3.0 );
     witSetDemandObj1ShipReward( theWitRun, "Cheese_Omlet", "demand", iship );
   }
 
   /* Add Demand Ham_Omlet */

   {
     float iship[] = { 3.5, 3.5 };
     
     witAddDemand( theWitRun, "Ham_Omlet", "demand" );
     witSetDemandGrossRev( theWitRun, "Ham_Omlet", "demand", 3.5 );
     witSetDemandObj1ShipReward( theWitRun, "Ham_Omlet", "demand", iship );
   }
    
   /* Add Demand Ham_and_Cheese_Omlet */

   {
     float iship[] = { 4.0, 4.0 };
     
     witAddDemand( theWitRun, "Ham_and_Cheese_Omlet", "demand" );
     witSetDemandGrossRev( theWitRun, "Ham_and_Cheese_Omlet", "demand", 4.0 );
     witSetDemandObj1ShipReward( theWitRun, "Ham_Omlet", "demand", iship );
   }
   
   /* Add Demand Vegtable_Omlet */

   {
     float iship[] = { 2.75, 2.75 };
     
     witAddDemand( theWitRun, "Vegtable_Omlet", "demand" );
     witSetDemandGrossRev( theWitRun, "Vegtable_Omlet", "demand", 2.75 );
     witSetDemandObj1ShipReward( theWitRun, "Vegtable_Omlet", "demand",
                                 iship );
   }

   /* Add Demand Grilled_Cheese_Sandwich */

   {
     float iship[] = { 1.5, 1.5 };
     float quantity[] = { 5.0, 6.0 };
     
     witAddDemand( theWitRun, "Grilled_Cheese_Sandwich", "demand" );
     witSetDemandDemandVol( theWitRun, "Grilled_Cheese_Sandwich", "demand",
                            quantity );
     witSetDemandGrossRev( theWitRun, "Grilled_Cheese_Sandwich", "demand",
                           1.5 );
     witSetDemandObj1ShipReward( theWitRun, "Grilled_Cheese_Sandwich",
                                 "demand", iship );
   }

   /* Add Demand Ham_Sandwich */

   {
     float iship[] = { 2.5, 2.5 };
     float quantity[] = { 7.0, 8.0 };
     
     witAddDemand( theWitRun, "Ham_Sandwich", "demand" );
     witSetDemandDemandVol( theWitRun, "Ham_Sandwich", "demand", quantity );
     witSetDemandGrossRev( theWitRun, "Ham_Sandwich", "demand", 2.5 );
     witSetDemandObj1ShipReward( theWitRun, "Ham_Sandwich", "demand", iship );
   }

   /* Add Demand Ham_and_Cheese_Sandwich */

   {
     float iship[] = { 3.0, 3.0 };
     float quantity[] = { 6.0, 9.0 };
     
     witAddDemand( theWitRun, "Ham_and_Cheese_Sandwich", "demand" );
     witSetDemandDemandVol( theWitRun, "Ham_and_Cheese_Sandwich", "demand",
                            quantity );
     witSetDemandGrossRev( theWitRun, "Ham_and_Cheese_Sandwich", "demand",
                           3.0 );
     witSetDemandObj1ShipReward( theWitRun, "Ham_and_Cheese_Sandwich",
                                 "demand", iship );
   }

   /* Add Demand Ham_and_Egg_Sandwich */

   {
     float iship[] = { 3.5, 3.5 };
     float quantity[] = { 3.0, 4.0 };
     
     witAddDemand( theWitRun, "Ham_and_Egg_Sandwich", "demand" );
     witSetDemandDemandVol( theWitRun, "Ham_and_Egg_Sandwich", "demand",
                            quantity );
     witSetDemandGrossRev( theWitRun, "Ham_and_Egg_Sandwich", "demand", 3.5 );
     witSetDemandObj1ShipReward( theWitRun, "Ham_and_Egg_Sandwich",
                                 "demand", iship );
   }


   /*************************************************************************
    *
    * Finished entering data
    *
    ************************************************************************/

   witOptImplode( theWitRun );
   witWriteExecSched( theWitRun, WitSTDOUT, WitBSV );
   witWriteShipSched( theWitRun, WitSTDOUT, WitBSV );


   witDeleteRun( theWitRun );

   exit (0);

} /* main */

