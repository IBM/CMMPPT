
//==============================================================================
// WIT-J/Test Source File ErrTester.java.
//==============================================================================

package       com.ibm.witj.test;

import        com.ibm.witj.*;
import static com.ibm.witj.Attribute.*;

//==============================================================================
// Class ErrTester
//
// Responsible for performing tests on WIT-J that involve terminal errors.
//==============================================================================

final class ErrTester
{
//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// performTest ().
//
// Performs an error test.
//------------------------------------------------------------------------------

   static void performTest (String[] theArgs)
      {
      ErrTester theErrTester;

      theErrTester = new ErrTester ();

      theErrTester.perfTest (theArgs);
      }

//==============================================================================
// Private Constructor
//==============================================================================

   private ErrTester ()
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
      int theCaseIdx;

      theCaseIdx = compCaseIndex (theArgs);

      switch (theCaseIdx)
         {
         case 1:
            testCase1 ();
            break;

         case 2:
            testCase2 ();
            break;

         case 3:
            testCase3 ();
            break;

         case 4:
            testCase4 ();
            break;

         case 5:
            testCase5 ();
            break;

         case 6:
            testCase6 ();
            break;

         case 7:
            testCase7 ();
            break;

         case 8:
            testCase8 ();
            break;

         case 9:
            testCase9 ();
            break;

         case 10:
            testCase10 ();
            break;

         case 11:
            testCase11 ();
            break;

         case 12:
            testCase12 ();
            break;

         case 13:
            testCase13 ();
            break;

         case 14:
            testCase14 ();
            break;

         case 15:
            testCase15 ();
            break;

         case 16:
            testCase16 ();
            break;

         case 17:
            testCase17 ();
            break;

         default:
            System.out.println (
                 "\n"
               + "ERROR:\n"
               + "   When error testing is requested, the second argument\n"
               + "   to the WIT-J tester must be an integer from 1 to 17.\n");

            System.exit (3);
         }
      }

//------------------------------------------------------------------------------
// compCaseIndex (String[] theArgs)
//
// Computes and returns the error case index from the command line arguments.
//------------------------------------------------------------------------------

   private int compCaseIndex (String[] theArgs)
      {
      if (theArgs.length < 2)
         {
         Tester.abortWmsg (
              "   The WIT-J Tester requires at least 2 command line arguments\n"
            + "   in the \"e\" case.");
         }

      try
         {
         return Integer.parseInt (theArgs[1]);
         }

      catch (NumberFormatException theExc)
         {
         System.out.println (
              "\n"
            + "ERROR:\n"
            + "   When error testing is requested, the second argument"
            + "   to the WIT-J tester must be an integer.\n");

         System.exit (3);

         return -1;
         }
      }

//------------------------------------------------------------------------------
// testCase1 ().
//
// Performs error test case 1.
//------------------------------------------------------------------------------

   private void testCase1 ()
      {
      Problem theProblem;

      printHeading (
         1,
         "Throws an instance of an unexpected Java Throwable class.");

      theProblem = newProblem ();

      assert (! TerminalException.witjHasTerminated ());

      try
         {
         theProblem.performAction ("throw-unexpected-java-throwable");

         throw new AssertionError ();
         }

      catch (InternalErrorException theExc)
         {
         theExc.printStackTrace ();
         }

      System.out.println ("");

      assert (TerminalException.witjHasTerminated ());

      try
         {
         Problem.newInstance ();

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }

      System.out.println (
           "\n"
         + "Successful completion of Error Test Case 1.");
      }

//------------------------------------------------------------------------------
// testCase2 ().
//
// Performs error test case 2.
//------------------------------------------------------------------------------

   private void testCase2 ()
      {
      Problem theProblem;

      printHeading (
         2,
         "Fails a Java assertion.");

      theProblem = newProblem ();

      try
         {
         theProblem.performAction ("fail-java-assert");

         throw new AssertionError ();
         }

      catch (InternalErrorException theExc)
         {
         theExc.printStackTrace ();
         }

      System.out.println (
           "\n"
         + "Successful completion of Error Test Case 2.");
      }

//------------------------------------------------------------------------------
// testCase3 ().
//
// Performs error test case 3.
//------------------------------------------------------------------------------

   private void testCase3 ()
      {
      Problem theProblem;

      printHeading (
         3,
         "Throws a C++ exception of an unexpected type.\n"
       + "The native method returns a value.");

      theProblem = newProblem ();

      try
         {
         theProblem.performAction ("throw-unexpected-c++-exception");

         throw new AssertionError ();
         }

      catch (InternalErrorException theExc)
         {
         theExc.printStackTrace ();
         }

      System.out.println (
           "\n"
         + "Successful completion of Error Test Case 3.");
      }

