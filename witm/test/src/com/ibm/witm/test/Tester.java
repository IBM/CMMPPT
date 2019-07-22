
////////////////////////////////////////////////////////////////////////////////
// WIT-M/Test Source File Tester.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm.test;

/*******************************************************************************
 * Responsible for performing a test of WIT-M indicated by the command line
 * arguments.<br>
 * Noninstantiatable class.
 *******************************************************************************
 */
public final class Tester
{
////////////////////////////////////////////////////////////////////////////////
// Private Constructors
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * Constructor (Not to be called).
 *******************************************************************************
 */
   private Tester ()
      {
      throw new UnsupportedOperationException ();
      }
   
////////////////////////////////////////////////////////////////////////////////
// Private Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Aborts the program with a message.
 * 
 * @param errorText The error message to be printed.
 *******************************************************************************
 */
   static private void abortWmsg (String errorText)
      {
      System.out.println (
           "\n"
         + "Error:\n"
         + "\n"
         + errorText
         + "\n"
         + "\n"
         + "WIT-MF Test aborted.\n");
   
      System.exit (3);
      }

////////////////////////////////////////////////////////////////////////////////
// Public Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Main program.
 * 
 * @param theArgs<br>
 *        The list of command line arguments.
 *        The first argument indicates the test to be performed;
 *        all subsequent arguments are ignored.
 *******************************************************************************
 */
   public static void main (String[] theArgs)
      {
      if (theArgs.length == 0)
         {
         abortWmsg (
            "   The WIT-M Tester requires at least one command line argument.");
         }

      if      (theArgs[0].equals ("unit-test"))
         {
         UnitTester.performTest ();
         }

      else if (theArgs[0].equals ("error-test"))
         {
         ErrorTester.performTest ();
         }

      else if (theArgs[0].equals ("thread-test"))
         {
         ThreadTester.performTest ();
         }

      else if (theArgs[0].equals ("full-test"))
         {
         UnitTester  .performTest ();
         ErrorTester .performTest ();
         ThreadTester.performTest ();
         }

      else
         {
         abortWmsg (
            "   The first argument to the WIT-M Tester must be "
          +    "one of the following:\n"
          + "\n"
          + "      \"unit-test\"\n"
          + "      \"error-test\"\n"
          + "      \"full-test\"\n"
            );
         }
      }

////////////////////////////////////////////////////////////////////////////////
}
