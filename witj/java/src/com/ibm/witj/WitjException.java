
//==============================================================================
// WIT-J Java Source File WitjException.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class WitjException
//
// An unchecked Exception thrown by WIT-J.
//
// Class Hierarchy:
//
// java.lang.RuntimeException
//    WitjException
//       StatusQuoException
//       TerminalException
//          TerminalAppException
//          ReadDataException
//          BadCharacterException
//          OutOfMemoryException
//          InternalErrorException
//==============================================================================

public abstract class WitjException extends RuntimeException
{
//==============================================================================
// Package-Internal Constructor
//==============================================================================

   WitjException (String message)
      {
      super (message);
      }

//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// getExceptionToThrow (theThrowable)
//
// Returns a WitjException to be thrown when theThrowable was caught by WIT-J.
//------------------------------------------------------------------------------

   static WitjException getExceptionToThrow (Throwable theThrowable)
      {
      try
         {
         throw theThrowable;
         }


      catch (WitjException theWitjExc)
         {
         return theWitjExc;
         }

      catch (OutOfMemoryError theMemoryErr)
         {
         return new OutOfMemoryException (theMemoryErr);
         }

      catch (AssertionError theAssertErr)
         {
         return
            InternalErrorException.
               newInstanceForJavaAssertionError (theAssertErr);
         }

      catch (UnsatisfiedLinkError theLinkErr)
         {
         return
            TerminalAppException.
               newInstanceForUnsatisfiedLink (theLinkErr);
         }

      catch (Throwable unexpThrowable)
         {
         return
            InternalErrorException.
               newInstanceForUnexpJavaThrowable (unexpThrowable);
         }
      }

//==============================================================================
// Private methods
//==============================================================================

//------------------------------------------------------------------------------
// mentionExceptionsForCpp ()
//
// This method is never called.
// The purpose of this method is to force the Exception subclasses that it
// mentions to be compiled, so that the Exceptions that they define will be
// available to be constructed and thrown from C++.
//------------------------------------------------------------------------------

   private void mentionExceptionsForCpp ()
      {
      ReadDataException     theReadDataExc;
      BadCharacterException theBadCharacterExc;
      }

//==============================================================================
}