//------------------------------------------------------------------------------
// testCase4 ().
//
// Performs error test case 4.
//------------------------------------------------------------------------------

   private void testCase4 ()
      {
      Problem theProblem;

      printHeading (
         4,
         "Fails a C++ EXCEPTION_ASSERT.");

      theProblem = newProblem ();

      try
         {
         theProblem.performAction ("fail-c++-exception-assert");

         throw new AssertionError ();
         }

      catch (InternalErrorException theExc)
         {
         theExc.printStackTrace ();
         }

      System.out.println (
           "\n"
         + "Successful completion of Error Test Case 4.");
      }

//------------------------------------------------------------------------------
// testCase5 ().
//
// Performs error test case 5.
//------------------------------------------------------------------------------

   private void testCase5 ()
      {
      Problem theProblem;

      printHeading (
         5,
         "Fails a C++ ABORTING_ASSERT.");

      theProblem = newProblem ();

      theProblem.performAction ("fail-c++-aborting-assert");

      throw new AssertionError ();
      }

//------------------------------------------------------------------------------
// testCase6 ().
//
// Performs error test case 6.
//------------------------------------------------------------------------------

   private void testCase6 ()
      {
      Problem theProblem;

      printHeading (
         6,
         "Exhausts memory in the WIT-J/Java code.");

      theProblem = newProblem ();

      try
         {
         theProblem.performAction ("exhaust-memory-in-witj-java");

         throw new AssertionError ();
         }

      catch (OutOfMemoryException theExc)
         {
         System.out.println ();

         theExc.printStackTrace ();
         }

      System.out.println (
           "\n"
         + "Successful completion of Error Test Case 6.");
      }

//------------------------------------------------------------------------------
// testCase7 ().
//
// Performs error test case 7.
//------------------------------------------------------------------------------

   private void testCase7 ()
      {
      Problem theProblem;

      printHeading (
         7,
         "Exhausts memory in the WIT-J/C++ code.");

      theProblem = newProblem ();

      try
         {
         theProblem.performAction ("exhaust-memory-in-witj-c++");

         throw new AssertionError ();
         }

      catch (OutOfMemoryException theExc)
         {
         System.out.println ();

         theExc.printStackTrace ();
         }

      System.out.println (
           "\n"
         + "Successful completion of Error Test Case 7.");
      }

//------------------------------------------------------------------------------
// testCase8 ().
//
// Performs error test case 8.
//------------------------------------------------------------------------------

   private void testCase8 ()
      {
      Problem theProblem;

      printHeading (
         8,
         "Exhausts memory in JNI.");

      theProblem = newProblem ();

      try
         {
         theProblem.performAction ("exhaust-memory-in-jni");

         throw new AssertionError ();
         }

      catch (OutOfMemoryException theExc)
         {
         theExc.printStackTrace ();
         }

      System.out.println (
           "\n"
         + "Successful completion of Error Test Case 8.");
      }

//------------------------------------------------------------------------------
// testCase9 ().
//
// Performs error test case 9.
//------------------------------------------------------------------------------

   private void testCase9 ()
      {
      Problem theProblem;

      printHeading (
         9,
         "Triggers a JNI Exception.");

      theProblem = newProblem ();

      try
         {
         theProblem.performAction ("trigger-jni-exception");

         throw new AssertionError ();
         }

      catch (InternalErrorException theExc)
         {
         theExc.printStackTrace ();
         }

      System.out.println (
           "\n"
         + "Successful completion of Error Test Case 9.");
      }

//------------------------------------------------------------------------------
// testCase10 ().
//
// Performs error test case 10.
//------------------------------------------------------------------------------

   private void testCase10 ()
      {
      Problem theProblem;

      printHeading (
         10,
         "Attempts to set N_PERIODS to a negative number.");

      theProblem = newProblem ();

      try
         {
         theProblem.set (N_PERIODS, -3);

         throw new AssertionError ();
         }

      catch (TerminalAppException theExc)
         {
         theExc.printStackTrace ();
         }

      System.out.println (
           "\n"
         + "Successful completion of Error Test Case 10.");
      }

//------------------------------------------------------------------------------
// testCase11 ().
//
// Performs error test case 11.
//------------------------------------------------------------------------------

   private void testCase11 ()
      {
      Problem theProblem;

      printHeading (
         11,
         "Causes WIT to issue a fatal message.");

      theProblem = newProblem ();

      try
         {
         theProblem.performAction ("trigger-wit-fatal-message");

         throw new AssertionError ();
         }

      catch (InternalErrorException theExc)
         {
         theExc.printStackTrace ();
         }

      System.out.println (
           "\n"
         + "Successful completion of Error Test Case 11.");
      }

