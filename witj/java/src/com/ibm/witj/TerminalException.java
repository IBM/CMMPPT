
//==============================================================================
// WIT-J Java Source File TerminalException.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class TerminalException
//
// One kind of unchecked Exception thrown by WIT-J.
//
// When WIT-J throws a TerminalException, this indicates that:
//
// * As a result of the throw, WIT-J is now in a non-functional state.
//   No further calls to WIT-J methods should be made for the rest of the
//   program process.
//
// Class Hierarchy:
//
// RuntimeException (java.lang.RuntimeException)
//    WitjException
//       TerminalException
//          TerminalAppException
//          ReadDataException
//          BadCharacterException
//          OutOfMemoryException
//          InternalErrorException
//==============================================================================

public abstract class TerminalException extends WitjException
{
//==============================================================================
// Public Methods
//==============================================================================

//------------------------------------------------------------------------------
// Accessors.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// witjHasTerminated ()
//
// Returns true, iff WIT-J has terminated its functioning.
//------------------------------------------------------------------------------

   public static boolean witjHasTerminated ()
      {
      try
         {
         return terminated;
         }

      catch (Throwable theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//==============================================================================
// Package-Internal Constructor
//==============================================================================

   TerminalException (String message)
      {
      super (
            "\n"
       +    "\n"
       +     message
       +    "\n"
       + "   As a result of this Exception, "
       +    "WIT-J has terminated its functioning."
       +    "\n"
       + "   No further calls to WIT-J methods should be made for the rest of\n"
       + "   the program process.\n");

      terminated = true;
      }

//==============================================================================
// Package-Internal Methods
//==============================================================================

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
      if (terminated)
         throw
            new StatusQuoException (
               "   A WIT-J method was called after WIT-J had terminated "
             +    "its functioning.\n"
             + "\n"
             + "      Method: " + methName);
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// terminated
//
// True, iff WIT-J has terminated its functioning.
//------------------------------------------------------------------------------

   private static boolean terminated = false;

//==============================================================================
}
