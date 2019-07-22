
//==============================================================================
// Example WIT-J Application Program WitJDemo1.
//
// The program builds a trivial implosion problem, solves it with heuristic
// implosion and displays the shipment schedule and the supplyVol PIP.
//==============================================================================

import        com.ibm.witj.*;
import static com.ibm.witj.Attribute.*;
import static com.ibm.witj.Part.Category.*;

import                java.util.*;

public final class WitJDemo1
{
//------------------------------------------------------------------------------
// Main Program
//------------------------------------------------------------------------------

   public static void main (String[] theArgs)
      {
      WitJDemo1 theDemo;

      theDemo = new WitJDemo1 ();

      theDemo.performDemo ();
      }

//------------------------------------------------------------------------------
// myProblem
//
// The WIT-J Problem for this WIT-J application.
//------------------------------------------------------------------------------

   private Problem myProblem = null;

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------

   private WitJDemo1 ()
      {
      }

//------------------------------------------------------------------------------
// performDemo ()
//------------------------------------------------------------------------------

   private void performDemo ()
      {
      try
         {
         buildProblem ();

         myProblem.heurImplode ();
         myProblem.buildPip    ();

         displaySolution ();

         myProblem.deactivate ();

         myProblem = null;
         }

      catch (WitjException theWitjException)
         {
         handleWitjException (theWitjException);
         }
      }

//------------------------------------------------------------------------------
// buildProblem ()
//
// Creates and populates myProblem.
//------------------------------------------------------------------------------

   private void buildProblem ()
      {
      Part      thePartA;
      Operation theOpnB;
      Part      thePartC;
      Demand    theDemandD;
      Demand    theDemandE;

      myProblem = Problem.newInstance ();

      myProblem.getMessageMgr ().setMesgFileName ("witj-demo1.wmsg");

      myProblem.set (N_PERIODS,         2);
      myProblem.set (PIP_SEQ_FROM_HEUR, true);

      thePartA   = Part     .newInstance (myProblem, "A",      CAPACITY);
      theOpnB    = Operation.newInstance (myProblem, "B");
                   BomEntry .newInstance (theOpnB,   thePartA);
      thePartC   = Part     .newInstance (myProblem, "C",      MATERIAL);
                   BopEntry .newInstance (theOpnB,   thePartC);
      theDemandD = Demand   .newInstance (thePartC,  "D");
      theDemandE = Demand   .newInstance (thePartC,  "E");

      thePartC  .set (BUILD_NSTN, true);

      thePartA  .set (SUPPLY_VOL, new double[]{30,  0});
      thePartC  .set (SUPPLY_VOL, new double[]{30, 10});

      theDemandD.set (DEMAND_VOL, new double[]{20, 20});
      theDemandE.set (DEMAND_VOL, new double[]{20, 20});

      theDemandD.setVectorToScalar (PRIORITY, 1);
      theDemandE.setVectorToScalar (PRIORITY, 2);
      }

//------------------------------------------------------------------------------
// displaySolution ()
//
// Displays the shipment schedule and the supplyVol pegging from PIP.
//------------------------------------------------------------------------------

   private void displaySolution ()
      {
      double[]                         shipVolArr;
      int                              thePer;
      ArrayList <PeggingTriple <Part>> supplyVolPip;

      for (Demand theDemand: myProblem.getDemands ())
         {
         shipVolArr = theDemand.get (SHIP_VOL);

         for (thePer = 0; thePer < 2; thePer ++)
            {
            if (shipVolArr[thePer] < .000001)
               continue;

            System.out.printf (
               "%n"
             + "Part %s, Demand %s, Period %d:          shipVol = %2.0f%n",
               theDemand.getPart ().get (PART_NAME),
               theDemand           .get (DEMAND_NAME),
               thePer,
               shipVolArr[thePer]);

            supplyVolPip = theDemand.getSupplyVolPip (thePer);
            
            for (PeggingTriple <Part> theTriple: supplyVolPip)
               {
               System.out.printf (
                  "Part %s,           Period %d: pegged supplyVol = %2.0f%n",
                  theTriple.getRoot   ().get (PART_NAME),
                  theTriple.getPeriod (),
                  theTriple.getVolume ());
               }
            }
         }
      }

//------------------------------------------------------------------------------
// handleWitjException (theWitjException)
//
// Handles any exception thrown from WIT-J.
//------------------------------------------------------------------------------

   private void handleWitjException (WitjException theWitjException)
      {
      theWitjException.printStackTrace ();

      if (! TerminalException.witjHasTerminated ())
         {
         if (myProblem != null)
            {
            if (myProblem.isActive ())
               {
               myProblem.deactivate ();
               }

            myProblem = null;
            }
         }

      System.out.printf (
         "%n"
       + "WitJDemo1 is terminating due to the above exception.%n");

      System.exit (3);
      }
}
