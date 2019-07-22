
//==============================================================================
// WIT-J Java Source File Utility.java.
//==============================================================================

package com.ibm.witj;

import     java.util.*;

//==============================================================================
// Class Utility
//
// Noninstantiable class.
//
// Contains various static utility methods.
//==============================================================================

final class Utility
{
//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// getExceptionToThrow (theThrowable)
//
// Return a WitjException to be thrown when theThrowable was caught by WIT-J.
//------------------------------------------------------------------------------

   static WitjException getExceptionToThrow (Throwable theThrowable)
      {
      return WitjException.getExceptionToThrow (theThrowable);
      }

//------------------------------------------------------------------------------
// mustBeNonNull
//
// Verifies that an Object argument to a method is non-null.
//
// Arguments:
//    theArg:   The argument being checked.
//    argName:  The name of the argument being checked.
//    methName: The name of the method.
//------------------------------------------------------------------------------

   static void mustBeNonNull (Object theArg, String argName, String methName)
      {
      if (theArg == null)
         throw
            new StatusQuoException (
               "   A method was called with a null argument "
             +    "where this is not allowed.\n"
             + "\n"
             + "      Method:   " + methName + "\n"
             + "      Argument: " + argName);
      }

//------------------------------------------------------------------------------
// notAllowedWhenTerminated (methName)
//
// Verifies that WIT-J has not terminated its functioning.
//
// Arguments:
//    methName: The name of the method.
//------------------------------------------------------------------------------

   static void notAllowedWhenTerminated (String methName)
      {
      TerminalException.notAllowedWhenTerminated (methName);
      }

//------------------------------------------------------------------------------
// notYetAvailable (methName)
//------------------------------------------------------------------------------

   static void notYetAvailable (String methName)
      {
      throw
         new StatusQuoException (
            "   The method called is not yet available in WIT-J.\n"
          + "\n"
          + "      Method: " + methName);
      }

//------------------------------------------------------------------------------
// println (text)
//
// Wrapper method.
//------------------------------------------------------------------------------

   static void println (String text)
      {
      System.out.println (text);
      }

//------------------------------------------------------------------------------
// newHashSet (theStrings)
//
// Creates and returns a HashSet whose elements are theStrings.
//------------------------------------------------------------------------------

   static HashSet <String> newHashSet (String... theStrings)
      {
      HashSet <String> theHashSet;

      theHashSet = new HashSet <String> (theStrings.length * 3);

      for (String theString: theStrings)
         {
         theHashSet.add (theString);
         }

      return theHashSet;
      }

//==============================================================================
// Private Constructor.
//
// Prevents instantiation.
//==============================================================================

   private Utility ()
      {
      }

//==============================================================================
}
