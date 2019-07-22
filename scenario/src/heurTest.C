// Test witHeurImplode with hard upper bounds on prodBounds.


#include <iostream.h>
#include <stdlib.h>
#include <scenario/src/testScen.h>
#include <scenario/src/timeVecs.h>


int
main ()
{
  int i;

  LgFrInitialProblemFromFile
      initProb( "../../../../wit/wit-data/brenda/denise/wit.data" );
  LgFrTestScenario testScenario(initProb);
  LgFrWorkingImplosionProblem &wip ( testScenario.workingImplosionProblem() );
  LgFrLastProblemSolved       &lps ( testScenario.lastProblemSolved() );
  LgFrSetOfParts              &sop ( testScenario.setOfParts() );
  LgFrSetOfDemands            &sod ( testScenario.setOfDemands() );
  LgFrImplode                 implode( lps );

  // Setup production schedule
  LgFrSortingPartScheduleFloat mutableProdVol;
  const LgFrSortingPartScheduleFloat & prodVol = mutableProdVol;
  LgFrProductionScheduleStrategy prodSS(testScenario);
  mutableProdVol.attributeScheduleStrategy(prodSS);

  // Schedule to save hard upper bounds production schedule
  LgFrSortingPartScheduleFloat mutableHubPB;
  const LgFrSortingPartScheduleFloat & hubPB = mutableHubPB;
 
  lps.computeCriticalList( TRUE );

  lps.heurImplode();

  // Set the production bounds to production volume
  for (i=0; i<sop.entries(); i=i+1 ) {
 
     if ( lps.category( &sop[i] ) == LgFrPRODUCT ) {
        LgFrTimeVecFloat newBounds( lps.numberOfPeriods() );
        newBounds=prodVol[ sop[i] ].timeVecFloat();
        mutableHubPB.insertKeyAndValue( sop[i], newBounds );
        lps.productionBounds( &sop[i], 
            NULL, NULL, &hubPB[ sop[i] ].timeVecFloat() );
     }

  }


  // Loop, testing to ensure production bounds are not violated and
  // then set new production bounds. 
  for ( int testNum=0; testNum<1000; testNum++ ) {

     lps.heurImplode();

     // Test that bound not violated
     for (i=0; i<hubPB.entries(); i++ ) {
         assert( 
                prodVol[hubPB[i].part()].timeVecFloat() <= 
                hubPB[i].timeVecFloat() 
               );
     }

     // Check for exit criteria of all upper prod bounds being 0.
     RWBoolean allZero = TRUE;
     for (i=0; i<hubPB.entries(); i++ ) {
        if ( !( hubPB[i].timeVecFloat() == 0.0) ) {
           allZero=FALSE;
           break;
        }
     }
     if ( allZero ) break;

     // Set new upper production bound for some part
     for ( ;; ) {
 
        // Pick a random part
        i = rand() % sop.entries();

        // Check to see if the i'th part is a product
        if ( lps.category( &sop[i] ) == LgFrPRODUCT ) 

           // Check to see if prodBounds are already zero
           if ( !(hubPB[ sop[i] ].timeVecFloat() == 0.0) ) {

              // Set a new upper production bound i'th part
              LgFrTimeVecFloat newBounds( lps.numberOfPeriods() );
              newBounds=prodVol[ sop[i] ].timeVecFloat()  *  .75;
              mutableHubPB.insertKeyAndValue( sop[i], newBounds );
              lps.productionBounds( &sop[i], 
                  NULL, NULL, &hubPB[ sop[i] ].timeVecFloat() );

              cout <<"Implosion " <<testNum <<endl;
              cout <<"  new UpperProdBounds: " 
                   <<hubPB[ sop[i] ].format() <<endl;
              break;

           }

     }

   }
}
