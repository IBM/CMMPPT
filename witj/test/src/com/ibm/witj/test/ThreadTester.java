
//==============================================================================
// WIT-J/Test Source File ThreadTester.java.
//==============================================================================

package       com.ibm.witj.test;

import        com.ibm.witj.*;
import static com.ibm.witj.Attribute.*;
import static com.ibm.witj.MessageGroup.*;

//==============================================================================
// Class ThreadTester
//
// Responsible for performing a test of WIT-J's thread safety capabilities.
//==============================================================================

final class ThreadTester
{
//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// performTest ().
//
// Performs a complete WIT-J thread safety test.
//------------------------------------------------------------------------------

   static void performTest ()
      {
      int theCaseIdx;

      for (theCaseIdx = 1; theCaseIdx <= 4; theCaseIdx ++)
         {
         new ThreadTester (theCaseIdx).performCase ();
         }
      }

//==============================================================================
// Private Constructor
//==============================================================================

   private ThreadTester (int theCaseIdx)
      {
      myCaseIndex  = theCaseIdx;

      secondThread =
         new Thread ()
            {
            public void run ()
               {
               continueCase ();
               }
            };
      }

//==============================================================================
// Private Methods
//==============================================================================

//------------------------------------------------------------------------------
// performCase ().
//
// Performs the selected thread safety test case.
//------------------------------------------------------------------------------

   private void performCase ()
      {
      switch (myCaseIndex)
         {
         case 1:
            {
            performCase1 ();
            break;
            }

         case 2:
            {
            performCase2 ();
            break;
            }

         case 3:
            {
            performCase3 ();
            break;
            }

         case 4:
            {
            performCase4 ();
            break;
            }

         default:
            {
            throw new AssertionError ();
            }
         }
      }

//------------------------------------------------------------------------------
// continueCase ()
//
// Performs those aspects of the selected thread safety test case to be done in
// a second Thread.
//------------------------------------------------------------------------------

   private void continueCase ()
      {
      switch (myCaseIndex)
         {
         case 1:
            {
            continueCase1 ();
            break;
            }

         case 2:
            {
            continueCase2 ();
            break;
            }

         case 3:
            {
            continueCase3 ();
            break;
            }

         case 4:
            {
            continueCase4 ();
            break;
            }

         default:
            {
            throw new AssertionError ();
            }
         }
      }

//------------------------------------------------------------------------------
// performCase1 ().
//
// Performs thread safety test case 1.
//------------------------------------------------------------------------------

   private void performCase1 ()
      {
      printHeading (
         "   Invokes a method of a ThreadedObject from outside the\n"
       + "   ThreadedObject's associated Thread.");

      myProblem   = Problem  .newInstance ();
      myOperation = Operation.newInstance (myProblem, "A");

      myProblem.getMessageMgr ().flushFile ();

      System.out.printf ("\n\n");

      secondThread.start ();

      joinSecondThread ();

      myProblem.deactivate ();

      printClosing ();
      }

//------------------------------------------------------------------------------
// continueCase1 ().
//
// Performs those aspects of thread safety test case 1 to be done in a second
// Thread.
//------------------------------------------------------------------------------

   private void continueCase1 ()
      {
      try
         {
         myOperation.get (EXEC_VOL);

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }
      }

//------------------------------------------------------------------------------
// performCase2 ().
//
// Performs thread safety test case 2.
//------------------------------------------------------------------------------

   private void performCase2 ()
      {
      printHeading (
         "   Passes a ThreadedObject as an argument to a method from outside\n"
       + "   the ThreadedObject's associated Thread.");

      myProblem = Problem.newInstance ();

      System.out.printf ("\n");

      secondThread.start ();

      joinSecondThread ();

      myProblem.deactivate ();

      printClosing ();
      }

//------------------------------------------------------------------------------
// continueCase2 ().
//
// Performs those aspects of thread safety test case 2 to be done in a second
// Thread.
//------------------------------------------------------------------------------

   private void continueCase2 ()
      {
      try
         {
         Operation.newInstance (myProblem, "A");

         throw new AssertionError ();
         }

      catch (StatusQuoException theExc)
         {
         theExc.printStackTrace ();
         }
      }

//------------------------------------------------------------------------------
// performCase3 ().
//
// Performs thread safety test case 3.
//------------------------------------------------------------------------------

