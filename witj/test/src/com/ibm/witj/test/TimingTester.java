
//==============================================================================
// WIT-J/Test Source File TimingTester.java.
//==============================================================================

package       com.ibm.witj.test;

import        com.ibm.witj.*;
import static com.ibm.witj.Attribute.*;
import static com.ibm.witj.Part.Category.*;
import static com.ibm.witj.MessageGroup.*;

import       java.lang.management.*;

//==============================================================================
// Class TimingTester
//
// Reponsible for performing a timing test on WIT-J.
//==============================================================================

final class TimingTester
{
//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// performTest ().
//
// Performs a timing test on WIT-J.
//------------------------------------------------------------------------------

   static void performTest (String[] theArgs)
      {
      TimingTester theTimingTester;

      theTimingTester = new TimingTester ();

      theTimingTester.perfTest (theArgs);
      }

//==============================================================================
// Private Constructor
//==============================================================================

   private TimingTester ()
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

   private void perfTest (String[] theArgs)
      {
      int runIdx;

      System.out.printf (
         "\n"
       + "---------------------------------------------------\n"
       + " WIT-J Timing Test: Java WIT-J Application Program\n"
       + "---------------------------------------------------\n");

      parseArgs (theArgs);

      aveNonHeurTime = 0.0;
      aveTotalTime   = 0.0;

      for (runIdx = 0; runIdx < nRuns; runIdx ++)
         {
         do1Run ();
         }

      displayCpuTimes ();
      }

//------------------------------------------------------------------------------
// parseArgs (theArgs)
//
// Parses theArgs to set the parameter values.
//------------------------------------------------------------------------------

   private void parseArgs (String[] theArgs)
      {
      int    fillerLength;
      char[] fillerArr;
      int    theIdx;

      nPers        = Integer.parseInt (theArgs[1]);
      nChains      = Integer.parseInt (theArgs[2]);
      chainLength  = Integer.parseInt (theArgs[3]);
      supVol       = Integer.parseInt (theArgs[4]);
      fillerLength = Integer.parseInt (theArgs[5]);
      nRuns        = Integer.parseInt (theArgs[6]);

      System.out.printf (
         "\n"
       + "Parameters:\n"
       + "   # Periods:     %6d\n"
       + "   # Chains:      %6d\n"
       + "   Chain Length:  %6d\n"
       + "   SupplyVol:     %6d\n"
       + "   Filler Length: %6d\n"
       + "   # Runs:        %6d\n",
         nPers,
         nChains,
         chainLength,
         supVol,
         fillerLength,
         nRuns);

      fillerArr = new char[fillerLength];

      for (theIdx = 0; theIdx < fillerLength; theIdx ++)
         {
         fillerArr[theIdx] = '-';
         }

      filler = new String (fillerArr);
      }

//------------------------------------------------------------------------------
// do1Run ()
//
// Does one run of the timing test.
//------------------------------------------------------------------------------

   private void do1Run ()
      {
      double timeAtStart;
      double timeBefHeur;
      double timeAftHeur;
      double timeAtEnd;

      timeAtStart     = getCpuTime ();

      createInitProblem ();
      populateProblem   ();

      timeBefHeur     = getCpuTime ();

      aveNonHeurTime += (timeBefHeur - timeAtStart) / nRuns;

      myProblem.heurImplode ();

      timeAftHeur     = getCpuTime ();

      retrieveSoln ();

      myProblem.deactivate ();

      myProblem       = null;

      timeAtEnd       = getCpuTime ();

      aveNonHeurTime += (timeAtEnd - timeAftHeur) / nRuns;
      aveTotalTime   += (timeAtEnd - timeAtStart) / nRuns;
      }

//------------------------------------------------------------------------------
// createInitProblem ()
//
// Creates and sets up myProblem, unpopulated.
//------------------------------------------------------------------------------

   private void createInitProblem ()
      {
      myProblem = Problem.newInstance ();

      myProblem.getMessageMgr ().setMesgTimesPrint (INFORMATIONAL, 0);
//    myProblem.getMessageMgr ().setMesgTimesPrint (750,          -1);
//    myProblem.getMessageMgr ().setMesgTimesPrint (751,          -1);
//    myProblem.getMessageMgr ().setMesgTimesPrint (752,          -1);
      myProblem.getMessageMgr ().setMesgTimesPrint (338,           0);
      }

//------------------------------------------------------------------------------
// populateProblem ()
//
// Builds up the contents of myProblem.
//------------------------------------------------------------------------------

