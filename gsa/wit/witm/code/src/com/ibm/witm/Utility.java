
////////////////////////////////////////////////////////////////////////////////
// WIT-M Source File Utility.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm;

/*******************************************************************************
 * Contains various static utility methods.
 * Noninstantiatable class.
 *******************************************************************************
 */
final class Utility
{
////////////////////////////////////////////////////////////////////////////////
// Private Constructor
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * (Private Constructor (Not to be called).
 *******************************************************************************
 */
   private Utility ()
      {
      throw new AssertionError ();
      }

////////////////////////////////////////////////////////////////////////////////
// Package-Internal Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Verifies that an argument to a method is non-null.
 * 
 * @param  theArg  The argument being checked.
 * @param  argName The name of the argument being checked.
 *******************************************************************************
 */
   static void banNull (Object theArg, String argName)
      {
      if (theArg == null)
         throw
            new NullPointerException (
               "\n\n"
             + "   The following argument was a prohibited null pointer:\n"
             + "\n"
             + "      " + argName  + "\n");
      }

/*******************************************************************************
 * Verifies that array-of-references argument is not null and its references are
 * not null.
 * 
 * @param  theArr  The array argument being checked.
 * @param  argName The name of the argument being checked.
 *******************************************************************************
 */
   static void banNullArray (Object[] theArr, String argName)
      {
      int theIdx;
      
      banNull (theArr, argName);
      
      for (theIdx = 0; theIdx < theArr.length; theIdx ++)
         if (theArr[theIdx] == null)
            banNull (null, argName + "[" + theIdx + "]");
      }

/*******************************************************************************
 * Returns the current Embodiment, if any; otherwise null.
 *******************************************************************************
 */
   static Embodiment curEmb ()
      {
      return Embodiment.currentEmbodiment ();
      }

/*******************************************************************************
 * Verifies that the current Embodiment is not null.
 *******************************************************************************
 */
   static void reqCurEmb ()
      {
      if (curEmb () == null)
         {
         throw
            new IllegalStateException (
               "\n\n"
             + "   Call to a method that requires a current Embodiment\n"
             + "   when the current Embodiment is null.\n");
         }
      }

/*******************************************************************************
 * Efficiently builds and returns the concatenation of the Strings in
 * theStrings.
 *******************************************************************************
 */
   static String concatenation (String... theStrings)
      {
      int           totalLength;
      StringBuilder theBuilder;
      
      totalLength = 0;
      
      for (String theString: theStrings)
         totalLength += theString.length ();
      
      theBuilder = new StringBuilder (totalLength);
      
      for (String theString: theStrings)
         theBuilder.append (theString);      
      
      return theBuilder.toString ();
      }

////////////////////////////////////////////////////////////////////////////////
}
