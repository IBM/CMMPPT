
//==============================================================================
// WIT-J/Test Source File PurgeTester.java.
//==============================================================================

package       com.ibm.witj.test;

import        com.ibm.witj.*;
import static com.ibm.witj.Attribute.*;
import static com.ibm.witj.Part.Category.*;

import       java.util.*;

//==============================================================================
// Class PurgeTester
//
// Responsible for performing a test of the purgeData method of class Problem.
//==============================================================================

final class PurgeTester
{
//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// performTest ().
//
// Performs a test of theProblem.purgeData ().
//------------------------------------------------------------------------------

   static void performTest ()
      {
      PurgeTester thePurgeTester;

      thePurgeTester = new PurgeTester ();

      thePurgeTester.perfTest ();
      }

//==============================================================================
// Private Constructor
//==============================================================================

   private PurgeTester ()
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
      List <Component> prePurgeComponents;

      setUpProblem ();

      buildComponents ();

      setSelForDels ();

      prePurgeComponents =
         new ArrayList <Component> (myProblem.getComponents ());

      println (
         "\n"
       + "All Components before purgeData ():\n");

      for (Component theComp: prePurgeComponents)
         println (theComp);

      System.out.printf (
         "%n"
       + "Before Purge:%n"
       + "   # Parts Created:      %d%n"
       + "   # Demands Created:    %d%n"
       + "   # Operations Created: %d%n",
         myProblem.getNPartsCreated      (),
         myProblem.getNDemandsCreated    (),
         myProblem.getNOperationsCreated ());

      myProblem.purgeData ();

      println (
         "\n"
       + "All Components after purgeData ():\n");

      for (Component theComp: prePurgeComponents)
         println (theComp);

      println (
         "\n"
       + "Listed Components after purgeData ():\n");

      for (Component theComp: myProblem.getComponents ())
         println (theComp);

      System.out.printf (
         "%n"
       + "After Purge:%n"
       + "   # Parts Created:      %d%n"
       + "   # Demands Created:    %d%n"
       + "   # Operations Created: %d%n",
         myProblem.getNPartsCreated      (),
         myProblem.getNDemandsCreated    (),
         myProblem.getNOperationsCreated ());

      println ("");
      println ("Part C:      " + myProblem.getPart      ("C"));
      println ("Part D:      " + myProblem.getPart      ("D"));
      println ("Operation I: " + myProblem.getOperation ("I"));
      println ("Operation J: " + myProblem.getOperation ("J"));

      printPartComponents     ();
      printOpnComponents      ();
      printBomEntryComponents ();

      myProblem.deactivate ();
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
// buildComponents ()
//
// Builds the Components;
//------------------------------------------------------------------------------

   private void buildComponents ()
      {
      myPartA    = Part      .newInstance (myProblem,  "A", CAPACITY);
      myPartB    = Part      .newInstance (myProblem,  "B", CAPACITY);
      myPartC    = Part      .newInstance (myProblem,  "C", MATERIAL);
      myPartD    = Part      .newInstance (myProblem,  "D", MATERIAL);
      myPartE    = Part      .newInstance (myProblem,  "E", MATERIAL);

      myDemandCF = Demand    .newInstance (myPartC,    "F");
      myDemandEG = Demand    .newInstance (myPartE,    "G");
      myDemandEH = Demand    .newInstance (myPartE,    "H");

      myOpnI     = Operation .newInstance (myProblem,  "I");
      myOpnJ     = Operation .newInstance (myProblem,  "J");
      myOpnK     = Operation .newInstance (myProblem,  "K");

      myBomEntIA = BomEntry  .newInstance (myOpnI,     myPartA);
      myBomEntJB = BomEntry  .newInstance (myOpnJ,     myPartB);
      myBomEntKB = BomEntry  .newInstance (myOpnK,     myPartB);
      myBomEntKC = BomEntry  .newInstance (myOpnK,     myPartC);
      myBomEntKD = BomEntry  .newInstance (myOpnK,     myPartD);
   
      mySubIAA   = Substitute.newInstance (myBomEntIA, myPartA);
      mySubIAB   = Substitute.newInstance (myBomEntIA, myPartB);
      mySubJBA   = Substitute.newInstance (myBomEntJB, myPartA);
      mySubKCD   = Substitute.newInstance (myBomEntKC, myPartD);
      mySubKDC   = Substitute.newInstance (myBomEntKD, myPartC);

      myBopEntIE = BopEntry  .newInstance (myOpnI,     myPartE);
      myBopEntIC = BopEntry  .newInstance (myOpnI,     myPartC);
      myBopEntID = BopEntry  .newInstance (myOpnI,     myPartD);
      myBopEntJD = BopEntry  .newInstance (myOpnJ,     myPartD);
      myBopEntKE = BopEntry  .newInstance (myOpnK,     myPartE);
      }

//------------------------------------------------------------------------------
// setSelForDels ()
//
// Sets the SEL_FOR_DEL Attribute as appropriate.
//------------------------------------------------------------------------------