   private void populateProblem ()
      {
      double[]  supVolArr;
      double[]  demVolArr;
      int       thePer;
      int       chainIdx;
      String    theName;
      Part      thePart;
      int       vertIdx;
      Operation theOpn;
      Demand    theDemand;

      supVolArr = new double[nPers];
      demVolArr = new double[nPers];

      for (thePer = 0; thePer < nPers; thePer ++)
         {
         supVolArr[thePer] = supVol;
         }

      demVolArr[nPers - 1] = supVol * (nPers + 1);

      myProblem.set (N_PERIODS, nPers);

      for (chainIdx = 0; chainIdx < nChains; chainIdx ++)
         {
         theName = "Cap" + chainIdx + filler;

         thePart = Part.newInstance (myProblem, theName, CAPACITY);

         thePart.set (SUPPLY_VOL, supVolArr);

         for (vertIdx = 0; vertIdx < chainLength; vertIdx ++)
            {
            theName = "Opn" + chainIdx + "-" + vertIdx + filler;
            
            theOpn  = Operation.newInstance (myProblem, theName);

            BomEntry.newInstance (theOpn, thePart);

            theName = "Mat" + chainIdx + "-" + vertIdx + filler;
            
            thePart = Part.newInstance (myProblem, theName, MATERIAL);

            BopEntry.newInstance (theOpn, thePart);
            }

         thePart.set (BUILD_NSTN, true);

         theName   = "Dem" + chainIdx + filler;

         theDemand = Demand.newInstance (thePart, theName);

         theDemand.set (DEMAND_VOL, demVolArr);
         }
      }

//------------------------------------------------------------------------------
// retrieveSoln ()
//
// Retrieves (and discards) the solution.
//------------------------------------------------------------------------------

   private void retrieveSoln ()
      {
      for (Demand theDemand: myProblem.getDemands ())
         {
         theDemand.get (SHIP_VOL);
         }

      for (Operation theOpn: myProblem.getOperations ())
         {
         theOpn.get (EXEC_VOL);
         }
      }
//------------------------------------------------------------------------------
// getCpuTime ()
//
// Returns the current cumulative CPU time of the program process in seconds.
//------------------------------------------------------------------------------

   private static double getCpuTime ()
      {
      long   nanoTime;
      double secTime;

      nanoTime =
         ManagementFactory.
            getThreadMXBean ().
               getCurrentThreadCpuTime ();

      secTime  = ((double) nanoTime) / 1000000000.0;

      return secTime;
      }

//------------------------------------------------------------------------------
// displayCpuTimes ()
//------------------------------------------------------------------------------

   private void displayCpuTimes ()
      {
      System.out.printf (
         "\n"
       + "Ave. Non-Heur Time: %6.2f\n"
       + "Ave. Total    Time: %6.2f\n",
         aveNonHeurTime,
         aveTotalTime);
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// nPers
//
// # periods
//------------------------------------------------------------------------------

   private int nPers;

//------------------------------------------------------------------------------
// nChains
//
// # Chains
//------------------------------------------------------------------------------

   private int nChains;

//------------------------------------------------------------------------------
// chainLength
//
// # Operations in each chain.
//------------------------------------------------------------------------------

   private int chainLength;

//------------------------------------------------------------------------------
// supVol
//
// The SUPPLY_VOL value of each capacity in each period.
//------------------------------------------------------------------------------

   private int supVol;

//------------------------------------------------------------------------------
// filler
//
// A String to be included in names to make them longer.
//------------------------------------------------------------------------------

   private String filler;

//------------------------------------------------------------------------------
// nRuns
//
// # runs to be done.
//------------------------------------------------------------------------------

   private int nRuns;

//------------------------------------------------------------------------------
// aveNonHeurTime
//
// Average CPU time without heuristic implosion.
//------------------------------------------------------------------------------

   double aveNonHeurTime;

//------------------------------------------------------------------------------
// aveTotalTime
//
// Average total CPU time.
//------------------------------------------------------------------------------

   double aveTotalTime;

//------------------------------------------------------------------------------
// myProblem
//------------------------------------------------------------------------------

   private Problem myProblem;

//------------------------------------------------------------------------------
}
