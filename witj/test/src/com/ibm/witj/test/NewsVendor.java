
//==============================================================================
// WIT-J/Test Source File NewsVendor.java.
//==============================================================================

package       com.ibm.witj.test;

import        com.ibm.witj.*;
import static com.ibm.witj.Attribute.*;
import static com.ibm.witj.Part.Category.*;

//==============================================================================
// Class NewsVendor
//
// Responsible for testing the use of stochastic implosion accessed through
// WIT-J:
// Formulates a simple news vendor problem, solves it, and displays the
// solution.
//==============================================================================

final class NewsVendor
{
//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// performTest ().
//
// Formulates a simple news vendor problem, solves it, and displays the
// solution.
//------------------------------------------------------------------------------

   static void performTest ()
      {
      NewsVendor theNewsVendor;

      theNewsVendor = new NewsVendor ();

      theNewsVendor.performTestInt ();
      }

//==============================================================================
// Private Constructor
//==============================================================================

   private NewsVendor ()
      {
      }

//==============================================================================
// Private Methods
//==============================================================================

//------------------------------------------------------------------------------
// performTestInt ().
//
// Internal implementation of performTest ().
//------------------------------------------------------------------------------

   private void performTestInt ()
      {
      System.out.printf (
         "\n"
       + "-------------------------------------------------\n"
       + " News Vendor Test\n"
       + "-------------------------------------------------\n");

      setUpProblem           ();
      buildCoreProblem       ();

      enterStochData         (false);
      myProblem.stochImplode ();
      displaySoln            ();

      enterStochData         (true);
      myProblem.stochImplode ();
      displaySoln            ();

      myProblem.deactivate   ();
      }

//------------------------------------------------------------------------------
// setUpProblem ()
//
// Creates and sets up myProblem.
//------------------------------------------------------------------------------

   private void setUpProblem ()
      {
      myProblem = Problem.newInstance ();

      myProblem.getMessageMgr ().setMesgFileAccessMode ("a");
      myProblem.getMessageMgr ().setMesgFileName       ("UnitTester.output");
      }

//------------------------------------------------------------------------------
// buildCoreProblem ()
//
// Populates myProblem with the core deterministic portion of the stochastic
// implosion formulation.
//------------------------------------------------------------------------------

   private void buildCoreProblem ()
      {
      myProblem.set (N_PERIODS, 2);

      myProblem.setOptInitMethod (OptInitMethod.CRASH);

      source = Part     .newInstance (myProblem, "Source", CAPACITY);
      buy    = Operation.newInstance (myProblem, "Buy");
               BomEntry .newInstance (buy,        source);
      hold   = Part     .newInstance (myProblem, "Hold",   MATERIAL);
               BopEntry .newInstance (buy,        hold);
      sell   = Demand   .newInstance (hold,      "Sell");

      source.set               (SUPPLY_VOL,  new double[] {500, 0});
      buy   .setVectorToScalar (EXEC_COST,   0.6);
      sell  .setVectorToScalar (SHIP_REWARD, 1.0);
      }

//------------------------------------------------------------------------------
// enterStochData (byObject)
//
// Populates myProblem with the stochastic portion of the stochastic implosion
// formulation.
// Uses stage-by-object, iff myObject is true.
//------------------------------------------------------------------------------

   private void enterStochData (boolean byObject)
      {
      myProblem.set (N_SCENARIOS,     3);
      myProblem.set (STAGE_BY_OBJECT, byObject);
      myProblem.set (STOCH_MODE,      true);

      if (byObject)
         {
         source.set (OBJECT_STAGE, 0);
         buy   .set (OBJECT_STAGE, 0);
         hold  .set (OBJECT_STAGE, 1);
         }
      else
         myProblem.set (PERIOD_STAGE, new int[] {0, 1});

      enterScenarioData (0, 0.25, 200);
      enterScenarioData (1, 0.50, 300);
      enterScenarioData (2, 0.25, 400);
      }


//------------------------------------------------------------------------------
// enterScenarioData (theScenario, theProbability, demandVolPer1)
//
// Sets the PROBABILITY and DEMAND_VOL in period 1 for theScenario.
//------------------------------------------------------------------------------

   private void enterScenarioData (
         int    theScenario,
         double theProbability,
         double demandVolPer1)
      {
      double[] theDemandVol;

      theDemandVol    = new double[2];
      theDemandVol[0] = 0;
      theDemandVol[1] = demandVolPer1;

      myProblem.set (CURRENT_SCENARIO, theScenario);
      myProblem.set (PROBABILITY,      theProbability);
      sell     .set (DEMAND_VOL,       theDemandVol);
      }

//------------------------------------------------------------------------------
// displaySoln ()
//
// Extracts the solution to the newsvendor problem from WIT-J and displays it.
//------------------------------------------------------------------------------

   private void displaySoln ()
      {
      boolean  byObject;
      double[] execVolArr;
      int      theScen;
      double[] demandVolArr;
      double[] shipVolArr;
      double   profit;

      byObject = myProblem.get (STAGE_BY_OBJECT);

      System.out.printf (
         "\n"
       + "Stage-By-%s Model\n"
       + "\n",
         byObject? "Object": "Period");

      execVolArr = buy.get (EXEC_VOL);

      System.out.printf (
         "Buy %.0f papers.\n"
       + "\n",
         execVolArr[0]);

      for (theScen = 0; theScen < 3; theScen ++)
         {
         myProblem.set (CURRENT_SCENARIO, theScen);

         demandVolArr = sell.get (DEMAND_VOL);
         shipVolArr   = sell.get (SHIP_VOL);

         System.out.printf (
            "If the demand is for %.0f papers, sell %.0f papers.\n",
            demandVolArr[1],
            shipVolArr  [1]);
         }

      profit = myProblem.get (OBJ_VALUE);

      System.out.printf (
         "\n"
       + "The expected profit is $%.2f.\n"
       + "\n"
       + "-------------------------------------------------\n",
         profit);

      myProblem.clearStochSoln ();

      myProblem .set (STOCH_MODE, false);
      }

//==============================================================================
// Private Fields
//==============================================================================

   Problem   myProblem = null;
   Part      source    = null;
   Operation buy       = null;
   Part      hold      = null;
   Demand    sell      = null;

//------------------------------------------------------------------------------
}