   private void performCase3 ()
      {
      Problem theProblem;

      printHeading (
         "   Invokes readData in two threads at the same time.");

      theProblem = Problem.newInstance ();

      secondThread.start ();

      synchronized (this)
         {
         while (! t2IsReady)
            waitForNotify ();
         }

      theProblem.readData ("thread1.data");

      theProblem.getMessageMgr ().flushFile ();

      theProblem.deactivate ();

      joinSecondThread ();

      printClosing ();
      }

//------------------------------------------------------------------------------
// continueCase3 ().
//
// Performs those aspects of thread safety test case 3 to be done in a second
// Thread.
//------------------------------------------------------------------------------

   private void continueCase3 ()
      {
      Problem theProblem;

      theProblem = Problem.newInstance ();

      synchronized (this)
         {
         t2IsReady = true;

         notifyAll ();
         }

      theProblem.readData ("thread2.data");

      theProblem.getMessageMgr ().flushFile ();

      theProblem.deactivate ();
      }

//------------------------------------------------------------------------------
// performCase4 ().
//
// Performs thread safety test case 4.
//------------------------------------------------------------------------------

   private void performCase4 ()
      {
      Problem theProblem;

      printHeading (
         "   Runs optimizing implosion in two threads at the same time.");

      secondThread.start ();

      theProblem = Problem.newInstance ();

      theProblem.getMessageMgr ().setMesgTimesPrint (INFORMATIONAL, 0);

      theProblem.readData ("diner-utf8.data");

      theProblem.getMessageMgr ().setMesgTimesPrint (INFORMATIONAL, -1);

      synchronized (this)
         {
         t1IsReady = true;

         notifyAll ();

         while (! t2IsReady)
            waitForNotify ();
         }

      theProblem.optImplode ();

      theProblem.deactivate  ();

      joinSecondThread ();

      printClosing ();
      }

//------------------------------------------------------------------------------
// continueCase4 ().
//
// Performs those aspects of thread safety test case 4 to be done in a second
// Thread.
//------------------------------------------------------------------------------

   private void continueCase4 ()
      {
      Problem theProblem;

      theProblem = Problem.newInstance ();

      theProblem.getMessageMgr ().setMesgTimesPrint (INFORMATIONAL, 0);

      theProblem.readData ("diner-utf8.data");

      theProblem.getMessageMgr ().setMesgTimesPrint (INFORMATIONAL, -1);

      synchronized (this)
         {
         t2IsReady = true;

         notifyAll ();

         while (! t1IsReady)
            waitForNotify ();
         }

      theProblem.optImplode ();

      theProblem.deactivate ();
      }

//------------------------------------------------------------------------------
// joinSecondThread ()
//
// Waits for the second Thread to complete.
//------------------------------------------------------------------------------

   private void joinSecondThread ()
      {
      try
         {
         secondThread.join ();
         }

      catch (InterruptedException theExc)
         {
         theExc.printStackTrace ();

         throw new AssertionError ();
         }
      }

//------------------------------------------------------------------------------
// waitForNotify ()
// 
// Calls wait () in the required try block.
//------------------------------------------------------------------------------

   private void waitForNotify ()
      {
      try
         {
         wait ();
         }

      catch (InterruptedException theExc)
         {
         throw new AssertionError ();
         }
      }

//------------------------------------------------------------------------------
// printHeading (text)
//
// Prints a heading for the selected thread safety test case, where text
// describes the case.
//------------------------------------------------------------------------------

   private void printHeading (String text)
      {
      System.out.printf (
         "\n"
       + "----------------------------------------"
       + "----------------------------------------\n"
       + "WIT-J Thread safety Test Case %d:\n"
       + "%s\n"
       + "----------------------------------------"
       + "----------------------------------------\n",
         myCaseIndex,
         text);
      }

//------------------------------------------------------------------------------
// printClosing ()
//
// Prints a closing for the selected thread safety test case.
//------------------------------------------------------------------------------

   private void printClosing ()
      {
      System.out.printf (
         "\n"
       + "Successful completion of Thread safety Test Case %d.\n",
         myCaseIndex);
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// myCaseIndex
//
// The index of the test case to be perfomed.
//------------------------------------------------------------------------------

   private final int myCaseIndex;

//------------------------------------------------------------------------------
// secondThread
//
// The second Thread to be used in any test case.
// This field is set to an instance of an anonymous inner class whose run method
// simply invokes continueCase ().
//------------------------------------------------------------------------------

   private final Thread secondThread;

//------------------------------------------------------------------------------
// t1IsReady
// t2IsReady
//
// Conditions used for wait/notify.
//------------------------------------------------------------------------------

   private boolean t1IsReady = false;
   private boolean t2IsReady = false;

//------------------------------------------------------------------------------
// Data used by various test cases.
//------------------------------------------------------------------------------

   private Problem   myProblem   = null;
   private Operation myOperation = null;

//==============================================================================
}
