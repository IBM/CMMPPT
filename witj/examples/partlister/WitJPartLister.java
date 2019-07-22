
//==============================================================================
// Example WIT-J Application Program PartLister.
//
// The program consists of a single class, WitJPartLister, which invokes WIT-J.
//
// The program reads in the WIT data file "diner.data", then prints out the
// following information for each Part in the Problem:
//
// * PART_NAME
// * SUPPLY_VOL
// * the OPERATION_NAME of the Operation associated with each BomEntry
// * associated with the part
//==============================================================================

import        com.ibm.witj.*;
import static com.ibm.witj.Attribute.*;

//==============================================================================
// Class WitJPartLister
//==============================================================================

public final class WitJPartLister
{
//==============================================================================
// Public Methods
//==============================================================================

//------------------------------------------------------------------------------
// Main Program.
//------------------------------------------------------------------------------

   public static void main (String[] theArgs)
      {
      WitJPartLister theWitLister;

      theWitLister = new WitJPartLister ();

      theWitLister.listParts ();
      }

//==============================================================================
// Private Constructor
//==============================================================================

   private WitJPartLister ()
      {
      }

//==============================================================================
// Private Methods
//==============================================================================

//------------------------------------------------------------------------------
// listParts ()
//
// Reads the file and lists the Parts.
//------------------------------------------------------------------------------

   private void listParts ()
      {
      Problem  theProblem;
      int      nPeriodsVal;
      double[] supplyVolArr;

      theProblem = Problem.newInstance ();

      theProblem.
         getMessageMgr ().
            setMesgTimesPrint (MessageGroup.INFORMATIONAL, 0);
      
      theProblem.readData ("diner.data");

      nPeriodsVal  = theProblem.get (N_PERIODS);

      supplyVolArr = new double[nPeriodsVal];

      System.out.printf (
         "%n"                         +
         "Parts in \"diner.data\":%n" +
         "%n");

      for (Part thePart: theProblem.getParts ())
         {
         System.out.printf (
            "%s:%n"         +
            "   SupplyVol:",
            thePart.get (PART_NAME));

         thePart.getVector (SUPPLY_VOL, supplyVolArr);

         for (double theDbl: supplyVolArr)
            System.out.printf (
               " %.0f",
               theDbl);

         System.out.printf (
            "%n"           +
            "   Used by:%n");

         for (BomEntry theBomEnt: thePart.getBomEntries ())
            System.out.printf (
               "      %s%n",
               theBomEnt.getOperation ().get (OPERATION_NAME));

         System.out.printf ("%n");
         }

      theProblem.deactivate ();
      }

//==============================================================================

}
