
//------------------------------------------------------------------------------
// Program:    ThreadTester.
// Java Class: ThreadTester
//
// This is the main Java class for program ThreadTester.
//
// The purpose of program ThreadTester is to test WIT's thread safety features.
// A run of program ThreadTester proceeds as follows:
//
// * The command line argument is parsed.
//   * There must be exactly one argument.
//   * Its value must be a string that represents an int in the range [1, 7].
//   * There are 7 cases that can be tested.
//   * Each case is indicated by a case index, an int in the range [1, 7].
//   * The argument is interpreted as the index (from 1 to 7) of the case to be
//     tested.
//
// * A second Thread is created.
//
// * A method is called, that runs the first-Thread portion of the indicated
//   test case.
//   * This method is called thread1Case{N}, where {N} is the case index.
//
// * At some point, the test method launches the second Thread.
//
// * The second thread calls a method that runs the second-Thread portion of the
//   indicated test case.
//   * This method is called thread2Case{N}, where {N} is the case index.
//
// * All calls to WIT are done by C++ implementations of native methods of this
//   class.
//------------------------------------------------------------------------------

class ThreadTester implements Runnable
   {
   //---------------------------------------------------------------------------

   public static void main (String[] args)
      {
      int          theCaseIdx;
      ThreadTester theTester;

      if (args.length != 1)
         {
         System.out.println (
              "ERROR:\n"
            + "   ThreadTester must be invoked with 1 argument."
            );

         return;
         }

      theCaseIdx = Integer.parseInt (args[0]);

      theTester  = new ThreadTester (theCaseIdx);

      theTester.testThreads ();
      }

   //---------------------------------------------------------------------------

   public void run ()
      {
      switch (myCaseIdx)
         {
         case 1: thread2Case1 (); break;
         case 2: thread2Case2 (); break;
         case 3: thread2Case3 (); break;
         case 4: thread2Case4 (); break;
         case 5: thread2Case5 (); break;
         case 6: thread2Case6 (); break;
         case 7: thread2Case7 (); break;

         default:
            System.out.println ("Internal error in thread-test.");
         }
      }

   //---------------------------------------------------------------------------

   private ThreadTester (int theCaseIdx)
      {
      myCaseIdx = theCaseIdx;

      myThread2 = new Thread (this);

      System.loadLibrary ("ThreadTester");
      }

   //---------------------------------------------------------------------------

   private void testThreads ()
      {
      switch (myCaseIdx)
         {
         case 1: thread1Case1 (); break;
         case 2: thread1Case2 (); break;
         case 3: thread1Case3 (); break;
         case 4: thread1Case4 (); break;
         case 5: thread1Case5 (); break;
         case 6: thread1Case6 (); break;
         case 7: thread1Case7 (); break;

         default:
            System.out.println (
               "ERROR:\n"
             + "   The argument to ThreadTester must be "
             + "   an integer from 1 to 7.\n"
             + "   The argument passed was \""
             + myCaseIdx
             + "\".");
         }
      }

   //---------------------------------------------------------------------------
   // Test Case #1
   //---------------------------------------------------------------------------

   private void thread1Case1 ()
      {
      System.out.println (
           "\n\n\n\n\n"
         + "Case 1:\n"
         + "Testing multi-threaded calls to a single WitRun.");

      newRunA         ();
      initializeA     ();
      readRch818A     ();

      myThread2.start ();

      optImplodeA     ();
      deleteRunA      ();
      }

   private void thread2Case1 ()
      {
      heurImplodeA    ();
      }

   //---------------------------------------------------------------------------
   // Test Case #2
   //---------------------------------------------------------------------------

   private void thread1Case2 ()
      {
      System.out.println (
           "\n\n\n\n\n"
         + "Case 2:\n"
         + "Testing a multi-threaded call to witCopyData.");

      newRunA         ();
      initializeA     ();
      readRch818A     ();
      newRunB         ();
      initializeB     ();

      myThread2.start ();

      heurImplodeA    ();
      }

   private void thread2Case2 ()
      {
      copyDataBA  ();
      }

   //---------------------------------------------------------------------------
   // Test Case #3
   //---------------------------------------------------------------------------

   private void thread1Case3 ()
      {
      System.out.println (
           "\n\n\n\n\n"
         + "Case 3:\n"
         + "Testing a reverse multi-threaded call to witCopyData.");

      newRunA         ();
      initializeA     ();
      readRch818A     ();
      newRunB         ();
      initializeB     ();

      myThread2.start ();

      copyDataBA      ();
      copyDataBA      ();
      }

   private void thread2Case3 ()
      {
      heurImplodeA    ();
      }

   //---------------------------------------------------------------------------
   // Test Case #4
   //---------------------------------------------------------------------------

   private void thread1Case4 ()
      {
      System.out.println (
           "\n\n\n\n\n"
         + "Case 4:\n"
         + "Testing a multi-threaded call to witReadData.");

      newRunA         ();
      initializeA     ();
      newRunB         ();
      initializeB     ();

      myThread2.start ();

      readEastmanA    ();
      }

   private void thread2Case4 ()
      {
      readRch818B ();
      }

   //---------------------------------------------------------------------------
   // Test Case #5
   //---------------------------------------------------------------------------

   private void thread1Case5 ()
      {
      System.out.println (
           "\n\n\n\n\n"
         + "Case 5:\n"
         + "Testing a multi-threaded call to witCopyPartData.");

      newRunA         ();
      initializeA     ();
      newRunB         ();
      initializeB     ();
      readRch818A     ();

      myThread2.start ();

      heurImplodeA    ();
      }

   private void thread2Case5 ()
      {
      copyPartDataBA  ();
      }

   //---------------------------------------------------------------------------
   // Test Case #6
   //---------------------------------------------------------------------------

   private void thread1Case6 ()
      {
      System.out.println (
           "\n\n\n\n\n"
         + "Case 6:\n"
         + "Testing a multi-threaded call to witExecStandAlone.");

      newRunA         ();
      initializeA     ();
      newRunB         ();
      initializeB     ();

      myThread2.start ();

      standAloneA     ();
      }

   private void thread2Case6 ()
      {
      standAloneB     ();
      }

   //---------------------------------------------------------------------------
   // Test Case #7
   //---------------------------------------------------------------------------

   private void thread1Case7 ()
      {
      System.out.println (
           "\n\n\n\n\n"
         + "Case 7:\n"
         + "Testing multi-threaded calls to witOptImplode.\n"
         + "(This is an non-error case.)");

      newRunA         ();
      initializeA     ();
      readEastmanA    ();

      newRunB         ();
      initializeB     ();
      readRch818B     ();

      myThread2.start ();

      optImplodeB     ();
      deleteRunB      ();
      }

   private void thread2Case7 ()
      {
      optImplodeA     ();
      deleteRunA      ();
      }

   //---------------------------------------------------------------------------
   // Native methods.
   //---------------------------------------------------------------------------

   private native void newRunA        ();
   private native void initializeA    ();
   private native void readRch818A    ();
   private native void readEastmanA   ();
   private native void heurImplodeA   ();
   private native void optImplodeA    ();
   private native void standAloneA    ();
   private native void deleteRunA     ();

   private native void newRunB        ();
   private native void initializeB    ();
   private native void copyDataBA     ();
   private native void copyPartDataBA ();
   private native void readRch818B    ();
   private native void optImplodeB    ();
   private native void standAloneB    ();
   private native void deleteRunB     ();

   //---------------------------------------------------------------------------

   private final int myCaseIdx;
      //
      // The index of the test case to be run.
   
   private final Thread myThread2;
      //
      // The second Thread.
   }
