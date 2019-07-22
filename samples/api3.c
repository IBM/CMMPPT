/****************************************************************************** 
 *
 * Sample WIT API Program
 *
 ******************************************************************************/
 
#include <stdio.h>
#include <stdlib.h>

#include <wit.h>

int main (int argc, char * argv[])
{

  /* Setup the WitRun */
  WitRun * theWitRun;
  witNewRun( &theWitRun );
  witInitialize ( theWitRun );

  /* Set global attributes */
  witSetNPeriods    ( theWitRun, 4 );

  /* Create objects */

  /* Create part A, operation A, and connecting BOP */
  {
  float stockCost[] = { 50., 50., 50., 50. };
  float scrapCost[] = { 50., 50., 50., 50. };
  witAddPartWithOperation( theWitRun, "A" );
  witSetPartStockCost    ( theWitRun, "A", stockCost );
  witSetPartScrapCost    ( theWitRun, "A", scrapCost );
  }

  /* Create part B */
  {
  float stockCost[] = {  1.,   1.,  1.,  1. };
  float scrapCost[] = { 10.,  10., 10., 10. };
  float supplyVol    [] = {  0., 100.,  0., 50. };
  witAddPart         ( theWitRun, "B", WitMATERIAL );
  witSetPartSupplyVol( theWitRun, "B", supplyVol     );
  witSetPartStockCost( theWitRun, "B", stockCost );
  witSetPartScrapCost( theWitRun, "B", scrapCost );
  }

  /* Create capacity C */
  {
  float supplyVol    [] = {  30.,  30.,  30.,  30. };
  witAddPart         ( theWitRun, "C", WitCAPACITY );
  witSetPartSupplyVol( theWitRun, "C", supplyVol     );
  }

  /* Create part E */
  {
  float stockCost[] = {   1.,   1.,   1.,   1. };
  float scrapCost[] = {  10.,  10.,  10.,  10. };
  float supplyVol    [] = {  25.,  25.,  25.,  25. };
  witAddPart         ( theWitRun, "E", WitMATERIAL );
  witSetPartSupplyVol( theWitRun, "E", supplyVol     );
  witSetPartStockCost( theWitRun, "E", stockCost );
  witSetPartScrapCost( theWitRun, "E", scrapCost );
  }

  /* Create demand F on part A */
  witAddDemand( theWitRun, "A", "F" );

  /* Create operation D */
  witAddOperation( theWitRun, "D" );

  /* Create Bill-of-manufacturing entries */
  witAddBomEntry( theWitRun, "A", "C" );
  witAddBomEntry( theWitRun, "A", "B" );
  witAddBomEntry( theWitRun, "D", "E" );

  /* Create Substitute BOM Entry where part E may be used in place */
  /* of part B in the BOM entry representing the consumption of    */
  /* part B by operation A.                                        */
  witAddSubsBomEntry( theWitRun, "A", 1, "E" );

  /* Create Bill-of-process entries */
  witAddBopEntry( theWitRun, "D", "A" );

  /* Set object attributes */

  /* Set part A attributes */
  {
  float softLowerBound[] = { 10., 10., 10., 10. };
  float hardUpperBound[] = { 30., 30., 20., 20. };
  float supplyVol     [] = { 17.,  0.,  0.,  0. };
  witSetPartStockBounds( theWitRun, "A", 
                         NULL, softLowerBound, hardUpperBound );
  witSetPartSupplyVol  ( theWitRun, "A", supplyVol );
  }

  /* Set demand F on part A attributes */
  {
  float demandVol        [] = {   50.,   60.,   70.,   80. };
  float shipReward   [] = { 1000., 1000., 1000., 1000. };
  float cumShipReward[] = {   10.,   10.,   10.,   10. };
  witSetDemandDemandVol    ( theWitRun, "A", "F", demandVol );
  witSetDemandShipReward   ( theWitRun, "A", "F", shipReward );
  witSetDemandCumShipReward( theWitRun, "A", "F", cumShipReward );
  }

  /* Set operation A attributes */
  {
  int yield[] = {   95,   95,   95,   95 };
  witSetOperationYield( theWitRun, "A", yield );
  }

  /* Set BOM Entry attributes */
  {
  float offset[] = { 1., 1., 1., 1. };
  witSetBomEntryOffset( theWitRun, "A", 1, offset );
  }

  /* Set substitute BOM Entry attributes */
  witSetSubsBomEntryLatestPeriod( theWitRun, "A", 1, 0, 2 );

  /* Set BOP Entry attributes */
  witSetBopEntryProdRate( theWitRun, "A", 0, 2 );
   

  /* Perform Implosion and write production and shipment schedule */
  witOptImplode( theWitRun );
  witWriteExecSched( theWitRun, WitSTDOUT, WitBSV );
  witWriteShipSched( theWitRun, WitSTDOUT, WitBSV );

  /* Get and print a few attribute values for part B */
  {
  int     i, nPeriods;
  float * supplyVol;
  float * consVol;
  float * stockVol;
  float * excessVol;
  witGetNPeriods     ( theWitRun,      &nPeriods  );
  witGetPartSupplyVol( theWitRun, "B", &supplyVol );
  witGetPartConsVol  ( theWitRun, "B", &consVol   );
  witGetPartStockVol ( theWitRun, "B", &stockVol  );
  witGetPartExcessVol( theWitRun, "B", &excessVol );
  for( i=0; i<nPeriods; i++ ) 
     printf( "part B: supplyVol[%d]=%f, consVol[%d]  =%f\n"
             "        stockVol[%d] =%f, excessVol[%d]=%f\n",
             i, supplyVol[i], 
             i, consVol[i], 
             i, stockVol[i], 
             i, excessVol[i] );
  free( supplyVol );
  free( consVol   );
  free( stockVol  );
  free( excessVol );
  }

  witDeleteRun( theWitRun );

} /* main */
