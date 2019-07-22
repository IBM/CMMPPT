
//==============================================================================
// WIT-J/Test Source File Tester.java.
//==============================================================================

package com.ibm.witj.test;

import java.io.*;

//==============================================================================
// Class Tester
//
// Noninstantiable class.
//
// Responsible for performing a test of WIT-J indicated by the command line
// arguments.
//==============================================================================

public final class Tester
{
//==============================================================================
// Public Methods
//==============================================================================

//------------------------------------------------------------------------------
// Main Program.
//------------------------------------------------------------------------------

   public static void main (String[] theArgs)
      {
      if (theArgs.length == 0)
         {
         abortWmsg (
            "   The WIT-J Tester requires at least one command line argument.");
         }

      if      (theArgs[0].equals ("unit-test"))
         {
         UnitTester.performTest ();
         }
      else if (theArgs[0].equals ("status-quo-test"))
         {
         StatusQuoTester.performTest ();
         }
      else if (theArgs[0].equals ("error-test"))
         {
         ErrTester.performTest (theArgs);
         }
      else if (theArgs[0].equals ("thread-test"))
         {
         ThreadTester.performTest ();
         }
      else if (theArgs[0].equals ("timing-test"))
         {
         TimingTester.performTest (theArgs);
         }
      else
         {
         abortWmsg (
            "   The first argument to the WIT-J Tester must be "
          +    "one of the following:\n"
          + "      \"unit-test\"\n"
          + "      \"status-quo-test\"\n"
          + "      \"error-test\"\n"
          + "      \"thread-test\"\n"
          + "      \"timing-test\"\n"
            );
         }
      }

//==============================================================================
// Package internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// abortWmsg (errorText)
//
// Aborts the program with a message that includes errorText.
//------------------------------------------------------------------------------

   static void abortWmsg (String errorText)
      {
      System.out.println (
           "\n"
         + "Error:\n"
         + "\n"
         + errorText
         + "\n"
         + "\n"
         + "WIT-J Test aborted.\n");
   
      System.exit (3);
      }

//------------------------------------------------------------------------------
// printlnUTF8 (String theString)
//
// Prints theString to the standard output stream, followed by a line break.
// Uses UTF-8 encoding.
//------------------------------------------------------------------------------

   static void printlnUTF8 (String theString)
      {
      FileOutputStream   stdout;
      OutputStreamWriter stdoutUTF8;

      try
         {
         stdout     = new FileOutputStream   (FileDescriptor.out);

         stdoutUTF8 = new OutputStreamWriter (stdout, "UTF-8");

         stdoutUTF8.write (theString + "\n");

         stdoutUTF8.flush ();
         }

      catch (IOException theExc)
         {
         theExc.printStackTrace ();

         System.exit (4);
         }
      }

//==============================================================================
// Private Constructor
//
// Prevents instantiation.
//==============================================================================

   private Tester ()
      {
      }

//==============================================================================
}
