
/******************************************************************************
 *
 * Sample WIT API Program
 *
 * This program is an example of using operation nodes.
 *
 * It models a yield where the production of PartA has a yield
 * of 50% completed the first period, 30% completed in 2nd period, and
 * the remaining 20% are completed in the 3rd period.
 *
 *****************************************************************************/

#include <stdlib.h>
#include <wit.h>

/*
 * Write demand attribute
 */
void writeDemandAttributeValue(
   WitRun * const theWitRun,
   const char * const demandedPartName,
   const char * const demandName,
   witReturnCode (*witGetDemandAttribFunc)(
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      float ** attributeValue ),
   char * title
)
{
  int nPeriods,i;
  float * attributeValue;

  witGetNPeriods( theWitRun, &nPeriods );
  witGetDemandAttribFunc( theWitRun, demandedPartName, demandName, &attributeValue );

  printf("%s %s %s : ",demandedPartName, demandName, title);
  for ( i=0; i<nPeriods; i++ )
    {
    printf("%10f ",attributeValue[i] );
    }
  printf("\n");

  free( attributeValue );

}

/*
 * Write Part attribute
 */
void writePartAttributeValue(
   WitRun * const theWitRun,
   const char * const partName,
   witReturnCode (*witGetPartAttribFunc)(
      WitRun * const theWitRun,
      const char * const partName,
      float ** attributeValue ),
   char * title
)
{
  int nPeriods,i;
  float * attributeValue;

  witGetNPeriods( theWitRun, &nPeriods );
  witGetPartAttribFunc( theWitRun, partName, &attributeValue );

  printf("%s %s : ",partName,title);
  for ( i=0; i<nPeriods; i++ )
    {
    printf("%10f ",attributeValue[i] );
    }
  printf("\n");

  free( attributeValue );

}

/*
 * Write Operation attribute
 */
void writeOperationAttributeValue(
   WitRun * const theWitRun,
   const char * const operationName,
   witReturnCode (*witGetOperationAttribFunc)(
      WitRun * const theWitRun,
      const char * const operationName,
      float ** attributeValue ),
   char * title
)
{
  int nPeriods,i;
  float * attributeValue;

  witGetNPeriods( theWitRun, &nPeriods );
  witGetOperationAttribFunc( theWitRun, operationName, &attributeValue );

  printf("%s %s : ",operationName,title);
  for ( i=0; i<nPeriods; i++ )
    {
    printf("%10f ",attributeValue[i] );
    }
  printf("\n");

  free( attributeValue );

}


/*
 * Main Program
 */
void main (int argc, char * argv[])
{

   WitRun * theWitRun;
   int nPeriods = 5;                     /* Number of periods in model   */

   /*
    * Establish environment for WIT to run.
    */
   witNewRun(&theWitRun);

   /*
    * Initialize WIT
    */
   witInitialize( theWitRun );

   /*
    * Set up wit global attributes.
    */
   witSetWit34Compatible( theWitRun, WitFALSE );
   witSetNPeriods( theWitRun, nPeriods );
   witSetObjChoice( theWitRun, 1 );

   /*
    * Add the parts and operations
    */
   witAddPart( theWitRun, "PartA", WitMATERIAL );
   {
     float stockCost[] = {   .01,   .01, .01, .01, .01 };
     witSetPartObj1StockCost( theWitRun, "PartA", stockCost );
   }
   
   witAddOperation( theWitRun, "OperationA" );

   witAddPart( theWitRun, "Component", WitMATERIAL );
   {
     float supplyVol[] = { 100., 100., 0., 0., 0. };
     witSetPartSupplyVol( theWitRun, "Component", supplyVol );
   }

   /*
    * Add demands
    */
   witAddDemand( theWitRun, "PartA", "Demand1" );
   {
     float demandVol[] = { 100., 0., 0., 0., 0. };
     float cumShipReward[] = { 1., 1., 1., 1., 1. };
     witSetDemandDemandVol( theWitRun, "PartA", "Demand1", demandVol );
   }

   /*
    * Add BOMs (bill-of-manufacturing)
    */
   witAddBomEntry( theWitRun, "OperationA", "Component" );


   /*
    * Add BOPs (bill-of-products)
    */
   witAddBopEntry( theWitRun, "OperationA", "PartA" );
   {
     witSetBopEntryProdRate( theWitRun, "OperationA", 0, .5     );
   }
   witAddBopEntry( theWitRun, "OperationA", "PartA" );
   {
     float offset[] = {-1.,-1.,-1.,-1.,-1. };
     witSetBopEntryOffset  ( theWitRun, "OperationA", 1, offset );
     witSetBopEntryProdRate( theWitRun, "OperationA", 1, .3     );
   }
   witAddBopEntry( theWitRun, "OperationA", "PartA" );
   {
     float offset[] = {-2.,-2.,-2.,-2.,-2. };
     witSetBopEntryOffset  ( theWitRun, "OperationA", 2, offset );
     witSetBopEntryProdRate( theWitRun, "OperationA", 2, .2     );
   }


   /*
    * Implode
    */
   witOptImplode( theWitRun );

   /*
    * Explode
    */
   witMrp( theWitRun );

   /*
    * Turn WIT Messages Off
    */
   witSetMesgTimesPrint( theWitRun, WitTRUE, WitINFORMATIONAL_MESSAGES, 0 );

   /*
    * Write Results
    */
   writeDemandAttributeValue( theWitRun,
                               "PartA",
                               "Demand1",
                               witGetDemandDemandVol,
                               "DemandVol" );
   writeDemandAttributeValue( theWitRun,
                               "PartA",
                               "Demand1",
                               witGetDemandShipVol,
                               "ShipVol  " );

   writeOperationAttributeValue( theWitRun,
                                  "OperationA",
                                  witGetOperationExecVol,
                                  "execVol" );

   writePartAttributeValue( theWitRun,
                             "PartA",
                             witGetPartProdVol,
                             "ProdVol " );
   writePartAttributeValue( theWitRun,
                             "PartA",
                             witGetPartConsVol,
                             "ConsVol " );
   writePartAttributeValue( theWitRun,
                             "PartA",
                             witGetPartStockVol,
                             "StockVol" );

   writePartAttributeValue( theWitRun,
                             "Component",
                             witGetPartSupplyVol,
                             "SupplyVol" );
   writePartAttributeValue( theWitRun,
                             "Component",
                             witGetPartConsVol,
                             "ConsVol  " );
   writePartAttributeValue( theWitRun,
                            "Component",
                            witGetPartStockVol,
                            "StockVol  " );
   writePartAttributeValue( theWitRun,
                            "Component",
                            witGetPartReqVol,
                            "ReqVol    " );
}

/****************************************************************************/

