
////////////////////////////////////////////////////////////////////////////////
// WIT-M/Test Source File ThreadTester.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm.test;

import static com.ibm.witm.Embodiment.currentEmbodiment;
import static com.ibm.witm.Scheme.freezeCurrentScheme;
import static com.ibm.witm.Scheme.currentScheme;
import static com.ibm.witm.Scheme.newScheme;

import com.ibm.witm.Embodiment;
import com.ibm.witm.Scheme;

/*******************************************************************************
 * Responsible for performing a thread test of WIT-M.
 *******************************************************************************
 */
final class ThreadTester
{
////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Fields
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * The first Thread for the Thread test.
 *******************************************************************************
 */
   private final Thread myThread1;
   
/*******************************************************************************
 * The second Thread for the Thread test.
 *******************************************************************************
 */
   private final Thread myThread2;
   
/*******************************************************************************
 * The currently active Thread.
 *******************************************************************************
 */
   private Thread activeThread;
   
/*******************************************************************************
 * The index (1 or 2) of each Thread.
 *******************************************************************************
 */
   private final ThreadLocal <Integer> myThreadIndex;
   
////////////////////////////////////////////////////////////////////////////////
// Private Constructor
////////////////////////////////////////////////////////////////////////////////

   private ThreadTester ()
      {
      myThread1 =
         new Thread ()
            {
                       @Override
            public void run ()
               {
               doThread1Tasks ();
               }
            };

      myThread2 =
         new Thread ()
            {
                       @Override
            public void run ()
               {
               doThread2Tasks ();
               }
            };

      activeThread  = myThread1;

      myThreadIndex = new ThreadLocal <Integer> ();
      }

////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Makes theThread active.
 *******************************************************************************
 */
   private void activate (Thread theThread)
      {
      activeThread = theThread;

      notifyAll ();
      }

/*******************************************************************************
 * Waits until the current Thread is the active Thread.
 *******************************************************************************
 */
   private void waitUntilActive (int waitIdx)
      {
      System.out.printf (
         "Thread #%d: Wait #%d begins.%n",
         myThreadIndex.get (),
         waitIdx);

      while (Thread.currentThread () != activeThread)
         {
         try
            {
            wait ();
            }
         catch (InterruptedException theExc)
            {
            throw new AssertionError (theExc);
            }
         }

      System.out.printf (
         "Thread #%d: Wait #%d ends.%n",
         myThreadIndex.get (),
         waitIdx);
      }

/*******************************************************************************
 * Performs the tasks to be done in the first Thread.
 *******************************************************************************
 */
   private synchronized void doThread1Tasks ()
      {
      Scheme     SCHEME1;
      Embodiment theEmb1;
      
      myThreadIndex.set (1);

      assert (currentScheme () == null);
      
      SCHEME1 = newScheme ("SCHEME1");

      assert (currentScheme () == SCHEME1);
      
      activate (myThread2);
      
      waitUntilActive (1);

      assert (currentScheme () == SCHEME1);

      freezeCurrentScheme ();
      
      assert (currentScheme     () == null);
      assert (currentEmbodiment () == null);

      theEmb1 = SCHEME1.newEmbodiment ("Emb1");

      assert (currentEmbodiment () == theEmb1);
      
      activate (myThread2);
      
      waitUntilActive (2);

      assert (currentEmbodiment () == theEmb1);
      }

/*******************************************************************************
 * Performs the tasks to be done in the second Thread.
 *******************************************************************************
 */
   private synchronized void doThread2Tasks ()
      {
      Scheme     SCHEME2;
      Embodiment theEmb2;

      myThreadIndex.set (2);
      
      waitUntilActive (1);
      
      assert (currentScheme () == null);
      
      SCHEME2 = newScheme ("SCHEME2");

      assert (currentScheme () == SCHEME2);

      activate (myThread1);
      
      waitUntilActive (2);

      assert (currentScheme () == SCHEME2);

      freezeCurrentScheme ();
      
      assert (currentScheme     () == null);
      assert (currentEmbodiment () == null);

      theEmb2 = SCHEME2.newEmbodiment ("Emb2");
      
      assert (currentEmbodiment () == theEmb2);

      activate (myThread1);
      }

/*******************************************************************************
 * Internal implementation of the thread test.
 *******************************************************************************
 */
   private void perfTest ()
      {
      System.out.printf (
         "%n"
       + "----------------------------------------"
       + "----------------------------------------%n"
       + "Thread Test%n"
       + "----------------------------------------"
       + "----------------------------------------%n"
       + "%n");

      myThread1.start ();
      myThread2.start ();

      try
         {
         myThread1.join ();
         myThread2.join ();
         }
      catch (InterruptedException theExc)
         {
         throw new AssertionError (theExc);
         }

      System.out.printf (
         "%n"
       + "----------------------------------------"
       + "----------------------------------------%n"
       + "Successful Completion of Thread Test%n"
       + "----------------------------------------"
       + "----------------------------------------%n"
       + "%n");
      }

////////////////////////////////////////////////////////////////////////////////
// Package Internal Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Performs a unit test of WIT-M.
 *******************************************************************************
 */
   static void performTest ()
      {
      ThreadTester theThreadTester;
         
      theThreadTester = new ThreadTester ();

      theThreadTester.perfTest ();
      }

////////////////////////////////////////////////////////////////////////////////
}