//------------------------------------------------------------------------------
// testCase12 ().
//
// Performs error test case 12.
//------------------------------------------------------------------------------

   private void testCase12 ()
      {
      Problem theProblem;

      printHeading (
         12,
         "Exhausts memory in WIT.");

      theProblem = newProblem ();

      try
         {
         theProblem.performAction ("exhaust-memory-in-wit");

         throw new AssertionError ();
         }

      catch (OutOfMemoryException theExc)
         {
         System.out.println ();

         theExc.printStackTrace ();
         }

      System.out.println (
           "\n"
         + "Successful completion of Error Test Case 12.");
      }

//------------------------------------------------------------------------------
// testCase13 ().
//
// Performs error test case 13.
//------------------------------------------------------------------------------

   private void testCase13 ()
      {
      Problem theProblem;

      printHeading (
         13,
         "Invokes readData on a file that does not exist.");

      theProblem = newProblem ();

      try
         {
         theProblem.readData ("no-file.data");

         throw new AssertionError ();
         }

      catch (ReadDataException theExc)
         {
         System.out.println ();

         theExc.printStackTrace ();
         }

      System.out.println (
           "\n"
         + "Successful completion of Error Test Case 13.");
      }

//------------------------------------------------------------------------------
// testCase14 ().
//
// Performs error test case 14.
//------------------------------------------------------------------------------

   private void testCase14 ()
      {
      Problem theProblem;

      printHeading (
         14,
         "Invokes readData on a file that sets wit34Compatible to TRUE.");

      theProblem = newProblem ();

      try
         {
         theProblem.readData ("wit34.data");

         throw new AssertionError ();
         }

      catch (ReadDataException theExc)
         {
         System.out.println ();

         theExc.printStackTrace ();
         }

      System.out.println (
           "\n"
         + "Successful completion of Error Test Case 14.");
      }

//------------------------------------------------------------------------------
// testCase15 ().
//
// Performs error test case 15.
//------------------------------------------------------------------------------

   private void testCase15 ()
      {
      Problem theProblem;

      printHeading (
         15,
         "Invokes readData on a file that contains a character in Latin-1.");

      theProblem = newProblem ();

      try
         {
         theProblem.readData ("latin1.data");

         throw new AssertionError ();
         }

      catch (BadCharacterException theExc)
         {
         theExc.printStackTrace ();

         Tester.printlnUTF8 (
              "\n"
            + "The offending String given in the BadCharacterException was: "
            + theExc.getOffendingString ());
         }

      System.out.println (
           "\n"
         + "Successful completion of Error Test Case 15.");
      }

//------------------------------------------------------------------------------
// testCase16 ().
//
// Performs error test case 16.
//------------------------------------------------------------------------------

   private void testCase16 ()
      {
      printHeading (
         16,
         "Invokes WIT-J with a bad native library path.");

      System.setProperty ("java.library.path", "/invalid-library-path");

      try
         {
         newProblem ();

         throw new AssertionError ();
         }

      catch (TerminalAppException theExc)
         {
         theExc.printStackTrace ();
         }

      System.out.println (
           "\n"
         + "Successful completion of Error Test Case 16.");
      }

//------------------------------------------------------------------------------
// testCase17 ().
//
// Performs error test case 17.
//------------------------------------------------------------------------------

   private void testCase17 ()
      {
      Problem theProblem;

      printHeading (
         17,
         "Calls an unimplemented native method.");

      theProblem = newProblem ();

      try
         {
         theProblem.performAction ("call-unimplemented-native-method");

         throw new AssertionError ();
         }

      catch (TerminalAppException theExc)
         {
         theExc.printStackTrace ();
         }

      System.out.println (
           "\n"
         + "Successful completion of Error Test Case 17.");
      }

//------------------------------------------------------------------------------
// newProblem ()
//
// Creates and returns a new Problem set up for error testing.
//------------------------------------------------------------------------------

   private Problem newProblem ()
      {
      Problem theProblem;

      theProblem = Problem.newInstance ();

      theProblem.getMessageMgr ().setMesgFileName ("ErrTester.output");

      return theProblem;
      }

//------------------------------------------------------------------------------
// printHeading (theCaseIdx, text)
//
// Prints a heading for the error test case of theCaseIdx, where text describes
// error test case.
//------------------------------------------------------------------------------

   private void printHeading (int theCaseIdx, String text)
      {
      System.out.printf (
         "\n"
       + "----------------------------------------"
       + "----------------------------------------\n"
       + "WIT-J Error Test Case %d:\n"
       + "%s\n"
       + "----------------------------------------"
       + "----------------------------------------\n"
       + "\n",
         theCaseIdx,
         text);
      }

//==============================================================================
}