   private void setSelForDels ()
      {
      myOpnI    .set (SEL_FOR_DEL, true);

      myPartC   .set (SEL_FOR_DEL, true);
      myDemandEG.set (SEL_FOR_DEL, true);
      myOpnJ    .set (SEL_FOR_DEL, true);
      myBomEntKB.set (SEL_FOR_DEL, true);
      mySubIAA  .set (SEL_FOR_DEL, true);
      myBopEntIE.set (SEL_FOR_DEL, true);

      myOpnI    .set (SEL_FOR_DEL, false);
      }

//------------------------------------------------------------------------------
// printPartComponents ()
//
// Prints the Components associated with each Part.
//------------------------------------------------------------------------------

   private void printPartComponents ()
      {
      Part thePart;

      for (Component theComp: myProblem.getComponents ())
         {
         if (! (theComp instanceof Part))
            continue;

         thePart = (Part) theComp;

         println ("\n" + thePart + ":");

         printObjects ("Demands",     thePart.getDemands     ());
         printObjects ("BomEntries",  thePart.getBomEntries  ());
         printObjects ("Substitutes", thePart.getSubstitutes ());
         printObjects ("BopEntries",  thePart.getBopEntries  ());
         }
      }

//------------------------------------------------------------------------------
// printOpnComponents ()
//
// Prints the Components associated with each Operation.
//------------------------------------------------------------------------------

   private void printOpnComponents ()
      {
      Operation theOpn;

      for (Component theComp: myProblem.getComponents ())
         {
         if (! (theComp instanceof Operation))
            continue;

         theOpn = (Operation) theComp;

         println ("\n" + theOpn + ":");

         printObjects ("BomEntries",  theOpn.getBomEntries ());
         printObjects ("BopEntries",  theOpn.getBopEntries ());
         }
      }

//------------------------------------------------------------------------------
// printBomEntryComponents ()
//
// Prints the Components associated with each BomEntry.
//------------------------------------------------------------------------------

   private void printBomEntryComponents ()
      {
      BomEntry theBomEnt;

      for (Component theComp: myProblem.getComponents ())
         {
         if (! (theComp instanceof BomEntry))
            continue;

         theBomEnt = (BomEntry) theComp;

         println ("\n" + theBomEnt + ":");

         printObjects ("Substitutes", theBomEnt.getSubstitutes ());
         }
      }

//------------------------------------------------------------------------------
// printObjects (theClassName, theObjects)
//
// Prints the Objects in theObjects.
// Uses theClassName in the heading.
//------------------------------------------------------------------------------

   private void printObjects (String theClassName, List <?> theObjects)
      {
      println ("   " + theClassName + ":");

      for (Object theObject: theObjects)
         println ("      " + theObject);
      }

//------------------------------------------------------------------------------
// println (Object theObject)
//------------------------------------------------------------------------------

   private void println (Object theObject)
      {
      System.out.println (theObject);
      }

//==============================================================================
// Private Fields
//==============================================================================

   Problem    myProblem  = null;

   Part       myPartA    = null;
   Part       myPartB    = null;
   Part       myPartC    = null;
   Part       myPartD    = null;
   Part       myPartE    = null;

   Demand     myDemandCF = null;
   Demand     myDemandEG = null;
   Demand     myDemandEH = null;

   Operation  myOpnI     = null;
   Operation  myOpnJ     = null;
   Operation  myOpnK     = null;

   BomEntry   myBomEntIA = null;
   BomEntry   myBomEntJB = null;
   BomEntry   myBomEntKB = null;
   BomEntry   myBomEntKC = null;
   BomEntry   myBomEntKD = null;

   Substitute mySubIAA   = null;
   Substitute mySubIAB   = null;
   Substitute mySubJBA   = null;
   Substitute mySubKCD   = null;
   Substitute mySubKDC   = null;

   BopEntry   myBopEntIE = null;
   BopEntry   myBopEntIC = null;
   BopEntry   myBopEntID = null;
   BopEntry   myBopEntJD = null;
   BopEntry   myBopEntKE = null;

//------------------------------------------------------------------------------
}
