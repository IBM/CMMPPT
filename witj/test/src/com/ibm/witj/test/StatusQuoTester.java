
//==============================================================================
// WIT-J/Test Source File StatusQuoTester.java.
//==============================================================================

package       com.ibm.witj.test;

import        com.ibm.witj.*;
import static com.ibm.witj.Attribute.*;
import static com.ibm.witj.Part.Category.*;
import static com.ibm.witj.MessageGroup.*;

import                java.util.*;

//==============================================================================
// Class StatusQuoTester
//
// Responsible for performing a series of tests on WIT-J each of which
// generates an error condition that that causes a StatusQuoException to be
// thrown. (or a java.lang exception)
//==============================================================================

final class StatusQuoTester
{
//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// performTest ().
//
// Performs a status quo test.
//------------------------------------------------------------------------------

   static void performTest ()
      {
      StatusQuoTester theStatusQuoTester;

      theStatusQuoTester = new StatusQuoTester ();

      theStatusQuoTester.perfTest ();
      }

//==============================================================================
// Private Constructor
//==============================================================================

   private StatusQuoTester ()
      {
      }

//==============================================================================
// Private Methods
//==============================================================================

//------------------------------------------------------------------------------
// perfTest ().
//
// Internal implementation of performTest ().
//------------------------------------------------------------------------------

   private void perfTest ()
      {
      testCase1  ();
      testCase2  ();
      testCase3  ();
      testCase4  ();
      testCase5  ();
      testCase6  ();
      testCase7  ();
      testCase8  ();
      testCase9  ();
      testCase10 ();
      testCase11 ();
      testCase12 ();
      testCase13 ();
      testCase14 ();
      testCase15 ();
      testCase16 ();
      testCase17 ();
      testCase18 ();
      testCase19 ();
      testCase20 ();
      testCase21 ();
      testCase22 ();
      testCase23 ();
      testCase24 ();
      testCase25 ();
      testCase26 ();

      System.out.println (
         "\n"
       + "Normal termination of status quo test.");
      }

//------------------------------------------------------------------------------
// testCase1 ().
//
// Performs status quo test case 1.
//------------------------------------------------------------------------------

