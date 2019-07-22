//------------------------------------------------------------------------------
// Program: "newsVendor.C".
//
// A C++ WIT application program to illustrate stochastic implosion.
// This program uses stochastic implosion to solve a very simple instance of
// the news vendor problem.
// Stage-By-Period version.
//------------------------------------------------------------------------------

#include <wit.h>
#include <iostream>

//------------------------------------------------------------------------------
// Function declarations.
//------------------------------------------------------------------------------

void buildCoreProblem (WitRun * & theWitRun);
   //
   // Builds the core problem in theWitRun.

void enterStochData (WitRun * theWitRun);
   //
   // Enters the stochastic data into theWitRun.

void enterScenarioData (
      WitRun * theWitRun,
      int      theScen,
      float    probability,
      float    demandVolPer1);
   //
   // Enters the data for one scenario into theWitRun.
   // theScen       is the index       of the scenario.
   // probability   is the probability of the scenario.
   // demandVolPer1 is the period 1 demandVol for demand "Sell" in the scenario.

void displaySoln (WitRun * theWitRun);
   //
   // Displays the solution to the news vendor problem from theWitRun.

//------------------------------------------------------------------------------
// Main Program
//------------------------------------------------------------------------------

int main ()
   {
   WitRun * theWitRun;

   buildCoreProblem (theWitRun);
   enterStochData   (theWitRun);
   witStochImplode  (theWitRun);
   displaySoln      (theWitRun);
   witDeleteRun     (theWitRun);

   exit (0);
   }

//------------------------------------------------------------------------------

void buildCoreProblem (WitRun * & theWitRun)
   {
   float supplyVol [] = {500.0, 0.0};
   float execCost  [] = {  0.6, 0.6};
   float shipReward[] = {  1.0, 1.0};

   witNewRun              (& theWitRun);

   witSetMesgFileAccessMode (theWitRun, WitTRUE, "w");
   witSetMesgFileName       (theWitRun, WitTRUE, "newsVendor.log");

   witInitialize            (theWitRun);
   witSetNPeriods           (theWitRun, 2);

   witAddPart               (theWitRun, "Source",           WitCAPACITY);
   witAddOperation          (theWitRun, "Buy");
   witAddBomEntry           (theWitRun, "Buy",    "Source");
   witAddPart               (theWitRun, "Hold",             WitMATERIAL);
   witAddBopEntry           (theWitRun, "Buy",    "Hold");
   witAddDemand             (theWitRun, "Hold",   "Sell");

   witSetPartSupplyVol      (theWitRun, "Source",           supplyVol);
   witSetOperationExecCost  (theWitRun, "Buy",              execCost);
   witSetDemandShipReward   (theWitRun, "Hold",   "Sell",   shipReward);
   }

//------------------------------------------------------------------------------

void enterStochData (WitRun * theWitRun)
   {
   int periodStage[] = {0, 1};

   witSetNScenarios    (theWitRun, 3);
   witSetStageByObject (theWitRun, WitFALSE);

   witSetStochMode     (theWitRun, WitTRUE);
   
   witSetPeriodStage   (theWitRun, periodStage);

   enterScenarioData   (theWitRun, 0, 0.25, 200.0);
   enterScenarioData   (theWitRun, 1, 0.50, 300.0);
   enterScenarioData   (theWitRun, 2, 0.25, 400.0);
   }

//------------------------------------------------------------------------------

void enterScenarioData (
      WitRun * theWitRun,
      int      theScen,
      float    probability,
      float    demandVolPer1)
   {
   float demandVol[2];

   demandVol[0] = 0.0;
   demandVol[1] = demandVolPer1;

   witSetCurrentScenario (theWitRun,                 theScen);
   witSetProbability     (theWitRun,                 probability);
   witSetDemandDemandVol (theWitRun, "Hold", "Sell", demandVol);
   }

//------------------------------------------------------------------------------

void displaySoln (WitRun * theWitRun)
   {
   int     theScen;
   float * execVol;
   float * demandVol;
   float * shipVol;
   float   objValue;
   float   boundsValue;

   witGetOperationExecVol (theWitRun, "Buy",  & execVol);

   std::cout
      << "Buy "
      << execVol[0]
      << " papers."
      << std::endl
      << std::endl;
   
   witFree (execVol);

   for (theScen = 0; theScen < 3; theScen ++)
      {
      witSetCurrentScenario (theWitRun,                   theScen);
      
      witGetDemandDemandVol (theWitRun, "Hold", "Sell", & demandVol);
      witGetDemandShipVol   (theWitRun, "Hold", "Sell", & shipVol);

      std::cout
         << "If the demand is for "
         << demandVol[1]
         << " papers, sell "
         << shipVol[1]
         << " papers."
         << std::endl;

      witFree (demandVol);
      witFree (shipVol);
      }

   witGetObjValues (theWitRun, & objValue, & boundsValue);

   std::cout
      << std::endl
      << "The expected profit is $"
      << objValue
      << "."
      << std::endl;
   }