   private void testCase1 ()
      {
      Problem theProblem;

      printHeading (
         1,
         "Call to an active method on an inactive PreservedObject.");

      theProblem = newProblem ();

      theProblem.deactivate ();

      try
         {
         theProblem.displayData ("error.txt");

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      printClosing (1);
      }

//------------------------------------------------------------------------------
// testCase2 ().
//
// Performs status quo test case 2.
//------------------------------------------------------------------------------

   private void testCase2 ()
      {
      printHeading (
         2,
         "Passing a null reference as the Class argument to "
       + "theAttribute.appliesTo.");

      try
         {
         N_PERIODS.appliesTo (null);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      printClosing (2);
      }

//------------------------------------------------------------------------------
// testCase3 ().
//
// Performs status quo test case 3.
//------------------------------------------------------------------------------

   private void testCase3 ()
      {
      printHeading (
         3,
         "Call to Attribute.appliesTo with Component as the Class.");

      try
         {
         EXEC_VOL.appliesTo (Component.class);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      printClosing (3);
      }

//------------------------------------------------------------------------------
// testCase4 ().
//
// Performs status quo test case 4.
//------------------------------------------------------------------------------

   private void testCase4 ()
      {
      Problem    theProblemA;
      Operation  theOpnA;
      MessageMgr theMessageMgrB;
      Problem    theProblemB;
      Part       thePartB;

      printHeading (
         4,
         "Constructing a BomEntry whose Operation "
       + "and Part belong to different Problems.");

      theProblemA    = newProblem                    ();
      theOpnA        = Operation  .newInstance       (theProblemA,   "A");

      theProblemB    = Problem    .newInstance       ();
      theMessageMgrB = theProblemB.getMessageMgr     ();
      theMessageMgrB              .setMesgTimesPrint (INFORMATIONAL, 0);
      thePartB       = Part       .newInstance       (theProblemB,   "B",
                                                                     MATERIAL);
      
      try
         {
         BomEntry                 .newInstance       (theOpnA, thePartB);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      theProblemB.deactivate ();
      theProblemA.deactivate ();

      printClosing (4);
      }

//------------------------------------------------------------------------------
// testCase5 ().
//
// Performs status quo test case 5.
//------------------------------------------------------------------------------

   private void testCase5 ()
      {
      Problem theProblem;

      printHeading (
         5,
         "Call to Problem.readData on a Problem that has a Part.");

      theProblem = newProblem ();

      Part.newInstance (theProblem, "A", MATERIAL);

      try
         {
         theProblem.readData ("test.data");

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      theProblem.deactivate ();

      printClosing (5);
      }

//------------------------------------------------------------------------------
// testCase6 ().
//
// Performs status quo test case 6.
//------------------------------------------------------------------------------

   private void testCase6 ()
      {
      Problem theProblem;

      printHeading (
         6,
         "Passing an inactive PreservedObject to a method\n"
       + "that requires an active PreservedObject.");

      theProblem = newProblem ();

      theProblem.deactivate ();

      try
         {
         Part.newInstance (theProblem, "A", MATERIAL);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      printClosing (6);
      }

//------------------------------------------------------------------------------
// testCase7 ().
//
// Performs status quo test case 7.
//------------------------------------------------------------------------------

   private void testCase7 ()
      {
      Problem theProblem;

      printHeading (
         7,
         "Call to Problem.performAction with an invalid command.");

      theProblem = newProblem ();

      try
         {
         theProblem.performAction ("Bogus Command");

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      theProblem.deactivate ();

      printClosing (7);
      }

//------------------------------------------------------------------------------
// testCase8 ().
//
// Performs status quo test case 8.
//------------------------------------------------------------------------------

   private void testCase8 ()
      {
      Problem          theProblem;
      List <Component> theCompList;

      printHeading (
         8,
         "Call to a modifying method on an unmodifiable List returned by a "
       + "WIT-J method.");

      theProblem  = newProblem ();

      theCompList = theProblem.getComponents ();

      try
         {
         theCompList.clear ();

         throw new AssertionError ();
         }

      catch (UnsupportedOperationException theExc)
         {
         theExc.printStackTrace ();
         }

      theProblem.deactivate ();

      printClosing (8);
      }

//------------------------------------------------------------------------------
// testCase9 ().
//
// Performs status quo test case 9.
//------------------------------------------------------------------------------

   private void testCase9 ()
      {
      Problem   theProblem;
      Part      thePart;
      Operation theOpn;

      printHeading (
         9,
         "Call to theOpn.getUniqueBomEntryTo (thePart) when theOpn and\n"
       + "thePart are connected by more than one BomEntry.");

      theProblem  = newProblem ();

      thePart     = Part     .newInstance (theProblem, "A", CAPACITY);
      theOpn      = Operation.newInstance (theProblem, "B");

      BomEntry.newInstance (theOpn, thePart);
      BomEntry.newInstance (theOpn, thePart);

      try
         {
         theOpn.getUniqueBomEntryTo (thePart);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      theProblem.deactivate ();

      printClosing (9);
      }

//------------------------------------------------------------------------------
// testCase10 ().
//
// Performs status quo test case 10.
//------------------------------------------------------------------------------

   private void testCase10 ()
      {
      Problem             theProblem;
      Part                thePart;
      Demand              theDemand;
      ArrayList <Demand>  theDemandList;
      ArrayList <Integer> shipPeriodList;
      ArrayList <Double>  desIncVolList;
      ArrayList <Double>  incVolList;

      printHeading (
         10,
         "Call to Problem.eqHeurAlloc "
       + "with a null reference in desIncVolList.");

      theProblem     = newProblem ();

      thePart        = Part  .newInstance (theProblem,  "A", CAPACITY);
      theDemand      = Demand.newInstance (thePart,     "B");

      theDemandList  = new ArrayList <Demand>  (3);
      shipPeriodList = new ArrayList <Integer> (3);
      desIncVolList  = new ArrayList <Double>  (3);
      incVolList     = new ArrayList <Double>  (3);

      theDemandList .add (theDemand);
      shipPeriodList.add (2);
      desIncVolList .add (10.0);

      theDemandList .add (theDemand);
      shipPeriodList.add (8);
      desIncVolList .add (19.0);

      theDemandList .add (theDemand);
      shipPeriodList.add (5);
      desIncVolList .add (null);

      theProblem.startHeurAlloc ();

      try
         {
         theProblem.
            eqHeurAlloc (
               theDemandList,
               shipPeriodList,
               desIncVolList,
               incVolList);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      theProblem.deactivate ();

      printClosing (10);
      }

//------------------------------------------------------------------------------
// testCase11 ().
//
// Performs status quo test case 11.
//------------------------------------------------------------------------------

   private void testCase11 ()
      {
      Problem             theProblem;
      Part                thePart;
      Demand              theDemandB;
      Demand              theDemandC;
      ArrayList <Demand>  theDemandList;
      ArrayList <Integer> shipPeriodList;
      ArrayList <Double>  desIncVolList;
      ArrayList <Double>  incVolList;

      printHeading (
         11,
         "Call to Problem.eqHeurAlloc "
       + "with an inactive Demand in the Demand List.");

      theProblem     = newProblem ();

      thePart        = Part  .newInstance (theProblem,  "A", CAPACITY);
      theDemandB     = Demand.newInstance (thePart,     "B");
      theDemandC     = Demand.newInstance (thePart,     "C");

      theDemandList  = new ArrayList <Demand>  (3);
      shipPeriodList = new ArrayList <Integer> (3);
      desIncVolList  = new ArrayList <Double>  (3);
      incVolList     = new ArrayList <Double>  (3);

      theDemandList .add (theDemandB);
      shipPeriodList.add (2);
      desIncVolList .add (10.0);

      theDemandList .add (theDemandB);
      shipPeriodList.add (8);
      desIncVolList .add (19.0);

      theDemandList .add (theDemandC);
      shipPeriodList.add (5);
      desIncVolList .add (12.0);

      theDemandC.set (SEL_FOR_DEL, true);

      theProblem.purgeData ();

      theProblem.startHeurAlloc ();

      try
         {
         theProblem.
            eqHeurAlloc (
               theDemandList,
               shipPeriodList,
               desIncVolList,
               incVolList);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      theProblem.deactivate ();

      printClosing (11);
      }

//------------------------------------------------------------------------------
// testCase12 ().
//
// Performs status quo test case 12.
//------------------------------------------------------------------------------

   private void testCase12 ()
      {
      Problem             theProblem1;
      Part                thePartA;
      Demand              theDemandB;
      Problem             theProblem2;
      Part                thePartC;
      Demand              theDemandD;
      ArrayList <Demand>  theDemandList;
      ArrayList <Integer> shipPeriodList;
      ArrayList <Double>  desIncVolList;
      ArrayList <Double>  incVolList;

      printHeading (
         12,
         "Call to Problem.eqHeurAlloc with a Demand in the Demand List\n"
       + "that does not belong to the Problem.");

      theProblem1    = newProblem ();

      thePartA       = Part  .newInstance (theProblem1, "A", CAPACITY);
      theDemandB     = Demand.newInstance (thePartA,    "B");

      theProblem2    = newProblem ();

      thePartC       = Part  .newInstance (theProblem2, "C", CAPACITY);
      theDemandD     = Demand.newInstance (thePartC,    "D");

      theDemandList  = new ArrayList <Demand>  (3);
      shipPeriodList = new ArrayList <Integer> (3);
      desIncVolList  = new ArrayList <Double>  (3);
      incVolList     = new ArrayList <Double>  (3);

      theDemandList .add (theDemandB);
      shipPeriodList.add (2);
      desIncVolList .add (10.0);

      theDemandList .add (theDemandB);
      shipPeriodList.add (8);
      desIncVolList .add (19.0);

      theDemandList .add (theDemandD);
      shipPeriodList.add (5);
      desIncVolList .add (12.0);

      theProblem1.startHeurAlloc ();

      try
         {
         theProblem1.
            eqHeurAlloc (
               theDemandList,
               shipPeriodList,
               desIncVolList,
               incVolList);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      theProblem2.deactivate ();
      theProblem1.deactivate ();

      printClosing (12);
      }

//------------------------------------------------------------------------------
// testCase13 ().
//
// Performs status quo test case 13.
//------------------------------------------------------------------------------

   private void testCase13 ()
      {
      Problem             theProblem;
      Part                thePart;
      Demand              theDemand;
      ArrayList <Demand>  theDemandList;
      ArrayList <Integer> shipPeriodList;
      ArrayList <Double>  desIncVolList;
      ArrayList <Double>  incVolList;

      printHeading (
         13,
         "Call to Problem.eqHeurAlloc with ArrayLists of non-matching size.");

      theProblem     = newProblem ();

      thePart        = Part  .newInstance (theProblem,  "A", CAPACITY);
      theDemand      = Demand.newInstance (thePart,     "B");

      theDemandList  = new ArrayList <Demand>  (3);
      shipPeriodList = new ArrayList <Integer> (3);
      desIncVolList  = new ArrayList <Double>  (3);
      incVolList     = new ArrayList <Double>  (3);

      theDemandList .add (theDemand);
      shipPeriodList.add (2);
      desIncVolList .add (10.0);

      theDemandList .add (theDemand);
      shipPeriodList.add (8);
      desIncVolList .add (19.0);

      theDemandList .add (theDemand);
      shipPeriodList.add (5);

      theProblem.startHeurAlloc ();

      try
         {
         theProblem.
            eqHeurAlloc (
               theDemandList,
               shipPeriodList,
               desIncVolList,
               incVolList);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      theProblem.deactivate ();

      printClosing (13);
      }

//------------------------------------------------------------------------------
// testCase14 ().
//
// Performs status quo test case 14.
//------------------------------------------------------------------------------

   private void testCase14 ()
      {
      Problem theProblem1;
      Problem theProblem2;

      printHeading (
         14,
         "Call to Component.copyComponentData for an instance of class "
       + "Problem.");

      theProblem1 = newProblem ();
      theProblem2 = newProblem ();

      try
         {
         theProblem2.copyComponentData (theProblem1);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      theProblem1.deactivate ();
      theProblem2.deactivate ();

      printClosing (14);
      }

//------------------------------------------------------------------------------
// testCase15 ().
//
// Performs status quo test case 15.
//------------------------------------------------------------------------------

   private void testCase15 ()
      {
      Problem   theProblem;
      Part      thePart;
      Operation theOpn;

      printHeading (
         15,
         "Call to Component.copyComponentData with Components of different "
       + "classes.");

      theProblem = newProblem ();

      thePart    = Part     .newInstance (theProblem, "A", MATERIAL);
      theOpn     = Operation.newInstance (theProblem, "B");

      try
         {
         thePart.copyComponentData (theOpn);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      theProblem.deactivate ();

      printClosing (15);
      }

//------------------------------------------------------------------------------
// testCase16 ().
//
// Performs status quo test case 16.
//------------------------------------------------------------------------------

   private void testCase16 ()
      {
      Problem theProblem;
      Part    thePartA;

      printHeading (
         16,
         "Call to Component.get for an Component class\n"
       + "to which the Attribute does not apply.");

      theProblem = newProblem ();

      thePartA   = Part.newInstance (theProblem, "A", MATERIAL);

      try
         {
         thePartA.get (EARLIEST_PERIOD);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      theProblem.deactivate ();

      printClosing (16);
      }

//------------------------------------------------------------------------------
// testCase17 ().
//
// Performs status quo test case 17.
//------------------------------------------------------------------------------

   private void testCase17 ()
      {
      Problem  theProblem;
      Part     thePartA;
      double[] theDblArr;

      printHeading (
         17,
         "Call to Component.set (theAtt, theV) with an unmodifiable "
       + "Attribute.");

      theProblem = newProblem ();

      thePartA   = Part.newInstance (theProblem, "A", MATERIAL);

      theDblArr  = new double[26];

      try
         {
         thePartA.set (STOCK_VOL, theDblArr);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      theProblem.deactivate ();

      printClosing (17);
      }

//------------------------------------------------------------------------------
// testCase18 ().
//
// Performs status quo test case 18.
//------------------------------------------------------------------------------

   private void testCase18 ()
      {
      Problem  theProblem;
      Part     thePartA;
      double[] theDblArr;

      printHeading (
         18,
         "Call to Component.set (theAtt, theV) with an array of the wrong "
       + "length.");

      theProblem = newProblem ();

      thePartA   = Part.newInstance (theProblem, "A", MATERIAL);

      theDblArr  = new double[27];

      try
         {
         thePartA.set (STOCK_COST, theDblArr);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      theProblem.deactivate ();

      printClosing (18);
      }

//------------------------------------------------------------------------------
// testCase19 ().
//
// Performs status quo test case 19.
//------------------------------------------------------------------------------

   private void testCase19 ()
      {
      Problem theProblem;
      Part    thePartA;
      Boolean theBoolean;

      printHeading (
         19,
         "Call to Component.getVector (theAtt, theV) with an Attribute whose\n"
       + "value type is not an array type.");

      theProblem = newProblem ();

      thePartA   = Part.newInstance (theProblem, "A", MATERIAL);

      theBoolean = true;

      try
         {
         thePartA.getVector (BUILD_ASAP, theBoolean);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      theProblem.deactivate ();

      printClosing (19);
      }

//------------------------------------------------------------------------------
// testCase20 ().
//
// Performs status quo test case 20.
//------------------------------------------------------------------------------

   private void testCase20 ()
      {
      Problem   theProblem;
      Operation theOpnA;

      printHeading (
         20,
         "Call to Component.get (theAtt) with an Attribute whose value type\n"
       + "is BoundSet.");

      theProblem = newProblem ();

      theOpnA    = Operation.newInstance (theProblem, "A");

      try
         {
         theOpnA.get (EXEC_BOUNDS);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      theProblem.deactivate ();

      printClosing (20);
      }

//------------------------------------------------------------------------------
// testCase21 ().
//
// Performs status quo test case 21.
//------------------------------------------------------------------------------

   private void testCase21 ()
      {
      Problem   theProblem;
      Operation theOpnA;
      double[]  hardLB;
      double[]  hardUB;

      printHeading (
         21,
         "Call to Component.setBoundSet (...) with an array of the wrong "
       + "length.");

      theProblem = newProblem ();

      theOpnA    = Operation.newInstance (theProblem, "A");

      hardLB     = new double[26];
      hardUB     = new double[25];

      try
         {
         theOpnA.setBoundSet (EXEC_BOUNDS, hardLB, null, hardUB);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      theProblem.deactivate ();

      printClosing (21);
      }

//------------------------------------------------------------------------------
// testCase22 ().
//
// Performs status quo test case 22.
//------------------------------------------------------------------------------

   private void testCase22 ()
      {
      Problem theProblem;

      printHeading (
         22,
         "Call to Problem.set (N_PERIODS, ...) on a Problem that has an "
       + "Operation.");

      theProblem = newProblem ();

      Operation.newInstance (theProblem, "Test Opn 22");

      try
         {
         theProblem.set (N_PERIODS, 7);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      theProblem.deactivate ();

      printClosing (22);
      }

//------------------------------------------------------------------------------
// testCase23 ().
//
// Performs status quo test case 23.
//------------------------------------------------------------------------------

   private void testCase23 ()
      {
      Problem theProblem;

      printHeading (
         23,
         "Call to Attribute.getDefaultValue (...) on an Attribute that has no\n"
       + "default value.");

      theProblem = newProblem ();

      try
         {
         PART_NAME.getDefaultValue (theProblem);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      theProblem.deactivate ();

      printClosing (23);
      }

//------------------------------------------------------------------------------
// testCase24 ().
//
// Performs status quo test case 24.
//------------------------------------------------------------------------------

   private void testCase24 ()
      {
      Problem   theProblem;
      Operation theOpnA;

      printHeading (
         24,
         "Call to Attribute.getDefaultBoundSet on an Attribute whose value\n"
       + "type is not BoundSet.");

      theProblem = newProblem ();

      try
         {
         SUPPLY_VOL.getDefaultBoundSet (theProblem, null, null, null);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      theProblem.deactivate ();

      printClosing (24);
      }

//------------------------------------------------------------------------------
// testCase25 ().
//
// Performs status quo test case 25.
//------------------------------------------------------------------------------

   private void testCase25 ()
      {
      Problem  theProblem;
      double[] hardLB;
      double[] softLB;
      double[] hardUB;

      printHeading (
         25,
         "Call to Attribute.getDefaultBoundSet with an array of the wrong "
       + "length.");

      theProblem = newProblem ();

      hardLB     = new double[26];
      softLB     = new double[13];
      hardUB     = new double[25];

      try
         {
         STOCK_BOUNDS.getDefaultBoundSet (theProblem, hardLB, softLB, hardUB);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      theProblem.deactivate ();

      printClosing (25);
      }

//------------------------------------------------------------------------------
// testCase26 ().
//
// Performs status quo test case 26.
//------------------------------------------------------------------------------

   private void testCase26 ()
      {
      printHeading (
         26,
         "Call to Attribute.asAttOfType (Boolean.class) with an Attribute\n"
       + "whose value type is not Boolean.");

      try
         {
         getAttributes ().get (5).asAttOfType (Boolean.class);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      printClosing (26);
      }

//------------------------------------------------------------------------------
// newProblem ()
//
// Creates and returns a new Problem set up for status quo testing.
//------------------------------------------------------------------------------

   private Problem newProblem ()
      {
      Problem theProblem;

      theProblem = Problem.newInstance ();

      theProblem.getMessageMgr ().setMesgFileAccessMode ("a");

      theProblem.getMessageMgr ().setMesgFileName ("StatusQuoTester.output");

      theProblem.getMessageMgr ().setMesgTimesPrint (1060, 0);

      return theProblem;
      }

//------------------------------------------------------------------------------
// printHeading (theCaseIdx, text)
//
// Prints a heading for the status quo test case of theCaseIdx, where text
// describes status quo test case.
//------------------------------------------------------------------------------

   private void printHeading (int theCaseIdx, String text)
      {
      System.out.printf (
         "\n"
       + "----------------------------------------"
       + "----------------------------------------\n"
       + "WIT-J Status Quo Test Case %d:\n"
       + "%s\n"
       + "----------------------------------------"
       + "----------------------------------------\n"
       + "\n",
         theCaseIdx,
         text);
      }

//------------------------------------------------------------------------------
// printClosing (theCaseIdx)
//
// Prints a closing for the status quo test case of theCaseIdx.
//------------------------------------------------------------------------------

   private void printClosing (int theCaseIdx)
      {
      System.out.printf (
         "\n"
       + "Successful completion of Status Quo Test Case %d.\n",
         theCaseIdx);
      }

//==============================================================================
}
